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


#ifndef QSSLSOCKET_P_H
#define QSSLSOCKET_P_H

#include "qsslsocket.h"

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists for the convenience
// of the QLibrary class.  This header file may change from
// version to version without notice, or even be removed.
//
// We mean it.
//

#include <private/qtcpsocket_p.h>
#include "qsslkey.h"
#include "qsslconfiguration_p.h"

#include <QtCore/qstringlist.h>

#include <private/qringbuffer_p.h>

QT_BEGIN_NAMESPACE

#if defined(Q_OS_MAC)
#include <Security/SecCertificate.h>
#include <CoreFoundation/CFArray.h>
    typedef OSStatus (*PtrSecCertificateGetData)(SecCertificateRef, CSSM_DATA_PTR);
    typedef OSStatus (*PtrSecTrustSettingsCopyCertificates)(int, CFArrayRef*);
    typedef OSStatus (*PtrSecTrustCopyAnchorCertificates)(CFArrayRef*);
#elif defined(Q_OS_WIN)
#include <wincrypt.h>
#ifndef HCRYPTPROV_LEGACY
#define HCRYPTPROV_LEGACY HCRYPTPROV
#endif
#if defined(Q_OS_WINCE)
    typedef HCERTSTORE (WINAPI *PtrCertOpenSystemStoreW)(LPCSTR, DWORD, HCRYPTPROV_LEGACY, DWORD, const void*);
#else
    typedef HCERTSTORE (WINAPI *PtrCertOpenSystemStoreW)(HCRYPTPROV_LEGACY, LPCWSTR);
#endif
    typedef PCCERT_CONTEXT (WINAPI *PtrCertFindCertificateInStore)(HCERTSTORE, DWORD, DWORD, DWORD, const void*, PCCERT_CONTEXT);
    typedef BOOL (WINAPI *PtrCertCloseStore)(HCERTSTORE, DWORD);
#endif



class QSslSocketPrivate : public QTcpSocketPrivate
{
    Q_DECLARE_PUBLIC(QSslSocket)
public:
    QSslSocketPrivate();
    virtual ~QSslSocketPrivate();

    void init();
    bool initialized;

    QSslSocket::SslMode mode;
    bool autoStartHandshake;
    bool connectionEncrypted;
    bool ignoreAllSslErrors;
    QList<QSslError> ignoreErrorsList;
    bool* readyReadEmittedPointer;

    QSslConfigurationPrivate configuration;
    QList<QSslError> sslErrors;

    // if set, this hostname is used for certificate validation instead of the hostname
    // that was used for connecting to.
    QString verificationPeerName;

    static bool supportsSsl();
    static void ensureInitialized();
    static void deinitialize();
    static QList<QSslCipher> defaultCiphers();
    static QList<QSslCipher> supportedCiphers();
    static void setDefaultCiphers(const QList<QSslCipher> &ciphers);
    static void setDefaultSupportedCiphers(const QList<QSslCipher> &ciphers);
    static void resetDefaultCiphers();

    static QList<QSslCertificate> defaultCaCertificates();
    static QList<QSslCertificate> systemCaCertificates();
    static void setDefaultCaCertificates(const QList<QSslCertificate> &certs);
    static bool addDefaultCaCertificates(const QString &path, QSsl::EncodingFormat format,
                                         QRegExp::PatternSyntax syntax);
    static void addDefaultCaCertificate(const QSslCertificate &cert);
    static void addDefaultCaCertificates(const QList<QSslCertificate> &certs);

#if defined(Q_OS_MAC)
    static PtrSecCertificateGetData ptrSecCertificateGetData;
    static PtrSecTrustSettingsCopyCertificates ptrSecTrustSettingsCopyCertificates;
    static PtrSecTrustCopyAnchorCertificates ptrSecTrustCopyAnchorCertificates;
#elif defined(Q_OS_WIN)
    static PtrCertOpenSystemStoreW ptrCertOpenSystemStoreW;
    static PtrCertFindCertificateInStore ptrCertFindCertificateInStore;
    static PtrCertCloseStore ptrCertCloseStore;
#endif

    // The socket itself, including private slots.
    QTcpSocket *plainSocket;
    void createPlainSocket(QIODevice::OpenMode openMode);
    void _q_connectedSlot();
    void _q_hostFoundSlot();
    void _q_disconnectedSlot();
    void _q_stateChangedSlot(QAbstractSocket::SocketState);
    void _q_errorSlot(QAbstractSocket::SocketError);
    void _q_readyReadSlot();
    void _q_bytesWrittenSlot(qint64);
    void _q_flushWriteBuffer();
    void _q_flushReadBuffer();

    // Platform specific functions
    virtual void startClientEncryption() = 0;
    virtual void startServerEncryption() = 0;
    virtual void transmit() = 0;
    virtual void disconnectFromHost() = 0;
    virtual void disconnected() = 0;
    virtual QSslCipher sessionCipher() const = 0;

private:
    static bool ensureLibraryLoaded();
    static void ensureCiphersAndCertsLoaded();

    static bool s_libraryLoaded;
    static bool s_loadedCiphersAndCerts;
};

QT_END_NAMESPACE

#endif
