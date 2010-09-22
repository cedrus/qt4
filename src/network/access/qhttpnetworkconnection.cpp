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

#include "qhttpnetworkconnection_p.h"
#include "qhttpnetworkconnectionchannel_p.h"
#include "private/qnoncontiguousbytedevice_p.h"
#include <private/qnetworkrequest_p.h>
#include <private/qobject_p.h>
#include <private/qauthenticator_p.h>
#include <qnetworkproxy.h>
#include <qauthenticator.h>

#include <qbuffer.h>
#include <qpair.h>
#include <qhttp.h>
#include <qdebug.h>

#ifndef QT_NO_HTTP

#ifndef QT_NO_OPENSSL
#    include <QtNetwork/qsslkey.h>
#    include <QtNetwork/qsslcipher.h>
#    include <QtNetwork/qsslconfiguration.h>
#endif



QT_BEGIN_NAMESPACE

#ifdef Q_OS_SYMBIAN
const int QHttpNetworkConnectionPrivate::defaultChannelCount = 3;
#else
const int QHttpNetworkConnectionPrivate::defaultChannelCount = 6;
#endif

// The pipeline length. So there will be 4 requests in flight.
const int QHttpNetworkConnectionPrivate::defaultPipelineLength = 3;
// Only re-fill the pipeline if there's defaultRePipelineLength slots free in the pipeline.
// This means that there are 2 requests in flight and 2 slots free that will be re-filled.
const int QHttpNetworkConnectionPrivate::defaultRePipelineLength = 2;


QHttpNetworkConnectionPrivate::QHttpNetworkConnectionPrivate(const QString &hostName, quint16 port, bool encrypt)
: hostName(hostName), port(port), encrypt(encrypt),
  channelCount(defaultChannelCount),
  pendingAuthSignal(false), pendingProxyAuthSignal(false)
#ifndef QT_NO_NETWORKPROXY
  , networkProxy(QNetworkProxy::NoProxy)
#endif
{
    channels = new QHttpNetworkConnectionChannel[channelCount];
}

QHttpNetworkConnectionPrivate::QHttpNetworkConnectionPrivate(quint16 channelCount, const QString &hostName, quint16 port, bool encrypt)
: hostName(hostName), port(port), encrypt(encrypt),
  channelCount(channelCount),
  pendingAuthSignal(false), pendingProxyAuthSignal(false)
#ifndef QT_NO_NETWORKPROXY
  , networkProxy(QNetworkProxy::NoProxy)
#endif
{
    channels = new QHttpNetworkConnectionChannel[channelCount];
}



QHttpNetworkConnectionPrivate::~QHttpNetworkConnectionPrivate()
{
    for (int i = 0; i < channelCount; ++i) {
        if (channels[i].socket) {
            channels[i].socket->close();
            delete channels[i].socket;
        }
    }
    delete []channels;
}

void QHttpNetworkConnectionPrivate::init()
{
    for (int i = 0; i < channelCount; i++) {
        channels[i].setConnection(this->q_func());
        channels[i].init();
    }
}

int QHttpNetworkConnectionPrivate::indexOf(QAbstractSocket *socket) const
{
    for (int i = 0; i < channelCount; ++i)
        if (channels[i].socket == socket)
            return i;

    qFatal("Called with unknown socket object.");
    return 0;
}

qint64 QHttpNetworkConnectionPrivate::uncompressedBytesAvailable(const QHttpNetworkReply &reply) const
{
    return reply.d_func()->responseData.byteAmount();
}

qint64 QHttpNetworkConnectionPrivate::uncompressedBytesAvailableNextBlock(const QHttpNetworkReply &reply) const
{
    return reply.d_func()->responseData.sizeNextBlock();
}

