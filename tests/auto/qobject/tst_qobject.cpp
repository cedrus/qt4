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


#define QT3_SUPPORT
#include <QtTest/QtTest>


#include <qcoreapplication.h>

#include <qpointer.h>
#include <qtimer.h>
#include <qregexp.h>
#include <qmetaobject.h>
#include <qvariant.h>

#include <QTcpServer>
#include <QTcpSocket>
#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <QProcess>

#include "qobject.h"
#ifdef QT_BUILD_INTERNAL
#include <private/qobject_p.h>
#endif


#include <math.h>

//TESTED_CLASS=
//TESTED_FILES=

class tst_QObject : public QObject
{
    Q_OBJECT

public:
    tst_QObject();
    virtual ~tst_QObject();


public slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();
private slots:
    void disconnect();
    void connectByName();
    void connectSignalsToSignalsWithDefaultArguments();
    void receivers();
    void normalize();
    void qobject_castTemplate();
    void findChildren();
    void connectDisconnectNotify_data();
    void connectDisconnectNotify();
    void emitInDefinedOrder();
    void customTypes();
    void streamCustomTypes();
    void metamethod();
    void namespaces();
    void threadSignalEmissionCrash();
    void thread();
    void thread0();
    void moveToThread();
    void sender();
    void declareInterface();
    void qpointerResetBeforeDestroyedSignal();
    void testUserData();
    void childDeletesItsSibling();
    void dynamicProperties();
    void floatProperty();
    void qrealProperty();
    void property();
    void recursiveSignalEmission();
    void blockingQueuedConnection();
    void compatibilityChildInsertedEvents();
    void installEventFilter();
    void deleteSelfInSlot();
    void disconnectSelfInSlotAndDeleteAfterEmit();
    void dumpObjectInfo();
    void connectToSender();
    void qobjectConstCast();
    void uniqConnection();
    void interfaceIid();
    void deleteQObjectWhenDeletingEvent();
    void overloads();
    void isSignalConnected();
    void qMetaObjectConnect();
    void qMetaObjectDisconnectOne();
    void sameName();
protected:
};

tst_QObject::tst_QObject()
{

}

tst_QObject::~tst_QObject()
{

}

void tst_QObject::initTestCase()
{
}

void tst_QObject::cleanupTestCase()
{
}

void tst_QObject::init()
{
}

void tst_QObject::cleanup()
{
}

class SenderObject : public QObject
{
    Q_OBJECT

public:
    SenderObject() : aPublicSlotCalled(0), recursionCount(0) {}

    void emitSignal1AfterRecursion()
    {
        if (recursionCount++ < 100)
            emitSignal1AfterRecursion();
        else
            emitSignal1();
    }

    void emitSignal1() { emit signal1(); }
    void emitSignal2() { emit signal2(); }
    void emitSignal3() { emit signal3(); }
    void emitSignal4() { emit signal4(); }

signals:
    void signal1();
    void signal2();
    void signal3();
    void signal4();
    QT_MOC_COMPAT void signal5();

public slots:
    void aPublicSlot() { aPublicSlotCalled++; }

public:
    Q_INVOKABLE void invoke1(){}
    Q_SCRIPTABLE void sinvoke1(){}
    int aPublicSlotCalled;
protected:
    Q_INVOKABLE QT_MOC_COMPAT void invoke2(){}
    Q_INVOKABLE QT_MOC_COMPAT void invoke2(int){}
    Q_SCRIPTABLE QT_MOC_COMPAT void sinvoke2(){}
private:
    Q_INVOKABLE void invoke3(int hinz = 0, int kunz = 0){Q_UNUSED(hinz) Q_UNUSED(kunz)}
    Q_SCRIPTABLE void sinvoke3(){}

    int recursionCount;
};

class ReceiverObject : public QObject
{
    Q_OBJECT

public:
    ReceiverObject() : sequence_slot1( 0 ),
		       sequence_slot2( 0 ),
		       sequence_slot3( 0 ),
		       sequence_slot4( 0 ) {}

    void reset() {
	sequence_slot4 = 0;
	sequence_slot3 = 0;
	sequence_slot2 = 0;
	sequence_slot1 = 0;
        count_slot1 = 0;
        count_slot2 = 0;
        count_slot3 = 0;
        count_slot4 = 0;
    }

    int sequence_slot1;
    int sequence_slot2;
    int sequence_slot3;
    int sequence_slot4;
    int count_slot1;
    int count_slot2;
    int count_slot3;
    int count_slot4;

    bool called(int slot) {
        switch (slot) {
        case 1: return sequence_slot1;
        case 2: return sequence_slot2;
        case 3: return sequence_slot3;
        case 4: return sequence_slot4;
        default: return false;
        }
    }

    static int sequence;

public slots:
    void slot1() { sequence_slot1 = ++sequence; count_slot1++; }
    void slot2() { sequence_slot2 = ++sequence; count_slot2++; }
    void slot3() { sequence_slot3 = ++sequence; count_slot3++; }
    void slot4() { sequence_slot4 = ++sequence; count_slot4++; }

};

int ReceiverObject::sequence = 0;

void tst_QObject::disconnect()
{
    SenderObject *s = new SenderObject;
    ReceiverObject *r1 = new ReceiverObject;
    ReceiverObject *r2 = new ReceiverObject;

    connect( s, SIGNAL( signal1() ), r1, SLOT( slot1() ) );

    connect( s, SIGNAL( signal2() ), r1, SLOT( slot2() ) );
    connect( s, SIGNAL( signal3() ), r1, SLOT( slot3() ) );
    connect( s, SIGNAL( signal4() ), r1, SLOT( slot4() ) );

    s->emitSignal1();
    s->emitSignal2();
    s->emitSignal3();
    s->emitSignal4();

    QCOMPARE( r1->called(1), TRUE );
    QCOMPARE( r1->called(2), TRUE );
    QCOMPARE( r1->called(3), TRUE );
    QCOMPARE( r1->called(4), TRUE );
    r1->reset();

    // usual disconnect with all parameters given
    bool ret = QObject::disconnect( s, SIGNAL( signal1() ), r1, SLOT( slot1() ) );

    s->emitSignal1();

    QCOMPARE( r1->called(1), FALSE );
    r1->reset();

    QCOMPARE( ret, TRUE );
    ret = QObject::disconnect( s, SIGNAL( signal1() ), r1, SLOT( slot1() ) );
    QCOMPARE( ret, FALSE  );

    // disconnect all signals from s from all slots from r1
    QObject::disconnect( s, 0, r1, 0 );

    s->emitSignal2();
    s->emitSignal3();
    s->emitSignal4();

    QCOMPARE( r1->called(2), FALSE );
    QCOMPARE( r1->called(3), FALSE );
    QCOMPARE( r1->called(4), FALSE );
    r1->reset();

    connect( s, SIGNAL( signal1() ), r1, SLOT( slot1() ) );
    connect( s, SIGNAL( signal1() ), r1, SLOT( slot2() ) );
    connect( s, SIGNAL( signal1() ), r1, SLOT( slot3() ) );
    connect( s, SIGNAL( signal2() ), r1, SLOT( slot4() ) );

    // disconnect s's signal1() from all slots of r1
    QObject::disconnect( s, SIGNAL( signal1() ), r1, 0 );

    s->emitSignal1();
    s->emitSignal2();

    QCOMPARE( r1->called(1), FALSE );
    QCOMPARE( r1->called(2), FALSE );
    QCOMPARE( r1->called(3), FALSE );
    QCOMPARE( r1->called(4), TRUE );
    r1->reset();
    // make sure all is disconnected again
    QObject::disconnect( s, 0, r1, 0 );

    connect( s, SIGNAL( signal1() ), r1, SLOT( slot1() ) );
    connect( s, SIGNAL( signal1() ), r2, SLOT( slot1() ) );
    connect( s, SIGNAL( signal2() ), r1, SLOT( slot2() ) );
    connect( s, SIGNAL( signal2() ), r2, SLOT( slot2() ) );
    connect( s, SIGNAL( signal3() ), r1, SLOT( slot3() ) );
    connect( s, SIGNAL( signal3() ), r2, SLOT( slot3() ) );

    // disconnect signal1() from all receivers
    QObject::disconnect( s, SIGNAL( signal1() ), 0, 0 );
    s->emitSignal1();
    s->emitSignal2();
    s->emitSignal3();

    QCOMPARE( r1->called(1), FALSE );
    QCOMPARE( r2->called(1), FALSE );
    QCOMPARE( r1->called(2), TRUE );
    QCOMPARE( r2->called(2), TRUE );
    QCOMPARE( r1->called(2), TRUE );
    QCOMPARE( r2->called(2), TRUE );

    r1->reset();
    r2->reset();

    // disconnect all signals of s from all receivers
    QObject::disconnect( s, 0, 0, 0 );

    QCOMPARE( r1->called(2), FALSE );
    QCOMPARE( r2->called(2), FALSE );
    QCOMPARE( r1->called(2), FALSE );
    QCOMPARE( r2->called(2), FALSE );

    delete r2;
    delete r1;
    delete s;
}

class AutoConnectSender : public QObject
{
    Q_OBJECT

public:
    AutoConnectSender(QObject *parent)
        : QObject(parent)
    {}

    void emitSignalNoParams() { emit signalNoParams(); }
    void emitSignalWithParams(int i) { emit signalWithParams(i); }
    void emitSignalWithParams(int i, QString string) { emit signalWithParams(i, string); }
    void emitSignalManyParams(int i1, int i2, int i3, QString string, bool onoff) { emit signalManyParams(i1, i2, i3, string, onoff); }
    void emitSignalManyParams2(int i1, int i2, int i3, QString string, bool onoff) { emit signalManyParams2(i1, i2, i3, string, onoff); }
    void emitSignalLoopBack() { emit signalLoopBack(); }

signals:
    void signalNoParams();
    void signalWithParams(int i);
    void signalWithParams(int i, QString string);
    void signalManyParams(int i1, int i2, int i3, QString string, bool onoff);
    void signalManyParams(int i1, int i2, int i3, QString string, bool onoff, bool);
    void signalManyParams2(int i1, int i2, int i3, QString string, bool onoff);
    void signalLoopBack();
};

class AutoConnectReceiver : public QObject
{
    Q_OBJECT

public:
    AutoConnectReceiver()
    {
        reset();

        connect(this, SIGNAL(on_Sender_signalLoopBack()), this, SLOT(slotLoopBack()));
    }

    void reset() {
        called_slot8 = 0;
        called_slot7 = 0;
        called_slot6 = 0;
        called_slot5 = 0;
        called_slot4 = 0;
        called_slot3 = 0;
        called_slot2 = 0;
        called_slot1 = 0;
    }

    int called_slot1;
    int called_slot2;
    int called_slot3;
    int called_slot4;
    int called_slot5;
    int called_slot6;
    int called_slot7;
    int called_slot8;

    bool called(int slot) {
        switch (slot) {
        case 1: return called_slot1;
        case 2: return called_slot2;
        case 3: return called_slot3;
        case 4: return called_slot4;
        case 5: return called_slot5;
        case 6: return called_slot6;
        case 7: return called_slot7;
        case 8: return called_slot8;
        default: return false;
        }
    }

public slots:
    void on_Sender_signalNoParams() { ++called_slot1; }
    void on_Sender_signalWithParams(int i = 0) { ++called_slot2; Q_UNUSED(i); }
    void on_Sender_signalWithParams(int i, QString string) { ++called_slot3; Q_UNUSED(i);Q_UNUSED(string); }
    void on_Sender_signalManyParams() { ++called_slot4; }
    void on_Sender_signalManyParams(int i1, int i2, int i3, QString string, bool onoff) { ++called_slot5; Q_UNUSED(i1);Q_UNUSED(i2);Q_UNUSED(i3);Q_UNUSED(string);Q_UNUSED(onoff); }
    void on_Sender_signalManyParams(int i1, int i2, int i3, QString string, bool onoff, bool dummy)
    { ++called_slot6; Q_UNUSED(i1);Q_UNUSED(i2);Q_UNUSED(i3);Q_UNUSED(string);Q_UNUSED(onoff); Q_UNUSED(dummy);}
    void on_Sender_signalManyParams2(int i1, int i2, int i3, QString string, bool onoff)
    { ++called_slot7; Q_UNUSED(i1);Q_UNUSED(i2);Q_UNUSED(i3);Q_UNUSED(string);Q_UNUSED(onoff); }
    void slotLoopBack() { ++called_slot8; }

protected slots:
    void o() { Q_ASSERT(0); }
    void on() { Q_ASSERT(0); }

signals:
    void on_Sender_signalLoopBack();
};

void tst_QObject::connectByName()
{
    AutoConnectReceiver receiver;
    AutoConnectSender sender(&receiver);
    sender.setObjectName("Sender");

    QMetaObject::connectSlotsByName(&receiver);

    sender.emitSignalNoParams();
    QCOMPARE(receiver.called(1), true);
    QCOMPARE(receiver.called(2), false);
    QCOMPARE(receiver.called(3), false);
    QCOMPARE(receiver.called(4), false);
    QCOMPARE(receiver.called(5), false);
    QCOMPARE(receiver.called(6), false);
    QCOMPARE(receiver.called(7), false);
    QCOMPARE(receiver.called(8), false);
    receiver.reset();

    sender.emitSignalWithParams(0);
    QCOMPARE(receiver.called(1), false);
    QCOMPARE(receiver.called(2), true);
    QCOMPARE(receiver.called(3), false);
    QCOMPARE(receiver.called(4), false);
    QCOMPARE(receiver.called(5), false);
    QCOMPARE(receiver.called(6), false);
    QCOMPARE(receiver.called(7), false);
    QCOMPARE(receiver.called(8), false);
    receiver.reset();

    sender.emitSignalWithParams(0, "string");
    QCOMPARE(receiver.called(1), false);
    QCOMPARE(receiver.called(2), false);
    QCOMPARE(receiver.called(3), true);
    QCOMPARE(receiver.called(4), false);
    QCOMPARE(receiver.called(5), false);
    QCOMPARE(receiver.called(6), false);
    QCOMPARE(receiver.called(7), false);
    QCOMPARE(receiver.called(8), false);
    receiver.reset();

    sender.emitSignalManyParams(1, 2, 3, "string", true);
    QCOMPARE(receiver.called(1), false);
    QCOMPARE(receiver.called(2), false);
    QCOMPARE(receiver.called(3), false);
    QCOMPARE(receiver.called(4), true);
    QCOMPARE(receiver.called(5), true);
    QCOMPARE(receiver.called(6), false);
    QCOMPARE(receiver.called(7), false);
    QCOMPARE(receiver.called(8), false);
    receiver.reset();

    sender.emitSignalManyParams2(1, 2, 3, "string", true);
    QCOMPARE(receiver.called(1), false);
    QCOMPARE(receiver.called(2), false);
    QCOMPARE(receiver.called(3), false);
    QCOMPARE(receiver.called(4), false);
    QCOMPARE(receiver.called(5), false);
    QCOMPARE(receiver.called(6), false);
    QCOMPARE(receiver.called(7), true);
    QCOMPARE(receiver.called(8), false);
    receiver.reset();

    sender.emitSignalLoopBack();
    QCOMPARE(receiver.called(1), false);
    QCOMPARE(receiver.called(2), false);
    QCOMPARE(receiver.called(3), false);
    QCOMPARE(receiver.called(4), false);
    QCOMPARE(receiver.called(5), false);
    QCOMPARE(receiver.called(6), false);
    QCOMPARE(receiver.called(7), false);
    QCOMPARE(receiver.called(8), true);
    receiver.reset();
}

void tst_QObject::qobject_castTemplate()
{
    QObject *o = 0;
    QVERIFY( !::qobject_cast<QObject*>(o) );

    o = new SenderObject;
    QVERIFY( ::qobject_cast<SenderObject*>(o) );
    QVERIFY( ::qobject_cast<QObject*>(o) );
    QVERIFY( !::qobject_cast<ReceiverObject*>(o) );
    delete o;
}

