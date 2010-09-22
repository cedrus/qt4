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
#include <qvarlengtharray.h>
#include <qvariant.h>

const int N = 1;

//TESTED_CLASS=
//TESTED_FILES=

class tst_QVarLengthArray : public QObject
{
    Q_OBJECT

public:
    tst_QVarLengthArray() {}
    virtual ~tst_QVarLengthArray() {}

private slots:
    void append();
    void removeLast();
    void oldTests();
    void task214223();
    void QTBUG6718_resize();
    void QTBUG10978_realloc();
};

int fooCtor = 0;
int fooDtor = 0;

struct Foo
{
    int *p;

    Foo() { p = new int; ++fooCtor; }
    Foo(const Foo &/*other*/) { p = new int; ++fooCtor; }

    void operator=(const Foo & /* other */) { }

    ~Foo() { delete p; ++fooDtor; }
};

void tst_QVarLengthArray::append()
{
    QVarLengthArray<QString> v;
    v.append(QString("hello"));

    QVarLengthArray<int> v2; // rocket!
    v2.append(5);
}

void tst_QVarLengthArray::removeLast()
{
    {
        QVarLengthArray<char, 2> v;
        v.append(0);
        v.append(1);
        QCOMPARE(v.size(), 2);
        v.append(2);
        v.append(3);
        QCOMPARE(v.size(), 4);
        v.removeLast();
        QCOMPARE(v.size(), 3);
        v.removeLast();
        QCOMPARE(v.size(), 2);
    }

    {
        QVarLengthArray<QString, 2> v;
        v.append("0");
        v.append("1");
        QCOMPARE(v.size(), 2);
        v.append("2");
        v.append("3");
        QCOMPARE(v.size(), 4);
        v.removeLast();
        QCOMPARE(v.size(), 3);
        v.removeLast();
        QCOMPARE(v.size(), 2);
    }
}