void QHttpNetworkConnectionPrivate::prepareRequest(HttpMessagePair &messagePair)
{
    QHttpNetworkRequest &request = messagePair.first;
    QHttpNetworkReply *reply = messagePair.second;

    // add missing fields for the request
    QByteArray value;
    // check if Content-Length is provided
    QNonContiguousByteDevice* uploadByteDevice = request.uploadByteDevice();
    if (uploadByteDevice) {
        if (request.contentLength() != -1 && uploadByteDevice->size() != -1) {
            // both values known, take the smaller one.
            request.setContentLength(qMin(uploadByteDevice->size(), request.contentLength()));
        } else if (request.contentLength() == -1 && uploadByteDevice->size() != -1) {
            // content length not supplied by user, but the upload device knows it
            request.setContentLength(uploadByteDevice->size());
        } else if (request.contentLength() != -1 && uploadByteDevice->size() == -1) {
            // everything OK, the user supplied us the contentLength
        } else if (request.contentLength() == -1 && uploadByteDevice->size() == -1) {
            qFatal("QHttpNetworkConnectionPrivate: Neither content-length nor upload device size were given");
        }
    }
    // set the Connection/Proxy-Connection: Keep-Alive headers
#ifndef QT_NO_NETWORKPROXY
    if (networkProxy.type() == QNetworkProxy::HttpCachingProxy)  {
        value = request.headerField("proxy-connection");
        if (value.isEmpty())
            request.setHeaderField("Proxy-Connection", "Keep-Alive");
    } else {
#endif
        value = request.headerField("connection");
        if (value.isEmpty())
            request.setHeaderField("Connection", "Keep-Alive");
#ifndef QT_NO_NETWORKPROXY
    }
#endif

    // If the request had a accept-encoding set, we better not mess
    // with it. If it was not set, we announce that we understand gzip
    // and remember this fact in request.d->autoDecompress so that
    // we can later decompress the HTTP reply if it has such an
    // encoding.
    value = request.headerField("accept-encoding");
    if (value.isEmpty()) {
#ifndef QT_NO_COMPRESS
        request.setHeaderField("Accept-Encoding", "gzip");
        request.d->autoDecompress = true;
#else
        // if zlib is not available set this to false always
        request.d->autoDecompress = false;
#endif
    }

    // some websites mandate an accept-language header and fail
    // if it is not sent. This is a problem with the website and
    // not with us, but we work around this by setting
    // one always.
    value = request.headerField("accept-language");
    if (value.isEmpty()) {
        QString systemLocale = QLocale::system().name().replace(QChar::fromAscii('_'),QChar::fromAscii('-'));
        QString acceptLanguage;
        if (systemLocale == QLatin1String("C"))
            acceptLanguage = QString::fromAscii("en,*");
        else if (systemLocale.startsWith(QLatin1String("en-")))
            acceptLanguage = QString::fromAscii("%1,*").arg(systemLocale);
        else
            acceptLanguage = QString::fromAscii("%1,en,*").arg(systemLocale);
        request.setHeaderField("Accept-Language", acceptLanguage.toAscii());
    }

    // set the User Agent
    value = request.headerField("user-agent");
    if (value.isEmpty())
        request.setHeaderField("User-Agent", "Mozilla/5.0");
    // set the host
    value = request.headerField("host");
    if (value.isEmpty()) {
        QByteArray host = QUrl::toAce(hostName);

        int port = request.url().port();
        if (port != -1) {
            host += ':';
            host += QByteArray::number(port);
        }

        request.setHeaderField("Host", host);
    }

    reply->d_func()->requestIsPrepared = true;
}




void QHttpNetworkConnectionPrivate::emitReplyError(QAbstractSocket *socket,
                                                   QHttpNetworkReply *reply,
                                                   QNetworkReply::NetworkError errorCode)
{
    Q_Q(QHttpNetworkConnection);
    if (socket && reply) {
        // this error matters only to this reply
        reply->d_func()->errorString = errorDetail(errorCode, socket);
        emit reply->finishedWithError(errorCode, reply->d_func()->errorString);
        int i = indexOf(socket);
        // remove the corrupt data if any
        reply->d_func()->eraseData();
        channels[i].close();
        // send the next request
        QMetaObject::invokeMethod(q, "_q_startNextRequest", Qt::QueuedConnection);
    }
}

void QHttpNetworkConnectionPrivate::copyCredentials(int fromChannel, QAuthenticator *auth, bool isProxy)
{
    Q_ASSERT(auth);

    // select another channel
    QAuthenticator* otherAuth = 0;
    for (int i = 0; i < channelCount; ++i) {
        if (i == fromChannel)
            continue;
        if (isProxy)
            otherAuth = &channels[i].proxyAuthenticator;
        else
            otherAuth = &channels[i].authenticator;
        // if the credentials are different, copy them
        if (otherAuth->user().compare(auth->user()))
            otherAuth->setUser(auth->user());
        if (otherAuth->password().compare(auth->password()))
            otherAuth->setPassword(auth->password());
    }
}