void tst_QObject::findChildren()
{
    QObject o;
    QObject o1(&o);
    QObject o2(&o);
    QObject o11(&o1);
    QObject o12(&o1);
    QObject o111(&o11);
    QObject unnamed(&o);
    QTimer t1(&o);
    QTimer t121(&o12);
    QTimer emptyname(&o);

    o.setObjectName("o");
    o1.setObjectName("o1");
    o2.setObjectName("o2");
    o11.setObjectName("o11");
    o12.setObjectName("o12");
    o111.setObjectName("o111");
    t1.setObjectName("t1");
    t121.setObjectName("t121");
    emptyname.setObjectName("");

    QObject *op = 0;

    op = qFindChild<QObject*>(&o, "o1");
    QCOMPARE(op, &o1);
    op = qFindChild<QObject*>(&o, "o2");
    QCOMPARE(op, &o2);
    op = qFindChild<QObject*>(&o, "o11");
    QCOMPARE(op, &o11);
    op = qFindChild<QObject*>(&o, "o12");
    QCOMPARE(op, &o12);
    op = qFindChild<QObject*>(&o, "o111");
    QCOMPARE(op, &o111);
    op = qFindChild<QObject*>(&o, "t1");
    QCOMPARE(op, static_cast<QObject *>(&t1));
    op = qFindChild<QObject*>(&o, "t121");
    QCOMPARE(op, static_cast<QObject *>(&t121));
    op = qFindChild<QTimer*>(&o, "t1");
    QCOMPARE(op, static_cast<QObject *>(&t1));
    op = qFindChild<QTimer*>(&o, "t121");
    QCOMPARE(op, static_cast<QObject *>(&t121));
    op = qFindChild<QTimer*>(&o, "o12");
    QCOMPARE(op, static_cast<QObject *>(0));
    op = qFindChild<QObject*>(&o, "o");
    QCOMPARE(op, static_cast<QObject *>(0));
    op = qFindChild<QObject*>(&o, "harry");
    QCOMPARE(op, static_cast<QObject *>(0));
    op = qFindChild<QObject*>(&o, "o1");
    QCOMPARE(op, &o1);

    QList<QObject*> l;
    QList<QTimer*> tl;

    l = qFindChildren<QObject*>(&o, "o1");
    QCOMPARE(l.size(), 1);
    QCOMPARE(l.at(0), &o1);
    l = qFindChildren<QObject*>(&o, "o2");
    QCOMPARE(l.size(), 1);
    QCOMPARE(l.at(0), &o2);
    l = qFindChildren<QObject*>(&o, "o11");
    QCOMPARE(l.size(), 1);
    QCOMPARE(l.at(0), &o11);
    l = qFindChildren<QObject*>(&o, "o12");
    QCOMPARE(l.size(), 1);
    QCOMPARE(l.at(0), &o12);
    l = qFindChildren<QObject*>(&o, "o111");
    QCOMPARE(l.size(), 1);
    QCOMPARE(l.at(0), &o111);
    l = qFindChildren<QObject*>(&o, "t1");
    QCOMPARE(l.size(), 1);
    QCOMPARE(l.at(0), static_cast<QObject *>(&t1));
    l = qFindChildren<QObject*>(&o, "t121");
    QCOMPARE(l.size(), 1);
    QCOMPARE(l.at(0), static_cast<QObject *>(&t121));
    tl = qFindChildren<QTimer*>(&o, "t1");
    QCOMPARE(tl.size(), 1);
    QCOMPARE(tl.at(0), &t1);
    tl = qFindChildren<QTimer*>(&o, "t121");
    QCOMPARE(tl.size(), 1);
    QCOMPARE(tl.at(0), &t121);
    l = qFindChildren<QObject*>(&o, "o");
    QCOMPARE(l.size(), 0);
    l = qFindChildren<QObject*>(&o, "harry");
    QCOMPARE(l.size(), 0);
    tl = qFindChildren<QTimer*>(&o, "o12");
    QCOMPARE(tl.size(), 0);
    l = qFindChildren<QObject*>(&o, "o1");
    QCOMPARE(l.size(), 1);
    QCOMPARE(l.at(0), &o1);

    l = qFindChildren<QObject*>(&o, QRegExp("o.*"));
    QCOMPARE(l.size(), 5);
    QVERIFY(l.contains(&o1));
    QVERIFY(l.contains(&o2));
    QVERIFY(l.contains(&o11));
    QVERIFY(l.contains(&o12));
    QVERIFY(l.contains(&o111));
    l = qFindChildren<QObject*>(&o, QRegExp("t.*"));
    QCOMPARE(l.size(), 2);
    QVERIFY(l.contains(&t1));
    QVERIFY(l.contains(&t121));
    tl = qFindChildren<QTimer*>(&o, QRegExp(".*"));
    QCOMPARE(tl.size(), 3);
    QVERIFY(tl.contains(&t1));
    QVERIFY(tl.contains(&t121));
    tl = qFindChildren<QTimer*>(&o, QRegExp("o.*"));
    QCOMPARE(tl.size(), 0);
    l = qFindChildren<QObject*>(&o, QRegExp("harry"));
    QCOMPARE(l.size(), 0);

    // empty and null string check
    op = qFindChild<QObject*>(&o);
    QCOMPARE(op, &o1);
    op = qFindChild<QObject*>(&o, "");
    QCOMPARE(op, &unnamed);
    op = qFindChild<QObject*>(&o, "unnamed");
    QCOMPARE(op, static_cast<QObject *>(0));

    l = qFindChildren<QObject*>(&o);
    QCOMPARE(l.size(), 9);
    l = qFindChildren<QObject*>(&o, "");
    QCOMPARE(l.size(), 2);
    l = qFindChildren<QObject*>(&o, "unnamed");
    QCOMPARE(l.size(), 0);

#ifndef QT_NO_MEMBER_TEMPLATES
    tl = o.findChildren<QTimer *>("t1");
    QCOMPARE(tl.size(), 1);
    QCOMPARE(tl.at(0), &t1);
#endif
}


class NotifyObject : public SenderObject, public ReceiverObject
{
public:
    NotifyObject() : SenderObject(), ReceiverObject()
    {}

    QString org_signal;
    QString nw_signal;

protected:
    void connectNotify( const char *signal )
    {
    	org_signal = signal;
    	nw_signal = QMetaObject::normalizedSignature(signal);
    };
    void disconnectNotify( const char *signal )
    {
    	org_signal = signal;
    	nw_signal = QMetaObject::normalizedSignature(signal);
    };
};

void tst_QObject::connectDisconnectNotify_data()
{
    QTest::addColumn<QString>("a_signal");
    QTest::addColumn<QString>("a_slot");

    QTest::newRow("combo1") << SIGNAL( signal1() )        << SLOT( slot1() );
    QTest::newRow("combo2") << SIGNAL( signal2(void) )    << SLOT( slot2(  ) );
    QTest::newRow("combo3") << SIGNAL( signal3(  ) )      << SLOT( slot3(void) );
    QTest::newRow("combo4") << SIGNAL(  signal4( void )  )<< SLOT(  slot4( void )  );
}

void tst_QObject::connectDisconnectNotify()
{
    NotifyObject *s = new NotifyObject;
    NotifyObject *r = new NotifyObject;

    QFETCH(QString, a_signal);
    QFETCH(QString, a_slot);

    // Test connectNotify
    connect( (SenderObject*)s, a_signal.toLatin1(), (ReceiverObject*)r, a_slot.toLatin1() );
    QCOMPARE( s->org_signal, s->nw_signal );
    QCOMPARE( s->org_signal.toLatin1(), QMetaObject::normalizedSignature(a_signal.toLatin1().constData()) );

    // Test disconnectNotify
    QObject::disconnect( (SenderObject*)s, a_signal.toLatin1(), (ReceiverObject*)r, a_slot.toLatin1() );
    QCOMPARE( s->org_signal, s->nw_signal );
    QCOMPARE( s->org_signal.toLatin1(), QMetaObject::normalizedSignature(a_signal.toLatin1().constData()) );

    // Reconnect
    connect( (SenderObject*)s, a_signal.toLatin1(), (ReceiverObject*)r, a_slot.toLatin1() );
    // Test disconnectNotify for a complete disconnect
    ((SenderObject*)s)->disconnect((ReceiverObject*)r);

    delete s;
    delete r;
}

class SequenceObject : public ReceiverObject
{
    Q_OBJECT

public:
    QObject *next;
    SequenceObject() : next(0) { }

public slots:
    void slot1_disconnectThis()
    {
        slot1();
        disconnect(sender(), SIGNAL(signal1()), this, SLOT(slot1_disconnectThis()));
    }

    void slot2_reconnectThis()
    {
        slot2();

        const QObject *s = sender();
        disconnect(s, SIGNAL(signal1()), this, SLOT(slot2_reconnectThis()));
        connect(s, SIGNAL(signal1()), this, SLOT(slot2_reconnectThis()));
    }

    void slot1_disconnectNext()
    {
        slot1();
        disconnect(sender(), SIGNAL(signal1()), next, SLOT(slot1()));
    }

    void slot2_reconnectNext()
    {
        slot2();

        // modify the connection list in 'this'
        disconnect(sender(), SIGNAL(signal1()), next, SLOT(slot2()));
        connect(sender(), SIGNAL(signal1()), next, SLOT(slot2()));

        // modify the sender list in 'this'
        connect(next, SIGNAL(destroyed()), this, SLOT(deleteLater()));
        connect(QCoreApplication::instance(), SIGNAL(aboutToQuit()), this, SLOT(deleteLater()));
        disconnect(next, SIGNAL(destroyed()), this, SLOT(deleteLater()));
        disconnect(QCoreApplication::instance(), SIGNAL(aboutToQuit()), this, SLOT(deleteLater()));
    }

    void slot1_deleteNext()
    {
        slot1();
        delete next;
    }

    void slot2_deleteSender()
    {
        slot2();
        delete sender();
    }
};

void tst_QObject::emitInDefinedOrder()
{
    SenderObject sender;
    ReceiverObject receiver1, receiver2, receiver3, receiver4;

    connect(&sender, SIGNAL(signal1()), &receiver1, SLOT(slot1()));
    connect(&sender, SIGNAL(signal1()), &receiver2, SLOT(slot1()));
    connect(&sender, SIGNAL(signal1()), &receiver3, SLOT(slot1()));
    connect(&sender, SIGNAL(signal1()), &receiver4, SLOT(slot1()));
    connect(&sender, SIGNAL(signal1()), &receiver1, SLOT(slot2()));
    connect(&sender, SIGNAL(signal1()), &receiver2, SLOT(slot2()));
    connect(&sender, SIGNAL(signal1()), &receiver3, SLOT(slot2()));
    connect(&sender, SIGNAL(signal1()), &receiver4, SLOT(slot2()));

    int sequence;
    ReceiverObject::sequence = sequence = 0;
    sender.emitSignal1();
    QCOMPARE(receiver1.sequence_slot1, ++sequence);
    QCOMPARE(receiver2.sequence_slot1, ++sequence);
    QCOMPARE(receiver3.sequence_slot1, ++sequence);
    QCOMPARE(receiver4.sequence_slot1, ++sequence);
    QCOMPARE(receiver1.sequence_slot2, ++sequence);
    QCOMPARE(receiver2.sequence_slot2, ++sequence);
    QCOMPARE(receiver3.sequence_slot2, ++sequence);
    QCOMPARE(receiver4.sequence_slot2, ++sequence);

    QObject::disconnect(&sender, SIGNAL(signal1()), &receiver2, SLOT(slot1()));
    connect(&sender, SIGNAL(signal1()), &receiver2, SLOT(slot1()));

    ReceiverObject::sequence = sequence =  0;
    sender.emitSignal1();
    QCOMPARE(receiver1.sequence_slot1, ++sequence);
    QCOMPARE(receiver3.sequence_slot1, ++sequence);
    QCOMPARE(receiver4.sequence_slot1, ++sequence);
    QCOMPARE(receiver1.sequence_slot2, ++sequence);
    QCOMPARE(receiver2.sequence_slot2, ++sequence);
    QCOMPARE(receiver3.sequence_slot2, ++sequence);
    QCOMPARE(receiver4.sequence_slot2, ++sequence);
    QCOMPARE(receiver2.sequence_slot1, ++sequence);

    QObject::disconnect(&sender, SIGNAL(signal1()), &receiver1, SLOT(slot1()));
    connect(&sender, SIGNAL(signal1()), &receiver1, SLOT(slot1()));

    ReceiverObject::sequence = sequence =  0;
    sender.emitSignal1();
    QCOMPARE(receiver3.sequence_slot1, ++sequence);
    QCOMPARE(receiver4.sequence_slot1, ++sequence);
    QCOMPARE(receiver1.sequence_slot2, ++sequence);
    QCOMPARE(receiver2.sequence_slot2, ++sequence);
    QCOMPARE(receiver3.sequence_slot2, ++sequence);
    QCOMPARE(receiver4.sequence_slot2, ++sequence);
    QCOMPARE(receiver2.sequence_slot1, ++sequence);
    QCOMPARE(receiver1.sequence_slot1, ++sequence);

    // ensure emission order even if the connections change during emission
    SenderObject *sender2 = new SenderObject;
    SequenceObject seq1, seq2, *seq3 = new SequenceObject, seq4;
    seq1.next = &seq2;
    seq2.next = seq3;
    seq3->next = &seq4;

    // try 1
    connect(sender2, SIGNAL(signal1()), &seq1, SLOT(slot1_disconnectThis()));
    connect(sender2, SIGNAL(signal1()), &seq2, SLOT(slot1_disconnectNext()));
    connect(sender2, SIGNAL(signal1()), seq3, SLOT(slot1()));
    connect(sender2, SIGNAL(signal1()), &seq4, SLOT(slot1()));
    connect(sender2, SIGNAL(signal1()), &seq1, SLOT(slot2_reconnectThis()));
    connect(sender2, SIGNAL(signal1()), &seq2, SLOT(slot2_reconnectNext()));
    connect(sender2, SIGNAL(signal1()), seq3, SLOT(slot2()));
    connect(sender2, SIGNAL(signal1()), &seq4, SLOT(slot2()));

    SequenceObject::sequence = sequence = 0;
    sender2->emitSignal1();
    QCOMPARE(seq1.called(1), TRUE);
    QCOMPARE(seq2.called(1), TRUE);
    QCOMPARE(seq3->called(1), FALSE);
    QCOMPARE(seq4.called(1), TRUE);
    QCOMPARE(seq1.called(2), TRUE);
    QCOMPARE(seq2.called(2), TRUE);
    QCOMPARE(seq3->called(2), FALSE);
    QCOMPARE(seq4.called(2), TRUE);
    QCOMPARE(seq1.sequence_slot1, ++sequence);
    QCOMPARE(seq2.sequence_slot1, ++sequence);
    QCOMPARE(seq4.sequence_slot1, ++sequence);
    QCOMPARE(seq1.sequence_slot2, ++sequence);
    QCOMPARE(seq2.sequence_slot2, ++sequence);
    QCOMPARE(seq4.sequence_slot2, ++sequence);

    QObject::disconnect(sender2, 0, &seq1, 0);
    QObject::disconnect(sender2, 0, &seq2, 0);
    QObject::disconnect(sender2, 0, seq3, 0);
    QObject::disconnect(sender2, 0, &seq4, 0);
    seq1.reset();
    seq2.reset();
    seq3->reset();
    seq4.reset();

    // try 2
    connect(sender2, SIGNAL(signal1()), &seq1, SLOT(slot2_reconnectThis()));
    connect(sender2, SIGNAL(signal1()), &seq2, SLOT(slot2_reconnectNext()));
    connect(sender2, SIGNAL(signal1()), seq3, SLOT(slot2()));
    connect(sender2, SIGNAL(signal1()), &seq4, SLOT(slot2()));
    connect(sender2, SIGNAL(signal1()), &seq1, SLOT(slot1_disconnectThis()));
    connect(sender2, SIGNAL(signal1()), &seq2, SLOT(slot1_disconnectNext()));
    connect(sender2, SIGNAL(signal1()), seq3, SLOT(slot1()));
    connect(sender2, SIGNAL(signal1()), &seq4, SLOT(slot1()));

    SequenceObject::sequence = sequence = 0;
    sender2->emitSignal1();
    QCOMPARE(seq1.called(2), TRUE);
    QCOMPARE(seq2.called(2), TRUE);
    QCOMPARE(seq3->called(2), FALSE);
    QCOMPARE(seq4.called(2), TRUE);
    QCOMPARE(seq1.called(1), TRUE);
    QCOMPARE(seq2.called(1), TRUE);
    QCOMPARE(seq3->called(1), FALSE);
    QCOMPARE(seq4.called(1), TRUE);
    QCOMPARE(seq1.sequence_slot2, ++sequence);
    QCOMPARE(seq2.sequence_slot2, ++sequence);
    QCOMPARE(seq4.sequence_slot2, ++sequence);
    QCOMPARE(seq1.sequence_slot1, ++sequence);
    QCOMPARE(seq2.sequence_slot1, ++sequence);
    QCOMPARE(seq4.sequence_slot1, ++sequence);

    QObject::disconnect(sender2, 0, &seq1, 0);
    QObject::disconnect(sender2, 0, &seq2, 0);
    QObject::disconnect(sender2, 0, seq3, 0);
    QObject::disconnect(sender2, 0, &seq4, 0);
    seq1.reset();
    seq2.reset();
    seq3->reset();
    seq4.reset();

    // try 3
    connect(sender2, SIGNAL(signal1()), &seq1, SLOT(slot1()));
    connect(sender2, SIGNAL(signal1()), &seq2, SLOT(slot1_disconnectNext()));
    connect(sender2, SIGNAL(signal1()), seq3, SLOT(slot1()));
    connect(sender2, SIGNAL(signal1()), &seq4, SLOT(slot1()));
    connect(sender2, SIGNAL(signal1()), &seq1, SLOT(slot2()));
    connect(sender2, SIGNAL(signal1()), &seq2, SLOT(slot2_reconnectNext()));
    connect(sender2, SIGNAL(signal1()), seq3, SLOT(slot2()));
    connect(sender2, SIGNAL(signal1()), &seq4, SLOT(slot2()));

    SequenceObject::sequence = sequence = 0;
    sender2->emitSignal1();
    QCOMPARE(seq1.called(1), TRUE);
    QCOMPARE(seq2.called(1), TRUE);
    QCOMPARE(seq3->called(1), FALSE);
    QCOMPARE(seq4.called(1), TRUE);
    QCOMPARE(seq1.called(2), TRUE);
    QCOMPARE(seq2.called(2), TRUE);
    QCOMPARE(seq3->called(2), FALSE);
    QCOMPARE(seq4.called(2), TRUE);
    QCOMPARE(seq1.sequence_slot1, ++sequence);
    QCOMPARE(seq2.sequence_slot1, ++sequence);
    QCOMPARE(seq4.sequence_slot1, ++sequence);
    QCOMPARE(seq1.sequence_slot2, ++sequence);
    QCOMPARE(seq2.sequence_slot2, ++sequence);
    QCOMPARE(seq4.sequence_slot2, ++sequence);

    // ensure emission order even if objects are destroyed during emission
    QObject::disconnect(sender2, 0, &seq1, 0);
    QObject::disconnect(sender2, 0, &seq2, 0);
    QObject::disconnect(sender2, 0, seq3, 0);
    QObject::disconnect(sender2, 0, &seq4, 0);
    seq1.reset();
    seq2.reset();
    seq3->reset();
    seq4.reset();

    connect(sender2, SIGNAL(signal1()), &seq1, SLOT(slot1()));
    connect(sender2, SIGNAL(signal1()), &seq2, SLOT(slot1_deleteNext()));
    connect(sender2, SIGNAL(signal1()), seq3, SLOT(slot1()));
    connect(sender2, SIGNAL(signal1()), &seq4, SLOT(slot1()));
    connect(sender2, SIGNAL(signal1()), &seq1, SLOT(slot2()));
    connect(sender2, SIGNAL(signal1()), &seq2, SLOT(slot2_deleteSender()));
    connect(sender2, SIGNAL(signal1()), seq3, SLOT(slot2()));
    connect(sender2, SIGNAL(signal1()), &seq4, SLOT(slot2()));

    QPointer<SenderObject> psender = sender2;
    QPointer<SequenceObject> pseq3 = seq3;

    SequenceObject::sequence = sequence = 0;
    sender2->emitSignal1();
    QCOMPARE(static_cast<QObject *>(psender), static_cast<QObject *>(0));
    QCOMPARE(static_cast<QObject *>(pseq3), static_cast<QObject *>(0));
    QCOMPARE(seq1.called(1), TRUE);
    QCOMPARE(seq2.called(1), TRUE);
    QCOMPARE(seq4.called(1), TRUE);
    QCOMPARE(seq1.called(2), TRUE);
    QCOMPARE(seq2.called(2), TRUE);
    QCOMPARE(seq4.called(2), FALSE);
    QCOMPARE(seq1.sequence_slot1, ++sequence);
    QCOMPARE(seq2.sequence_slot1, ++sequence);
    QCOMPARE(seq4.sequence_slot1, ++sequence);
    QCOMPARE(seq1.sequence_slot2, ++sequence);
    QCOMPARE(seq2.sequence_slot2, ++sequence);

    QPointer<SenderObject> psender3 = new SenderObject;
    connect(psender3, SIGNAL(signal1()), psender3, SIGNAL(signal2()));
    connect(psender3, SIGNAL(signal2()), &seq1, SLOT(slot2_deleteSender()));
    psender3->emitSignal1();
    QVERIFY(!psender3);
}

