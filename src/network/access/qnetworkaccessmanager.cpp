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

#include "qnetworkaccessmanager.h"
#include "qnetworkaccessmanager_p.h"
#include "qnetworkrequest.h"
#include "qnetworkreply.h"
#include "qnetworkreply_p.h"
#include "qnetworkcookie.h"
#include "qabstractnetworkcache.h"

#include "QtNetwork/qnetworksession.h"

#include "qnetworkaccesshttpbackend_p.h"
#include "qnetworkaccessftpbackend_p.h"
#include "qnetworkaccessfilebackend_p.h"
#include "qnetworkaccessdatabackend_p.h"
#include "qnetworkaccessdebugpipebackend_p.h"
#include "qfilenetworkreply_p.h"

#include "QtCore/qbuffer.h"
#include "QtCore/qurl.h"
#include "QtCore/qvector.h"
#include "QtNetwork/qauthenticator.h"
#include "QtNetwork/qsslconfiguration.h"
#include "QtNetwork/qnetworkconfigmanager.h"

QT_BEGIN_NAMESPACE

#ifndef QT_NO_HTTP
Q_GLOBAL_STATIC(QNetworkAccessHttpBackendFactory, httpBackend)
#endif // QT_NO_HTTP
Q_GLOBAL_STATIC(QNetworkAccessFileBackendFactory, fileBackend)
Q_GLOBAL_STATIC(QNetworkAccessDataBackendFactory, dataBackend)
#ifndef QT_NO_FTP
Q_GLOBAL_STATIC(QNetworkAccessFtpBackendFactory, ftpBackend)
#endif // QT_NO_FTP

#ifdef QT_BUILD_INTERNAL
Q_GLOBAL_STATIC(QNetworkAccessDebugPipeBackendFactory, debugpipeBackend)
#endif

static void ensureInitialized()
{
#ifndef QT_NO_HTTP
    (void) httpBackend();
#endif // QT_NO_HTTP
    (void) dataBackend();
#ifndef QT_NO_FTP
    (void) ftpBackend();
#endif

#ifdef QT_BUILD_INTERNAL
    (void) debugpipeBackend();
#endif

    // leave this one last since it will query the special QAbstractFileEngines
    (void) fileBackend();
}

/*!
    \class QNetworkAccessManager
    \brief The QNetworkAccessManager class allows the application to
    send network requests and receive replies
    \since 4.4

    \ingroup network
    \inmodule QtNetwork
    \reentrant

    The Network Access API is constructed around one QNetworkAccessManager
    object, which holds the common configuration and settings for the requests
    it sends. It contains the proxy and cache configuration, as well as the
    signals related to such issues, and reply signals that can be used to
    monitor the progress of a network operation. One QNetworkAccessManager
    should be enough for the whole Qt application.

    Once a QNetworkAccessManager object has been created, the application can
    use it to send requests over the network. A group of standard functions
    are supplied that take a request and optional data, and each return a
    QNetworkReply object. The returned object is used to obtain any data
    returned in response to the corresponding request.

    A simple download off the network could be accomplished with:
    \snippet doc/src/snippets/code/src_network_access_qnetworkaccessmanager.cpp 0

    QNetworkAccessManager has an asynchronous API.
    When the \tt replyFinished slot above is called, the parameter it
    takes is the QNetworkReply object containing the downloaded data
    as well as meta-data (headers, etc.).

    \note After the request has finished, it is the responsibility of the user
    to delete the QNetworkReply object at an appropriate time. Do not directly
    delete it inside the slot connected to finished(). You can use the
    deleteLater() function.

    \note QNetworkAccessManager queues the requests it receives. The number
    of requests executed in parallel is dependent on the protocol.
    Currently, for the HTTP protocol on desktop platforms, 6 requests are
    executed in parallel for one host/port combination.

    A more involved example, assuming the manager is already existent,
    can be:
    \snippet doc/src/snippets/code/src_network_access_qnetworkaccessmanager.cpp 1

    \section1 Network and Roaming support

    With the addition of the \l {Bearer Management} API to Qt 4.7
    QNetworkAccessManager gained the ability to manage network connections.
    QNetworkAccessManager can start the network interface if the device is
    offline and terminates the interface if the current process is the last
    one to use the uplink. Note that some platform utilize grace periods from
    when the last application stops using a uplink until the system actually
    terminates the connectivity link. Roaming is equally transparent. Any
    queued/pending network requests are automatically transferred to new
    access point.

    Clients wanting to utilize this feature should not require any changes. In fact
    it is likely that existing platform specific connection code can simply be
    removed from the application.

    \note The network and roaming support in QNetworkAccessManager is conditional
    upon the platform supporting connection management. The
    \l QNetworkConfigurationManager::NetworkSessionRequired can be used to
    detect whether QNetworkAccessManager utilizes this feature. Currently only
    Meego/Harmattan and Symbian platforms provide connection management support.

    \note This feature cannot be used in combination with the Bearer Management
    API as provided by QtMobility. Applications have to migrate to the Qt version
    of Bearer Management.

    \section1 Symbian Platform Security Requirements

    On Symbian, processes which use this class must have the
    \c NetworkServices platform security capability. If the client
    process lacks this capability, operations will result in a panic.

    Platform security capabilities are added via the
    \l{qmake-variable-reference.html#target-capability}{TARGET.CAPABILITY}
    qmake variable.

    \sa QNetworkRequest, QNetworkReply, QNetworkProxy
*/

/*!
    \enum QNetworkAccessManager::Operation

    Indicates the operation this reply is processing.

    \value HeadOperation        retrieve headers operation (created
    with head())

    \value GetOperation         retrieve headers and download contents
    (created with get())

    \value PutOperation         upload contents operation (created
    with put())

    \value PostOperation        send the contents of an HTML form for
    processing via HTTP POST (created with post())

    \value DeleteOperation      delete contents operation (created with
    deleteResource())

    \value CustomOperation      custom operation (created with
    sendCustomRequest())

    \omitvalue UnknownOperation

    \sa QNetworkReply::operation()
*/

