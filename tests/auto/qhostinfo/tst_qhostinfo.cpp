/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the test suite of the Qt Toolkit.
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


// When using WinSock2 on Windows, it's the first thing that can be included
// (except qglobal.h), or else you'll get tons of compile errors
#include <qglobal.h>

// MinGW doesn't provide getaddrinfo(), so we test for Q_OS_WIN
// and Q_CC_GNU, which indirectly tells us whether we're using MinGW.
#if defined(Q_OS_WIN) && defined(Q_CC_GNU)
# define QT_NO_GETADDRINFO
#endif

#if defined(Q_OS_WIN) && !defined(QT_NO_GETADDRINFO)
# include <winsock2.h>
# include <ws2tcpip.h>
#endif

#include <QtTest/QtTest>
#include <qcoreapplication.h>
#include <QDebug>
#include <QTcpSocket>
#include <private/qthread_p.h>
#include <QTcpServer>

#include <time.h>
#include <qlibrary.h>
#if defined(Q_OS_WIN32) || defined(Q_OS_WINCE)
#include <windows.h>
#else
#include <unistd.h>
#include <signal.h>
#endif

#include <qhostinfo.h>
#include "private/qhostinfo_p.h"

#if !defined(QT_NO_GETADDRINFO)
# if !defined(Q_OS_WINCE)
#  include <sys/types.h>
# else
#  include <types.h>
# endif
# if defined(Q_OS_UNIX)
#  include <sys/socket.h>
# endif
# if !defined(Q_OS_WIN)
#  include <netdb.h>
# endif
#endif

#include "../network-settings.h"
#include "../../shared/util.h"

//TESTED_CLASS=
//TESTED_FILES=

const char * const lupinellaIp = "10.3.4.6";


class tst_QHostInfo : public QObject
{
    Q_OBJECT

public:
    tst_QHostInfo();
    virtual ~tst_QHostInfo();


public slots:
    void init();
    void cleanup();
    void initTestCase();

private slots:
    void getSetCheck();
    void staticInformation();
    void lookupIPv4_data();
    void lookupIPv4();
    void lookupIPv6_data();
    void lookupIPv6();
    void reverseLookup_data();
    void reverseLookup();

    void blockingLookup_data();
    void blockingLookup();

    void raceCondition();
    void threadSafety();

    void multipleSameLookups();
    void multipleDifferentLookups_data();
    void multipleDifferentLookups();

    void cache();

protected slots:
    void resultsReady(const QHostInfo &);

private:
    bool ipv6LookupsAvailable;
    bool ipv6Available;
    bool lookupDone;
    int lookupsDoneCounter;
    QHostInfo lookupResults;
};

// Testing get/set functions
void tst_QHostInfo::getSetCheck()
{
    QHostInfo obj1;
    // HostInfoError QHostInfo::error()
    // void QHostInfo::setError(HostInfoError)
    obj1.setError(QHostInfo::HostInfoError(0));
    QCOMPARE(QHostInfo::HostInfoError(0), obj1.error());
    obj1.setError(QHostInfo::HostInfoError(1));
    QCOMPARE(QHostInfo::HostInfoError(1), obj1.error());

    // int QHostInfo::lookupId()
    // void QHostInfo::setLookupId(int)
    obj1.setLookupId(0);
    QCOMPARE(0, obj1.lookupId());
    obj1.setLookupId(INT_MIN);
    QCOMPARE(INT_MIN, obj1.lookupId());
    obj1.setLookupId(INT_MAX);
    QCOMPARE(INT_MAX, obj1.lookupId());
}

void tst_QHostInfo::staticInformation()
{
    qDebug() << "Hostname:" << QHostInfo::localHostName();
    qDebug() << "Domain name:" << QHostInfo::localDomainName();
}

tst_QHostInfo::tst_QHostInfo()
{
    Q_SET_DEFAULT_IAP
}

tst_QHostInfo::~tst_QHostInfo()
{
}