static int instanceCount = 0;

struct CustomType
{
    CustomType(int l1 = 0, int l2 = 0, int l3 = 0): i1(l1), i2(l2), i3(l3)
    { ++instanceCount; }
    CustomType(const CustomType &other): i1(other.i1), i2(other.i2), i3(other.i3)
    { ++instanceCount; }
    ~CustomType() { --instanceCount; }

    int i1, i2, i3;
    int value() { return i1 + i2 + i3; }
};

Q_DECLARE_METATYPE(CustomType*)

class QCustomTypeChecker: public QObject
{
    Q_OBJECT

public:
    QCustomTypeChecker(QObject *parent = 0): QObject(parent) {}
    void doEmit(CustomType ct)
    { emit signal1(ct); }

public slots:
    void slot1(CustomType ct);

signals:
    void signal1(CustomType ct);

public:
    CustomType received;
};

void QCustomTypeChecker::slot1(CustomType ct)
{ received = ct; }


void tst_QObject::customTypes()
{
    CustomType t0;
    CustomType t1(1, 2, 3);
    CustomType t2(2, 3, 4);

    {
        QCustomTypeChecker checker;
        QCOMPARE(instanceCount, 4);

        connect(&checker, SIGNAL(signal1(CustomType)), &checker, SLOT(slot1(CustomType)),
                Qt::DirectConnection);
        QCOMPARE(checker.received.value(), 0);
        checker.doEmit(t1);
        QCOMPARE(checker.received.value(), t1.value());
        checker.received = t0;

        int idx = qRegisterMetaType<CustomType>("CustomType");
        QCOMPARE(QMetaType::type("CustomType"), idx);

        checker.disconnect();
        connect(&checker, SIGNAL(signal1(CustomType)), &checker, SLOT(slot1(CustomType)),
                Qt::QueuedConnection);
        QCOMPARE(instanceCount, 4);
        checker.doEmit(t2);
        QCOMPARE(instanceCount, 5);
        QCOMPARE(checker.received.value(), t0.value());

        QCoreApplication::processEvents();
        QCOMPARE(checker.received.value(), t2.value());
        QCOMPARE(instanceCount, 4);

        QVERIFY(QMetaType::isRegistered(idx));
        QCOMPARE(qRegisterMetaType<CustomType>("CustomType"), idx);
        QCOMPARE(QMetaType::type("CustomType"), idx);
        QVERIFY(QMetaType::isRegistered(idx));
    }
    QCOMPARE(instanceCount, 3);
}

QDataStream &operator<<(QDataStream &stream, const CustomType &ct)
{
    stream << ct.i1 << ct.i2 << ct.i3;
    return stream;
}

QDataStream &operator>>(QDataStream &stream, CustomType &ct)
{
    stream >> ct.i1;
    stream >> ct.i2;
    stream >> ct.i3;
    return stream;
}

void tst_QObject::streamCustomTypes()
{
    QByteArray ba;

    int idx = qRegisterMetaType<CustomType>("CustomType");
    qRegisterMetaTypeStreamOperators<CustomType>("CustomType");

    {
        CustomType t1(1, 2, 3);
        QCOMPARE(instanceCount, 1);
        QDataStream stream(&ba, (QIODevice::OpenMode)QIODevice::WriteOnly);
        QMetaType::save(stream, idx, &t1);
    }

    QCOMPARE(instanceCount, 0);

    {
        CustomType t2;
        QCOMPARE(instanceCount, 1);
        QDataStream stream(&ba, (QIODevice::OpenMode)QIODevice::ReadOnly);
        QMetaType::load(stream, idx, &t2);
        QCOMPARE(instanceCount, 1);
        QCOMPARE(t2.i1, 1);
        QCOMPARE(t2.i2, 2);
        QCOMPARE(t2.i3, 3);
    }
    QCOMPARE(instanceCount, 0);
}

typedef QString CustomString;

class PropertyObject : public QObject
{
    Q_OBJECT
    Q_ENUMS(Alpha Priority)

    Q_PROPERTY(Alpha alpha READ alpha WRITE setAlpha)
    Q_PROPERTY(Priority priority READ priority WRITE setPriority)
    Q_PROPERTY(int number READ number WRITE setNumber)
    Q_PROPERTY(QString string READ string WRITE setString)
    Q_PROPERTY(QVariant variant READ variant WRITE setVariant)
    Q_PROPERTY(CustomType* custom READ custom WRITE setCustom)
    Q_PROPERTY(float myFloat READ myFloat WRITE setMyFloat)
    Q_PROPERTY(qreal myQReal READ myQReal WRITE setMyQReal)
    Q_PROPERTY(CustomString customString READ customString WRITE setCustomString )

public:
    enum Alpha {
        Alpha0,
        Alpha1,
        Alpha2
    };

    enum Priority { High, Low, VeryHigh, VeryLow };

    PropertyObject()
        : m_alpha(Alpha0), m_priority(High), m_number(0), m_custom(0), m_float(42)
    {}

    Alpha alpha() const { return m_alpha; }
    void setAlpha(Alpha alpha) { m_alpha = alpha; }

    Priority priority() const { return m_priority; }
    void setPriority(Priority priority) { m_priority = priority; }

    int number() const { return m_number; }
    void setNumber(int number) { m_number = number; }

    QString string() const { return m_string; }
    void setString(const QString &string) { m_string = string; }

    QVariant variant() const { return m_variant; }
    void setVariant(const QVariant &variant) { m_variant = variant; }

    CustomType *custom() const { return m_custom; }
    void setCustom(CustomType *custom) { m_custom = custom; }

    void setMyFloat(float value) { m_float = value; }
    inline float myFloat() const { return m_float; }

    void setMyQReal(qreal value) { m_qreal = value; }
    qreal myQReal() const { return m_qreal; }

    CustomString customString() const { return m_customString; }
    void setCustomString(const QString &string) { m_customString = string; }

private:
    Alpha m_alpha;
    Priority m_priority;
    int m_number;
    QString m_string;
    QVariant m_variant;
    CustomType *m_custom;
    float m_float;
    qreal m_qreal;
    CustomString m_customString;
};

Q_DECLARE_METATYPE(PropertyObject::Priority)

void tst_QObject::threadSignalEmissionCrash()
{
#if defined(Q_OS_WINCE) || defined(Q_OS_SYMBIAN)
    int loopCount = 100;
#else
    int loopCount = 1000;
#endif
    for (int i = 0; i < loopCount; ++i) {
        QTcpSocket socket;
        socket.connectToHost("localhost", 80);
    }
}

class TestThread : public QThread
{
    Q_OBJECT
public:
    inline void run()
    {
        *object = new QObject;
        *child = new QObject(*object);
        mutex.lock();
        cond.wakeOne();
        cond.wait(&mutex);
        mutex.unlock();
    }

    QObject **object, **child;
    QMutex mutex;
    QWaitCondition cond;
};

void tst_QObject::thread()
{
    QThread *currentThread = QThread::currentThread();
    // the current thread is the same as the QApplication
    // thread... see tst_QApplication::thread()

    {
        QObject object;
        // thread affinity for objects with no parent should be the
        // current thread
        QVERIFY(object.thread() != 0);
        QCOMPARE(object.thread(), currentThread);
        // children inherit their parent's thread
        QObject child(&object);
        QCOMPARE(child.thread(), object.thread());
    }

    QObject *object = 0;
    QObject *child = 0;

    {
        TestThread thr;
        QVERIFY(thr.thread() != 0);
        QCOMPARE(thr.thread(), currentThread);

        thr.object = &object;
        thr.child = &child;

        thr.mutex.lock();
        thr.start();
        thr.cond.wait(&thr.mutex);

        // thread affinity for an object with no parent should be the
        // thread in which the object was created
        QCOMPARE(object->thread(), (QThread *)&thr);
        // children inherit their parent's thread
        QCOMPARE(child->thread(), object->thread());

        thr.cond.wakeOne();
        thr.mutex.unlock();
        thr.wait();

        // even though the thread is no longer running, the affinity
        // should not change
        QCOMPARE(object->thread(), (QThread *)&thr);
        QCOMPARE(child->thread(), object->thread());
    }

    // the thread has been destroyed, thread affinity should
    // automatically reset to no thread
    QCOMPARE(object->thread(), (QThread *)0);
    QCOMPARE(child->thread(), object->thread());

    delete object;
}

class MoveToThreadObject : public QObject
{
    Q_OBJECT
public:
    QThread *timerEventThread;
    QThread *customEventThread;
    QThread *slotThread;

    MoveToThreadObject(QObject *parent = 0)
        : QObject(parent), timerEventThread(0), customEventThread(0), slotThread(0)
    { }

    void customEvent(QEvent *)
    {
        Q_ASSERT(customEventThread == 0);
        customEventThread = QThread::currentThread();
        emit theSignal();
    }

    void timerEvent(QTimerEvent *)
    {
        Q_ASSERT(timerEventThread == 0);
        timerEventThread = QThread::currentThread();
        emit theSignal();
    }

public slots:
    void theSlot()
    {
        Q_ASSERT(slotThread == 0);
        slotThread = QThread::currentThread();
        emit theSignal();
    }

signals:
    void theSignal();
};

class MoveToThreadThread : public QThread
{
public:
    ~MoveToThreadThread()
    {
        if (isRunning()) {
            terminate();
            wait();
        }
    }
    void start()
    {
        QEventLoop eventLoop;
        connect(this, SIGNAL(started()), &eventLoop, SLOT(quit()), Qt::QueuedConnection);
        QThread::start();
        // wait for thread to start
        (void) eventLoop.exec();
    }
    void run()
    { (void) exec(); }
};

void tst_QObject::thread0()
{
    QObject *object = new QObject;
    object->moveToThread(0);
    QObject *child = new QObject(object);
    QCOMPARE(child->parent(), object);
    QCOMPARE(child->thread(), (QThread *)0);

#if 0
    // We don't support moving children into a parent that has no thread
    // affinity (yet?).
    QObject *child2 = new QObject;
    child2->moveToThread(0);
    child2->setParent(object);
    QCOMPARE(child2->parent(), object);
    QCOMPARE(child2->thread(), (QThread *)0);
#endif

    delete object;
}

void tst_QObject::moveToThread()
{
    QThread *currentThread = QThread::currentThread();

    {
        QObject *object = new QObject;
        QObject *child = new QObject(object);
        QCOMPARE(object->thread(), currentThread);
        QCOMPARE(child->thread(), currentThread);
        object->moveToThread(0);
        QCOMPARE(object->thread(), (QThread *)0);
        QCOMPARE(child->thread(), (QThread *)0);
        object->moveToThread(currentThread);
        QCOMPARE(object->thread(), currentThread);
        QCOMPARE(child->thread(), currentThread);
        object->moveToThread(0);
        QCOMPARE(object->thread(), (QThread *)0);
        QCOMPARE(child->thread(), (QThread *)0);
        // can delete an object with no thread anywhere
        delete object;
    }

    {
        MoveToThreadThread thread;
        thread.start();

        QObject *object = new QObject;
        QObject *child = new QObject(object);
        QPointer<QObject> opointer = object;
        QPointer<QObject> cpointer = object;

        QCOMPARE(object->thread(), currentThread);
        QCOMPARE(child->thread(), currentThread);
        object->moveToThread(&thread);
        QCOMPARE(object->thread(), (QThread *)&thread);
        QCOMPARE(child->thread(), (QThread *)&thread);

        connect(object, SIGNAL(destroyed()), &thread, SLOT(quit()), Qt::DirectConnection);
        QMetaObject::invokeMethod(object, "deleteLater", Qt::QueuedConnection);
        thread.wait();

        QVERIFY(opointer == 0);
        QVERIFY(cpointer == 0);
    }

    {
        // make sure posted events are moved with the object
        MoveToThreadThread thread;
        thread.start();

        MoveToThreadObject *object = new MoveToThreadObject;
        MoveToThreadObject *child = new MoveToThreadObject(object);

        connect(object, SIGNAL(theSignal()), &thread, SLOT(quit()), Qt::DirectConnection);
        QCoreApplication::postEvent(child, new QEvent(QEvent::User));
        QCoreApplication::postEvent(object, new QEvent(QEvent::User));

        QCOMPARE(object->thread(), currentThread);
        QCOMPARE(child->thread(), currentThread);
        object->moveToThread(&thread);
        QCOMPARE(object->thread(), (QThread *)&thread);
        QCOMPARE(child->thread(), (QThread *)&thread);

        thread.wait();

        QCOMPARE(object->customEventThread, (QThread *)&thread);
        QCOMPARE(child->customEventThread, (QThread *)&thread);

        thread.start();
        connect(object, SIGNAL(destroyed()), &thread, SLOT(quit()), Qt::DirectConnection);
        QMetaObject::invokeMethod(object, "deleteLater", Qt::QueuedConnection);
        thread.wait();
    }

    {
        // make sure timers are moved with the object
        MoveToThreadThread thread;
        thread.start();

        MoveToThreadObject *object = new MoveToThreadObject;
        MoveToThreadObject *child = new MoveToThreadObject(object);

        connect(object, SIGNAL(theSignal()), &thread, SLOT(quit()), Qt::DirectConnection);

#if defined(Q_OS_SYMBIAN)
        // Child timer will be registered after parent timer in the new
        // thread, and 10ms is less than symbian timer resolution, so
        // child->timerEventThread compare after thread.wait() will
        // usually fail unless timers are farther apart.
        child->startTimer(100);
        object->startTimer(150);
#else
        child->startTimer(90);
        object->startTimer(100);
#endif

        QCOMPARE(object->thread(), currentThread);
        QCOMPARE(child->thread(), currentThread);
        object->moveToThread(&thread);
        QCOMPARE(object->thread(), (QThread *)&thread);
        QCOMPARE(child->thread(), (QThread *)&thread);

        thread.wait();

        QCOMPARE(object->timerEventThread, (QThread *)&thread);
        QCOMPARE(child->timerEventThread, (QThread *)&thread);

        thread.start();
        connect(object, SIGNAL(destroyed()), &thread, SLOT(quit()), Qt::DirectConnection);
        QMetaObject::invokeMethod(object, "deleteLater", Qt::QueuedConnection);
        thread.wait();
    }

    {
        // make sure socket notifiers are moved with the object
        MoveToThreadThread thread;
        thread.start();

        QTcpServer server;
        QVERIFY(server.listen(QHostAddress::LocalHost, 0));
        QTcpSocket *socket = new QTcpSocket;
        MoveToThreadObject *child = new MoveToThreadObject(socket);
        connect(socket, SIGNAL(disconnected()), child, SLOT(theSlot()), Qt::DirectConnection);
        connect(child, SIGNAL(theSignal()), &thread, SLOT(quit()), Qt::DirectConnection);

        socket->connectToHost(server.serverAddress(), server.serverPort());

        QVERIFY(server.waitForNewConnection(1000));
        QTcpSocket *serverSocket = server.nextPendingConnection();
        QVERIFY(serverSocket);

        socket->waitForConnected();

        QCOMPARE(socket->thread(), currentThread);
        socket->moveToThread(&thread);
        QCOMPARE(socket->thread(), (QThread *)&thread);

        serverSocket->close();

        QVERIFY(thread.wait(10000));

        QCOMPARE(child->slotThread, (QThread *)&thread);

        thread.start();
        connect(socket, SIGNAL(destroyed()), &thread, SLOT(quit()), Qt::DirectConnection);
        QMetaObject::invokeMethod(socket, "deleteLater", Qt::QueuedConnection);
        thread.wait();
    }
}