/*!
    \enum QNetworkAccessManager::NetworkAccessibility

    Indicates whether the network is accessible via this network access manager.

    \value UnknownAccessibility     The network accessibility cannot be determined.
    \value NotAccessible            The network is not currently accessible, either because there
                                    is currently no network coverage or network access has been
                                    explicitly disabled by a call to setNetworkAccessible().
    \value Accessible               The network is accessible.

    \sa networkAccessible
*/

/*!
    \property QNetworkAccessManager::networkAccessible
    \brief whether the network is currently accessible via this network access manager.

    \since 4.7

    If the network is \l {NotAccessible}{not accessible} the network access manager will not
    process any new network requests, all such requests will fail with an error.  Requests with
    URLs with the file:// scheme will still be processed.

    By default the value of this property reflects the physical state of the device.  Applications
    may override it to disable all network requests via this network access manager by calling

    \snippet doc/src/snippets/code/src_network_access_qnetworkaccessmanager.cpp 4

    Network requests can be reenabled again by calling

    \snippet doc/src/snippets/code/src_network_access_qnetworkaccessmanager.cpp 5

    \note Calling setNetworkAccessible() does not change the network state.
*/

/*!
    \fn void QNetworkAccessManager::networkAccessibleChanged(QNetworkAccessManager::NetworkAccessibility accessible)

    This signal is emitted when the value of the \l networkAccessible property changes.
    \a accessible is the new network accessibility.
*/

/*!
    \fn void QNetworkAccessManager::networkSessionConnected()

    \since 4.7

    \internal

    This signal is emitted when the status of the network session changes into a usable (Connected)
    state. It is used to signal to QNetworkReplys to start or migrate their network operation once
    the network session has been opened or finished roaming.
*/

/*!
    \fn void QNetworkAccessManager::proxyAuthenticationRequired(const QNetworkProxy &proxy, QAuthenticator *authenticator)

    This signal is emitted whenever a proxy requests authentication
    and QNetworkAccessManager cannot find a valid, cached
    credential. The slot connected to this signal should fill in the
    credentials for the proxy \a proxy in the \a authenticator object.

    QNetworkAccessManager will cache the credentials internally. The
    next time the proxy requests authentication, QNetworkAccessManager
    will automatically send the same credential without emitting the
    proxyAuthenticationRequired signal again.

    If the proxy rejects the credentials, QNetworkAccessManager will
    emit the signal again.

    \sa proxy(), setProxy(), authenticationRequired()
*/

/*!
    \fn void QNetworkAccessManager::authenticationRequired(QNetworkReply *reply, QAuthenticator *authenticator)

    This signal is emitted whenever a final server requests
    authentication before it delivers the requested contents. The slot
    connected to this signal should fill the credentials for the
    contents (which can be determined by inspecting the \a reply
    object) in the \a authenticator object.

    QNetworkAccessManager will cache the credentials internally and
    will send the same values if the server requires authentication
    again, without emitting the authenticationRequired() signal. If it
    rejects the credentials, this signal will be emitted again.

    \sa proxyAuthenticationRequired()
*/

/*!
    \fn void QNetworkAccessManager::finished(QNetworkReply *reply)

    This signal is emitted whenever a pending network reply is
    finished. The \a reply parameter will contain a pointer to the
    reply that has just finished. This signal is emitted in tandem
    with the QNetworkReply::finished() signal.

    See QNetworkReply::finished() for information on the status that
    the object will be in.

    \note Do not delete the \a reply object in the slot connected to this
    signal. Use deleteLater().

    \sa QNetworkReply::finished(), QNetworkReply::error()
*/

/*!
    \fn void QNetworkAccessManager::sslErrors(QNetworkReply *reply, const QList<QSslError> &errors)

    This signal is emitted if the SSL/TLS session encountered errors
    during the set up, including certificate verification errors. The
    \a errors parameter contains the list of errors and \a reply is
    the QNetworkReply that is encountering these errors.

    To indicate that the errors are not fatal and that the connection
    should proceed, the QNetworkReply::ignoreSslErrors() function should be called
    from the slot connected to this signal. If it is not called, the
    SSL session will be torn down before any data is exchanged
    (including the URL).

    This signal can be used to display an error message to the user
    indicating that security may be compromised and display the
    SSL settings (see sslConfiguration() to obtain it). If the user
    decides to proceed after analyzing the remote certificate, the
    slot should call ignoreSslErrors().

    \sa QSslSocket::sslErrors(), QNetworkReply::sslErrors(),
    QNetworkReply::sslConfiguration(), QNetworkReply::ignoreSslErrors()
*/

class QNetworkAuthenticationCredential
{
public:
    QString domain;
    QString user;
    QString password;
};
Q_DECLARE_TYPEINFO(QNetworkAuthenticationCredential, Q_MOVABLE_TYPE);
inline bool operator<(const QNetworkAuthenticationCredential &t1, const QString &t2)
{ return t1.domain < t2; }

