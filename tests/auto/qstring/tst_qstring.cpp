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
#include <qregexp.h>
#include <qtextcodec.h>
#include <qtextstream.h>
#include <qstringlist.h>
#include <qstringmatcher.h>
#include <qbytearraymatcher.h>
#include <qvariant.h>

#include <qlocale.h>
#include <locale.h>

Q_DECLARE_METATYPE(qlonglong)

//TESTED_CLASS=
//TESTED_FILES=

class tst_QString : public QObject
{
    Q_OBJECT

public:
    tst_QString();
    virtual ~tst_QString();


public slots:
    void init();
    void cleanup();
private slots:
    void fromStdString();
    void toStdString();
    void check_QTextIOStream();
    void check_QTextStream();
    void check_QDataStream();
    void fromRawData();
    void endsWith();
    void startsWith();
    void setNum();
    void toDouble_data();
    void toDouble();
    void toFloat();
    void toLong_data();
    void toLong();
    void toULong_data();
    void toULong();
    void toLongLong();
    void toULongLong();
    void toUInt();
    void toInt();
    void toShort();
    void toUShort();
    void replace_qchar_qchar_data();
    void replace_qchar_qchar();
    void replace_qchar_qstring_data();
    void replace_qchar_qstring();
    void replace_uint_uint_data();
    void replace_uint_uint();
    void replace_uint_uint_extra();
    void replace_string_data();
    void replace_string();
    void replace_regexp_data();
    void replace_regexp();
    void remove_uint_uint_data();
    void remove_uint_uint();
    void remove_string_data();
    void remove_string();
    void remove_regexp_data();
    void remove_regexp();
    void prepend();
    void prepend_bytearray_data();
    void prepend_bytearray();
    void append();
    void append_bytearray_data();
    void append_bytearray();
    void operator_pluseq_bytearray_data();
    void operator_pluseq_bytearray();
    void operator_eqeq_nullstring();
    void operator_smaller();
    void insert();
    void simplified_data();
    void simplified();
    void trimmed();
    void toLower();
    void toUpper();
    void rightJustified();
    void leftJustified();
    void mid();
    void right();
    void left();
    void midRef();
    void rightRef();
    void leftRef();
    void stringRef();
    void contains();
    void lastIndexOf_data();
    void lastIndexOf();
    void indexOf_data();
    void indexOf();
    void indexOf2_data();
    void indexOf2();
    void indexOf3_data();
//  void indexOf3();
    void sprintf();
    void copy();
    void fill();
    void truncate();
    void constructor();
    void constructorQByteArray_data();
    void constructorQByteArray();
    void STL();
    void isEmpty();
    void isNull();
    void acc_01();
    void length_data();
    void length();
    void utf8_data();
    void utf8();
    void fromUtf8_data();
    void fromUtf8();
    void nullFromUtf8();
    void fromLocal8Bit_data();
    void fromLocal8Bit();
    void local8Bit_data();
    void local8Bit();
    void fromLatin1Roundtrip_data();
    void fromLatin1Roundtrip();
    void toLatin1Roundtrip_data();
    void toLatin1Roundtrip();
    void fromLatin1();
    void fromAscii();
    void arg();
    void number();
    void arg_fillChar_data();
    void arg_fillChar();
    void capacity_data();
    void capacity();
    void section_data();
    void section();
    void double_conversion_data();
    void double_conversion();
    void integer_conversion_data();
    void integer_conversion();
    void tortureSprintfDouble();
    void toNum();
    void localeAwareCompare_data();
    void localeAwareCompare();
    void split_data();
    void split();
    void split_regexp();
    void fromUtf16_data();
    void fromUtf16();
    void latin1String();
    void nanAndInf();
    void compare_data();
    void compare();
    void resizeAfterFromRawData();
    void resizeAfterReserve();
    void resizeWithNegative() const;
    void truncateWithNegative() const;
    void QCharRefMutableUnicode() const;
    void QCharRefDetaching() const;
    void sprintfZU() const;
    void repeatedSignature() const;
    void repeated() const;
    void repeated_data() const;
    void task262677remove();
    void QTBUG10404_compareRef();
    void QTBUG9281_arg_locale();
};

typedef QList<int> IntList;

Q_DECLARE_METATYPE(QList<QVariant>)
Q_DECLARE_METATYPE(IntList)

// This next bit is needed for the NAN and INF in string -> number conversion tests
#include <float.h>
#include <limits.h>
#include <math.h>
#if defined (Q_WS_WIN)
#   include <windows.h>
// mingw defines NAN and INFINITY to 0/0 and x/0
#   if defined(Q_CC_GNU)
#      undef NAN
#      undef INFINITY
#   else
#      define isnan(d) _isnan(d)
#   endif
#endif
#if defined (Q_OS_MAC) && !defined isnan
#define isnan(d) __isnand(d)
#endif
#if defined (Q_OS_SOLARIS)
#   include <ieeefp.h>
#endif
#if defined (Q_OS_OSF) && (defined(__DECC) || defined(__DECCXX))
#   define INFINITY DBL_INFINITY
#   define NAN DBL_QNAN
#endif
#if defined(Q_OS_IRIX) && defined(Q_CC_GNU)
#   include <ieeefp.h>
#   define isnan(d) isnand(d)
#endif

enum {
    LittleEndian,
    BigEndian
#ifdef Q_BYTE_ORDER
#  if Q_BYTE_ORDER == Q_BIG_ENDIAN
    , ByteOrder = BigEndian
#  elif Q_BYTE_ORDER == Q_LITTLE_ENDIAN
    , ByteOrder = LittleEndian
#  else
#    error "undefined byte order"
#  endif
};
#else
};
static const unsigned int one = 1;
static const bool ByteOrder = ((*((unsigned char *) &one) == 0) ? BigEndian : LittleEndian);
#endif
#if !defined(INFINITY)
static const unsigned char be_inf_bytes[] = { 0x7f, 0xf0, 0, 0, 0, 0, 0,0 };
static const unsigned char le_inf_bytes[] = { 0, 0, 0, 0, 0, 0, 0xf0, 0x7f };
static inline double inf()
{
    if (ByteOrder == BigEndian)
        return *reinterpret_cast<const double *>(be_inf_bytes);
    return *reinterpret_cast<const double *>(le_inf_bytes);
}
#   define INFINITY (::inf())
#endif
#if !defined(NAN)
static const unsigned char be_nan_bytes[] = { 0x7f, 0xf8, 0, 0, 0, 0, 0,0 };
static const unsigned char le_nan_bytes[] = { 0, 0, 0, 0, 0, 0, 0xf8, 0x7f };
static inline double nan()
{
    if (ByteOrder == BigEndian)
        return *reinterpret_cast<const double *>(be_nan_bytes);
    return *reinterpret_cast<const double *>(le_nan_bytes);
}
#   define NAN (::nan())
#endif

tst_QString::tst_QString()
{
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("ISO 8859-1"));
}

tst_QString::~tst_QString()
{
}

void tst_QString::init()
{
}

void tst_QString::cleanup()
{
    QLocale::setDefault(QString("C"));
}

void tst_QString::remove_uint_uint_data()
{
    replace_uint_uint_data();
}

void tst_QString::remove_string_data()
{
    replace_string_data();
}

void tst_QString::remove_regexp_data()
{
    replace_regexp_data();
}

void tst_QString::indexOf3_data()
{
    indexOf2_data();
}

void tst_QString::length_data()
{
    QTest::addColumn<QString>("s1" );
    QTest::addColumn<int>("res" );

    QTest::newRow( "data0" )  << QString("Test") << 4;
    QTest::newRow( "data1" )  << QString("The quick brown fox jumps over the lazy dog") << 43;
    QTest::newRow( "data2" )  << QString() << 0;
    QTest::newRow( "data3" )  << QString("A") << 1;
    QTest::newRow( "data4" )  << QString("AB") << 2;
    QTest::newRow( "data5" )  << QString("AB\n") << 3;
    QTest::newRow( "data6" )  << QString("AB\nC") << 4;
    QTest::newRow( "data7" )  << QString("\n") << 1;
    QTest::newRow( "data8" )  << QString("\nA") << 2;
    QTest::newRow( "data9" )  << QString("\nAB") << 3;
    QTest::newRow( "data10" )  << QString("\nAB\nCDE") << 7;
    QTest::newRow( "data11" )  << QString("shdnftrheid fhgnt gjvnfmd chfugkh bnfhg thgjf vnghturkf chfnguh bjgnfhvygh hnbhgutjfv dhdnjds dcjs d") << 100;
}

void tst_QString::replace_qchar_qchar_data()
{
    QTest::addColumn<QString>("src" );
    QTest::addColumn<QChar>("before" );
    QTest::addColumn<QChar>("after" );
    QTest::addColumn<int>("cs" );
    QTest::addColumn<QString>("expected" );

    QTest::newRow( "1" ) << QString("foo") << QChar('o') << QChar('a')
                      << int(Qt::CaseSensitive) << QString("faa");
    QTest::newRow( "2" ) << QString("foo") << QChar('o') << QChar('a')
                      << int(Qt::CaseInsensitive) << QString("faa");
    QTest::newRow( "3" ) << QString("foo") << QChar('O') << QChar('a')
                      << int(Qt::CaseSensitive) << QString("foo");
    QTest::newRow( "4" ) << QString("foo") << QChar('O') << QChar('a')
                      << int(Qt::CaseInsensitive) << QString("faa");
    QTest::newRow( "5" ) << QString("ababABAB") << QChar('a') << QChar(' ')
                      << int(Qt::CaseSensitive) << QString(" b bABAB");
    QTest::newRow( "6" ) << QString("ababABAB") << QChar('a') << QChar(' ')
                      << int(Qt::CaseInsensitive) << QString(" b b B B");
    QTest::newRow( "7" ) << QString("ababABAB") << QChar() << QChar(' ')
                      << int(Qt::CaseInsensitive) << QString("ababABAB");
}

void tst_QString::replace_qchar_qchar()
{
    QFETCH(QString, src);
    QFETCH(QChar, before);
    QFETCH(QChar, after);
    QFETCH(int, cs);
    QFETCH(QString, expected);

    QCOMPARE(src.replace(before, after, Qt::CaseSensitivity(cs)), expected);
}

void tst_QString::replace_qchar_qstring_data()
{
    QTest::addColumn<QString>("src" );
    QTest::addColumn<QChar>("before" );
    QTest::addColumn<QString>("after" );
    QTest::addColumn<int>("cs" );
    QTest::addColumn<QString>("expected" );

    QTest::newRow( "1" ) << QString("foo") << QChar('o') << QString("aA")
                      << int(Qt::CaseSensitive) << QString("faAaA");
    QTest::newRow( "2" ) << QString("foo") << QChar('o') << QString("aA")
                      << int(Qt::CaseInsensitive) << QString("faAaA");
    QTest::newRow( "3" ) << QString("foo") << QChar('O') << QString("aA")
                      << int(Qt::CaseSensitive) << QString("foo");
    QTest::newRow( "4" ) << QString("foo") << QChar('O') << QString("aA")
                      << int(Qt::CaseInsensitive) << QString("faAaA");
    QTest::newRow( "5" ) << QString("ababABAB") << QChar('a') << QString("  ")
                      << int(Qt::CaseSensitive) << QString("  b  bABAB");
    QTest::newRow( "6" ) << QString("ababABAB") << QChar('a') << QString("  ")
                      << int(Qt::CaseInsensitive) << QString("  b  b  B  B");
    QTest::newRow( "7" ) << QString("ababABAB") << QChar() << QString("  ")
                      << int(Qt::CaseInsensitive) << QString("ababABAB");
    QTest::newRow( "8" ) << QString("ababABAB") << QChar() << QString()
                      << int(Qt::CaseInsensitive) << QString("ababABAB");
}

void tst_QString::replace_qchar_qstring()
{
    QFETCH(QString, src);
    QFETCH(QChar, before);
    QFETCH(QString, after);
    QFETCH(int, cs);
    QFETCH(QString, expected);

    QCOMPARE(src.replace(before, after, Qt::CaseSensitivity(cs)), expected);
}

void tst_QString::replace_uint_uint_data()
{
    QTest::addColumn<QString>("string" );
    QTest::addColumn<int>("index" );
    QTest::addColumn<int>("len" );
    QTest::addColumn<QString>("after" );
    QTest::addColumn<QString>("result" );

    QTest::newRow( "rem00" ) << QString("-<>ABCABCABCABC>") << 0 << 3 << QString("") << QString("ABCABCABCABC>");
    QTest::newRow( "rem01" ) << QString("ABCABCABCABC>") << 1 << 4 << QString("") << QString("ACABCABC>");
    QTest::newRow( "rem04" ) << QString("ACABCABC>") << 8 << 4 << QString("") << QString("ACABCABC");
    QTest::newRow( "rem05" ) << QString("ACABCABC") << 7 << 1 << QString("") << QString("ACABCAB");
    QTest::newRow( "rem06" ) << QString("ACABCAB") << 4 << 0 << QString("") << QString("ACABCAB");

    QTest::newRow( "rep00" ) << QString("ACABCAB") << 4 << 0 << QString("X") << QString("ACABXCAB");
    QTest::newRow( "rep01" ) << QString("ACABXCAB") << 4 << 1 << QString("Y") << QString("ACABYCAB");
    QTest::newRow( "rep02" ) << QString("ACABYCAB") << 4 << 1 << QString("") << QString("ACABCAB");
    QTest::newRow( "rep03" ) << QString("ACABCAB") << 0 << 9999 << QString("XX") << QString("XX");
    QTest::newRow( "rep04" ) << QString("XX") << 0 << 9999 << QString("") << QString("");
    QTest::newRow( "rep05" ) << QString("ACABCAB") << 0 << 2 << QString("XX") << QString("XXABCAB");
    QTest::newRow( "rep06" ) << QString("ACABCAB") << 1 << 2 << QString("XX") << QString("AXXBCAB");
    QTest::newRow( "rep07" ) << QString("ACABCAB") << 2 << 2 << QString("XX") << QString("ACXXCAB");
    QTest::newRow( "rep08" ) << QString("ACABCAB") << 3 << 2 << QString("XX") << QString("ACAXXAB");
    QTest::newRow( "rep09" ) << QString("ACABCAB") << 4 << 2 << QString("XX") << QString("ACABXXB");
    QTest::newRow( "rep10" ) << QString("ACABCAB") << 5 << 2 << QString("XX") << QString("ACABCXX");
    QTest::newRow( "rep10" ) << QString("ACABCAB") << 6 << 2 << QString("XX") << QString("ACABCAXX");
    QTest::newRow( "rep12" ) << QString() << 0 << 10 << QString("X") << QString("X");
    QTest::newRow( "rep13" ) << QString("short") << 0 << 10 << QString("X") << QString("X");
    QTest::newRow( "rep14" ) << QString() << 0 << 10 << QString("XX") << QString("XX");
    QTest::newRow( "rep15" ) << QString("short") << 0 << 10 << QString("XX") << QString("XX");
}

void tst_QString::replace_string_data()
{
    QTest::addColumn<QString>("string" );
    QTest::addColumn<QString>("before" );
    QTest::addColumn<QString>("after" );
    QTest::addColumn<QString>("result" );
    QTest::addColumn<bool>("bcs" );

    QTest::newRow( "rem00" ) << QString("") << QString("") << QString("") << QString("") << true;
    QTest::newRow( "rem01" ) << QString("A") << QString("") << QString("") << QString("A") << true;
    QTest::newRow( "rem02" ) << QString("A") << QString("A") << QString("") << QString("") << true;
    QTest::newRow( "rem03" ) << QString("A") << QString("B") << QString("") << QString("A") << true;
    QTest::newRow( "rem04" ) << QString("AA") << QString("A") << QString("") << QString("") << true;
    QTest::newRow( "rem05" ) << QString("AB") << QString("A") << QString("") << QString("B") << true;
    QTest::newRow( "rem06" ) << QString("AB") << QString("B") << QString("") << QString("A") << true;
    QTest::newRow( "rem07" ) << QString("AB") << QString("C") << QString("") << QString("AB") << true;
    QTest::newRow( "rem08" ) << QString("ABA") << QString("A") << QString("") << QString("B") << true;
    QTest::newRow( "rem09" ) << QString("ABA") << QString("B") << QString("") << QString("AA") << true;
    QTest::newRow( "rem10" ) << QString("ABA") << QString("C") << QString("") << QString("ABA") << true;
    QTest::newRow( "rem11" ) << QString("banana") << QString("an") << QString("") << QString("ba") << true;
    QTest::newRow( "rem12" ) << QString("") << QString("A") << QString("") << QString("") << true;
    QTest::newRow( "rem13" ) << QString("") << QString("A") << QString() << QString("") << true;
    QTest::newRow( "rem14" ) << QString() << QString("A") << QString("") << QString() << true;
    QTest::newRow( "rem15" ) << QString() << QString("A") << QString() << QString() << true;
    QTest::newRow( "rem16" ) << QString() << QString("") << QString("") << QString("") << true;
    QTest::newRow( "rem17" ) << QString("") << QString() << QString("") << QString("") << true;
    QTest::newRow( "rem18" ) << QString("a") << QString("a") << QString("") << QString("") << false;
    QTest::newRow( "rem19" ) << QString("A") << QString("A") << QString("") << QString("") << false;
    QTest::newRow( "rem20" ) << QString("a") << QString("A") << QString("") << QString("") << false;
    QTest::newRow( "rem21" ) << QString("A") << QString("a") << QString("") << QString("") << false;
    QTest::newRow( "rem22" ) << QString("Alpha beta") << QString("a") << QString("") << QString("lph bet") << false;

    QTest::newRow( "rep00" ) << QString("ABC") << QString("B") << QString("-") << QString("A-C") << true;
    QTest::newRow( "rep01" ) << QString("$()*+.?[\\]^{|}") << QString("$()*+.?[\\]^{|}") << QString("X") << QString("X") << true;
    QTest::newRow( "rep02" ) << QString("ABCDEF") << QString("") << QString("X") << QString("XAXBXCXDXEXFX") << true;
    QTest::newRow( "rep03" ) << QString("") << QString("") << QString("X") << QString("X") << true;
    QTest::newRow( "rep04" ) << QString("a") << QString("a") << QString("b") << QString("b") << false;
    QTest::newRow( "rep05" ) << QString("A") << QString("A") << QString("b") << QString("b") << false;
    QTest::newRow( "rep06" ) << QString("a") << QString("A") << QString("b") << QString("b") << false;
    QTest::newRow( "rep07" ) << QString("A") << QString("a") << QString("b") << QString("b") << false;
    QTest::newRow( "rep08" ) << QString("a") << QString("a") << QString("a") << QString("a") << false;
    QTest::newRow( "rep09" ) << QString("A") << QString("A") << QString("a") << QString("a") << false;
    QTest::newRow( "rep10" ) << QString("a") << QString("A") << QString("a") << QString("a") << false;
    QTest::newRow( "rep11" ) << QString("A") << QString("a") << QString("a") << QString("a") << false;
    QTest::newRow( "rep12" ) << QString("Alpha beta") << QString("a") << QString("o") << QString("olpho beto") << false;
    QTest::newRow( "rep13" ) << QString() << QString("") << QString("A") << QString("A") << true;
    QTest::newRow( "rep14" ) << QString("") << QString() << QString("A") << QString("A") << true;
    QTest::newRow( "rep15" ) << QString("fooxbarxbazxblub") << QString("x") << QString("yz") << QString("fooyzbaryzbazyzblub") << true;
    QTest::newRow( "rep16" ) << QString("fooxbarxbazxblub") << QString("x") << QString("z") << QString("foozbarzbazzblub") << true;
    QTest::newRow( "rep17" ) << QString("fooxybarxybazxyblub") << QString("xy") << QString("z") << QString("foozbarzbazzblub") << true;
}

void tst_QString::replace_regexp_data()
{
    QTest::addColumn<QString>("string" );
    QTest::addColumn<QString>("regexp" );
    QTest::addColumn<QString>("after" );
    QTest::addColumn<QString>("result" );

    QTest::newRow( "rem00" ) << QString("alpha") << QString("a+") << QString("") << QString("lph");
    QTest::newRow( "rem01" ) << QString("banana") << QString("^.a") << QString("") << QString("nana");
    QTest::newRow( "rem02" ) << QString("") << QString("^.a") << QString("") << QString("");
    QTest::newRow( "rem03" ) << QString("") << QString("^.a") << QString() << QString("");
    QTest::newRow( "rem04" ) << QString() << QString("^.a") << QString("") << QString();
    QTest::newRow( "rem05" ) << QString() << QString("^.a") << QString() << QString();

    QTest::newRow( "rep00" ) << QString("A <i>bon mot</i>.") << QString("<i>([^<]*)</i>") << QString("\\emph{\\1}") << QString("A \\emph{bon mot}.");
    QTest::newRow( "rep01" ) << QString("banana") << QString("^.a()") << QString("\\1") << QString("nana");
    QTest::newRow( "rep02" ) << QString("banana") << QString("(ba)") << QString("\\1X\\1") << QString("baXbanana");
    QTest::newRow( "rep03" ) << QString("banana") << QString("(ba)(na)na") << QString("\\2X\\1") << QString("naXba");

    QTest::newRow("backref00") << QString("\\1\\2\\3\\4\\5\\6\\7\\8\\9\\A\\10\\11") << QString("\\\\[34]")
                               << QString("X") << QString("\\1\\2XX\\5\\6\\7\\8\\9\\A\\10\\11");
    QTest::newRow("backref01") << QString("foo") << QString("[fo]") << QString("\\1") << QString("\\1\\1\\1");
    QTest::newRow("backref02") << QString("foo") << QString("([fo])") << QString("(\\1)") << QString("(f)(o)(o)");
    QTest::newRow("backref03") << QString("foo") << QString("([fo])") << QString("\\2") << QString("\\2\\2\\2");
    QTest::newRow("backref04") << QString("foo") << QString("([fo])") << QString("\\10") << QString("f0o0o0");
    QTest::newRow("backref05") << QString("foo") << QString("([fo])") << QString("\\11") << QString("f1o1o1");
    QTest::newRow("backref06") << QString("foo") << QString("([fo])") << QString("\\19") << QString("f9o9o9");
    QTest::newRow("backref07") << QString("foo") << QString("(f)(o+)")
                               << QString("\\2\\1\\10\\20\\11\\22\\19\\29\\3")
                               << QString("ooff0oo0f1oo2f9oo9\\3");
    QTest::newRow("backref08") << QString("abc") << QString("(((((((((((((([abc]))))))))))))))")
                               << QString("{\\14}") << QString("{a}{b}{c}");
    QTest::newRow("backref09") << QString("abcdefghijklmn")
                               << QString("(a)(b)(c)(d)(e)(f)(g)(h)(i)(j)(k)(l)(m)(n)")
                               << QString("\\19\\18\\17\\16\\15\\14\\13\\12\\11\\10"
                                          "\\9\\90\\8\\80\\7\\70\\6\\60\\5\\50\\4\\40\\3\\30\\2\\20\\1")
                               << QString("a9a8a7a6a5nmlkjii0hh0gg0ff0ee0dd0cc0bb0a");
    QTest::newRow("backref10") << QString("abc") << QString("((((((((((((((abc))))))))))))))")
                               << QString("\\0\\01\\011") << QString("\\0\\01\\011");
}

void tst_QString::utf8_data()
{
    QString str;
    QTest::addColumn<QByteArray>("utf8" );
    QTest::addColumn<QString>("res" );

    QTest::newRow( "str0" ) << QByteArray("abcdefgh")
			  << QString("abcdefgh");
    QTest::newRow( "str1" ) << QByteArray("\303\266\303\244\303\274\303\226\303\204\303\234\303\270\303\246\303\245\303\230\303\206\303\205")
			  << QString("\366\344\374\326\304\334\370\346\345\330\306\305") ;
    str += QChar( 0x05e9 );
    str += QChar( 0x05d3 );
    str += QChar( 0x05d2 );
    QTest::newRow( "str2" ) << QByteArray("\327\251\327\223\327\222")
			  << str;

    str = QChar( 0x20ac );
    str += " some text";
    QTest::newRow( "str3" ) << QByteArray("\342\202\254 some text")
			  << str;

    str = "Old Italic: ";
    str += QChar(0xd800);
    str += QChar(0xdf00);
    str += QChar(0xd800);
    str += QChar(0xdf01);
    str += QChar(0xd800);
    str += QChar(0xdf02);
    str += QChar(0xd800);
    str += QChar(0xdf03);
    str += QChar(0xd800);
    str += QChar(0xdf04);
    QTest::newRow("surrogate") << QByteArray("Old Italic: \360\220\214\200\360\220\214\201\360\220\214\202\360\220\214\203\360\220\214\204") << str;
}

void tst_QString::length()
{
    QFETCH( QString, s1 );
    QTEST( (int)s1.length(), "res" );
}

#include <qfile.h>

void tst_QString::acc_01()
{
    QString a;
    QString b; //b(10);
    QString bb; //bb((int)0);
    QString c("String C");
    QChar tmp[10];
    tmp[0] = 'S';
    tmp[1] = 't';
    tmp[2] = 'r';
    tmp[3] = 'i';
    tmp[4] = 'n';
    tmp[5] = 'g';
    tmp[6] = ' ';
    tmp[7] = 'D';
    tmp[8] = 'X';
    tmp[9] = '\0';
    QString d(tmp,8);
    QString ca(a);
    QString cb(b);
    QString cc(c);
    QString n;
    QString e("String E");
    QString f;
    f = e;
    f[7]='F';
    QCOMPARE(e,(QString)"String E");
    char text[]="String f";
    f = text;
    text[7]='!';
    QCOMPARE(f,(QString)"String f");
    f[7]='F';
    QCOMPARE(text[7],'!');

    a="";
    a[0]='A';
    QCOMPARE(a,(QString)"A");
    QCOMPARE(a.length(),1);
    a[1]='B';
    QCOMPARE(a,(QString)"AB");
    QCOMPARE(a.length(),2);
    a[2]='C';
    QCOMPARE(a,(QString)"ABC");
    QCOMPARE(a.length(),3);
    a = QString();
    QVERIFY(a.isNull());
    a[0]='A';
    QCOMPARE(a,(QString)"A");
    QCOMPARE(a.length(),1);
    a[1]='B';
    QCOMPARE(a,(QString)"AB");
    QCOMPARE(a.length(),2);
    a[2]='C';
    QCOMPARE(a,(QString)"ABC");
    QCOMPARE(a.length(),3);

    a="123";
    b="456";
    a[0]=a[1];
    QCOMPARE(a,(QString)"223");
    a[1]=b[1];
    QCOMPARE(b,(QString)"456");
    QCOMPARE(a,(QString)"253");

    char t[]="TEXT";
    a="A";
    a=t;
    QCOMPARE(a,(QString)"TEXT");
    QCOMPARE(a,(QString)t);
    a[0]='X';
    QCOMPARE(a,(QString)"XEXT");
    QCOMPARE(t[0],'T');
    t[0]='Z';
    QCOMPARE(a,(QString)"XEXT");

    a="ABC";
    QCOMPARE(char(a.toLatin1()[1]),'B');
    QCOMPARE(strcmp(a.toLatin1(),((QString)"ABC").toLatin1()),0);
    QCOMPARE(a+="DEF",(QString)"ABCDEF");
    QCOMPARE(a+='G',(QString)"ABCDEFG");
    QCOMPARE(a+=((const char*)(0)),(QString)"ABCDEFG");

    // non-member operators

    a="ABC";
    b="ABC";
    c="ACB";
    d="ABCD";
    QVERIFY(a==b);
    QVERIFY(!(a==d));
    QVERIFY(!(a!=b));
    QVERIFY(a!=d);
    QVERIFY(!(a<b));
    QVERIFY(a<c);
    QVERIFY(a<d);
    QVERIFY(!(d<a));
    QVERIFY(!(c<a));
    QVERIFY(a<=b);
    QVERIFY(a<=d);
    QVERIFY(a<=c);
    QVERIFY(!(c<=a));
    QVERIFY(!(d<=a));
    QCOMPARE(a+b,(QString)"ABCABC");
    QCOMPARE(a	+"XXXX",(QString)"ABCXXXX");
    QCOMPARE(a+'X',(QString)"ABCX");
    QCOMPARE("XXXX"+a,(QString)"XXXXABC");
    QCOMPARE('X'+a,(QString)"XABC");
    a = (const char*)0;
    QVERIFY(a.isNull());
    QVERIFY(*a.toLatin1().constData() == '\0');
    {
#if defined(Q_OS_WINCE)
    int argc = 0;
    QCoreApplication app(argc, 0);
#endif
	QFile f("COMPARE.txt");
	f.open(QIODevice::ReadOnly);
	QTextStream ts( &f );
        ts.setCodec(QTextCodec::codecForName("UTF-16"));
	ts << "Abc";
    }
}