void tst_QObject::property()
{
    PropertyObject object;
    const QMetaObject *mo = object.metaObject();
    QMetaProperty property;
    QVERIFY(mo);

    QVERIFY(mo->indexOfProperty("alpha") != -1);
    property = mo->property(mo->indexOfProperty("alpha"));
    QVERIFY(property.isEnumType());
    QCOMPARE(property.typeName(), "Alpha");
    QCOMPARE(property.type(), QVariant::Int);

    QVariant var = object.property("alpha");
    QVERIFY(!var.isNull());
    QCOMPARE(var.toInt(), int(PropertyObject::Alpha0));
    object.setAlpha(PropertyObject::Alpha1);
    QCOMPARE(object.property("alpha").toInt(), int(PropertyObject::Alpha1));
    QVERIFY(object.setProperty("alpha", PropertyObject::Alpha2));
    QCOMPARE(object.property("alpha").toInt(), int(PropertyObject::Alpha2));
    QVERIFY(object.setProperty("alpha", "Alpha1"));
    QCOMPARE(object.property("alpha").toInt(), int(PropertyObject::Alpha1));
    QVERIFY(!object.setProperty("alpha", QVariant()));

    QVERIFY(mo->indexOfProperty("number") != -1);
    QCOMPARE(object.property("number").toInt(), 0);
    object.setNumber(24);
    QCOMPARE(object.property("number"), QVariant(24));
    QVERIFY(object.setProperty("number", 12));
    QCOMPARE(object.property("number"), QVariant(12));
    QVERIFY(object.setProperty("number", "42"));
    QCOMPARE(object.property("number"), QVariant(42));

    QVERIFY(mo->indexOfProperty("string") != -1);
    QCOMPARE(object.property("string").toString(), QString());
    object.setString("String1");
    QCOMPARE(object.property("string"), QVariant("String1"));
    QVERIFY(object.setProperty("string", "String2"));
    QCOMPARE(object.property("string"), QVariant("String2"));
    QVERIFY(!object.setProperty("string", QVariant()));

    const int idx = mo->indexOfProperty("variant");
    QVERIFY(idx != -1);
    QVERIFY(mo->property(idx).type() == QVariant::LastType);
    QCOMPARE(object.property("variant"), QVariant());
    QVariant variant1(42);
    QVariant variant2("string");
    object.setVariant(variant1);
    QCOMPARE(object.property("variant"), variant1);
    QVERIFY(object.setProperty("variant", variant2));
    QCOMPARE(object.variant(), QVariant(variant2));
    QCOMPARE(object.property("variant"), variant2);
    QVERIFY(object.setProperty("variant", QVariant()));
    QCOMPARE(object.property("variant"), QVariant());

    QVERIFY(mo->indexOfProperty("custom") != -1);
    property = mo->property(mo->indexOfProperty("custom"));
    QVERIFY(property.isValid());
    QVERIFY(property.isWritable());
    QVERIFY(!property.isEnumType());
    QCOMPARE(property.typeName(), "CustomType*");
    QCOMPARE(property.type(), QVariant::UserType);

    CustomType *customPointer = 0;
    QVariant customVariant = object.property("custom");
    customPointer = qVariantValue<CustomType *>(customVariant);
    QCOMPARE(customPointer, object.custom());

    CustomType custom;
    customPointer = &custom;
    qVariantSetValue(customVariant, customPointer);

    property = mo->property(mo->indexOfProperty("custom"));
    QVERIFY(property.isWritable());
    QCOMPARE(property.typeName(), "CustomType*");
    QCOMPARE(property.type(), QVariant::UserType);

    QVERIFY(object.setProperty("custom", customVariant));
    QCOMPARE(object.custom(), customPointer);

    customVariant = object.property("custom");
    customPointer = qVariantValue<CustomType *>(customVariant);
    QCOMPARE(object.custom(), customPointer);

    // this enum property has a meta type, but it's not yet registered, so we know this fails
    QVERIFY(mo->indexOfProperty("priority") != -1);
    property = mo->property(mo->indexOfProperty("priority"));
    QVERIFY(property.isEnumType());
    QCOMPARE(property.typeName(), "Priority");
    QCOMPARE(property.type(), QVariant::Int);

    var = object.property("priority");
    QVERIFY(!var.isNull());
    QCOMPARE(var.toInt(), int(PropertyObject::High));
    object.setPriority(PropertyObject::Low);
    QCOMPARE(object.property("priority").toInt(), int(PropertyObject::Low));
    QVERIFY(object.setProperty("priority", PropertyObject::VeryHigh));
    QCOMPARE(object.property("priority").toInt(), int(PropertyObject::VeryHigh));
    QVERIFY(object.setProperty("priority", "High"));
    QCOMPARE(object.property("priority").toInt(), int(PropertyObject::High));
    QVERIFY(!object.setProperty("priority", QVariant()));

    // now it's registered, so it works as expected
    int priorityMetaTypeId = qRegisterMetaType<PropertyObject::Priority>("PropertyObject::Priority");

    QVERIFY(mo->indexOfProperty("priority") != -1);
    property = mo->property(mo->indexOfProperty("priority"));
    QVERIFY(property.isEnumType());
    QCOMPARE(property.typeName(), "Priority");
    QCOMPARE(property.type(), QVariant::UserType);
    QCOMPARE(property.userType(), priorityMetaTypeId);

    var = object.property("priority");
    QVERIFY(!var.isNull());
    QVERIFY(qVariantCanConvert<PropertyObject::Priority>(var));
    QCOMPARE(qVariantValue<PropertyObject::Priority>(var), PropertyObject::High);
    object.setPriority(PropertyObject::Low);
    QCOMPARE(qVariantValue<PropertyObject::Priority>(object.property("priority")), PropertyObject::Low);
    QVERIFY(object.setProperty("priority", PropertyObject::VeryHigh));
    QCOMPARE(qVariantValue<PropertyObject::Priority>(object.property("priority")), PropertyObject::VeryHigh);
    QVERIFY(object.setProperty("priority", "High"));
    QCOMPARE(qVariantValue<PropertyObject::Priority>(object.property("priority")), PropertyObject::High);
    QVERIFY(!object.setProperty("priority", QVariant()));

    var = object.property("priority");
    QCOMPARE(qVariantValue<PropertyObject::Priority>(var), PropertyObject::High);
    object.setPriority(PropertyObject::Low);
    QCOMPARE(qVariantValue<PropertyObject::Priority>(object.property("priority")), PropertyObject::Low);
    object.setProperty("priority", var);
    QCOMPARE(qVariantValue<PropertyObject::Priority>(object.property("priority")), PropertyObject::High);

    qRegisterMetaType<CustomString>("CustomString");
    QVERIFY(mo->indexOfProperty("customString") != -1);
    QCOMPARE(object.property("customString").toString(), QString());
    object.setCustomString("String1");
    QCOMPARE(object.property("customString"), QVariant("String1"));
    QVERIFY(object.setProperty("customString", "String2"));
    QCOMPARE(object.property("customString"), QVariant("String2"));
    QVERIFY(!object.setProperty("customString", QVariant()));
}

void tst_QObject::metamethod()
{
    SenderObject obj;
    const QMetaObject *mobj = obj.metaObject();
    QMetaMethod m;

    m = mobj->method(mobj->indexOfMethod("invoke1()"));
    QVERIFY(QByteArray(m.signature()) == "invoke1()");
    QVERIFY(m.methodType() == QMetaMethod::Method);
    QVERIFY(m.access() == QMetaMethod::Public);
    QVERIFY(!(m.attributes() & QMetaMethod::Scriptable));
    QVERIFY(!(m.attributes() & QMetaMethod::Compatibility));

    m = mobj->method(mobj->indexOfMethod("sinvoke1()"));
    QVERIFY(QByteArray(m.signature()) == "sinvoke1()");
    QVERIFY(m.methodType() == QMetaMethod::Method);
    QVERIFY(m.access() == QMetaMethod::Public);
    QVERIFY((m.attributes() & QMetaMethod::Scriptable));
    QVERIFY(!(m.attributes() & QMetaMethod::Compatibility));

    m = mobj->method(mobj->indexOfMethod("invoke2()"));
    QVERIFY(QByteArray(m.signature()) == "invoke2()");
    QVERIFY(m.methodType() == QMetaMethod::Method);
    QVERIFY(m.access() == QMetaMethod::Protected);
    QVERIFY(!(m.attributes() & QMetaMethod::Scriptable));
    QVERIFY((m.attributes() & QMetaMethod::Compatibility));

    m = mobj->method(mobj->indexOfMethod("sinvoke2()"));
    QVERIFY(QByteArray(m.signature()) == "sinvoke2()");
    QVERIFY(m.methodType() == QMetaMethod::Method);
    QVERIFY(m.access() == QMetaMethod::Protected);
    QVERIFY((m.attributes() & QMetaMethod::Scriptable));
    QVERIFY((m.attributes() & QMetaMethod::Compatibility));

    m = mobj->method(mobj->indexOfMethod("invoke3()"));
    QVERIFY(QByteArray(m.signature()) == "invoke3()");
    QVERIFY(m.methodType() == QMetaMethod::Method);
    QVERIFY(m.access() == QMetaMethod::Private);
    QVERIFY(!(m.attributes() & QMetaMethod::Scriptable));
    QVERIFY(!(m.attributes() & QMetaMethod::Compatibility));

    m = mobj->method(mobj->indexOfMethod("sinvoke3()"));
    QVERIFY(QByteArray(m.signature()) == "sinvoke3()");
    QVERIFY(m.methodType() == QMetaMethod::Method);
    QVERIFY(m.access() == QMetaMethod::Private);
    QVERIFY((m.attributes() & QMetaMethod::Scriptable));
    QVERIFY(!(m.attributes() & QMetaMethod::Compatibility));

    m = mobj->method(mobj->indexOfMethod("signal5()"));
    QVERIFY(QByteArray(m.signature()) == "signal5()");
    QVERIFY(m.methodType() == QMetaMethod::Signal);
    QVERIFY(m.access() == QMetaMethod::Protected);
    QVERIFY(!(m.attributes() & QMetaMethod::Scriptable));
    QVERIFY((m.attributes() & QMetaMethod::Compatibility));

    m = mobj->method(mobj->indexOfMethod("aPublicSlot()"));
    QVERIFY(QByteArray(m.signature()) == "aPublicSlot()");
    QVERIFY(m.methodType() == QMetaMethod::Slot);
    QVERIFY(m.access() == QMetaMethod::Public);
    QVERIFY(!(m.attributes() & QMetaMethod::Scriptable));
    QVERIFY(!(m.attributes() & QMetaMethod::Compatibility));

    m = mobj->method(mobj->indexOfMethod("invoke1()"));
    QCOMPARE(m.parameterNames().count(), 0);
    QCOMPARE(m.parameterTypes().count(), 0);

    m = mobj->method(mobj->indexOfMethod("invoke2(int)"));
    QCOMPARE(m.parameterNames().count(), 1);
    QCOMPARE(m.parameterTypes().count(), 1);
    QCOMPARE(m.parameterTypes().at(0), QByteArray("int"));
    QVERIFY(m.parameterNames().at(0).isEmpty());

    m = mobj->method(mobj->indexOfMethod("invoke3(int,int)"));
    QCOMPARE(m.parameterNames().count(), 2);
    QCOMPARE(m.parameterTypes().count(), 2);
    QCOMPARE(m.parameterTypes().at(0), QByteArray("int"));
    QCOMPARE(m.parameterNames().at(0), QByteArray("hinz"));
    QCOMPARE(m.parameterTypes().at(1), QByteArray("int"));
    QCOMPARE(m.parameterNames().at(1), QByteArray("kunz"));

}

namespace QObjectTest
{
    class TestObject: public QObject
    {
    Q_OBJECT
    public:
        TestObject(): QObject(), i(0) {}
        void doEmit() { emit aSignal(); }
        int i;
    public slots:
        void aSlot() { ++i; }
    signals:
        void aSignal();
    };
}

void tst_QObject::namespaces()
{
    QObjectTest::TestObject obj;

    QVERIFY(connect(&obj, SIGNAL(aSignal()), &obj, SLOT(aSlot())));
    obj.doEmit();
    QCOMPARE(obj.i, 1);
}

class SuperObject : public QObject
{
    Q_OBJECT
public:
    QObject *theSender;
    SuperObject()
    {
        theSender = 0;
    }

    friend class tst_QObject;

    using QObject::sender;

public slots:
    void rememberSender()
    {
        theSender = sender();
    }

    void deleteAndRememberSender()
    {
        delete theSender;
        theSender = sender();
    }
signals:
    void theSignal();

};

void tst_QObject::sender()
{
    {
        SuperObject sender;
        SuperObject receiver;
        connect(&sender, SIGNAL(theSignal()),
                &receiver, SLOT(rememberSender()));
        QCOMPARE(receiver.sender(), (QObject *)0);
        emit sender.theSignal();
        QCOMPARE(receiver.theSender, (QObject *)&sender);
        QCOMPARE(receiver.sender(), (QObject *)0);
    }

    {
        SuperObject *sender = new SuperObject;
        SuperObject *receiver = new SuperObject;
        connect(sender, SIGNAL(theSignal()),
                receiver, SLOT(rememberSender()),
                Qt::BlockingQueuedConnection);

        QThread thread;
        receiver->moveToThread(&thread);
        connect(sender, SIGNAL(theSignal()),
                &thread, SLOT(quit()),
                Qt::DirectConnection);

        QCOMPARE(receiver->sender(), (QObject *)0);
        receiver->theSender = 0;
        thread.start();
        emit sender->theSignal();
        QCOMPARE(receiver->theSender, (QObject *) sender);
        QCOMPARE(receiver->sender(), (QObject *)0);

        QVERIFY(thread.wait(10000));
        delete receiver;
        delete sender;
    }

    {
        SuperObject *sender = new SuperObject;
        SuperObject receiver;
        connect(sender, SIGNAL(theSignal()),
                &receiver, SLOT(deleteAndRememberSender()));
        QCOMPARE(receiver.sender(), (QObject *)0);
        receiver.theSender = sender;
        emit sender->theSignal();
        QCOMPARE(receiver.theSender, (QObject *)0);
        QCOMPARE(receiver.sender(), (QObject *)0);
    }

    {
        SuperObject *sender = new SuperObject;
        SuperObject *receiver = new SuperObject;
        connect(sender, SIGNAL(theSignal()),
                receiver, SLOT(deleteAndRememberSender()),
                Qt::BlockingQueuedConnection);

        QThread thread;
        receiver->moveToThread(&thread);
        connect(sender, SIGNAL(destroyed()),
                &thread, SLOT(quit()),
                Qt::DirectConnection);

        QCOMPARE(receiver->sender(), (QObject *)0);
        receiver->theSender = sender;
        thread.start();
        emit sender->theSignal();
        QCOMPARE(receiver->theSender, (QObject *)0);
        QCOMPARE(receiver->sender(), (QObject *)0);

        QVERIFY(thread.wait(10000));
        delete receiver;
    }
}

namespace Foo
{
    struct Bar
    {
        virtual ~Bar() {}
        virtual int rtti() const = 0;
    };

    struct Bleh
    {
        virtual ~Bleh() {}
        virtual int rtti() const = 0;
    };
}

QT_BEGIN_NAMESPACE
Q_DECLARE_INTERFACE(Foo::Bar, "com.qtest.foobar")
QT_END_NAMESPACE

#define Bleh_iid "com.qtest.bleh"
QT_BEGIN_NAMESPACE
Q_DECLARE_INTERFACE(Foo::Bleh, Bleh_iid)
QT_END_NAMESPACE

class FooObject: public QObject, public Foo::Bar
{
    Q_OBJECT
    Q_INTERFACES(Foo::Bar)
public:
    int rtti() const { return 42; }
};

class BlehObject : public QObject, public Foo::Bleh
{
    Q_OBJECT
    Q_INTERFACES(Foo::Bleh)
public:
    int rtti() const { return 43; }
};

void tst_QObject::declareInterface()
{
    FooObject obj;

    Foo::Bar *bar = qobject_cast<Foo::Bar *>(&obj);
    QVERIFY(bar);
    QCOMPARE(bar->rtti(), 42);
    QCOMPARE(static_cast<Foo::Bar *>(&obj), bar);

    BlehObject bleh;

    bar = qobject_cast<Foo::Bar *>(&bleh);
    QVERIFY(!bar);
    Foo::Bleh *b = qobject_cast<Foo::Bleh *>(&bleh);
    QCOMPARE(b->rtti(), 43);
    QCOMPARE(static_cast<Foo::Bleh *>(&bleh), b);

}

class CustomData : public QObjectUserData
{
public:
    int id;
};

void tst_QObject::testUserData()
{
    const int USER_DATA_COUNT = 100;
    int user_data_ids[USER_DATA_COUNT];

    // Register a few
    for (int i=0; i<USER_DATA_COUNT; ++i) {
        user_data_ids[i] = QObject::registerUserData();
    }

    // Randomize the table a bit
    for (int i=0; i<100; ++i) {
        int p1 = rand() % USER_DATA_COUNT;
        int p2 = rand() % USER_DATA_COUNT;

        int tmp = user_data_ids[p1];
        user_data_ids[p1] = user_data_ids[p2];
        user_data_ids[p2] = tmp;
    }

    // insert the user data into an object
    QObject my_test_object;
    for (int i=0; i<USER_DATA_COUNT; ++i) {
        CustomData *data = new CustomData;
        data->id = user_data_ids[i];
        my_test_object.setUserData(data->id, data);
    }

    // verify that all ids and positions are matching
    for (int i=0; i<USER_DATA_COUNT; ++i) {
        int id = user_data_ids[i];
        CustomData *data = static_cast<CustomData *>(my_test_object.userData(id));
        QVERIFY(data != 0);
        QVERIFY(data->id == id);
    }
}