// handles the authentication for one channel and eventually re-starts the other channels
bool QHttpNetworkConnectionPrivate::handleAuthenticateChallenge(QAbstractSocket *socket, QHttpNetworkReply *reply,
                                                                bool isProxy, bool &resend)
{
    Q_ASSERT(socket);
    Q_ASSERT(reply);

    Q_Q(QHttpNetworkConnection);

    resend = false;
    //create the response header to be used with QAuthenticatorPrivate.
    QList<QPair<QByteArray, QByteArray> > fields = reply->header();

    //find out the type of authentication protocol requested.
    QAuthenticatorPrivate::Method authMethod = reply->d_func()->authenticationMethod(isProxy);
    if (authMethod != QAuthenticatorPrivate::None) {
        int i = indexOf(socket);
        //Use a single authenticator for all domains. ### change later to use domain/realm
        QAuthenticator* auth = 0;
        if (isProxy) {
            auth = &channels[i].proxyAuthenticator;
            channels[i].proxyAuthMehtod = authMethod;
        } else {
            auth = &channels[i].authenticator;
            channels[i].authMehtod = authMethod;
        }
        //proceed with the authentication.
        if (auth->isNull())
            auth->detach();
        QAuthenticatorPrivate *priv = QAuthenticatorPrivate::getPrivate(*auth);
        priv->parseHttpResponse(fields, isProxy);

        if (priv->phase == QAuthenticatorPrivate::Done) {
            if ((isProxy && pendingProxyAuthSignal) ||(!isProxy && pendingAuthSignal)) {
                // drop the request
                reply->d_func()->eraseData();
                channels[i].close();
                channels[i].lastStatus = 0;
                channels[i].state =  QHttpNetworkConnectionChannel::Wait4AuthState;
                return false;
            }
            // cannot use this socket until the slot returns
            channels[i].state = QHttpNetworkConnectionChannel::WaitingState;
            socket->blockSignals(true);
            if (!isProxy) {
                pendingAuthSignal = true;
                emit q->authenticationRequired(reply->request(), auth, q);
                pendingAuthSignal = false;
#ifndef QT_NO_NETWORKPROXY
            } else {
                pendingProxyAuthSignal = true;
                emit q->proxyAuthenticationRequired(networkProxy, auth, q);
                pendingProxyAuthSignal = false;
#endif
            }
            socket->blockSignals(false);
            // socket free to use
            channels[i].state = QHttpNetworkConnectionChannel::IdleState;
            if (priv->phase != QAuthenticatorPrivate::Done) {
                // send any pending requests
                copyCredentials(i,  auth, isProxy);
                QMetaObject::invokeMethod(q, "_q_restartAuthPendingRequests", Qt::QueuedConnection);
            }
        } else if (priv->phase == QAuthenticatorPrivate::Start) {
            // If the url's authenticator has a 'user' set we will end up here (phase is only set to 'Done' by
            // parseHttpResponse above if 'user' is empty). So if credentials were supplied with the request,
            // such as in the case of an XMLHttpRequest, this is our only opportunity to cache them.
            emit q->cacheCredentials(reply->request(), auth, q);
        }
        // - Changing values in QAuthenticator will reset the 'phase'.
        // - If withCredentials has been set to false (e.g. by QtWebKit for a cross-origin XMLHttpRequest) then
        //   we need to bail out if authentication is required.
        if (priv->phase == QAuthenticatorPrivate::Done || !reply->request().withCredentials()) {
            // authentication is cancelled, send the current contents to the user.
            emit channels[i].reply->headerChanged();
            emit channels[i].reply->readyRead();
            QNetworkReply::NetworkError errorCode =
                isProxy
                ? QNetworkReply::ProxyAuthenticationRequiredError
                : QNetworkReply::AuthenticationRequiredError;
            reply->d_func()->errorString = errorDetail(errorCode, socket);
            emit q->error(errorCode, reply->d_func()->errorString);
            emit channels[i].reply->finished();
            // ### at this point the reply could be deleted
            socket->close();
            // remove pending request on the other channels
            for (int j = 0; j < channelCount; ++j) {
                if (j != i && channels[j].state ==  QHttpNetworkConnectionChannel::Wait4AuthState)
                    channels[j].state = QHttpNetworkConnectionChannel::IdleState;
            }
            return true;
        }
        //resend the request
        resend = true;
        return true;
    }
    return false;
}

