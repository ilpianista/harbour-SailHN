/*
  The MIT License (MIT)

  Copyright (c) 2015-2021 Andrea Scarpino <andrea@scarpino.dev>

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

#include "item.h"

#include "item_p.h"

Item::Item(QObject *parent) : QObject(parent)
  , d(new ItemPrivate)
{
}

Item::~Item()
{
    delete d;
}

int Item::id() const
{
    return d->id;
}

void Item::setId(const int id)
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

bool Item::dead() const
{
    return d->dead;
}

void Item::setDead(const bool dead)
{
    d->dead = dead;
}

bool Item::deleted() const
{
    return d->deleted;
}

void Item::setDeleted(const bool deleted)
{
    d->deleted = deleted;
}

int Item::descendants() const
{
    return d->descendants;
}

void Item::setDescendants(const int descendants)
{
    d->descendants = descendants;
}

QList<int> Item::kids() const
{
    return d->kids;
}

void Item::setKids(const QList<int> kids)
{
    d->kids = kids;
}

int Item::parent() const
{
    return d->parent;
}

void Item::setParent(const int parent)
{
    d->parent = parent;
}

QList<int> Item::parts() const
{
    return d->parts;
}

void Item::setParts(const QList<int> parts)
{
    d->parts = parts;
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

Item::Type Item::type() const
{
    return d->type;
}

void Item::setType(const Item::Type type)
{
    d->type = type;
}

QUrl Item::url() const
{
    return d->url;
}

void Item::setUrl(const QUrl url)
{
    d->url = url;
}