void tst_QHostInfo::initTestCase()
{
    ipv6Available = false;
    ipv6LookupsAvailable = false;
#if !defined(QT_NO_GETADDRINFO)
    // check if the system getaddrinfo can do IPv6 lookups
    struct addrinfo hint, *result = 0;
    memset(&hint, 0, sizeof hint);
    hint.ai_family = AF_UNSPEC;
# ifdef AI_ADDRCONFIG
    hint.ai_flags = AI_ADDRCONFIG;
# endif

    int res = getaddrinfo("::1", "80", &hint, &result);
    if (res == 0) {
        // this test worked
        freeaddrinfo(result);
        res = getaddrinfo("ipv6-test.dev.troll.no", "80", &hint, &result);
        if (res == 0 && result != 0 && result->ai_family != AF_INET) {
            freeaddrinfo(result);
            ipv6LookupsAvailable = true;
        }
    }
#endif

    QTcpServer server;
    if (server.listen(QHostAddress("::1"))) {
        // We have IPv6 support
        ipv6Available = true;
    }


    // run each testcase with and without test enabled
    QTest::addColumn<bool>("cache");
    QTest::newRow("WithCache") << true;
    QTest::newRow("WithoutCache") << false;
}

void tst_QHostInfo::init()
{
    // delete the cache so inidividual testcase results are independant from each other
    qt_qhostinfo_clear_cache();

    QFETCH_GLOBAL(bool, cache);
    qt_qhostinfo_enable_cache(cache);
}

void tst_QHostInfo::cleanup()
{
}

void tst_QHostInfo::lookupIPv4_data()
{
    QTest::addColumn<QString>("hostname");
    QTest::addColumn<QString>("addresses");
    QTest::addColumn<int>("err");

#ifdef Q_OS_SYMBIAN
    // Test server lookup
    QTest::newRow("lookup_01") << QtNetworkSettings::serverName() << QtNetworkSettings::serverIP().toString() << int(QHostInfo::NoError);
    QTest::newRow("literal_ip4") << QtNetworkSettings::serverIP().toString() << QtNetworkSettings::serverIP().toString() << int(QHostInfo::NoError);
    QTest::newRow("multiple_ip4") << "multi.dev.troll.no" << "1.2.3.4 1.2.3.5 10.3.3.31" << int(QHostInfo::NoError);
#else
    QTest::newRow("empty") << "" << "" << int(QHostInfo::HostNotFound);

    QTest::newRow("single_ip4") << "lupinella.troll.no" << lupinellaIp << int(QHostInfo::NoError);
    QTest::newRow("multiple_ip4") << "multi.dev.troll.no" << "1.2.3.4 1.2.3.5 10.3.3.31" << int(QHostInfo::NoError);
    QTest::newRow("literal_ip4") << lupinellaIp << lupinellaIp << int(QHostInfo::NoError);
#endif
    QTest::newRow("notfound") << "this-name-does-not-exist-hopefully." << "" << int(QHostInfo::HostNotFound);

    QTest::newRow("idn-ace") << "xn--alqualond-34a.troll.no" << "10.3.3.55" << int(QHostInfo::NoError);
    QTest::newRow("idn-unicode") << QString::fromLatin1("alqualond\353.troll.no") << "10.3.3.55" << int(QHostInfo::NoError);
}

void tst_QHostInfo::lookupIPv4()
{
    QFETCH(QString, hostname);
    QFETCH(int, err);
    QFETCH(QString, addresses);

    lookupDone = false;
    QHostInfo::lookupHost(hostname, this, SLOT(resultsReady(const QHostInfo&)));

    QTestEventLoop::instance().enterLoop(10);
    QVERIFY(!QTestEventLoop::instance().timeout());
    QVERIFY(lookupDone);

    if ((int)lookupResults.error() != (int)err) {
        qWarning() << hostname << "=>" << lookupResults.errorString();
    }
    QCOMPARE((int)lookupResults.error(), (int)err);

    QStringList tmp;
    for (int i = 0; i < lookupResults.addresses().count(); ++i)
        tmp.append(lookupResults.addresses().at(i).toString());
    tmp.sort();

    QStringList expected = addresses.split(' ');
    expected.sort();

    QCOMPARE(tmp.join(" "), expected.join(" "));
}

void tst_QHostInfo::lookupIPv6_data()
{
    QTest::addColumn<QString>("hostname");
    QTest::addColumn<QString>("addresses");
    QTest::addColumn<int>("err");

    QTest::newRow("ip6") << "www.ipv6-net.org" << "62.93.217.177 2001:618:1401:0:0:0:0:4" << int(QHostInfo::NoError);

    // avoid using real IPv6 addresses here because this will do a DNS query
    // real addresses are between 2000:: and 3fff:ffff:ffff:ffff:ffff:ffff:ffff
    QTest::newRow("literal_ip6") << "f001:6b0:1:ea:202:a5ff:fecd:13a6" << "f001:6b0:1:ea:202:a5ff:fecd:13a6" << int(QHostInfo::NoError);
    QTest::newRow("literal_shortip6") << "f001:618:1401::4" << "f001:618:1401:0:0:0:0:4" << int(QHostInfo::NoError);
}