void QHttpNetworkConnectionPrivate::createAuthorization(QAbstractSocket *socket, QHttpNetworkRequest &request)
{
    Q_ASSERT(socket);

    int i = indexOf(socket);

    if (channels[i].authMehtod != QAuthenticatorPrivate::None) {
        if (!(channels[i].authMehtod == QAuthenticatorPrivate::Ntlm && channels[i].lastStatus != 401)) {
            QAuthenticatorPrivate *priv = QAuthenticatorPrivate::getPrivate(channels[i].authenticator);
            if (priv && priv->method != QAuthenticatorPrivate::None) {
                QByteArray response = priv->calculateResponse(request.d->methodName(), request.d->uri(false));
                request.setHeaderField("Authorization", response);
            }
        }
    }
    if (channels[i].proxyAuthMehtod != QAuthenticatorPrivate::None) {
        if (!(channels[i].proxyAuthMehtod == QAuthenticatorPrivate::Ntlm && channels[i].lastStatus != 407)) {
            QAuthenticatorPrivate *priv = QAuthenticatorPrivate::getPrivate(channels[i].proxyAuthenticator);
            if (priv && priv->method != QAuthenticatorPrivate::None) {
                QByteArray response = priv->calculateResponse(request.d->methodName(), request.d->uri(false));
                request.setHeaderField("Proxy-Authorization", response);
            }
        }
    }
}

QHttpNetworkReply* QHttpNetworkConnectionPrivate::queueRequest(const QHttpNetworkRequest &request)
{
    Q_Q(QHttpNetworkConnection);

    // The reply component of the pair is created initially.
    QHttpNetworkReply *reply = new QHttpNetworkReply(request.url());
    reply->setRequest(request);
    reply->d_func()->connection = q;
    reply->d_func()->connectionChannel = &channels[0]; // will have the correct one set later
    HttpMessagePair pair = qMakePair(request, reply);

    switch (request.priority()) {
    case QHttpNetworkRequest::HighPriority:
        highPriorityQueue.prepend(pair);
        break;
    case QHttpNetworkRequest::NormalPriority:
    case QHttpNetworkRequest::LowPriority:
        lowPriorityQueue.prepend(pair);
        break;
    }

    // this used to be called via invokeMethod and a QueuedConnection
    // It is the only place _q_startNextRequest is called directly without going
    // through the event loop using a QueuedConnection.
    // This is dangerous because of recursion that might occur when emitting
    // signals as DirectConnection from this code path. Therefore all signal
    // emissions that can come out from this code path need to
    // be QueuedConnection.
    // We are currently trying to fine-tune this.
    _q_startNextRequest();


    return reply;
}

void QHttpNetworkConnectionPrivate::requeueRequest(const HttpMessagePair &pair)
{
    Q_Q(QHttpNetworkConnection);

    QHttpNetworkRequest request = pair.first;
    switch (request.priority()) {
    case QHttpNetworkRequest::HighPriority:
        highPriorityQueue.prepend(pair);
        break;
    case QHttpNetworkRequest::NormalPriority:
    case QHttpNetworkRequest::LowPriority:
        lowPriorityQueue.prepend(pair);
        break;
    }

    QMetaObject::invokeMethod(q, "_q_startNextRequest", Qt::QueuedConnection);
}

void QHttpNetworkConnectionPrivate::dequeueAndSendRequest(QAbstractSocket *socket)
{
    Q_ASSERT(socket);

    int i = indexOf(socket);

    if (!highPriorityQueue.isEmpty()) {
        // remove from queue before sendRequest! else we might pipeline the same request again
        HttpMessagePair messagePair = highPriorityQueue.takeLast();
        if (!messagePair.second->d_func()->requestIsPrepared)
            prepareRequest(messagePair);
        channels[i].request = messagePair.first;
        channels[i].reply = messagePair.second;
        channels[i].sendRequest();
        return;
    }

    if (!lowPriorityQueue.isEmpty()) {
        // remove from queue before sendRequest! else we might pipeline the same request again
        HttpMessagePair messagePair = lowPriorityQueue.takeLast();
        if (!messagePair.second->d_func()->requestIsPrepared)
            prepareRequest(messagePair);
        channels[i].request = messagePair.first;
        channels[i].reply = messagePair.second;
        channels[i].sendRequest();
        return;
    }
}

