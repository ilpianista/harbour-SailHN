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
    Q_PROPERTY(int id READ id)
    Q_PROPERTY(QString by READ by)
    Q_PROPERTY(int descendants READ descendants)
    Q_PROPERTY(QList<int> kids READ kids)
    Q_PROPERTY(quint16 score READ score)
    Q_PROPERTY(QString text READ text)
    Q_PROPERTY(QDateTime time READ time)
    Q_PROPERTY(QString title READ title)
    Q_PROPERTY(QUrl url READ url)

public:
    explicit Item(QObject *parent = 0);
    virtual ~Item();

    int id() const;
    void setId(const int id);

    QString by() const;
    void setBy(const QString by);

    bool deleted() const;
    void setDeleted(const bool deleted);

    int descendants() const;
    void setDescendants(const int descendants);

    QList<int> kids() const;
    void setKids(const QList<int> kids);

    quint16 score() const;
    void setScore(const quint16 score);

    QString text() const;
    void setText(const QString text);

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
