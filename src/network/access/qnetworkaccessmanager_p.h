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

#ifndef QNETWORKACCESSMANAGER_P_H
#define QNETWORKACCESSMANAGER_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists for the convenience
// of the Network Access API.  This header file may change from
// version to version without notice, or even be removed.
//
// We mean it.
//

#include "qnetworkaccessmanager.h"
#include "qnetworkaccesscache_p.h"
#include "qnetworkaccessbackend_p.h"
#include "private/qobject_p.h"
#include "QtNetwork/qnetworkproxy.h"
#include "QtNetwork/qnetworksession.h"

QT_BEGIN_NAMESPACE

class QAuthenticator;
class QAbstractNetworkCache;
class QNetworkAuthenticationCredential;
class QNetworkCookieJar;

class QNetworkAccessManagerPrivate: public QObjectPrivate
{
public:
    QNetworkAccessManagerPrivate()
        : networkCache(0), cookieJar(0),
#ifndef QT_NO_NETWORKPROXY
          proxyFactory(0),
#endif
#ifndef QT_NO_BEARERMANAGEMENT
          networkSession(0),
          networkAccessible(QNetworkAccessManager::Accessible),
          online(false),
          initializeSession(true),
#endif
          cookieJarCreated(false)
    { }
    ~QNetworkAccessManagerPrivate();

    void _q_replyFinished();
    void _q_replySslErrors(const QList<QSslError> &errors);
    QNetworkReply *postProcess(QNetworkReply *reply);
    void createCookieJar() const;

    void authenticationRequired(QNetworkAccessBackend *backend, QAuthenticator *authenticator);
    void addCredentials(const QUrl &url, const QAuthenticator *auth);
    QNetworkAuthenticationCredential *fetchCachedCredentials(const QUrl &url,
                                                             const QAuthenticator *auth = 0);

#ifndef QT_NO_NETWORKPROXY
    void proxyAuthenticationRequired(QNetworkAccessBackend *backend, const QNetworkProxy &proxy,
                                     QAuthenticator *authenticator);
    void addCredentials(const QNetworkProxy &proxy, const QAuthenticator *auth);
    QNetworkAuthenticationCredential *fetchCachedCredentials(const QNetworkProxy &proxy,
                                                             const QAuthenticator *auth = 0);
    QList<QNetworkProxy> queryProxy(const QNetworkProxyQuery &query);
#endif

    QNetworkAccessBackend *findBackend(QNetworkAccessManager::Operation op, const QNetworkRequest &request);

#ifndef QT_NO_BEARERMANAGEMENT
    void createSession(const QNetworkConfiguration &config);

    void _q_networkSessionClosed();
    void _q_networkSessionNewConfigurationActivated();
    void _q_networkSessionPreferredConfigurationChanged(const QNetworkConfiguration &config,
                                                        bool isSeamless);
    void _q_networkSessionStateChanged(QNetworkSession::State state);
#endif

    // this is the cache for storing downloaded files
    QAbstractNetworkCache *networkCache;

    QNetworkCookieJar *cookieJar;


#ifndef QT_NO_NETWORKPROXY
    QNetworkProxy proxy;
    QNetworkProxyFactory *proxyFactory;
#endif

#ifndef QT_NO_BEARERMANAGEMENT
    QNetworkSession *networkSession;
    QString networkConfiguration;
    QNetworkAccessManager::NetworkAccessibility networkAccessible;
    bool online;
    bool initializeSession;
#endif

    bool cookieJarCreated;

    // this cache can be used by individual backends to cache e.g. their TCP connections to a server
    // and use the connections for multiple requests.
    QNetworkAccessCache objectCache;
    static inline QNetworkAccessCache *getObjectCache(QNetworkAccessBackend *backend)
    { return &backend->manager->objectCache; }
    Q_AUTOTEST_EXPORT static void clearCache(QNetworkAccessManager *manager);

    Q_DECLARE_PUBLIC(QNetworkAccessManager)
};

QT_END_NAMESPACE

#endif