class QNetworkAuthenticationCache: private QVector<QNetworkAuthenticationCredential>,
                                   public QNetworkAccessCache::CacheableObject
{
public:
    QNetworkAuthenticationCache()
    {
        setExpires(false);
        setShareable(true);
        reserve(1);
    }

    QNetworkAuthenticationCredential *findClosestMatch(const QString &domain)
    {
        iterator it = qLowerBound(begin(), end(), domain);
        if (it == end() && !isEmpty())
            --it;
        if (it == end() || !domain.startsWith(it->domain))
            return 0;
        return &*it;
    }

    void insert(const QString &domain, const QString &user, const QString &password)
    {
        QNetworkAuthenticationCredential *closestMatch = findClosestMatch(domain);
        if (closestMatch && closestMatch->domain == domain) {
            // we're overriding the current credentials
            closestMatch->user = user;
            closestMatch->password = password;
        } else {
            QNetworkAuthenticationCredential newCredential;
            newCredential.domain = domain;
            newCredential.user = user;
            newCredential.password = password;

            if (closestMatch)
                QVector<QNetworkAuthenticationCredential>::insert(++closestMatch, newCredential);
            else
                QVector<QNetworkAuthenticationCredential>::insert(end(), newCredential);
        }
    }

    virtual void dispose() { delete this; }
};

#ifndef QT_NO_NETWORKPROXY
static QByteArray proxyAuthenticationKey(const QNetworkProxy &proxy, const QString &realm)
{
    QUrl key;

    switch (proxy.type()) {
    case QNetworkProxy::Socks5Proxy:
        key.setScheme(QLatin1String("proxy-socks5"));
        break;

    case QNetworkProxy::HttpProxy:
    case QNetworkProxy::HttpCachingProxy:
        key.setScheme(QLatin1String("proxy-http"));
        break;

    case QNetworkProxy::FtpCachingProxy:
        key.setScheme(QLatin1String("proxy-ftp"));
        break;

    case QNetworkProxy::DefaultProxy:
    case QNetworkProxy::NoProxy:
        // shouldn't happen
        return QByteArray();

        // no default:
        // let there be errors if a new proxy type is added in the future
    }

    if (key.scheme().isEmpty())
        // proxy type not handled
        return QByteArray();

    key.setUserName(proxy.user());
    key.setHost(proxy.hostName());
    key.setPort(proxy.port());
    key.setFragment(realm);
    return "auth:" + key.toEncoded();
}
#endif

static inline QByteArray authenticationKey(const QUrl &url, const QString &realm)
{
    QUrl copy = url;
    copy.setFragment(realm);
    return "auth:" + copy.toEncoded(QUrl::RemovePassword | QUrl::RemovePath | QUrl::RemoveQuery);
}

/*!
    Constructs a QNetworkAccessManager object that is the center of
    the Network Access API and sets \a parent as the parent object.
*/
QNetworkAccessManager::QNetworkAccessManager(QObject *parent)
    : QObject(*new QNetworkAccessManagerPrivate, parent)
{
    ensureInitialized();
}

/*!
    Destroys the QNetworkAccessManager object and frees up any
    resources. Note that QNetworkReply objects that are returned from
    this class have this object set as their parents, which means that
    they will be deleted along with it if you don't call
    QObject::setParent() on them.
*/
QNetworkAccessManager::~QNetworkAccessManager()
{
#ifndef QT_NO_NETWORKPROXY
    delete d_func()->proxyFactory;
#endif

    // Delete the QNetworkReply children first.
    // Else a QAbstractNetworkCache might get deleted in ~QObject
    // before a QNetworkReply that accesses the QAbstractNetworkCache
    // object in its destructor.
    qDeleteAll(findChildren<QNetworkReply *>());
    // The other children will be deleted in this ~QObject
    // FIXME instead of this "hack" make the QNetworkReplyImpl
    // properly watch the cache deletion, e.g. via a QWeakPointer.
}

#ifndef QT_NO_NETWORKPROXY
/*!
    Returns the QNetworkProxy that the requests sent using this
    QNetworkAccessManager object will use. The default value for the
    proxy is QNetworkProxy::DefaultProxy.

    \sa setProxy(), setProxyFactory(), proxyAuthenticationRequired()
*/
QNetworkProxy QNetworkAccessManager::proxy() const
{
    return d_func()->proxy;
}

/*!
    Sets the proxy to be used in future requests to be \a proxy. This
    does not affect requests that have already been sent. The
    proxyAuthenticationRequired() signal will be emitted if the proxy
    requests authentication.

    A proxy set with this function will be used for all requests
    issued by QNetworkAccessManager. In some cases, it might be
    necessary to select different proxies depending on the type of
    request being sent or the destination host. If that's the case,
    you should consider using setProxyFactory().

    \sa proxy(), proxyAuthenticationRequired()
*/
void QNetworkAccessManager::setProxy(const QNetworkProxy &proxy)
{
    Q_D(QNetworkAccessManager);
    delete d->proxyFactory;
    d->proxy = proxy;
    d->proxyFactory = 0;
}

/*!
    \fn QNetworkProxyFactory *QNetworkAccessManager::proxyFactory() const
    \since 4.5

    Returns the proxy factory that this QNetworkAccessManager object
    is using to determine the proxies to be used for requests.

    Note that the pointer returned by this function is managed by
    QNetworkAccessManager and could be deleted at any time.

    \sa setProxyFactory(), proxy()
*/
QNetworkProxyFactory *QNetworkAccessManager::proxyFactory() const
{
    return d_func()->proxyFactory;
}

/*!
    \since 4.5

    Sets the proxy factory for this class to be \a factory. A proxy
    factory is used to determine a more specific list of proxies to be
    used for a given request, instead of trying to use the same proxy
    value for all requests.

    All queries sent by QNetworkAccessManager will have type
    QNetworkProxyQuery::UrlRequest.

    For example, a proxy factory could apply the following rules:
    \list
      \o if the target address is in the local network (for example,
         if the hostname contains no dots or if it's an IP address in
         the organization's range), return QNetworkProxy::NoProxy
      \o if the request is FTP, return an FTP proxy
      \o if the request is HTTP or HTTPS, then return an HTTP proxy
      \o otherwise, return a SOCKSv5 proxy server
    \endlist

    The lifetime of the object \a factory will be managed by
    QNetworkAccessManager. It will delete the object when necessary.

    \note If a specific proxy is set with setProxy(), the factory will not
    be used.

    \sa proxyFactory(), setProxy(), QNetworkProxyQuery
*/
void QNetworkAccessManager::setProxyFactory(QNetworkProxyFactory *factory)
{
    Q_D(QNetworkAccessManager);
    delete d->proxyFactory;
    d->proxyFactory = factory;
    d->proxy = QNetworkProxy();
}
#endif

