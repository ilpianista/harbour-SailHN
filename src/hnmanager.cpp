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

#include <QCoreApplication>
#include <QDebug>
#include <QEventLoop>
#include <QNetworkAccessManager>
#include <QNetworkCookie>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QSettings>
#include <QUrlQuery>

#include "cookiejar.h"
#include "hackernewsapi.h"

const static QString BASE_URL = QStringLiteral("https://news.ycombinator.com");

HNManager::HNManager(QObject *parent) :
    QObject(parent)
  , api(new HackerNewsAPI(this))
  , network(new QNetworkAccessManager(this))
  , m_loggedUser(0)
{
    m_settings = new QSettings(QCoreApplication::applicationName(), QCoreApplication::applicationName(), this);
    setUsername(m_settings->value("Username").toString());

    network->setCookieJar(new CookieJar(this));

    if (!network->cookieJar()->cookiesForUrl(QUrl(BASE_URL + "/")).isEmpty()) {
        api->getUser(getUsername());

        connect(api, &HackerNewsAPI::userFetched, this, &HNManager::onLoggedUserFetched);
    }
}

HNManager::~HNManager()
{
    delete api;
    delete network;
    delete m_loggedUser;
    delete m_settings;
}

void HNManager::authenticate(const QString &username, const QString &password)
{
    qDebug() << "Log in with username" << username;
    setUsername(username);

    QNetworkRequest req(QUrl(BASE_URL + QLatin1String("/login")));
    req.setHeader(QNetworkRequest::ContentTypeHeader, QLatin1String("application/x-www-form-urlencoded"));

    QUrlQuery data;
    data.addQueryItem(QLatin1String("acct"), username);
    data.addQueryItem(QLatin1String("pw"), password);

    QNetworkReply* reply = network->post(req, data.toString(QUrl::FullyEncoded).toUtf8());

    connect(reply, &QNetworkReply::finished, this, &HNManager::onAuthenticateResult);
}

void HNManager::onAuthenticateResult()
{
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(QObject::sender());

    bool logged = false;
    if (reply->error() != QNetworkReply::NoError) {
        qCritical() << "Cannot perform login" << reply->errorString();
    } else {
        if (!reply->readAll().contains("Bad login.")) {
            logged = true;
            api->getUser(m_loggedUsername);

            connect(api, &HackerNewsAPI::userFetched, this, &HNManager::onLoggedUserFetched);
        }
    }

    Q_EMIT authenticated(logged);

    reply->deleteLater();
}

void HNManager::onLoggedUserFetched(User *user)
{
    disconnect(api, &HackerNewsAPI::userFetched, this, &HNManager::onLoggedUserFetched);

    m_loggedUser = user;

    Q_EMIT loggedUserFetched(user);
}

bool HNManager::isAuthenticated() const
{
    //qDebug() << "Is authenticated as:" << m_loggedUser;

    return m_loggedUser != 0;
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

User* HNManager::loggedUser()
{
    return m_loggedUser;
}

void HNManager::logout()
{
    setUsername(QString());
    m_loggedUser = 0;

    QNetworkCookieJar* cookieJar = network->cookieJar();
    Q_FOREACH (const QNetworkCookie cookie, cookieJar->cookiesForUrl(QUrl(BASE_URL + "/"))) {
       cookieJar->deleteCookie(cookie);
    }
    m_settings->setValue("Cookies", QVariantList());
}

void HNManager::submit(const QString &title, const QString &url, const QString &text)
{
    qDebug() << "Submit item with title" << title;

    QNetworkRequest req(QUrl(BASE_URL + QLatin1String("/r")));
    req.setHeader(QNetworkRequest::ContentTypeHeader, QLatin1String("application/x-www-form-urlencoded"));

    QUrlQuery data;
    data.addQueryItem(QLatin1String("title"), title);
    data.addQueryItem(QLatin1String("url"), url);
    data.addQueryItem(QLatin1String("text"), text);
    data.addQueryItem(QLatin1String("fnop"), QLatin1String("submit-page"));
    data.addQueryItem(QLatin1String("fnid"), getSubmitCSRF());

    QNetworkReply* reply = network->post(req, data.toString(QUrl::FullyEncoded).toUtf8());

    connect(reply, &QNetworkReply::finished, this, &HNManager::onSubmitResult);
}

void HNManager::comment(const int parentId, const QString &text)
{
    qDebug() << "Comment item with id" << parentId;

    QNetworkRequest req(QUrl(BASE_URL + QLatin1String("/comment")));
    req.setHeader(QNetworkRequest::ContentTypeHeader, QLatin1String("application/x-www-form-urlencoded"));

    QUrlQuery data;
    data.addQueryItem(QLatin1String("parent"), QString::number(parentId));
    data.addQueryItem(QLatin1String("goto"), QStringLiteral("item?id=%1").arg(parentId));
    data.addQueryItem(QLatin1String("text"), text);
    data.addQueryItem(QLatin1String("hmac"), getCommentCSRF(parentId));

    QNetworkReply* reply = network->post(req, data.toString(QUrl::FullyEncoded).toUtf8());

    connect(reply, &QNetworkReply::finished, this, &HNManager::onCommentResult);
}

void HNManager::onSubmitResult()
{
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(QObject::sender());

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
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(QObject::sender());

    bool res = false;

    if (reply->error() != QNetworkReply::NoError) {
        qCritical() << "Cannot send comment" << reply->errorString();
    } else {
        if (!reply->readAll().contains("Please confirm that this is your comment by submitting it one more time.")) {
            res = true;
        }
    }

    Q_EMIT commented(res);

    reply->deleteLater();
}

QString HNManager::getSubmitCSRF() const
{
    QNetworkRequest req(QUrl(BASE_URL + QLatin1String("/submit")));
    QNetworkReply* reply = network->get(req);

    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    const QRegularExpression regexp("<input type=\"hidden\" name=\"fnid\" value=\"([^\"]+)\">");

    return getCSRF(reply, regexp);
}

QString HNManager::getCommentCSRF(const int itemId) const
{
    QUrl url(BASE_URL + QLatin1String("/item"));

    QUrlQuery query;
    query.addQueryItem(QLatin1String("id"), QString::number(itemId));
    url.setQuery(query);

    QNetworkRequest req(url);
    QNetworkReply* reply = network->get(req);

    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    const QRegularExpression regexp("<input type=\"hidden\" name=\"hmac\" value=\"([^\"]+)\">");

    return getCSRF(reply, regexp);
}

QString HNManager::getCSRF(QNetworkReply *reply, const QRegularExpression &regexp) const
{
    QTextStream stream(reply->readAll(), QIODevice::ReadOnly);

    QString line;
    while (!stream.atEnd()) {
        line = stream.readLine();

        QRegularExpressionMatch match = regexp.match(line);
        if (match.hasMatch()) {
            return match.captured(1);
        }
    }

    reply->deleteLater();

    return QString();
}