// this is called from _q_startNextRequest and when a request has been sent down a socket from the channel
void QHttpNetworkConnectionPrivate::fillPipeline(QAbstractSocket *socket)
{
    // return fast if there is nothing to pipeline
    if (highPriorityQueue.isEmpty() && lowPriorityQueue.isEmpty())
        return;

    int i = indexOf(socket);

    // return fast if there was no reply right now processed
    if (channels[i].reply == 0)
        return;

    if (! (defaultPipelineLength - channels[i].alreadyPipelinedRequests.length() >= defaultRePipelineLength)) {
        return;
    }

    if (channels[i].pipeliningSupported != QHttpNetworkConnectionChannel::PipeliningProbablySupported)
        return;

    // the current request that is in must already support pipelining
    if (!channels[i].request.isPipeliningAllowed())
        return;

    // the current request must be a idempotent (right now we only check GET)
    if (channels[i].request.operation() != QHttpNetworkRequest::Get)
        return;

    // check if socket is connected
    if (socket->state() != QAbstractSocket::ConnectedState)
        return;

    // check for resendCurrent
    if (channels[i].resendCurrent)
        return;

    // we do not like authentication stuff
    // ### make sure to be OK with this in later releases
    if (!channels[i].authenticator.isNull() || !channels[i].authenticator.user().isEmpty())
        return;
    if (!channels[i].proxyAuthenticator.isNull() || !channels[i].proxyAuthenticator.user().isEmpty())
        return;

    // must be in ReadingState or WaitingState
    if (! (channels[i].state == QHttpNetworkConnectionChannel::WaitingState
           || channels[i].state == QHttpNetworkConnectionChannel::ReadingState))
        return;


    //qDebug() << "QHttpNetworkConnectionPrivate::fillPipeline processing highPriorityQueue, size=" << highPriorityQueue.size() << " alreadyPipelined=" << channels[i].alreadyPipelinedRequests.length();
    int lengthBefore;
    while (!highPriorityQueue.isEmpty()) {
        lengthBefore = channels[i].alreadyPipelinedRequests.length();
        fillPipeline(highPriorityQueue, channels[i]);

        if (channels[i].alreadyPipelinedRequests.length() >= defaultPipelineLength)
            return;

        if (lengthBefore == channels[i].alreadyPipelinedRequests.length())
            break; // did not process anything, now do the low prio queue
    }

    //qDebug() << "QHttpNetworkConnectionPrivate::fillPipeline processing lowPriorityQueue, size=" << lowPriorityQueue.size() << " alreadyPipelined=" << channels[i].alreadyPipelinedRequests.length();
    while (!lowPriorityQueue.isEmpty()) {
        lengthBefore = channels[i].alreadyPipelinedRequests.length();
        fillPipeline(lowPriorityQueue, channels[i]);

        if (channels[i].alreadyPipelinedRequests.length() >= defaultPipelineLength)
            return;

        if (lengthBefore == channels[i].alreadyPipelinedRequests.length())
            break; // did not process anything
    }


}

// returns true when the processing of a queue has been done
bool QHttpNetworkConnectionPrivate::fillPipeline(QList<HttpMessagePair> &queue, QHttpNetworkConnectionChannel &channel)
{
    if (queue.isEmpty())
        return true;

    for (int i = queue.count() - 1; i >= 0; --i) {
        HttpMessagePair messagePair = queue.at(i);
        const QHttpNetworkRequest &request = messagePair.first;

        // we currently do not support pipelining if HTTP authentication is used
        if (!request.url().userInfo().isEmpty())
            continue;

        // take only GET requests
        if (request.operation() != QHttpNetworkRequest::Get)
            continue;

        if (!request.isPipeliningAllowed())
            continue;

        // remove it from the queue
        queue.takeAt(i);
        // we modify the queue we iterate over here, but since we return from the function
        // afterwards this is fine.

        // actually send it
        if (!messagePair.second->d_func()->requestIsPrepared)
            prepareRequest(messagePair);
        channel.pipelineInto(messagePair);

        // return false because we processed something and need to process again
        return false;
    }

    // return true, the queue has been processed and not changed
    return true;
}