/*!
    \since 4.5

    Returns the cache that is used to store data obtained from the network.

    \sa setCache()
*/
QAbstractNetworkCache *QNetworkAccessManager::cache() const
{
    Q_D(const QNetworkAccessManager);
    return d->networkCache;
}

/*!
    \since 4.5

    Sets the manager's network cache to be the \a cache specified. The cache
    is used for all requests dispatched by the manager.

    Use this function to set the network cache object to a class that implements
    additional features, like saving the cookies to permanent storage.

    \note QNetworkAccessManager takes ownership of the \a cache object.

    QNetworkAccessManager by default does not have a set cache.
    Qt provides a simple disk cache, QNetworkDiskCache, which can be used.

    \sa cache(), QNetworkRequest::CacheLoadControl
*/
void QNetworkAccessManager::setCache(QAbstractNetworkCache *cache)
{
    Q_D(QNetworkAccessManager);
    if (d->networkCache != cache) {
        delete d->networkCache;
        d->networkCache = cache;
        if (d->networkCache)
            d->networkCache->setParent(this);
    }
}

/*!
    Returns the QNetworkCookieJar that is used to store cookies
    obtained from the network as well as cookies that are about to be
    sent.

    \sa setCookieJar()
*/
QNetworkCookieJar *QNetworkAccessManager::cookieJar() const
{
    Q_D(const QNetworkAccessManager);
    if (!d->cookieJar)
        d->createCookieJar();
    return d->cookieJar;
}

/*!
    Sets the manager's cookie jar to be the \a cookieJar specified.
    The cookie jar is used by all requests dispatched by the manager.

    Use this function to set the cookie jar object to a class that
    implements additional features, like saving the cookies to permanent
    storage.

    \note QNetworkAccessManager takes ownership of the \a cookieJar object.

    If \a cookieJar is in the same thread as this QNetworkAccessManager,
    it will set the parent of the \a cookieJar
    so that the cookie jar is deleted when this
    object is deleted as well. If you want to share cookie jars
    between different QNetworkAccessManager objects, you may want to
    set the cookie jar's parent to 0 after calling this function.

    QNetworkAccessManager by default does not implement any cookie
    policy of its own: it accepts all cookies sent by the server, as
    long as they are well formed and meet the minimum security
    requirements (cookie domain matches the request's and cookie path
    matches the request's). In order to implement your own security
    policy, override the QNetworkCookieJar::cookiesForUrl() and
    QNetworkCookieJar::setCookiesFromUrl() virtual functions. Those
    functions are called by QNetworkAccessManager when it detects a
    new cookie.

    \sa cookieJar(), QNetworkCookieJar::cookiesForUrl(), QNetworkCookieJar::setCookiesFromUrl()
*/
void QNetworkAccessManager::setCookieJar(QNetworkCookieJar *cookieJar)
{
    Q_D(QNetworkAccessManager);
    d->cookieJarCreated = true;
    if (d->cookieJar != cookieJar) {
        if (d->cookieJar && d->cookieJar->parent() == this)
            delete d->cookieJar;
        d->cookieJar = cookieJar;
        if (thread() == cookieJar->thread())
            d->cookieJar->setParent(this);
    }
}

/*!
    Posts a request to obtain the network headers for \a request
    and returns a new QNetworkReply object which will contain such headers.

    The function is named after the HTTP request associated (HEAD).
*/
QNetworkReply *QNetworkAccessManager::head(const QNetworkRequest &request)
{
    return d_func()->postProcess(createRequest(QNetworkAccessManager::HeadOperation, request));
}

/*!
    Posts a request to obtain the contents of the target \a request
    and returns a new QNetworkReply object opened for reading which emits the 
    \l{QIODevice::readyRead()}{readyRead()} signal whenever new data 
    arrives.

    The contents as well as associated headers will be downloaded.

    \sa post(), put(), deleteResource(), sendCustomRequest()
*/
QNetworkReply *QNetworkAccessManager::get(const QNetworkRequest &request)
{
    return d_func()->postProcess(createRequest(QNetworkAccessManager::GetOperation, request));
}

/*!
    Sends an HTTP POST request to the destination specified by \a request
    and returns a new QNetworkReply object opened for reading that will 
    contain the reply sent by the server. The contents of  the \a data 
    device will be uploaded to the server.

    \a data must be open for reading and must remain valid until the 
    finished() signal is emitted for this reply.

    \note Sending a POST request on protocols other than HTTP and
    HTTPS is undefined and will probably fail.

    \sa get(), put(), deleteResource(), sendCustomRequest()
*/
QNetworkReply *QNetworkAccessManager::post(const QNetworkRequest &request, QIODevice *data)
{
    return d_func()->postProcess(createRequest(QNetworkAccessManager::PostOperation, request, data));
}

/*!
    \overload

    Sends the contents of the \a data byte array to the destination 
    specified by \a request.
*/
QNetworkReply *QNetworkAccessManager::post(const QNetworkRequest &request, const QByteArray &data)
{
    QBuffer *buffer = new QBuffer;
    buffer->setData(data);
    buffer->open(QIODevice::ReadOnly);

    QNetworkReply *reply = post(request, buffer);
    buffer->setParent(reply);
    return reply;
}