class DestroyedListener : public QObject
{
    Q_OBJECT
public:
    inline DestroyedListener() : pointerWasZero(false) {}

    QPointer<QObject> pointer;
    bool pointerWasZero;

private slots:
    inline void otherObjectDestroyed()
    { pointerWasZero = pointer.isNull(); }
};

void tst_QObject::qpointerResetBeforeDestroyedSignal()
{
    QObject *obj = new QObject;
    DestroyedListener listener;
    listener.pointer = obj;
    listener.pointerWasZero = false;
    connect(obj, SIGNAL(destroyed()), &listener, SLOT(otherObjectDestroyed()));
    delete obj;
    QVERIFY(listener.pointerWasZero);
    QVERIFY(listener.pointer.isNull());
}

class DefaultArguments : public QObject
{
    Q_OBJECT

public slots:

    void theSlot(const QString &s) { result = s; }

signals:
    void theOriginalSignal();
    void theSecondSignal(const QString &s = QString("secondDefault"));

public:

    void emitTheOriginalSignal() { emit theOriginalSignal(); }
    void emitTheSecondSignal() { emit theSecondSignal(); }
    QString result;
};

void tst_QObject::connectSignalsToSignalsWithDefaultArguments()
{
    DefaultArguments o;
    connect(&o, SIGNAL(theOriginalSignal()), &o, SIGNAL(theSecondSignal()));
    connect(&o, SIGNAL(theSecondSignal(QString)), &o, SLOT(theSlot(QString)));
    QVERIFY( o.result.isEmpty() );
    o.emitTheSecondSignal();
    QCOMPARE(o.result, QString("secondDefault"));
    o.result = "Not called";
    o.emitTheOriginalSignal();
    QCOMPARE(o.result, QString("secondDefault"));

}

void tst_QObject::receivers()
{
    class Object : public QObject
    {
    public:
        int receivers(const char* signal) const
        { return QObject::receivers(signal); }
    };

    Object object;
    QCOMPARE(object.receivers(SIGNAL(destroyed())), 0);
    object.connect(&object, SIGNAL(destroyed()), SLOT(deleteLater()));
    QCOMPARE(object.receivers(SIGNAL(destroyed())), 1);
    object.connect(&object, SIGNAL(destroyed()), SLOT(deleteLater()));
    QCOMPARE(object.receivers(SIGNAL(destroyed())), 2);
    object.disconnect(SIGNAL(destroyed()), &object, SLOT(deleteLater()));
    QCOMPARE(object.receivers(SIGNAL(destroyed())), 0);
}

enum Enum { };

struct Struct { };
class Class { };
template <typename T> class Template { };

class NormalizeObject : public QObject
{
    Q_OBJECT

public:

signals:
    void uintPointerSignal(uint *);
    void ulongPointerSignal(ulong *);
    void constUintPointerSignal(const uint *);
    void constUlongPointerSignal(const ulong *);

    void structSignal(Struct s);
    void classSignal(Class c);
    void enumSignal(Enum e);

    void structPointerSignal(Struct *s);
    void classPointerSignal(Class *c);
    void enumPointerSignal(Enum *e);

    void constStructPointerSignal(const Struct *s);
    void constClassPointerSignal(const Class *c);
    void constEnumPointerSignal(const Enum *e);

    void constStructPointerConstPointerSignal(const Struct * const *s);
    void constClassPointerConstPointerSignal(const Class * const *c);
    void constEnumPointerConstPointerSignal(const Enum * const *e);

    void unsignedintSignal(unsigned int);
    void unsignedSignal(unsigned);
    void unsignedlongSignal(unsigned long);
    void unsignedlonglongSignal(quint64);
    void unsignedlongintSignal(unsigned long int);
    void unsignedshortSignal(unsigned short);
    void unsignedcharSignal(unsigned char);

    void typeRefSignal(Template<Class &> &ref);
    void constTypeRefSignal(const Template<Class const &> &ref);
    void typeConstRefSignal(Template<Class const &> const &ref);

    void typePointerConstRefSignal(Class * const &);

    void constTemplateSignal1( Template<int > );
    void constTemplateSignal2( Template< const int >);

public slots:
    void uintPointerSlot(uint *) { }
    void ulongPointerSlot(ulong *) { }
    void constUintPointerSlot(const uint *) { }
    void constUlongPointerSlot(const ulong *) { }

    void structSlot(Struct s) { Q_UNUSED(s); }
    void classSlot(Class c) { Q_UNUSED(c); }
    void enumSlot(Enum e) { Q_UNUSED(e); }

    void structPointerSlot(Struct *s) { Q_UNUSED(s); }
    void classPointerSlot(Class *c) { Q_UNUSED(c); }
    void enumPointerSlot(Enum *e) { Q_UNUSED(e); }

    void constStructPointerSlot(const Struct *s) { Q_UNUSED(s); }
    void constClassPointerSlot(const Class *c) { Q_UNUSED(c); }
    void constEnumPointerSlot(const Enum *e) { Q_UNUSED(e); }

    void constStructPointerConstPointerSlot(const Struct * const *s) { Q_UNUSED(s); }
    void constClassPointerConstPointerSlot(const Class * const *c) { Q_UNUSED(c); }
    void constEnumPointerConstPointerSlot(const Enum * const *e) { Q_UNUSED(e); }

    void uintSlot(uint) {};
    void unsignedintSlot(unsigned int) {};
    void unsignedSlot(unsigned) {};
    void unsignedlongSlot(unsigned long) {};
    void unsignedlonglongSlot(quint64) {};
    void unsignedlongintSlot(unsigned long int) {};
    void unsignedshortSlot(unsigned short) {};
    void unsignedcharSlot(unsigned char) {};

    void typeRefSlot(Template<Class &> &) {}
    void constTypeRefSlot(const Template<const Class &> &) {}
    void typeConstRefSlot(Template<Class const &> const &) {}

    void typePointerConstRefSlot(Class * const &) {}

    void constTemplateSlot1(Template<int > const) {}
    void constTemplateSlot2(const Template<int > ) {}
    void constTemplateSlot3(const Template< const int >) {}
};

#include "oldnormalizeobject.h"

