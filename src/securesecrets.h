/**
 * The MIT License (MIT)
 *
 * Copyright (c) 2026 Andrea Scarpino <andrea@scarpino.dev>
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

#ifndef SECURESECRETS_H
#define SECURESECRETS_H

#include <Sailfish/Secrets/createcollectionrequest.h>
#include <Sailfish/Secrets/deletesecretrequest.h>
#include <Sailfish/Secrets/result.h>
#include <Sailfish/Secrets/secret.h>
#include <Sailfish/Secrets/secretmanager.h>
#include <Sailfish/Secrets/storedsecretrequest.h>
#include <Sailfish/Secrets/storesecretrequest.h>
#include <QList>
#include <QNetworkCookie>
#include <QObject>

class SecureSecrets : public QObject
{
    Q_OBJECT

public:
    explicit SecureSecrets(QObject *parent = nullptr);

    Q_INVOKABLE void initialize();

    Q_INVOKABLE void storeCookies(const QList<QNetworkCookie> &cookies);
    Q_INVOKABLE QList<QNetworkCookie> loadCookies() const;
    Q_INVOKABLE void clearCookies();

signals:
    void initialized();
    void error(const QString &message);

private:
    void ensureCollection();
    void storeSecret(const QString &name, const QByteArray &value);
    QByteArray getSecret(const QString &name) const;
    void deleteSecret(const QString &name);

    Sailfish::Secrets::SecretManager m_secretManager;
    QString m_collectionName;
    bool m_initialized;
};

#endif // SECURESECRETS_H
