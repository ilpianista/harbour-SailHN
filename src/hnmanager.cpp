/*
  The MIT License (MIT)

  Copyright (c) 2016-2021 Andrea Scarpino <andrea@scarpino.dev>

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
*/

#include "hnmanager.h"

#include <memory>
#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <QNetworkAccessManager>
#include <QNetworkCookie>
#include <QNetworkCookieJar>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QSettings>
#include <QStandardPaths>
#include <QUrlQuery>

#include "hackernewsapi.h"
#include "securesecrets.h"

const static QString BASE_URL = QStringLiteral("https://news.ycombinator.com");

HNManager::HNManager(QObject *parent)
    : QObject(parent)
    , api(new HackerNewsAPI(this))
    , network(new QNetworkAccessManager(this))
    , m_loggedUser(nullptr)
{
    const QString settingsPath = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation)
                                 + QDir::separator() + QCoreApplication::applicationName()
                                 + ".conf";
    m_settings = new QSettings(settingsPath, QSettings::NativeFormat, this);

    if (!m_settings->contains("migrated")) {
        QSettings oldSettings(QCoreApplication::applicationName(),
                              QCoreApplication::applicationName(),
                              this);

        for (const QString &key : oldSettings.childKeys())
            m_settings->setValue(key, oldSettings.value(key));

        oldSettings.clear();

        m_settings->setValue("migrated", "true");
    }

    setUsername(m_settings->value("Username").toString());

    if (!getUsername().isEmpty()) {
        auto conn = std::make_shared<QMetaObject::Connection>();
        *conn = connect(api, &HackerNewsAPI::userFetched, this, [this, conn](User *user) {
            disconnect(*conn);
            onLoggedUserFetched(user);
        });
        api->getUser(getUsername());
    }

    // Use SecureSecrets for cookie storage
    m_secureStorage = new SecureSecrets(this);
    connect(m_secureStorage, &SecureSecrets::initialized, this, [this]() {
        // Load cookies from secure storage
        QNetworkCookieJar *cookieJar = network->cookieJar();
        for (const QNetworkCookie &cookie : m_secureStorage->loadCookies()) {
            cookieJar->insertCookie(cookie);
        }
    });
    m_secureStorage->initialize();
}

HNManager::~HNManager()
{
    if (m_loggedUser && !m_loggedUser->parent()) {
        delete m_loggedUser;
    }
}

void HNManager::authenticate(const QString &username, const QString &password)
{
    qDebug() << "Log in with username" << username;
    setUsername(username);

    QNetworkRequest req(QUrl(BASE_URL + QLatin1String("/login")));
    req.setHeader(QNetworkRequest::ContentTypeHeader,
                  QLatin1String("application/x-www-form-urlencoded"));

    // QUrlQuery doesn't convert "+" to "&2B" so we must perform this manually
    // See https://doc.qt.io/qt-5/qurlquery.html#handling-of-spaces-and-plus
    QUrlQuery data;
    data.addQueryItem(QLatin1String("acct"), QString(username).replace("+", "%2B"));
    data.addQueryItem(QLatin1String("pw"), QString(password).replace("+", "%2B"));

    QNetworkReply *reply = network->post(req, data.toString(QUrl::FullyEncoded).toUtf8());

    connect(reply, &QNetworkReply::finished, this, &HNManager::onAuthenticateResult);
}

void HNManager::onAuthenticateResult()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(QObject::sender());
    if (!reply) {
        Q_EMIT authenticated(false);
        return;
    }

    bool logged = false;
    if (reply->error() != QNetworkReply::NoError) {
        qCritical() << "Cannot perform login" << reply->errorString();
    } else {
        if (!reply->readAll().contains("Bad login.")) {
            logged = true;

            QList<QNetworkCookie> cookies = network->cookieJar()->cookiesForUrl(
                QUrl(BASE_URL + "/"));
            m_secureStorage->storeCookies(cookies);

            // Use a single-shot connection to avoid duplicates
            auto conn = std::make_shared<QMetaObject::Connection>();
            *conn = connect(api, &HackerNewsAPI::userFetched, this, [this, conn](User *user) {
                disconnect(*conn);
                onLoggedUserFetched(user);
            });
            api->getUser(m_loggedUsername);
        }
    }

    Q_EMIT authenticated(logged);

    reply->deleteLater();
}

void HNManager::onLoggedUserFetched(User *user)
{
    if (m_loggedUser) {
        m_loggedUser->deleteLater();
    }

    m_loggedUser = user;
    m_loggedUser->setParent(this);

    Q_EMIT loggedUserFetched(user);
}

bool HNManager::isAuthenticated() const
{
    return m_loggedUser != nullptr;
}

void HNManager::setUsername(const QString &username)
{
    m_loggedUsername = username;
    m_settings->setValue("Username", username);
}

QString HNManager::getUsername() const
{
    return m_loggedUsername;
}

User *HNManager::loggedUser()
{
    return m_loggedUser;
}

void HNManager::logout()
{
    setUsername(QString());

    if (m_loggedUser) {
        m_loggedUser->deleteLater();
        m_loggedUser = nullptr;
    }

    QNetworkCookieJar *cookieJar = network->cookieJar();
    for (const QNetworkCookie &cookie : cookieJar->cookiesForUrl(QUrl(BASE_URL + "/"))) {
        cookieJar->deleteCookie(cookie);
    }

    m_secureStorage->clearCookies();
    m_settings->setValue("Username", QString());
}