void tst_QObject::normalize()
{
    NormalizeObject object;

    // unsigned int -> uint, unsigned long -> ulong
    QVERIFY(object.connect(&object,
                           SIGNAL(uintPointerSignal(uint *)),
                           SLOT(uintPointerSlot(uint *))));
    QVERIFY(object.connect(&object,
                           SIGNAL(uintPointerSignal(unsigned int *)),
                           SLOT(uintPointerSlot(uint *))));
    QVERIFY(object.connect(&object,
                           SIGNAL(uintPointerSignal(uint *)),
                           SLOT(uintPointerSlot(unsigned int *))));

    QVERIFY(object.connect(&object,
                           SIGNAL(constUintPointerSignal(const uint *)),
                           SLOT(constUintPointerSlot(const uint *))));
    QVERIFY(object.connect(&object,
                           SIGNAL(constUintPointerSignal(const unsigned int *)),
                           SLOT(constUintPointerSlot(const uint *))));
    QVERIFY(object.connect(&object,
                           SIGNAL(constUintPointerSignal(const uint *)),
                           SLOT(constUintPointerSlot(const unsigned int *))));

    QVERIFY(object.connect(&object,
                           SIGNAL(ulongPointerSignal(ulong *)),
                           SLOT(ulongPointerSlot(ulong *))));
    QVERIFY(object.connect(&object,
                           SIGNAL(ulongPointerSignal(unsigned long *)),
                           SLOT(ulongPointerSlot(ulong *))));
    QVERIFY(object.connect(&object,
                           SIGNAL(ulongPointerSignal(ulong *)),
                           SLOT(ulongPointerSlot(unsigned long *))));

    QVERIFY(object.connect(&object,
                           SIGNAL(constUlongPointerSignal(const ulong *)),
                           SLOT(constUlongPointerSlot(const ulong *))));
    QVERIFY(object.connect(&object,
                           SIGNAL(constUlongPointerSignal(const unsigned long *)),
                           SLOT(constUlongPointerSlot(const ulong *))));
    QVERIFY(object.connect(&object,
                           SIGNAL(constUlongPointerSignal(const ulong *)),
                           SLOT(constUlongPointerSlot(const unsigned long *))));

    // struct, class, and enum are optional
    QVERIFY(object.connect(&object,
                           SIGNAL(structSignal(struct Struct)),
                           SLOT(structSlot(struct Struct))));
    QVERIFY(object.connect(&object,
                           SIGNAL(structSignal(Struct)),
                           SLOT(structSlot(struct Struct))));
    QVERIFY(object.connect(&object,
                           SIGNAL(structSignal(struct Struct)),
                           SLOT(structSlot(Struct))));
    QVERIFY(object.connect(&object,
                           SIGNAL(classSignal(class Class)),
                           SLOT(classSlot(class Class))));
    QVERIFY(object.connect(&object,
                           SIGNAL(classSignal(Class)),
                           SLOT(classSlot(class Class))));
    QVERIFY(object.connect(&object,
                           SIGNAL(classSignal(class Class)),
                           SLOT(classSlot(Class))));
    QVERIFY(object.connect(&object,
                           SIGNAL(enumSignal(enum Enum)),
                           SLOT(enumSlot(enum Enum))));
    QVERIFY(object.connect(&object,
                           SIGNAL(enumSignal(Enum)),
                           SLOT(enumSlot(enum Enum))));
    QVERIFY(object.connect(&object,
                           SIGNAL(enumSignal(enum Enum)),
                           SLOT(enumSlot(Enum))));

    QVERIFY(object.connect(&object,
                           SIGNAL(structPointerSignal(struct Struct *)),
                           SLOT(structPointerSlot(struct Struct *))));
    QVERIFY(object.connect(&object,
                           SIGNAL(structPointerSignal(Struct *)),
                           SLOT(structPointerSlot(struct Struct *))));
    QVERIFY(object.connect(&object,
                           SIGNAL(structPointerSignal(struct Struct *)),
                           SLOT(structPointerSlot(Struct *))));
    QVERIFY(object.connect(&object,
                           SIGNAL(classPointerSignal(class Class *)),
                           SLOT(classPointerSlot(class Class *))));
    QVERIFY(object.connect(&object,
                           SIGNAL(classPointerSignal(Class *)),
                           SLOT(classPointerSlot(class Class *))));
    QVERIFY(object.connect(&object,
                           SIGNAL(classPointerSignal(class Class *)),
                           SLOT(classPointerSlot(Class *))));
    QVERIFY(object.connect(&object,
                           SIGNAL(enumPointerSignal(enum Enum *)),
                           SLOT(enumPointerSlot(enum Enum *))));
    QVERIFY(object.connect(&object,
                           SIGNAL(enumPointerSignal(Enum *)),
                           SLOT(enumPointerSlot(enum Enum *))));
    QVERIFY(object.connect(&object,
                           SIGNAL(enumPointerSignal(enum Enum *)),
                           SLOT(enumPointerSlot(Enum *))));

    QVERIFY(object.connect(&object,
                           SIGNAL(constStructPointerSignal(const struct Struct *)),
                           SLOT(constStructPointerSlot(const struct Struct *))));
    QVERIFY(object.connect(&object,
                           SIGNAL(constStructPointerSignal(const Struct *)),
                           SLOT(constStructPointerSlot(const struct Struct *))));
    QVERIFY(object.connect(&object,
                           SIGNAL(constStructPointerSignal(const struct Struct *)),
                           SLOT(constStructPointerSlot(const Struct *))));
    QVERIFY(object.connect(&object,
                           SIGNAL(constClassPointerSignal(const class Class *)),
                           SLOT(constClassPointerSlot(const class Class *))));
    QVERIFY(object.connect(&object,
                           SIGNAL(constClassPointerSignal(const Class *)),
                           SLOT(constClassPointerSlot(const class Class *))));
    QVERIFY(object.connect(&object,
                           SIGNAL(constClassPointerSignal(const class Class *)),
                           SLOT(constClassPointerSlot(const Class *))));
    QVERIFY(object.connect(&object,
                           SIGNAL(constEnumPointerSignal(const enum Enum *)),
                           SLOT(constEnumPointerSlot(const enum Enum *))));
    QVERIFY(object.connect(&object,
                           SIGNAL(constEnumPointerSignal(const Enum *)),
                           SLOT(constEnumPointerSlot(const enum Enum *))));
    QVERIFY(object.connect(&object,
                           SIGNAL(constEnumPointerSignal(const enum Enum *)),
                           SLOT(constEnumPointerSlot(const Enum *))));

    QVERIFY(object.connect(&object,
                           SIGNAL(constStructPointerSignal(struct Struct const *)),
                           SLOT(constStructPointerSlot(struct Struct const *))));
    QVERIFY(object.connect(&object,
                           SIGNAL(constStructPointerSignal(Struct const *)),
                           SLOT(constStructPointerSlot(struct Struct const *))));
    QVERIFY(object.connect(&object,
                           SIGNAL(constStructPointerSignal(struct Struct const *)),
                           SLOT(constStructPointerSlot(Struct const *))));
    QVERIFY(object.connect(&object,
                           SIGNAL(constClassPointerSignal(class Class const *)),
                           SLOT(constClassPointerSlot(class Class const *))));
    QVERIFY(object.connect(&object,
                           SIGNAL(constClassPointerSignal(Class const *)),
                           SLOT(constClassPointerSlot(class Class const *))));
    QVERIFY(object.connect(&object,
                           SIGNAL(constClassPointerSignal(class Class const *)),
                           SLOT(constClassPointerSlot(Class const *))));
    QVERIFY(object.connect(&object,
                           SIGNAL(constEnumPointerSignal(enum Enum const *)),
                           SLOT(constEnumPointerSlot(enum Enum const *))));
    QVERIFY(object.connect(&object,
                           SIGNAL(constEnumPointerSignal(Enum const *)),
                           SLOT(constEnumPointerSlot(enum Enum const *))));
    QVERIFY(object.connect(&object,
                           SIGNAL(constEnumPointerSignal(enum Enum const *)),
                           SLOT(constEnumPointerSlot(Enum const *))));

    QVERIFY(object.connect(&object,
                           SIGNAL(constStructPointerConstPointerSignal(const struct Struct * const *)),
                           SLOT(constStructPointerConstPointerSlot(const struct Struct * const *))));
    QVERIFY(object.connect(&object,
                           SIGNAL(constStructPointerConstPointerSignal(const Struct * const *)),
                           SLOT(constStructPointerConstPointerSlot(const struct Struct * const *))));
    QVERIFY(object.connect(&object,
                           SIGNAL(constStructPointerConstPointerSignal(const struct Struct * const *)),
                           SLOT(constStructPointerConstPointerSlot(const Struct * const *))));
    QVERIFY(object.connect(&object,
                           SIGNAL(constClassPointerConstPointerSignal(const class Class * const *)),
                           SLOT(constClassPointerConstPointerSlot(const class Class * const *))));
    QVERIFY(object.connect(&object,
                           SIGNAL(constClassPointerConstPointerSignal(const Class * const *)),
                           SLOT(constClassPointerConstPointerSlot(const class Class * const *))));
    QVERIFY(object.connect(&object,
                           SIGNAL(constClassPointerConstPointerSignal(const class Class * const *)),
                           SLOT(constClassPointerConstPointerSlot(const Class * const *))));
    QVERIFY(object.connect(&object,
                           SIGNAL(constEnumPointerConstPointerSignal(const enum Enum * const *)),
                           SLOT(constEnumPointerConstPointerSlot(const enum Enum * const *))));
    QVERIFY(object.connect(&object,
                           SIGNAL(constEnumPointerConstPointerSignal(const Enum * const *)),
                           SLOT(constEnumPointerConstPointerSlot(const enum Enum * const *))));
    QVERIFY(object.connect(&object,
                           SIGNAL(constEnumPointerConstPointerSignal(const enum Enum * const *)),
                           SLOT(constEnumPointerConstPointerSlot(const Enum * const *))));

    QVERIFY(object.connect(&object,
                           SIGNAL(constStructPointerConstPointerSignal(struct Struct const * const *)),
                           SLOT(constStructPointerConstPointerSlot(struct Struct const * const *))));
    QVERIFY(object.connect(&object,
                           SIGNAL(constStructPointerConstPointerSignal(Struct const * const *)),
                           SLOT(constStructPointerConstPointerSlot(struct Struct const * const *))));
    QVERIFY(object.connect(&object,
                           SIGNAL(constStructPointerConstPointerSignal(struct Struct const * const *)),
                           SLOT(constStructPointerConstPointerSlot(Struct const * const *))));
    QVERIFY(object.connect(&object,
                           SIGNAL(constClassPointerConstPointerSignal(class Class const * const *)),
                           SLOT(constClassPointerConstPointerSlot(class Class const * const *))));
    QVERIFY(object.connect(&object,
                           SIGNAL(constClassPointerConstPointerSignal(Class const * const *)),
                           SLOT(constClassPointerConstPointerSlot(class Class const * const *))));
    QVERIFY(object.connect(&object,
                           SIGNAL(constClassPointerConstPointerSignal(class Class const * const *)),
                           SLOT(constClassPointerConstPointerSlot(Class const * const *))));
    QVERIFY(object.connect(&object,
                           SIGNAL(constEnumPointerConstPointerSignal(enum Enum const * const *)),
                           SLOT(constEnumPointerConstPointerSlot(enum Enum const * const *))));
    QVERIFY(object.connect(&object,
                           SIGNAL(constEnumPointerConstPointerSignal(Enum const * const *)),
                           SLOT(constEnumPointerConstPointerSlot(enum Enum const * const *))));
    QVERIFY(object.connect(&object,
                           SIGNAL(constEnumPointerConstPointerSignal(enum Enum const * const *)),
                           SLOT(constEnumPointerConstPointerSlot(Enum const * const *))));

    QVERIFY(object.connect(&object,
                           SIGNAL(unsignedintSignal(unsigned int)),
                           SLOT(unsignedintSlot(unsigned int))));
    QVERIFY(object.connect(&object,
                           SIGNAL(unsignedSignal(unsigned)),
                           SLOT(unsignedSlot(unsigned))));
    QVERIFY(object.connect(&object,
                           SIGNAL(unsignedSignal(unsigned)),
                           SLOT(uintSlot(uint))));
    QVERIFY(object.connect(&object,
                           SIGNAL(unsignedlongSignal(unsigned long)),
                           SLOT(unsignedlongSlot(unsigned long))));
    QVERIFY(object.connect(&object,
                           SIGNAL(unsignedlonglongSignal(quint64)),
                           SLOT(unsignedlonglongSlot(quint64))));
    QVERIFY(object.connect(&object,
                           SIGNAL(unsignedlongintSignal(unsigned long int)),
                           SLOT(unsignedlongintSlot(unsigned long int))));
    QVERIFY(object.connect(&object,
                           SIGNAL(unsignedshortSignal(unsigned short)),
                           SLOT(unsignedshortSlot(unsigned short))));
    QVERIFY(object.connect(&object,
                           SIGNAL(unsignedcharSignal(unsigned char)),
                           SLOT(unsignedcharSlot(unsigned char))));

    // connect when original template signature and mixed usage of 'T<C const &> const &',
    // 'const T<const C &> &', and 'T<const C &>'

    QVERIFY(object.connect(&object,
                           SIGNAL(typeRefSignal(Template<Class &> &)),
                           SLOT(typeRefSlot(Template<Class &> &))));

    QVERIFY(object.connect(&object,
                           SIGNAL(constTypeRefSignal(const Template<const Class &> &)),
                           SLOT(constTypeRefSlot(const Template<const Class &> &))));
    QVERIFY(object.connect(&object,
                           SIGNAL(constTypeRefSignal(const Template<const Class &> &)),
                           SLOT(constTypeRefSlot(const Template<Class const &> &))));
    QVERIFY(object.connect(&object,
                           SIGNAL(constTypeRefSignal(const Template<const Class &> &)),
                           SLOT(constTypeRefSlot(Template<Class const &> const &))));
    QVERIFY(object.connect(&object,
                           SIGNAL(constTypeRefSignal(Template<const Class &> const &)),
                           SLOT(constTypeRefSlot(Template<Class const &> const &))));
    QVERIFY(object.connect(&object,
                           SIGNAL(constTypeRefSignal(Template<Class const &> const &)),
                           SLOT(constTypeRefSlot(Template<Class const &> const &))));

    QVERIFY(object.connect(&object,
                           SIGNAL(constTypeRefSignal(const Template<const Class &> &)),
                           SLOT(typeConstRefSlot(const Template<const Class &> &))));
    QVERIFY(object.connect(&object,
                           SIGNAL(constTypeRefSignal(const Template<const Class &> &)),
                           SLOT(typeConstRefSlot(const Template<Class const &> &))));
    QVERIFY(object.connect(&object,
                           SIGNAL(constTypeRefSignal(const Template<const Class &> &)),
                           SLOT(typeConstRefSlot(Template<Class const &> const &))));
    QVERIFY(object.connect(&object,
                           SIGNAL(constTypeRefSignal(Template<const Class &> const &)),
                           SLOT(typeConstRefSlot(Template<Class const &> const &))));
    QVERIFY(object.connect(&object,
                           SIGNAL(constTypeRefSignal(Template<Class const &> const &)),
                           SLOT(typeConstRefSlot(Template<Class const &> const &))));

    QVERIFY(object.connect(&object,
                           SIGNAL(typeConstRefSignal(const Template<const Class &> &)),
                           SLOT(constTypeRefSlot(const Template<const Class &> &))));
    QVERIFY(object.connect(&object,
                           SIGNAL(typeConstRefSignal(const Template<const Class &> &)),
                           SLOT(constTypeRefSlot(const Template<Class const &> &))));
    QVERIFY(object.connect(&object,
                           SIGNAL(typeConstRefSignal(const Template<const Class &> &)),
                           SLOT(constTypeRefSlot(Template<Class const &> const &))));
    QVERIFY(object.connect(&object,
                           SIGNAL(typeConstRefSignal(Template<const Class &> const &)),
                           SLOT(constTypeRefSlot(Template<Class const &> const &))));
    QVERIFY(object.connect(&object,
                           SIGNAL(typeConstRefSignal(Template<Class const &> const &)),
                           SLOT(constTypeRefSlot(Template<Class const &> const &))));

    QVERIFY(object.connect(&object,
                           SIGNAL(typeConstRefSignal(const Template<const Class &> &)),
                           SLOT(typeConstRefSlot(const Template<const Class &> &))));
    QVERIFY(object.connect(&object,
                           SIGNAL(typeConstRefSignal(const Template<const Class &> &)),
                           SLOT(typeConstRefSlot(const Template<Class const &> &))));
    QVERIFY(object.connect(&object,
                           SIGNAL(typeConstRefSignal(const Template<const Class &> &)),
                           SLOT(typeConstRefSlot(Template<Class const &> const &))));
    QVERIFY(object.connect(&object,
                           SIGNAL(typeConstRefSignal(Template<const Class &> const &)),
                           SLOT(typeConstRefSlot(Template<Class const &> const &))));
    QVERIFY(object.connect(&object,
                           SIGNAL(typeConstRefSignal(Template<Class const &> const &)),
                           SLOT(typeConstRefSlot(Template<Class const &> const &))));

    // same test again, this time with an object compiled with old moc output... we know that
    // it is not possible to connect everything, whic is the purpose for this test
    OldNormalizeObject oldobject;

    QVERIFY(oldobject.connect(&oldobject,
                              SIGNAL(constTypeRefSignal(const Template<const Class &> &)),
                              SLOT(constTypeRefSlot(const Template<const Class &> &))));
    QVERIFY(oldobject.connect(&oldobject,
                              SIGNAL(constTypeRefSignal(const Template<const Class &> &)),
                              SLOT(constTypeRefSlot(const Template<Class const &> &))));
    // this fails in older versions, but passes now due to proper normalizing
    QVERIFY(oldobject.connect(&oldobject,
                               SIGNAL(constTypeRefSignal(const Template<const Class &> &)),
                               SLOT(constTypeRefSlot(Template<Class const &> const &))));
    // this fails in older versions, but passes now due to proper normalizing
    QVERIFY(oldobject.connect(&oldobject,
                               SIGNAL(constTypeRefSignal(Template<const Class &> const &)),
                               SLOT(constTypeRefSlot(Template<Class const &> const &))));
    // this fails in older versions, but passes now due to proper normalizing
    QVERIFY(oldobject.connect(&oldobject,
                               SIGNAL(constTypeRefSignal(Template<Class const &> const &)),
                               SLOT(constTypeRefSlot(Template<Class const &> const &))));

    // these fail in older Qt versions, but pass now due to proper normalizing
    QVERIFY(oldobject.connect(&oldobject,
                               SIGNAL(constTypeRefSignal(const Template<const Class &> &)),
                               SLOT(typeConstRefSlot(const Template<const Class &> &))));
    QVERIFY(oldobject.connect(&oldobject,
                               SIGNAL(constTypeRefSignal(const Template<const Class &> &)),
                               SLOT(typeConstRefSlot(const Template<Class const &> &))));
    QVERIFY(oldobject.connect(&oldobject,
                               SIGNAL(constTypeRefSignal(const Template<const Class &> &)),
                               SLOT(typeConstRefSlot(Template<Class const &> const &))));
    QVERIFY(oldobject.connect(&oldobject,
                               SIGNAL(constTypeRefSignal(Template<const Class &> const &)),
                               SLOT(typeConstRefSlot(Template<Class const &> const &))));
    QVERIFY(oldobject.connect(&oldobject,
                               SIGNAL(constTypeRefSignal(Template<Class const &> const &)),
                               SLOT(typeConstRefSlot(Template<Class const &> const &))));

    // these also fail in older Qt versions, but pass now due to proper normalizing
    QVERIFY(oldobject.connect(&oldobject,
                               SIGNAL(typeConstRefSignal(const Template<const Class &> &)),
                               SLOT(constTypeRefSlot(const Template<const Class &> &))));
    QVERIFY(oldobject.connect(&oldobject,
                               SIGNAL(typeConstRefSignal(const Template<const Class &> &)),
                               SLOT(constTypeRefSlot(const Template<Class const &> &))));
    QVERIFY(oldobject.connect(&oldobject,
                               SIGNAL(typeConstRefSignal(const Template<const Class &> &)),
                               SLOT(constTypeRefSlot(Template<Class const &> const &))));
    QVERIFY(oldobject.connect(&oldobject,
                               SIGNAL(typeConstRefSignal(Template<const Class &> const &)),
                               SLOT(constTypeRefSlot(Template<Class const &> const &))));
    QVERIFY(oldobject.connect(&oldobject,
                               SIGNAL(typeConstRefSignal(Template<Class const &> const &)),
                               SLOT(constTypeRefSlot(Template<Class const &> const &))));

    // this fails in older versions, but passes now due to proper normalizing
    QVERIFY(oldobject.connect(&oldobject,
                               SIGNAL(typeConstRefSignal(const Template<const Class &> &)),
                               SLOT(typeConstRefSlot(const Template<const Class &> &))));
    // this fails in older versions, but passes now due to proper normalizing
    QVERIFY(oldobject.connect(&oldobject,
                               SIGNAL(typeConstRefSignal(const Template<const Class &> &)),
                               SLOT(typeConstRefSlot(const Template<Class const &> &))));
    // this fails in older versions, but passes now due to proper normalizing
    QVERIFY(oldobject.connect(&oldobject,
                               SIGNAL(typeConstRefSignal(const Template<const Class &> &)),
                               SLOT(typeConstRefSlot(Template<Class const &> const &))));
    QVERIFY(oldobject.connect(&oldobject,
                               SIGNAL(typeConstRefSignal(Template<const Class &> const &)),
                               SLOT(typeConstRefSlot(Template<Class const &> const &))));
    QVERIFY(oldobject.connect(&oldobject,
                               SIGNAL(typeConstRefSignal(Template<Class const &> const &)),
                               SLOT(typeConstRefSlot(Template<Class const &> const &))));

    QVERIFY(object.connect(&object,
                           SIGNAL(typePointerConstRefSignal(Class*const&)),
                           SLOT(typePointerConstRefSlot(Class*const&))));
    QVERIFY(object.connect(&object,
                           SIGNAL(typePointerConstRefSignal(Class*const&)),
                           SLOT(typePointerConstRefSlot(Class*))));
    QVERIFY(object.connect(&object,
                           SIGNAL(typePointerConstRefSignal(Class*)),
                           SLOT(typePointerConstRefSlot(Class*const&))));
    QVERIFY(object.connect(&object,
                           SIGNAL(typePointerConstRefSignal(Class*)),
                           SLOT(typePointerConstRefSlot(Class*))));

    QVERIFY( connect(&object, SIGNAL(constTemplateSignal1(Template <int>)),
                     &object , SLOT(constTemplateSlot1 (Template<int > )  ) ));
    QVERIFY( connect(&object, SIGNAL(constTemplateSignal1(Template <int>)),
                     &object , SLOT(constTemplateSlot2 (Template<int > )  ) ));
    QVERIFY( connect(&object, SIGNAL(constTemplateSignal2(Template <const int>)),
                     &object , SLOT(constTemplateSlot3(Template<int const > ) ) ));

    //type does not match
    QTest::ignoreMessage(QtWarningMsg, "QObject::connect: Incompatible sender/receiver arguments\n"
                    "        NormalizeObject::constTemplateSignal1(Template<int>) --> NormalizeObject::constTemplateSlot3(Template<const int>)");
    QVERIFY(!connect(&object, SIGNAL(constTemplateSignal1(Template <int>)),
                     &object , SLOT(constTemplateSlot3(Template<int const> ) ) ));
}

class SiblingDeleter : public QObject
{
public:
    inline SiblingDeleter(QObject *sibling, QObject *parent)
        : QObject(parent), sibling(sibling) {}
    inline virtual ~SiblingDeleter() { delete sibling; }

private:
    QPointer<QObject> sibling;
};


void tst_QObject::childDeletesItsSibling()
{
    QObject *commonParent = new QObject(0);
    QPointer<QObject> child = new QObject(0);
    QPointer<QObject> siblingDeleter = new SiblingDeleter(child, commonParent);
    child->setParent(commonParent);
    delete commonParent; // don't crash
    QVERIFY(!child);
    QVERIFY(!siblingDeleter);
}

void tst_QObject::floatProperty()
{
    PropertyObject obj;
    const int idx = obj.metaObject()->indexOfProperty("myFloat");
    QVERIFY(idx > 0);
    QMetaProperty prop = obj.metaObject()->property(idx);
    QVERIFY(prop.isValid());
    QVERIFY(prop.type() == uint(QMetaType::type("float")));
    QVERIFY(!prop.write(&obj, QVariant("Hello")));
    QVERIFY(prop.write(&obj, qVariantFromValue(128.0f)));
    QVariant v = prop.read(&obj);
    QVERIFY(int(v.userType()) == QMetaType::Float);
    QVERIFY(qVariantValue<float>(v) == 128.0f);
}

void tst_QObject::qrealProperty()
{
    PropertyObject obj;
    const int idx = obj.metaObject()->indexOfProperty("myQReal");
    QVERIFY(idx > 0);
    QMetaProperty prop = obj.metaObject()->property(idx);
    QVERIFY(prop.isValid());
    QVERIFY(prop.type() == uint(QMetaType::type("qreal")));
    QVERIFY(!prop.write(&obj, QVariant("Hello")));

    QVERIFY(prop.write(&obj, qVariantFromValue(128.0f)));
    QVariant v = prop.read(&obj);
    QCOMPARE(v.userType(), qMetaTypeId<qreal>());
    QVERIFY(qVariantValue<qreal>(v) == 128.0);

    QVERIFY(prop.write(&obj, qVariantFromValue(double(127))));
    v = prop.read(&obj);
    QCOMPARE(v.userType(), qMetaTypeId<qreal>());
    QVERIFY(qVariantValue<qreal>(v) == 127.0);
}

class DynamicPropertyObject : public PropertyObject
{
public:
    inline DynamicPropertyObject() {}

    inline virtual bool event(QEvent *e) {
        if (e->type() == QEvent::DynamicPropertyChange) {
            changedDynamicProperties.append(static_cast<QDynamicPropertyChangeEvent *>(e)->propertyName());
        }
        return QObject::event(e);
    }

    QList<QByteArray> changedDynamicProperties;
};

void tst_QObject::dynamicProperties()
{
    DynamicPropertyObject obj;

    QVERIFY(obj.dynamicPropertyNames().isEmpty());

    QVERIFY(obj.setProperty("number", 42));
    QVERIFY(obj.changedDynamicProperties.isEmpty());
    QCOMPARE(obj.property("number").toInt(), 42);

    QVERIFY(!obj.setProperty("number", "invalid string"));
    QVERIFY(obj.changedDynamicProperties.isEmpty());

    QVERIFY(!obj.setProperty("myuserproperty", "Hello"));
    QCOMPARE(obj.changedDynamicProperties.count(), 1);
    QCOMPARE(obj.changedDynamicProperties.first(), QByteArray("myuserproperty"));
    obj.changedDynamicProperties.clear();

    QCOMPARE(obj.property("myuserproperty").toString(), QString("Hello"));

    QCOMPARE(obj.dynamicPropertyNames().count(), 1);
    QCOMPARE(obj.dynamicPropertyNames().first(), QByteArray("myuserproperty"));

    QVERIFY(!obj.setProperty("myuserproperty", QVariant()));

    QCOMPARE(obj.changedDynamicProperties.count(), 1);
    QCOMPARE(obj.changedDynamicProperties.first(), QByteArray("myuserproperty"));
    obj.changedDynamicProperties.clear();

    QVERIFY(obj.property("myuserproperty").isNull());

    QVERIFY(obj.dynamicPropertyNames().isEmpty());
}

void tst_QObject::recursiveSignalEmission()
{
#if defined(Q_OS_SYMBIAN) && defined(Q_CC_NOKIAX86)
    QSKIP("Emulator builds in Symbian do not support launching processes linking to Qt", SkipAll);
#elif defined(QT_NO_PROCESS)
    QSKIP("Test requires QProcess", SkipAll);
#else
    QProcess proc;
    proc.start("./signalbug");
    QVERIFY(proc.waitForFinished());
    QVERIFY(proc.exitStatus() == QProcess::NormalExit);
    QCOMPARE(proc.exitCode(), 0);
#endif
}