/*!
    Uploads the contents of \a data to the destination \a request and
    returnes a new QNetworkReply object that will be open for reply.

    \a data must be opened for reading when this function is called
    and must remain valid until the finished() signal is emitted for
    this reply.

    Whether anything will be available for reading from the returned
    object is protocol dependent. For HTTP, the server may send a 
    small HTML page indicating the upload was successful (or not). 
    Other protocols will probably have content in their replies.

    \note For HTTP, this request will send a PUT request, which most servers
    do not allow. Form upload mechanisms, including that of uploading
    files through HTML forms, use the POST mechanism.

    \sa get(), post(), deleteResource(), sendCustomRequest()
*/
QNetworkReply *QNetworkAccessManager::put(const QNetworkRequest &request, QIODevice *data)
{
    return d_func()->postProcess(createRequest(QNetworkAccessManager::PutOperation, request, data));
}

/*!
    \overload
    Sends the contents of the \a data byte array to the destination 
    specified by \a request.
*/
QNetworkReply *QNetworkAccessManager::put(const QNetworkRequest &request, const QByteArray &data)
{
    QBuffer *buffer = new QBuffer;
    buffer->setData(data);
    buffer->open(QIODevice::ReadOnly);

    QNetworkReply *reply = put(request, buffer);
    buffer->setParent(reply);
    return reply;
}

/*!
    \since 4.6

    Sends a request to delete the resource identified by the URL of \a request.

    \note This feature is currently available for HTTP only, performing an 
    HTTP DELETE request.

    \sa get(), post(), put(), sendCustomRequest()
*/
QNetworkReply *QNetworkAccessManager::deleteResource(const QNetworkRequest &request)
{
    return d_func()->postProcess(createRequest(QNetworkAccessManager::DeleteOperation, request));
}

#ifndef QT_NO_BEARERMANAGEMENT

/*!
    \since 4.7

    Sets the network configuration that will be used when creating the
    \l {QNetworkSession}{network session} to \a config.

    The network configuration is used to create and open a network session before any request that
    requires network access is process.  If no network configuration is explicitly set via this
    function the network configuration returned by
    QNetworkConfigurationManager::defaultConfiguration() will be used.

    To restore the default network configuration set the network configuration to the value
    returned from QNetworkConfigurationManager::defaultConfiguration().

    \snippet doc/src/snippets/code/src_network_access_qnetworkaccessmanager.cpp 2

    If an invalid network configuration is set, a network session will not be created.  In this
    case network requests will be processed regardless, but may fail.  For example:

    \snippet doc/src/snippets/code/src_network_access_qnetworkaccessmanager.cpp 3

    \sa configuration(), QNetworkSession
*/
void QNetworkAccessManager::setConfiguration(const QNetworkConfiguration &config)
{
    d_func()->createSession(config);
}

/*!
    \since 4.7

    Returns the network configuration that will be used to create the
    \l {QNetworkSession}{network session} which will be used when processing network requests.

    \sa setConfiguration(), activeConfiguration()
*/
QNetworkConfiguration QNetworkAccessManager::configuration() const
{
    Q_D(const QNetworkAccessManager);

    if (d->networkSession)
        return d->networkSession->configuration();
    else
        return QNetworkConfiguration();
}

/*!
    \since 4.7

    Returns the current active network configuration.

    If the network configuration returned by configuration() is of type
    QNetworkConfiguration::ServiceNetwork this function will return the current active child
    network configuration of that configuration.  Otherwise returns the same network configuration
    as configuration().

    Use this function to return the actual network configuration currently in use by the network
    session.

    \sa configuration()
*/
QNetworkConfiguration QNetworkAccessManager::activeConfiguration() const
{
    Q_D(const QNetworkAccessManager);

    if (d->networkSession) {
        QNetworkConfigurationManager manager;

        return manager.configurationFromIdentifier(
            d->networkSession->sessionProperty(QLatin1String("ActiveConfiguration")).toString());
    } else {
        return QNetworkConfiguration();
    }
}

/*!
    \since 4.7

    Overrides the reported network accessibility.  If \a accessible is NotAccessible the reported
    network accessiblity will always be NotAccessible.  Otherwise the reported network
    accessibility will reflect the actual device state.
*/
void QNetworkAccessManager::setNetworkAccessible(QNetworkAccessManager::NetworkAccessibility accessible)
{
    Q_D(QNetworkAccessManager);

    if (d->networkAccessible != accessible) {
        NetworkAccessibility previous = networkAccessible();
        d->networkAccessible = accessible;
        NetworkAccessibility current = networkAccessible();
        if (previous != current)
            emit networkAccessibleChanged(current);
    }
}

/*!
    \since 4.7

    Returns the current network accessibility.
*/
QNetworkAccessManager::NetworkAccessibility QNetworkAccessManager::networkAccessible() const
{
    Q_D(const QNetworkAccessManager);

    if (d->networkSession) {
        // d->online holds online/offline state of this network session.
        if (d->online)
            return d->networkAccessible;
        else
            return NotAccessible;
    } else {
        // Network accessibility is either disabled or unknown.
        return (d->networkAccessible == NotAccessible) ? NotAccessible : UnknownAccessibility;
    }
}

#endif // QT_NO_BEARERMANAGEMENT

/*!
    \since 4.7

    Sends a custom request to the server identified by the URL of \a request.

    It is the user's responsibility to send a \a verb to the server that is valid
    according to the HTTP specification.

    This method provides means to send verbs other than the common ones provided
    via get() or post() etc., for instance sending an HTTP OPTIONS command.

    If \a data is not empty, the contents of the \a data
    device will be uploaded to the server; in that case, data must be open for
    reading and must remain valid until the finished() signal is emitted for this reply.

    \note This feature is currently available for HTTP only.

    \sa get(), post(), put(), deleteResource()
*/
QNetworkReply *QNetworkAccessManager::sendCustomRequest(const QNetworkRequest &request, const QByteArray &verb, QIODevice *data)
{
    QNetworkRequest newRequest(request);
    newRequest.setAttribute(QNetworkRequest::CustomVerbAttribute, verb);
    return d_func()->postProcess(createRequest(QNetworkAccessManager::CustomOperation, newRequest, data));
}

