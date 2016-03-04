/*
  The MIT License (MIT)

  Copyright (c) 2015-2016 Andrea Scarpino <me@andreascarpino.it>

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

#include "hackernewsapi.h"

#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>

#include "item.h"
#include "user.h"

const static QString API_URL = QStringLiteral("https://hacker-news.firebaseio.com/v0");

HackerNewsAPI::HackerNewsAPI(QObject *parent) :
    QObject(parent)
  , network(new QNetworkAccessManager(this))
{
}

HackerNewsAPI::~HackerNewsAPI()
{
    delete network;
}

void HackerNewsAPI::getItem(const int id)
{
    //qDebug() << "Requesting item with id" << id;

    QUrl url(API_URL + QStringLiteral("/item/%1.json").arg(id));
    QNetworkRequest req(url);
    QNetworkReply* reply = network->get(req);

    connect(reply, &QNetworkReply::finished, this, &HackerNewsAPI::onGetItemResult);
}

void HackerNewsAPI::getUser(const QString id)
{
    //qDebug() << "Requesting user with id" << id;

    QUrl url(API_URL + QStringLiteral("/user/%1.json").arg(id));

    QNetworkRequest req(url);
    QNetworkReply* reply = network->get(req);

    connect(reply, &QNetworkReply::finished, this, &HackerNewsAPI::onGetUserResult);
}

void HackerNewsAPI::getStories(Stories kind)
{
    qDebug() << "Requesting stories";

    QString path;
    switch (kind) {
        case Ask: path = QStringLiteral("/askstories.json"); break;
        case Job: path = QStringLiteral("/jobstories.json"); break;
        case New: path = QStringLiteral("/newstories.json"); break;
        case Show: path = QStringLiteral("/showstories.json"); break;
        case Top: path = QStringLiteral("/topstories.json"); break;
        default: qCritical() << "Unrecognized kind" << kind;
    }

    QUrl url(API_URL + path);
    QNetworkRequest req(url);
    QNetworkReply* reply = network->get(req);

    connect(reply, &QNetworkReply::finished, this, &HackerNewsAPI::onStoriesResult);
}

void HackerNewsAPI::onGetItemResult()
{
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(QObject::sender());

    if (reply->error() != QNetworkReply::NoError) {
        qCritical() << "Cannot fetch item" << reply->errorString();
    } else {
        QJsonDocument json = QJsonDocument::fromJson(reply->readAll());
        if (!json.isNull()) {
            //qDebug() << "Got item:\n" << json;

            Item* item = new Item();
            QJsonObject jsonObj = json.object();
            item->setId(jsonObj.value("id").toInt());
            //qDebug() << "Got item with id" << item->id();

            item->setBy(jsonObj.value("by").toString());
            item->setDeleted(jsonObj.value("deleted").toBool());
            item->setDescendants(jsonObj.value("descendants").toInt());

            QJsonArray jsonKids = jsonObj.value("kids").toArray();
            QList<int> kids;
            Q_FOREACH (const QVariant kid, jsonKids.toVariantList()) {
                kids.append(kid.toInt());
            }
            item->setKids(kids);

            item->setText(jsonObj.value("text").toString());
            item->setTitle(jsonObj.value("title").toString());
            item->setUrl(QUrl(jsonObj.value("url").toString()));
            item->setScore(jsonObj.value("score").toInt());
            QDateTime timestamp;
            timestamp.setTime_t(jsonObj.value("time").toInt());
            item->setTime(timestamp);

            Q_EMIT itemFetched(item);
        } else {
            qCritical() << "Got an invalid JSON!";
        }
    }

    reply->deleteLater();
}

void HackerNewsAPI::onGetUserResult()
{
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(QObject::sender());

    if (reply->error() != QNetworkReply::NoError) {
        qCritical() << "Cannot fetch user:" << reply->errorString();
    } else {
        QJsonDocument json = QJsonDocument::fromJson(reply->readAll());
        if (!json.isNull()) {
            //qDebug() << "Got user:\n" << json;

            User* user = new User();
            QJsonObject jsonObj = json.object();
            user->setId(jsonObj.value("id").toString());
            //qDebug() << "Got user with id" << user->id();

            user->setDelay(jsonObj.value("delay").toInt());
            user->setKarma(jsonObj.value("karma").toInt());
            user->setAbout(jsonObj.value("about").toString());

            QDateTime created;
            created.setTime_t(jsonObj.value("created").toInt());
            user->setCreated(created);

            QJsonArray jsonSumitted = jsonObj.value("submitted").toArray();
            QList<int> submitted;
            Q_FOREACH (const QVariant s, jsonSumitted.toVariantList()) {
                submitted.append(s.toInt());
            }
            user->setSubmitted(submitted);

            Q_EMIT userFetched(user);
        } else {
            qCritical() << "Got an invalid JSON!";
        }
    }

    reply->deleteLater();
}

void HackerNewsAPI::onStoriesResult()
{
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(QObject::sender());

    if (reply->error() != QNetworkReply::NoError) {
        qCritical() << "Cannot fetch stories" << reply->errorString();
    } else {
        QJsonDocument json = QJsonDocument::fromJson(reply->readAll());
        if (!json.isNull()) {
            qDebug() << "There are" << json.array().size() << "items";

            QList<int> ids;
            Q_FOREACH (const QJsonValue id, json.array()) {
                ids.append(id.toInt());
            }

            Q_EMIT storiesFetched(ids);
        } else {
            qCritical() << "Got an invalid JSON!";
        }
    }

    reply->deleteLater();
}
