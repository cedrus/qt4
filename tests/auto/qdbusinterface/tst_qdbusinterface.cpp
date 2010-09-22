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
/* -*- C++ -*-
 */
#include <qcoreapplication.h>
#include <qmetatype.h>
#include <QtTest/QtTest>
#include <QtCore/qvariant.h>
#include <QtDBus/QtDBus>

#include "../qdbusmarshall/common.h"

Q_DECLARE_METATYPE(QVariantList)

#define TEST_INTERFACE_NAME "com.trolltech.QtDBus.MyObject"
#define TEST_SIGNAL_NAME "somethingHappened"

class MyObject: public QObject
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.trolltech.QtDBus.MyObject")
    Q_CLASSINFO("D-Bus Introspection", ""
"  <interface name=\"com.trolltech.QtDBus.MyObject\" >\n"
"    <property access=\"readwrite\" type=\"i\" name=\"prop1\" />\n"
"    <property name=\"complexProp\" type=\"ai\" access=\"readwrite\">\n"
"      <annotation name=\"com.trolltech.QtDBus.QtTypeName\" value=\"QList&lt;int&gt;\"/>\n"
"    </property>\n"
"    <signal name=\"somethingHappened\" >\n"
"      <arg direction=\"out\" type=\"s\" />\n"
"    </signal>\n"
"    <method name=\"ping\" >\n"
"      <arg direction=\"in\" type=\"v\" name=\"ping\" />\n"
"      <arg direction=\"out\" type=\"v\" name=\"ping\" />\n"
"    </method>\n"
"    <method name=\"ping_invokable\" >\n"
"      <arg direction=\"in\" type=\"v\" name=\"ping_invokable\" />\n"
"      <arg direction=\"out\" type=\"v\" name=\"ping_invokable\" />\n"
"    </method>\n"
"    <method name=\"ping\" >\n"
"      <arg direction=\"in\" type=\"v\" name=\"ping1\" />\n"
"      <arg direction=\"in\" type=\"v\" name=\"ping2\" />\n"
"      <arg direction=\"out\" type=\"v\" name=\"pong1\" />\n"
"      <arg direction=\"out\" type=\"v\" name=\"pong2\" />\n"
"    </method>\n"
"    <method name=\"ping_invokable\" >\n"
"      <arg direction=\"in\" type=\"v\" name=\"ping1_invokable\" />\n"
"      <arg direction=\"in\" type=\"v\" name=\"ping2_invokable\" />\n"
"      <arg direction=\"out\" type=\"v\" name=\"pong1_invokable\" />\n"
"      <arg direction=\"out\" type=\"v\" name=\"pong2_invokable\" />\n"
"    </method>\n"
"    <method name=\"ping\" >\n"
"      <arg direction=\"in\" type=\"ai\" name=\"ping\" />\n"
"      <arg direction=\"out\" type=\"ai\" name=\"ping\" />\n"
"      <annotation name=\"com.trolltech.QtDBus.QtTypeName.In0\" value=\"QList&lt;int&gt;\"/>\n"
"      <annotation name=\"com.trolltech.QtDBus.QtTypeName.Out0\" value=\"QList&lt;int&gt;\"/>\n"
"    </method>\n"
"    <method name=\"ping_invokable\" >\n"
"      <arg direction=\"in\" type=\"ai\" name=\"ping_invokable\" />\n"
"      <arg direction=\"out\" type=\"ai\" name=\"ping_invokable\" />\n"
"      <annotation name=\"com.trolltech.QtDBus.QtTypeName.In0\" value=\"QList&lt;int&gt;\"/>\n"
"      <annotation name=\"com.trolltech.QtDBus.QtTypeName.Out0\" value=\"QList&lt;int&gt;\"/>\n"
"    </method>\n"
"  </interface>\n"
        "")
    Q_PROPERTY(int prop1 READ prop1 WRITE setProp1)
    Q_PROPERTY(QList<int> complexProp READ complexProp WRITE setComplexProp)

