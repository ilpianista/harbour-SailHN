#include "item.h"

#include "item_p.h"

Item::Item(QObject *parent) : QObject(parent)
  , d(new ItemPrivate)
{
}

Item::~Item()
{
}

int Item::id() const
{
    return d->id;
}

void Item::setId(const qint32 id)
{
    d->id = id;
}

QString Item::by() const
{
    return d->by;
}

void Item::setBy(const QString by)
{
    d->by = by;
}

QList<qint32> Item::kids() const
{
    return d->kids;
}

void Item::setKids(const QList<qint32> kids)
{
    d->kids = kids;
}

quint16 Item::score() const
{
    return d->score;
}

void Item::setScore(const quint16 score)
{
    d->score = score;
}

QString Item::text() const
{
    return d->text;
}

void Item::setText(const QString text)
{
    d->text = text;
}

QDateTime Item::time() const
{
    return d->time;
}

void Item::setTime(const QDateTime time)
{
    d->time = time;
}

QString Item::title() const
{
    return d->title;
}

void Item::setTitle(const QString title)
{
    d->title = title;
}

QUrl Item::url() const
{
    return d->url;
}

void Item::setUrl(const QUrl url)
{
    d->url = url;
}