void tst_QVarLengthArray::oldTests()
{
    {
	QVarLengthArray<int, 256> sa(128);
	QVERIFY(sa.data() == &sa[0]);
	sa[0] = 0xfee;
	sa[10] = 0xff;
	QVERIFY(sa[0] == 0xfee);
	QVERIFY(sa[10] == 0xff);
	sa.resize(512);
	QVERIFY(sa.data() == &sa[0]);
	QVERIFY(sa[0] == 0xfee);
	QVERIFY(sa[10] == 0xff);
        QVERIFY(sa.at(0) == 0xfee);
        QVERIFY(sa.at(10) == 0xff);
        QVERIFY(sa.value(0) == 0xfee);
        QVERIFY(sa.value(10) == 0xff);
        QVERIFY(sa.value(1000) == 0);
        QVERIFY(sa.value(1000, 12) == 12);
	QVERIFY(sa.size() == 512);
	sa.reserve(1024);
	QVERIFY(sa.capacity() == 1024);
	QVERIFY(sa.size() == 512);
    }
    {
	QVarLengthArray<QString> sa(10);
	sa[0] = "Hello";
	sa[9] = "World";
	QVERIFY(*sa.data() == "Hello");
	QVERIFY(sa[9] == "World");
	sa.reserve(512);
	QVERIFY(*sa.data() == "Hello");
	QVERIFY(sa[9] == "World");
	sa.resize(512);
	QVERIFY(*sa.data() == "Hello");
	QVERIFY(sa[9] == "World");
    }
    {
        int arr[2] = {1, 2};
        QVarLengthArray<int> sa(10);
        QCOMPARE(sa.size(), 10);
        sa.append(arr, 2);
        QCOMPARE(sa.size(), 12);
        QCOMPARE(sa[10], 1);
        QCOMPARE(sa[11], 2);
    }
    {
        QString arr[2] = { QString("hello"), QString("world") };
        QVarLengthArray<QString> sa(10);
        QCOMPARE(sa.size(), 10);
        sa.append(arr, 2);
        QCOMPARE(sa.size(), 12);
        QCOMPARE(sa[10], QString("hello"));
        QCOMPARE(sa[11], QString("world"));
        QCOMPARE(sa.at(10), QString("hello"));
        QCOMPARE(sa.at(11), QString("world"));
        QCOMPARE(sa.value(10), QString("hello"));
        QCOMPARE(sa.value(11), QString("world"));
        QCOMPARE(sa.value(10000), QString());
        QCOMPARE(sa.value(1212112, QString("none")), QString("none"));
        QCOMPARE(sa.value(-12, QString("neg")), QString("neg"));

        sa.append(arr, 1);
        QCOMPARE(sa.size(), 13);
        QCOMPARE(sa[12], QString("hello"));

        sa.append(arr, 0);
        QCOMPARE(sa.size(), 13);
    }
    {
        // assignment operator and copy constructor

        QVarLengthArray<int> sa(10);
        sa[5] = 5;

        QVarLengthArray<int> sa2(10);
        sa2[5] = 6;
        sa2 = sa;
        QCOMPARE(sa2[5], 5);

        QVarLengthArray<int> sa3(sa);
        QCOMPARE(sa3[5], 5);
    }

    QSKIP("This test causes the machine to crash when allocating too much memory.", SkipSingle);
    {
        QVarLengthArray<Foo> a;
        const int N = 0x7fffffff / sizeof(Foo);
        const int Prealloc = a.capacity();
        const Foo *data0 = a.constData();

        a.resize(N);
        if (a.size() == N) {
            QVERIFY(a.capacity() >= N);
            QCOMPARE(fooCtor, N);
            QCOMPARE(fooDtor, 0);

            for (int i = 0; i < N; i += 35000)
                a[i] = Foo();
        } else {
            // this is the case we're actually testing
            QCOMPARE(a.size(), 0);
            QCOMPARE(a.capacity(), Prealloc);
            QCOMPARE(a.constData(), data0);
            QCOMPARE(fooCtor, 0);
            QCOMPARE(fooDtor, 0);

            a.resize(5);
            QCOMPARE(a.size(), 5);
            QCOMPARE(a.capacity(), Prealloc);
            QCOMPARE(a.constData(), data0);
            QCOMPARE(fooCtor, 5);
            QCOMPARE(fooDtor, 0);

            a.resize(Prealloc + 1);
            QCOMPARE(a.size(), Prealloc + 1);
            QVERIFY(a.capacity() >= Prealloc + 1);
            QVERIFY(a.constData() != data0);
            QCOMPARE(fooCtor, Prealloc + 6);
            QCOMPARE(fooDtor, 5);

            const Foo *data1 = a.constData();

            a.resize(0x10000000);
            QCOMPARE(a.size(), 0);
            QVERIFY(a.capacity() >= Prealloc + 1);
            QVERIFY(a.constData() == data1);
            QCOMPARE(fooCtor, Prealloc + 6);
            QCOMPARE(fooDtor, Prealloc + 6);
        }
    }
}

void tst_QVarLengthArray::task214223()
{
    //creating a QVarLengthArray of the same size as the prealloc size
    // will make the next call to append(const T&) corrupt the memory
    // you should get a segfault pretty soon after that :-)
    QVarLengthArray<float, 1> d(1);
    for (int i=0; i<30; i++)
        d.append(i);
}

void tst_QVarLengthArray::QTBUG6718_resize()
{
    //MOVABLE
    {
        QVarLengthArray<QVariant,1> values(1);
        QCOMPARE(values.size(), 1);
        values[0] = 1;
        values.resize(2);
        QCOMPARE(values[1], QVariant());
        QCOMPARE(values[0], QVariant(1));
        values[1] = 2;
        QCOMPARE(values[1], QVariant(2));
        QCOMPARE(values.size(), 2);
    }

    //POD
    {
        QVarLengthArray<int,1> values(1);
        QCOMPARE(values.size(), 1);
        values[0] = 1;
        values.resize(2);
        QCOMPARE(values[0], 1);
        values[1] = 2;
        QCOMPARE(values[1], 2);
        QCOMPARE(values.size(), 2);
    }

    //COMPLEX
    {
        QVarLengthArray<QVarLengthArray<QString, 15>,1> values(1);
        QCOMPARE(values.size(), 1);
        values[0].resize(10);
        values.resize(2);
        QCOMPARE(values[1].size(), 0);
        QCOMPARE(values[0].size(), 10);
        values[1].resize(20);
        QCOMPARE(values[1].size(), 20);
        QCOMPARE(values.size(), 2);
    }
}

struct MyBase
{
    MyBase()
        : data(this)
        , isCopy(false)
    {
        ++liveCount;
    }