public:
    static int callCount;
    static QVariantList callArgs;
    MyObject()
    {
        QObject *subObject = new QObject(this);
        subObject->setObjectName("subObject");
    }

    int m_prop1;
    int prop1() const
    {
        ++callCount;
        return m_prop1;
    }
    void setProp1(int value)
    {
        ++callCount;
        m_prop1 = value;
    }

    QList<int> m_complexProp;
    QList<int> complexProp() const
    {
        ++callCount;
        return m_complexProp;
    }
    void setComplexProp(const QList<int> &value)
    {
        ++callCount;
        m_complexProp = value;
    }

    Q_INVOKABLE void ping_invokable(QDBusMessage msg)
    {
        QDBusConnection sender = QDBusConnection::sender();
        if (!sender.isConnected())
            exit(1);

        ++callCount;
        callArgs = msg.arguments();

        msg.setDelayedReply(true);
        if (!sender.send(msg.createReply(callArgs)))
            exit(1);
    }

public slots:

    void ping(QDBusMessage msg)
    {
        QDBusConnection sender = QDBusConnection::sender();
        if (!sender.isConnected())
            exit(1);

        ++callCount;
        callArgs = msg.arguments();

        msg.setDelayedReply(true);
        if (!sender.send(msg.createReply(callArgs)))
            exit(1);
    }
};
int MyObject::callCount = 0;
QVariantList MyObject::callArgs;

class Spy: public QObject
{
    Q_OBJECT
public:
    QString received;
    int count;

    Spy() : count(0)
    { }

public slots:
    void spySlot(const QString& arg)
    {
        received = arg;
        ++count;
    }
};

class DerivedFromQDBusInterface: public QDBusInterface
{
    Q_OBJECT
public:
    DerivedFromQDBusInterface()
        : QDBusInterface("com.example.Test", "/")
    {}

public slots:
    void method() {}
};

// helper function
void emitSignal(const QString &interface, const QString &name, const QString &arg)
{
    QDBusMessage msg = QDBusMessage::createSignal("/", interface, name);
    msg << arg;
    QDBusConnection::sessionBus().send(msg);

    QTest::qWait(1000);
}

class tst_QDBusInterface: public QObject
{
    Q_OBJECT
    MyObject obj;
public slots:
    void testServiceOwnerChanged(const QString &service)
    {
        if (service == "com.example.Test")
            QTestEventLoop::instance().exitLoop();
    }

private slots:
    void initTestCase();

    void notConnected();
    void notValid();
    void notValidDerived();
    void invalidAfterServiceOwnerChanged();
    void introspect();
    void callMethod();
    void invokeMethod();
    void invokeMethodWithReturn();
    void invokeMethodWithMultiReturn();
    void invokeMethodWithComplexReturn();

    void signal();

    void propertyRead();
    void propertyWrite();
    void complexPropertyRead();
    void complexPropertyWrite();
};

void tst_QDBusInterface::initTestCase()
{
    QDBusConnection con = QDBusConnection::sessionBus();
    QVERIFY(con.isConnected());
    QTest::qWait(500);

    con.registerObject("/", &obj, QDBusConnection::ExportAllProperties
                       | QDBusConnection::ExportAllSlots
                       | QDBusConnection::ExportAllInvokables
                       | QDBusConnection::ExportChildObjects);
}

void tst_QDBusInterface::notConnected()
{
    QDBusConnection connection("");
    QVERIFY(!connection.isConnected());

    QDBusInterface interface("org.freedesktop.DBus", "/", "org.freedesktop.DBus",
                             connection);

    QVERIFY(!interface.isValid());
    QVERIFY(!QMetaObject::invokeMethod(&interface, "ListNames", Qt::DirectConnection));
}

void tst_QDBusInterface::notValid()
{
    QDBusConnection connection("");
    QVERIFY(!connection.isConnected());

    QDBusInterface interface("com.example.Test", QString(), "org.example.Test",
                             connection);

    QVERIFY(!interface.isValid());
    QVERIFY(!QMetaObject::invokeMethod(&interface, "ListNames", Qt::DirectConnection));
}

void tst_QDBusInterface::notValidDerived()
{
    DerivedFromQDBusInterface c;
    QVERIFY(!c.isValid());
    QMetaObject::invokeMethod(&c, "method", Qt::DirectConnection);
}

