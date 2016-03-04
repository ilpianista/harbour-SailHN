/*
  The MIT License (MIT)

  Copyright (c) 2016 Andrea Scarpino <me@andreascarpino.it>

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

#ifndef USER_H
#define USER_H

#include <QDateTime>
#include <QObject>
#include <QString>

class UserPrivate;

class User : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString id READ id)
    Q_PROPERTY(quint16 delay READ delay)
    Q_PROPERTY(QDateTime created READ created)
    Q_PROPERTY(int karma READ karma)
    Q_PROPERTY(QString about READ about)

public:
    explicit User(QObject *parent = 0);
    virtual ~User();

    QString id() const;
    void setId(const QString id);

    quint16 delay() const;
    void setDelay(const quint16 delay);

    QDateTime created() const;
    void setCreated(const QDateTime created);

    int karma() const;
    void setKarma(const int karma);

    QString about() const;
    void setAbout(const QString about);

    QList<int> submitted() const;
    void setSubmitted(const QList<int> submitted);

private:
    UserPrivate *d;

};

#endif // USER_H
