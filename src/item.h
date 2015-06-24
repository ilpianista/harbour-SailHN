#ifndef ITEM_H
#define ITEM_H

#include <QDateTime>
#include <QObject>
#include <QString>
#include <QUrl>

class ItemPrivate;

class Item : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString by READ by)
    Q_PROPERTY(QList<qint32> kids READ kids)
    Q_PROPERTY(quint16 score READ score)
    Q_PROPERTY(QDateTime time READ time)
    Q_PROPERTY(QString title READ title)
    Q_PROPERTY(QUrl url READ url)
public:
    explicit Item(QObject *parent = 0);
    virtual ~Item();

    int id() const;
    void setId(const qint32 id);

    QString by() const;
    void setBy(const QString by);

    QList<qint32> kids() const;
    void setKids(const QList<qint32> kids);

    quint16 score() const;
    void setScore(const quint16 score);

    QDateTime time() const;
    void setTime(const QDateTime time);

    QString title() const;
    void setTitle(const QString title);

    QUrl url() const;
    void setUrl(const QUrl url);

private:
    ItemPrivate *d;

};

#endif // ITEM_H