void tst_QDBusInterface::invalidAfterServiceOwnerChanged()
{
    // this test is technically the same as tst_QDBusAbstractInterface::followSignal
    QDBusConnection conn = QDBusConnection::sessionBus();
    QDBusConnectionInterface *connIface = conn.interface();

    QDBusInterface validInterface(conn.baseService(), "/");
    QVERIFY(validInterface.isValid());
    QDBusInterface invalidInterface("com.example.Test", "/");
    QVERIFY(!invalidInterface.isValid());

    QTestEventLoop::instance().connect(connIface, SIGNAL(serviceOwnerChanged(QString, QString, QString)),
                                       SLOT(exitLoop()));
    QVERIFY(connIface->registerService("com.example.Test") == QDBusConnectionInterface::ServiceRegistered);

    QTestEventLoop::instance().enterLoop(5);

    QVERIFY(!QTestEventLoop::instance().timeout());
    QVERIFY(invalidInterface.isValid());
}

void tst_QDBusInterface::introspect()
{
    QDBusConnection con = QDBusConnection::sessionBus();
    QDBusInterface iface(QDBusConnection::sessionBus().baseService(), QLatin1String("/"),
                         TEST_INTERFACE_NAME);

    const QMetaObject *mo = iface.metaObject();

    QCOMPARE(mo->methodCount() - mo->methodOffset(), 7);
    QVERIFY(mo->indexOfSignal(TEST_SIGNAL_NAME "(QString)") != -1);

    QCOMPARE(mo->propertyCount() - mo->propertyOffset(), 2);
    QVERIFY(mo->indexOfProperty("prop1") != -1);
    QVERIFY(mo->indexOfProperty("complexProp") != -1);
}

void tst_QDBusInterface::callMethod()
{
    QDBusConnection con = QDBusConnection::sessionBus();
    QDBusInterface iface(QDBusConnection::sessionBus().baseService(), QLatin1String("/"),
                         TEST_INTERFACE_NAME);

    MyObject::callCount = 0;
   
    // call a SLOT method
    QDBusMessage reply = iface.call("ping", qVariantFromValue(QDBusVariant("foo")));
    QCOMPARE(MyObject::callCount, 1);
    QCOMPARE(reply.type(), QDBusMessage::ReplyMessage);

    // verify what the callee received
    QCOMPARE(MyObject::callArgs.count(), 1);
    QVariant v = MyObject::callArgs.at(0);
    QDBusVariant dv = qdbus_cast<QDBusVariant>(v);
    QCOMPARE(dv.variant().type(), QVariant::String);
    QCOMPARE(dv.variant().toString(), QString("foo"));

    // verify reply
    QCOMPARE(reply.arguments().count(), 1);
    v = reply.arguments().at(0);
    dv = qdbus_cast<QDBusVariant>(v);
    QCOMPARE(dv.variant().type(), QVariant::String);
    QCOMPARE(dv.variant().toString(), QString("foo"));
    
    // call an INVOKABLE method
    reply = iface.call("ping_invokable", qVariantFromValue(QDBusVariant("bar")));
    QCOMPARE(MyObject::callCount, 2);
    QCOMPARE(reply.type(), QDBusMessage::ReplyMessage);

    // verify what the callee received
    QCOMPARE(MyObject::callArgs.count(), 1);
    v = MyObject::callArgs.at(0);
    dv = qdbus_cast<QDBusVariant>(v);
    QCOMPARE(dv.variant().type(), QVariant::String);
    QCOMPARE(dv.variant().toString(), QString("bar"));

    // verify reply
    QCOMPARE(reply.arguments().count(), 1);
    v = reply.arguments().at(0);
    dv = qdbus_cast<QDBusVariant>(v);
    QCOMPARE(dv.variant().type(), QVariant::String);
    QCOMPARE(dv.variant().toString(), QString("bar"));
}

