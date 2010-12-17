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


#include <QtTest/QtTest>

#include <qcoreapplication.h>
#include <quuid.h>



#include <quuid.h>

//TESTED_CLASS=
//TESTED_FILES=

class tst_QUuid : public QObject
{
    Q_OBJECT

public:
    tst_QUuid();

private slots:
    void toString();
    void isNull();
    void equal();
    void notEqual();

    // Only in Qt > 3.2.x
    void generate();
    void less();
    void more();
    void variants();
    void versions();

    void threadUniqueness();
    void processUniqueness();

public:
    // Variables
    QUuid uuidA;
    QUuid uuidB;
};

tst_QUuid::tst_QUuid()
{
    uuidA = "{fc69b59e-cc34-4436-a43c-ee95d128b8c5}";
    uuidB = "{1ab6e93a-b1cb-4a87-ba47-ec7e99039a7b}";
}


void tst_QUuid::toString()
{
    QCOMPARE(uuidA.toString(), QString("{fc69b59e-cc34-4436-a43c-ee95d128b8c5}"));
}


void tst_QUuid::isNull()
{
    QVERIFY( !uuidA.isNull() );

    QUuid should_be_null_uuid;
    QVERIFY( should_be_null_uuid.isNull() );
}


void tst_QUuid::equal()
{
    QVERIFY( !(uuidA == uuidB) );

    QUuid copy(uuidA);
    QVERIFY(uuidA == copy);

    QUuid assigned;
    assigned = uuidA;
    QVERIFY(uuidA == assigned);
}


void tst_QUuid::notEqual()
{
    QVERIFY( uuidA != uuidB );
}


void tst_QUuid::generate()
{
    QUuid shouldnt_be_null_uuidA = QUuid::createUuid();
    QUuid shouldnt_be_null_uuidB = QUuid::createUuid();
    QVERIFY( !shouldnt_be_null_uuidA.isNull() );
    QVERIFY( !shouldnt_be_null_uuidB.isNull() );
    QVERIFY( shouldnt_be_null_uuidA != shouldnt_be_null_uuidB );
}


void tst_QUuid::less()
{
    QVERIFY( !(uuidA < uuidB) );

    QUuid null_uuid;
    QVERIFY(null_uuid < uuidA); // Null uuid is always less than a valid one
}


void tst_QUuid::more()
{
    QVERIFY( uuidA > uuidB );

    QUuid null_uuid;
    QVERIFY( !(null_uuid > uuidA) ); // Null uuid is always less than a valid one
}


void tst_QUuid::variants()
{
    QVERIFY( uuidA.variant() == QUuid::DCE );
    QVERIFY( uuidB.variant() == QUuid::DCE );

    QUuid NCS = "{3a2f883c-4000-000d-0000-00fb40000000}";
    QVERIFY( NCS.variant() == QUuid::NCS );
}


void tst_QUuid::versions()
{
    QVERIFY( uuidA.version() == QUuid::Random );
    QVERIFY( uuidB.version() == QUuid::Random );

    QUuid DCE_time= "{406c45a0-3b7e-11d0-80a3-0000c08810a7}";
    QVERIFY( DCE_time.version() == QUuid::Time );

    QUuid NCS = "{3a2f883c-4000-000d-0000-00fb40000000}";
    QVERIFY( NCS.version() == QUuid::VerUnknown );
}

class UuidThread : public QThread
{
public:
    QUuid uuid;

    void run()
    {
        uuid = QUuid::createUuid();
    }
};

void tst_QUuid::threadUniqueness()
{
    QVector<UuidThread *> threads(qMax(2, QThread::idealThreadCount()));
    for (int i = 0; i < threads.count(); ++i)
        threads[i] = new UuidThread;
    for (int i = 0; i < threads.count(); ++i)
        threads[i]->start();
    for (int i = 0; i < threads.count(); ++i)
        QVERIFY(threads[i]->wait(1000));
    for (int i = 1; i < threads.count(); ++i)
        QVERIFY(threads[0]->uuid != threads[i]->uuid);
    qDeleteAll(threads);
}

void tst_QUuid::processUniqueness()
{
    QProcess process;
    QString processOneOutput;
    QString processTwoOutput;

    // Start it once
#ifdef Q_OS_MAC
    process.start("testProcessUniqueness/testProcessUniqueness.app");
#else
    process.start("testProcessUniqueness/testProcessUniqueness");
#endif
    QVERIFY(process.waitForFinished());
    processOneOutput = process.readAllStandardOutput();

    // Start it twice
#ifdef Q_OS_MAC
    process.start("testProcessUniqueness/testProcessUniqueness.app");
#else
    process.start("testProcessUniqueness/testProcessUniqueness");
#endif
    QVERIFY(process.waitForFinished());
    processTwoOutput = process.readAllStandardOutput();

    // They should be *different*!
    QVERIFY(processOneOutput != processTwoOutput);
}

QTEST_MAIN(tst_QUuid)
#include "tst_quuid.moc"