    MyBase(MyBase const &)
        : data(this)
        , isCopy(true)
    {
        ++copyCount;
        ++liveCount;
    }

    MyBase & operator=(MyBase const &)
    {
        if (!isCopy) {
            isCopy = true;
            ++copyCount;
        } else {
            ++errorCount;
        }

        return *this;
    }

    ~MyBase()
    {
        if (isCopy) {
            if (!copyCount)
                ++errorCount;
            else
                --copyCount;
        }

        if (!liveCount)
            ++errorCount;
        else
            --liveCount;
    }

    bool hasMoved() const
    {
        return this != data;
    }

protected:
    MyBase const * const data;
    bool isCopy;

public:
    static int errorCount;
    static int liveCount;
    static int copyCount;
};

int MyBase::errorCount = 0;
int MyBase::liveCount = 0;
int MyBase::copyCount = 0;

struct MyPrimitive
    : MyBase
{
    MyPrimitive()
    {
        ++errorCount;
    }

    ~MyPrimitive()
    {
        ++errorCount;
    }

    MyPrimitive(MyPrimitive const &other)
        : MyBase(other)
    {
        ++errorCount;
    }
};

struct MyMovable
    : MyBase
{
};

struct MyComplex
    : MyBase
{
};

QT_BEGIN_NAMESPACE

Q_DECLARE_TYPEINFO(MyPrimitive, Q_PRIMITIVE_TYPE);
Q_DECLARE_TYPEINFO(MyMovable, Q_MOVABLE_TYPE);
Q_DECLARE_TYPEINFO(MyComplex, Q_COMPLEX_TYPE);

QT_END_NAMESPACE

bool QTBUG10978_proceed = true;

template <class T, int PreAlloc>
int countMoved(QVarLengthArray<T, PreAlloc> const &c)
{
    int result = 0;
    for (int i = 0; i < c.size(); ++i)
        if (c[i].hasMoved())
            ++result;

    return result;
}