void tst_QDBusInterface::invokeMethod()
{
    QDBusConnection con = QDBusConnection::sessionBus();
    QDBusInterface iface(QDBusConnection::sessionBus().baseService(), QLatin1String("/"),
                         TEST_INTERFACE_NAME);

    MyObject::callCount = 0;
    
    // make the SLOT call without a return type
    QDBusVariant arg("foo");
    QVERIFY(QMetaObject::invokeMethod(&iface, "ping", Q_ARG(QDBusVariant, arg)));
    QCOMPARE(MyObject::callCount, 1);

    // verify what the callee received
    QCOMPARE(MyObject::callArgs.count(), 1);
    QVariant v = MyObject::callArgs.at(0);
    QDBusVariant dv = qdbus_cast<QDBusVariant>(v);
    QCOMPARE(dv.variant().type(), QVariant::String);
    QCOMPARE(dv.variant().toString(), QString("foo"));
    
    // make the INVOKABLE call without a return type
    QDBusVariant arg2("bar");
    QVERIFY(QMetaObject::invokeMethod(&iface, "ping_invokable", Q_ARG(QDBusVariant, arg2)));
    QCOMPARE(MyObject::callCount, 2);

    // verify what the callee received
    QCOMPARE(MyObject::callArgs.count(), 1);
    v = MyObject::callArgs.at(0);
    dv = qdbus_cast<QDBusVariant>(v);
    QCOMPARE(dv.variant().type(), QVariant::String);
    QCOMPARE(dv.variant().toString(), QString("bar"));
}

void tst_QDBusInterface::invokeMethodWithReturn()
{
    QDBusConnection con = QDBusConnection::sessionBus();
    QDBusInterface iface(QDBusConnection::sessionBus().baseService(), QLatin1String("/"),
                         TEST_INTERFACE_NAME);

    MyObject::callCount = 0;
    QDBusVariant retArg;

    // make the SLOT call without a return type
    QDBusVariant arg("foo");
    QVERIFY(QMetaObject::invokeMethod(&iface, "ping", Q_RETURN_ARG(QDBusVariant, retArg), Q_ARG(QDBusVariant, arg)));
    QCOMPARE(MyObject::callCount, 1);

    // verify what the callee received
    QCOMPARE(MyObject::callArgs.count(), 1);
    QVariant v = MyObject::callArgs.at(0);
    QDBusVariant dv = qdbus_cast<QDBusVariant>(v);
    QCOMPARE(dv.variant().type(), QVariant::String);
    QCOMPARE(dv.variant().toString(), arg.variant().toString());

    // verify that we got the reply as expected
    QCOMPARE(retArg.variant(), arg.variant());
    
    // make the INVOKABLE call without a return type
    QDBusVariant arg2("bar");
    QVERIFY(QMetaObject::invokeMethod(&iface, "ping_invokable", Q_RETURN_ARG(QDBusVariant, retArg), Q_ARG(QDBusVariant, arg2)));
    QCOMPARE(MyObject::callCount, 2);

    // verify what the callee received
    QCOMPARE(MyObject::callArgs.count(), 1);
    v = MyObject::callArgs.at(0);
    dv = qdbus_cast<QDBusVariant>(v);
    QCOMPARE(dv.variant().type(), QVariant::String);
    QCOMPARE(dv.variant().toString(), arg2.variant().toString());

    // verify that we got the reply as expected
    QCOMPARE(retArg.variant(), arg2.variant());
}