void tst_QString::isNull()
{
    QString a;
    QVERIFY(a.isNull());

    const char *zero = 0;
    a.sprintf( zero );
    QVERIFY(!a.isNull());
}

void tst_QString::isEmpty()
{
    QString a;
    QVERIFY(a.isEmpty());
    QString c("Not empty");
    QVERIFY(!c.isEmpty());
}

void tst_QString::constructor()
{
    QString a;
    QString b; //b(10);
    QString c("String C");
    QChar tmp[10];
    tmp[0] = 'S';
    tmp[1] = 't';
    tmp[2] = 'r';
    tmp[3] = 'i';
    tmp[4] = 'n';
    tmp[5] = 'g';
    tmp[6] = ' ';
    tmp[7] = 'D';
    tmp[8] = 'X';
    tmp[9] = '\0';
    QString d(tmp,8);
    QString ca(a);
    QString cb(b);
    QString cc(c);

    QCOMPARE(a,ca);
    QVERIFY(a.isNull());
    QVERIFY(a == (QString)"");
    QCOMPARE(b,cb);
    QCOMPARE(c,cc);
    QCOMPARE(d,(QString)"String D");

    QString nullStr;
    QVERIFY( nullStr.isNull() );
    QVERIFY( nullStr.isEmpty() );
    QString empty("");
    QVERIFY( !empty.isNull() );
    QVERIFY( empty.isEmpty() );
}



void tst_QString::constructorQByteArray_data()
{
    QTest::addColumn<QByteArray>("src" );
    QTest::addColumn<QString>("expected" );

    QByteArray ba( 4, 0 );
    ba[0] = 'C';
    ba[1] = 'O';
    ba[2] = 'M';
    ba[3] = 'P';

    QTest::newRow( "1" ) << ba << QString("COMP");

    QByteArray ba1( 7, 0 );
    ba1[0] = 'a';
    ba1[1] = 'b';
    ba1[2] = 'c';
    ba1[3] = '\0';
    ba1[4] = 'd';
    ba1[5] = 'e';
    ba1[6] = 'f';

    QTest::newRow( "2" ) << ba1 << QString("abc");

    QTest::newRow( "3" ) << QByteArray::fromRawData("abcd", 3) << QString("abc");
}

void tst_QString::constructorQByteArray()
{
    QFETCH(QByteArray, src);
    QFETCH(QString, expected);

    QString str1(src);
    QCOMPARE(str1.length(), expected.length());
    QCOMPARE( str1, expected );

    QTextCodec::setCodecForCStrings( QTextCodec::codecForMib(4) ); // Latin 1
    QString strBA(src);
    QTextCodec::setCodecForCStrings( 0 );

    QCOMPARE( strBA, expected );
}

void tst_QString::STL()
{
#ifdef Q_CC_HPACC
    QSKIP("This test crashes on HP-UX with aCC", SkipSingle);
#endif
#ifndef QT_NO_STL
#ifndef QT_NO_CAST_TO_ASCII
    QString qt( "QString" );

    std::string stdstr( "QString" );

    QString stlqt;

    // constructor
    stlqt = QString( stdstr );
    QCOMPARE( stlqt, qt );

    // assignment
    stlqt = stdstr;
    QCOMPARE( stlqt, qt );

    std::string stdstr2 = stlqt;
    QCOMPARE( stdstr2, stdstr );

    // prepend
    stlqt = QString();
    stlqt.prepend( stdstr );
    QCOMPARE( stlqt, qt );

    // append
    stlqt = QString();
    stlqt.append( stdstr );
    QCOMPARE( stlqt, qt );

    // pathologics (null-strings not supported by many STLs, so test only empty strings)
    stdstr = std::string();
    stlqt = stdstr;
    QVERIFY( stlqt.isEmpty() );
    std::string stdstr3 = stlqt;
    QVERIFY( !stdstr3.length() );
#endif

//skip test if glibc is not compiled with wide character support
#if (defined Q_CC_GNU && !defined _GLIBCPP_USE_WCHAR_T) || defined QT_NO_STL_WCHAR
    QSKIP( "Not tested without wide character support", SkipAll);
#else
    const wchar_t arr[] = {'h', 'e', 'l', 'l', 'o', 0};
    QStdWString stlStr = arr;

    QString s = QString::fromStdWString(stlStr);

    QCOMPARE(s, QString::fromLatin1("hello"));
    QCOMPARE(stlStr, s.toStdWString());
#endif
#else
    QSKIP( "Not tested without STL support", SkipAll);
#endif
}

void tst_QString::truncate()
{
    QString e("String E");
    e.truncate(4);
    QCOMPARE(e,(QString)"Stri");

    e = "String E";
    e.truncate(0);
    QCOMPARE(e,(QString)"");
    QVERIFY(e.isEmpty());
    QVERIFY(!e.isNull());

}

void tst_QString::fill()
{
    QString e;
    e.fill('e',1);
    QCOMPARE(e,(QString)"e");
    QString f;
    f.fill('f',3);
    QCOMPARE(f,(QString)"fff");
    f.fill('F');
    QCOMPARE(f,(QString)"FFF");
}

void tst_QString::copy()
{
#ifdef QT3_SUPPORT
    QString e;
    e = "String E";
    QString ce = e.copy();
    QCOMPARE(ce,(QString)"String E");
    e = "XXX";
    QCOMPARE(ce,(QString)"String E");
    QCOMPARE(e,(QString)"XXX");
#endif
}

void tst_QString::sprintf()
{
    QString a;
    a.sprintf("COMPARE");
    QCOMPARE(a,(QString)"COMPARE");
    a.sprintf("%%%d",1);
    QCOMPARE(a,(QString)"%1");
    QCOMPARE(a.sprintf("X%dY",2),(QString)"X2Y");
    QCOMPARE(a.sprintf("X%9iY", 50000 ),(QString)"X    50000Y");
    QCOMPARE(a.sprintf("X%-9sY","hello"),(QString)"Xhello    Y");
    QCOMPARE(a.sprintf("X%-9iY", 50000 ),(QString)"X50000    Y");
    QCOMPARE(a.sprintf("%lf", 1.23), QString("1.230000"));
    QCOMPARE(a.sprintf("%lf", 1.23456789), QString("1.234568"));
    QCOMPARE(a.sprintf("%p", (void *)0xbfffd350), QString("0xbfffd350"));
    QCOMPARE(a.sprintf("%p", (void *)0), QString("0x0"));

    int i = 6;
    long l = -2;
    float f = 4.023f;
    QString S1;
    S1.sprintf("%d %ld %f",i,l,f);
    QCOMPARE(S1,QString("6 -2 4.023000"));

    double d = -514.25683;
    S1.sprintf("%f",d);
    QCOMPARE(S1, QString("-514.256830"));

    QCOMPARE(a.sprintf("%.3s", "Hello" ),(QString)"Hel");
    QCOMPARE(a.sprintf("%10.3s", "Hello" ),(QString)"       Hel");
    QCOMPARE(a.sprintf("%.10s", "Hello" ),(QString)"Hello");
    QCOMPARE(a.sprintf("%10.10s", "Hello" ),(QString)"     Hello");
    QCOMPARE(a.sprintf("%-10.10s", "Hello" ),(QString)"Hello     ");
    QCOMPARE(a.sprintf("%-10.3s", "Hello" ),(QString)"Hel       ");
    QCOMPARE(a.sprintf("%-5.5s", "Hello" ),(QString)"Hello");

    // Check utf8 conversion for %s
    QCOMPARE(a.sprintf("%s", "\303\266\303\244\303\274\303\226\303\204\303\234\303\270\303\246\303\245\303\230\303\206\303\205"), QString("\366\344\374\326\304\334\370\346\345\330\306\305"));

    // Check codecForCStrings is used to read non-modifier sequences in the format string
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    QCOMPARE(a.sprintf("\303\251\303\250\303\240 %s", "\303\251\303\250\303\240"), QString("\303\251\303\250\303\240 \303\251\303\250\303\240"));
    QTextCodec::setCodecForCStrings(0);

    int n1;
    a.sprintf("%s%n%s", "hello", &n1, "goodbye");
    QCOMPARE(n1, 5);
    QCOMPARE(a, QString("hellogoodbye"));
    qlonglong n2;
    a.sprintf("%s%s%lln%s", "foo", "bar", &n2, "whiz");
    QCOMPARE((int)n2, 6);
    QCOMPARE(a, QString("foobarwhiz"));
}

/*
    indexOf() and indexOf02() test QString::indexOf(),
    QString::lastIndexOf(), and their QByteArray equivalents.

    lastIndexOf() tests QString::lastIndexOf() more in depth, but it
    should probably be rewritten to use a data table.
*/

void tst_QString::indexOf_data()
{
    QTest::addColumn<QString>("haystack" );
    QTest::addColumn<QString>("needle" );
    QTest::addColumn<int>("startpos" );
    QTest::addColumn<bool>("bcs" );
    QTest::addColumn<int>("resultpos" );

    QTest::newRow( "data0" ) << QString("abc") << QString("a") << 0 << true << 0;
    QTest::newRow( "data1" ) << QString("abc") << QString("a") << 0 << false << 0;
    QTest::newRow( "data2" ) << QString("abc") << QString("A") << 0 << true << -1;
    QTest::newRow( "data3" ) << QString("abc") << QString("A") << 0 << false << 0;
    QTest::newRow( "data4" ) << QString("abc") << QString("a") << 1 << true << -1;
    QTest::newRow( "data5" ) << QString("abc") << QString("a") << 1 << false << -1;
    QTest::newRow( "data6" ) << QString("abc") << QString("A") << 1 << true << -1;
    QTest::newRow( "data7" ) << QString("abc") << QString("A") << 1 << false << -1;
    QTest::newRow( "data8" ) << QString("abc") << QString("b") << 0 << true << 1;
    QTest::newRow( "data9" ) << QString("abc") << QString("b") << 0 << false << 1;
    QTest::newRow( "data10" ) << QString("abc") << QString("B") << 0 << true << -1;
    QTest::newRow( "data11" ) << QString("abc") << QString("B") << 0 << false << 1;
    QTest::newRow( "data12" ) << QString("abc") << QString("b") << 1 << true << 1;
    QTest::newRow( "data13" ) << QString("abc") << QString("b") << 1 << false << 1;
    QTest::newRow( "data14" ) << QString("abc") << QString("B") << 1 << true << -1;
    QTest::newRow( "data15" ) << QString("abc") << QString("B") << 1 << false << 1;
    QTest::newRow( "data16" ) << QString("abc") << QString("b") << 2 << true << -1;
    QTest::newRow( "data17" ) << QString("abc") << QString("b") << 2 << false << -1;

    QTest::newRow( "data20" ) << QString("ABC") << QString("A") << 0 << true << 0;
    QTest::newRow( "data21" ) << QString("ABC") << QString("A") << 0 << false << 0;
    QTest::newRow( "data22" ) << QString("ABC") << QString("a") << 0 << true << -1;
    QTest::newRow( "data23" ) << QString("ABC") << QString("a") << 0 << false << 0;
    QTest::newRow( "data24" ) << QString("ABC") << QString("A") << 1 << true << -1;
    QTest::newRow( "data25" ) << QString("ABC") << QString("A") << 1 << false << -1;
    QTest::newRow( "data26" ) << QString("ABC") << QString("a") << 1 << true << -1;
    QTest::newRow( "data27" ) << QString("ABC") << QString("a") << 1 << false << -1;
    QTest::newRow( "data28" ) << QString("ABC") << QString("B") << 0 << true << 1;
    QTest::newRow( "data29" ) << QString("ABC") << QString("B") << 0 << false << 1;
    QTest::newRow( "data30" ) << QString("ABC") << QString("b") << 0 << true << -1;
    QTest::newRow( "data31" ) << QString("ABC") << QString("b") << 0 << false << 1;
    QTest::newRow( "data32" ) << QString("ABC") << QString("B") << 1 << true << 1;
    QTest::newRow( "data33" ) << QString("ABC") << QString("B") << 1 << false << 1;
    QTest::newRow( "data34" ) << QString("ABC") << QString("b") << 1 << true << -1;
    QTest::newRow( "data35" ) << QString("ABC") << QString("b") << 1 << false << 1;
    QTest::newRow( "data36" ) << QString("ABC") << QString("B") << 2 << true << -1;
    QTest::newRow( "data37" ) << QString("ABC") << QString("B") << 2 << false << -1;

    QTest::newRow( "data40" ) << QString("aBc") << QString("bc") << 0 << true << -1;
    QTest::newRow( "data41" ) << QString("aBc") << QString("Bc") << 0 << true << 1;
    QTest::newRow( "data42" ) << QString("aBc") << QString("bC") << 0 << true << -1;
    QTest::newRow( "data43" ) << QString("aBc") << QString("BC") << 0 << true << -1;
    QTest::newRow( "data44" ) << QString("aBc") << QString("bc") << 0 << false << 1;
    QTest::newRow( "data45" ) << QString("aBc") << QString("Bc") << 0 << false << 1;
    QTest::newRow( "data46" ) << QString("aBc") << QString("bC") << 0 << false << 1;
    QTest::newRow( "data47" ) << QString("aBc") << QString("BC") << 0 << false << 1;
    QTest::newRow( "data48" ) << QString("AbC") << QString("bc") << 0 << true << -1;
    QTest::newRow( "data49" ) << QString("AbC") << QString("Bc") << 0 << true << -1;
    QTest::newRow( "data50" ) << QString("AbC") << QString("bC") << 0 << true << 1;
    QTest::newRow( "data51" ) << QString("AbC") << QString("BC") << 0 << true << -1;
    QTest::newRow( "data52" ) << QString("AbC") << QString("bc") << 0 << false << 1;
    QTest::newRow( "data53" ) << QString("AbC") << QString("Bc") << 0 << false << 1;

    QTest::newRow( "data54" ) << QString("AbC") << QString("bC") << 0 << false << 1;
    QTest::newRow( "data55" ) << QString("AbC") << QString("BC") << 0 << false << 1;
    QTest::newRow( "data56" ) << QString("AbC") << QString("BC") << 1 << false << 1;
    QTest::newRow( "data57" ) << QString("AbC") << QString("BC") << 2 << false << -1;
#if 0
    QTest::newRow( "null-in-null") << QString() << QString() << 0 << false << 0;
    QTest::newRow( "empty-in-null") << QString() << QString("") << 0 << false << 0;
    QTest::newRow( "null-in-empty") << QString("") << QString() << 0 << false << 0;
    QTest::newRow( "empty-in-empty") << QString("") << QString("") << 0 << false << 0;
#endif


    QString s1 = "abc";
    s1 += QChar(0xb5);
    QString s2;
    s2 += QChar(0x3bc);
    QTest::newRow( "data58" ) << s1 << s2 << 0 << false << 3;
    s2.prepend("C");
    QTest::newRow( "data59" ) << s1 << s2 << 0 << false << 2;

    QString veryBigHaystack(500, 'a');
    veryBigHaystack += 'B';
    QTest::newRow("BoyerMooreStressTest") << veryBigHaystack << veryBigHaystack << 0 << true << 0;
    QTest::newRow("BoyerMooreStressTest2") << veryBigHaystack + 'c' << veryBigHaystack << 0 << true << 0;
    QTest::newRow("BoyerMooreStressTest3") << 'c' + veryBigHaystack << veryBigHaystack << 0 << true << 1;
    QTest::newRow("BoyerMooreStressTest4") << veryBigHaystack << veryBigHaystack + 'c' << 0 << true << -1;
    QTest::newRow("BoyerMooreStressTest5") << veryBigHaystack << 'c' + veryBigHaystack << 0 << true << -1;
    QTest::newRow("BoyerMooreStressTest6") << 'd' + veryBigHaystack << 'c' + veryBigHaystack << 0 << true << -1;
    QTest::newRow("BoyerMooreStressTest6") << veryBigHaystack + 'c' << 'c' + veryBigHaystack << 0 << true << -1;

    QTest::newRow("BoyerMooreInsensitiveStressTest") << veryBigHaystack << veryBigHaystack << 0 << false << 0;

}

void tst_QString::indexOf()
{
    QFETCH( QString, haystack );
    QFETCH( QString, needle );
    QFETCH( int, startpos );
    QFETCH( bool, bcs );
    QFETCH( int, resultpos );

    Qt::CaseSensitivity cs = bcs ? Qt::CaseSensitive : Qt::CaseInsensitive;

    bool needleIsLatin = (QString::fromLatin1(needle.toLatin1()) == needle);

    QCOMPARE( haystack.indexOf(needle, startpos, cs), resultpos );
    if (needleIsLatin) {
        QCOMPARE( haystack.indexOf(needle.toLatin1(), startpos, cs), resultpos );
        QCOMPARE( haystack.indexOf(needle.toLatin1().data(), startpos, cs), resultpos );
    }

    {
        QRegExp rx1 = QRegExp(QRegExp::escape(needle), cs);
        QRegExp rx2 = QRegExp(needle, cs, QRegExp::FixedString);
        QCOMPARE( haystack.indexOf(rx1, startpos), resultpos );
        QCOMPARE( haystack.indexOf(rx2, startpos), resultpos );
        // these QRegExp must have been modified
        QVERIFY( resultpos == -1 || rx1.matchedLength() > 0);
        QVERIFY( resultpos == -1 || rx2.matchedLength() > 0);
    }

    {
        const QRegExp rx1 = QRegExp(QRegExp::escape(needle), cs);
        const QRegExp rx2 = QRegExp(needle, cs, QRegExp::FixedString);
        QCOMPARE( haystack.indexOf(rx1, startpos), resultpos );
        QCOMPARE( haystack.indexOf(rx2, startpos), resultpos );
        // our QRegExp mustn't have been modified
        QCOMPARE( rx1.matchedLength(), -1 );
        QCOMPARE( rx2.matchedLength(), -1 );
    }

    if (cs == Qt::CaseSensitive) {
        QCOMPARE( haystack.indexOf(needle, startpos), resultpos );
        if (needleIsLatin) {
            QCOMPARE( haystack.indexOf(needle.toLatin1(), startpos), resultpos );
            QCOMPARE( haystack.indexOf(needle.toLatin1().data(), startpos), resultpos );
        }
        if (startpos == 0) {
            QCOMPARE( haystack.indexOf(needle), resultpos );
            if (needleIsLatin) {
                QCOMPARE( haystack.indexOf(needle.toLatin1()), resultpos );
                QCOMPARE( haystack.indexOf(needle.toLatin1().data()), resultpos );
            }
        }
    }
    if (needle.size() == 1) {
        QCOMPARE(haystack.indexOf(needle.at(0), startpos, cs), resultpos);
    }

}

void tst_QString::indexOf2_data()
{
    QTest::addColumn<QString>("haystack" );
    QTest::addColumn<QString>("needle" );
    QTest::addColumn<int>("resultpos" );

    QTest::newRow( "data0" ) << QString() << QString() << 0;
    QTest::newRow( "data1" ) << QString() << QString("") << 0;
    QTest::newRow( "data2" ) << QString("") << QString() << 0;
    QTest::newRow( "data3" ) << QString("") << QString("") << 0;
    QTest::newRow( "data4" ) << QString() << QString("a") << -1;
    QTest::newRow( "data5" ) << QString() << QString("abcdefg") << -1;
    QTest::newRow( "data6" ) << QString("") << QString("a") << -1;
    QTest::newRow( "data7" ) << QString("") << QString("abcdefg") << -1;

    QTest::newRow( "data8" ) << QString("a") << QString() << 0;
    QTest::newRow( "data9" ) << QString("a") << QString("") << 0;
    QTest::newRow( "data10" ) << QString("a") << QString("a") << 0;
    QTest::newRow( "data11" ) << QString("a") << QString("b") << -1;
    QTest::newRow( "data12" ) << QString("a") << QString("abcdefg") << -1;
    QTest::newRow( "data13" ) << QString("ab") << QString() << 0;
    QTest::newRow( "data14" ) << QString("ab") << QString("") << 0;
    QTest::newRow( "data15" ) << QString("ab") << QString("a") << 0;
    QTest::newRow( "data16" ) << QString("ab") << QString("b") << 1;
    QTest::newRow( "data17" ) << QString("ab") << QString("ab") << 0;
    QTest::newRow( "data18" ) << QString("ab") << QString("bc") << -1;
    QTest::newRow( "data19" ) << QString("ab") << QString("abcdefg") << -1;

    QTest::newRow( "data30" ) << QString("abc") << QString("a") << 0;
    QTest::newRow( "data31" ) << QString("abc") << QString("b") << 1;
    QTest::newRow( "data32" ) << QString("abc") << QString("c") << 2;
    QTest::newRow( "data33" ) << QString("abc") << QString("d") << -1;
    QTest::newRow( "data34" ) << QString("abc") << QString("ab") << 0;
    QTest::newRow( "data35" ) << QString("abc") << QString("bc") << 1;
    QTest::newRow( "data36" ) << QString("abc") << QString("cd") << -1;
    QTest::newRow( "data37" ) << QString("abc") << QString("ac") << -1;

    // sizeof(whale) > 32
    QString whale = "a5zby6cx7dw8evf9ug0th1si2rj3qkp4lomn";
    QString minnow = "zby";
    QTest::newRow( "data40" ) << whale << minnow << 2;
    QTest::newRow( "data41" ) << (whale + whale) << minnow << 2;
    QTest::newRow( "data42" ) << (minnow + whale) << minnow << 0;
    QTest::newRow( "data43" ) << whale << whale << 0;
    QTest::newRow( "data44" ) << (whale + whale) << whale << 0;
    QTest::newRow( "data45" ) << whale << (whale + whale) << -1;
    QTest::newRow( "data46" ) << (whale + whale) << (whale + whale) << 0;
    QTest::newRow( "data47" ) << (whale + whale) << (whale + minnow) << -1;
    QTest::newRow( "data48" ) << (minnow + whale) << whale << (int)minnow.length();
}

void tst_QString::indexOf2()
{
    QFETCH( QString, haystack );
    QFETCH( QString, needle );
    QFETCH( int, resultpos );

    QByteArray chaystack = haystack.toLatin1();
    QByteArray cneedle = needle.toLatin1();
    int got;

    QCOMPARE( haystack.indexOf(needle, 0, Qt::CaseSensitive), resultpos );
    QCOMPARE( QStringMatcher(needle, Qt::CaseSensitive).indexIn(haystack, 0), resultpos );
    QCOMPARE( haystack.indexOf(needle, 0, Qt::CaseInsensitive), resultpos );
    QCOMPARE( QStringMatcher(needle, Qt::CaseInsensitive).indexIn(haystack, 0), resultpos );
    if ( needle.length() > 0 ) {
	got = haystack.lastIndexOf( needle, -1, Qt::CaseSensitive );
	QVERIFY( got == resultpos || (resultpos >= 0 && got >= resultpos) );
	got = haystack.lastIndexOf( needle, -1, Qt::CaseInsensitive );
	QVERIFY( got == resultpos || (resultpos >= 0 && got >= resultpos) );
    }

    QCOMPARE( chaystack.indexOf(cneedle, 0), resultpos );
    QCOMPARE( QByteArrayMatcher(cneedle).indexIn(chaystack, 0), resultpos );
    if ( cneedle.length() > 0 ) {
	got = chaystack.lastIndexOf(cneedle, -1);
	QVERIFY( got == resultpos || (resultpos >= 0 && got >= resultpos) );
    }
}

void tst_QString::lastIndexOf_data()
{
    QTest::addColumn<QString>("haystack" );
    QTest::addColumn<QString>("needle" );
    QTest::addColumn<int>("from" );
    QTest::addColumn<int>("expected" );
    QTest::addColumn<bool>("caseSensitive" );

    QString a = "ABCDEFGHIEfGEFG";

    QTest::newRow("-1") << a << "G" << a.size() - 1 << 14 << true;
    QTest::newRow("1") << a << "G" << - 1 << 14 << true;
    QTest::newRow("2") << a << "G" << -3 << 11 << true;
    QTest::newRow("3") << a << "G" << -5 << 6 << true;
    QTest::newRow("4") << a << "G" << 14 << 14 << true;
    QTest::newRow("5") << a << "G" << 13 << 11 << true;
    QTest::newRow("6") << a << "B" << a.size() - 1 << 1 << true;
    QTest::newRow("6") << a << "B" << - 1 << 1 << true;
    QTest::newRow("7") << a << "B" << 1 << 1 << true;
    QTest::newRow("8") << a << "B" << 0 << -1 << true;

    QTest::newRow("9") << a << "G" <<  -1 <<  a.size()-1 << true;
    QTest::newRow("10") << a << "G" <<  a.size()-1 <<  a.size()-1 << true;
    QTest::newRow("11") << a << "G" <<  a.size() <<  -1 << true;
    QTest::newRow("12") << a << "A" <<  0 <<  0 << true;
    QTest::newRow("13") << a << "A" <<  -1*a.size() <<  0 << true;

    QTest::newRow("15") << a << "efg" << 0 << -1 << false;
    QTest::newRow("16") << a << "efg" << a.size() << -1 << false;
    QTest::newRow("17") << a << "efg" << -1 * a.size() << -1 << false;
    QTest::newRow("19") << a << "efg" << a.size() - 1 << 12 << false;
    QTest::newRow("20") << a << "efg" << 12 << 12 << false;
    QTest::newRow("21") << a << "efg" << -12 << -1 << false;
    QTest::newRow("22") << a << "efg" << 11 << 9 << false;

    QTest::newRow("24") << "" << "asdf" << -1 << -1 << false;
    QTest::newRow("25") << "asd" << "asdf" << -1 << -1 << false;
    QTest::newRow("26") << "" << QString() << -1 << -1 << false;

    QTest::newRow("27") << a << "" << a.size() << a.size() << false;
    QTest::newRow("28") << a << "" << a.size() + 10 << -1 << false;
}

