#ifndef ITEM_PRIVATE_H
#define ITEM_PRIVATE_H

#include <QString>
#include <QDateTime>
#include <QUrl>

struct ItemPrivate {
    qint32 id;
    QString by;
    QList<qint32> kids;
    quint16 score;
    QDateTime time;
    QString title;
    QUrl url;
};

#endif // ITEM_PRIVATE_H