void tst_QDBusInterface::invokeMethodWithMultiReturn()
{
    QDBusConnection con = QDBusConnection::sessionBus();
    QDBusInterface iface(QDBusConnection::sessionBus().baseService(), QLatin1String("/"),
                         TEST_INTERFACE_NAME);

    MyObject::callCount = 0;
    QDBusVariant retArg, retArg2;
    
    // make the SLOT call without a return type
    QDBusVariant arg("foo"), arg2("bar");
    QVERIFY(QMetaObject::invokeMethod(&iface, "ping",
                                      Q_RETURN_ARG(QDBusVariant, retArg),
                                      Q_ARG(QDBusVariant, arg),
                                      Q_ARG(QDBusVariant, arg2),
                                      Q_ARG(QDBusVariant&, retArg2)));
    QCOMPARE(MyObject::callCount, 1);

    // verify what the callee received
    QCOMPARE(MyObject::callArgs.count(), 2);
    QVariant v = MyObject::callArgs.at(0);
    QDBusVariant dv = qdbus_cast<QDBusVariant>(v);
    QCOMPARE(dv.variant().type(), QVariant::String);
    QCOMPARE(dv.variant().toString(), arg.variant().toString());

    v = MyObject::callArgs.at(1);
    dv = qdbus_cast<QDBusVariant>(v);
    QCOMPARE(dv.variant().type(), QVariant::String);
    QCOMPARE(dv.variant().toString(), arg2.variant().toString());

    // verify that we got the replies as expected
    QCOMPARE(retArg.variant(), arg.variant());
    QCOMPARE(retArg2.variant(), arg2.variant());
    
    // make the INVOKABLE call without a return type
    QDBusVariant arg3("hello"), arg4("world");
    QVERIFY(QMetaObject::invokeMethod(&iface, "ping_invokable",
                                      Q_RETURN_ARG(QDBusVariant, retArg),
                                      Q_ARG(QDBusVariant, arg3),
                                      Q_ARG(QDBusVariant, arg4),
                                      Q_ARG(QDBusVariant&, retArg2)));
    QCOMPARE(MyObject::callCount, 2);

    // verify what the callee received
    QCOMPARE(MyObject::callArgs.count(), 2);
    v = MyObject::callArgs.at(0);
    dv = qdbus_cast<QDBusVariant>(v);
    QCOMPARE(dv.variant().type(), QVariant::String);
    QCOMPARE(dv.variant().toString(), arg3.variant().toString());

    v = MyObject::callArgs.at(1);
    dv = qdbus_cast<QDBusVariant>(v);
    QCOMPARE(dv.variant().type(), QVariant::String);
    QCOMPARE(dv.variant().toString(), arg4.variant().toString());

    // verify that we got the replies as expected
    QCOMPARE(retArg.variant(), arg3.variant());
    QCOMPARE(retArg2.variant(), arg4.variant());
}

void tst_QDBusInterface::invokeMethodWithComplexReturn()
{
    QDBusConnection con = QDBusConnection::sessionBus();
    QDBusInterface iface(QDBusConnection::sessionBus().baseService(), QLatin1String("/"),
                         TEST_INTERFACE_NAME);

    MyObject::callCount = 0;
    QList<int> retArg;
    
    // make the SLOT call without a return type
    QList<int> arg = QList<int>() << 42 << -47;
    QVERIFY(QMetaObject::invokeMethod(&iface, "ping", Q_RETURN_ARG(QList<int>, retArg), Q_ARG(QList<int>, arg)));
    QCOMPARE(MyObject::callCount, 1);

    // verify what the callee received
    QCOMPARE(MyObject::callArgs.count(), 1);
    QVariant v = MyObject::callArgs.at(0);
    QCOMPARE(v.userType(), qMetaTypeId<QDBusArgument>());
    QCOMPARE(qdbus_cast<QList<int> >(v), arg);

    // verify that we got the reply as expected
    QCOMPARE(retArg, arg);
    
    // make the INVOKABLE call without a return type
    QList<int> arg2 = QList<int>() << 24 << -74;
    QVERIFY(QMetaObject::invokeMethod(&iface, "ping", Q_RETURN_ARG(QList<int>, retArg), Q_ARG(QList<int>, arg2)));
    QCOMPARE(MyObject::callCount, 2);

    // verify what the callee received
    QCOMPARE(MyObject::callArgs.count(), 1);
    v = MyObject::callArgs.at(0);
    QCOMPARE(v.userType(), qMetaTypeId<QDBusArgument>());
    QCOMPARE(qdbus_cast<QList<int> >(v), arg2);

    // verify that we got the reply as expected
    QCOMPARE(retArg, arg2);
}

