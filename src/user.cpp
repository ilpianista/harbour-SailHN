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

#include "user.h"

#include "user_p.h"

User::User(QObject *parent) : QObject(parent)
  , d(new UserPrivate)
{
}

User::~User()
{
    delete d;
}

QString User::id() const
{
    return d->id;
}

void User::setId(const QString id)
{
    d->id = id;
}

quint16 User::delay() const
{
    return d->delay;
}

void User::setDelay(const quint16 delay)
{
    d->delay = delay;
}

QDateTime User::created() const
{
    return d->created;
}

void User::setCreated(const QDateTime created)
{
    d->created = created;
}

int User::karma() const
{
    return d->karma;
}

void User::setKarma(const int karma)
{
    d->karma = karma;
}

QString User::about() const
{
    return d->about;
}

void User::setAbout(const QString about)
{
    d->about = about;
}

QList<int> User::submitted() const
{
    return d->submitted;
}

void User::setSubmitted(const QList<int> submitted)
{
    d->submitted = submitted;
}