void tst_QString::lastIndexOf()
{
    QFETCH(QString, haystack);
    QFETCH(QString, needle);
    QFETCH(int, from);
    QFETCH(int, expected);
    QFETCH(bool, caseSensitive);

    Qt::CaseSensitivity cs = (caseSensitive ? Qt::CaseSensitive : Qt::CaseInsensitive);

    QCOMPARE(haystack.lastIndexOf(needle, from, cs), expected);
    QCOMPARE(haystack.lastIndexOf(needle.toLatin1(), from, cs), expected);
    QCOMPARE(haystack.lastIndexOf(needle.toLatin1().data(), from, cs), expected);

    if (from >= -1 && from < haystack.size()) {
        // unfortunately, QString and QRegExp don't have the same out of bound semantics
        // I think QString is wrong -- See file log for contact information.
        {
            QRegExp rx1 = QRegExp(QRegExp::escape(needle), cs);
            QRegExp rx2 = QRegExp(needle, cs, QRegExp::FixedString);
            QCOMPARE(haystack.lastIndexOf(rx1, from), expected);
            QCOMPARE(haystack.lastIndexOf(rx2, from), expected);
            // our QRegExp mustn't have been modified
            QVERIFY(expected == -1 || rx1.matchedLength() > 0);
            QVERIFY(expected == -1 || rx2.matchedLength() > 0);
        }

        {
            const QRegExp rx1 = QRegExp(QRegExp::escape(needle), cs);
            const QRegExp rx2 = QRegExp(needle, cs, QRegExp::FixedString);
            QCOMPARE(haystack.lastIndexOf(rx1, from), expected);
            QCOMPARE(haystack.lastIndexOf(rx2, from), expected);
            // our QRegExp mustn't have been modified
            QCOMPARE(rx1.matchedLength(), -1);
            QCOMPARE(rx2.matchedLength(), -1);
        }
    }

    if (cs == Qt::CaseSensitive) {
        QCOMPARE(haystack.lastIndexOf(needle, from), expected);
        QCOMPARE(haystack.lastIndexOf(needle.toLatin1(), from), expected);
        QCOMPARE(haystack.lastIndexOf(needle.toLatin1().data(), from), expected);
        if (from == -1) {
            QCOMPARE(haystack.lastIndexOf(needle), expected);
            QCOMPARE(haystack.lastIndexOf(needle.toLatin1()), expected);
            QCOMPARE(haystack.lastIndexOf(needle.toLatin1().data()), expected);
        }
    }
    if (needle.size() == 1) {
        QCOMPARE(haystack.lastIndexOf(needle.at(0), from), expected);
    }
}

void tst_QString::contains()
{
    QString a;
    a="ABCDEFGHIEfGEFG"; // 15 chars
    QCOMPARE(a.count('A'),1);
    QCOMPARE(a.count('Z'),0);
    QCOMPARE(a.count('E'),3);
    QCOMPARE(a.count('F'),2);
    QCOMPARE(a.count('F',Qt::CaseInsensitive),3);
    QCOMPARE(a.count("FG"),2);
    QCOMPARE(a.count("FG",Qt::CaseInsensitive),3);
    QCOMPARE(a.count( QString(), Qt::CaseInsensitive), 16);
    QCOMPARE(a.count( "", Qt::CaseInsensitive), 16);
    QCOMPARE(a.count(QRegExp("[FG][HI]")),1);
    QCOMPARE(a.count(QRegExp("[G][HE]")),2);
}

void tst_QString::left()
{
    QString a;
    a="ABCDEFGHIEfGEFG"; // 15 chars
    QCOMPARE(a.left(3),(QString)"ABC");
    QVERIFY(!a.left(0).isNull());
    QCOMPARE(a.left(0),(QString)"");

    QString n;
    QVERIFY(n.left(3).isNull());
    QVERIFY(n.left(0).isNull());
    QVERIFY(n.left(0).isNull());

    QString l = "Left";
    QCOMPARE(l.left(-1), l);
    QCOMPARE(l.left(100), l);
}

void tst_QString::leftRef()
{
    QString a;
    a="ABCDEFGHIEfGEFG"; // 15 chars
    QCOMPARE(a.leftRef(3).toString(),(QString)"ABC");

    QVERIFY(a.leftRef(0).toString().isEmpty());
    QCOMPARE(a.leftRef(0).toString(),(QString)"");

    QString n;
    QVERIFY(n.leftRef(3).toString().isEmpty());
    QVERIFY(n.leftRef(0).toString().isEmpty());
    QVERIFY(n.leftRef(0).toString().isEmpty());

    QString l = "Left";
    QCOMPARE(l.leftRef(-1).toString(), l);
    QCOMPARE(l.leftRef(100).toString(), l);
}

void tst_QString::right()
{
    QString a;
    a="ABCDEFGHIEfGEFG"; // 15 chars
    QCOMPARE(a.right(3),(QString)"EFG");
    QCOMPARE(a.right(0),(QString)"");

    QString n;
    QVERIFY(n.right(3).isNull());
    QVERIFY(n.right(0).isNull());

    QString r = "Right";
    QCOMPARE(r.right(-1), r);
    QCOMPARE(r.right(100), r);
}

void tst_QString::rightRef()
{
    QString a;
    a="ABCDEFGHIEfGEFG"; // 15 chars
    QCOMPARE(a.rightRef(3).toString(),(QString)"EFG");
    QCOMPARE(a.rightRef(0).toString(),(QString)"");

    QString n;
    QVERIFY(n.rightRef(3).toString().isEmpty());
    QVERIFY(n.rightRef(0).toString().isEmpty());

    QString r = "Right";
    QCOMPARE(r.rightRef(-1).toString(), r);
    QCOMPARE(r.rightRef(100).toString(), r);
}

void tst_QString::mid()
{
    QString a;
    a="ABCDEFGHIEfGEFG"; // 15 chars

    QCOMPARE(a.mid(3,3),(QString)"DEF");
    QCOMPARE(a.mid(0,0),(QString)"");
    QVERIFY(a.mid(9999).isNull());
    QVERIFY(a.mid(9999,1).isNull());

    QString n;
    QVERIFY(n.mid(3,3).isNull());
    QVERIFY(n.mid(0,0).isNull());
    QVERIFY(n.mid(9999,0).isNull());
    QVERIFY(n.mid(9999,1).isNull());

    QString x = "Nine pineapples";
    QCOMPARE(x.mid(5, 4), QString("pine"));
    QCOMPARE(x.mid(5), QString("pineapples"));

}

void tst_QString::midRef()
{
    QString a;
    a="ABCDEFGHIEfGEFG"; // 15 chars

    QCOMPARE(a.midRef(3,3).toString(),(QString)"DEF");
    QCOMPARE(a.midRef(0,0).toString(),(QString)"");
    QVERIFY(a.midRef(9999).toString().isEmpty());
    QVERIFY(a.midRef(9999,1).toString().isEmpty());

    QString n;
    QVERIFY(n.midRef(3,3).toString().isEmpty());
    QVERIFY(n.midRef(0,0).toString().isEmpty());
    QVERIFY(n.midRef(9999,0).toString().isEmpty());
    QVERIFY(n.midRef(9999,1).toString().isEmpty());

    QString x = "Nine pineapples";
    QCOMPARE(x.midRef(5, 4).toString(), QString("pine"));
    QCOMPARE(x.midRef(5).toString(), QString("pineapples"));

}

void tst_QString::stringRef()
{
    QString a;
    a="ABCDEFGHIEfGEFG"; // 15 chars

    QVERIFY(QStringRef(&a, 0, 0) == (QString)"");

    QVERIFY(QStringRef(&a, 3, 3) == (QString)"DEF");
    QVERIFY(QStringRef(&a, 3, 3) == QLatin1String("DEF"));
    QVERIFY(QStringRef(&a, 3, 3) == "DEF");
    QVERIFY((QString)"DEF" == QStringRef(&a, 3, 3));
    QVERIFY(QLatin1String("DEF") == QStringRef(&a, 3, 3));
    QVERIFY("DEF" == QStringRef(&a, 3, 3));

    QVERIFY(QStringRef(&a, 3, 3) != (QString)"DE");
    QVERIFY(QStringRef(&a, 3, 3) != QLatin1String("DE"));
    QVERIFY(QStringRef(&a, 3, 3) != "DE");
    QVERIFY((QString)"DE" != QStringRef(&a, 3, 3));
    QVERIFY(QLatin1String("DE") != QStringRef(&a, 3, 3));
    QVERIFY("DE" != QStringRef(&a, 3, 3));

    QString s_alpha("alpha");
    QString s_beta("beta");
    QStringRef alpha(&s_alpha);
    QStringRef beta(&s_beta);

    QVERIFY(alpha < beta);
    QVERIFY(alpha <= beta);
    QVERIFY(alpha <= alpha);
    QVERIFY(beta > alpha);
    QVERIFY(beta >= alpha);
    QVERIFY(beta >= beta);

    QString s_alpha2("alpha");

    QMap<QStringRef, QString> map;
    map.insert(alpha, "alpha");
    map.insert(beta, "beta");
    QVERIFY(alpha == map.value(QStringRef(&s_alpha2)));

    QHash<QStringRef, QString> hash;
    hash.insert(alpha, "alpha");
    hash.insert(beta, "beta");

    QVERIFY(alpha == hash.value(QStringRef(&s_alpha2)));
}

void tst_QString::leftJustified()
{
    QString a;
    a="ABC";
    QCOMPARE(a.leftJustified(5,'-'),(QString)"ABC--");
    QCOMPARE(a.leftJustified(4,'-'),(QString)"ABC-");
    QCOMPARE(a.leftJustified(4),(QString)"ABC ");
    QCOMPARE(a.leftJustified(3),(QString)"ABC");
    QCOMPARE(a.leftJustified(2),(QString)"ABC");
    QCOMPARE(a.leftJustified(1),(QString)"ABC");
    QCOMPARE(a.leftJustified(0),(QString)"ABC");

    QString n;
    QVERIFY(!n.leftJustified(3).isNull());
    QCOMPARE(a.leftJustified(4,' ',true),(QString)"ABC ");
    QCOMPARE(a.leftJustified(3,' ',true),(QString)"ABC");
    QCOMPARE(a.leftJustified(2,' ',true),(QString)"AB");
    QCOMPARE(a.leftJustified(1,' ',true),(QString)"A");
    QCOMPARE(a.leftJustified(0,' ',true),(QString)"");
}

void tst_QString::rightJustified()
{
    QString a;
    a="ABC";
    QCOMPARE(a.rightJustified(5,'-'),(QString)"--ABC");
    QCOMPARE(a.rightJustified(4,'-'),(QString)"-ABC");
    QCOMPARE(a.rightJustified(4),(QString)" ABC");
    QCOMPARE(a.rightJustified(3),(QString)"ABC");
    QCOMPARE(a.rightJustified(2),(QString)"ABC");
    QCOMPARE(a.rightJustified(1),(QString)"ABC");
    QCOMPARE(a.rightJustified(0),(QString)"ABC");

    QString n;
    QVERIFY(!n.rightJustified(3).isNull());
    QCOMPARE(a.rightJustified(4,'-',true),(QString)"-ABC");
    QCOMPARE(a.rightJustified(4,' ',true),(QString)" ABC");
    QCOMPARE(a.rightJustified(3,' ',true),(QString)"ABC");
    QCOMPARE(a.rightJustified(2,' ',true),(QString)"AB");
    QCOMPARE(a.rightJustified(1,' ',true),(QString)"A");
    QCOMPARE(a.rightJustified(0,' ',true),(QString)"");
    QCOMPARE(a,(QString)"ABC");
}

void tst_QString::toUpper()
{
    QCOMPARE( QString().toUpper(), QString() );
    QCOMPARE( QString("").toUpper(), QString("") );
    QCOMPARE( QString("text").toUpper(), QString("TEXT") );
    QCOMPARE( QString("Text").toUpper(), QString("TEXT") );
    QCOMPARE( QString("tExt").toUpper(), QString("TEXT") );
    QCOMPARE( QString("teXt").toUpper(), QString("TEXT") );
    QCOMPARE( QString("texT").toUpper(), QString("TEXT") );
    QCOMPARE( QString("TExt").toUpper(), QString("TEXT") );
    QCOMPARE( QString("teXT").toUpper(), QString("TEXT") );
    QCOMPARE( QString("tEXt").toUpper(), QString("TEXT") );
    QCOMPARE( QString("tExT").toUpper(), QString("TEXT") );
    QCOMPARE( QString("TEXT").toUpper(), QString("TEXT") );
    QCOMPARE( QString("@ABYZ[").toUpper(), QString("@ABYZ["));
    QCOMPARE( QString("@abyz[").toUpper(), QString("@ABYZ["));
    QCOMPARE( QString("`ABYZ{").toUpper(), QString("`ABYZ{"));
    QCOMPARE( QString("`abyz{").toUpper(), QString("`ABYZ{"));

    QCOMPARE( QString(1, QChar(0xdf)).toUpper(), QString("SS"));

    QString lower;
    lower += QChar(QChar::highSurrogate(0x10428));
    lower += QChar(QChar::lowSurrogate(0x10428));
    QString upper;
    upper += QChar(QChar::highSurrogate(0x10400));
    upper += QChar(QChar::lowSurrogate(0x10400));
    QCOMPARE( lower.toUpper(), upper);


    for (int i = 0; i < 65536; ++i) {
        QString str(1, QChar(i));
        QString upper = str.toUpper();
        QVERIFY(upper.length() >= 1);
        if (upper.length() == 1)
            QVERIFY(upper == QString(1, QChar(i).toUpper()));
    }
}

void tst_QString::toLower()
{
    QCOMPARE( QString().toLower(), QString() );
    QCOMPARE( QString("").toLower(), QString("") );
    QCOMPARE( QString("text").toLower(), QString("text") );
    QCOMPARE( QString("Text").toLower(), QString("text") );
    QCOMPARE( QString("tExt").toLower(), QString("text") );
    QCOMPARE( QString("teXt").toLower(), QString("text") );
    QCOMPARE( QString("texT").toLower(), QString("text") );
    QCOMPARE( QString("TExt").toLower(), QString("text") );
    QCOMPARE( QString("teXT").toLower(), QString("text") );
    QCOMPARE( QString("tEXt").toLower(), QString("text") );
    QCOMPARE( QString("tExT").toLower(), QString("text") );
    QCOMPARE( QString("TEXT").toLower(), QString("text") );
    QCOMPARE( QString("@ABYZ[").toLower(), QString("@abyz["));
    QCOMPARE( QString("@abyz[").toLower(), QString("@abyz["));
    QCOMPARE( QString("`ABYZ{").toLower(), QString("`abyz{"));
    QCOMPARE( QString("`abyz{").toLower(), QString("`abyz{"));

    QCOMPARE( QString(1, QChar(0x130)).toLower(), QString(1, QChar(0x69)) + QChar(0x307));

    QString lower;
    lower += QChar(QChar::highSurrogate(0x10428));
    lower += QChar(QChar::lowSurrogate(0x10428));
    QString upper;
    upper += QChar(QChar::highSurrogate(0x10400));
    upper += QChar(QChar::lowSurrogate(0x10400));
    QCOMPARE( upper.toLower(), lower);

    for (int i = 0; i < 65536; ++i) {
        QString str(1, QChar(i));
        QString lower = str.toLower();
        QVERIFY(lower.length() >= 1);
        if (lower.length() == 1)
            QVERIFY(str.toLower() == QString(1, QChar(i).toLower()));
    }
}

void tst_QString::trimmed()
{
    QString a;
    a="Text";
    QCOMPARE(a,(QString)"Text");
    QCOMPARE(a.trimmed(),(QString)"Text");
    QCOMPARE(a,(QString)"Text");
    a=" ";
    QCOMPARE(a.trimmed(),(QString)"");
    QCOMPARE(a,(QString)" ");
    a=" a   ";
    QCOMPARE(a.trimmed(),(QString)"a");
}

void tst_QString::simplified_data()
{
    QTest::addColumn<QString>("full" );
    QTest::addColumn<QString>("simple" );

    QTest::newRow("null") << QString() << QString();
    QTest::newRow("empty") << "" << "";
    QTest::newRow("one char") << "a" << "a";
    QTest::newRow("one word") << "foo" << "foo";
    QTest::newRow("chars trivial") << "a b" << "a b";
    QTest::newRow("words trivial") << "foo bar" << "foo bar";
    QTest::newRow("allspace") << "  \t\v " << "";
    QTest::newRow("char trailing") << "a " << "a";
    QTest::newRow("char trailing tab") << "a\t" << "a";
    QTest::newRow("char multitrailing") << "a   " << "a";
    QTest::newRow("char multitrailing tab") << "a   \t" << "a";
    QTest::newRow("char leading") << " a" << "a";
    QTest::newRow("char leading tab") << "\ta" << "a";
    QTest::newRow("char multileading") << "   a" << "a";
    QTest::newRow("char multileading tab") << "\t   a" << "a";
    QTest::newRow("chars apart") << "a  b" << "a b";
    QTest::newRow("words apart") << "foo  bar" << "foo bar";
    QTest::newRow("enclosed word") << "   foo \t " << "foo";
    QTest::newRow("enclosed chars apart") << " a   b " << "a b";
    QTest::newRow("enclosed words apart") << " foo   bar " << "foo bar";
    QTest::newRow("chars apart posttab") << "a \tb" << "a b";
    QTest::newRow("chars apart pretab") << "a\t b" << "a b";
    QTest::newRow("many words") << "  just some    random\ttext here" << "just some random text here";
}

void tst_QString::simplified()
{
    QFETCH(QString, full);
    QFETCH(QString, simple);

    QString result = full.simplified();
    if (simple.isNull()) {
        QVERIFY2(result.isNull(), qPrintable("'" + full + "' did not yield null: " + result));
    } else if (simple.isEmpty()) {
        QVERIFY2(result.isEmpty() && !result.isNull(), qPrintable("'" + full + "' did not yield empty: " + result));
    } else {
        QCOMPARE(result, simple);
        if (full == simple)
            QVERIFY(result.isSharedWith(full));
    }
}

void tst_QString::insert()
{
    QString a;

    a = "Ys";
    QCOMPARE(a.insert(1,'e'), QString("Yes"));
    QCOMPARE(a.insert(3,'!'), QString("Yes!"));
    QCOMPARE(a.insert(5,'?'), QString("Yes! ?"));

    a = "ABC";
    QCOMPARE(a.insert(5,"DEF"), QString("ABC  DEF"));

    a = "ABC";
    QCOMPARE(a.insert(2, QString()), QString("ABC"));
    QCOMPARE(a.insert(0,"ABC"), QString("ABCABC"));
    QCOMPARE(a, QString("ABCABC"));
    QCOMPARE(a.insert(0,a), QString("ABCABCABCABC"));

    QCOMPARE(a, QString("ABCABCABCABC"));
    QCOMPARE(a.insert(0,'<'), QString("<ABCABCABCABC"));
    QCOMPARE(a.insert(1,'>'), QString("<>ABCABCABCABC"));

    a = "Meal";
    QCOMPARE(a.insert(1, QLatin1String("ontr")), QString("Montreal"));
    QCOMPARE(a.insert(4, ""), QString("Montreal"));
    QCOMPARE(a.insert(3, QLatin1String("")), QString("Montreal"));
    QCOMPARE(a.insert(3, QLatin1String(0)), QString("Montreal"));
    QCOMPARE(a.insert(3, static_cast<const char *>(0)), QString("Montreal"));
    QCOMPARE(a.insert(0, QLatin1String("a")), QString("aMontreal"));
}

void tst_QString::append()
{
    QString a;
    a = "<>ABCABCABCABC";
    QCOMPARE(a.append(">"),(QString)"<>ABCABCABCABC>");
#if 0
#if !defined(QT_NO_STL)
    /*
      The std::string support is fairly trivial in itself. The major
      problem is whether it compiles or not, because of the way
      different compilers choose an overload based on the type of the
      argument. If it compiles, we're happy.
    */
    std::string stroustrup;
    a.append( stroustrup );
    a += stroustrup;
#endif
#endif
}

void tst_QString::append_bytearray_data()
{
    QTest::addColumn<QString>("str" );
    QTest::addColumn<QByteArray>("ba" );
    QTest::addColumn<QString>("res" );

    QByteArray ba( 5, 0 );
    ba[0] = 'a';
    ba[1] = 'b';
    ba[2] = 'c';
    ba[3] = 'd';
#if 0
    ba[4] = 0;

    for ( int i=0; i<2; i++ ) {
	// i == 0: the byte array is exactly strlen+1
	// i == 1: the byte array is larger than strlen+1
	QTest::newRow( QString("nullTerminated_%1_0").arg(i) ) << QString() << ba << QString("abcd");
	QTest::newRow( QString("nullTerminated_%1_1").arg(i) ) << QString("") << ba << QString("abcd");
	QTest::newRow( QString("nullTerminated_%1_2").arg(i) ) << QString("foobar ") << ba << QString("foobar abcd");

	ba.resize( 8 );
    }
#endif
    // no 0 termination
    ba.resize( 4 );
    QTest::newRow( "notTerminated_0" ) << QString() << ba << QString("abcd");
    QTest::newRow( "notTerminated_1" ) << QString("") << ba << QString("abcd");
    QTest::newRow( "notTerminated_2" ) << QString("foobar ") << ba << QString("foobar abcd");

    // byte array with only a 0
    ba.resize( 1 );
    ba[0] = 0;
    QByteArray ba2("foobar ");
    ba2.append('\0');
    QTest::newRow( "emptyString" ) << QString("foobar ") << ba << QString(ba2);

    // empty byte array
    ba.resize( 0 );
    QTest::newRow( "emptyByteArray" ) << QString("foobar ") << ba << QString("foobar ");
}

void tst_QString::append_bytearray()
{
    {
	QFETCH( QString, str );
	QFETCH( QByteArray, ba );

	str.append( ba );

	QTEST( str, "res" );
    }
    {
	QFETCH( QString, str );
	QFETCH( QByteArray, ba );

	QTextCodec::setCodecForCStrings( QTextCodec::codecForMib(4) ); // Latin 1
	str.append( ba );
	QTextCodec::setCodecForCStrings( 0 );

	QTEST( str, "res" );
    }
}

void tst_QString::operator_pluseq_bytearray_data()
{
    append_bytearray_data();
}

void tst_QString::operator_pluseq_bytearray()
{
    {
	QFETCH( QString, str );
	QFETCH( QByteArray, ba );

	str += ba;

	QTEST( str, "res" );
    }
    {
	QFETCH( QString, str );
	QFETCH( QByteArray, ba );

	QTextCodec::setCodecForCStrings( QTextCodec::codecForMib(4) ); // Latin 1
	str += ba;
	QTextCodec::setCodecForCStrings( 0 );

	QTEST( str, "res" );
    }
}

void tst_QString::prepend()
{
    QString a;
    a = "<>ABCABCABCABC>";
    QCOMPARE(a.prepend("-"),(QString)"-<>ABCABCABCABC>");
#if 0
#if !defined(QT_NO_STL)
    std::string stroustrup;
    a.prepend( stroustrup );
#endif
#endif
}

void tst_QString::prepend_bytearray_data()
{
    QTest::addColumn<QString>("str" );
    QTest::addColumn<QByteArray>("ba" );
    QTest::addColumn<QString>("res" );

    QByteArray ba( 5, 0 );
    ba[0] = 'a';
    ba[1] = 'b';
    ba[2] = 'c';
    ba[3] = 'd';
#if 0
    ba[4] = 0;

    for ( int i=0; i<2; i++ ) {
	// i == 0: the byte array is exactly strlen+1
	// i == 1: the byte array is larger than strlen+1
	QTest::newRow( QString("nullTerminated_%1_0").arg(i) ) << QString() << ba << QString("abcd");
	QTest::newRow( QString("nullTerminated_%1_1").arg(i) ) << QString("") << ba << QString("abcd");
	QTest::newRow( QString("nullTerminated_%1_2").arg(i) ) << QString(" foobar") << ba << QString("abcd foobar");

	ba.resize( 8 );
    }
    // no 0 termination
    ba.resize( 4 );
    QTest::newRow( "notTerminated_0" ) << QString() << ba << QString("abcd");
    QTest::newRow( "notTerminated_1" ) << QString("") << ba << QString("abcd");
    QTest::newRow( "notTerminated_2" ) << QString(" foobar") << ba << QString("abcd foobar");
#endif

    // byte array with only a 0
    ba.resize( 1 );
    ba[0] = 0;
    QTest::newRow( "emptyString" ) << QString("foobar ") << ba << QString("foobar ");

    // empty byte array
    ba.resize( 0 );
    QTest::newRow( "emptyByteArray" ) << QString(" foobar") << ba << QString(" foobar");
}

void tst_QString::prepend_bytearray()
{
    {
	QFETCH( QString, str );
	QFETCH( QByteArray, ba );

    str.prepend( ba );

	QFETCH( QString, res );
	QCOMPARE( str, res );
    }
    {
	QFETCH( QString, str );
	QFETCH( QByteArray, ba );

	QTextCodec::setCodecForCStrings( QTextCodec::codecForMib(4) ); // Latin 1
	str.prepend( ba );
	QTextCodec::setCodecForCStrings( 0 );

	QTEST( str, "res" );
    }
}

void tst_QString::replace_uint_uint()
{
    QFETCH( QString, string );
    QFETCH( int, index );
    QFETCH( int, len );
    QFETCH( QString, after );

    QString s1 = string;
    s1.replace( (uint) index, (int) len, after );
    QTEST( s1, "result" );

    QString s2 = string;
    s2.replace( (uint) index, (uint) len, after.unicode(), after.length() );
    QTEST( s2, "result" );

    if ( after.length() == 1 ) {
	QString s3 = string;
	s3.replace( (uint) index, (uint) len, QChar(after[0]) );
	QTEST( s3, "result" );

	QString s4 = string;
	s4.replace( (uint) index, (uint) len, QChar(after[0]).toLatin1() );
	QTEST( s4, "result" );
    }
}

void tst_QString::replace_uint_uint_extra()
{
    /*
        This test is designed to be extremely slow if QString::replace() doesn't optimize the case
        len == after.size().
    */
    QString str("dsfkljfdsjklsdjsfjklfsdjkldfjslkjsdfkllkjdsfjklsfdkjsdflkjlsdfjklsdfkjldsflkjsddlkj");
    for (int j = 1; j < 12; ++j)
        str += str;

    QString str2("aaaaaaaaaaaaaaaaaaaa");
    for (int i = 0; i < 2000000; ++i) {
        str.replace(10, 20, str2);
    }

    /*
        Make sure that replacing with itself works.
    */
    QString copy(str);
    copy.detach();
    str.replace(0, str.length(), str);
    QVERIFY(copy == str);

    /*
        Make sure that replacing a part of oneself with itself works.
    */
    QString str3("abcdefghij");
    str3.replace(0, 1, str3);
    QCOMPARE(str3, QString("abcdefghijbcdefghij"));

    QString str4("abcdefghij");
    str4.replace(1, 3, str4);
    QCOMPARE(str4, QString("aabcdefghijefghij"));

    QString str5("abcdefghij");
    str5.replace(8, 10, str5);
    QCOMPARE(str5, QString("abcdefghabcdefghij"));
}