QString QHttpNetworkConnectionPrivate::errorDetail(QNetworkReply::NetworkError errorCode, QAbstractSocket* socket,
                                                   const QString &extraDetail)
{
    Q_ASSERT(socket);

    QString errorString;
    switch (errorCode) {
    case QNetworkReply::HostNotFoundError:
        errorString = QString::fromLatin1(QT_TRANSLATE_NOOP("QHttp", "Host %1 not found"))
                              .arg(socket->peerName());
        break;
    case QNetworkReply::ConnectionRefusedError:
        errorString = QLatin1String(QT_TRANSLATE_NOOP("QHttp", "Connection refused"));
        break;
    case QNetworkReply::RemoteHostClosedError:
        errorString = QLatin1String(QT_TRANSLATE_NOOP("QHttp", "Connection closed"));
        break;
    case QNetworkReply::TimeoutError:
        errorString = QLatin1String(QT_TRANSLATE_NOOP("QAbstractSocket", "Socket operation timed out"));
        break;
    case QNetworkReply::ProxyAuthenticationRequiredError:
        errorString = QLatin1String(QT_TRANSLATE_NOOP("QHttp", "Proxy requires authentication"));
        break;
    case QNetworkReply::AuthenticationRequiredError:
        errorString = QLatin1String(QT_TRANSLATE_NOOP("QHttp", "Host requires authentication"));
        break;
    case QNetworkReply::ProtocolFailure:
        errorString = QLatin1String(QT_TRANSLATE_NOOP("QHttp", "Data corrupted"));
        break;
    case QNetworkReply::ProtocolUnknownError:
        errorString = QLatin1String(QT_TRANSLATE_NOOP("QHttp", "Unknown protocol specified"));
        break;
    case QNetworkReply::SslHandshakeFailedError:
        errorString = QLatin1String(QT_TRANSLATE_NOOP("QHttp", "SSL handshake failed"));
        break;
    default:
        // all other errors are treated as QNetworkReply::UnknownNetworkError
        errorString = extraDetail;
        break;
    }
    return errorString;
}

// this is called from the destructor of QHttpNetworkReply. It is called when
// the reply was finished correctly or when it was aborted.
void QHttpNetworkConnectionPrivate::removeReply(QHttpNetworkReply *reply)
{
    Q_Q(QHttpNetworkConnection);

    // check if the reply is currently being processed or it is pipelined in
    for (int i = 0; i < channelCount; ++i) {
        // is the reply associated the currently processing of this channel?
        if (channels[i].reply == reply) {
            channels[i].reply = 0;

            if (!reply->isFinished() && !channels[i].alreadyPipelinedRequests.isEmpty()) {
                // the reply had to be prematurely removed, e.g. it was not finished
                // therefore we have to requeue the already pipelined requests.
                channels[i].requeueCurrentlyPipelinedRequests();
            }

            // if HTTP mandates we should close
            // or the reply is not finished yet, e.g. it was aborted
            // we have to close that connection
            if (reply->d_func()->isConnectionCloseEnabled() || !reply->isFinished())
                channels[i].close();

            QMetaObject::invokeMethod(q, "_q_startNextRequest", Qt::QueuedConnection);
            return;
        }

        // is the reply inside the pipeline of this channel already?
        for (int j = 0; j < channels[i].alreadyPipelinedRequests.length(); j++) {
            if (channels[i].alreadyPipelinedRequests.at(j).second == reply) {
               // Remove that HttpMessagePair
               channels[i].alreadyPipelinedRequests.removeAt(j);

               channels[i].requeueCurrentlyPipelinedRequests();

               // Since some requests had already been pipelined, but we removed
               // one and re-queued the others
               // we must force a connection close after the request that is
               // currently in processing has been finished.
               if (channels[i].reply)
                   channels[i].reply->d_func()->forceConnectionCloseEnabled = true;

               QMetaObject::invokeMethod(q, "_q_startNextRequest", Qt::QueuedConnection);
               return;
            }
        }
    }
    // remove from the high priority queue
    if (!highPriorityQueue.isEmpty()) {
        for (int j = highPriorityQueue.count() - 1; j >= 0; --j) {
            HttpMessagePair messagePair = highPriorityQueue.at(j);
            if (messagePair.second == reply) {
                highPriorityQueue.removeAt(j);
                QMetaObject::invokeMethod(q, "_q_startNextRequest", Qt::QueuedConnection);
                return;
            }
        }
    }
    // remove from the low priority queue
    if (!lowPriorityQueue.isEmpty()) {
        for (int j = lowPriorityQueue.count() - 1; j >= 0; --j) {
            HttpMessagePair messagePair = lowPriorityQueue.at(j);
            if (messagePair.second == reply) {
                lowPriorityQueue.removeAt(j);
                QMetaObject::invokeMethod(q, "_q_startNextRequest", Qt::QueuedConnection);
                return;
            }
        }
    }
}



