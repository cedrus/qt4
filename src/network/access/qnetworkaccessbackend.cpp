/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the QtNetwork module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial Usage
** Licensees holding valid Qt Commercial licenses may use this file in
** accordance with the Qt Commercial License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Nokia.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights.  These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
** If you have questions regarding the use of this file, please contact
** Nokia at qt-info@nokia.com.
** $QT_END_LICENSE$
**
****************************************************************************/

#include "qnetworkaccessbackend_p.h"
#include "qnetworkaccessmanager_p.h"
#include "qnetworkrequest.h"
#include "qnetworkreply.h"
#include "qnetworkreply_p.h"
#include "QtCore/qhash.h"
#include "QtCore/qmutex.h"
#include "QtNetwork/qnetworksession.h"

#include "qnetworkaccesscachebackend_p.h"
#include "qabstractnetworkcache.h"
#include "qhostinfo.h"

#include "private/qnoncontiguousbytedevice_p.h"

QT_BEGIN_NAMESPACE

static bool factoryDataShutdown = false;
class QNetworkAccessBackendFactoryData: public QList<QNetworkAccessBackendFactory *>
{
public:
    QNetworkAccessBackendFactoryData() : mutex(QMutex::Recursive) { }
    ~QNetworkAccessBackendFactoryData()
    {
        QMutexLocker locker(&mutex); // why do we need to lock?
        factoryDataShutdown = true;
    }

    QMutex mutex;
};
Q_GLOBAL_STATIC(QNetworkAccessBackendFactoryData, factoryData)

QNetworkAccessBackendFactory::QNetworkAccessBackendFactory()
{
    QMutexLocker locker(&factoryData()->mutex);
    factoryData()->append(this);
}

QNetworkAccessBackendFactory::~QNetworkAccessBackendFactory()
{
    if (!factoryDataShutdown) {
        QMutexLocker locker(&factoryData()->mutex);
        factoryData()->removeAll(this);
    }
}

QNetworkAccessBackend *QNetworkAccessManagerPrivate::findBackend(QNetworkAccessManager::Operation op,
                                                                 const QNetworkRequest &request)
{
    QNetworkRequest::CacheLoadControl mode =
        static_cast<QNetworkRequest::CacheLoadControl>(
            request.attribute(QNetworkRequest::CacheLoadControlAttribute,
                              QNetworkRequest::PreferNetwork).toInt());
    if (mode == QNetworkRequest::AlwaysCache
        && (op == QNetworkAccessManager::GetOperation
        || op == QNetworkAccessManager::HeadOperation)) {
        QNetworkAccessBackend *backend = new QNetworkAccessCacheBackend;
        backend->manager = this;
        return backend;
    }

    if (!factoryDataShutdown) {
        QMutexLocker locker(&factoryData()->mutex);
        QNetworkAccessBackendFactoryData::ConstIterator it = factoryData()->constBegin(),
                                                           end = factoryData()->constEnd();
        while (it != end) {
            QNetworkAccessBackend *backend = (*it)->create(op, request);
            if (backend) {
                backend->manager = this;
                return backend; // found a factory that handled our request
            }
            ++it;
        }
    }
    return 0;
}

QNonContiguousByteDevice* QNetworkAccessBackend::createUploadByteDevice()
{
    QNonContiguousByteDevice* device = 0;

    if (reply->outgoingDataBuffer)
        device = QNonContiguousByteDeviceFactory::create(reply->outgoingDataBuffer);
    else if (reply->outgoingData) {
        device = QNonContiguousByteDeviceFactory::create(reply->outgoingData);
    } else {
        return 0;
    }

    bool bufferDisallowed =
            reply->request.attribute(QNetworkRequest::DoNotBufferUploadDataAttribute,
                          QVariant(false)) == QVariant(true);
    if (bufferDisallowed)
        device->disableReset();

    // make sure we delete this later
    device->setParent(this);

    connect(device, SIGNAL(readProgress(qint64,qint64)), this, SLOT(emitReplyUploadProgress(qint64,qint64)));

    return device;
}

// need to have this function since the reply is a private member variable
// and the special backends need to access this.
void QNetworkAccessBackend::emitReplyUploadProgress(qint64 bytesSent, qint64 bytesTotal)
{
    if (reply->isFinished())
        return;
    reply->emitUploadProgress(bytesSent, bytesTotal);
}

QNetworkAccessBackend::QNetworkAccessBackend()
    : manager(0)
    , reply(0)
{
}

QNetworkAccessBackend::~QNetworkAccessBackend()
{
}

void QNetworkAccessBackend::downstreamReadyWrite()
{
    // do nothing
}

void QNetworkAccessBackend::setDownstreamLimited(bool b)
{
    Q_UNUSED(b);
    // do nothing
}

void QNetworkAccessBackend::copyFinished(QIODevice *)
{
    // do nothing
}

void QNetworkAccessBackend::ignoreSslErrors()
{
    // do nothing
}

void QNetworkAccessBackend::ignoreSslErrors(const QList<QSslError> &errors)
{
    Q_UNUSED(errors);
    // do nothing
}

void QNetworkAccessBackend::fetchSslConfiguration(QSslConfiguration &) const
{
    // do nothing
}

