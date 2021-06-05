/**
 * The MIT License (MIT)
 *
 * Copyright (c) 2015 Nathan Osman
 *               2016-2021 Andrea Scarpino <andrea@scarpino.dev>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 **/

#include "cookiejar.h"

#include <QCoreApplication>
#include <QNetworkCookie>
#include <QSettings>
#include <QVariantList>

CookieJar::CookieJar(QObject *parent) :
    QNetworkCookieJar(parent)
{
    // Load the list of cookies
    m_settings = new QSettings(QCoreApplication::applicationName(), QCoreApplication::applicationName(), this);

    // Read and parse each of the cookies
    QList<QNetworkCookie> cookies;
    Q_FOREACH (const QVariant cookie, m_settings->value("Cookies").toList()) {
        cookies.append(QNetworkCookie::parseCookies(cookie.toByteArray()));
    }

    // Set the list
    setAllCookies(cookies);
}

CookieJar::~CookieJar()
{
    // Build a list of all cookies
    QVariantList cookies;
    Q_FOREACH (const QNetworkCookie cookie, allCookies()) {
        cookies.append(cookie.toRawForm());
    }

    // Store the list
    m_settings->setValue("Cookies", cookies);
    m_settings->deleteLater();
}