void tst_QDBusInterface::signal()
{
    QDBusConnection con = QDBusConnection::sessionBus();
    QDBusInterface iface(QDBusConnection::sessionBus().baseService(), QLatin1String("/"),
                         TEST_INTERFACE_NAME);

    QString arg = "So long and thanks for all the fish";
    {
        Spy spy;
        spy.connect(&iface, SIGNAL(somethingHappened(QString)), SLOT(spySlot(QString)));

        emitSignal(TEST_INTERFACE_NAME, TEST_SIGNAL_NAME, arg);
        QCOMPARE(spy.count, 1);
        QCOMPARE(spy.received, arg);
    }

    QDBusInterface iface2(QDBusConnection::sessionBus().baseService(), QLatin1String("/"),
                          TEST_INTERFACE_NAME);
    {
        Spy spy;
        spy.connect(&iface, SIGNAL(somethingHappened(QString)), SLOT(spySlot(QString)));
        spy.connect(&iface2, SIGNAL(somethingHappened(QString)), SLOT(spySlot(QString)));

        emitSignal(TEST_INTERFACE_NAME, TEST_SIGNAL_NAME, arg);
        QCOMPARE(spy.count, 2);
        QCOMPARE(spy.received, arg);
    }

    {
        Spy spy, spy2;
        spy.connect(&iface, SIGNAL(somethingHappened(QString)), SLOT(spySlot(QString)));
        spy2.connect(&iface2, SIGNAL(somethingHappened(QString)), SLOT(spySlot(QString)));

        emitSignal(TEST_INTERFACE_NAME, TEST_SIGNAL_NAME, arg);
        QCOMPARE(spy.count, 1);
        QCOMPARE(spy.received, arg);
        QCOMPARE(spy2.count, 1);
        QCOMPARE(spy2.received, arg);
    }
}

void tst_QDBusInterface::propertyRead()
{
    QDBusConnection con = QDBusConnection::sessionBus();
    QDBusInterface iface(QDBusConnection::sessionBus().baseService(), QLatin1String("/"),
                         TEST_INTERFACE_NAME);

    int arg = obj.m_prop1 = 42;
    MyObject::callCount = 0;

    QVariant v = iface.property("prop1");
    QVERIFY(v.isValid());
    QCOMPARE(v.userType(), int(QVariant::Int));
    QCOMPARE(v.toInt(), arg);
    QCOMPARE(MyObject::callCount, 1);
}

void tst_QDBusInterface::propertyWrite()
{
    QDBusConnection con = QDBusConnection::sessionBus();
    QDBusInterface iface(QDBusConnection::sessionBus().baseService(), QLatin1String("/"),
                         TEST_INTERFACE_NAME);

    int arg = 42;
    obj.m_prop1 = 0;
    MyObject::callCount = 0;

    QVERIFY(iface.setProperty("prop1", arg));
    QCOMPARE(MyObject::callCount, 1);
    QCOMPARE(obj.m_prop1, arg);
}

void tst_QDBusInterface::complexPropertyRead()
{
    QDBusConnection con = QDBusConnection::sessionBus();
    QDBusInterface iface(QDBusConnection::sessionBus().baseService(), QLatin1String("/"),
                         TEST_INTERFACE_NAME);

    QList<int> arg = obj.m_complexProp = QList<int>() << 42 << -47;
    MyObject::callCount = 0;

    QVariant v = iface.property("complexProp");
    QVERIFY(v.isValid());
    QCOMPARE(v.userType(), qMetaTypeId<QList<int> >());
    QCOMPARE(v.value<QList<int> >(), arg);
    QCOMPARE(MyObject::callCount, 1);
}

void tst_QDBusInterface::complexPropertyWrite()
{
    QDBusConnection con = QDBusConnection::sessionBus();
    QDBusInterface iface(QDBusConnection::sessionBus().baseService(), QLatin1String("/"),
                         TEST_INTERFACE_NAME);

    QList<int> arg = QList<int>() << -47 << 42;
    obj.m_complexProp.clear();
    MyObject::callCount = 0;

    QVERIFY(iface.setProperty("complexProp", qVariantFromValue(arg)));
    QCOMPARE(MyObject::callCount, 1);
    QCOMPARE(obj.m_complexProp, arg);
}

QTEST_MAIN(tst_QDBusInterface)

#include "tst_qdbusinterface.moc"