void QNetworkAccessBackend::setSslConfiguration(const QSslConfiguration &)
{
    // do nothing
}

QNetworkCacheMetaData QNetworkAccessBackend::fetchCacheMetaData(const QNetworkCacheMetaData &) const
{
    return QNetworkCacheMetaData();
}

QNetworkAccessManager::Operation QNetworkAccessBackend::operation() const
{
    return reply->operation;
}

QNetworkRequest QNetworkAccessBackend::request() const
{
    return reply->request;
}

#ifndef QT_NO_NETWORKPROXY
QList<QNetworkProxy> QNetworkAccessBackend::proxyList() const
{
    return reply->proxyList;
}
#endif

QAbstractNetworkCache *QNetworkAccessBackend::networkCache() const
{
    if (!manager)
        return 0;
    return manager->networkCache;
}

void QNetworkAccessBackend::setCachingEnabled(bool enable)
{
    reply->setCachingEnabled(enable);
}

bool QNetworkAccessBackend::isCachingEnabled() const
{
    return reply->isCachingEnabled();
}

qint64 QNetworkAccessBackend::nextDownstreamBlockSize() const
{
    return reply->nextDownstreamBlockSize();
}

void QNetworkAccessBackend::writeDownstreamData(QByteDataBuffer &list)
{
    reply->appendDownstreamData(list);
}

void QNetworkAccessBackend::writeDownstreamData(QIODevice *data)
{
    reply->appendDownstreamData(data);
}

QVariant QNetworkAccessBackend::header(QNetworkRequest::KnownHeaders header) const
{
    return reply->q_func()->header(header);
}

void QNetworkAccessBackend::setHeader(QNetworkRequest::KnownHeaders header, const QVariant &value)
{
    reply->setCookedHeader(header, value);
}

bool QNetworkAccessBackend::hasRawHeader(const QByteArray &headerName) const
{
    return reply->q_func()->hasRawHeader(headerName);
}

QByteArray QNetworkAccessBackend::rawHeader(const QByteArray &headerName) const
{
    return reply->q_func()->rawHeader(headerName);
}

QList<QByteArray> QNetworkAccessBackend::rawHeaderList() const
{
    return reply->q_func()->rawHeaderList();
}

void QNetworkAccessBackend::setRawHeader(const QByteArray &headerName, const QByteArray &headerValue)
{
    reply->setRawHeader(headerName, headerValue);
}

QVariant QNetworkAccessBackend::attribute(QNetworkRequest::Attribute code) const
{
    return reply->q_func()->attribute(code);
}

void QNetworkAccessBackend::setAttribute(QNetworkRequest::Attribute code, const QVariant &value)
{
    if (value.isValid())
        reply->attributes.insert(code, value);
    else
        reply->attributes.remove(code);
}
QUrl QNetworkAccessBackend::url() const
{
    return reply->url;
}

void QNetworkAccessBackend::setUrl(const QUrl &url)
{
    reply->url = url;
}

void QNetworkAccessBackend::finished()
{
    reply->finished();
}

void QNetworkAccessBackend::error(QNetworkReply::NetworkError code, const QString &errorString)
{
    reply->error(code, errorString);
}

#ifndef QT_NO_NETWORKPROXY
void QNetworkAccessBackend::proxyAuthenticationRequired(const QNetworkProxy &proxy,
                                                        QAuthenticator *authenticator)
{
    manager->proxyAuthenticationRequired(this, proxy, authenticator);
}
#endif

void QNetworkAccessBackend::authenticationRequired(QAuthenticator *authenticator)
{
    manager->authenticationRequired(this, authenticator);
}

void QNetworkAccessBackend::cacheCredentials(QAuthenticator *authenticator)
{
    manager->addCredentials(this->reply->url, authenticator);
}

void QNetworkAccessBackend::metaDataChanged()
{
    reply->metaDataChanged();
}

void QNetworkAccessBackend::redirectionRequested(const QUrl &target)
{
    reply->redirectionRequested(target);
}

void QNetworkAccessBackend::sslErrors(const QList<QSslError> &errors)
{
#ifndef QT_NO_OPENSSL
    reply->sslErrors(errors);
#else
    Q_UNUSED(errors);
#endif
}

#ifndef QT_NO_BEARERMANAGEMENT

/*!
    Starts the backend.  Returns true if the backend is started.  Returns false if the backend
    could not be started due to an unopened or roaming session.  The caller should recall this
    function once the session has been opened or the roaming process has finished.
*/
bool QNetworkAccessBackend::start()
{
    if (!manager->networkSession) {
        open();
        return true;
    }

    // This is not ideal.
    const QString host = reply->url.host();
    if (host == QLatin1String("localhost") ||
        QHostAddress(host) == QHostAddress::LocalHost ||
        QHostAddress(host) == QHostAddress::LocalHostIPv6) {
        // Don't need an open session for localhost access.
        open();
        return true;
    }

    if (manager->networkSession->isOpen() &&
        manager->networkSession->state() == QNetworkSession::Connected) {
        open();
        return true;
    }

    return false;
}
#endif

QT_END_NAMESPACE