void tst_QHostInfo::lookupIPv6()
{
    QFETCH(QString, hostname);
    QFETCH(int, err);
    QFETCH(QString, addresses);

    if (!ipv6LookupsAvailable)
        QSKIP("This platform does not support IPv6 lookups", SkipAll);

    lookupDone = false;
    QHostInfo::lookupHost(hostname, this, SLOT(resultsReady(const QHostInfo&)));

    QTestEventLoop::instance().enterLoop(3);
    QVERIFY(!QTestEventLoop::instance().timeout());
    QVERIFY(lookupDone);

    QCOMPARE((int)lookupResults.error(), (int)err);

    QStringList tmp;
    for (int i = 0; i < lookupResults.addresses().count(); ++i)
        tmp.append(lookupResults.addresses().at(i).toString());
    tmp.sort();

    QStringList expected = addresses.split(' ');
    expected.sort();

    QCOMPARE(tmp.join(" ").toLower(), expected.join(" ").toLower());
}

void tst_QHostInfo::reverseLookup_data()
{
    QTest::addColumn<QString>("address");
    QTest::addColumn<QStringList>("hostNames");
    QTest::addColumn<int>("err");

    QTest::newRow("trolltech.com") << QString("62.70.27.69") << QStringList(QString("diverse.troll.no")) << 0;

    // ### Use internal DNS instead. Discussed with Andreas.
    //QTest::newRow("classical.hexago.com") << QString("2001:5c0:0:2::24") << QStringList(QString("classical.hexago.com")) << 0;
    QTest::newRow("origin.cisco.com") << QString("12.159.148.94") << QStringList(QString("origin.cisco.com")) << 0;
    QTest::newRow("bogus-name") << QString("1::2::3::4") << QStringList() << 1;
}

void tst_QHostInfo::reverseLookup()
{
    QFETCH(QString, address);
    QFETCH(QStringList, hostNames);
    QFETCH(int, err);

    if (!ipv6LookupsAvailable && hostNames.contains("classical.hexago.com")) {
        QSKIP("IPv6 lookups are not supported on this platform", SkipSingle);
    }
#if defined(Q_OS_HPUX) && defined(__ia64)
    if (hostNames.contains("classical.hexago.com"))
        QSKIP("HP-UX 11i does not support IPv6 reverse lookups.", SkipSingle);
#endif

    QHostInfo info = QHostInfo::fromName(address);

    if (err == 0) {
        QVERIFY(hostNames.contains(info.hostName()));
        QCOMPARE(info.addresses().first(), QHostAddress(address));
    } else {
        QCOMPARE(info.hostName(), address);
        QCOMPARE(info.error(), QHostInfo::HostNotFound);
    }

}

void tst_QHostInfo::blockingLookup_data()
{
    lookupIPv4_data();
    if (ipv6LookupsAvailable)
        lookupIPv6_data();
}

void tst_QHostInfo::blockingLookup()
{
    QFETCH(QString, hostname);
    QFETCH(int, err);
    QFETCH(QString, addresses);

    QHostInfo hostInfo = QHostInfo::fromName(hostname);
    QStringList tmp;
    for (int i = 0; i < hostInfo.addresses().count(); ++i)
        tmp.append(hostInfo.addresses().at(i).toString());
    tmp.sort();

    if ((int)hostInfo.error() != (int)err) {
        qWarning() << hostname << "=>" << lookupResults.errorString();
    }
    QCOMPARE((int)hostInfo.error(), (int)err);

    QStringList expected = addresses.split(' ');
    expected.sort();

    QCOMPARE(tmp.join(" ").toUpper(), expected.join(" ").toUpper());
}

void tst_QHostInfo::raceCondition()
{
    for (int i = 0; i < 1000; ++i) {
        QTcpSocket socket;
        socket.connectToHost("notavalidname.troll.no", 80);
    }
}

class LookupThread : public QThread
{
protected:
    inline void run()
    {
         QHostInfo info = QHostInfo::fromName("qt.nokia.com");
         QCOMPARE(info.addresses().at(0).toString(), QString("87.238.50.178"));
    }
};