void tst_QString::replace_string()
{
    QFETCH( QString, string );
    QFETCH( QString, before );
    QFETCH( QString, after );
    QFETCH( bool, bcs );

    Qt::CaseSensitivity cs = bcs ? Qt::CaseSensitive : Qt::CaseInsensitive;

    if ( before.length() == 1 ) {
	QChar ch = before.at( 0 );

	QString s1 = string;
	s1.replace( ch, after, cs );
	QTEST( s1, "result" );

	if ( QChar(ch.toLatin1()) == ch ) {
	    QString s2 = string;
	    s2.replace( ch.toLatin1(), after, cs );
	    QTEST( s2, "result" );
	}
    }

    QString s3 = string;
    s3.replace( before, after, cs );
    QTEST( s3, "result" );

    QString s4 = string;
    s4.replace( QRegExp(QRegExp::escape(before), cs), after );
    QTEST( s4, "result" );

    QString s5 = string;
    s5.replace(QRegExp(before, cs, QRegExp::FixedString), after);
    QTEST( s5, "result" );
}

void tst_QString::replace_regexp()
{
    QFETCH( QString, string );
    QFETCH( QString, regexp );
    QFETCH( QString, after );

    QString s2 = string;
    s2.replace( QRegExp(regexp), after );
    QTEST( s2, "result" );
}

void tst_QString::remove_uint_uint()
{
    QFETCH( QString, string );
    QFETCH( int, index );
    QFETCH( int, len );
    QFETCH( QString, after );

    if ( after.length() == 0 ) {
	QString s1 = string;
	s1.remove( (uint) index, (uint) len );
	QTEST( s1, "result" );
    } else
	QCOMPARE( 0, 0 ); // shut QtTest
}

void tst_QString::remove_string()
{
    QFETCH( QString, string );
    QFETCH( QString, before );
    QFETCH( QString, after );
    QFETCH( bool, bcs );

    Qt::CaseSensitivity cs = bcs ? Qt::CaseSensitive : Qt::CaseInsensitive;

    if ( after.length() == 0 ) {
	if ( before.length() == 1 && cs ) {
	    QChar ch = before.at( 0 );

	    QString s1 = string;
	    s1.remove( ch );
	    QTEST( s1, "result" );

	    if ( QChar(ch.toLatin1()) == ch ) {
		QString s2 = string;
		s2.remove( ch );
		QTEST( s2, "result" );
	    }
	}

	QString s3 = string;
	s3.remove( before, cs );
	QTEST( s3, "result" );

	QString s4 = string;
	s4.replace( QRegExp(QRegExp::escape(before), cs), after );
	QTEST( s4, "result" );

	QString s5 = string;
	s5.replace( QRegExp(before, cs, QRegExp::FixedString), after );
	QTEST( s5, "result" );
    } else {
	QCOMPARE( 0, 0 ); // shut QtTest
    }
}

void tst_QString::remove_regexp()
{
    QFETCH( QString, string );
    QFETCH( QString, regexp );
    QFETCH( QString, after );

    if ( after.length() == 0 ) {
	string.remove( QRegExp(regexp) );
	QTEST( string, "result" );
    } else {
	QCOMPARE( 0, 0 ); // shut QtTest
    }
}