/*!
    Returns a new QNetworkReply object to handle the operation \a op
    and request \a req. The device \a outgoingData is always 0 for Get and
    Head requests, but is the value passed to post() and put() in
    those operations (the QByteArray variants will pass a QBuffer
    object).

    The default implementation calls QNetworkCookieJar::cookiesForUrl()
    on the cookie jar set with setCookieJar() to obtain the cookies to
    be sent to the remote server.

    The returned object must be in an open state.
*/
QNetworkReply *QNetworkAccessManager::createRequest(QNetworkAccessManager::Operation op,
                                                    const QNetworkRequest &req,
                                                    QIODevice *outgoingData)
{
    Q_D(QNetworkAccessManager);

    // fast path for GET on file:// URLs
    // Also if the scheme is empty we consider it a file.
    // The QNetworkAccessFileBackend will right now only be used
    // for PUT or qrc://
    if ((op == QNetworkAccessManager::GetOperation || op == QNetworkAccessManager::HeadOperation)
         && (req.url().scheme() == QLatin1String("file")
             || req.url().scheme().isEmpty())) {
        return new QFileNetworkReply(this, req, op);
    }

#ifndef QT_NO_BEARERMANAGEMENT
    // Return a disabled network reply if network access is disabled.
    // Except if the scheme is empty or file://.
    if (!d->networkAccessible && !(req.url().scheme() == QLatin1String("file") ||
                                      req.url().scheme().isEmpty())) {
        return new QDisabledNetworkReply(this, req, op);
    }

    if (!d->networkSession && (d->initializeSession || !d->networkConfiguration.isEmpty())) {
        QNetworkConfigurationManager manager;
        if (!d->networkConfiguration.isEmpty()) {
            d->createSession(manager.configurationFromIdentifier(d->networkConfiguration));
        } else {
            if (manager.capabilities() & QNetworkConfigurationManager::NetworkSessionRequired)
                d->createSession(manager.defaultConfiguration());
            else
                d->initializeSession = false;
        }
    }

    if (d->networkSession)
        d->networkSession->setSessionProperty(QLatin1String("AutoCloseSessionTimeout"), -1);
#endif

    QNetworkRequest request = req;
    if (!request.header(QNetworkRequest::ContentLengthHeader).isValid() &&
        outgoingData && !outgoingData->isSequential()) {
        // request has no Content-Length
        // but the data that is outgoing is random-access
        request.setHeader(QNetworkRequest::ContentLengthHeader, outgoingData->size());
    }

    if (static_cast<QNetworkRequest::LoadControl>
        (request.attribute(QNetworkRequest::CookieLoadControlAttribute,
                           QNetworkRequest::Automatic).toInt()) == QNetworkRequest::Automatic) {
        if (d->cookieJar) {
            QList<QNetworkCookie> cookies = d->cookieJar->cookiesForUrl(request.url());
            if (!cookies.isEmpty())
                request.setHeader(QNetworkRequest::CookieHeader, qVariantFromValue(cookies));
        }
    }

    // first step: create the reply
    QUrl url = request.url();
    QNetworkReplyImpl *reply = new QNetworkReplyImpl(this);
#ifndef QT_NO_BEARERMANAGEMENT
    if (req.url().scheme() != QLatin1String("file") && !req.url().scheme().isEmpty()) {
        connect(this, SIGNAL(networkSessionConnected()),
                reply, SLOT(_q_networkSessionConnected()));
    }
#endif
    QNetworkReplyImplPrivate *priv = reply->d_func();
    priv->manager = this;

    // second step: fetch cached credentials
    if (static_cast<QNetworkRequest::LoadControl>
        (request.attribute(QNetworkRequest::AuthenticationReuseAttribute,
                           QNetworkRequest::Automatic).toInt()) == QNetworkRequest::Automatic) {
        QNetworkAuthenticationCredential *cred = d->fetchCachedCredentials(url);
        if (cred) {
            url.setUserName(cred->user);
            url.setPassword(cred->password);
            priv->urlForLastAuthentication = url;
        }
    }

    // third step: find a backend
    priv->backend = d->findBackend(op, request);

#ifndef QT_NO_NETWORKPROXY
    QList<QNetworkProxy> proxyList = d->queryProxy(QNetworkProxyQuery(request.url()));
    priv->proxyList = proxyList;
#endif
    if (priv->backend) {
        priv->backend->setParent(reply);
        priv->backend->reply = priv;
    }
    // fourth step: setup the reply
    priv->setup(op, request, outgoingData);

#ifndef QT_NO_OPENSSL
    reply->setSslConfiguration(request.sslConfiguration());
#endif
    return reply;
}

void QNetworkAccessManagerPrivate::_q_replyFinished()
{
    Q_Q(QNetworkAccessManager);

    QNetworkReply *reply = qobject_cast<QNetworkReply *>(q->sender());
    if (reply)
        emit q->finished(reply);

#ifndef QT_NO_BEARERMANAGEMENT
    if (networkSession && q->findChildren<QNetworkReply *>().count() == 1)
        networkSession->setSessionProperty(QLatin1String("AutoCloseSessionTimeout"), 120000);
#endif
}

void QNetworkAccessManagerPrivate::_q_replySslErrors(const QList<QSslError> &errors)
{
#ifndef QT_NO_OPENSSL
    Q_Q(QNetworkAccessManager);
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(q->sender());
    if (reply)
        emit q->sslErrors(reply, errors);
#else
    Q_UNUSED(errors);
#endif
}