// This function must be called from the event loop. The only
// exception is documented in QHttpNetworkConnectionPrivate::queueRequest
// although it is called _q_startNextRequest, it will actually start multiple requests when possible
void QHttpNetworkConnectionPrivate::_q_startNextRequest()
{
    //resend the necessary ones.
    for (int i = 0; i < channelCount; ++i) {
        if (channels[i].resendCurrent) {
            channels[i].resendCurrent = false;
            channels[i].state = QHttpNetworkConnectionChannel::IdleState;

            // if this is not possible, error will be emitted and connection terminated
            if (!channels[i].resetUploadData())
                continue;
            channels[i].sendRequest();
        }
    }

    // dequeue new ones

    // return fast if there is nothing to do
    if (highPriorityQueue.isEmpty() && lowPriorityQueue.isEmpty())
        return;
    // try to get a free AND connected socket
    for (int i = 0; i < channelCount; ++i) {
        if (!channels[i].isSocketBusy() && channels[i].socket->state() == QAbstractSocket::ConnectedState) {
            dequeueAndSendRequest(channels[i].socket);
        }
    }

    // return fast if there is nothing to do
    if (highPriorityQueue.isEmpty() && lowPriorityQueue.isEmpty())
        return;
    // try to get a free unconnected socket
    for (int i = 0; i < channelCount; ++i) {
        if (!channels[i].isSocketBusy()) {
            dequeueAndSendRequest(channels[i].socket);
        }
    }

    // try to push more into all sockets
    // ### FIXME we should move this to the beginning of the function
    // as soon as QtWebkit is properly using the pipelining
    // (e.g. not for XMLHttpRequest or the first page load)
    // ### FIXME we should also divide the requests more even
    // on the connected sockets
    //tryToFillPipeline(socket);
    // return fast if there is nothing to pipeline
    if (highPriorityQueue.isEmpty() && lowPriorityQueue.isEmpty())
        return;
    for (int i = 0; i < channelCount; i++)
        if (channels[i].socket->state() == QAbstractSocket::ConnectedState)
            fillPipeline(channels[i].socket);
}

void QHttpNetworkConnectionPrivate::_q_restartAuthPendingRequests()
{
    // send the request using the idle socket
    for (int i = 0 ; i < channelCount; ++i) {
        if (channels[i].state ==  QHttpNetworkConnectionChannel::Wait4AuthState) {
            channels[i].state = QHttpNetworkConnectionChannel::IdleState;
            if (channels[i].reply)
                channels[i].sendRequest();
        }
    }
}

void QHttpNetworkConnectionPrivate::readMoreLater(QHttpNetworkReply *reply)
{
    for (int i = 0 ; i < channelCount; ++i) {
        if (channels[i].reply ==  reply) {
            // emulate a readyRead() from the socket
            QMetaObject::invokeMethod(&channels[i], "_q_readyRead", Qt::QueuedConnection);
            return;
        }
    }
}

QHttpNetworkConnection::QHttpNetworkConnection(const QString &hostName, quint16 port, bool encrypt, QObject *parent)
    : QObject(*(new QHttpNetworkConnectionPrivate(hostName, port, encrypt)), parent)
{
    Q_D(QHttpNetworkConnection);
    d->init();
}

QHttpNetworkConnection::QHttpNetworkConnection(quint16 connectionCount, const QString &hostName, quint16 port, bool encrypt, QObject *parent)
     : QObject(*(new QHttpNetworkConnectionPrivate(connectionCount, hostName, port, encrypt)), parent)
{
    Q_D(QHttpNetworkConnection);
    d->init();
}

QHttpNetworkConnection::~QHttpNetworkConnection()
{
}

QString QHttpNetworkConnection::hostName() const
{
    Q_D(const QHttpNetworkConnection);
    return d->hostName;
}

quint16 QHttpNetworkConnection::port() const
{
    Q_D(const QHttpNetworkConnection);
    return d->port;
}

QHttpNetworkReply* QHttpNetworkConnection::sendRequest(const QHttpNetworkRequest &request)
{
    Q_D(QHttpNetworkConnection);
    return d->queueRequest(request);
}

void QHttpNetworkConnection::enableEncryption()
{
    Q_D(QHttpNetworkConnection);
    d->encrypt = true;
}

bool QHttpNetworkConnection::isEncrypted() const
{
    Q_D(const QHttpNetworkConnection);
    return d->encrypt;
}