void tst_QString::toNum()
{
#if defined (Q_WS_WIN) && defined (Q_CC_MSVC)
#define TEST_TO_INT(num, func) \
    a = #num; \
    QVERIFY2(a.func(&ok) == num ## i64 && ok, "Failed: num=" #num ", func=" #func);
#else
#define TEST_TO_INT(num, func) \
    a = #num; \
    QVERIFY2(a.func(&ok) == num ## LL && ok, "Failed: num=" #num ", func=" #func);
#endif

    QString a;
    bool ok = false;

    TEST_TO_INT(0, toInt)
    TEST_TO_INT(-1, toInt)
    TEST_TO_INT(1, toInt)
    TEST_TO_INT(2147483647, toInt)
    TEST_TO_INT(-2147483648, toInt)

    TEST_TO_INT(0, toShort)
    TEST_TO_INT(-1, toShort)
    TEST_TO_INT(1, toShort)
    TEST_TO_INT(32767, toShort)
    TEST_TO_INT(-32768, toShort)

    TEST_TO_INT(0, toLong)
    TEST_TO_INT(-1, toLong)
    TEST_TO_INT(1, toLong)
    TEST_TO_INT(2147483647, toLong)
    TEST_TO_INT(-2147483648, toLong)
    TEST_TO_INT(0, toLongLong)
    TEST_TO_INT(-1, toLongLong)
    TEST_TO_INT(1, toLongLong)
    TEST_TO_INT(9223372036854775807, toLongLong)
    TEST_TO_INT(-9223372036854775807, toLongLong)

#undef TEST_TO_INT

#if defined (Q_WS_WIN) && defined (Q_CC_MSVC)
#define TEST_TO_UINT(num, func) \
    a = #num; \
    QVERIFY2(a.func(&ok) == num ## i64 && ok, "Failed: num=" #num ", func=" #func);
#else
#define TEST_TO_UINT(num, func) \
    a = #num; \
    QVERIFY2(a.func(&ok) == num ## ULL && ok, "Failed: num=" #num ", func=" #func);
#endif

    TEST_TO_UINT(0, toUInt)
    TEST_TO_UINT(1, toUInt)
    TEST_TO_UINT(4294967295, toUInt)

    TEST_TO_UINT(0, toUShort)
    TEST_TO_UINT(1, toUShort)
    TEST_TO_UINT(65535, toUShort)

    TEST_TO_UINT(0, toULong)
    TEST_TO_UINT(1, toULong)
    TEST_TO_UINT(4294967295, toULong)

    TEST_TO_UINT(0, toULongLong)
    TEST_TO_UINT(1, toULongLong)
    TEST_TO_UINT(18446744073709551615, toULongLong)
#undef TEST_TO_UINT


#define TEST_BASE(str, base, num) \
    a = str; \
    QVERIFY2(a.toInt(&ok, base) == num && ok, "Failed: str=" #str " base= "#base " num=" #num ", func=toInt"); \
    QVERIFY2(a.toUInt(&ok, base) == num && ok, "Failed: str=" #str " base= "#base " num=" #num ", func=toUInt"); \
    QVERIFY2(a.toShort(&ok, base) == num && ok, "Failed: str=" #str " base= "#base " num=" #num ", func=toShort"); \
    QVERIFY2(a.toUShort(&ok, base) == num && ok, "Failed: str=" #str " base= "#base " num=" #num ", func=toUShort"); \
    QVERIFY2(a.toLong(&ok, base) == num && ok, "Failed: str=" #str " base= "#base " num=" #num ", func=toLong"); \
    QVERIFY2(a.toULong(&ok, base) == num && ok, "Failed: str=" #str " base= "#base " num=" #num ", func=toULong"); \
    QVERIFY2(a.toLongLong(&ok, base) == num && ok, "Failed: str=" #str " base= "#base " num=" #num ", func=toLongLong"); \
    QVERIFY2(a.toULongLong(&ok, base) == num && ok, "Failed: str=" #str " base= "#base " num=" #num ", func=toULongLong");

    TEST_BASE("FF", 16, 255)
    TEST_BASE("0xFF", 16, 255)
    TEST_BASE("77", 8, 63)
    TEST_BASE("077", 8, 63)

    TEST_BASE("0xFF", 0, 255)
    TEST_BASE("077", 0, 63)
    TEST_BASE("255", 0, 255)

    TEST_BASE(" FF", 16, 255)
    TEST_BASE(" 0xFF", 16, 255)
    TEST_BASE(" 77", 8, 63)
    TEST_BASE(" 077", 8, 63)

    TEST_BASE(" 0xFF", 0, 255)
    TEST_BASE(" 077", 0, 63)
    TEST_BASE(" 255", 0, 255)

    TEST_BASE("\tFF\t", 16, 255)
    TEST_BASE("\t0xFF  ", 16, 255)
    TEST_BASE("   77   ", 8, 63)
    TEST_BASE("77  ", 8, 63)

#undef TEST_BASE

#define TEST_NEG_BASE(str, base, num) \
    a = str; \
    QVERIFY2(a.toInt(&ok, base) == num && ok, "Failed: str=" #str " base= "#base " num=" #num ", func=toInt"); \
    QVERIFY2(a.toShort(&ok, base) == num && ok, "Failed: str=" #str " base= "#base " num=" #num ", func=toShort"); \
    QVERIFY2(a.toLong(&ok, base) == num && ok, "Failed: str=" #str " base= "#base " num=" #num ", func=toLong"); \
    QVERIFY2(a.toLongLong(&ok, base) == num && ok, "Failed: str=" #str " base= "#base " num=" #num ", func=toLongLong");

    TEST_NEG_BASE("-FE", 16, -254)
    TEST_NEG_BASE("-0xFE", 16, -254)
    TEST_NEG_BASE("-77", 8, -63)
    TEST_NEG_BASE("-077", 8, -63)

    TEST_NEG_BASE("-0xFE", 0, -254)
    TEST_NEG_BASE("-077", 0, -63)
    TEST_NEG_BASE("-254", 0, -254)

#undef TEST_NEG_BASE

#define TEST_DOUBLE(num, str) \
    a = str; \
    QCOMPARE(a.toDouble(&ok), num); \
    QVERIFY(ok);

    TEST_DOUBLE(1.2345, "1.2345")
    TEST_DOUBLE(12.345, "1.2345e+01")
    TEST_DOUBLE(12.345, "1.2345E+01")
    TEST_DOUBLE(12345.6, "12345.6")

#undef TEST_DOUBLE


#define TEST_BAD(str, func) \
    a = str; \
    a.func(&ok); \
    QVERIFY2(!ok, "Failed: str=" #str " func=" #func);

    TEST_BAD("32768", toShort)
    TEST_BAD("-32769", toShort)
    TEST_BAD("65536", toUShort)
    TEST_BAD("2147483648", toInt)
    TEST_BAD("-2147483649", toInt)
    TEST_BAD("4294967296", toUInt)
    if (sizeof(long) == 4) {
        TEST_BAD("2147483648", toLong)
        TEST_BAD("-2147483649", toLong)
        TEST_BAD("4294967296", toULong)
    }
    TEST_BAD("9223372036854775808", toLongLong)
    TEST_BAD("-9223372036854775809", toLongLong)
    TEST_BAD("18446744073709551616", toULongLong)
    TEST_BAD("-1", toUShort)
    TEST_BAD("-1", toUInt)
    TEST_BAD("-1", toULong)
    TEST_BAD("-1", toULongLong)
#undef TEST_BAD

#define TEST_BAD_ALL(str) \
    a = str; \
    a.toShort(&ok); \
    QVERIFY2(!ok, "Failed: str=" #str); \
    a.toUShort(&ok); \
    QVERIFY2(!ok, "Failed: str=" #str); \
    a.toInt(&ok); \
    QVERIFY2(!ok, "Failed: str=" #str); \
    a.toUInt(&ok); \
    QVERIFY2(!ok, "Failed: str=" #str); \
    a.toLong(&ok); \
    QVERIFY2(!ok, "Failed: str=" #str); \
    a.toULong(&ok); \
    QVERIFY2(!ok, "Failed: str=" #str); \
    a.toLongLong(&ok); \
    QVERIFY2(!ok, "Failed: str=" #str); \
    a.toULongLong(&ok); \
    QVERIFY2(!ok, "Failed: str=" #str); \
    a.toFloat(&ok); \
    QVERIFY2(!ok, "Failed: str=" #str); \
    a.toDouble(&ok); \
    QVERIFY2(!ok, "Failed: str=" #str);

    TEST_BAD_ALL((const char*)0);
    TEST_BAD_ALL("");
    TEST_BAD_ALL(" ");
    TEST_BAD_ALL(".");
    TEST_BAD_ALL("-");
    TEST_BAD_ALL("hello");
    TEST_BAD_ALL("1.2.3");
    TEST_BAD_ALL("0x0x0x");
    TEST_BAD_ALL("123-^~<");
    TEST_BAD_ALL("123ThisIsNotANumber");

#undef TEST_BAD_ALL

    a = "FF";
    a.toULongLong(&ok, 10);
    QVERIFY(!ok);

    a = "FF";
    a.toULongLong(&ok, 0);
    QVERIFY(!ok);

#ifdef QT_NO_FPU
    double d = 3.40282346638528e+38; // slightly off FLT_MAX when using hardfloats
#else
    double d = 3.4028234663852886e+38; // FLT_MAX
#endif
    QString::number(d, 'e', 17).toFloat(&ok);
    QVERIFY(ok);
    QString::number(d + 1e32, 'e', 17).toFloat(&ok);
    QVERIFY(!ok);
    QString::number(-d, 'e', 17).toFloat(&ok);
    QVERIFY(ok);
    QString::number(-d - 1e32, 'e', 17).toFloat(&ok);
    QVERIFY(!ok);
    QString::number(d + 1e32, 'e', 17).toDouble(&ok);
    QVERIFY(ok);
    QString::number(-d - 1e32, 'e', 17).toDouble(&ok);
    QVERIFY(ok);
}

void tst_QString::toUShort()
{
    QString a;
    bool ok;
    QCOMPARE(a.toUShort(),(ushort)0);
    QCOMPARE(a.toUShort(&ok),(ushort)0);
    QVERIFY(!ok);

    a="";
    QCOMPARE(a.toUShort(),(ushort)0);
    QCOMPARE(a.toUShort(&ok),(ushort)0);
    QVERIFY(!ok);

    a="COMPARE";
    QCOMPARE(a.toUShort(),(ushort)0);
    QCOMPARE(a.toUShort(&ok),(ushort)0);
    QVERIFY(!ok);

    a="123";
    QCOMPARE(a.toUShort(),(ushort)123);
    QCOMPARE(a.toUShort(&ok),(ushort)123);
    QVERIFY(ok);

    a="123A";
    QCOMPARE(a.toUShort(),(ushort)0);
    QCOMPARE(a.toUShort(&ok),(ushort)0);
    QVERIFY(!ok);

    a="1234567";
    QCOMPARE(a.toUShort(),(ushort)0);
    QCOMPARE(a.toUShort(&ok),(ushort)0);
    QVERIFY(!ok);

    a = "aaa123aaa";
    QCOMPARE(a.toUShort(),(ushort)0);
    QCOMPARE(a.toUShort(&ok),(ushort)0);
    QVERIFY(!ok);

    a = "aaa123";
    QCOMPARE(a.toUShort(),(ushort)0);
    QCOMPARE(a.toUShort(&ok),(ushort)0);
    QVERIFY(!ok);

    a = "123aaa";
    QCOMPARE(a.toUShort(),(ushort)0);
    QCOMPARE(a.toUShort(&ok),(ushort)0);
    QVERIFY(!ok);

    a = "32767";
    QCOMPARE(a.toUShort(),(ushort)32767);
    QCOMPARE(a.toUShort(&ok),(ushort)32767);
    QVERIFY(ok);

    a = "-32767";
    QCOMPARE(a.toUShort(),(ushort)0);
    QCOMPARE(a.toUShort(&ok),(ushort)0);
    QVERIFY(!ok);

    a = "65535";
    QCOMPARE(a.toUShort(),(ushort)65535);
    QCOMPARE(a.toUShort(&ok),(ushort)65535);
    QVERIFY(ok);

    if (sizeof(short) == 2) {
	a = "65536";
	QCOMPARE(a.toUShort(),(ushort)0);
	QCOMPARE(a.toUShort(&ok),(ushort)0);
	QVERIFY(!ok);

	a = "123456";
	QCOMPARE(a.toUShort(),(ushort)0);
	QCOMPARE(a.toUShort(&ok),(ushort)0);
	QVERIFY(!ok);
    }
}

void tst_QString::toShort()
{
    QString a;
    bool ok;
    QCOMPARE(a.toShort(),(short)0);
    QCOMPARE(a.toShort(&ok),(short)0);
    QVERIFY(!ok);

    a="";
    QCOMPARE(a.toShort(),(short)0);
    QCOMPARE(a.toShort(&ok),(short)0);
    QVERIFY(!ok);

    a="COMPARE";
    QCOMPARE(a.toShort(),(short)0);
    QCOMPARE(a.toShort(&ok),(short)0);
    QVERIFY(!ok);

    a="123";
    QCOMPARE(a.toShort(),(short)123);
    QCOMPARE(a.toShort(&ok),(short)123);
    QVERIFY(ok);

    a="123A";
    QCOMPARE(a.toShort(),(short)0);
    QCOMPARE(a.toShort(&ok),(short)0);
    QVERIFY(!ok);

    a="1234567";
    QCOMPARE(a.toShort(),(short)0);
    QCOMPARE(a.toShort(&ok),(short)0);
    QVERIFY(!ok);

    a = "aaa123aaa";
    QCOMPARE(a.toShort(),(short)0);
    QCOMPARE(a.toShort(&ok),(short)0);
    QVERIFY(!ok);

    a = "aaa123";
    QCOMPARE(a.toShort(),(short)0);
    QCOMPARE(a.toShort(&ok),(short)0);
    QVERIFY(!ok);

    a = "123aaa";
    QCOMPARE(a.toShort(),(short)0);
    QCOMPARE(a.toShort(&ok),(short)0);
    QVERIFY(!ok);

    a = "32767";
    QCOMPARE(a.toShort(),(short)32767);
    QCOMPARE(a.toShort(&ok),(short)32767);
    QVERIFY(ok);

    a = "-32767";
    QCOMPARE(a.toShort(),(short)-32767);
    QCOMPARE(a.toShort(&ok),(short)-32767);
    QVERIFY(ok);

    a = "-32768";
    QCOMPARE(a.toShort(),(short)-32768);
    QCOMPARE(a.toShort(&ok),(short)-32768);
    QVERIFY(ok);

    if (sizeof(short) == 2) {
	a = "32768";
	QCOMPARE(a.toShort(),(short)0);
	QCOMPARE(a.toShort(&ok),(short)0);
	QVERIFY(!ok);

	a = "-32769";
	QCOMPARE(a.toShort(),(short)0);
	QCOMPARE(a.toShort(&ok),(short)0);
	QVERIFY(!ok);
    }
}

void tst_QString::toInt()
{
    QString a;
    bool ok;
    QCOMPARE(a.toInt(),0);
    QCOMPARE(a.toInt(&ok),0);
    QVERIFY(!ok);

    a = "";
    QCOMPARE(a.toInt(),0);
    QCOMPARE(a.toInt(&ok),0);
    QVERIFY(!ok);

    a="COMPARE";
    QCOMPARE(a.toInt(),0);
    QCOMPARE(a.toInt(&ok),0);
    QVERIFY(!ok);

    a="123";
    QCOMPARE(a.toInt(),123);
    QCOMPARE(a.toInt(&ok),123);
    QVERIFY(ok);

    a="123A";
    QCOMPARE(a.toInt(),0);
    QCOMPARE(a.toInt(&ok),0);
    QVERIFY(!ok);

    a="1234567";
    QCOMPARE(a.toInt(),1234567);
    QCOMPARE(a.toInt(&ok),1234567);
    QVERIFY(ok);

    a="12345678901234";
    QCOMPARE(a.toInt(),0);
    QCOMPARE(a.toInt(&ok),0);
    QVERIFY(!ok);

    a="3234567890";
    QCOMPARE(a.toInt(),0);
    QCOMPARE(a.toInt(&ok),0);
    QVERIFY(!ok);

    a = "aaa12345aaa";
    QCOMPARE(a.toInt(),0);
    QCOMPARE(a.toInt(&ok),0);
    QVERIFY(!ok);

    a = "aaa12345";
    QCOMPARE(a.toInt(),0);
    QCOMPARE(a.toInt(&ok),0);
    QVERIFY(!ok);

    a = "12345aaa";
    QCOMPARE(a.toInt(),0);
    QCOMPARE(a.toInt(&ok),0);
    QVERIFY(!ok);

    a = "2147483647"; // 2**31 - 1
    QCOMPARE(a.toInt(),2147483647);
    QCOMPARE(a.toInt(&ok),2147483647);
    QVERIFY(ok);

    if (sizeof(int) == 4) {
	a = "-2147483647"; // -(2**31 - 1)
	QCOMPARE(a.toInt(),-2147483647);
	QCOMPARE(a.toInt(&ok),-2147483647);
	QVERIFY(ok);

	a = "2147483648"; // 2**31
	QCOMPARE(a.toInt(),0);
	QCOMPARE(a.toInt(&ok),0);
	QVERIFY(!ok);

	a = "-2147483648"; // -2**31
	QCOMPARE(a.toInt(),-2147483647 - 1);
	QCOMPARE(a.toInt(&ok),-2147483647 - 1);
	QVERIFY(ok);

	a = "2147483649"; // 2**31 + 1
	QCOMPARE(a.toInt(),0);
	QCOMPARE(a.toInt(&ok),0);
	QVERIFY(!ok);
    }
}

void tst_QString::toUInt()
{
    bool ok;
    QString a;
    a="3234567890";
    QCOMPARE(a.toUInt(&ok),3234567890u);
    QVERIFY(ok);

    a = "-50";
    QCOMPARE(a.toUInt(),0u);
    QCOMPARE(a.toUInt(&ok),0u);
    QVERIFY(!ok);

    a = "4294967295"; // 2**32 - 1
    QCOMPARE(a.toUInt(),4294967295u);
    QCOMPARE(a.toUInt(&ok),4294967295u);
    QVERIFY(ok);

    if (sizeof(int) == 4) {
	a = "4294967296"; // 2**32
	QCOMPARE(a.toUInt(),0u);
	QCOMPARE(a.toUInt(&ok),0u);
	QVERIFY(!ok);
    }
}

///////////////////////////// to*Long //////////////////////////////////////

void tst_QString::toULong_data()
{
    QTest::addColumn<QString>("str" );
    QTest::addColumn<int>("base" );
    QTest::addColumn<ulong>("result" );
    QTest::addColumn<bool>("ok" );

    QTest::newRow( "default" ) << QString() << 10 << 0UL << false;
    QTest::newRow( "empty" ) << QString("") << 10 << 0UL << false;
    QTest::newRow( "ulong1" ) << QString("3234567890") << 10 << 3234567890UL << true;
    QTest::newRow( "ulong2" ) << QString("fFFfFfFf") << 16 << 0xFFFFFFFFUL << true;
}

void tst_QString::toULong()
{
    QFETCH( QString, str );
    QFETCH( int, base );
    QFETCH( ulong, result );
    QFETCH( bool, ok );

    bool b;
    QCOMPARE( str.toULong( 0, base ), result );
    QCOMPARE( str.toULong( &b, base ), result );
    QCOMPARE( b, ok );
}

void tst_QString::toLong_data()
{
    QTest::addColumn<QString>("str" );
    QTest::addColumn<int>("base" );
    QTest::addColumn<long>("result" );
    QTest::addColumn<bool>("ok" );

    QTest::newRow( "default" ) << QString() << 10 << 0L << false;
    QTest::newRow( "empty" ) << QString("") << 10 << 0L << false;
    QTest::newRow( "normal" ) << QString("7fFFfFFf") << 16 << 0x7fFFfFFfL << true;
    QTest::newRow( "long_max" ) << QString("2147483647") << 10 << 2147483647L << true;
    if (sizeof(long) == 4) {
        QTest::newRow( "long_max+1" ) << QString("2147483648") << 10 << 0L << false;
        QTest::newRow( "long_min-1" ) << QString("-80000001") << 16 << 0L << false;
    }
    QTest::newRow( "negative" ) << QString("-7fffffff") << 16 << -0x7fffffffL << true;
//    QTest::newRow( "long_min" ) << QString("-80000000") << 16 << 0x80000000uL << true;
}

void tst_QString::toLong()
{
    QFETCH( QString, str );
    QFETCH( int, base );
    QFETCH( long, result );
    QFETCH( bool, ok );

    bool b;
    QCOMPARE( str.toLong( 0, base ), result );
    QCOMPARE( str.toLong( &b, base ), result );
    QCOMPARE( b, ok );
}


////////////////////////// to*LongLong //////////////////////////////////////

void tst_QString::toULongLong()
{
    QString str;
    bool ok;
    str = "18446744073709551615"; // ULLONG_MAX
    QCOMPARE( str.toULongLong( 0 ), Q_UINT64_C(18446744073709551615) );
    QCOMPARE( str.toULongLong( &ok ), Q_UINT64_C(18446744073709551615) );
    QVERIFY( ok );

    str = "18446744073709551616"; // ULLONG_MAX + 1
    QCOMPARE( str.toULongLong( 0 ), Q_UINT64_C(0) );
    QCOMPARE( str.toULongLong( &ok ), Q_UINT64_C(0) );
    QVERIFY( !ok );

    str = "-150";
    QCOMPARE( str.toULongLong( 0 ), Q_UINT64_C(0) );
    QCOMPARE( str.toULongLong( &ok ), Q_UINT64_C(0) );
    QVERIFY( !ok );
}

void tst_QString::toLongLong()
{
    QString str;
    bool ok;

    str = "9223372036854775807"; // LLONG_MAX
    QCOMPARE( str.toLongLong( 0 ), Q_INT64_C(9223372036854775807) );
    QCOMPARE( str.toLongLong( &ok ), Q_INT64_C(9223372036854775807) );
    QVERIFY( ok );

    str = "-9223372036854775808"; // LLONG_MIN
    QCOMPARE( str.toLongLong( 0 ),
	     -Q_INT64_C(9223372036854775807) - Q_INT64_C(1) );
    QCOMPARE( str.toLongLong( &ok ),
	     -Q_INT64_C(9223372036854775807) - Q_INT64_C(1) );
    QVERIFY( ok );

    str = "aaaa9223372036854775807aaaa";
    QCOMPARE( str.toLongLong( 0 ), Q_INT64_C(0) );
    QCOMPARE( str.toLongLong( &ok ), Q_INT64_C(0) );
    QVERIFY( !ok );

    str = "9223372036854775807aaaa";
    QCOMPARE( str.toLongLong( 0 ), Q_INT64_C(0) );
    QCOMPARE( str.toLongLong( &ok ), Q_INT64_C(0) );
    QVERIFY( !ok );

    str = "aaaa9223372036854775807";
    QCOMPARE( str.toLongLong( 0 ), Q_INT64_C(0) );
    QCOMPARE( str.toLongLong( &ok ), Q_INT64_C(0) );
    QVERIFY( !ok );

    static char digits[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    for (int i = 0; i < 36; ++i) {
        for (int j = 0; j < 36; ++j) {
            for (int k = 0; k < 36; ++k) {
                QString str;
                str += QChar(digits[i]);
                str += QChar(digits[j]);
                str += QChar(digits[k]);
                qlonglong value = (((i * 36) + j) * 36) + k;
                QVERIFY(str.toLongLong(0, 36) == value);
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////

void tst_QString::toFloat()
{
    QString a;
    bool ok;
    a="0.000000000931322574615478515625";
    QCOMPARE(a.toFloat(&ok),(float)(0.000000000931322574615478515625));
    QVERIFY(ok);
}

void tst_QString::toDouble_data()
{
    QTest::addColumn<QString>("str" );
    QTest::addColumn<double>("result" );
    QTest::addColumn<bool>("result_ok" );

    QTest::newRow( "ok00" ) << QString("0.000000000931322574615478515625") << 0.000000000931322574615478515625 << true;
    QTest::newRow( "ok01" ) << QString(" 123.45") << 123.45 << true;

    QTest::newRow( "ok02" ) << QString("0.1e10") << 0.1e10 << true;
    QTest::newRow( "ok03" ) << QString("0.1e-10") << 0.1e-10 << true;

    QTest::newRow( "ok04" ) << QString("1e10") << 1.0e10 << true;
    QTest::newRow( "ok05" ) << QString("1e+10") << 1.0e10 << true;
    QTest::newRow( "ok06" ) << QString("1e-10") << 1.0e-10 << true;

    QTest::newRow( "ok07" ) << QString(" 1e10") << 1.0e10 << true;
    QTest::newRow( "ok08" ) << QString("  1e+10") << 1.0e10 << true;
    QTest::newRow( "ok09" ) << QString("   1e-10") << 1.0e-10 << true;

    QTest::newRow( "ok10" ) << QString("1.") << 1.0 << true;
    QTest::newRow( "ok11" ) << QString(".1") << 0.1 << true;

    QTest::newRow( "wrong00" ) << QString("123.45 ") << 123.45 << true;
    QTest::newRow( "wrong01" ) << QString(" 123.45 ") << 123.45 << true;

    QTest::newRow( "wrong02" ) << QString("aa123.45aa") << 0.0 << false;
    QTest::newRow( "wrong03" ) << QString("123.45aa") << 0.0 << false;
    QTest::newRow( "wrong04" ) << QString("123erf") << 0.0 << false;

    QTest::newRow( "wrong05" ) << QString("abc") << 0.0 << false;
    QTest::newRow( "wrong06" ) << QString() << 0.0 << false;
    QTest::newRow( "wrong07" ) << QString("") << 0.0 << false;
}

void tst_QString::toDouble()
{
    QFETCH( QString, str );
    QFETCH( bool, result_ok );
    bool ok;
    double d = str.toDouble( &ok );
    if ( result_ok ) {
	QTEST( d, "result" );
	QVERIFY( ok );
    } else {
	QVERIFY( !ok );
    }
}

void tst_QString::setNum()
{
    QString a;
    QCOMPARE(a.setNum(123),(QString)"123");
    QCOMPARE(a.setNum(-123),(QString)"-123");
    QCOMPARE(a.setNum(0x123,16),(QString)"123");
    QCOMPARE(a.setNum((short)123),(QString)"123");
    QCOMPARE(a.setNum(123L),(QString)"123");
    QCOMPARE(a.setNum(123UL),(QString)"123");
    QCOMPARE(a.setNum(2147483647L), QString("2147483647")); // 32 bit LONG_MAX
    QCOMPARE(a.setNum(-2147483647L), QString("-2147483647")); // LONG_MIN + 1
    QCOMPARE(a.setNum(-2147483647L-1L), QString("-2147483648")); // LONG_MIN
    QCOMPARE(a.setNum(1.23), QString("1.23"));
    QCOMPARE(a.setNum(1.234567), QString("1.23457"));
#if defined(LONG_MAX) && defined(LLONG_MAX) && LONG_MAX == LLONG_MAX
    // LONG_MAX and LONG_MIN on 64 bit systems
    QCOMPARE(a.setNum(9223372036854775807L), QString("9223372036854775807"));
    QCOMPARE(a.setNum(-9223372036854775807L-1L), QString("-9223372036854775808"));
    QCOMPARE(a.setNum(18446744073709551615UL), QString("18446744073709551615"));
#endif
    QCOMPARE(a.setNum(Q_INT64_C(123)), QString("123"));
    // 2^40 == 1099511627776
    QCOMPARE(a.setNum(Q_INT64_C(-1099511627776)), QString("-1099511627776"));
    QCOMPARE(a.setNum(Q_UINT64_C(1099511627776)), QString("1099511627776"));
    QCOMPARE(a.setNum(Q_INT64_C(9223372036854775807)), // LLONG_MAX
	    QString("9223372036854775807"));
    QCOMPARE(a.setNum(-Q_INT64_C(9223372036854775807) - Q_INT64_C(1)),
	    QString("-9223372036854775808"));
    QCOMPARE(a.setNum(Q_UINT64_C(18446744073709551615)), // ULLONG_MAX
	    QString("18446744073709551615"));
    QCOMPARE(a.setNum(0.000000000931322574615478515625),QString("9.31323e-10"));

//  QCOMPARE(a.setNum(0.000000000931322574615478515625,'g',30),(QString)"9.31322574615478515625e-010");
//  QCOMPARE(a.setNum(0.000000000931322574615478515625,'f',30),(QString)"0.00000000093132257461547852");
}

void tst_QString::startsWith()
{
    QString a;
    a = "AB";
    QVERIFY( a.startsWith("A") );
    QVERIFY( a.startsWith("AB") );
    QVERIFY( !a.startsWith("C") );
    QVERIFY( !a.startsWith("ABCDEF") );
    QVERIFY( a.startsWith("") );
    QVERIFY( a.startsWith(QString::null) );
    QVERIFY( a.startsWith('A') );
    QVERIFY( a.startsWith(QLatin1Char('A')) );
    QVERIFY( a.startsWith(QChar('A')) );
    QVERIFY( !a.startsWith('C') );
    QVERIFY( !a.startsWith(QChar()) );
    QVERIFY( !a.startsWith(QLatin1Char(0)) );

    QVERIFY( a.startsWith(QLatin1String("A")) );
    QVERIFY( a.startsWith(QLatin1String("AB")) );
    QVERIFY( !a.startsWith(QLatin1String("C")) );
    QVERIFY( !a.startsWith(QLatin1String("ABCDEF")) );
    QVERIFY( a.startsWith(QLatin1String("")) );
    QVERIFY( a.startsWith(QLatin1String(0)) );

    QVERIFY( a.startsWith("A", Qt::CaseSensitive) );
    QVERIFY( a.startsWith("A", Qt::CaseInsensitive) );
    QVERIFY( !a.startsWith("a", Qt::CaseSensitive) );
    QVERIFY( a.startsWith("a", Qt::CaseInsensitive) );
    QVERIFY( !a.startsWith("aB", Qt::CaseSensitive) );
    QVERIFY( a.startsWith("aB", Qt::CaseInsensitive) );
    QVERIFY( !a.startsWith("C", Qt::CaseSensitive) );
    QVERIFY( !a.startsWith("C", Qt::CaseInsensitive) );
    QVERIFY( !a.startsWith("c", Qt::CaseSensitive) );
    QVERIFY( !a.startsWith("c", Qt::CaseInsensitive) );
    QVERIFY( !a.startsWith("abcdef", Qt::CaseInsensitive) );
    QVERIFY( a.startsWith("", Qt::CaseInsensitive) );
    QVERIFY( a.startsWith(QString::null, Qt::CaseInsensitive) );
    QVERIFY( a.startsWith('a', Qt::CaseInsensitive) );
    QVERIFY( a.startsWith('A', Qt::CaseInsensitive) );
    QVERIFY( a.startsWith(QLatin1Char('a'), Qt::CaseInsensitive) );
    QVERIFY( a.startsWith(QChar('a'), Qt::CaseInsensitive) );
    QVERIFY( !a.startsWith('c', Qt::CaseInsensitive) );
    QVERIFY( !a.startsWith(QChar(), Qt::CaseInsensitive) );
    QVERIFY( !a.startsWith(QLatin1Char(0), Qt::CaseInsensitive) );

    QVERIFY( a.startsWith(QLatin1String("A"), Qt::CaseSensitive) );
    QVERIFY( a.startsWith(QLatin1String("A"), Qt::CaseInsensitive) );
    QVERIFY( !a.startsWith(QLatin1String("a"), Qt::CaseSensitive) );
    QVERIFY( a.startsWith(QLatin1String("a"), Qt::CaseInsensitive) );
    QVERIFY( !a.startsWith(QLatin1String("aB"), Qt::CaseSensitive) );
    QVERIFY( a.startsWith(QLatin1String("aB"), Qt::CaseInsensitive) );
    QVERIFY( !a.startsWith(QLatin1String("C"), Qt::CaseSensitive) );
    QVERIFY( !a.startsWith(QLatin1String("C"), Qt::CaseInsensitive) );
    QVERIFY( !a.startsWith(QLatin1String("c"), Qt::CaseSensitive) );
    QVERIFY( !a.startsWith(QLatin1String("c"), Qt::CaseInsensitive) );
    QVERIFY( !a.startsWith(QLatin1String("abcdef"), Qt::CaseInsensitive) );
    QVERIFY( a.startsWith(QLatin1String(""), Qt::CaseInsensitive) );
    QVERIFY( a.startsWith(QLatin1String(0), Qt::CaseInsensitive) );
    QVERIFY( a.startsWith('A', Qt::CaseSensitive) );
    QVERIFY( a.startsWith(QLatin1Char('A'), Qt::CaseSensitive) );
    QVERIFY( a.startsWith(QChar('A'), Qt::CaseSensitive) );
    QVERIFY( !a.startsWith('a', Qt::CaseSensitive) );
    QVERIFY( !a.startsWith(QChar(), Qt::CaseSensitive) );
    QVERIFY( !a.startsWith(QLatin1Char(0), Qt::CaseSensitive) );

    a = "";
    QVERIFY( a.startsWith("") );
    QVERIFY( a.startsWith(QString::null) );
    QVERIFY( !a.startsWith("ABC") );

    QVERIFY( a.startsWith(QLatin1String("")) );
    QVERIFY( a.startsWith(QLatin1String(0)) );
    QVERIFY( !a.startsWith(QLatin1String("ABC")) );

    QVERIFY( !a.startsWith(QLatin1Char(0)) );
    QVERIFY( !a.startsWith(QLatin1Char('x')) );
    QVERIFY( !a.startsWith(QChar()) );

    a = QString::null;
    QVERIFY( !a.startsWith("") );
    QVERIFY( a.startsWith(QString::null) );
    QVERIFY( !a.startsWith("ABC") );

    QVERIFY( !a.startsWith(QLatin1String("")) );
    QVERIFY( a.startsWith(QLatin1String(0)) );
    QVERIFY( !a.startsWith(QLatin1String("ABC")) );

    QVERIFY( !a.startsWith(QLatin1Char(0)) );
    QVERIFY( !a.startsWith(QLatin1Char('x')) );
    QVERIFY( !a.startsWith(QChar()) );
}

void tst_QString::endsWith()
{
    QString a;
    a = "AB";
    QVERIFY( a.endsWith("B") );
    QVERIFY( a.endsWith("AB") );
    QVERIFY( !a.endsWith("C") );
    QVERIFY( !a.endsWith("ABCDEF") );
    QVERIFY( a.endsWith("") );
    QVERIFY( a.endsWith(QString::null) );
    QVERIFY( a.endsWith('B') );
    QVERIFY( a.endsWith(QLatin1Char('B')) );
    QVERIFY( a.endsWith(QChar('B')) );
    QVERIFY( !a.endsWith('C') );
    QVERIFY( !a.endsWith(QChar()) );
    QVERIFY( !a.endsWith(QLatin1Char(0)) );

    QVERIFY( a.endsWith(QLatin1String("B")) );
    QVERIFY( a.endsWith(QLatin1String("AB")) );
    QVERIFY( !a.endsWith(QLatin1String("C")) );
    QVERIFY( !a.endsWith(QLatin1String("ABCDEF")) );
    QVERIFY( a.endsWith(QLatin1String("")) );
    QVERIFY( a.endsWith(QLatin1String(0)) );

    QVERIFY( a.endsWith("B", Qt::CaseSensitive) );
    QVERIFY( a.endsWith("B", Qt::CaseInsensitive) );
    QVERIFY( !a.endsWith("b", Qt::CaseSensitive) );
    QVERIFY( a.endsWith("b", Qt::CaseInsensitive) );
    QVERIFY( !a.endsWith("aB", Qt::CaseSensitive) );
    QVERIFY( a.endsWith("aB", Qt::CaseInsensitive) );
    QVERIFY( !a.endsWith("C", Qt::CaseSensitive) );
    QVERIFY( !a.endsWith("C", Qt::CaseInsensitive) );
    QVERIFY( !a.endsWith("c", Qt::CaseSensitive) );
    QVERIFY( !a.endsWith("c", Qt::CaseInsensitive) );
    QVERIFY( !a.endsWith("abcdef", Qt::CaseInsensitive) );
    QVERIFY( a.endsWith("", Qt::CaseInsensitive) );
    QVERIFY( a.endsWith(QString::null, Qt::CaseInsensitive) );
    QVERIFY( a.endsWith('b', Qt::CaseInsensitive) );
    QVERIFY( a.endsWith('B', Qt::CaseInsensitive) );
    QVERIFY( a.endsWith(QLatin1Char('b'), Qt::CaseInsensitive) );
    QVERIFY( a.endsWith(QChar('b'), Qt::CaseInsensitive) );
    QVERIFY( !a.endsWith('c', Qt::CaseInsensitive) );
    QVERIFY( !a.endsWith(QChar(), Qt::CaseInsensitive) );
    QVERIFY( !a.endsWith(QLatin1Char(0), Qt::CaseInsensitive) );

    QVERIFY( a.endsWith(QLatin1String("B"), Qt::CaseSensitive) );
    QVERIFY( a.endsWith(QLatin1String("B"), Qt::CaseInsensitive) );
    QVERIFY( !a.endsWith(QLatin1String("b"), Qt::CaseSensitive) );
    QVERIFY( a.endsWith(QLatin1String("b"), Qt::CaseInsensitive) );
    QVERIFY( !a.endsWith(QLatin1String("aB"), Qt::CaseSensitive) );
    QVERIFY( a.endsWith(QLatin1String("aB"), Qt::CaseInsensitive) );
    QVERIFY( !a.endsWith(QLatin1String("C"), Qt::CaseSensitive) );
    QVERIFY( !a.endsWith(QLatin1String("C"), Qt::CaseInsensitive) );
    QVERIFY( !a.endsWith(QLatin1String("c"), Qt::CaseSensitive) );
    QVERIFY( !a.endsWith(QLatin1String("c"), Qt::CaseInsensitive) );
    QVERIFY( !a.endsWith(QLatin1String("abcdef"), Qt::CaseInsensitive) );
    QVERIFY( a.endsWith(QLatin1String(""), Qt::CaseInsensitive) );
    QVERIFY( a.endsWith(QLatin1String(0), Qt::CaseInsensitive) );
    QVERIFY( a.endsWith('B', Qt::CaseSensitive) );
    QVERIFY( a.endsWith(QLatin1Char('B'), Qt::CaseSensitive) );
    QVERIFY( a.endsWith(QChar('B'), Qt::CaseSensitive) );
    QVERIFY( !a.endsWith('b', Qt::CaseSensitive) );
    QVERIFY( !a.endsWith(QChar(), Qt::CaseSensitive) );
    QVERIFY( !a.endsWith(QLatin1Char(0), Qt::CaseSensitive) );

    a = "";
    QVERIFY( a.endsWith("") );
    QVERIFY( a.endsWith(QString::null) );
    QVERIFY( !a.endsWith("ABC") );
    QVERIFY( !a.endsWith(QLatin1Char(0)) );
    QVERIFY( !a.endsWith(QLatin1Char('x')) );
    QVERIFY( !a.endsWith(QChar()) );

    QVERIFY( a.endsWith(QLatin1String("")) );
    QVERIFY( a.endsWith(QLatin1String(0)) );
    QVERIFY( !a.endsWith(QLatin1String("ABC")) );

    a = QString::null;
    QVERIFY( !a.endsWith("") );
    QVERIFY( a.endsWith(QString::null) );
    QVERIFY( !a.endsWith("ABC") );

    QVERIFY( !a.endsWith(QLatin1String("")) );
    QVERIFY( a.endsWith(QLatin1String(0)) );
    QVERIFY( !a.endsWith(QLatin1String("ABC")) );

    QVERIFY( !a.endsWith(QLatin1Char(0)) );
    QVERIFY( !a.endsWith(QLatin1Char('x')) );
    QVERIFY( !a.endsWith(QChar()) );
}

void tst_QString::check_QDataStream()
{
    QString a;
    QByteArray ar;
    {
	QDataStream out(&ar,QIODevice::WriteOnly);
	out << QString("COMPARE Text");
    }
    {
        QDataStream in(&ar,QIODevice::ReadOnly);
        in >> a;
        QCOMPARE(a,(QString)"COMPARE Text");
    }
}

void tst_QString::check_QTextStream()
{
    QString a;
    QByteArray ar;
    {
	QTextStream out(&ar,QIODevice::WriteOnly);
	out << QString("This is COMPARE Text");
    }
    {
	QTextStream in(&ar,QIODevice::ReadOnly);
	in >> a;
	QCOMPARE(a,(QString)"This");
    }
}

void tst_QString::check_QTextIOStream()
{
    QString a;
    {
	a="";
	QTextStream ts(&a);
	ts << "pi \261= " << 3.125;
	QCOMPARE(a,(QString)"pi \261= 3.125");
    }
    {
	a="123 456";
	int x,y;
	QTextStream(&a) >> x >> y;
	QCOMPARE(x,123);
	QCOMPARE(y,456);
    }
}

void tst_QString::fromRawData()
{
    const QChar ptr[] = { 0x1234, 0x0000 };
    QString cstr = QString::fromRawData(ptr, 1);
    QVERIFY(cstr.constData() == ptr);
    cstr.squeeze();
    QVERIFY(cstr.constData() == ptr);
    cstr.detach();
    QVERIFY(cstr.size() == 1);
    QVERIFY(cstr.capacity() == 1);
    QVERIFY(cstr.constData() != ptr);
    QVERIFY(cstr.constData()[0] == QChar(0x1234));
    QVERIFY(cstr.constData()[1] == QChar(0x0000));
}

void tst_QString::fromStdString()
{
#ifdef Q_CC_HPACC
    QSKIP("This test crashes on HP-UX with aCC", SkipSingle);
#endif
#if !defined(QT_NO_STL)
    std::string stroustrup = "foo";
    QString eng = QString::fromStdString( stroustrup );
    QCOMPARE( eng, QString("foo") );
    const char cnull[] = "Embedded\0null\0character!";
    std::string stdnull( cnull, sizeof(cnull)-1 );
    QString qtnull = QString::fromStdString( stdnull );
    QCOMPARE( qtnull.size(), int(stdnull.size()) );
#endif
}

void tst_QString::toStdString()
{
#ifdef Q_CC_HPACC
    QSKIP("This test crashes on HP-UX with aCC", SkipSingle);
#endif
#if !defined(QT_NO_STL)
    QString nord = "foo";
    std::string stroustrup1 = nord.toStdString();
    QVERIFY( qstrcmp(stroustrup1.c_str(), "foo") == 0 );
    // For now, most QString constructors are also broken with respect
    // to embedded null characters, had to find one that works...
    const QChar qcnull[] = {
        'E', 'm', 'b', 'e', 'd', 'd', 'e', 'd', '\0',
        'n', 'u', 'l', 'l', '\0',
        'c', 'h', 'a', 'r', 'a', 'c', 't', 'e', 'r', '!'
    };
    QString qtnull( qcnull, sizeof(qcnull)/sizeof(QChar) );
    std::string stdnull = qtnull.toStdString();
    QCOMPARE( int(stdnull.size()), qtnull.size() );
#endif
}

void tst_QString::utf8()
{
    QFETCH( QByteArray, utf8 );
    QFETCH( QString, res );

    QCOMPARE( utf8, QByteArray(res.toUtf8()) );
}

// copied to tst_QTextCodec::utf8Codec_data()
void tst_QString::fromUtf8_data()
{
    QTest::addColumn<QByteArray>("utf8");
    QTest::addColumn<QString>("res");
    QTest::addColumn<int>("len");
    QString str;

    QTest::newRow("str0") << QByteArray("abcdefgh") << QString("abcdefgh") << -1;
    QTest::newRow("str0-len") << QByteArray("abcdefgh") << QString("abc") << 3;
    QTest::newRow("str1") << QByteArray("\303\266\303\244\303\274\303\226\303\204\303\234\303\270\303\246\303\245\303\230\303\206\303\205") << QString("\366\344\374\326\304\334\370\346\345\330\306\305") << -1;
    QTest::newRow("str1-len") << QByteArray("\303\266\303\244\303\274\303\226\303\204\303\234\303\270\303\246\303\245\303\230\303\206\303\205") << QString("\366\344\374\326\304") << 10;

    str += QChar(0x05e9);
    str += QChar(0x05d3);
    str += QChar(0x05d2);
    QTest::newRow("str2") << QByteArray("\327\251\327\223\327\222") << str << -1;

    str = QChar(0x05e9);
    QTest::newRow("str2-len") << QByteArray("\327\251\327\223\327\222") << str << 2;

    str = QChar(0x20ac);
    str += " some text";
    QTest::newRow("str3") << QByteArray("\342\202\254 some text") << str << -1;

    str = QChar(0x20ac);
    str += " some ";
    QTest::newRow("str3-len") << QByteArray("\342\202\254 some text") << str << 9;

    str = "hello";
    str += QChar::ReplacementCharacter;
    str += QChar(0x68);
    str += QChar::ReplacementCharacter;
    str += QChar::ReplacementCharacter;
    str += QChar::ReplacementCharacter;
    str += QChar::ReplacementCharacter;
    str += QChar(0x61);
    str += QChar::ReplacementCharacter;
    QTest::newRow("invalid utf8") << QByteArray("hello\344h\344\344\366\344a\304") << str << -1;
    QTest::newRow("invalid utf8-len") << QByteArray("hello\344h\344\344\366\344a\304") << QString("hello") << 5;

    str = "Prohl";
    str += QChar::ReplacementCharacter;
    str += QChar::ReplacementCharacter;
    str += "e";
    str += QChar::ReplacementCharacter;
    str += " plugin";
    str += QChar::ReplacementCharacter;
    str += " Netscape";

    QTest::newRow("task28417") << QByteArray("Prohl\355\276e\350 plugin\371 Netscape") << str << -1;
    QTest::newRow("task28417-len") << QByteArray("Prohl\355\276e\350 plugin\371 Netscape") << QString("") << 0;

    QTest::newRow("null-1") << QByteArray() << QString() << -1;
    QTest::newRow("null0") << QByteArray() << QString() << 0;
    QTest::newRow("null5") << QByteArray() << QString() << 5;
    QTest::newRow("empty-1") << QByteArray("\0abcd", 5) << QString() << -1;
    QTest::newRow("empty0") << QByteArray() << QString() << 0;
    QTest::newRow("empty5") << QByteArray("\0abcd", 5) << QString::fromAscii("\0abcd", 5) << 5;
    QTest::newRow("other-1") << QByteArray("ab\0cd", 5) << QString::fromAscii("ab") << -1;
    QTest::newRow("other5") << QByteArray("ab\0cd", 5) << QString::fromAscii("ab\0cd", 5) << 5;

    str = "Old Italic: ";
    str += QChar(0xd800);
    str += QChar(0xdf00);
    str += QChar(0xd800);
    str += QChar(0xdf01);
    str += QChar(0xd800);
    str += QChar(0xdf02);
    str += QChar(0xd800);
    str += QChar(0xdf03);
    str += QChar(0xd800);
    str += QChar(0xdf04);
    QTest::newRow("surrogate") << QByteArray("Old Italic: \360\220\214\200\360\220\214\201\360\220\214\202\360\220\214\203\360\220\214\204") << str << -1;

    QTest::newRow("surrogate-len") << QByteArray("Old Italic: \360\220\214\200\360\220\214\201\360\220\214\202\360\220\214\203\360\220\214\204") << str.left(16) << 20;

}

void tst_QString::fromUtf8()
{
    QFETCH(QByteArray, utf8);
    QFETCH(QString, res);
    QFETCH(int, len);

    QCOMPARE(QString::fromUtf8(utf8.isNull() ? 0 : utf8.data(), len), res);
}

void tst_QString::nullFromUtf8()
{
    QString a;
    a = QString::fromUtf8(0);
    QVERIFY(a.isNull());
    QVERIFY(a.isEmpty());
    a = QString::fromUtf8("");
    QVERIFY(!a.isNull());
    QVERIFY(a.isEmpty());
}

void tst_QString::fromLocal8Bit_data()
{
    QTest::addColumn<QByteArray>("local8Bit");
    QTest::addColumn<int>("len");
    QTest::addColumn<QString>("result");

    //QTest::newRow("nullString") << QByteArray() << -1 << QString();
    //QTest::newRow("emptyString") << QByteArray("") << -1 << QString("");
    //QTest::newRow("string") << QByteArray("test") << -1 << QString("test");
    //QTest::newRow("stringlen0") << QByteArray("test") << 0 << QString("");
    //QTest::newRow("stringlen3") << QByteArray("test") << 3 << QString("tes");
    QTest::newRow("stringlen99") << QByteArray("test\0foo", 8) << 8 << QString::fromLatin1("test\0foo", 8);

    QByteArray longQByteArray;
    QString longQString;

    for (int l=0;l<111;l++) {
	longQByteArray = longQByteArray + "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
	longQString += "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
    }

    //QTest::newRow("longString") << longQByteArray << -1 << longQString;
    //QTest::newRow("longStringlen0") << longQByteArray << 0 << QString("");
    //QTest::newRow("longStringlen3") << longQByteArray << 3 << QString("aaa");
    //QTest::newRow("someNonAlphaChars") << QByteArray("d:/this/is/a/test.h") << -1 << QString("d:/this/is/a/test.h");

    //QTest::newRow("null-1") << QByteArray() << -1 << QString();
    //QTest::newRow("null0") << QByteArray() << 0 << QString();
    //QTest::newRow("null5") << QByteArray() << 5 << QString();
    //QTest::newRow("empty-1") << QByteArray("\0abcd", 5) << -1 << QString();
    //QTest::newRow("empty0") << QByteArray() << 0 << QString();
    //QTest::newRow("empty5") << QByteArray("\0abcd", 5) << 5 << QString::fromAscii("\0abcd", 5);
    //QTest::newRow("other-1") << QByteArray("ab\0cd", 5) << -1 << QString::fromAscii("ab");
    //QTest::newRow("other5") << QByteArray("ab\0cd", 5) << 5 << QString::fromAscii("ab\0cd", 5);
}

void tst_QString::fromLocal8Bit()
{
    QFETCH(QByteArray, local8Bit);
    QFETCH(int, len);
    QFETCH(QString, result);

    QCOMPARE(QString::fromLocal8Bit(local8Bit.isNull() ? 0 : local8Bit.data(), len).length(),
            result.length());
    QCOMPARE(QString::fromLocal8Bit(local8Bit.isNull() ? 0 : local8Bit.data(), len), result);
}

void tst_QString::local8Bit_data()
{
    QTest::addColumn<QString>("local8Bit");
    QTest::addColumn<QByteArray>("result");

/*
    QString::local8Bit() called on a null QString returns an _empty_
    QByteArray. This is the correct behavior since Qt 3.1.
*/
    QTest::newRow("nullString") << QString() << QByteArray("");
    QTest::newRow("emptyString") << QString("") << QByteArray("");
    QTest::newRow("string") << QString("test") << QByteArray("test");

    QByteArray longQByteArray;
    QString longQString;

    for (int l=0;l<111;l++) {
	longQByteArray = longQByteArray + "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
	longQString += "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
    }

    QTest::newRow("longString") << longQString << longQByteArray;
    QTest::newRow("someNonAlphaChars") << QString("d:/this/is/a/test.h") << QByteArray("d:/this/is/a/test.h");
}

void tst_QString::local8Bit()
{
    QFETCH(QString, local8Bit);
    QFETCH(QByteArray, result);

    QCOMPARE(local8Bit.toLocal8Bit(), QByteArray(result));
}

void tst_QString::fromLatin1Roundtrip_data()
{
    QTest::addColumn<QByteArray>("latin1");
    QTest::addColumn<QString>("unicode");

    QTest::newRow("null") << QByteArray() << QString();
    QTest::newRow("empty") << QByteArray("") << "";

    static const ushort unicode1[] = { 'H', 'e', 'l', 'l', 'o', 1, '\r', '\n', 0x7f };
    QTest::newRow("ascii-only") << QByteArray("Hello") << QString::fromUtf16(unicode1, 5);
    QTest::newRow("ascii+control") << QByteArray("Hello\1\r\n\x7f") << QString::fromUtf16(unicode1, 9);

    static const ushort unicode3[] = { 'a', 0, 'z' };
    QTest::newRow("ascii+nul") << QByteArray("a\0z", 3) << QString::fromUtf16(unicode3, 3);

    static const ushort unicode4[] = { 0x80, 0xc0, 0xff };
    QTest::newRow("non-ascii") << QByteArray("\x80\xc0\xff") << QString::fromUtf16(unicode4, 3);
}

void tst_QString::fromLatin1Roundtrip()
{
    QFETCH(QByteArray, latin1);
    QFETCH(QString, unicode);

    // QtTest safety check:
    Q_ASSERT(latin1.isNull() == unicode.isNull());
    Q_ASSERT(latin1.isEmpty() == unicode.isEmpty());
    Q_ASSERT(latin1.length() == unicode.length());

    if (!latin1.isEmpty())
        while (latin1.length() < 128) {
            latin1 += latin1;
            unicode += unicode;
        }

    // fromLatin1
    QCOMPARE(QString::fromLatin1(latin1, latin1.length()).length(), unicode.length());
    QCOMPARE(QString::fromLatin1(latin1, latin1.length()), unicode);

    // and back:
    QCOMPARE(unicode.toLatin1().length(), latin1.length());
    QCOMPARE(unicode.toLatin1(), latin1);
}

void tst_QString::toLatin1Roundtrip_data()
{
    QTest::addColumn<QByteArray>("latin1");
    QTest::addColumn<QString>("unicodesrc");
    QTest::addColumn<QString>("unicodedst");

    QTest::newRow("null") << QByteArray() << QString() << QString();
    QTest::newRow("empty") << QByteArray("") << "" << "";

    static const ushort unicode1[] = { 'H', 'e', 'l', 'l', 'o', 1, '\r', '\n', 0x7f };
    QTest::newRow("ascii-only") << QByteArray("Hello") << QString::fromUtf16(unicode1, 5) << QString::fromUtf16(unicode1, 5);
    QTest::newRow("ascii+control") << QByteArray("Hello\1\r\n\x7f") << QString::fromUtf16(unicode1, 9)  << QString::fromUtf16(unicode1, 9);

    static const ushort unicode3[] = { 'a', 0, 'z' };
    QTest::newRow("ascii+nul") << QByteArray("a\0z", 3) << QString::fromUtf16(unicode3, 3) << QString::fromUtf16(unicode3, 3);

    static const ushort unicode4[] = { 0x80, 0xc0, 0xff };
    QTest::newRow("non-ascii") << QByteArray("\x80\xc0\xff") << QString::fromUtf16(unicode4, 3) << QString::fromUtf16(unicode4, 3);

    static const ushort unicodeq[] = { '?', '?', '?', '?', '?' };
    const QString questionmarks = QString::fromUtf16(unicodeq, 5);

    static const ushort unicode5[] = { 0x100, 0x101, 0x17f, 0x7f00, 0x7f7f };
    QTest::newRow("non-latin1a") << QByteArray("?????") << QString::fromUtf16(unicode5, 5) << questionmarks;

    static const ushort unicode6[] = { 0x180, 0x1ff, 0x8001, 0x8080, 0xfffc };
    QTest::newRow("non-latin1b") << QByteArray("?????") << QString::fromUtf16(unicode6, 5) << questionmarks;
}

void tst_QString::toLatin1Roundtrip()
{
    QFETCH(QByteArray, latin1);
    QFETCH(QString, unicodesrc);
    QFETCH(QString, unicodedst);

    // QtTest safety check:
    Q_ASSERT(latin1.isNull() == unicodesrc.isNull());
    Q_ASSERT(latin1.isEmpty() == unicodesrc.isEmpty());
    Q_ASSERT(latin1.length() == unicodesrc.length());
    Q_ASSERT(latin1.isNull() == unicodedst.isNull());
    Q_ASSERT(latin1.isEmpty() == unicodedst.isEmpty());
    Q_ASSERT(latin1.length() == unicodedst.length());

    if (!latin1.isEmpty())
        while (latin1.length() < 128) {
            latin1 += latin1;
            unicodesrc += unicodesrc;
            unicodedst += unicodedst;
        }

    // toLatin1
    QCOMPARE(unicodesrc.toLatin1().length(), latin1.length());
    QCOMPARE(unicodesrc.toLatin1(), latin1);

    // and back:
    QCOMPARE(QString::fromLatin1(latin1, latin1.length()).length(), unicodedst.length());
    QCOMPARE(QString::fromLatin1(latin1, latin1.length()), unicodedst);
}

void tst_QString::fromLatin1()
{
    QString a;
    a = QString::fromLatin1( 0 );
    QVERIFY( a.isNull() );
    QVERIFY( a.isEmpty() );
    a = QString::fromLatin1( "" );
    QVERIFY( !a.isNull() );
    QVERIFY( a.isEmpty() );

    a = QString::fromLatin1(0, 0);
    QVERIFY(a.isNull());
    a = QString::fromLatin1(0, 5);
    QVERIFY(a.isNull());
    a = QString::fromLatin1("\0abcd", 0);
    QVERIFY(!a.isNull());
    QVERIFY(a.isEmpty());
    a = QString::fromLatin1("\0abcd", 5);
    QVERIFY(a.size() == 5);
}

void tst_QString::fromAscii()
{
    QString a;
    a = QString::fromAscii( 0 );
    QVERIFY( a.isNull() );
    QVERIFY( a.isEmpty() );
    a = QString::fromAscii( "" );
    QVERIFY( !a.isNull() );
    QVERIFY( a.isEmpty() );

    a = QString::fromAscii(0, 0);
    QVERIFY(a.isNull());
    a = QString::fromAscii(0, 5);
    QVERIFY(a.isNull());
    a = QString::fromAscii("\0abcd", 0);
    QVERIFY(!a.isNull());
    QVERIFY(a.isEmpty());
    a = QString::fromAscii("\0abcd", 5);
    QVERIFY(a.size() == 5);
}

void tst_QString::arg()
{
/*
    Warning: If any of these test fails, the warning given by QtTest
    is all messed up, because QtTest itself uses QString::arg().
*/

    QLocale::setDefault(QString("de_DE"));

    QString s4( "[%0]" );
    QString s5( "[%1]" );
    QString s6( "[%3]" );
    QString s7( "[%9]" );
    QString s8( "[%0 %1]" );
    QString s9( "[%0 %3]" );
    QString s10( "[%1 %2 %3]" );
    QString s11( "[%9 %3 %0]" );
    QString s12( "[%9 %1 %3 %9 %0 %8]" );
    QString s13( "%1% %x%c%2 %d%2-%" );
    QString s14( "%1%2%3" );

    QCOMPARE( s4.arg("foo"), QString("[foo]") );
    QCOMPARE( s5.arg("foo"), QString("[foo]") );
    QCOMPARE( s6.arg("foo"), QString("[foo]") );
    QCOMPARE( s7.arg("foo"), QString("[foo]") );
    QCOMPARE( s8.arg("foo"), QString("[foo %1]") );
    QCOMPARE( s8.arg("foo").arg("bar"), QString("[foo bar]") );
    QCOMPARE( s8.arg("foo", "bar"), QString("[foo bar]") );
    QCOMPARE( s9.arg("foo"), QString("[foo %3]") );
    QCOMPARE( s9.arg("foo").arg("bar"), QString("[foo bar]") );
    QCOMPARE( s9.arg("foo", "bar"), QString("[foo bar]") );
    QCOMPARE( s10.arg("foo"), QString("[foo %2 %3]") );
    QCOMPARE( s10.arg("foo").arg("bar"), QString("[foo bar %3]") );
    QCOMPARE( s10.arg("foo", "bar"), QString("[foo bar %3]") );
    QCOMPARE( s10.arg("foo").arg("bar").arg("baz"), QString("[foo bar baz]") );
    QCOMPARE( s10.arg("foo", "bar", "baz"), QString("[foo bar baz]") );
    QCOMPARE( s11.arg("foo"), QString("[%9 %3 foo]") );
    QCOMPARE( s11.arg("foo").arg("bar"), QString("[%9 bar foo]") );
    QCOMPARE( s11.arg("foo", "bar"), QString("[%9 bar foo]") );
    QCOMPARE( s11.arg("foo").arg("bar").arg("baz"), QString("[baz bar foo]") );
    QCOMPARE( s11.arg("foo", "bar", "baz"), QString("[baz bar foo]") );
    QCOMPARE( s12.arg("a").arg("b").arg("c").arg("d").arg("e"),
             QString("[e b c e a d]") );
    QCOMPARE( s12.arg("a", "b", "c", "d").arg("e"), QString("[e b c e a d]") );
    QCOMPARE( s12.arg("a").arg("b", "c", "d", "e"), QString("[e b c e a d]") );
    QCOMPARE( s13.arg("alpha").arg("beta"),
             QString("alpha% %x%cbeta %dbeta-%") );
    QCOMPARE( s13.arg("alpha", "beta"), QString("alpha% %x%cbeta %dbeta-%") );
    QCOMPARE( s14.arg("a", "b", "c"), QString("abc") );
    QCOMPARE( s8.arg("%1").arg("foo"), QString("[foo foo]") );
    QCOMPARE( s8.arg("%1", "foo"), QString("[%1 foo]") );
    QCOMPARE( s4.arg("foo", 2), QString("[foo]") );
    QCOMPARE( s4.arg("foo", -2), QString("[foo]") );
    QCOMPARE( s4.arg("foo", 10), QString("[       foo]") );
    QCOMPARE( s4.arg("foo", -10), QString("[foo       ]") );

    QString firstName( "James" );
    QString lastName( "Bond" );
    QString fullName = QString( "My name is %2, %1 %2" )
		       .arg( firstName ).arg( lastName );
    QCOMPARE( fullName, QString("My name is Bond, James Bond") );

    // number overloads
    QCOMPARE( s4.arg(0), QString("[0]") );
    QCOMPARE( s4.arg(-1), QString("[-1]") );
    QCOMPARE( s4.arg(4294967295UL), QString("[4294967295]") ); // ULONG_MAX 32
    QCOMPARE( s4.arg(Q_INT64_C(9223372036854775807)), // LLONG_MAX
	     QString("[9223372036854775807]") );

    QCOMPARE( QString().arg(0), QString() );
    QCOMPARE( QString("").arg(0), QString("") );
    QCOMPARE( QString(" ").arg(0), QString(" ") );
    QCOMPARE( QString("%").arg(0), QString("%") );
    QCOMPARE( QString("%%").arg(0), QString("%%") );
    QCOMPARE( QString("%%%").arg(0), QString("%%%") );
    QCOMPARE( QString("%%%1%%%2").arg("foo").arg("bar"), QString("%%foo%%bar") );

    QCOMPARE( QString("%1").arg("hello", -10), QString("hello     ") );
    QCOMPARE( QString("%1").arg("hello", -5), QString("hello") );
    QCOMPARE( QString("%1").arg("hello", -2), QString("hello") );
    QCOMPARE( QString("%1").arg("hello", 0), QString("hello") );
    QCOMPARE( QString("%1").arg("hello", 2), QString("hello") );
    QCOMPARE( QString("%1").arg("hello", 5), QString("hello") );
    QCOMPARE( QString("%1").arg("hello", 10), QString("     hello") );
    QCOMPARE( QString("%1%1").arg("hello"), QString("hellohello") );
    QCOMPARE( QString("%2%1").arg("hello"), QString("%2hello") );
    QCOMPARE( QString("%1%1").arg(QString::null), QString("") );
    QCOMPARE( QString("%2%1").arg(""), QString("%2") );

    QCOMPARE( QString("%2 %L1").arg(12345.6789).arg(12345.6789),
             QString("12345.7 12.345,7") );
    QCOMPARE( QString("%2 %L1").arg(123456789).arg(123456789),
             QString("123456789 123.456.789") );
    QCOMPARE( QString("%L2 %L1 %3").arg(12345.7).arg(123456789).arg('c'),
             QString("123.456.789 12.345,7 c") );

    // multi-digit replacement
    QString input("%%%L0 %1 %02 %3 %4 %5 %L6 %7 %8 %%% %090 %10 %11 %L12 %14 %L9888 %9999 %%%%%%%L");
    input = input.arg("A").arg("B").arg("C")
                 .arg("D").arg("E").arg("f")
                 .arg("g").arg("h").arg("i").arg("j")
                 .arg("k").arg("l").arg("m")
                 .arg("n").arg("o").arg("p");

    QCOMPARE(input, QString("%%A B C D E f g h i %%% j0 k l m n o88 p99 %%%%%%%L"));

    QString str("%1 %2 %3 %4 %5 %6 %7 %8 %9 foo %10 %11 bar");
    str = str.arg("one", "2", "3", "4", "5", "6", "7", "8", "9");
    str = str.arg("ahoy", "there");
    QCOMPARE(str, QString("one 2 3 4 5 6 7 8 9 foo ahoy there bar"));

    QString str2("%123 %234 %345 %456 %567 %999 %1000 %1230");
    str2 = str2.arg("A", "B", "C", "D", "E", "F");
    QCOMPARE(str2, QString("A B C D E F %1000 %1230"));

    // task 201185
    QCOMPARE(QString("%1").arg(-1, 3, 10, QChar('0')), QString("-01"));
    QCOMPARE(QString("%1").arg(-100, 3, 10, QChar('0')), QString("-100"));
    QCOMPARE(QString("%1").arg(-1, 3, 10, QChar(' ')), QString(" -1"));
    QCOMPARE(QString("%1").arg(-100, 3, 10, QChar(' ')), QString("-100"));
    QCOMPARE(QString("%1").arg(1U, 3, 10, QChar(' ')), QString("  1"));
    QCOMPARE(QString("%1").arg(1000U, 3, 10, QChar(' ')), QString("1000"));
    QCOMPARE(QString("%1").arg(-1, 3, 10, QChar('x')), QString("x-1"));
    QCOMPARE(QString("%1").arg(-100, 3, 10, QChar('x')), QString("-100"));
    QCOMPARE(QString("%1").arg(1U, 3, 10, QChar('x')), QString("xx1"));
    QCOMPARE(QString("%1").arg(1000U, 3, 10, QChar('x')), QString("1000"));

    QCOMPARE(QString("%1").arg(-1., 3, 'g', -1, QChar('0')), QString("-01"));
    QCOMPARE(QString("%1").arg(-100., 3, 'g', -1, QChar('0')), QString("-100"));
    QCOMPARE(QString("%1").arg(-1., 3, 'g', -1, QChar(' ')), QString(" -1"));
    QCOMPARE(QString("%1").arg(-100., 3, 'g', -1, QChar(' ')), QString("-100"));
    QCOMPARE(QString("%1").arg(1., 3, 'g', -1, QChar('x')), QString("xx1"));
    QCOMPARE(QString("%1").arg(1000., 3, 'g', -1, QChar('x')), QString("1000"));
    QCOMPARE(QString("%1").arg(-1., 3, 'g', -1, QChar('x')), QString("x-1"));
    QCOMPARE(QString("%1").arg(-100., 3, 'g', -1, QChar('x')), QString("-100"));
}

void tst_QString::number()
{
    QCOMPARE( QString::number(int(0)), QString("0") );
    QCOMPARE( QString::number((unsigned int)(11)), QString("11") );
    QCOMPARE( QString::number(-22L), QString("-22") );
    QCOMPARE( QString::number(333UL), QString("333") );
    QCOMPARE( QString::number(4.4), QString("4.4") );
    QCOMPARE( QString::number(Q_INT64_C(-555)), QString("-555") );
    QCOMPARE( QString::number(Q_UINT64_C(6666)), QString("6666") );
}

void tst_QString::capacity_data()
{
    length_data();
}

void tst_QString::capacity()
{
    QFETCH( QString, s1 );
    QFETCH( int, res );

    QString s2( s1 );
    s2.reserve( res );
    QVERIFY( (int)s2.capacity() >= res );
    QCOMPARE( s2, s1 );

    s2.reserve( res * 2 );
    QVERIFY( (int)s2.capacity() >=  res * 2 );
    QCOMPARE( s2, s1 );

    s2.squeeze();
    QVERIFY( (int)s2.capacity() == res );
    QCOMPARE( s2, s1 );

}

void tst_QString::section_data()
{
    QTest::addColumn<QString>("wholeString" );
    QTest::addColumn<QString>("sep" );
    QTest::addColumn<int>("start" );
    QTest::addColumn<int>("end" );
    QTest::addColumn<int>("flags" );
    QTest::addColumn<QString>("sectionString" );
    QTest::addColumn<bool>("regexp" );

    QTest::newRow( "data0" ) << QString("forename,middlename,surname,phone") << QString(",") << 2 << 2 << int(QString::SectionDefault) << QString("surname") << false;
    QTest::newRow( "data1" ) << QString("/usr/local/bin/myapp") << QString("/") << 3 << 4 << int(QString::SectionDefault) << QString("bin/myapp") << false;
    QTest::newRow( "data2" ) << QString("/usr/local/bin/myapp") << QString("/") << 3 << 3 << int(QString::SectionSkipEmpty) << QString("myapp") << false;
    QTest::newRow( "data3" ) << QString("forename**middlename**surname**phone") << QString("**") << 2 << 2 << int(QString::SectionDefault) << QString("surname") << false;
    QTest::newRow( "data4" ) << QString("forename**middlename**surname**phone") << QString("**") << -3 << -2 << int(QString::SectionDefault) << QString("middlename**surname") << false;
    QTest::newRow( "task-21749-1" ) << QString("##Datt######wollen######wir######mal######sehen##") << QString("#") << 0 << 0 << int(QString::SectionSkipEmpty) << QString("Datt") << false;
    QTest::newRow( "task-21749-2" ) << QString("##Datt######wollen######wir######mal######sehen##") << QString("#") << 1 << 1 << int(QString::SectionSkipEmpty) << QString("wollen") << false;
    QTest::newRow( "task-21749-3" ) << QString("##Datt######wollen######wir######mal######sehen##") << QString("#") << 2 << 2 << int(QString::SectionSkipEmpty) << QString("wir") << false;
    QTest::newRow( "task-21749-4" ) << QString("##Datt######wollen######wir######mal######sehen##") << QString("#") << 3 << 3 << int(QString::SectionSkipEmpty) << QString("mal") << false;
    QTest::newRow( "task-21749-5" ) << QString("##Datt######wollen######wir######mal######sehen##") << QString("#") << 4 << 4 << int(QString::SectionSkipEmpty) << QString("sehen") << false;
    // not fixed for 3.1
    QTest::newRow( "task-27269" ) << QString("a/b/c/d") << QString("/") << 1 << -1 << int(QString::SectionIncludeLeadingSep | QString::SectionIncludeTrailingSep) << QString("/b/c/d") << false;
    QTest::newRow( "task-43641" ) << QString("aoLoboLocolod") << QString("olo") << -1 << -1 << int(QString::SectionCaseInsensitiveSeps) << QString("d") << false;
    QTest::newRow( "task-27593" ) << QString("F0") << QString("F") << 0 << 0 << int(QString::SectionSkipEmpty) << QString("0") << false;
    QTest::newRow( "foo1" ) << QString("foo;foo;") << QString(";") << 0 << 0
                         << int(QString::SectionIncludeLeadingSep) << QString("foo") << false;
    QTest::newRow( "foo2" ) << QString("foo;foo;") << QString(";") << 1 << 1
                         << int(QString::SectionIncludeLeadingSep) << QString(";foo") << false;
    QTest::newRow( "foo3" ) << QString("foo;foo;") << QString(";") << 2 << 2
                         << int(QString::SectionIncludeLeadingSep) << QString(";") << false;
    QTest::newRow( "foo1rx" ) << QString("foo;foo;") << QString(";") << 0 << 0
                           << int(QString::SectionIncludeLeadingSep) << QString("foo") << true;
    QTest::newRow( "foo2rx" ) << QString("foo;foo;") << QString(";") << 1 << 1
                           << int(QString::SectionIncludeLeadingSep) << QString(";foo") << true;
    QTest::newRow( "foo3rx" ) << QString("foo;foo;") << QString(";") << 2 << 2
                           << int(QString::SectionIncludeLeadingSep) << QString(";") << true;

    QTest::newRow( "qmake_path" ) << QString("/Users/sam/troll/qt4.0/src/corelib/QtCore_debug.xcode/")
                               << QString("/") << 0 << -2 << int(QString::SectionDefault)
                               << QString("/Users/sam/troll/qt4.0/src/corelib/QtCore_debug.xcode") << false;
    QTest::newRow( "qmake_pathrx" ) << QString("/Users/sam/troll/qt4.0/src/corelib/QtCore_debug.xcode/")
                                 << QString("/") << 0 << -2 << int(QString::SectionDefault)
                                 << QString("/Users/sam/troll/qt4.0/src/corelib/QtCore_debug.xcode") << true;
    QTest::newRow( "task72972" ) << QString("||2|3|||")
                              << QString("|") << 0 << 1 << int(QString::SectionIncludeLeadingSep|QString::SectionIncludeTrailingSep)
                              << QString("||") << false;
    QTest::newRow( "task72972rx" ) << QString("||2|3|||")
                                << QString("\\|") << 0 << 1 << int(QString::SectionIncludeLeadingSep|QString::SectionIncludeTrailingSep)
                                << QString("||") << true;
    QTest::newRow( "task72972-2" ) << QString("|1|2|")
                                << QString("|") << 0 << 1 << int(QString::SectionIncludeLeadingSep|QString::SectionIncludeTrailingSep)
                                << QString("|1|") << false;
    QTest::newRow( "task72972-2rx" ) << QString("|1|2|")
                                  << QString("\\|") << 0 << 1
                                  << int(QString::SectionIncludeLeadingSep|QString::SectionIncludeTrailingSep)
                                  << QString("|1|") << true;
    QTest::newRow( "normal1" ) << QString("o1o2o")
                            << QString("o") << 0 << 0
                            << int(QString::SectionIncludeLeadingSep|QString::SectionIncludeTrailingSep)
                            << QString("o") << false;
    QTest::newRow( "normal2" ) << QString("o1o2o")
                            << QString("o") << 1 << 1
                            << int(QString::SectionIncludeLeadingSep|QString::SectionIncludeTrailingSep)
                            << QString("o1o") << false;
    QTest::newRow( "normal3" ) << QString("o1o2o")
                            << QString("o") << 2 << 2
                            << int(QString::SectionIncludeLeadingSep|QString::SectionIncludeTrailingSep)
                            << QString("o2o") << false;
    QTest::newRow( "normal4" ) << QString("o1o2o")
                            << QString("o") << 2 << 3
                            << int(QString::SectionIncludeLeadingSep|QString::SectionIncludeTrailingSep)
                            << QString("o2o") << false;
    QTest::newRow( "normal5" ) << QString("o1o2o")
                            << QString("o") << 1 << 2
                            << int(QString::SectionIncludeLeadingSep|QString::SectionIncludeTrailingSep)
                            << QString("o1o2o") << false;
    QTest::newRow( "rx1" ) << QString("o1o2o")
                        << QString("[a-z]") << 0 << 0
                        << int(QString::SectionIncludeLeadingSep|QString::SectionIncludeTrailingSep)
                        << QString("o") << true;
    QTest::newRow( "rx2" ) << QString("o1o2o")
                        << QString("[a-z]") << 1 << 1
                        << int(QString::SectionIncludeLeadingSep|QString::SectionIncludeTrailingSep)
                        << QString("o1o") << true;
    QTest::newRow( "rx3" ) << QString("o1o2o")
                        << QString("[a-z]") << 2 << 2
                        << int(QString::SectionIncludeLeadingSep|QString::SectionIncludeTrailingSep)
                        << QString("o2o") << true;
    QTest::newRow( "rx4" ) << QString("o1o2o")
                        << QString("[a-z]") << 2 << 3
                        << int(QString::SectionIncludeLeadingSep|QString::SectionIncludeTrailingSep)
                        << QString("o2o") << true;
    QTest::newRow( "rx5" ) << QString("o1o2o")
                        << QString("[a-z]") << 1 << 2
                        << int(QString::SectionIncludeLeadingSep|QString::SectionIncludeTrailingSep)
                        << QString("o1o2o") << true;
    QTest::newRow( "task45855-rx" ) << QString("This is a story, a small story")
                        << QString("\\b") << 3 << 3
                        << int(QString::SectionDefault)
                        << QString("is") << true;
    QTest::newRow( "task257941-rx" ) << QString("99.0 42.3")
                        << QString("\\s*[AaBb]\\s*") << 1 << 1
                        << int(QString::SectionIncludeLeadingSep)
                        << QString() << true;
}

void tst_QString::section()
{
    QFETCH( QString, wholeString );
    QFETCH( QString, sep );
    QFETCH( int, start );
    QFETCH( int, end );
    QFETCH( int, flags );
    QFETCH( QString, sectionString );
    QFETCH( bool, regexp );
    if (regexp) {
        QCOMPARE( wholeString.section( QRegExp(sep), start, end, QString::SectionFlag(flags) ), sectionString );
    } else {
        QCOMPARE( wholeString.section( sep, start, end, QString::SectionFlag(flags) ), sectionString );
    }
}


void tst_QString::operator_eqeq_nullstring()
{
    /* Some of these might not be all that logical but it's the behaviour we've had since 3.0.0
       so we should probably stick with it. */

    QVERIFY( QString() == "" );
    QVERIFY( "" == QString() );

    QVERIFY( QString("") == "" );
    QVERIFY( "" == QString("") );

    QVERIFY( QString().size() == 0 );

    QVERIFY( QString("").size() == 0 );

    QVERIFY( QString() == QString("") );
    QVERIFY( QString("") == QString() );
}

void tst_QString::operator_smaller()
{
    QString null;
    QString empty("");
    QString foo("foo");

    QVERIFY( !(null < QString()) );
    QVERIFY( !(null > QString()) );

    QVERIFY( !(empty < QString("")) );
    QVERIFY( !(empty > QString("")) );

    QVERIFY( !(null < empty) );
    QVERIFY( !(null > empty) );

    QVERIFY( null < foo );
    QVERIFY( !(null > foo) );
    QVERIFY( foo > null );
    QVERIFY( !(foo < null) );

    QVERIFY( empty < foo );
    QVERIFY( !(empty > foo) );
    QVERIFY( foo > empty );
    QVERIFY( !(foo < empty) );

    QVERIFY( !(null < QLatin1String(0)) );
    QVERIFY( !(null > QLatin1String(0)) );
    QVERIFY( !(null < QLatin1String("")) );
    QVERIFY( !(null > QLatin1String("")) );

    QVERIFY( !(null < QLatin1String("")) );
    QVERIFY( !(null > QLatin1String("")) );
    QVERIFY( !(empty < QLatin1String("")) );
    QVERIFY( !(empty > QLatin1String("")) );

    QVERIFY( !(QLatin1String(0) < null) );
    QVERIFY( !(QLatin1String(0) > null) );
    QVERIFY( !(QLatin1String("") < null) );
    QVERIFY( !(QLatin1String("") > null) );

    QVERIFY( !(QLatin1String(0) < empty) );
    QVERIFY( !(QLatin1String(0) > empty) );
    QVERIFY( !(QLatin1String("") < empty) );
    QVERIFY( !(QLatin1String("") > empty) );

    QVERIFY( QLatin1String(0) < foo );
    QVERIFY( !(QLatin1String(0) > foo) );
    QVERIFY( QLatin1String("") < foo );
    QVERIFY( !(QLatin1String("") > foo) );

    QVERIFY( foo > QLatin1String(0) );
    QVERIFY( !(foo < QLatin1String(0)) );
    QVERIFY( foo > QLatin1String("") );
    QVERIFY( !(foo < QLatin1String("")) );

    QVERIFY( QLatin1String(0) == empty);
    QVERIFY( QLatin1String(0) == null);
    QVERIFY( QLatin1String("") == empty);
    QVERIFY( QLatin1String("") == null);

    QVERIFY( !(foo < QLatin1String("foo")));
    QVERIFY( !(foo > QLatin1String("foo")));
    QVERIFY( !(QLatin1String("foo") < foo));
    QVERIFY( !(QLatin1String("foo") > foo));

    QVERIFY( !(foo < QLatin1String("a")));
    QVERIFY( (foo > QLatin1String("a")));
    QVERIFY( (QLatin1String("a") < foo));
    QVERIFY( !(QLatin1String("a") > foo));

    QVERIFY( (foo < QLatin1String("z")));
    QVERIFY( !(foo > QLatin1String("z")));
    QVERIFY( !(QLatin1String("z") < foo));
    QVERIFY( (QLatin1String("z") > foo));
}

void tst_QString::integer_conversion_data()
{
    QTest::addColumn<QString>("locale_name");
    QTest::addColumn<QString>("num_str");
    QTest::addColumn<int>("base");
    QTest::addColumn<bool>("good");
    QTest::addColumn<qlonglong>("num");

    QTest::newRow("C empty 0")    << QString("C")  << QString("")    << 0   << false << (qlonglong)0;
    QTest::newRow("C empty 8")    << QString("C")  << QString("")    << 8   << false << (qlonglong)0;
    QTest::newRow("C empty 10")   << QString("C")  << QString("")    << 10  << false << (qlonglong)0;
    QTest::newRow("C empty 16")   << QString("C")  << QString("")    << 16  << false << (qlonglong)0;

    QTest::newRow("C null 0")     << QString("C")  << QString()  << 0   << false << (qlonglong)0;
    QTest::newRow("C null 8")     << QString("C")  << QString()  << 8   << false << (qlonglong)0;
    QTest::newRow("C null 10")    << QString("C")  << QString()  << 10  << false << (qlonglong)0;
    QTest::newRow("C null 16")    << QString("C")  << QString()  << 16  << false << (qlonglong)0;

    QTest::newRow("C   -0xf 0")       << QString("C")  << QString("  -0xf")    << 0  << true  << (qlonglong)-15;
    QTest::newRow("C -0xf   0")       << QString("C")  << QString("-0xf  ")    << 0  << true  << (qlonglong)-15;
    QTest::newRow("C \t0xf\t 0")      << QString("C")  << QString("\t0xf\t")   << 0  << true  << (qlonglong)15;
    QTest::newRow("C   -010 0")       << QString("C")  << QString("  -010")    << 0  << true  << (qlonglong)-8;
    QTest::newRow("C 010   0")        << QString("C")  << QString("010  ")     << 0  << true  << (qlonglong)8;
    QTest::newRow("C \t-010\t 0")     << QString("C")  << QString("\t-010\t")  << 0  << true  << (qlonglong)-8;
    QTest::newRow("C   123 10")       << QString("C")  << QString("  123")     << 10 << true  << (qlonglong)123;
    QTest::newRow("C 123   10")       << QString("C")  << QString("123  ")     << 10 << true  << (qlonglong)123;
    QTest::newRow("C \t123\t 10")     << QString("C")  << QString("\t123\t")   << 10 << true  << (qlonglong)123;
    QTest::newRow("C   -0xf 16")      << QString("C")  << QString("  -0xf")    << 16 << true  << (qlonglong)-15;
    QTest::newRow("C -0xf   16")      << QString("C")  << QString("-0xf  ")    << 16 << true  << (qlonglong)-15;
    QTest::newRow("C \t0xf\t 16")     << QString("C")  << QString("\t0xf\t")   << 16 << true  << (qlonglong)15;

    QTest::newRow("C -0 0")       << QString("C")  << QString("-0")  << 0   << true  << (qlonglong)0;
    QTest::newRow("C -0 8")       << QString("C")  << QString("-0")  << 8   << true  << (qlonglong)0;
    QTest::newRow("C -0 10")      << QString("C")  << QString("-0")  << 10  << true  << (qlonglong)0;
    QTest::newRow("C -0 16")      << QString("C")  << QString("-0")  << 16  << true  << (qlonglong)0;

    QTest::newRow("C 1.234 10")      << QString("C")     << QString("1.234") << 10 << false << (qlonglong)0;
    QTest::newRow("C 1,234 10")      << QString("C")     << QString("1,234") << 10 << false << (qlonglong)0;
    QTest::newRow("de_DE 1.234 10")  << QString("de_DE") << QString("1.234") << 10 << false << (qlonglong)0;
    QTest::newRow("de_DE 1,234 10")  << QString("de_DE") << QString("1,234") << 10 << false << (qlonglong)0;

    QTest::newRow("C 0x 0")    << QString("C")  << QString("0x") << 0  << false << (qlonglong)0;
    QTest::newRow("C 0x 16")   << QString("C")  << QString("0x") << 16 << false << (qlonglong)0;

    QTest::newRow("C 10 0")      << QString("C") << QString("10")    << 0  << true  << (qlonglong)10;
    QTest::newRow("C 010 0")     << QString("C") << QString("010")   << 0  << true  << (qlonglong)8;
    QTest::newRow("C 0x10 0")    << QString("C") << QString("0x10")  << 0  << true  << (qlonglong)16;
    QTest::newRow("C 10 8")      << QString("C") << QString("10")    << 8  << true  << (qlonglong)8;
    QTest::newRow("C 010 8")     << QString("C") << QString("010")   << 8  << true  << (qlonglong)8;
    QTest::newRow("C 0x10 8")    << QString("C") << QString("0x10")  << 8  << false << (qlonglong)0;
    QTest::newRow("C 10 10")     << QString("C") << QString("10")    << 10 << true  << (qlonglong)10;
    QTest::newRow("C 010 10")    << QString("C") << QString("010")   << 10 << true  << (qlonglong)10;
    QTest::newRow("C 0x10 10")   << QString("C") << QString("0x10")  << 10 << false << (qlonglong)0;
    QTest::newRow("C 10 16")     << QString("C") << QString("10")    << 16 << true  << (qlonglong)16;
    QTest::newRow("C 010 16")    << QString("C") << QString("010")   << 16 << true  << (qlonglong)16;
    QTest::newRow("C 0x10 16")   << QString("C") << QString("0x10")  << 16 << true  << (qlonglong)16;

    QTest::newRow("de_DE 10 0")      << QString("de_DE") << QString("10")    << 0  << true  << (qlonglong)10;
    QTest::newRow("de_DE 010 0")     << QString("de_DE") << QString("010")   << 0  << true  << (qlonglong)8;
    QTest::newRow("de_DE 0x10 0")    << QString("de_DE") << QString("0x10")  << 0  << true  << (qlonglong)16;
    QTest::newRow("de_DE 10 8")      << QString("de_DE") << QString("10")    << 8  << true  << (qlonglong)8;
    QTest::newRow("de_DE 010 8")     << QString("de_DE") << QString("010")   << 8  << true  << (qlonglong)8;
    QTest::newRow("de_DE 0x10 8")    << QString("de_DE") << QString("0x10")  << 8  << false << (qlonglong)0;
    QTest::newRow("de_DE 10 10")     << QString("de_DE") << QString("10")    << 10 << true  << (qlonglong)10;
    QTest::newRow("de_DE 010 10")    << QString("de_DE") << QString("010")   << 10 << true  << (qlonglong)10;
    QTest::newRow("de_DE 0x10 10")   << QString("de_DE") << QString("0x10")  << 10 << false << (qlonglong)0;
    QTest::newRow("de_DE 10 16")     << QString("de_DE") << QString("10")    << 16 << true  << (qlonglong)16;
    QTest::newRow("de_DE 010 16")    << QString("de_DE") << QString("010")   << 16 << true  << (qlonglong)16;
    QTest::newRow("de_DE 0x10 16")   << QString("de_DE") << QString("0x10")  << 16 << true  << (qlonglong)16;

    QTest::newRow("C -10 0")      << QString("C") << QString("-10")    << 0  << true  << (qlonglong)-10;
    QTest::newRow("C -010 0")     << QString("C") << QString("-010")   << 0  << true  << (qlonglong)-8;
    QTest::newRow("C -0x10 0")    << QString("C") << QString("-0x10")  << 0  << true  << (qlonglong)-16;
    QTest::newRow("C -10 8")      << QString("C") << QString("-10")    << 8  << true  << (qlonglong)-8;
    QTest::newRow("C -010 8")     << QString("C") << QString("-010")   << 8  << true  << (qlonglong)-8;
    QTest::newRow("C -0x10 8")    << QString("C") << QString("-0x10")  << 8  << false << (qlonglong)0;
    QTest::newRow("C -10 10")     << QString("C") << QString("-10")    << 10 << true  << (qlonglong)-10;
    QTest::newRow("C -010 10")    << QString("C") << QString("-010")   << 10 << true  << (qlonglong)-10;
    QTest::newRow("C -0x10 10")   << QString("C") << QString("-0x10")  << 10 << false << (qlonglong)0;
    QTest::newRow("C -10 16")     << QString("C") << QString("-10")    << 16 << true  << (qlonglong)-16;
    QTest::newRow("C -010 16")    << QString("C") << QString("-010")   << 16 << true  << (qlonglong)-16;
    QTest::newRow("C -0x10 16")   << QString("C") << QString("-0x10")  << 16 << true  << (qlonglong)-16;

    // Let's try some Arabic
    const quint16 arabic_str[] = { 0x0661, 0x0662, 0x0663, 0x0664, 0x0000 }; // "1234"
    QTest::newRow("ar_SA 1234 0")  << QString("ar_SA") << QString::fromUtf16(arabic_str)  << 0  << true << (qlonglong)1234;
    QTest::newRow("ar_SA 1234 8")  << QString("ar_SA") << QString::fromUtf16(arabic_str)  << 8  << true << (qlonglong)668;
    QTest::newRow("ar_SA 1234 10") << QString("ar_SA") << QString::fromUtf16(arabic_str)  << 10 << true << (qlonglong)1234;
    QTest::newRow("ar_SA 1234 16") << QString("ar_SA") << QString::fromUtf16(arabic_str)  << 16 << true << (qlonglong)4660;
}

void tst_QString::integer_conversion()
{
    QFETCH(QString, locale_name);
    QFETCH(QString, num_str);
    QFETCH(int, base);
    QFETCH(bool, good);
    QFETCH(qlonglong, num);


    QLocale::setDefault(locale_name);
    QCOMPARE(QLocale().name(), locale_name);

    bool ok;
    qlonglong d = num_str.toLongLong(&ok, base);
    QCOMPARE(ok, good);

    if (ok) {
	QCOMPARE(d, num);
    }

    QLocale::setDefault(QLocale::C);
}

void tst_QString::double_conversion_data()
{
    QTest::addColumn<QString>("locale_name");
    QTest::addColumn<QString>("num_str");
    QTest::addColumn<bool>("good");
    QTest::addColumn<double>("num");
/*
    // The good...

    QTest::newRow("C 1")         << QString("C") << QString("1")          << true  << 1.0;
    QTest::newRow("C 1.0")       << QString("C") << QString("1.0")        << true  << 1.0;
    QTest::newRow("C 1.234")     << QString("C") << QString("1.234")      << true  << 1.234;
    QTest::newRow("C 1.234e-10") << QString("C") << QString("1.234e-10")  << true  << 1.234e-10;
    QTest::newRow("C 1.234E10")  << QString("C") << QString("1.234E10")   << true  << 1.234e10;
    QTest::newRow("C 1e10")      << QString("C") << QString("1e10")       << true  << 1.0e10;

    // The bad...

    QTest::newRow("C empty")     << QString("C") << QString("")           << false << 0.0;
    QTest::newRow("C null")      << QString("C") << QString()         << false << 0.0;
    QTest::newRow("C .")         << QString("C") << QString(".")          << false << 0.0;
    QTest::newRow("C 1e")        << QString("C") << QString("1e")         << false << 0.0;
    QTest::newRow("C 1,")        << QString("C") << QString("1,")         << false << 0.0;
    QTest::newRow("C 1,0")       << QString("C") << QString("1,0")        << false << 0.0;
    QTest::newRow("C 1,000")     << QString("C") << QString("1,000")      << false << 0.0;
    QTest::newRow("C 1e1.0")     << QString("C") << QString("1e1.0")      << false << 0.0;
    QTest::newRow("C 1e+")       << QString("C") << QString("1e+")        << false << 0.0;
    QTest::newRow("C 1e-")       << QString("C") << QString("1e-")        << false << 0.0;

    // And the ugly...

    QTest::newRow("C .1")        << QString("C") << QString(".1")         << true  << 0.1;
    QTest::newRow("C -.1")       << QString("C") << QString("-.1")        << true  << -0.1;
    QTest::newRow("C 1.")        << QString("C") << QString("1.")         << true  << 1.0;
    QTest::newRow("C 1.E10")     << QString("C") << QString("1.E10")      << true  << 1.0e10;
    QTest::newRow("C 1e+10")     << QString("C") << QString("1e+10")      << true  << 1.0e+10;

    QTest::newRow("de_DE 1")         << QString("de_DE") << QString("1")          << true  << 1.0;
    QTest::newRow("de_DE 1.0")       << QString("de_DE") << QString("1.0")        << true  << 1.0;
    QTest::newRow("de_DE 1.234")     << QString("de_DE") << QString("1.234")      << true  << 1.234;
    QTest::newRow("de_DE 1.234e-10") << QString("de_DE") << QString("1.234e-10")  << true  << 1.234e-10;
    QTest::newRow("de_DE 1.234E10")  << QString("de_DE") << QString("1.234E10")   << true  << 1.234e10;
    QTest::newRow("de_DE 1e10")      << QString("de_DE") << QString("1e10")       << true  << 1.0e10;
    QTest::newRow("de_DE .1")        << QString("de_DE") << QString(".1")         << true  << 0.1;
    QTest::newRow("de_DE -.1")       << QString("de_DE") << QString("-.1")        << true  << -0.1;
    QTest::newRow("de_DE 1.")        << QString("de_DE") << QString("1.")         << true  << 1.0;
    QTest::newRow("de_DE 1.E10")     << QString("de_DE") << QString("1.E10")      << true  << 1.0e10;
    QTest::newRow("de_DE 1e+10")     << QString("de_DE") << QString("1e+10")      << true  << 1.0e+10;

    QTest::newRow("de_DE 1,0")       << QString("de_DE") << QString("1,0")        << true  << 1.0;
    QTest::newRow("de_DE 1,234")     << QString("de_DE") << QString("1,234")      << true  << 1.234;
    QTest::newRow("de_DE 1,234e-10") << QString("de_DE") << QString("1,234e-10")  << true  << 1.234e-10;
    QTest::newRow("de_DE 1,234E10")  << QString("de_DE") << QString("1,234E10")   << true  << 1.234e10;
    QTest::newRow("de_DE ,1")        << QString("de_DE") << QString(",1")         << true  << 0.1;
    QTest::newRow("de_DE -,1")       << QString("de_DE") << QString("-,1")        << true  << -0.1;
    QTest::newRow("de_DE 1,")        << QString("de_DE") << QString("1,")         << true  << 1.0;
    QTest::newRow("de_DE 1,E10")     << QString("de_DE") << QString("1,E10")      << true  << 1.0e10;

    QTest::newRow("de_DE empty")     << QString("de_DE") << QString("")           << false << 0.0;
    QTest::newRow("de_DE null")      << QString("de_DE") << QString()         << false << 0.0;
    QTest::newRow("de_DE .")         << QString("de_DE") << QString(".")          << false << 0.0;
    QTest::newRow("de_DE 1e")        << QString("de_DE") << QString("1e")         << false << 0.0;
    QTest::newRow("de_DE 1e1.0")     << QString("de_DE") << QString("1e1.0")      << false << 0.0;
    QTest::newRow("de_DE 1e+")       << QString("de_DE") << QString("1e+")        << false << 0.0;
    QTest::newRow("de_DE 1e-")       << QString("de_DE") << QString("1e-")        << false << 0.0;

    QTest::newRow("C   1")           << QString("C")     << QString("  1")        << true  << 1.0;
    QTest::newRow("C 1  ")           << QString("C")     << QString("1  ")        << true  << 1.0;
    QTest::newRow("de_DE  1")        << QString("de_DE") << QString(" 1")         << true  << 1.0;
    QTest::newRow("de_DE 1 ")        << QString("de_DE") << QString("1 ")         << true  << 1.0;
*/
    // Let's try some Arabic
    const quint16 arabic_str[] = { 0x0660, 0x066B, 0x0661, 0x0662,
				    0x0663, 0x0664, 0x0065, 0x0662,
				    0x0000 };			    // "0.1234e2"
    QTest::newRow("ar_SA") << QString("ar_SA") << QString::fromUtf16(arabic_str) << true << 0.1234e2;
}

void tst_QString::double_conversion()
{
#define MY_DOUBLE_EPSILON (2.22045e-16)

    QFETCH(QString, locale_name);
    QFETCH(QString, num_str);
    QFETCH(bool, good);
    QFETCH(double, num);

    QLocale::setDefault(locale_name);
    QCOMPARE(QLocale().name(), locale_name);

    bool ok;
    double d = num_str.toDouble(&ok);
    QCOMPARE(ok, good);

    if (ok) {
	double diff = d - num;
	if (diff < 0)
	    diff = -diff;
	QVERIFY(diff <= MY_DOUBLE_EPSILON);
    }

    QLocale::setDefault(QLocale::C);
}

#ifndef Q_MOC_RUN
#include "double_data.h"
#endif

void tst_QString::tortureSprintfDouble()
{
    const SprintfDoubleData *data = g_sprintf_double_data;

    QString s;

    for (; data->fmt != 0; ++data) {
	double d;
	char *buff = (char *)&d;
#	ifndef Q_BYTE_ORDER
#	    error "Q_BYTE_ORDER not defined"
#	endif

#       ifdef QT_ARMFPA
        buff[0] = data->bytes[4];
        buff[1] = data->bytes[5];
        buff[2] = data->bytes[6];
        buff[3] = data->bytes[7];
        buff[4] = data->bytes[0];
        buff[5] = data->bytes[1];
        buff[6] = data->bytes[2];
        buff[7] = data->bytes[3];
#	elif Q_BYTE_ORDER == Q_LITTLE_ENDIAN
	for (uint i = 0; i < 8; ++i)
	    buff[i] = data->bytes[i];
#	else
	for (uint i = 0; i < 8; ++i)
	    buff[7 - i] = data->bytes[i];
#	endif
	s.sprintf(data->fmt, d);
#ifdef QT_NO_FPU // reduced precision when running with hardfloats in qemu
	if (d - 0.1 < 1e12)
	  QSKIP("clib sprintf doesn't fill with 0's on this platform",
		SkipSingle);
	QCOMPARE(s.left(16), QString(data->expected).left(16));
#else
	QCOMPARE(s, QString(data->expected));
#endif
    }
}

#include <locale.h>

void tst_QString::localeAwareCompare_data()
{
#ifdef Q_OS_WIN
    QTest::addColumn<ulong>("locale");
#else
    QTest::addColumn<QString>("locale");
#endif
    QTest::addColumn<QString>("s1");
    QTest::addColumn<QString>("s2");
    QTest::addColumn<int>("result");

    /*
        The C locale performs pure byte comparisons for
        Latin-1-specific characters (I think). Compare with Swedish
        below.
    */
#ifdef Q_OS_WIN // assume c locale to be english
    QTest::newRow("c1") << MAKELCID(MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US), SORT_DEFAULT) << QString("\xe5") << QString("\xe4") << 1;
    QTest::newRow("c2") << MAKELCID(MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US), SORT_DEFAULT) << QString("\xe4") << QString("\xf6") << -1;
    QTest::newRow("c3") << MAKELCID(MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US), SORT_DEFAULT) << QString("\xe5") << QString("\xf6") << -1;
#else
    QTest::newRow("c1") << QString("C") << QString("\xe5") << QString("\xe4") << 1;
    QTest::newRow("c2") << QString("C") << QString("\xe4") << QString("\xf6") << -1;
    QTest::newRow("c3") << QString("C") << QString("\xe5") << QString("\xf6") << -1;
#endif

    /*
        It's hard to test English, because it's treated differently
        on different platforms. For example, on Linux, it uses the
        iso14651_t1 template file, which happens to provide good
        defaults for Swedish. Mac OS X seems to do a pure bytewise
        comparison of Latin-1 values, although I'm not sure. So I
        just test digits to make sure that it's not totally broken.
    */
#ifdef Q_OS_WIN
    QTest::newRow("english1") << MAKELCID(MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US), SORT_DEFAULT) << QString("5") << QString("4") << 1;
    QTest::newRow("english2") << MAKELCID(MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US), SORT_DEFAULT) << QString("4") << QString("6") << -1;
    QTest::newRow("english3") << MAKELCID(MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US), SORT_DEFAULT) << QString("5") << QString("6") << -1;
#else
    QTest::newRow("english1") << QString("en_US") << QString("5") << QString("4") << 1;
    QTest::newRow("english2") << QString("en_US") << QString("4") << QString("6") << -1;
    QTest::newRow("english3") << QString("en_US") << QString("5") << QString("6") << -1;
#endif
    /*
        In Swedish, a with ring above (E5) comes before a with
        diaresis (E4), which comes before o diaresis (F6), which
        all come after z.
    */
#ifdef Q_OS_MAC
    QTest::newRow("swedish1") << QString("sv_SE.ISO8859-1") << QString("\xe5") << QString("\xe4") << -1;
    QTest::newRow("swedish2") << QString("sv_SE.ISO8859-1") << QString("\xe4") << QString("\xf6") << -1;
    QTest::newRow("swedish3") << QString("sv_SE.ISO8859-1") << QString("\xe5") << QString("\xf6") << -1;
    QTest::newRow("swedish4") << QString("sv_SE.ISO8859-1") << QString("z") << QString("\xe5") << -1;
#elif defined(Q_OS_WIN)
    QTest::newRow("swedish1") << MAKELCID(MAKELANGID(LANG_SWEDISH, SUBLANG_SWEDISH), SORT_DEFAULT) << QString("\xe5") << QString("\xe4") << -1;
    QTest::newRow("swedish2") << MAKELCID(MAKELANGID(LANG_SWEDISH, SUBLANG_SWEDISH), SORT_DEFAULT) << QString("\xe4") << QString("\xf6") << -1;
    QTest::newRow("swedish3") << MAKELCID(MAKELANGID(LANG_SWEDISH, SUBLANG_SWEDISH), SORT_DEFAULT) << QString("\xe5") << QString("\xf6") << -1;
    QTest::newRow("swedish4") << MAKELCID(MAKELANGID(LANG_SWEDISH, SUBLANG_SWEDISH), SORT_DEFAULT) << QString("z") << QString("\xe5") << -1;
#else
    QTest::newRow("swedish1") << QString("sv_SE") << QString("\xe5") << QString("\xe4") << -1;
    QTest::newRow("swedish2") << QString("sv_SE") << QString("\xe4") << QString("\xf6") << -1;
    QTest::newRow("swedish3") << QString("sv_SE") << QString("\xe5") << QString("\xf6") << -1;
    QTest::newRow("swedish4") << QString("sv_SE") << QString("z") << QString("\xe5") << -1;
#endif

#if 0
    /*
        In Norwegian, ae (E6) comes before o with stroke (D8), which
        comes before a with ring above (E5).
    */
    QTest::newRow("norwegian1") << QString("no_NO") << QString("\xe6") << QString("\xd8") << -1;
    QTest::newRow("norwegian2") << QString("no_NO") << QString("\xd8") << QString("\xe5") << -1;
    QTest::newRow("norwegian3") << QString("no_NO") << QString("\xe6") << QString("\xe5") << -1;
#endif

    /*
        In German, z comes *after* a with diaresis (E4),
        which comes before o diaresis (F6).
    */
#ifdef Q_OS_MAC
    QTest::newRow("german1") << QString("de_DE.ISO8859-1") << QString("z") << QString("\xe4") << 1;
    QTest::newRow("german2") << QString("de_DE.ISO8859-1") << QString("\xe4") << QString("\xf6") << -1;
    QTest::newRow("german3") << QString("de_DE.ISO8859-1") << QString("z") << QString("\xf6") << 1;
#elif defined(Q_OS_WIN)
    QTest::newRow("german1") << MAKELCID(MAKELANGID(LANG_GERMAN, SUBLANG_GERMAN), SORT_DEFAULT) << QString("z") << QString("\xe4") << 1;
    QTest::newRow("german2") << MAKELCID(MAKELANGID(LANG_GERMAN, SUBLANG_GERMAN), SORT_DEFAULT) << QString("\xe4") << QString("\xf6") << -1;
    QTest::newRow("german3") << MAKELCID(MAKELANGID(LANG_GERMAN, SUBLANG_GERMAN), SORT_DEFAULT) << QString("z") << QString("\xf6") << 1;
#else
    QTest::newRow("german1") << QString("de_DE") << QString("z") << QString("\xe4") << 1;
    QTest::newRow("german2") << QString("de_DE") << QString("\xe4") << QString("\xf6") << -1;
    QTest::newRow("german3") << QString("de_DE") << QString("z") << QString("\xf6") << 1;
#endif
}

void tst_QString::localeAwareCompare()
{
#ifdef Q_OS_WIN
#   ifndef Q_OS_WINCE
       QSKIP("On others than Win CE, we cannot set the system or user locale.", SkipAll);
#   endif
    QFETCH(ulong, locale);
#else
    QFETCH(QString, locale);
#endif
    QFETCH(QString, s1);
    QFETCH(QString, s2);
    QFETCH(int, result);

    QStringRef r1(&s1, 0, s1.length());
    QStringRef r2(&s2, 0, s2.length());

#ifdef Q_OS_WIN
#  if defined(Q_OS_WINCE)
    DWORD oldLcid = GetUserDefaultLCID();
    SetUserDefaultLCID(locale);

    QCOMPARE(locale, GetUserDefaultLCID());
#  else
    DWORD oldLcid = GetThreadLocale();
    SetThreadLocale(locale);

    QCOMPARE(locale, GetThreadLocale());
#  endif

#elif defined (Q_WS_MAC)
    QSKIP("Setting the locale is not supported on OS X (you can set the C locale, but that won't affect CFStringCompare which is used to compare strings)", SkipAll);
#else
    if (!locale.isEmpty()) {
        const char *newLocale = setlocale(LC_ALL, locale.toLatin1());
        if (!newLocale) {
            setlocale(LC_ALL, "");
            QSKIP("Please install the proper locale on this machine to test properly", SkipSingle);
            return;
        }
    }
#endif

    int testres = QString::localeAwareCompare(s1, s2);
    if (result < 0) {
        QVERIFY(testres < 0);
    } else if (result > 0) {
        QVERIFY(testres > 0);
    } else {
        QVERIFY(testres == 0);
    }

    testres = QString::localeAwareCompare(s2, s1);
    if (result > 0) {
        QVERIFY(testres < 0);
    } else if (result < 0) {
        QVERIFY(testres > 0);
    } else {
        QVERIFY(testres == 0);
    }

    testres = QString::localeAwareCompare(s1, r2);
    if (result < 0) {
        QVERIFY(testres < 0);
    } else if (result > 0) {
        QVERIFY(testres > 0);
    } else {
        QVERIFY(testres == 0);
    }

    testres = QStringRef::localeAwareCompare(r1, r2);
    if (result < 0) {
        QVERIFY(testres < 0);
    } else if (result > 0) {
        QVERIFY(testres > 0);
    } else {
        QVERIFY(testres == 0);
    }

    testres = QStringRef::localeAwareCompare(r2, r1);
    if (result > 0) {
        QVERIFY(testres < 0);
    } else if (result < 0) {
        QVERIFY(testres > 0);
    } else {
        QVERIFY(testres == 0);
    }

#ifdef Q_OS_WIN
#  if defined(Q_OS_WINCE)
    SetUserDefaultLCID(oldLcid);
#  else
    SetThreadLocale(oldLcid);
#  endif

#else
    if (!locale.isEmpty())
	    setlocale(LC_ALL, "");
#endif
}

void tst_QString::split_data()
{
    QTest::addColumn<QString>("str");
    QTest::addColumn<QString>("sep");
    QTest::addColumn<QStringList>("result");

    QTest::newRow("1") << "a,b,c" << "," << (QStringList() << "a" << "b" << "c");
    QTest::newRow("2") << QString("-rw-r--r--  1 0  0  519240 Jul  9  2002 bigfile")
		    << " "
                    << (QStringList() << "-rw-r--r--" << "" << "1" << "0" << "" << "0" << ""
                                      << "519240" << "Jul" << "" << "9" << "" << "2002" << "bigfile");
    QTest::newRow("one-empty") << "" << " " << (QStringList() << "");
    QTest::newRow("two-empty") << " " << " " << (QStringList() << "" << "");
    QTest::newRow("three-empty") << "  " << " " << (QStringList() << "" << "" << "");

    QTest::newRow("all-empty") << "" << "" << (QStringList() << "" << "");
    QTest::newRow("sep-empty") << "abc" << "" << (QStringList() << "" << "a" << "b" << "c" << "");
}

void tst_QString::split()
{
    QFETCH(QString, str);
    QFETCH(QString, sep);
    QFETCH(QStringList, result);

    QRegExp rx = QRegExp(QRegExp::escape(sep));

    QStringList list;

    list = str.split(sep);
    QVERIFY(list == result);
    list = str.split(rx);
    QVERIFY(list == result);
    if (sep.size() == 1) {
        list = str.split(sep.at(0));
        QVERIFY(list == result);
    }

    list = str.split(sep, QString::KeepEmptyParts);
    QVERIFY(list == result);
    list = str.split(rx, QString::KeepEmptyParts);
    QVERIFY(list == result);
    if (sep.size() == 1) {
        list = str.split(sep.at(0), QString::KeepEmptyParts);
        QVERIFY(list == result);
    }

    result.removeAll("");
    list = str.split(sep, QString::SkipEmptyParts);
    QVERIFY(list == result);
    list = str.split(rx, QString::SkipEmptyParts);
    QVERIFY(list == result);
    if (sep.size() == 1) {
        list = str.split(sep.at(0), QString::SkipEmptyParts);
        QVERIFY(list == result);
    }
}

void tst_QString::split_regexp()
{
    QString str1 = "Some  text\n\twith  strange whitespace.";
    QStringList list1 = str1.split(QRegExp("\\s+"));
    QStringList result1;
    result1 << "Some" << "text" << "with" << "strange" << "whitespace.";
    QVERIFY(list1 == result1);
    list1 = str1.split(QRegExp("\\s"), QString::SkipEmptyParts);
    QVERIFY(list1 == result1);

    QString str2 = "This time, a normal English sentence.";
    QStringList list2 = str2.split(QRegExp("\\W+"));
    QStringList result2;
    result2 << "This" << "time" << "a" << "normal" << "English" << "sentence" << "";
    QVERIFY(list2 == result2);
    list2 = str2.split(QRegExp("\\W"), QString::SkipEmptyParts);
    result2.removeAll(QString());
    QVERIFY(list2 == result2);

    QString str3 = "Now: this sentence fragment.";
    QStringList list3 = str3.split(QRegExp("\\b"));
    QStringList result3;
    result3 << "" << "Now" << ": " << "this" << " " << "sentence" << " " << "fragment" << ".";
    QVERIFY(list3 == result3);
    list3 = str3.split(QRegExp("\\b"), QString::SkipEmptyParts);
    result3.removeAll(QString());
    QVERIFY(list3 == result3);
}

void tst_QString::fromUtf16_data()
{
    QTest::addColumn<QString>("ucs2");
    QTest::addColumn<QString>("res");
    QTest::addColumn<int>("len");

    QTest::newRow("str0") << QString("abcdefgh") << QString("abcdefgh") << -1;
    QTest::newRow("str0-len") << QString("abcdefgh") << QString("abc") << 3;
}

void tst_QString::fromUtf16()
{
    QFETCH(QString, ucs2);
    QFETCH(QString, res);
    QFETCH(int, len);

    QCOMPARE(QString::fromUtf16(ucs2.utf16(), len), res);
}


void tst_QString::latin1String()
{
    QString s("Hello");

    QVERIFY(s == QLatin1String("Hello"));
    QVERIFY(s != QLatin1String("Hello World"));
    QVERIFY(s < QLatin1String("Helloa"));
    QVERIFY(!(s > QLatin1String("Helloa")));
    QVERIFY(s > QLatin1String("Helln"));
    QVERIFY(s > QLatin1String("Hell"));
    QVERIFY(!(s < QLatin1String("Helln")));
    QVERIFY(!(s < QLatin1String("Hell")));
}

void tst_QString::nanAndInf()
{
    bool ok;
    double d;

#define CHECK_DOUBLE(str, expected_ok, expected_inf) \
    d = QString(str).toDouble(&ok); \
    QVERIFY(ok == expected_ok); \
    QVERIFY((d == INFINITY) == expected_inf);

    CHECK_DOUBLE("inf", true, true)
    CHECK_DOUBLE("INF", true, true)
    CHECK_DOUBLE("inf  ", true, true)
    CHECK_DOUBLE("+inf", true, true)
    CHECK_DOUBLE("\t +INF", true, true)
    CHECK_DOUBLE("\t INF", true, true)
    CHECK_DOUBLE("inF  ", true, true)
    CHECK_DOUBLE("+iNf", true, true)
    CHECK_DOUBLE("INFe-10", false, false)
    CHECK_DOUBLE("0xINF", false, false)
    CHECK_DOUBLE("- INF", false, false)
    CHECK_DOUBLE("+ INF", false, false)
    CHECK_DOUBLE("-- INF", false, false)
    CHECK_DOUBLE("inf0", false, false)
    CHECK_DOUBLE("--INF", false, false)
    CHECK_DOUBLE("++INF", false, false)
    CHECK_DOUBLE("INF++", false, false)
    CHECK_DOUBLE("INF--", false, false)
    CHECK_DOUBLE("INF +", false, false)
    CHECK_DOUBLE("INF -", false, false)
    CHECK_DOUBLE("0INF", false, false)
#undef CHECK_INF

#define CHECK_NAN(str, expected_ok, expected_nan) \
    d = QString(str).toDouble(&ok); \
    QVERIFY(ok == expected_ok); \
    QVERIFY((bool)isnan(d) == expected_nan); \

    CHECK_NAN("nan", true, true)
    CHECK_NAN("NAN", true, true)
    CHECK_NAN("nan  ", true, true)
    CHECK_NAN("\t NAN", true, true)
    CHECK_NAN("\t NAN  ", true, true)
    CHECK_NAN("-nan", false, false)
    CHECK_NAN("+NAN", false, false)
    CHECK_NAN("NaN", true, true)
    CHECK_NAN("nAn", true, true)
    CHECK_NAN("NANe-10", false, false)
    CHECK_NAN("0xNAN", false, false)
    CHECK_NAN("0NAN", false, false)
#undef CHECK_NAN

    d = QString("-INF").toDouble(&ok);
    QVERIFY(ok);
    QVERIFY(d == -INFINITY);

    long l;
    l = QString("INF").toLong(&ok);
    QVERIFY(!ok);

    l = QString("INF").toLong(&ok, 36);
    QVERIFY(ok);

    l = QString("INF0").toLong(&ok, 36);
    QVERIFY(ok);

    l = QString("0INF0").toLong(&ok, 36);
    QVERIFY(ok);
}

void tst_QString::arg_fillChar_data()
{
    QTest::addColumn<QString>("pattern");
    QTest::addColumn<QList<QVariant> >("replaceValues");
    QTest::addColumn<IntList>("widths");
    QTest::addColumn<QString>("fillChars");
    QTest::addColumn<QString>("expected");

    QList<QVariant> replaceValues;
    IntList widths;
    QString fillChars;

    replaceValues << QVariant((int)5) << QVariant(QString("f")) << QVariant((int)0);
    widths << 3 << 2 << 5;
    QTest::newRow("str0") << QString("%1%2%3") << replaceValues << widths << QString("abc") << QString("aa5bfcccc0");

    replaceValues.clear();
    widths.clear();
    replaceValues << QVariant((int)5.5) << QVariant(QString("foo")) << QVariant((qulonglong)INT_MAX);
    widths << 10 << 2 << 5;
    QTest::newRow("str1") << QString("%3.%1.%3.%2") << replaceValues << widths << QString("0 c")
		       << QString("2147483647.0000000005.2147483647.foo");

    replaceValues.clear();
    widths.clear();
    replaceValues << QVariant(QString("fisk"));
    widths << 100;
    QTest::newRow("str2") << QString("%9 og poteter") << replaceValues << widths << QString("f")
		       << QString("fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffisk og poteter");
}

void tst_QString::arg_fillChar()
{
    static const int base = 10;
    static const char fmt = 'g';
    static const int prec = -1;

    QFETCH(QString, pattern);
    QFETCH(QList<QVariant>, replaceValues);
    QFETCH(IntList, widths);
    QFETCH(QString, fillChars);
    QFETCH(QString, expected);
    QCOMPARE(replaceValues.count(), fillChars.count());
    QCOMPARE(replaceValues.count(), widths.count());

    QString actual = pattern;
    for (int i=0; i<replaceValues.count(); ++i) {
	const QVariant &var = replaceValues.at(i);
	const int width = widths.at(i);
	const QChar &fillChar = fillChars.at(i);
        switch (var.type()) {
        case QVariant::String: actual = actual.arg(var.toString(), width, fillChar); break;
        case QVariant::Int: actual = actual.arg(var.toInt(), width, base, fillChar); break;
        case QVariant::UInt: actual = actual.arg(var.toUInt(), width, base, fillChar); break;
        case QVariant::Double: actual = actual.arg(var.toDouble(), width, fmt, prec, fillChar); break;
        case QVariant::LongLong: actual = actual.arg(var.toLongLong(), width, base, fillChar); break;
        case QVariant::ULongLong: actual = actual.arg(var.toULongLong(), width, base, fillChar); break;
        default: QVERIFY(0); break;
        }
    }

    QCOMPARE(actual, expected);
}

static inline int sign(int x)
{
    return x == 0 ? 0 : (x < 0 ? -1 : 1);
}

void tst_QString::compare_data()
{
    QTest::addColumn<QString>("s1");
    QTest::addColumn<QString>("s2");
    QTest::addColumn<int>("csr"); // case sensitive result
    QTest::addColumn<int>("cir"); // case insensitive result


    // null strings
    QTest::newRow("data0") << QString("") << QString("") << 0 << 0;
    QTest::newRow("data1") << QString("a") << QString("") << 1 << 1;
    QTest::newRow("data2") << QString("") << QString("a") << -1 << -1;

    // equal length
    QTest::newRow("data3") << QString("abc") << QString("abc") << 0 << 0;
    QTest::newRow("data4") << QString("abC") << QString("abc") << -1 << 0;
    QTest::newRow("data5") << QString("abc") << QString("abC") << 1 << 0;

    // different length
    QTest::newRow("data6") << QString("abcdef") << QString("abc") << 1 << 1;
    QTest::newRow("data6") << QString("abCdef") << QString("abc") << -1 << 1;
    QTest::newRow("data7") << QString("abc") << QString("abcdef") << -1 << -1;

    QString upper;
    upper += QChar(QChar::highSurrogate(0x10400));
    upper += QChar(QChar::lowSurrogate(0x10400));
    QString lower;
    lower += QChar(QChar::highSurrogate(0x10428));
    lower += QChar(QChar::lowSurrogate(0x10428));
    QTest::newRow("data8") << upper << lower << -1 << 0;

    // embedded nulls
    // These dont work as of now. Its OK that these dont work since \0 is not a valid unicode
    /*QTest::newRow("data9") << QString(QByteArray("\0", 1)) << QString(QByteArray("\0", 1)) << 0 << 0;
    QTest::newRow("data10") << QString(QByteArray("\0", 1)) << QString("") << 1 << 1;
    QTest::newRow("data11") << QString("") << QString(QByteArray("\0", 1)) << -1 << -1;
    QTest::newRow("data12") << QString("ab\0c") << QString(QByteArray("ab\0c", 4)) << 0 << 0;
    QTest::newRow("data13") << QString(QByteArray("ab\0c", 4)) << QString("abc") << -1 << -1;
    QTest::newRow("data14") << QString("abc") << QString(QByteArray("ab\0c", 4)) << 1 << 1;*/
}

static bool isLatin(const QString &s)
{
    for (int i = 0; i < s.length(); ++i)
        if (s.at(i).unicode() > 0xff)
            return false;
    return true;
}

void tst_QString::compare()
{
    QFETCH(QString, s1);
    QFETCH(QString, s2);
    QFETCH(int, csr);
    QFETCH(int, cir);

    QStringRef r1(&s1, 0, s1.length());
    QStringRef r2(&s2, 0, s2.length());

    QCOMPARE(sign(QString::compare(s1, s2)), csr);
    QCOMPARE(sign(QStringRef::compare(r1, r2)), csr);
    QCOMPARE(sign(s1.compare(s2)), csr);
    QCOMPARE(sign(s1.compare(r2)), csr);
    QCOMPARE(sign(r1.compare(r2)), csr);

    QCOMPARE(sign(s1.compare(s2, Qt::CaseSensitive)), csr);
    QCOMPARE(sign(s1.compare(s2, Qt::CaseInsensitive)), cir);
    QCOMPARE(sign(s1.compare(r2, Qt::CaseSensitive)), csr);
    QCOMPARE(sign(s1.compare(r2, Qt::CaseInsensitive)), cir);
    QCOMPARE(sign(r1.compare(r2, Qt::CaseSensitive)), csr);
    QCOMPARE(sign(r1.compare(r2, Qt::CaseInsensitive)), cir);

    QCOMPARE(sign(QString::compare(s1, s2, Qt::CaseSensitive)), csr);
    QCOMPARE(sign(QString::compare(s1, s2, Qt::CaseInsensitive)), cir);
    QCOMPARE(sign(QString::compare(s1, r2, Qt::CaseSensitive)), csr);
    QCOMPARE(sign(QString::compare(s1, r2, Qt::CaseInsensitive)), cir);
    QCOMPARE(sign(QStringRef::compare(r1, r2, Qt::CaseSensitive)), csr);
    QCOMPARE(sign(QStringRef::compare(r1, r2, Qt::CaseInsensitive)), cir);

    if (!cir) {
        QCOMPARE(s1.toCaseFolded(), s2.toCaseFolded());
    }

    if (isLatin(s2)) {
        QCOMPARE(sign(QString::compare(s1, QLatin1String(s2.toLatin1()))), csr);
        QCOMPARE(sign(QString::compare(s1, QLatin1String(s2.toLatin1()), Qt::CaseInsensitive)), cir);
        QCOMPARE(sign(QStringRef::compare(r1, QLatin1String(s2.toLatin1()))), csr);
        QCOMPARE(sign(QStringRef::compare(r1, QLatin1String(s2.toLatin1()), Qt::CaseInsensitive)), cir);
    }

    if (isLatin(s1)) {
        QCOMPARE(sign(QString::compare(QLatin1String(s1.toLatin1()), s2)), csr);
        QCOMPARE(sign(QString::compare(QLatin1String(s1.toLatin1()), s2, Qt::CaseInsensitive)), cir);
    }
}

void tst_QString::resizeAfterFromRawData()
{
    QString buffer("hello world");

    QString array = QString::fromRawData(buffer.constData(), buffer.size());
    QVERIFY(array.constData() == buffer.constData());
    array.resize(5);
    QVERIFY(array.constData() == buffer.constData());
}

void tst_QString::resizeAfterReserve()
{

    QString s;
    s.reserve(100);

    s += "hello world";

    // resize should not affect capacity
    s.resize(s.size());
    QVERIFY(s.capacity() == 100);

    // but squeeze does
    s.squeeze();
    QVERIFY(s.capacity() == s.size());

    // clear does too
    s.clear();
    QVERIFY(s.capacity() == 0);

    // test resize(0) border case
    s.reserve(100);
    s += "hello world";
    s.resize(0);
    QVERIFY(s.capacity() == 100);
}

void tst_QString::resizeWithNegative() const
{
    {
        QString string(QLatin1String("input"));
        string.resize(-1);
        QCOMPARE(string, QString());
    }

    {
        QString string(QLatin1String("input"));
        string.resize(-9099);
        QCOMPARE(string, QString());
    }

    {
        /* Example code from customer. */
        QString s(QLatin1String("hola"));
        s.reserve(1);
        s.resize(-1);
        QCOMPARE(s, QString());
    }
}

void tst_QString::truncateWithNegative() const
{
    {
        QString string(QLatin1String("input"));
        string.truncate(-1);
        QCOMPARE(string, QString());
    }

    {
        QString string(QLatin1String("input"));
        string.truncate(-9099);
        QCOMPARE(string, QString());
    }

    {
        /* Example code from customer. */
        QString test(QLatin1String("c"));

        test.replace(QRegExp(QLatin1String("c")), QLatin1String("z"));
        test.truncate(-1);
        QCOMPARE(test, QString());
    }
}

void tst_QString::QCharRefMutableUnicode() const
{
    QString str;
    str.resize(3);
    str[0].unicode() = 115;
    str[1].unicode() = 116;
    str[2].unicode() = 114;

    QCOMPARE(str, QString::fromLatin1("str"));
}

void tst_QString::QCharRefDetaching() const
{
    {
        QString str = QString::fromLatin1("str");
        QString copy;
        copy[0] = QLatin1Char('S');

        QCOMPARE(str, QString::fromLatin1("str"));
    }

    {
        ushort buf[] = { 's', 't', 'r' };
        QString str = QString::fromRawData((const QChar *)buf, 3);
        str[0] = QLatin1Char('S');

        QCOMPARE(buf[0], ushort('s'));
    }

    {
        static const ushort buf[] = { 's', 't', 'r' };
        QString str = QString::fromRawData((const QChar *)buf, 3);

        // this causes a crash in most systems if the detaching doesn't work
        str[0] = QLatin1Char('S');

        QCOMPARE(buf[0], ushort('s'));
    }
}

void tst_QString::sprintfZU() const
{
    {
        QString string;
        size_t s = 6;
        string.sprintf("%zu", s);
        QCOMPARE(string, QString::fromLatin1("6"));
    }

    {
        QString string;
        string.sprintf("%s\n", "foo");
        QCOMPARE(string, QString::fromLatin1("foo\n"));
    }

    {
        /* This code crashed. I don't know how to reduce it further. In other words,
         * both %zu and %s needs to be present. */
        size_t s = 6;
        QString string;
        string.sprintf("%zu%s", s, "foo");
        QCOMPARE(string, QString::fromLatin1("6foo"));
    }

    {
        size_t s = 6;
        QString string;
        string.sprintf("%zu %s\n", s, "foo");
        QCOMPARE(string, QString::fromLatin1("6 foo\n"));
    }
}

void tst_QString::repeatedSignature() const
{
    /* repated() should be a const member. */
    const QString string;
    string.repeated(3);
}

void tst_QString::repeated() const
{
    QFETCH(QString, string);
    QFETCH(QString, expected);
    QFETCH(int, count);

    QCOMPARE(string.repeated(count), expected);
}

void tst_QString::repeated_data() const
{
    QTest::addColumn<QString>("string" );
    QTest::addColumn<QString>("expected" );
    QTest::addColumn<int>("count" );

    /* Empty strings. */
    QTest::newRow("")
        << QString()
        << QString()
        << 0;

    QTest::newRow("")
        << QString()
        << QString()
        << -1004;

    QTest::newRow("")
        << QString()
        << QString()
        << 1;

    QTest::newRow("")
        << QString()
        << QString()
        << 5;

    /* On simple string. */
    QTest::newRow("")
        << QString(QLatin1String("abc"))
        << QString()
        << -1004;

    QTest::newRow("")
        << QString(QLatin1String("abc"))
        << QString()
        << -1;

    QTest::newRow("")
        << QString(QLatin1String("abc"))
        << QString()
        << 0;

    QTest::newRow("")
        << QString(QLatin1String("abc"))
        << QString(QLatin1String("abc"))
        << 1;

    QTest::newRow("")
        << QString(QLatin1String("abc"))
        << QString(QLatin1String("abcabc"))
        << 2;

    QTest::newRow("")
        << QString(QLatin1String("abc"))
        << QString(QLatin1String("abcabcabc"))
        << 3;

    QTest::newRow("")
        << QString(QLatin1String("abc"))
        << QString(QLatin1String("abcabcabcabc"))
        << 4;
}

void tst_QString::task262677remove()
{
    QString driveName = QLatin1String("V:\\blahblah\\more_blahblah\\");
    driveName.remove(2, INT_MAX); // should be "V:" - instead, it's "V::\\blahblah\\more_blahblah\\"
    QVERIFY(driveName == QLatin1String("V:"));
}

void tst_QString::QTBUG10404_compareRef()
{
    QString a = "ABCDEFGH";

    QCOMPARE(QStringRef(&a, 1, 2).compare(QLatin1String("BC")), 0);
    QVERIFY(QStringRef(&a, 1, 2).compare(QLatin1String("BCD")) < 0);
    QCOMPARE(QStringRef(&a, 1, 2).compare(QLatin1String("Bc"), Qt::CaseInsensitive), 0);
    QVERIFY(QStringRef(&a, 1, 2).compare(QLatin1String("bCD"), Qt::CaseInsensitive) < 0);

    QCOMPARE(QStringRef(&a, 1, 2).compare(QString::fromLatin1("BC")), 0);
    QVERIFY(QStringRef(&a, 1, 2).compare(QString::fromLatin1("BCD")) < 0);
    QCOMPARE(QStringRef(&a, 1, 2).compare(QString::fromLatin1("Bc"), Qt::CaseInsensitive), 0);
    QVERIFY(QStringRef(&a, 1, 2).compare(QString::fromLatin1("bCD"), Qt::CaseInsensitive) < 0);

    QCOMPARE(QString::fromLatin1("BC").compare(QStringRef(&a, 1, 2)), 0);
    QVERIFY(QString::fromLatin1("BCD").compare(QStringRef(&a, 1, 2)) > 0);
    QCOMPARE(QString::fromLatin1("Bc").compare(QStringRef(&a, 1, 2), Qt::CaseInsensitive), 0);
    QVERIFY(QString::fromLatin1("bCD").compare(QStringRef(&a, 1, 2), Qt::CaseInsensitive) > 0);

    QCOMPARE(QStringRef(&a, 1, 2).compare(QStringRef(&a, 1, 2)), 0);
    QVERIFY(QStringRef(&a, 1, 2).compare(QStringRef(&a, 1, 3)) < 0);
    QCOMPARE(QStringRef(&a, 1, 2).compare(QStringRef(&a, 1, 2), Qt::CaseInsensitive), 0);
    QVERIFY(QStringRef(&a, 1, 2).compare(QStringRef(&a, 1, 3), Qt::CaseInsensitive) < 0);

    QString a2 = "ABCDEFGh";
    QCOMPARE(QStringRef(&a2, 1, 2).compare(QStringRef(&a, 1, 2)), 0);
    QVERIFY(QStringRef(&a2, 1, 2).compare(QStringRef(&a, 1, 3)) < 0);
    QCOMPARE(QStringRef(&a2, 1, 2).compare(QStringRef(&a, 1, 2), Qt::CaseInsensitive), 0);
    QVERIFY(QStringRef(&a2, 1, 2).compare(QStringRef(&a, 1, 3), Qt::CaseInsensitive) < 0);
}

void tst_QString::QTBUG9281_arg_locale()
{
    QLocale l(QLocale::English, QLocale::UnitedKingdom);
    l.setNumberOptions(QLocale::OmitGroupSeparator);
    QLocale::setDefault(l);
    QString str("*%L1*%L2*");
    str = str.arg(123456).arg(1234.56);
    QCOMPARE(str, QString::fromLatin1("*123456*1234.56*"));
    QLocale::setDefault(QLocale::C);
}



QTEST_APPLESS_MAIN(tst_QString)

#include "tst_qstring.moc"