void HNManager::submit(const QString &title, const QString &url, const QString &text)
{
    qDebug() << "Submit item with title" << title;

    getSubmitCSRF([this, title, url, text](const QString &csrf) {
        if (csrf.isEmpty()) {
            qCritical() << "Failed to obtain submit CSRF token";
            Q_EMIT submitted(false);
            return;
        }

        QNetworkRequest req(QUrl(BASE_URL + QLatin1String("/r")));
        req.setHeader(QNetworkRequest::ContentTypeHeader,
                      QLatin1String("application/x-www-form-urlencoded"));

        QUrlQuery data;
        data.addQueryItem(QLatin1String("title"), title);
        data.addQueryItem(QLatin1String("url"), url);
        data.addQueryItem(QLatin1String("text"), text);
        data.addQueryItem(QLatin1String("fnop"), QLatin1String("submit-page"));
        data.addQueryItem(QLatin1String("fnid"), csrf);

        QNetworkReply *reply = network->post(req, data.toString(QUrl::FullyEncoded).toUtf8());

        connect(reply, &QNetworkReply::finished, this, &HNManager::onSubmitResult);
    });
}

void HNManager::comment(const int parentId, const QString &text)
{
    qDebug() << "Comment item with id" << parentId;

    getCommentCSRF(parentId, [this, parentId, text](const QString &csrf) {
        if (csrf.isEmpty()) {
            qCritical() << "Failed to obtain comment CSRF token";
            Q_EMIT commented(false);
            return;
        }

        QNetworkRequest req(QUrl(BASE_URL + QLatin1String("/comment")));
        req.setHeader(QNetworkRequest::ContentTypeHeader,
                      QLatin1String("application/x-www-form-urlencoded"));

        QUrlQuery data;
        data.addQueryItem(QLatin1String("parent"), QString::number(parentId));
        data.addQueryItem(QLatin1String("goto"), QStringLiteral("item?id=%1").arg(parentId));
        data.addQueryItem(QLatin1String("text"), text);
        data.addQueryItem(QLatin1String("hmac"), csrf);

        QNetworkReply *reply = network->post(req, data.toString(QUrl::FullyEncoded).toUtf8());

        connect(reply, &QNetworkReply::finished, this, &HNManager::onCommentResult);
    });
}

void HNManager::onSubmitResult()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(QObject::sender());
    if (!reply) {
        Q_EMIT submitted(false);
        return;
    }

    bool res = false;

    if (reply->error() != QNetworkReply::NoError) {
        qCritical() << "Cannot submit item" << reply->errorString();
    } else {
        if (!reply->readAll().contains("Unknown or expired link.")) {
            res = true;
        }
    }

    Q_EMIT submitted(res);

    reply->deleteLater();
}

void HNManager::onCommentResult()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(QObject::sender());
    if (!reply) {
        Q_EMIT commented(false);
        return;
    }

    bool res = false;

    if (reply->error() != QNetworkReply::NoError) {
        qCritical() << "Cannot send comment" << reply->errorString();
    } else {
        if (!reply->readAll().contains("Please confirm that this is your comment "
                                       "by submitting it one more time.")) {
            res = true;
        }
    }

    Q_EMIT commented(res);

    reply->deleteLater();
}

void HNManager::getSubmitCSRF(std::function<void(const QString &)> callback)
{
    QNetworkRequest req(QUrl(BASE_URL + QLatin1String("/submit")));
    QNetworkReply *reply = network->get(req);

    connect(reply, &QNetworkReply::finished, this, [this, reply, callback]() {
        const QRegularExpression regexp("<input type=\"hidden\" name=\"fnid\" value=\"([^\"]+)\">");
        callback(getCSRF(reply, regexp));
    });
}

void HNManager::getCommentCSRF(const int itemId, std::function<void(const QString &)> callback)
{
    QUrl url(BASE_URL + QLatin1String("/item"));

    QUrlQuery query;
    query.addQueryItem(QLatin1String("id"), QString::number(itemId));
    url.setQuery(query);

    QNetworkRequest req(url);
    QNetworkReply *reply = network->get(req);

    connect(reply, &QNetworkReply::finished, this, [this, reply, callback]() {
        const QRegularExpression regexp("<input type=\"hidden\" name=\"hmac\" value=\"([^\"]+)\">");
        callback(getCSRF(reply, regexp));
    });
}

QString HNManager::getCSRF(QNetworkReply *reply, const QRegularExpression &regexp) const
{
    QString csrf;

    if (reply->error() != QNetworkReply::NoError) {
        qCritical() << "CSRF request failed:" << reply->errorString();
        reply->deleteLater();
        return csrf;
    }

    QByteArray data = reply->readAll();
    QTextStream stream(data, QIODevice::ReadOnly);

    QString line;
    while (!stream.atEnd()) {
        line = stream.readLine();

        QRegularExpressionMatch match = regexp.match(line);
        if (match.hasMatch()) {
            csrf = match.captured(1);
        }
    }

    reply->deleteLater();

    return csrf;
}