void tst_QObject::blockingQueuedConnection()
{
    {
        SenderObject sender;

        MoveToThreadThread thread;
        ReceiverObject receiver;
        receiver.moveToThread(&thread);
        thread.start();

        receiver.connect(&sender, SIGNAL(signal1()), SLOT(slot1()), Qt::BlockingQueuedConnection);
        sender.emitSignal1();
        QVERIFY(receiver.called(1));

        receiver.reset();
        QVERIFY(QMetaObject::invokeMethod(&receiver, "slot1", Qt::BlockingQueuedConnection));
        QVERIFY(receiver.called(1));

        thread.quit();
        QVERIFY(thread.wait());
    }
}

class EventSpy : public QObject
{
    Q_OBJECT

public:
    typedef QList<QPair<QObject *, QEvent::Type> > EventList;

    EventSpy(QObject *parent = 0)
        : QObject(parent)
    { }

    EventList eventList()
    {
        return events;
    }

    void clear()
    {
        events.clear();
    }

    bool eventFilter(QObject *object, QEvent *event)
    {
        events.append(qMakePair(object, event->type()));
        return false;
    }

private:
    EventList events;
};

void tst_QObject::compatibilityChildInsertedEvents()
{
    EventSpy::EventList expected;

    {
        // no children created, so we expect no events
        QObject object;
        EventSpy spy;
        object.installEventFilter(&spy);

        QCoreApplication::postEvent(&object, new QEvent(QEvent::Type(QEvent::User + 1)));

        QCoreApplication::processEvents();

        expected =
            EventSpy::EventList()
            << qMakePair(&object, QEvent::Type(QEvent::User + 1));
        QCOMPARE(spy.eventList(), expected);
    }

    {
        // 2 children, so we expect 2 ChildAdded and 2 ChildInserted events
        QObject object;
        EventSpy spy;
        object.installEventFilter(&spy);

        QCoreApplication::postEvent(&object, new QEvent(QEvent::Type(QEvent::User + 1)));

        QObject child1(&object);
        QObject child2;
        child2.setParent(&object);

        QCoreApplication::postEvent(&object, new QEvent(QEvent::Type(QEvent::User + 2)));

        expected =
            EventSpy::EventList()
            << qMakePair(&object, QEvent::ChildAdded)
            << qMakePair(&object, QEvent::ChildAdded);
        QCOMPARE(spy.eventList(), expected);
        spy.clear();

        QCoreApplication::processEvents();

        expected =
            EventSpy::EventList()
#ifdef QT_HAS_QT3SUPPORT
            << qMakePair(&object, QEvent::ChildInsertedRequest)
            << qMakePair(&object, QEvent::ChildInserted)
            << qMakePair(&object, QEvent::ChildInserted)
#endif
            << qMakePair(&object, QEvent::Type(QEvent::User + 1))
            << qMakePair(&object, QEvent::Type(QEvent::User + 2));
        QCOMPARE(spy.eventList(), expected);
    }

    {
        // 2 children, but one is reparented away, so we expect:
        // 2 ChildAdded, 1 ChildRemoved, and 1 ChildInserted
        QObject object;
        EventSpy spy;
        object.installEventFilter(&spy);

        QCoreApplication::postEvent(&object, new QEvent(QEvent::Type(QEvent::User + 1)));

        QObject child1(&object);
        QObject child2;
        child2.setParent(&object);
        child2.setParent(0);

        QCoreApplication::postEvent(&object, new QEvent(QEvent::Type(QEvent::User + 2)));

        expected =
            EventSpy::EventList()
            << qMakePair(&object, QEvent::ChildAdded)
            << qMakePair(&object, QEvent::ChildAdded)
            << qMakePair(&object, QEvent::ChildRemoved);
        QCOMPARE(spy.eventList(), expected);
        spy.clear();

        QCoreApplication::processEvents();

        expected =
            EventSpy::EventList()
#ifdef QT_HAS_QT3SUPPORT
            << qMakePair(&object, QEvent::ChildInsertedRequest)
            << qMakePair(&object, QEvent::ChildInserted)
#endif
            << qMakePair(&object, QEvent::Type(QEvent::User + 1))
            << qMakePair(&object, QEvent::Type(QEvent::User + 2));
        QCOMPARE(spy.eventList(), expected);
    }
}

void tst_QObject::installEventFilter()
{
    QEvent event(QEvent::User);
    EventSpy::EventList expected;

    QObject object;
    EventSpy spy;
    object.installEventFilter(&spy);

    // nothing special, should just work
    QCoreApplication::sendEvent(&object, &event);
    expected =
        EventSpy::EventList()
        << qMakePair(&object, QEvent::User);
    QCOMPARE(spy.eventList(), expected);
    spy.clear();

    // moving the filter causes QCoreApplication to skip the filter
    spy.moveToThread(0);
    QTest::ignoreMessage(QtWarningMsg, "QCoreApplication: Object event filter cannot be in a different thread.");
    QCoreApplication::sendEvent(&object, &event);
    QVERIFY(spy.eventList().isEmpty());

    // move it back, and the filter works again
    spy.moveToThread(object.thread());
    QCoreApplication::sendEvent(&object, &event);
    expected =
        EventSpy::EventList()
        << qMakePair(&object, QEvent::User);
    QCOMPARE(spy.eventList(), expected);
    spy.clear();

    // cannot install an event filter that lives in a different thread
    object.removeEventFilter(&spy);
    spy.moveToThread(0);
    QTest::ignoreMessage(QtWarningMsg, "QObject::installEventFilter(): Cannot filter events for objects in a different thread.");
    object.installEventFilter(&spy);
    QCoreApplication::sendEvent(&object, &event);
    QVERIFY(spy.eventList().isEmpty());
}

class EmitThread : public QThread
{   Q_OBJECT
public:
    void run(void) {
        emit work();
    }
signals:
    void work();
};

class DeleteObject : public QObject
{
    Q_OBJECT

public slots:
    void deleteSelf()
    {
        delete this;
    }

    void relaySignalAndProcessEvents()
    {
        emit relayedSignal();
        QCoreApplication::processEvents();
    }

signals:
    void relayedSignal();
};

void tst_QObject::deleteSelfInSlot()
{
    {
        SenderObject sender;
        DeleteObject *receiver = new DeleteObject();
        receiver->connect(&sender,
                          SIGNAL(signal1()),
                          SLOT(deleteSelf()),
                          Qt::BlockingQueuedConnection);

        QThread thread;
        receiver->moveToThread(&thread);
        thread.connect(receiver, SIGNAL(destroyed()), SLOT(quit()), Qt::DirectConnection);
        thread.start();

        QPointer<DeleteObject> p = receiver;
        sender.emitSignal1();
        QVERIFY(p.isNull());

        QVERIFY(thread.wait(10000));
    }

    {
        SenderObject sender;
        DeleteObject *receiver = new DeleteObject();
        receiver->connect(&sender,
                          SIGNAL(signal1()),
                          SLOT(relaySignalAndProcessEvents()),
                          Qt::BlockingQueuedConnection);
        receiver->connect(receiver,
                          SIGNAL(relayedSignal()),
                          SLOT(deleteSelf()),
                          Qt::QueuedConnection);

        QThread thread;
        receiver->moveToThread(&thread);
        thread.connect(receiver, SIGNAL(destroyed()), SLOT(quit()), Qt::DirectConnection);
        thread.start();

        QPointer<DeleteObject> p = receiver;
        sender.emitSignal1();
        QVERIFY(p.isNull());

        QVERIFY(thread.wait(10000));
    }

    {
        EmitThread sender;
        DeleteObject *receiver = new DeleteObject();
        connect(&sender, SIGNAL(work()), receiver, SLOT(deleteSelf()), Qt::DirectConnection);
        QPointer<DeleteObject> p = receiver;
        sender.start();
        QVERIFY(sender.wait(10000));
        QVERIFY(p.isNull());
    }
}

class DisconnectObject : public QObject
{
    Q_OBJECT

public slots:
    void disconnectSelf()
    {
        disconnect(sender(), 0, this, 0);
    }

    void relaySignalAndProcessEvents()
    {
        emit relayedSignal();
        QCoreApplication::processEvents();
    }

signals:
    void relayedSignal();
};

void tst_QObject::disconnectSelfInSlotAndDeleteAfterEmit()
{
    {
        SenderObject sender;
        DisconnectObject *receiver = new DisconnectObject();
        receiver->connect(&sender, SIGNAL(signal1()), SLOT(disconnectSelf()));
        sender.emitSignal1AfterRecursion();
        delete receiver;
    }

    {
        SenderObject sender;
        DisconnectObject *receiver = new DisconnectObject();
        receiver->connect(&sender,
                          SIGNAL(signal1()),
                          SLOT(disconnectSelf()),
                          Qt::BlockingQueuedConnection);

        QThread thread;
        receiver->moveToThread(&thread);
        thread.connect(receiver, SIGNAL(destroyed()), SLOT(quit()), Qt::DirectConnection);
        thread.start();

        QPointer<DisconnectObject> p = receiver;
        sender.emitSignal1();
        QVERIFY(!p.isNull());

        receiver->deleteLater();

        QVERIFY(thread.wait(10000));
        QVERIFY(p.isNull());
    }

    {
        SenderObject sender;
        DisconnectObject *receiver = new DisconnectObject();
        receiver->connect(&sender,
                          SIGNAL(signal1()),
                          SLOT(relaySignalAndProcessEvents()),
                          Qt::BlockingQueuedConnection);
        receiver->connect(receiver,
                          SIGNAL(relayedSignal()),
                          SLOT(disconnectSelf()),
                          Qt::QueuedConnection);

        QThread thread;
        receiver->moveToThread(&thread);
        thread.connect(receiver, SIGNAL(destroyed()), SLOT(quit()), Qt::DirectConnection);
        thread.start();

        QPointer<DisconnectObject> p = receiver;
        sender.emitSignal1();
        QVERIFY(!p.isNull());

        receiver->deleteLater();

        QVERIFY(thread.wait(10000));
        QVERIFY(p.isNull());
    }
}

void tst_QObject::dumpObjectInfo()
{
    QObject a, b;
    QObject::connect(&a, SIGNAL(destroyed(QObject *)), &b, SLOT(deleteLater()));
    a.disconnect(&b);
    a.dumpObjectInfo(); // should not crash
}

class ConnectToSender : public QObject
{ Q_OBJECT
    public slots:
        void uselessSlot() { count++; }

        void harmfullSlot() {
            //this used to crash
            connect(sender(), SIGNAL(signal4()), this, SLOT(uselessSlot()));
            //play a little bit with the memory in order to really get a segfault.
            connect(sender(), SIGNAL(signal1()), this, SLOT(uselessSlot()));
            QList<double>() << 45 << 78 << 65 << 121 << 45 << 78 << 12;
        }
    public:
        int count;
};

void tst_QObject::connectToSender()
{
    SenderObject s;
    ConnectToSender r;
    r.count = 0;
    QObject::connect(&s, SIGNAL(signal1()), &r, SLOT(harmfullSlot()));
    QObject::connect(&s, SIGNAL(signal1()), &r, SLOT(uselessSlot()));

    s.emitSignal1();

    QCOMPARE(r.count, 1);
    s.emitSignal4();
    QCOMPARE(r.count, 2);
}

void tst_QObject::qobjectConstCast()
{
    FooObject obj;

    QObject *ptr = &obj;
    const QObject *cptr = &obj;

    QVERIFY(qobject_cast<FooObject *>(ptr));
    QVERIFY(qobject_cast<const FooObject *>(cptr));
}

void tst_QObject::uniqConnection()
{
    SenderObject *s = new SenderObject;
    ReceiverObject *r1 = new ReceiverObject;
    ReceiverObject *r2 = new ReceiverObject;
    r1->reset();
    r2->reset();
    ReceiverObject::sequence = 0;

    QVERIFY( connect( s, SIGNAL( signal1() ), r1, SLOT( slot1() ) , Qt::UniqueConnection) );
    QVERIFY( connect( s, SIGNAL( signal1() ), r2, SLOT( slot1() ) , Qt::UniqueConnection) );
    QVERIFY( connect( s, SIGNAL( signal1() ), r1, SLOT( slot3() ) , Qt::UniqueConnection) );
    QVERIFY( connect( s, SIGNAL( signal3() ), r1, SLOT( slot3() ) , Qt::UniqueConnection) );

    s->emitSignal1();
    s->emitSignal2();
    s->emitSignal3();
    s->emitSignal4();

    QCOMPARE( r1->count_slot1, 1 );
    QCOMPARE( r1->count_slot2, 0 );
    QCOMPARE( r1->count_slot3, 2 );
    QCOMPARE( r1->count_slot4, 0 );
    QCOMPARE( r2->count_slot1, 1 );
    QCOMPARE( r2->count_slot2, 0 );
    QCOMPARE( r2->count_slot3, 0 );
    QCOMPARE( r2->count_slot4, 0 );
    QCOMPARE( r1->sequence_slot1, 1 );
    QCOMPARE( r2->sequence_slot1, 2 );
    QCOMPARE( r1->sequence_slot3, 4 );

    r1->reset();
    r2->reset();
    ReceiverObject::sequence = 0;

    QVERIFY( connect( s, SIGNAL( signal4() ), r1, SLOT( slot4() ) , Qt::UniqueConnection) );
    QVERIFY( connect( s, SIGNAL( signal4() ), r2, SLOT( slot4() ) , Qt::UniqueConnection) );
    QVERIFY(!connect( s, SIGNAL( signal4() ), r2, SLOT( slot4() ) , Qt::UniqueConnection) );
    QVERIFY( connect( s, SIGNAL( signal1() ), r2, SLOT( slot4() ) , Qt::UniqueConnection) );
    QVERIFY(!connect( s, SIGNAL( signal4() ), r1, SLOT( slot4() ) , Qt::UniqueConnection) );

    s->emitSignal4();
    QCOMPARE( r1->count_slot4, 1 );
    QCOMPARE( r2->count_slot4, 1 );
    QCOMPARE( r1->sequence_slot4, 1 );
    QCOMPARE( r2->sequence_slot4, 2 );

    r1->reset();
    r2->reset();
    ReceiverObject::sequence = 0;

    connect( s, SIGNAL( signal4() ), r1, SLOT( slot4() ) );

    s->emitSignal4();
    QCOMPARE( r1->count_slot4, 2 );
    QCOMPARE( r2->count_slot4, 1 );
    QCOMPARE( r1->sequence_slot4, 3 );
    QCOMPARE( r2->sequence_slot4, 2 );

    delete s;
    delete r1;
    delete r2;
}

void tst_QObject::interfaceIid()
{
    QCOMPARE(QByteArray(qobject_interface_iid<Foo::Bleh *>()),
             QByteArray(Bleh_iid));
    QCOMPARE(QByteArray(qobject_interface_iid<Foo::Bar *>()),
             QByteArray("com.qtest.foobar"));
    QCOMPARE(QByteArray(qobject_interface_iid<FooObject *>()),
             QByteArray());
}

void tst_QObject::deleteQObjectWhenDeletingEvent()
{
    //this is related to task 259514
    //before the fix this used to dead lock when the QObject from the event was destroyed

    struct MyEvent : public QEvent
    {
        MyEvent() : QEvent(QEvent::User) { }
        QObject obj;
    };

    QObject o;
    QApplication::postEvent(&o, new MyEvent);
    QCoreApplication::removePostedEvents(&o); // here you would get a deadlock
}

class OverloadObject : public QObject
{
    friend class tst_QObject;
    Q_OBJECT
    signals:
        void sig(int i, char c, qreal m = 12);
        void sig(int i, int j = 12);
        void sig(QObject *o, QObject *p, QObject *q = 0, QObject *r = 0) const;
        void other(int a = 0);
        void sig(QObject *o, OverloadObject *p = 0, QObject *q = 0, QObject *r = 0);
        void sig(double r = 0.5);
    public slots:
        void slo(int i, int j = 43)
        {
            s_num += 1;
            i1_num = i;
            i2_num = j;
        }
        void slo(QObject *o, QObject *p = qApp, QObject *q = qApp, QObject *r = qApp)
        {
            s_num += 10;
            o1_obj = o;
            o2_obj = p;
            o3_obj = q;
            o4_obj = r;
        }
        void slo()
        {
            s_num += 100;
        }

    public:
        int s_num;
        int i1_num;
        int i2_num;
        QObject *o1_obj;
        QObject *o2_obj;
        QObject *o3_obj;
        QObject *o4_obj;
};

void tst_QObject::overloads()
{
    OverloadObject obj1;
    OverloadObject obj2;
    QObject obj3;
    obj1.s_num = 0;
    obj2.s_num = 0;

    connect (&obj1, SIGNAL(sig(int)) , &obj1, SLOT(slo(int)));
    connect (&obj1, SIGNAL(sig(QObject *, QObject *, QObject *)) , &obj1, SLOT(slo(QObject * , QObject *, QObject *)));

    connect (&obj1, SIGNAL(sig(QObject *, QObject *, QObject *, QObject *)) , &obj2, SLOT(slo(QObject * , QObject *, QObject *)));
    connect (&obj1, SIGNAL(sig(QObject *)) , &obj2, SLOT(slo()));
    connect (&obj1, SIGNAL(sig(int, int)) , &obj2, SLOT(slo(int, int)));

    emit obj1.sig(0.5); //connected to nothing
    emit obj1.sig(1, 'a'); //connected to nothing
    QCOMPARE(obj1.s_num, 0);
    QCOMPARE(obj2.s_num, 0);

    emit obj1.sig(1); //this signal is connected
    QCOMPARE(obj1.s_num, 1);
    QCOMPARE(obj1.i1_num, 1);
    QCOMPARE(obj1.i2_num, 43); //default argument of the slot

    QCOMPARE(obj2.s_num, 1);
    QCOMPARE(obj2.i1_num, 1);
    QCOMPARE(obj2.i2_num, 12); //default argument of the signal


    emit obj1.sig(&obj2); //this signal is conencted to obj2
    QCOMPARE(obj1.s_num, 1);
    QCOMPARE(obj2.s_num, 101);
    emit obj1.sig(&obj2, &obj3); //this signal is connected
    QCOMPARE(obj1.s_num, 11);
    QCOMPARE(obj1.o1_obj, &obj2);
    QCOMPARE(obj1.o2_obj, &obj3);
    QCOMPARE(obj1.o3_obj, (QObject *)0); //default arg of the signal
    QCOMPARE(obj1.o4_obj, qApp); //default arg of the slot

    QCOMPARE(obj2.s_num, 111);
    QCOMPARE(obj2.o1_obj, &obj2);
    QCOMPARE(obj2.o2_obj, &obj3);
    QCOMPARE(obj2.o3_obj, (QObject *)0); //default arg of the signal
    QCOMPARE(obj2.o4_obj, qApp); //default arg of the slot
}