void QHttpNetworkConnection::setProxyAuthentication(QAuthenticator *authenticator)
{
    Q_D(QHttpNetworkConnection);
    for (int i = 0; i < d->channelCount; ++i)
        d->channels[i].proxyAuthenticator = *authenticator;
}

void QHttpNetworkConnection::setAuthentication(const QString &domain, QAuthenticator *authenticator)
{
    Q_UNUSED(domain); // ### domain ?
    Q_D(QHttpNetworkConnection);
    for (int i = 0; i < d->channelCount; ++i)
        d->channels[i].authenticator = *authenticator;
}

#ifndef QT_NO_NETWORKPROXY
void QHttpNetworkConnection::setCacheProxy(const QNetworkProxy &networkProxy)
{
    Q_D(QHttpNetworkConnection);
    d->networkProxy = networkProxy;
    // update the authenticator
    if (!d->networkProxy.user().isEmpty()) {
        for (int i = 0; i < d->channelCount; ++i) {
            d->channels[i].proxyAuthenticator.setUser(d->networkProxy.user());
            d->channels[i].proxyAuthenticator.setPassword(d->networkProxy.password());
        }
    }
}

QNetworkProxy QHttpNetworkConnection::cacheProxy() const
{
    Q_D(const QHttpNetworkConnection);
    return d->networkProxy;
}

void QHttpNetworkConnection::setTransparentProxy(const QNetworkProxy &networkProxy)
{
    Q_D(QHttpNetworkConnection);
    for (int i = 0; i < d->channelCount; ++i)
        d->channels[i].socket->setProxy(networkProxy);
}

QNetworkProxy QHttpNetworkConnection::transparentProxy() const
{
    Q_D(const QHttpNetworkConnection);
    return d->channels[0].socket->proxy();
}
#endif


// SSL support below
#ifndef QT_NO_OPENSSL
void QHttpNetworkConnection::setSslConfiguration(const QSslConfiguration &config)
{
    Q_D(QHttpNetworkConnection);
    if (!d->encrypt)
        return;

    // set the config on all channels
    for (int i = 0; i < d->channelCount; ++i)
        static_cast<QSslSocket *>(d->channels[i].socket)->setSslConfiguration(config);
}

void QHttpNetworkConnection::ignoreSslErrors(int channel)
{
    Q_D(QHttpNetworkConnection);
    if (!d->encrypt)
        return;

    if (channel == -1) { // ignore for all channels
        for (int i = 0; i < d->channelCount; ++i) {
            static_cast<QSslSocket *>(d->channels[i].socket)->ignoreSslErrors();
            d->channels[i].ignoreAllSslErrors = true;
        }

    } else {
        static_cast<QSslSocket *>(d->channels[channel].socket)->ignoreSslErrors();
        d->channels[channel].ignoreAllSslErrors = true;
    }
}

void QHttpNetworkConnection::ignoreSslErrors(const QList<QSslError> &errors, int channel)
{
    Q_D(QHttpNetworkConnection);
    if (!d->encrypt)
        return;

    if (channel == -1) { // ignore for all channels
        for (int i = 0; i < d->channelCount; ++i) {
            static_cast<QSslSocket *>(d->channels[i].socket)->ignoreSslErrors(errors);
            d->channels[i].ignoreSslErrorsList = errors;
        }

    } else {
        static_cast<QSslSocket *>(d->channels[channel].socket)->ignoreSslErrors(errors);
        d->channels[channel].ignoreSslErrorsList = errors;
    }
}

#endif //QT_NO_OPENSSL

#ifndef QT_NO_NETWORKPROXY
// only called from QHttpNetworkConnectionChannel::_q_proxyAuthenticationRequired, not
// from QHttpNetworkConnectionChannel::handleAuthenticationChallenge
// e.g. it is for SOCKS proxies which require authentication.
void QHttpNetworkConnectionPrivate::emitProxyAuthenticationRequired(const QHttpNetworkConnectionChannel *chan, const QNetworkProxy &proxy, QAuthenticator* auth)
{
    Q_Q(QHttpNetworkConnection);
    emit q->proxyAuthenticationRequired(proxy, auth, q);
    int i = indexOf(chan->socket);
    copyCredentials(i, auth, true);
}
#endif


QT_END_NAMESPACE

#include "moc_qhttpnetworkconnection_p.cpp"

#endif // QT_NO_HTTP