QNetworkReply *QNetworkAccessManagerPrivate::postProcess(QNetworkReply *reply)
{
    Q_Q(QNetworkAccessManager);
    QNetworkReplyPrivate::setManager(reply, q);
    q->connect(reply, SIGNAL(finished()), SLOT(_q_replyFinished()));
#ifndef QT_NO_OPENSSL
    /* In case we're compiled without SSL support, we don't have this signal and we need to
     * avoid getting a connection error. */
    q->connect(reply, SIGNAL(sslErrors(QList<QSslError>)), SLOT(_q_replySslErrors(QList<QSslError>)));
#endif

    return reply;
}

void QNetworkAccessManagerPrivate::createCookieJar() const
{
    if (!cookieJarCreated) {
        // keep the ugly hack in here
        QNetworkAccessManagerPrivate *that = const_cast<QNetworkAccessManagerPrivate *>(this);
        that->cookieJar = new QNetworkCookieJar(that->q_func());
        that->cookieJarCreated = true;
    }
}

void QNetworkAccessManagerPrivate::authenticationRequired(QNetworkAccessBackend *backend,
                                                          QAuthenticator *authenticator)
{
    Q_Q(QNetworkAccessManager);

    // FIXME: Add support for domains (i.e., the leading path)
    QUrl url = backend->reply->url;

    // don't try the cache for the same URL twice in a row
    // being called twice for the same URL means the authentication failed
    if (url != backend->reply->urlForLastAuthentication) {
        QNetworkAuthenticationCredential *cred = fetchCachedCredentials(url, authenticator);
        if (cred) {
            authenticator->setUser(cred->user);
            authenticator->setPassword(cred->password);
            backend->reply->urlForLastAuthentication = url;
            return;
        }
    }

    backend->reply->urlForLastAuthentication = url;
    emit q->authenticationRequired(backend->reply->q_func(), authenticator);
    addCredentials(url, authenticator);
}

#ifndef QT_NO_NETWORKPROXY
void QNetworkAccessManagerPrivate::proxyAuthenticationRequired(QNetworkAccessBackend *backend,
                                                               const QNetworkProxy &proxy,
                                                               QAuthenticator *authenticator)
{
    Q_Q(QNetworkAccessManager);
    // ### FIXME Tracking of successful authentications
    // This code is a bit broken right now for SOCKS authentication
    // first request: proxyAuthenticationRequired gets emitted, credentials gets saved
    // second request: (proxy != backend->reply->lastProxyAuthentication) does not evaluate to true,
    //      proxyAuthenticationRequired gets emitted again
    // possible solution: some tracking inside the authenticator
    //      or a new function proxyAuthenticationSucceeded(true|false)
    if (proxy != backend->reply->lastProxyAuthentication) {
        QNetworkAuthenticationCredential *cred = fetchCachedCredentials(proxy);
        if (cred) {
            authenticator->setUser(cred->user);
            authenticator->setPassword(cred->password);
            return;
        }
    }

    backend->reply->lastProxyAuthentication = proxy;
    emit q->proxyAuthenticationRequired(proxy, authenticator);
    addCredentials(proxy, authenticator);
}

void QNetworkAccessManagerPrivate::addCredentials(const QNetworkProxy &p,
                                                  const QAuthenticator *authenticator)
{
    Q_ASSERT(authenticator);
    Q_ASSERT(p.type() != QNetworkProxy::DefaultProxy);
    Q_ASSERT(p.type() != QNetworkProxy::NoProxy);

    QString realm = authenticator->realm();
    QNetworkProxy proxy = p;
    proxy.setUser(authenticator->user());
    // Set two credentials: one with the username and one without
    do {
        // Set two credentials actually: one with and one without the realm
        do {
            QByteArray cacheKey = proxyAuthenticationKey(proxy, realm);
            if (cacheKey.isEmpty())
                return;             // should not happen

            QNetworkAuthenticationCache *auth = new QNetworkAuthenticationCache;
            auth->insert(QString(), authenticator->user(), authenticator->password());
            objectCache.addEntry(cacheKey, auth); // replace the existing one, if there's any

            if (realm.isEmpty()) {
                break;
            } else {
                realm.clear();
            }
        } while (true);

        if (proxy.user().isEmpty())
            break;
        else
            proxy.setUser(QString());
    } while (true);
}

QNetworkAuthenticationCredential *
QNetworkAccessManagerPrivate::fetchCachedCredentials(const QNetworkProxy &p,
                                                     const QAuthenticator *authenticator)
{
    QNetworkProxy proxy = p;
    if (proxy.type() == QNetworkProxy::DefaultProxy) {
        proxy = QNetworkProxy::applicationProxy();
    }
    if (!proxy.password().isEmpty())
        return 0;               // no need to set credentials if it already has them

    QString realm;
    if (authenticator)
        realm = authenticator->realm();

    QByteArray cacheKey = proxyAuthenticationKey(proxy, realm);
    if (cacheKey.isEmpty())
        return 0;
    if (!objectCache.hasEntry(cacheKey))
        return 0;

    QNetworkAuthenticationCache *auth =
        static_cast<QNetworkAuthenticationCache *>(objectCache.requestEntryNow(cacheKey));
    QNetworkAuthenticationCredential *cred = auth->findClosestMatch(QString());
    objectCache.releaseEntry(cacheKey);

    // proxy cache credentials always have exactly one item
    Q_ASSERT_X(cred, "QNetworkAccessManager",
               "Internal inconsistency: found a cache key for a proxy, but it's empty");
    return cred;
}

QList<QNetworkProxy> QNetworkAccessManagerPrivate::queryProxy(const QNetworkProxyQuery &query)
{
    QList<QNetworkProxy> proxies;
    if (proxyFactory) {
        proxies = proxyFactory->queryProxy(query);
        if (proxies.isEmpty()) {
            qWarning("QNetworkAccessManager: factory %p has returned an empty result set",
                     proxyFactory);
            proxies << QNetworkProxy::NoProxy;
        }
    } else if (proxy.type() == QNetworkProxy::DefaultProxy) {
        // no proxy set, query the application
        return QNetworkProxyFactory::proxyForQuery(query);
    } else {
        proxies << proxy;
    }

    return proxies;
}
#endif