template <class T>
void QTBUG10978_test()
{
    QTBUG10978_proceed = false;

    typedef QVarLengthArray<T, 16> Container;
    enum {
        isStatic = QTypeInfo<T>::isStatic,
        isComplex = QTypeInfo<T>::isComplex,

        isPrimitive = !isComplex && !isStatic,
        isMovable = !isStatic
    };

    // Constructors
    Container a;
    QCOMPARE( MyBase::liveCount, 0 );
    QCOMPARE( MyBase::copyCount, 0 );

    QVERIFY( a.capacity() >= 16 );
    QCOMPARE( a.size(), 0 );

    Container b_real(8);
    Container const &b = b_real;
    QCOMPARE( MyBase::liveCount, isPrimitive ? 0 : 8 );
    QCOMPARE( MyBase::copyCount, 0 );

    QVERIFY( b.capacity() >= 16 );
    QCOMPARE( b.size(), 8 );

    // Assignment
    a = b;
    QCOMPARE( MyBase::liveCount, isPrimitive ? 0 : 16 );
    QCOMPARE( MyBase::copyCount, isComplex ? 8 : 0 );
    QVERIFY( a.capacity() >= 16 );
    QCOMPARE( a.size(), 8 );

    QVERIFY( b.capacity() >= 16 );
    QCOMPARE( b.size(), 8 );

    // append
    a.append(b.data(), b.size());
    QCOMPARE( MyBase::liveCount, isPrimitive ? 0 : 24 );
    QCOMPARE( MyBase::copyCount, isComplex ? 16 : 0 );

    QVERIFY( a.capacity() >= 16 );
    QCOMPARE( a.size(), 16 );

    QVERIFY( b.capacity() >= 16 );
    QCOMPARE( b.size(), 8 );

    // removeLast
    a.removeLast();
    QCOMPARE( MyBase::liveCount, isPrimitive ? 0 : 23 );
    QCOMPARE( MyBase::copyCount, isComplex ? 15 : 0 );

    QVERIFY( a.capacity() >= 16 );
    QCOMPARE( a.size(), 15 );

    QVERIFY( b.capacity() >= 16 );
    QCOMPARE( b.size(), 8 );

    // Movable types
    const int capacity = a.capacity();
    if (!isPrimitive)
        QCOMPARE( countMoved(a), 0 );

    // Reserve, no re-allocation
    a.reserve(capacity);
    if (!isPrimitive)
        QCOMPARE( countMoved(a), 0 );
    QCOMPARE( MyBase::liveCount, isPrimitive ? 0 : 23 );
    QCOMPARE( MyBase::copyCount, isComplex ? 15 : 0 );

    QCOMPARE( a.capacity(), capacity );
    QCOMPARE( a.size(), 15 );

    QVERIFY( b.capacity() >= 16 );
    QCOMPARE( b.size(), 8 );

    // Reserve, force re-allocation
    a.reserve(capacity * 2);
    if (!isPrimitive)
        QCOMPARE( countMoved(a), isMovable ? 15 : 0 );
    QCOMPARE( MyBase::liveCount, isPrimitive ? 0 : 23 );
    QCOMPARE( MyBase::copyCount, isComplex ? 15 : 0 );

    QVERIFY( a.capacity() >= capacity * 2 );
    QCOMPARE( a.size(), 15 );

    QVERIFY( b.capacity() >= 16 );
    QCOMPARE( b.size(), 8 );

    // resize, grow
    a.resize(40);
    if (!isPrimitive)
        QCOMPARE( countMoved(a), isMovable ? 15 : 0 );
    QCOMPARE( MyBase::liveCount, isPrimitive ? 0 : 48 );
    QCOMPARE( MyBase::copyCount, isComplex ? 15 : 0 );

    QVERIFY( a.capacity() >= a.size() );
    QCOMPARE( a.size(), 40 );

    QVERIFY( b.capacity() >= 16 );
    QCOMPARE( b.size(), 8 );

    // Copy constructor, allocate
    {
        Container c(a);
        if (!isPrimitive)
            QCOMPARE( countMoved(c), 0 );
        QCOMPARE( MyBase::liveCount, isPrimitive ? 0 : 88 );
        QCOMPARE( MyBase::copyCount, isComplex ? 55 : 0 );

        QVERIFY( a.capacity() >= a.size() );
        QCOMPARE( a.size(), 40 );

        QVERIFY( b.capacity() >= 16 );
        QCOMPARE( b.size(), 8 );

        QVERIFY( c.capacity() >= 40 );
        QCOMPARE( c.size(), 40 );
    }

    // resize, shrink
    a.resize(10);
    if (!isPrimitive)
        QCOMPARE( countMoved(a), isMovable ? 10 : 0 );
    QCOMPARE( MyBase::liveCount, isPrimitive ? 0 : 18 );
    QCOMPARE( MyBase::copyCount, isComplex ? 10 : 0 );

    QVERIFY( a.capacity() >= a.size() );
    QCOMPARE( a.size(), 10 );

    QVERIFY( b.capacity() >= 16 );
    QCOMPARE( b.size(), 8 );

    // Copy constructor, don't allocate
    {
        Container c(a);
        if (!isPrimitive)
            QCOMPARE( countMoved(c), 0 );
        QCOMPARE( MyBase::liveCount, isPrimitive ? 0 : 28 );
        QCOMPARE( MyBase::copyCount, isComplex ? 20 : 0 );

        QVERIFY( a.capacity() >= a.size() );
        QCOMPARE( a.size(), 10 );

        QVERIFY( b.capacity() >= 16 );
        QCOMPARE( b.size(), 8 );

        QVERIFY( c.capacity() >= 16 );
        QCOMPARE( c.size(), 10 );
    }

    a.clear();
    QCOMPARE( a.size(), 0 );

    b_real.clear();
    QCOMPARE( b.size(), 0 );

    QCOMPARE(MyBase::errorCount, 0);
    QCOMPARE(MyBase::liveCount, 0);

    // All done
    QTBUG10978_proceed = true;
}

void tst_QVarLengthArray::QTBUG10978_realloc()
{
    QTBUG10978_test<MyBase>();
    QVERIFY(QTBUG10978_proceed);

    QTBUG10978_test<MyPrimitive>();
    QVERIFY(QTBUG10978_proceed);

    QTBUG10978_test<MyMovable>();
    QVERIFY(QTBUG10978_proceed);

    QTBUG10978_test<MyComplex>();
    QVERIFY(QTBUG10978_proceed);
}

QTEST_APPLESS_MAIN(tst_QVarLengthArray)
#include "tst_qvarlengtharray.moc"