void tst_QHostInfo::threadSafety()
{
    const int nattempts = 5;
#if defined(Q_OS_WINCE)
    const int runs = 10;
#else
    const int runs = 100;
#endif
    LookupThread thr[nattempts];
    for (int j = 0; j < runs; ++j) {
        for (int i = 0; i < nattempts; ++i)
            thr[i].start();
        for (int k = nattempts - 1; k >= 0; --k)
            thr[k].wait();
    }
}

// this test is for the multi-threaded QHostInfo rewrite. It is about getting results at all,
// not about getting correct IPs
void tst_QHostInfo::multipleSameLookups()
{
    const int COUNT = 10;
    lookupsDoneCounter = 0;

    for (int i = 0; i < COUNT; i++)
        QHostInfo::lookupHost("localhost", this, SLOT(resultsReady(const QHostInfo)));

    QElapsedTimer timer;
    timer.start();
    while (timer.elapsed() < 10000 && lookupsDoneCounter < COUNT) {
        QTestEventLoop::instance().enterLoop(2);
    }
    QCOMPARE(lookupsDoneCounter, COUNT);
}

// this test is for the multi-threaded QHostInfo rewrite. It is about getting results at all,
// not about getting correct IPs
void tst_QHostInfo::multipleDifferentLookups_data()
{
    QTest::addColumn<int>("repeats");
    QTest::newRow("1") << 1;
    QTest::newRow("2") << 2;
    QTest::newRow("5") << 5;
    QTest::newRow("10") << 10;
}

void tst_QHostInfo::multipleDifferentLookups()
{
    QStringList hostnameList;
    hostnameList << "www.ovi.com" << "www.nokia.com" << "qt.nokia.com" << "www.trolltech.com" << "troll.no"
            << "www.qtcentre.org" << "forum.nokia.com" << "www.nokia.com" << "wiki.forum.nokia.com"
            << "www.nokia.com" << "nokia.de" << "127.0.0.1" << "----";

    QFETCH(int, repeats);
    const int COUNT = hostnameList.size();
    lookupsDoneCounter = 0;

    for (int i = 0; i < hostnameList.size(); i++)
        for (int j = 0; j < repeats; ++j)
            QHostInfo::lookupHost(hostnameList.at(i), this, SLOT(resultsReady(const QHostInfo)));

    QElapsedTimer timer;
    timer.start();
    while (timer.elapsed() < 10000 && lookupsDoneCounter < repeats*COUNT) {
        QTestEventLoop::instance().enterLoop(2);
    }
    QCOMPARE(lookupsDoneCounter, repeats*COUNT);
}

void tst_QHostInfo::cache()
{
    QFETCH_GLOBAL(bool, cache);
    if (!cache)
        return; // test makes only sense when cache enabled

    // reset slot counter
    lookupsDoneCounter = 0;

    // lookup once, wait in event loop, result should not come directly.
    bool valid = true;
    int id = -1;
    QHostInfo result = qt_qhostinfo_lookup("localhost", this, SLOT(resultsReady(QHostInfo)), &valid, &id);
    QTestEventLoop::instance().enterLoop(5);
    QVERIFY(!QTestEventLoop::instance().timeout());
    QVERIFY(valid == false);
    QVERIFY(result.addresses().isEmpty());

    // loopkup second time, result should come directly
    valid = false;
    result = qt_qhostinfo_lookup("localhost", this, SLOT(resultsReady(QHostInfo)), &valid, &id);
    QVERIFY(valid == true);
    QVERIFY(!result.addresses().isEmpty());

    // clear the cache
    qt_qhostinfo_clear_cache();

    // lookup third time, result should not come directly.
    valid = true;
    result = qt_qhostinfo_lookup("localhost", this, SLOT(resultsReady(QHostInfo)), &valid, &id);
    QTestEventLoop::instance().enterLoop(5);
    QVERIFY(!QTestEventLoop::instance().timeout());
    QVERIFY(valid == false);
    QVERIFY(result.addresses().isEmpty());

    // the slot should have been called 2 times.
    QVERIFY(lookupsDoneCounter == 2);
}

void tst_QHostInfo::resultsReady(const QHostInfo &hi)
{
    lookupDone = true;
    lookupResults = hi;
    lookupsDoneCounter++;
    QMetaObject::invokeMethod(&QTestEventLoop::instance(), "exitLoop", Qt::QueuedConnection);
}

QTEST_MAIN(tst_QHostInfo)
#include "tst_qhostinfo.moc"