void QNetworkAccessManagerPrivate::addCredentials(const QUrl &url,
                                                  const QAuthenticator *authenticator)
{
    Q_ASSERT(authenticator);
    QString domain = QString::fromLatin1("/"); // FIXME: make QAuthenticator return the domain
    QString realm = authenticator->realm();

    // Set two credentials actually: one with and one without the username in the URL
    QUrl copy = url;
    copy.setUserName(authenticator->user());
    do {
        QByteArray cacheKey = authenticationKey(copy, realm);
        if (objectCache.hasEntry(cacheKey)) {
            QNetworkAuthenticationCache *auth =
                static_cast<QNetworkAuthenticationCache *>(objectCache.requestEntryNow(cacheKey));
            auth->insert(domain, authenticator->user(), authenticator->password());
            objectCache.releaseEntry(cacheKey);
        } else {
            QNetworkAuthenticationCache *auth = new QNetworkAuthenticationCache;
            auth->insert(domain, authenticator->user(), authenticator->password());
            objectCache.addEntry(cacheKey, auth);
        }

        if (copy.userName().isEmpty()) {
            break;
        } else {
            copy.setUserName(QString());
        }
    } while (true);
}

/*!
    Fetch the credential data from the credential cache.

    If auth is 0 (as it is when called from createRequest()), this will try to
    look up with an empty realm. That fails in most cases for HTTP (because the
    realm is seldom empty for HTTP challenges). In any case, QHttpNetworkConnection
    never sends the credentials on the first attempt: it needs to find out what
    authentication methods the server supports.

    For FTP, realm is always empty.
*/
QNetworkAuthenticationCredential *
QNetworkAccessManagerPrivate::fetchCachedCredentials(const QUrl &url,
                                                     const QAuthenticator *authentication)
{
    if (!url.password().isEmpty())
        return 0;               // no need to set credentials if it already has them

    QString realm;
    if (authentication)
        realm = authentication->realm();

    QByteArray cacheKey = authenticationKey(url, realm);
    if (!objectCache.hasEntry(cacheKey))
        return 0;

    QNetworkAuthenticationCache *auth =
        static_cast<QNetworkAuthenticationCache *>(objectCache.requestEntryNow(cacheKey));
    QNetworkAuthenticationCredential *cred = auth->findClosestMatch(url.path());
    objectCache.releaseEntry(cacheKey);
    return cred;
}

void QNetworkAccessManagerPrivate::clearCache(QNetworkAccessManager *manager)
{
    manager->d_func()->objectCache.clear();
}

QNetworkAccessManagerPrivate::~QNetworkAccessManagerPrivate()
{
}

#ifndef QT_NO_BEARERMANAGEMENT
void QNetworkAccessManagerPrivate::createSession(const QNetworkConfiguration &config)
{
    Q_Q(QNetworkAccessManager);

    initializeSession = false;

    if (networkSession)
        delete networkSession;

    if (!config.isValid()) {
        networkSession = 0;
        online = false;

        if (networkAccessible == QNetworkAccessManager::NotAccessible)
            emit q->networkAccessibleChanged(QNetworkAccessManager::NotAccessible);
        else
            emit q->networkAccessibleChanged(QNetworkAccessManager::UnknownAccessibility);

        return;
    }

    networkSession = new QNetworkSession(config, q);

    QObject::connect(networkSession, SIGNAL(opened()), q, SIGNAL(networkSessionConnected()));
    QObject::connect(networkSession, SIGNAL(closed()), q, SLOT(_q_networkSessionClosed()));
    QObject::connect(networkSession, SIGNAL(stateChanged(QNetworkSession::State)),
                     q, SLOT(_q_networkSessionStateChanged(QNetworkSession::State)));
    QObject::connect(networkSession, SIGNAL(newConfigurationActivated()),
                     q, SLOT(_q_networkSessionNewConfigurationActivated()));
    QObject::connect(networkSession,
                     SIGNAL(preferredConfigurationChanged(QNetworkConfiguration,bool)),
                     q,
                     SLOT(_q_networkSessionPreferredConfigurationChanged(QNetworkConfiguration,bool)));

    _q_networkSessionStateChanged(networkSession->state());
}

void QNetworkAccessManagerPrivate::_q_networkSessionClosed()
{
    if (networkSession) {
        networkConfiguration = networkSession->configuration().identifier();

        networkSession->deleteLater();
        networkSession = 0;
    }
}

void QNetworkAccessManagerPrivate::_q_networkSessionNewConfigurationActivated()
{
    Q_Q(QNetworkAccessManager);

    if (networkSession) {
        networkSession->accept();

        emit q->networkSessionConnected();
    }
}

void QNetworkAccessManagerPrivate::_q_networkSessionPreferredConfigurationChanged(const QNetworkConfiguration &, bool)
{
    if (networkSession)
        networkSession->migrate();
}

void QNetworkAccessManagerPrivate::_q_networkSessionStateChanged(QNetworkSession::State state)
{
    Q_Q(QNetworkAccessManager);

    if (online) {
        if (state != QNetworkSession::Connected && state != QNetworkSession::Roaming) {
            online = false;
            emit q->networkAccessibleChanged(QNetworkAccessManager::NotAccessible);
        }
    } else {
        if (state == QNetworkSession::Connected || state == QNetworkSession::Roaming) {
            online = true;
            emit q->networkAccessibleChanged(networkAccessible);
        }
    }
}
#endif // QT_NO_BEARERMANAGEMENT

QT_END_NAMESPACE

#include "moc_qnetworkaccessmanager.cpp"