class ManySignals : public QObject
{   Q_OBJECT
    friend class tst_QObject;
signals:
    void sig00(); void sig01(); void sig02(); void sig03(); void sig04();
    void sig05(); void sig06(); void sig07(); void sig08(); void sig09();
    void sig10(); void sig11(); void sig12(); void sig13(); void sig14();
    void sig15(); void sig16(); void sig17(); void sig18(); void sig19();
    void sig20(); void sig21(); void sig22(); void sig23(); void sig24();
    void sig25(); void sig26(); void sig27(); void sig28(); void sig29();
    void sig30(); void sig31(); void sig32(); void sig33(); void sig34();
    void sig35(); void sig36(); void sig37(); void sig38(); void sig39();
    void sig40(); void sig41(); void sig42(); void sig43(); void sig44();
    void sig45(); void sig46(); void sig47(); void sig48(); void sig49();
    void sig50(); void sig51(); void sig52(); void sig53(); void sig54();
    void sig55(); void sig56(); void sig57(); void sig58(); void sig59();
    void sig60(); void sig61(); void sig62(); void sig63(); void sig64();
    void sig65(); void sig66(); void sig67(); void sig68(); void sig69();

public slots:
    void received() { rec++; }
public:
    int rec;
};


void tst_QObject::isSignalConnected()
{
    ManySignals o;
    o.rec = 0;
#ifdef QT_BUILD_INTERNAL
    QObjectPrivate *priv = QObjectPrivate::get(&o);
    QVERIFY(!priv->isSignalConnected(priv->signalIndex("destroyed()")));
    QVERIFY(!priv->isSignalConnected(priv->signalIndex("sig00()")));
    QVERIFY(!priv->isSignalConnected(priv->signalIndex("sig05()")));
    QVERIFY(!priv->isSignalConnected(priv->signalIndex("sig15()")));
    QVERIFY(!priv->isSignalConnected(priv->signalIndex("sig29()")));
    QVERIFY(!priv->isSignalConnected(priv->signalIndex("sig60()")));
    QVERIFY(!priv->isSignalConnected(priv->signalIndex("sig61()")));
#endif

    QObject::connect(&o, SIGNAL(sig00()), &o, SIGNAL(sig69()));
    QObject::connect(&o, SIGNAL(sig34()), &o, SIGNAL(sig03()));
    QObject::connect(&o, SIGNAL(sig69()), &o, SIGNAL(sig34()));
    QObject::connect(&o, SIGNAL(sig03()), &o, SIGNAL(sig18()));

#ifdef QT_BUILD_INTERNAL
    QVERIFY(!priv->isSignalConnected(priv->signalIndex("destroyed()")));
    QVERIFY(!priv->isSignalConnected(priv->signalIndex("sig05()")));
    QVERIFY(!priv->isSignalConnected(priv->signalIndex("sig15()")));
    QVERIFY(!priv->isSignalConnected(priv->signalIndex("sig29()")));

    QVERIFY(priv->isSignalConnected(priv->signalIndex("sig00()")));
    QVERIFY(priv->isSignalConnected(priv->signalIndex("sig03()")));
    QVERIFY(priv->isSignalConnected(priv->signalIndex("sig34()")));
    QVERIFY(priv->isSignalConnected(priv->signalIndex("sig69()")));
    QVERIFY(!priv->isSignalConnected(priv->signalIndex("sig18()")));
#endif

    QObject::connect(&o, SIGNAL(sig18()), &o, SIGNAL(sig29()));
    QObject::connect(&o, SIGNAL(sig29()), &o, SIGNAL(sig62()));
    QObject::connect(&o, SIGNAL(sig62()), &o, SIGNAL(sig28()));
    QObject::connect(&o, SIGNAL(sig28()), &o, SIGNAL(sig27()));

#ifdef QT_BUILD_INTERNAL
    QVERIFY(priv->isSignalConnected(priv->signalIndex("sig18()")));
    QVERIFY(priv->isSignalConnected(priv->signalIndex("sig62()")));
    QVERIFY(priv->isSignalConnected(priv->signalIndex("sig28()")));
    QVERIFY(priv->isSignalConnected(priv->signalIndex("sig69()")));
    QVERIFY(!priv->isSignalConnected(priv->signalIndex("sig27()")));
#endif

    QCOMPARE(o.rec, 0);
    emit o.sig01();
    emit o.sig34();
    QCOMPARE(o.rec, 0);

    QObject::connect(&o, SIGNAL(sig27()), &o, SLOT(received()));

#ifdef QT_BUILD_INTERNAL
    QVERIFY(priv->isSignalConnected(priv->signalIndex("sig00()")));
    QVERIFY(priv->isSignalConnected(priv->signalIndex("sig03()")));
    QVERIFY(priv->isSignalConnected(priv->signalIndex("sig34()")));
    QVERIFY(priv->isSignalConnected(priv->signalIndex("sig18()")));
    QVERIFY(priv->isSignalConnected(priv->signalIndex("sig62()")));
    QVERIFY(priv->isSignalConnected(priv->signalIndex("sig28()")));
    QVERIFY(priv->isSignalConnected(priv->signalIndex("sig69()")));
    QVERIFY(priv->isSignalConnected(priv->signalIndex("sig27()")));

    QVERIFY(!priv->isSignalConnected(priv->signalIndex("sig04()")));
    QVERIFY(!priv->isSignalConnected(priv->signalIndex("sig21()")));
    QVERIFY(!priv->isSignalConnected(priv->signalIndex("sig25()")));
    QVERIFY(!priv->isSignalConnected(priv->signalIndex("sig55()")));
    QVERIFY(!priv->isSignalConnected(priv->signalIndex("sig61()")));
#endif

    emit o.sig00();
    QCOMPARE(o.rec, 1);
    emit o.sig69();
    QCOMPARE(o.rec, 2);
    emit o.sig36();
    QCOMPARE(o.rec, 2);
}

void tst_QObject::qMetaObjectConnect()
{
    SenderObject *s = new SenderObject;
    ReceiverObject *r1 = new ReceiverObject;
    ReceiverObject *r2 = new ReceiverObject;
    r1->reset();
    r2->reset();
    ReceiverObject::sequence = 0;

    int signal1Index = s->metaObject()->indexOfSignal("signal1()");
    int signal3Index = s->metaObject()->indexOfSignal("signal3()");
    int slot1Index = r1->metaObject()->indexOfSlot("slot1()");
    int slot2Index = r1->metaObject()->indexOfSlot("slot2()");
    int slot3Index = r1->metaObject()->indexOfSlot("slot3()");

    QVERIFY(slot1Index > 0);
    QVERIFY(slot2Index > 0);
    QVERIFY(slot3Index > 0);

    QVERIFY( QMetaObject::connect( s, signal1Index, r1, slot1Index) );
    QVERIFY( QMetaObject::connect( s, signal3Index, r2, slot3Index) );
    QVERIFY( QMetaObject::connect( s, -1, r2, slot2Index) );

    QCOMPARE( r1->count_slot1, 0 );
    QCOMPARE( r1->count_slot2, 0 );
    QCOMPARE( r1->count_slot3, 0 );
    QCOMPARE( r2->count_slot1, 0 );
    QCOMPARE( r2->count_slot2, 0 );
    QCOMPARE( r2->count_slot3, 0 );

    s->emitSignal1();

    QCOMPARE( r1->count_slot1, 1 );
    QCOMPARE( r1->count_slot2, 0 );
    QCOMPARE( r1->count_slot3, 0 );
    QCOMPARE( r2->count_slot1, 0 );
    QCOMPARE( r2->count_slot2, 1 );
    QCOMPARE( r2->count_slot3, 0 );

    s->emitSignal2();
    s->emitSignal3();
    s->emitSignal4();

    QCOMPARE( r1->count_slot1, 1 );
    QCOMPARE( r1->count_slot2, 0 );
    QCOMPARE( r1->count_slot3, 0 );
    QCOMPARE( r2->count_slot1, 0 );
    QCOMPARE( r2->count_slot2, 4 );
    QCOMPARE( r2->count_slot3, 1 );

    QVERIFY( QMetaObject::disconnect( s, signal1Index, r1, slot1Index) );
    QVERIFY( QMetaObject::disconnect( s, signal3Index, r2, slot3Index) );
    QVERIFY( QMetaObject::disconnect( s, -1, r2, slot2Index) );

    s->emitSignal1();
    s->emitSignal2();
    s->emitSignal3();
    s->emitSignal4();

    QCOMPARE( r1->count_slot1, 1 );
    QCOMPARE( r1->count_slot2, 0 );
    QCOMPARE( r1->count_slot3, 0 );
    QCOMPARE( r2->count_slot1, 0 );
    QCOMPARE( r2->count_slot2, 4 );
    QCOMPARE( r2->count_slot3, 1 );

    //some "dynamic" signal
    QVERIFY( QMetaObject::connect( s, s->metaObject()->methodOffset() + 20, r1, slot3Index) );
    QVERIFY( QMetaObject::connect( s, s->metaObject()->methodOffset() + 35, r2, slot1Index) );
    QVERIFY( QMetaObject::connect( s, -1, r1, slot2Index) );

    r1->reset();
    r2->reset();

    void *args[] = { 0 , 0 };
    QMetaObject::activate(s, s->metaObject()->methodOffset() + 20, args);
    QMetaObject::activate(s, s->metaObject()->methodOffset() + 48, args);
    QCOMPARE( r1->count_slot1, 0 );
    QCOMPARE( r1->count_slot2, 2 );
    QCOMPARE( r1->count_slot3, 1 );
    QCOMPARE( r2->count_slot1, 0 );
    QCOMPARE( r2->count_slot2, 0 );
    QCOMPARE( r2->count_slot3, 0 );

    QMetaObject::activate(s, s->metaObject()->methodOffset() + 35, args);
    s->emitSignal1();
    s->emitSignal2();

    QCOMPARE( r1->count_slot1, 0 );
    QCOMPARE( r1->count_slot2, 5 );
    QCOMPARE( r1->count_slot3, 1 );
    QCOMPARE( r2->count_slot1, 1 );
    QCOMPARE( r2->count_slot2, 0 );
    QCOMPARE( r2->count_slot3, 0 );

    delete s;
    r1->reset();
    r2->reset();

#define SIGNAL_INDEX(S)  obj1.metaObject()->indexOfSignal(QMetaObject::normalizedSignature(#S))
    OverloadObject obj1;
    QObject obj2, obj3;

    QMetaObject::connect(&obj1, SIGNAL_INDEX(sig(int)) , r1, slot1Index);
    QMetaObject::connect(&obj1, SIGNAL_INDEX(sig(QObject *, QObject *, QObject *)) , r2, slot1Index);

    QMetaObject::connect(&obj1, SIGNAL_INDEX(sig(QObject *, QObject *, QObject *, QObject *)) , r1, slot2Index);
    QMetaObject::connect(&obj1, SIGNAL_INDEX(sig(QObject *)) , r2, slot2Index);
    QMetaObject::connect(&obj1, SIGNAL_INDEX(sig(int, int)) , r1, slot3Index);

    emit obj1.sig(0.5); //connected to nothing
    emit obj1.sig(1, 'a'); //connected to nothing
    QCOMPARE( r1->count_slot1, 0 );
    QCOMPARE( r1->count_slot2, 0 );
    QCOMPARE( r1->count_slot3, 0 );
    QCOMPARE( r2->count_slot1, 0 );
    QCOMPARE( r2->count_slot2, 0 );
    QCOMPARE( r2->count_slot3, 0 );

    emit obj1.sig(1); //this signal is connected
    emit obj1.sig(&obj2);

    QCOMPARE( r1->count_slot1, 1 );
    QCOMPARE( r1->count_slot2, 0 );
    QCOMPARE( r1->count_slot3, 1 );
    QCOMPARE( r2->count_slot1, 0 );
    QCOMPARE( r2->count_slot2, 1 );
    QCOMPARE( r2->count_slot3, 0 );

    emit obj1.sig(&obj2, &obj3); //this signal is connected

    QCOMPARE( r1->count_slot1, 1 );
    QCOMPARE( r1->count_slot2, 1 );
    QCOMPARE( r1->count_slot3, 1 );
    QCOMPARE( r2->count_slot1, 1 );
    QCOMPARE( r2->count_slot2, 1 );
    QCOMPARE( r2->count_slot3, 0 );

    delete r1;
    delete r2;

}

void tst_QObject::qMetaObjectDisconnectOne()
{
    SenderObject *s = new SenderObject;
    ReceiverObject *r1 = new ReceiverObject;

    int signal1Index = s->metaObject()->indexOfSignal("signal1()");
    int signal3Index = s->metaObject()->indexOfSignal("signal3()");
    int slot1Index = r1->metaObject()->indexOfSlot("slot1()");
    int slot2Index = r1->metaObject()->indexOfSlot("slot2()");

    QVERIFY(signal1Index > 0);
    QVERIFY(signal3Index > 0);
    QVERIFY(slot1Index > 0);
    QVERIFY(slot2Index > 0);

    QVERIFY( QMetaObject::connect(s, signal1Index, r1, slot1Index) );
    QVERIFY( QMetaObject::connect(s, signal3Index, r1, slot2Index) );
    QVERIFY( QMetaObject::connect(s, signal3Index, r1, slot2Index) );
    QVERIFY( QMetaObject::connect(s, signal3Index, r1, slot2Index) );

    r1->reset();
    QCOMPARE( r1->count_slot1, 0 );
    QCOMPARE( r1->count_slot2, 0 );

    s->emitSignal1();
    QCOMPARE( r1->count_slot1, 1 );
    QCOMPARE( r1->count_slot2, 0 );

    s->emitSignal3();
    QCOMPARE( r1->count_slot1, 1 );
    QCOMPARE( r1->count_slot2, 3 );

    r1->reset();
    QVERIFY( QMetaObject::disconnectOne(s, signal1Index, r1, slot1Index) );
    QVERIFY( QMetaObject::disconnectOne(s, signal3Index, r1, slot2Index) );

    s->emitSignal1();
    QCOMPARE( r1->count_slot1, 0 );
    QCOMPARE( r1->count_slot2, 0 );

    s->emitSignal3();
    QCOMPARE( r1->count_slot1, 0 );
    QCOMPARE( r1->count_slot2, 2 );

    r1->reset();
    QVERIFY( false == QMetaObject::disconnectOne(s, signal1Index, r1, slot1Index) );
    QVERIFY( QMetaObject::disconnectOne(s, signal3Index, r1, slot2Index) );

    s->emitSignal1();
    QCOMPARE( r1->count_slot1, 0 );
    QCOMPARE( r1->count_slot2, 0 );

    s->emitSignal3();
    QCOMPARE( r1->count_slot1, 0 );
    QCOMPARE( r1->count_slot2, 1 );

    r1->reset();
    QVERIFY( false == QMetaObject::disconnectOne(s, signal1Index, r1, slot1Index) );
    QVERIFY( QMetaObject::disconnectOne(s, signal3Index, r1, slot2Index) );

    s->emitSignal1();
    QCOMPARE( r1->count_slot1, 0 );
    QCOMPARE( r1->count_slot2, 0 );

    s->emitSignal3();
    QCOMPARE( r1->count_slot1, 0 );
    QCOMPARE( r1->count_slot2, 0 );

    delete s;
    delete r1;
}

class ConfusingObject : public SenderObject
{ Q_OBJECT
public slots:
    void signal1() { s++; }
signals:
    void aPublicSlot();
public:
    int s;
    ConfusingObject() : s(0) {}
    friend class tst_QObject;
};

void tst_QObject::sameName()
{
    ConfusingObject c1, c2;
    QVERIFY(connect(&c1, SIGNAL(signal1()), &c1, SLOT(signal1())));
    c1.emitSignal1();
    QCOMPARE(c1.s, 1);

    QVERIFY(connect(&c2, SIGNAL(signal1()), &c1, SIGNAL(signal1())));
    c2.emitSignal1();
    QCOMPARE(c1.s, 2);

    QVERIFY(connect(&c2, SIGNAL(aPublicSlot()), &c1, SLOT(signal1())));
    c2.aPublicSlot();
    QCOMPARE(c2.aPublicSlotCalled, 0);
    QCOMPARE(c1.aPublicSlotCalled, 0);
    QCOMPARE(c1.s, 3);

    QVERIFY(connect(&c2, SIGNAL(aPublicSlot()), &c1, SLOT(aPublicSlot())));
    c2.aPublicSlot();
    QCOMPARE(c2.aPublicSlotCalled, 0);
    QCOMPARE(c1.aPublicSlotCalled, 1);
    QCOMPARE(c1.s, 4);
}

QTEST_MAIN(tst_QObject)
#include "tst_qobject.moc"
