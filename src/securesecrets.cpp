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

#include "securesecrets.h"

#include <QDebug>
#include <QEventLoop>
#include <QNetworkCookie>
#include <QTimer>

using namespace Sailfish::Secrets;

SecureSecrets::SecureSecrets(QObject *parent)
    : QObject(parent)
    , m_collectionName(QStringLiteral("SailHN"))
    , m_initialized(false)
{}

void SecureSecrets::initialize()
{
    ensureCollection();
}

void SecureSecrets::ensureCollection()
{
    CreateCollectionRequest *request = new CreateCollectionRequest(this);
    request->setManager(&m_secretManager);
    request->setCollectionName(m_collectionName);
    request->setAccessControlMode(SecretManager::OwnerOnlyMode);
    request->setCollectionLockType(CreateCollectionRequest::DeviceLock);
    request->setDeviceLockUnlockSemantic(SecretManager::DeviceLockKeepUnlocked);
    request->setStoragePluginName(SecretManager::DefaultEncryptedStoragePluginName);
    request->setEncryptionPluginName(SecretManager::DefaultEncryptedStoragePluginName);

    connect(request, &CreateCollectionRequest::statusChanged, this, [this, request]() {
        if (request->status() == Request::Finished) {
            if (request->result().code() == Result::Succeeded
                || request->result().errorCode() == Result::CollectionAlreadyExistsError) {
                m_initialized = true;
                emit initialized();
            } else {
                qWarning() << "Failed to create secrets collection:"
                           << request->result().errorMessage();
                emit error(request->result().errorMessage());
            }
            request->deleteLater();
        }
    });

    request->startRequest();
}

void SecureSecrets::storeCookies(const QList<QNetworkCookie> &cookies)
{
    if (!m_initialized) {
        qWarning() << "SecureSecrets not initialized";
        return;
    }

    // Serialize cookies to QByteArray
    QByteArray cookiesData = QByteArray();
    for (const QNetworkCookie &cookie : cookies) {
        if (!cookiesData.isEmpty())
            cookiesData += '\n'; // Separate cookies with newline
        cookiesData += cookie.toRawForm();
    }

    storeSecret(QStringLiteral("cookies"), cookiesData);
}

QList<QNetworkCookie> SecureSecrets::loadCookies() const
{
    QList<QNetworkCookie> cookies;

    if (!m_initialized) {
        qWarning() << "SecureSecrets not initialized";
        return cookies;
    }

    QByteArray cookieData = getSecret(QStringLiteral("cookies"));

    return QNetworkCookie::parseCookies(cookieData);
}

void SecureSecrets::clearCookies()
{
    deleteSecret(QStringLiteral("cookies"));
}

void SecureSecrets::storeSecret(const QString &name, const QByteArray &value)
{
    // First delete any existing secret with this name (to allow updates)
    DeleteSecretRequest *deleteRequest = new DeleteSecretRequest(this);
    deleteRequest->setManager(&m_secretManager);
    deleteRequest->setIdentifier(
        Secret::Identifier(name,
                           m_collectionName,
                           SecretManager::DefaultEncryptedStoragePluginName));
    deleteRequest->setUserInteractionMode(SecretManager::SystemInteraction);

    connect(deleteRequest,
            &DeleteSecretRequest::statusChanged,
            this,
            [this, deleteRequest, name, value]() {
                if (deleteRequest->status() == Request::Finished) {
                    deleteRequest->deleteLater();

                    // Now store the new secret
                    Secret secret(
                        Secret::Identifier(name,
                                           m_collectionName,
                                           SecretManager::DefaultEncryptedStoragePluginName));
                    secret.setData(value);
                    secret.setType(Secret::TypeBlob);

                    StoreSecretRequest *storeRequest = new StoreSecretRequest(this);
                    storeRequest->setManager(&m_secretManager);
                    storeRequest->setSecretStorageType(StoreSecretRequest::CollectionSecret);
                    storeRequest->setUserInteractionMode(SecretManager::SystemInteraction);
                    storeRequest->setSecret(secret);

                    connect(storeRequest,
                            &StoreSecretRequest::statusChanged,
                            this,
                            [storeRequest, name]() {
                                if (storeRequest->status() == Request::Finished) {
                                    if (storeRequest->result().code() != Result::Succeeded) {
                                        qWarning() << "Failed to store secret" << name << ":"
                                                   << storeRequest->result().errorMessage();
                                    }
                                    storeRequest->deleteLater();
                                }
                            });

                    storeRequest->startRequest();
                }
            });

    deleteRequest->startRequest();
}

QByteArray SecureSecrets::getSecret(const QString &name) const
{
    StoredSecretRequest request;
    request.setManager(const_cast<SecretManager *>(&m_secretManager));
    request.setIdentifier(Secret::Identifier(name,
                                             m_collectionName,
                                             SecretManager::DefaultEncryptedStoragePluginName));
    request.setUserInteractionMode(SecretManager::SystemInteraction);

    // Use event loop for synchronous retrieval with a safety timeout
    QEventLoop loop;
    QTimer timeoutTimer;
    timeoutTimer.setSingleShot(true);
    QObject::connect(&timeoutTimer, &QTimer::timeout, &loop, &QEventLoop::quit);
    QObject::connect(&request, &StoredSecretRequest::statusChanged, &loop, [&loop, &request]() {
        if (request.status() == Request::Finished) {
            loop.quit();
        }
    });

    request.startRequest();

    if (request.status() != Request::Finished) {
        timeoutTimer.start(5000);
        loop.exec();
    }

    if (request.status() != Request::Finished) {
        qWarning() << "Timed out waiting for secret" << name;
        return QByteArray();
    }

    if (request.result().code() == Result::Succeeded) {
        return request.secret().data();
    }

    qWarning() << "Failed to retrieve secret" << name << ":" << request.result().errorMessage();
    return QByteArray();
}

void SecureSecrets::deleteSecret(const QString &name)
{
    DeleteSecretRequest *request = new DeleteSecretRequest(this);
    request->setManager(&m_secretManager);
    request->setIdentifier(Secret::Identifier(name,
                                              m_collectionName,
                                              SecretManager::DefaultEncryptedStoragePluginName));
    request->setUserInteractionMode(SecretManager::SystemInteraction);

    connect(request, &DeleteSecretRequest::statusChanged, this, [request, name]() {
        if (request->status() == Request::Finished) {
            if (request->result().code() != Result::Succeeded) {
                qWarning() << "Failed to delete secret" << name << ":"
                           << request->result().errorMessage();
            }
            request->deleteLater();
        }
    });

    request->startRequest();
}
