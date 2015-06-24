#ifndef ITEM_PRIVATE_H
#define ITEM_PRIVATE_H

#include <QString>
#include <QDateTime>
#include <QUrl>

struct ItemPrivate {
    int id;
    QString by;
    bool deleted;
    QList<int> kids;
    quint16 score;
    QString text;
    QDateTime time;
    QString title;
    QUrl url;
};

#endif // ITEM_PRIVATE_H
