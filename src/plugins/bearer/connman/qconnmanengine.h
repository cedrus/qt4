/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the plugins of the Qt Toolkit.
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

#ifndef QCONNMANENGINE_P_H
#define QCONNMANENGINE_P_H

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

#include "../qbearerengine_impl.h"

#include "qconnmanservice_linux_p.h"

#include <QMap>
#include <QVariant>
#include <QtCore/qthread.h>

#ifndef QT_NO_BEARERMANAGEMENT
#ifndef QT_NO_DBUS

QT_BEGIN_NAMESPACE

class QConnmanConnectThread;
class QConnmanEngine : public QBearerEngineImpl
{
    Q_OBJECT

public:
    QConnmanEngine(QObject *parent = 0);
    ~QConnmanEngine();

    bool connmanAvailable() const;

    virtual QString getInterfaceFromId(const QString &id);
    bool hasIdentifier(const QString &id);

    virtual void connectToId(const QString &id);
    virtual void disconnectFromId(const QString &id);

    Q_INVOKABLE void initialize();
    Q_INVOKABLE void requestUpdate();

    QNetworkSession::State sessionStateForId(const QString &id);
    QNetworkSessionPrivate *createSessionBackend();

    virtual quint64 bytesWritten(const QString &id);
    virtual quint64 bytesReceived(const QString &id);
    virtual quint64 startTime(const QString &id);


    virtual QNetworkConfigurationManager::Capabilities capabilities() const;
    virtual QNetworkConfigurationPrivatePointer defaultConfiguration();

    void configurationChange(const QString &id);
    QList<QNetworkConfigurationPrivate *> getConfigurations();


private Q_SLOTS:

    void doRequestUpdate();
    void servicePropertyChangedContext(const QString &,const QString &,const QDBusVariant &);
    void networkPropertyChangedContext(const QString &,const QString &,const QDBusVariant &);
    void devicePropertyChangedContext(const QString &,const QString &,const QDBusVariant &);
    void propertyChangedContext(const QString &,const QString &,const QDBusVariant &);
    void technologyPropertyChangedContext(const QString &,const QString &, const QDBusVariant &);

private:
    QConnmanManagerInterface *connmanManager;

    QList<QNetworkConfigurationPrivate *> foundConfigurations;

    void getNetworkListing();

    QString getServiceForNetwork(const QString &network);

    QString serviceFromId(const QString &id);
    QString networkFromId(const QString &id);

    QNetworkConfiguration::StateFlags getStateForService(const QString &service);
    QNetworkConfiguration::BearerType typeToBearer(const QString &type);

    void removeConfiguration(const QString &servicePath);
    void addServiceConfiguration(const QString &servicePath);
    void addNetworkConfiguration(const QString &worknetPath);
    QDateTime activeTime;


    QMap<QString,QConnmanTechnologyInterface *> technologies; // techpath, tech interface
    QMap<QString,QString> configInterfaces; // id, interface name
    QMap<QString,QStringList> knownNetworks; //device path, net paths list
    QMap<QString,QStringList> deviceMap; //tech path,  device path
    QMap<QString, QString> serviceNetworks; //service, network

    QNetworkConfiguration::BearerType ofonoTechToBearerType(const QString &type);
    bool isRoamingAllowed(const QString &context);
protected:
    bool requiresPolling() const;
    QConnmanConnectThread *connThread;
};

class QConnmanConnectThread : public QThread
{
    Q_OBJECT

public:
    QConnmanConnectThread(QObject *parent = 0);
    ~QConnmanConnectThread();
    bool keepRunning;
    void stop();
    void setServicePath(const QString &path);
    void setIdentifier(const QString &id);

Q_SIGNALS:
    void connectionError(const QString &id, QBearerEngineImpl::ConnectionError error);

protected:
    void run();
    QString servicePath;
    QString identifier;

private:
    QMutex mutex;

};

QT_END_NAMESPACE

#endif // QT_NO_DBUS
#endif // QT_NO_BEARERMANAGEMENT

#endif

