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


/*-*-encoding:latin1-*-*/
//#include <iostream>
//using namespace std;

#include <QtTest/QtTest>

#ifdef Q_OS_UNIX
#include <locale.h>
#endif

#include <QBuffer>
#include <QByteArray>
#include <QCoreApplication>
#include <QDebug>
#include <QFile>
#include <QTcpSocket>
#include <QTextStream>
#include <QTextCodec>
#include <QProcess>

#include "../network-settings.h"

static const char *TestFileName = "testfile";

Q_DECLARE_METATYPE(qlonglong)
Q_DECLARE_METATYPE(qulonglong)
Q_DECLARE_METATYPE(QList<int>)

QT_BEGIN_NAMESPACE
template<> struct QMetaTypeId<QIODevice::OpenModeFlag>
{ enum { Defined = 1 }; static inline int qt_metatype_id() { return QMetaType::Int; } };
QT_END_NAMESPACE

//TESTED_CLASS=
//TESTED_FILES=

#ifdef Q_OS_SYMBIAN
#define SRCDIR ""
#endif

class tst_QTextStream : public QObject
{
    Q_OBJECT

public:
    tst_QTextStream();
    virtual ~tst_QTextStream();

public slots:
    void init();
    void cleanup();

private slots:
    void getSetCheck();
    void construction();

    // lines
    void readLineFromDevice_data();
    void readLineFromDevice();
    void readLineFromString_data();
    void readLineFromString();
    void readLineFromTextDevice_data();
    void readLineFromTextDevice();
    void readLineUntilNull();
    void readLineMaxlen_data();
    void readLineMaxlen();
    void readLinesFromBufferCRCR();

    // all
    void readAllFromDevice_data();
    void readAllFromDevice();
    void readAllFromString_data();
    void readAllFromString();
    void readLineFromStringThenChangeString();

    // device tests
    void setDevice();

    // char operators
    void QChar_operators_FromDevice_data();
    void QChar_operators_FromDevice();
    void char_operators_FromDevice_data();
    void char_operators_FromDevice();

    // natural number read operator
    void signedShort_read_operator_FromDevice_data();
    void signedShort_read_operator_FromDevice();
    void unsignedShort_read_operator_FromDevice_data();
    void unsignedShort_read_operator_FromDevice();
    void signedInt_read_operator_FromDevice_data();
    void signedInt_read_operator_FromDevice();
    void unsignedInt_read_operator_FromDevice_data();
    void unsignedInt_read_operator_FromDevice();
    void qlonglong_read_operator_FromDevice_data();
    void qlonglong_read_operator_FromDevice();
    void qulonglong_read_operator_FromDevice_data();
    void qulonglong_read_operator_FromDevice();

    // natural number write operator
    void signedShort_write_operator_ToDevice_data();
    void signedShort_write_operator_ToDevice();
    void unsignedShort_write_operator_ToDevice_data();
    void unsignedShort_write_operator_ToDevice();
    void signedInt_write_operator_ToDevice_data();
    void signedInt_write_operator_ToDevice();
    void unsignedInt_write_operator_ToDevice_data();
    void unsignedInt_write_operator_ToDevice();
    void qlonglong_write_operator_ToDevice_data();
    void qlonglong_write_operator_ToDevice();
    void qulonglong_write_operator_ToDevice_data();
    void qulonglong_write_operator_ToDevice();

    void int_read_with_locale_data();
    void int_read_with_locale();

    void int_write_with_locale_data();
    void int_write_with_locale();

    // real number read operator
    void float_read_operator_FromDevice_data();
    void float_read_operator_FromDevice();
    void double_read_operator_FromDevice_data();
    void double_read_operator_FromDevice();

    // real number write operator
    void float_write_operator_ToDevice_data();
    void float_write_operator_ToDevice();
    void double_write_operator_ToDevice_data();
    void double_write_operator_ToDevice();

    void double_write_with_flags_data();
    void double_write_with_flags();

    void double_write_with_precision_data();
    void double_write_with_precision();

    // text read operators
    void charPtr_read_operator_FromDevice_data();
    void charPtr_read_operator_FromDevice();
    void stringRef_read_operator_FromDevice_data();
    void stringRef_read_operator_FromDevice();
    void byteArray_read_operator_FromDevice_data();
    void byteArray_read_operator_FromDevice();

    // text write operators
    void string_write_operator_ToDevice_data();
    void string_write_operator_ToDevice();

    // other
    void skipWhiteSpace_data();
    void skipWhiteSpace();
    void lineCount_data();
    void lineCount();
    void performance();
    void hexTest_data();
    void hexTest();
    void binTest_data();
    void binTest();
    void octTest_data();
    void octTest();
    void zeroTermination();
    void ws_manipulator();
    void stillOpenWhenAtEnd();
    void readNewlines_data();
    void readNewlines();
    void seek();
    void pos();
    void pos2();
    void readStdin();
    void readAllFromStdin();
    void readLineFromStdin();
    void read();
    void qbool();
    void forcePoint();
    void forceSign();
    void read0d0d0a();
    void numeralCase_data();
    void numeralCase();
    void nanInf();
    void utf8IncompleteAtBufferBoundary_data();
    void utf8IncompleteAtBufferBoundary();
    void writeSeekWriteNoBOM();

    // status
    void status_real_read_data();
    void status_real_read();
    void status_integer_read();
    void status_word_read();

    // use case tests
    void useCase1();
    void useCase2();

    // manipulators
    void manipulators_data();
    void manipulators();

    // UTF-16 BOM (Byte Order Mark)
    void generateBOM();
    void readBomSeekBackReadBomAgain();

    // old tests
#ifdef QT3_SUPPORT
    void qt3_operator_shiftleft_data();
    void qt3_operator_shiftleft();
    void qt3_operator_shiftright_data();
    void qt3_operator_shiftright();
    void qt3_operator_shift_QChar_data();
    void qt3_operator_shift_QChar();
    void qt3_operator_shift_char_data();
    void qt3_operator_shift_char();
    void qt3_operator_shift_short_data();
    void qt3_operator_shift_short();
    void qt3_operator_shift_ushort_data();
    void qt3_operator_shift_ushort();
    void qt3_operator_shift_int_data();
    void qt3_operator_shift_int();
    void qt3_operator_shift_uint_data();
    void qt3_operator_shift_uint();
    void qt3_operator_shift_long_data();
    void qt3_operator_shift_long();
    void qt3_operator_shift_ulong_data();
    void qt3_operator_shift_ulong();
    void qt3_operator_shift_float_data();
    void qt3_operator_shift_float();
    void qt3_operator_shift_double_data();
    void qt3_operator_shift_double();
    void qt3_operator_shift_QString_data();
    void qt3_operator_shift_QString();
    void qt3_operator_shift_QByteArray_data();
    void qt3_operator_shift_QByteArray();

    void qt3_writeDataToFileReadAsLines_data();
    void qt3_writeDataToFileReadAsLines();
    void qt3_writeDataToQStringReadAsLines_data();
    void qt3_writeDataToQStringReadAsLines();

    void qt3_readLineFromString();
#endif

    // task-specific tests
    void task180679_alignAccountingStyle();
    void task178772_setCodec();

private:
    void generateLineData(bool for_QString);
    void generateAllData(bool for_QString);
    void generateOperatorCharData(bool for_QString);
    void generateNaturalNumbersData(bool for_QString);
    void generateRealNumbersData(bool for_QString);
    void generateStringData(bool for_QString);
    void generateRealNumbersDataWrite();

    // Qt 3 privates
#ifdef QT3_SUPPORT
    void qt3_createWriteStream( QTextStream *&os );
    void qt3_closeWriteStream( QTextStream *os );
    void qt3_createReadStream( QTextStream *&is );
    void qt3_closeReadStream( QTextStream *is );
    void qt3_read_QChar( QTextStream *s );
    void qt3_write_QChar( QTextStream *s );
    void qt3_read_char( QTextStream *s );
    void qt3_write_char( QTextStream *s );
    void qt3_read_short( QTextStream *s );
    void qt3_write_short( QTextStream *s );
    void qt3_read_ushort( QTextStream *s );
    void qt3_write_ushort( QTextStream *s );
    void qt3_read_int( QTextStream *s );
    void qt3_write_int( QTextStream *s );
    void qt3_read_uint( QTextStream *s );
    void qt3_write_uint( QTextStream *s );
    void qt3_read_long( QTextStream *s );
    void qt3_write_long( QTextStream *s );
    void qt3_read_ulong( QTextStream *s );
    void qt3_write_ulong( QTextStream *s );
    void qt3_read_float( QTextStream *s );
    void qt3_write_float( QTextStream *s );
    void qt3_read_double( QTextStream *s );
    void qt3_write_double( QTextStream *s );
    void qt3_read_QString( QTextStream *s );
    void qt3_write_QString( QTextStream *s );
    void qt3_read_QByteArray( QTextStream *s );
    void qt3_write_QByteArray( QTextStream *s );
    void qt3_operatorShift_data( QIODevice::OpenMode );
    void qt3_do_shiftleft( QTextStream *ts );
    QTextStream::Encoding qt3_toEncoding( const QString& );
    QString qt3_decodeString( QByteArray array, const QString& encoding );
#endif

    // Qt 3 data
    QTextStream *os;
    QTextStream *is;
    QTextStream *ts;
    QFile *inFile;
    QFile *inResource;
    QFile *outFile;
    QByteArray *inArray;
    QBuffer *inBuffer;
    QString *inString;
    bool file_is_empty;
};

// Testing get/set functions
void tst_QTextStream::getSetCheck()
{
    // Initialize codecs
    int argc = 0;
    char **argv = 0;
    QCoreApplication app(argc, argv);

    QTextStream obj1;
    // QTextCodec * QTextStream::codec()
    // void QTextStream::setCodec(QTextCodec *)
    QTextCodec *var1 = QTextCodec::codecForName("en");
    obj1.setCodec(var1);
    QCOMPARE(var1, obj1.codec());
    obj1.setCodec((QTextCodec *)0);
    QCOMPARE((QTextCodec *)0, obj1.codec());

    // bool QTextStream::autoDetectUnicode()
    // void QTextStream::setAutoDetectUnicode(bool)
    obj1.setAutoDetectUnicode(false);
    QCOMPARE(false, obj1.autoDetectUnicode());
    obj1.setAutoDetectUnicode(true);
    QCOMPARE(true, obj1.autoDetectUnicode());

    // bool QTextStream::generateByteOrderMark()
    // void QTextStream::setGenerateByteOrderMark(bool)
    obj1.setGenerateByteOrderMark(false);
    QCOMPARE(false, obj1.generateByteOrderMark());
    obj1.setGenerateByteOrderMark(true);
    QCOMPARE(true, obj1.generateByteOrderMark());

    // QIODevice * QTextStream::device()
    // void QTextStream::setDevice(QIODevice *)
    QFile *var4 = new QFile;
    obj1.setDevice(var4);
    QCOMPARE(static_cast<QIODevice *>(var4), obj1.device());
    obj1.setDevice((QIODevice *)0);
    QCOMPARE((QIODevice *)0, obj1.device());
    delete var4;

    // Status QTextStream::status()
    // void QTextStream::setStatus(Status)
    obj1.setStatus(QTextStream::Status(QTextStream::Ok));
    QCOMPARE(QTextStream::Status(QTextStream::Ok), obj1.status());
    obj1.setStatus(QTextStream::Status(QTextStream::ReadPastEnd));
    QCOMPARE(QTextStream::Status(QTextStream::ReadPastEnd), obj1.status());
    obj1.resetStatus();
    obj1.setStatus(QTextStream::Status(QTextStream::ReadCorruptData));
    QCOMPARE(QTextStream::Status(QTextStream::ReadCorruptData), obj1.status());

    // FieldAlignment QTextStream::fieldAlignment()
    // void QTextStream::setFieldAlignment(FieldAlignment)
    obj1.setFieldAlignment(QTextStream::FieldAlignment(QTextStream::AlignLeft));
    QCOMPARE(QTextStream::FieldAlignment(QTextStream::AlignLeft), obj1.fieldAlignment());
    obj1.setFieldAlignment(QTextStream::FieldAlignment(QTextStream::AlignRight));
    QCOMPARE(QTextStream::FieldAlignment(QTextStream::AlignRight), obj1.fieldAlignment());
    obj1.setFieldAlignment(QTextStream::FieldAlignment(QTextStream::AlignCenter));
    QCOMPARE(QTextStream::FieldAlignment(QTextStream::AlignCenter), obj1.fieldAlignment());
    obj1.setFieldAlignment(QTextStream::FieldAlignment(QTextStream::AlignAccountingStyle));
    QCOMPARE(QTextStream::FieldAlignment(QTextStream::AlignAccountingStyle), obj1.fieldAlignment());

    // QChar QTextStream::padChar()
    // void QTextStream::setPadChar(QChar)
    QChar var7 = 'Q';
    obj1.setPadChar(var7);
    QCOMPARE(var7, obj1.padChar());
    obj1.setPadChar(QChar());
    QCOMPARE(QChar(), obj1.padChar());

    // int QTextStream::fieldWidth()
    // void QTextStream::setFieldWidth(int)
    obj1.setFieldWidth(0);
    QCOMPARE(0, obj1.fieldWidth());
    obj1.setFieldWidth(INT_MIN);
    QCOMPARE(INT_MIN, obj1.fieldWidth());
    obj1.setFieldWidth(INT_MAX);
    QCOMPARE(INT_MAX, obj1.fieldWidth());

    // NumberFlags QTextStream::numberFlags()
    // void QTextStream::setNumberFlags(NumberFlags)
    obj1.setNumberFlags(QTextStream::NumberFlags(QTextStream::ShowBase));
    QCOMPARE(QTextStream::NumberFlags(QTextStream::ShowBase), obj1.numberFlags());
    obj1.setNumberFlags(QTextStream::NumberFlags(QTextStream::ForcePoint));
    QCOMPARE(QTextStream::NumberFlags(QTextStream::ForcePoint), obj1.numberFlags());
    obj1.setNumberFlags(QTextStream::NumberFlags(QTextStream::ForceSign));
    QCOMPARE(QTextStream::NumberFlags(QTextStream::ForceSign), obj1.numberFlags());
    obj1.setNumberFlags(QTextStream::NumberFlags(QTextStream::UppercaseBase));
    QCOMPARE(QTextStream::NumberFlags(QTextStream::UppercaseBase), obj1.numberFlags());
    obj1.setNumberFlags(QTextStream::NumberFlags(QTextStream::UppercaseDigits));
    QCOMPARE(QTextStream::NumberFlags(QTextStream::UppercaseDigits), obj1.numberFlags());

    // int QTextStream::integerBase()
    // void QTextStream::setIntegerBase(int)
    obj1.setIntegerBase(0);
    QCOMPARE(0, obj1.integerBase());
    obj1.setIntegerBase(INT_MIN);
    QCOMPARE(INT_MIN, obj1.integerBase());
    obj1.setIntegerBase(INT_MAX);
    QCOMPARE(INT_MAX, obj1.integerBase());

    // RealNumberNotation QTextStream::realNumberNotation()
    // void QTextStream::setRealNumberNotation(RealNumberNotation)
    obj1.setRealNumberNotation(QTextStream::RealNumberNotation(QTextStream::SmartNotation));
    QCOMPARE(QTextStream::RealNumberNotation(QTextStream::SmartNotation), obj1.realNumberNotation());
    obj1.setRealNumberNotation(QTextStream::RealNumberNotation(QTextStream::FixedNotation));
    QCOMPARE(QTextStream::RealNumberNotation(QTextStream::FixedNotation), obj1.realNumberNotation());
    obj1.setRealNumberNotation(QTextStream::RealNumberNotation(QTextStream::ScientificNotation));
    QCOMPARE(QTextStream::RealNumberNotation(QTextStream::ScientificNotation), obj1.realNumberNotation());

    // int QTextStream::realNumberPrecision()
    // void QTextStream::setRealNumberPrecision(int)
    obj1.setRealNumberPrecision(0);
    QCOMPARE(0, obj1.realNumberPrecision());
    obj1.setRealNumberPrecision(INT_MIN);
    QCOMPARE(6, obj1.realNumberPrecision()); // Setting a negative precision reverts it to the default value (6).
    obj1.setRealNumberPrecision(INT_MAX);
    QCOMPARE(INT_MAX, obj1.realNumberPrecision());
}

tst_QTextStream::tst_QTextStream()
{
    // Initialize Qt 3 data
    ts = 0;
    os = 0;
    is = 0;
    outFile = 0;
    inFile = 0;
    inResource = 0;
    inArray = 0;
    inBuffer = 0;
    inString = 0;
    file_is_empty = FALSE;

    Q_SET_DEFAULT_IAP
}

tst_QTextStream::~tst_QTextStream()
{
}

void tst_QTextStream::init()
{
    // Initialize Qt 3 data
    ts = 0;
    os = 0;
    is = 0;
    inFile = 0;
    inResource = 0;
    outFile = 0;
    inArray = 0;
    inBuffer = 0;
    inString = 0;
    file_is_empty = FALSE;
}

void tst_QTextStream::cleanup()
{
    // Clean up Qt 3 data
    delete ts;
    ts = 0;
    delete os;
    os = 0;
    delete is;
    is = 0;
    delete inResource;
    inResource = 0;
    delete inFile;
    inFile = 0;
    delete outFile;
    outFile = 0;
    delete inArray;
    inArray = 0;
    delete inBuffer;
    inBuffer = 0;
    delete inString;
    inString = 0;

    QCoreApplication::instance()->processEvents();
}

// ------------------------------------------------------------------------------
void tst_QTextStream::construction()
{
    QTextStream stream;
    QCOMPARE(stream.codec(), QTextCodec::codecForLocale());
    QCOMPARE(stream.device(), static_cast<QIODevice *>(0));
    QCOMPARE(stream.string(), static_cast<QString *>(0));

    QTest::ignoreMessage(QtWarningMsg, "QTextStream: No device");
    QVERIFY(stream.atEnd());

    QTest::ignoreMessage(QtWarningMsg, "QTextStream: No device");
    QCOMPARE(stream.readAll(), QString());

}

void tst_QTextStream::generateLineData(bool for_QString)
{
    QTest::addColumn<QByteArray>("data");
    QTest::addColumn<QStringList>("lines");

    // latin-1
    QTest::newRow("emptyer") << QByteArray() << QStringList();
    QTest::newRow("lf") << QByteArray("\n") << (QStringList() << "");
    QTest::newRow("crlf") << QByteArray("\r\n") << (QStringList() << "");
    QTest::newRow("oneline/nothing") << QByteArray("ole") << (QStringList() << "ole");
    QTest::newRow("oneline/lf") << QByteArray("ole\n") << (QStringList() << "ole");
    QTest::newRow("oneline/crlf") << QByteArray("ole\r\n") << (QStringList() << "ole");
    QTest::newRow("twolines/lf/lf") << QByteArray("ole\ndole\n") << (QStringList() << "ole" << "dole");
    QTest::newRow("twolines/crlf/crlf") << QByteArray("ole\r\ndole\r\n") << (QStringList() << "ole" << "dole");
    QTest::newRow("twolines/lf/crlf") << QByteArray("ole\ndole\r\n") << (QStringList() << "ole" << "dole");
    QTest::newRow("twolines/lf/nothing") << QByteArray("ole\ndole") << (QStringList() << "ole" << "dole");
    QTest::newRow("twolines/crlf/nothing") << QByteArray("ole\r\ndole") << (QStringList() << "ole" << "dole");
    QTest::newRow("threelines/lf/lf/lf") << QByteArray("ole\ndole\ndoffen\n") << (QStringList() << "ole" << "dole" << "doffen");
    QTest::newRow("threelines/crlf/crlf/crlf") << QByteArray("ole\r\ndole\r\ndoffen\r\n") << (QStringList() << "ole" << "dole" << "doffen");
    QTest::newRow("threelines/crlf/crlf/nothing") << QByteArray("ole\r\ndole\r\ndoffen") << (QStringList() << "ole" << "dole" << "doffen");

    if (!for_QString) {
        // utf-8
        QTest::newRow("utf8/twolines")
            << QByteArray("\xef\xbb\xbf"
                          "\x66\x67\x65\x0a"
                          "\x66\x67\x65\x0a", 11)
            << (QStringList() << "fge" << "fge");

        // utf-16
        // one line
        QTest::newRow("utf16-BE/nothing")
            << QByteArray("\xfe\xff"
                          "\x00\xe5\x00\x67\x00\x65", 8) << (QStringList() << "\345ge");
        QTest::newRow("utf16-LE/nothing")
            << QByteArray("\xff\xfe"
                          "\xe5\x00\x67\x00\x65\x00", 8) << (QStringList() << "\345ge");
        QTest::newRow("utf16-BE/lf")
            << QByteArray("\xfe\xff"
                          "\x00\xe5\x00\x67\x00\x65\x00\x0a", 10) << (QStringList() << "\345ge");
        QTest::newRow("utf16-LE/lf")
            << QByteArray("\xff\xfe"
                          "\xe5\x00\x67\x00\x65\x00\x0a\x00", 10) << (QStringList() << "\345ge");

        // two lines
        QTest::newRow("utf16-BE/twolines")
            << QByteArray("\xfe\xff"
                          "\x00\xe5\x00\x67\x00\x65\x00\x0a"
                          "\x00\xe5\x00\x67\x00\x65\x00\x0a", 18)
            << (QStringList() << "\345ge" << "\345ge");
        QTest::newRow("utf16-LE/twolines")
            << QByteArray("\xff\xfe"
                          "\xe5\x00\x67\x00\x65\x00\x0a\x00"
                          "\xe5\x00\x67\x00\x65\x00\x0a\x00", 18)
            << (QStringList() << "\345ge" << "\345ge");

        // three lines
        QTest::newRow("utf16-BE/threelines")
            << QByteArray("\xfe\xff"
                          "\x00\xe5\x00\x67\x00\x65\x00\x0a"
                          "\x00\xe5\x00\x67\x00\x65\x00\x0a"
                          "\x00\xe5\x00\x67\x00\x65\x00\x0a", 26)
            << (QStringList() << "\345ge" << "\345ge" << "\345ge");
        QTest::newRow("utf16-LE/threelines")
            << QByteArray("\xff\xfe"
                          "\xe5\x00\x67\x00\x65\x00\x0a\x00"
                          "\xe5\x00\x67\x00\x65\x00\x0a\x00"
                          "\xe5\x00\x67\x00\x65\x00\x0a\x00", 26)
            << (QStringList() << "\345ge" << "\345ge" << "\345ge");

        // utf-32
        QTest::newRow("utf32-BE/twolines")
            << QByteArray("\x00\x00\xfe\xff"
                          "\x00\x00\x00\xe5\x00\x00\x00\x67\x00\x00\x00\x65\x00\x00\x00\x0a"
                          "\x00\x00\x00\xe5\x00\x00\x00\x67\x00\x00\x00\x65\x00\x00\x00\x0a", 36)
            << (QStringList() << "\345ge" << "\345ge");
        QTest::newRow("utf32-LE/twolines")
            << QByteArray("\xff\xfe\x00\x00"
                          "\xe5\x00\x00\x00\x67\x00\x00\x00\x65\x00\x00\x00\x0a\x00\x00\x00"
                          "\xe5\x00\x00\x00\x67\x00\x00\x00\x65\x00\x00\x00\x0a\x00\x00\x00", 36)
            << (QStringList() << "\345ge" << "\345ge");
    }

    // partials
    QTest::newRow("cr") << QByteArray("\r") << (QStringList() << "");
    QTest::newRow("oneline/cr") << QByteArray("ole\r") << (QStringList() << "ole");
    if (!for_QString)
        QTest::newRow("utf16-BE/cr") << QByteArray("\xfe\xff\x00\xe5\x00\x67\x00\x65\x00\x0d", 10) << (QStringList() << "\345ge");
}

// ------------------------------------------------------------------------------
void tst_QTextStream::readLineFromDevice_data()
{
    generateLineData(false);
}

// ------------------------------------------------------------------------------
void tst_QTextStream::readLineFromDevice()
{
    QFETCH(QByteArray, data);
    QFETCH(QStringList, lines);

    QFile::remove(TestFileName);
    QFile file(TestFileName);
    QVERIFY(file.open(QFile::ReadWrite));
    QCOMPARE(file.write(data), qlonglong(data.size()));
    QVERIFY(file.flush());
    file.seek(0);

    QTextStream stream(&file);
    QStringList list;
    while (!stream.atEnd())
        list << stream.readLine();

    QCOMPARE(list, lines);
}

// ------------------------------------------------------------------------------
void tst_QTextStream::readLineMaxlen_data()
{
    QTest::addColumn<QString>("input");
    QTest::addColumn<QStringList>("lines");

    QTest::newRow("Hey")
        << QString("Hey")
        << (QStringList() << QString("Hey") << QString(""));
    QTest::newRow("Hey\\n")
        << QString("Hey\n")
        << (QStringList() << QString("Hey") << QString(""));
    QTest::newRow("HelloWorld")
        << QString("HelloWorld")
        << (QStringList() << QString("Hello") << QString("World"));
    QTest::newRow("Helo\\nWorlds")
        << QString("Helo\nWorlds")
        << (QStringList() << QString("Helo") << QString("World"));
    QTest::newRow("AAAAA etc.")
        << QString(16385, QLatin1Char('A'))
        << (QStringList() << QString("AAAAA") << QString("AAAAA"));
    QTest::newRow("multibyte string")
        << QString::fromUtf8("\341\233\222\341\233\226\341\232\251\341\232\271\341\232\242\341\233\232\341\232\240\n")
        << (QStringList() << QString::fromUtf8("\341\233\222\341\233\226\341\232\251\341\232\271\341\232\242")
            << QString::fromUtf8("\341\233\232\341\232\240"));
}

// ------------------------------------------------------------------------------
void tst_QTextStream::readLineMaxlen()
{
    QFETCH(QString, input);
    QFETCH(QStringList, lines);
    for (int i = 0; i < 2; ++i) {
        bool useDevice = (i == 1);
        QTextStream stream;
        QFile::remove("testfile");
        QFile file("testfile");
        if (useDevice) {
            file.open(QIODevice::ReadWrite);
            file.write(input.toUtf8());
            file.seek(0);
            stream.setDevice(&file);
            stream.setCodec("utf-8");
        } else {
            stream.setString(&input);
        }

        QStringList list;
        list << stream.readLine(5);
        list << stream.readLine(5);

        QCOMPARE(list, lines);
    }
}

// ------------------------------------------------------------------------------
void tst_QTextStream::readLinesFromBufferCRCR()
{
    QBuffer buffer;
    buffer.open(QIODevice::WriteOnly);
    QByteArray data("0123456789\r\r\n");

    for (int i = 0; i < 10000; ++i)
        buffer.write(data);

    buffer.close();
    if (buffer.open(QIODevice::ReadOnly|QIODevice::Text)) {
        QTextStream stream(&buffer);
        while (!stream.atEnd())
            QCOMPARE(stream.readLine(), QString("0123456789"));
    }
}

// ------------------------------------------------------------------------------
void tst_QTextStream::readLineFromString_data()
{
    generateLineData(true);
}

// ------------------------------------------------------------------------------
void tst_QTextStream::readLineFromString()
{
    QFETCH(QByteArray, data);
    QFETCH(QStringList, lines);

    QString dataString = data;

    QTextStream stream(&dataString, QIODevice::ReadOnly);
    QStringList list;
    while (!stream.atEnd())
        list << stream.readLine();

    QCOMPARE(list, lines);
}

// ------------------------------------------------------------------------------
void tst_QTextStream::readLineFromStringThenChangeString()
{
    QString first = "First string";
    QString second = "Second string";

    QTextStream stream(&first, QIODevice::ReadOnly);
    QString result = stream.readLine();
    QCOMPARE(first, result);

    stream.setString(&second, QIODevice::ReadOnly);
    result = stream.readLine();
    QCOMPARE(second, result);
}

// ------------------------------------------------------------------------------
void tst_QTextStream::setDevice()
{
    // Check that the read buffer is reset after setting a new device
    QByteArray data1("Hello World");
    QByteArray data2("How are you");

    QBuffer bufferOld(&data1);
    bufferOld.open(QIODevice::ReadOnly);

    QBuffer bufferNew(&data2);
    bufferNew.open(QIODevice::ReadOnly);

    QString text;
    QTextStream stream(&bufferOld);
    stream >> text;
    QCOMPARE(text, QString("Hello"));

    stream.setDevice(&bufferNew);
    stream >> text;
    QCOMPARE(text, QString("How"));
}

// ------------------------------------------------------------------------------
void tst_QTextStream::readLineFromTextDevice_data()
{
    generateLineData(false);
}

// ------------------------------------------------------------------------------
void tst_QTextStream::readLineFromTextDevice()
{
    QFETCH(QByteArray, data);
    QFETCH(QStringList, lines);

    for (int i = 0; i < 8; ++i) {
        QBuffer buffer(&data);
        if (i < 4)
            QVERIFY(buffer.open(QIODevice::ReadOnly | QIODevice::Text));
        else
            QVERIFY(buffer.open(QIODevice::ReadOnly));

        QTextStream stream(&buffer);
        QStringList list;
        while (!stream.atEnd()) {
            stream.pos(); // <- triggers side effects
            QString line;

            if (i & 1) {
                QChar c;
                while (!stream.atEnd()) {
                    stream >> c;
                    if (stream.status() == QTextStream::Ok) {
                        if (c != QLatin1Char('\n') && c != QLatin1Char('\r'))
                            line += c;
                        if (c == QLatin1Char('\n'))
                            break;
                    }
                }
            } else {
                line = stream.readLine();
            }

            if ((i & 3) == 3 && !QString(QTest::currentDataTag()).contains("utf16"))
                stream.seek(stream.pos());
            list << line;
        }
        QCOMPARE(list, lines);
    }
}

// ------------------------------------------------------------------------------
void tst_QTextStream::generateAllData(bool for_QString)
{
    QTest::addColumn<QByteArray>("input");
    QTest::addColumn<QString>("output");

    // latin-1
    QTest::newRow("empty") << QByteArray() << QString();
    QTest::newRow("latin1-a") << QByteArray("a") << QString("a");
    QTest::newRow("latin1-a\\r") << QByteArray("a\r") << QString("a\r");
    QTest::newRow("latin1-a\\r\\n") << QByteArray("a\r\n") << QString("a\r\n");
    QTest::newRow("latin1-a\\n") << QByteArray("a\n") << QString("a\n");

    // utf-16
    if (!for_QString) {
        // one line
        QTest::newRow("utf16-BE/nothing")
            << QByteArray("\xfe\xff"
                          "\x00\xe5\x00\x67\x00\x65", 8) << QString("\345ge");
        QTest::newRow("utf16-LE/nothing")
            << QByteArray("\xff\xfe"
                          "\xe5\x00\x67\x00\x65\x00", 8) << QString("\345ge");
        QTest::newRow("utf16-BE/lf")
            << QByteArray("\xfe\xff"
                          "\x00\xe5\x00\x67\x00\x65\x00\x0a", 10) << QString("\345ge\n");
        QTest::newRow("utf16-LE/lf")
            << QByteArray("\xff\xfe"
                          "\xe5\x00\x67\x00\x65\x00\x0a\x00", 10) << QString("\345ge\n");
        QTest::newRow("utf16-BE/crlf")
            << QByteArray("\xfe\xff"
                          "\x00\xe5\x00\x67\x00\x65\x00\x0d\x00\x0a", 12) << QString("\345ge\r\n");
        QTest::newRow("utf16-LE/crlf")
            << QByteArray("\xff\xfe"
                          "\xe5\x00\x67\x00\x65\x00\x0d\x00\x0a\x00", 12) << QString("\345ge\r\n");

        // two lines
        QTest::newRow("utf16-BE/twolines")
            << QByteArray("\xfe\xff"
                          "\x00\xe5\x00\x67\x00\x65\x00\x0a"
                          "\x00\xe5\x00\x67\x00\x65\x00\x0a", 18)
            << QString("\345ge\n\345ge\n");
        QTest::newRow("utf16-LE/twolines")
            << QByteArray("\xff\xfe"
                          "\xe5\x00\x67\x00\x65\x00\x0a\x00"
                          "\xe5\x00\x67\x00\x65\x00\x0a\x00", 18)
            << QString("\345ge\n\345ge\n");

        // three lines
        QTest::newRow("utf16-BE/threelines")
            << QByteArray("\xfe\xff"
                          "\x00\xe5\x00\x67\x00\x65\x00\x0a"
                          "\x00\xe5\x00\x67\x00\x65\x00\x0a"
                          "\x00\xe5\x00\x67\x00\x65\x00\x0a", 26)
            << QString("\345ge\n\345ge\n\345ge\n");
        QTest::newRow("utf16-LE/threelines")
            << QByteArray("\xff\xfe"
                          "\xe5\x00\x67\x00\x65\x00\x0a\x00"
                          "\xe5\x00\x67\x00\x65\x00\x0a\x00"
                          "\xe5\x00\x67\x00\x65\x00\x0a\x00", 26)
            << QString("\345ge\n\345ge\n\345ge\n");
    }
}

// ------------------------------------------------------------------------------
void tst_QTextStream::readLineUntilNull()
{
    QFile file(SRCDIR "rfc3261.txt");
    QVERIFY(file.open(QFile::ReadOnly));

    QTextStream stream(&file);
    for (int i = 0; i < 15066; ++i) {
        QString line = stream.readLine();
        QVERIFY(!line.isNull());
        QVERIFY(!line.isNull());
    }
    QVERIFY(!stream.readLine().isNull());
    QVERIFY(stream.readLine().isNull());
}

// ------------------------------------------------------------------------------
void tst_QTextStream::readAllFromDevice_data()
{
    generateAllData(false);
}

// ------------------------------------------------------------------------------
void tst_QTextStream::readAllFromDevice()
{
    QFETCH(QByteArray, input);
    QFETCH(QString, output);

    QBuffer buffer(&input);
    buffer.open(QBuffer::ReadOnly);

    QTextStream stream(&buffer);
    QCOMPARE(stream.readAll(), output);
}

// ------------------------------------------------------------------------------
void tst_QTextStream::readAllFromString_data()
{
    generateAllData(true);
}

// ------------------------------------------------------------------------------
void tst_QTextStream::readAllFromString()
{
    QFETCH(QByteArray, input);
    QFETCH(QString, output);

    QString str = input;

    QTextStream stream(&str);
    QCOMPARE(stream.readAll(), output);
}

// ------------------------------------------------------------------------------
void tst_QTextStream::skipWhiteSpace_data()
{
    QTest::addColumn<QByteArray>("input");
    QTest::addColumn<QChar>("output");

    // latin1
    QTest::newRow("empty") << QByteArray() << QChar('\0');
    QTest::newRow(" one") << QByteArray(" one") << QChar('o');
    QTest::newRow("\\none") << QByteArray("\none") << QChar('o');
    QTest::newRow("\\n one") << QByteArray("\n one") << QChar('o');
    QTest::newRow(" \\r\\n one") << QByteArray(" \r\n one") << QChar('o');

    // utf-16
    QTest::newRow("utf16-BE (empty)") << QByteArray("\xfe\xff", 2) << QChar('\0');
    QTest::newRow("utf16-BE ( one)") << QByteArray("\xfe\xff\x00 \x00o\x00n\x00e", 10) << QChar('o');
    QTest::newRow("utf16-BE (\\none)") << QByteArray("\xfe\xff\x00\n\x00o\x00n\x00e", 10) << QChar('o');
    QTest::newRow("utf16-BE (\\n one)") << QByteArray("\xfe\xff\x00\n\x00 \x00o\x00n\x00e", 12) << QChar('o');
    QTest::newRow("utf16-BE ( \\r\\n one)") << QByteArray("\xfe\xff\x00 \x00\r\x00\n\x00 \x00o\x00n\x00e", 20) << QChar('o');

    QTest::newRow("utf16-LE (empty)") << QByteArray("\xff\xfe", 2) << QChar('\0');
    QTest::newRow("utf16-LE ( one)") << QByteArray("\xff\xfe \x00o\x00n\x00e\x00", 10) << QChar('o');
    QTest::newRow("utf16-LE (\\none)") << QByteArray("\xff\xfe\n\x00o\x00n\x00e\x00", 10) << QChar('o');
    QTest::newRow("utf16-LE (\\n one)") << QByteArray("\xff\xfe\n\x00 \x00o\x00n\x00e\x00", 12) << QChar('o');
    QTest::newRow("utf16-LE ( \\r\\n one)") << QByteArray("\xff\xfe \x00\r\x00\n\x00 \x00o\x00n\x00e\x00", 20) << QChar('o');
}

// ------------------------------------------------------------------------------
void tst_QTextStream::skipWhiteSpace()
{
    QFETCH(QByteArray, input);
    QFETCH(QChar, output);

    QBuffer buffer(&input);
    buffer.open(QBuffer::ReadOnly);

    QTextStream stream(&buffer);
    stream.skipWhiteSpace();

    QChar tmp;
    stream >> tmp;

    QCOMPARE(tmp, output);

    QString str = input;
    QTextStream stream2(&input);
    stream2.skipWhiteSpace();

    stream2 >> tmp;

    QCOMPARE(tmp, output);
}

// ------------------------------------------------------------------------------
void tst_QTextStream::lineCount_data()
{
    QTest::addColumn<QByteArray>("data");
    QTest::addColumn<int>("lineCount");

    QTest::newRow("empty") << QByteArray() << 0;
    QTest::newRow("oneline") << QByteArray("a\n") << 1;
    QTest::newRow("twolines") << QByteArray("a\nb\n") << 2;
    QTest::newRow("oneemptyline") << QByteArray("\n") << 1;
    QTest::newRow("twoemptylines") << QByteArray("\n\n") << 2;
    QTest::newRow("buffersize-1 line") << QByteArray(16382, '\n') << 16382;
    QTest::newRow("buffersize line") << QByteArray(16383, '\n') << 16383;
    QTest::newRow("buffersize+1 line") << QByteArray(16384, '\n') << 16384;
    QTest::newRow("buffersize+2 line") << QByteArray(16385, '\n') << 16385;

    QFile file(SRCDIR "rfc3261.txt"); file.open(QFile::ReadOnly);
    QTest::newRow("rfc3261") << file.readAll() << 15067;
}

// ------------------------------------------------------------------------------
void tst_QTextStream::lineCount()
{
    QFETCH(QByteArray, data);
    QFETCH(int, lineCount);

    QFile out("out.txt");
    out.open(QFile::WriteOnly);

    QTextStream lineReader(data);
    int lines = 0;
    while (!lineReader.atEnd()) {
        QString line = lineReader.readLine();
        out.write(line.toLatin1() + "\n");
        ++lines;
    }

    out.close();
    QCOMPARE(lines, lineCount);
}

// ------------------------------------------------------------------------------
void tst_QTextStream::performance()
{
    // Phase #1 - test speed of reading a huge text file with QFile.
    QTime stopWatch;

    int elapsed1 = 0;
    int elapsed2 = 0;

        stopWatch.restart();
        int nlines1 = 0;
        QFile file(SRCDIR "rfc3261.txt");
        QVERIFY(file.open(QFile::ReadOnly));

        while (!file.atEnd()) {
            ++nlines1;
            file.readLine();
        }

        elapsed1 += stopWatch.elapsed();
        stopWatch.restart();

        int nlines2 = 0;
        QFile file2(SRCDIR "rfc3261.txt");
        QVERIFY(file2.open(QFile::ReadOnly));

        QTextStream stream(&file2);
        while (!stream.atEnd()) {
            ++nlines2;
            stream.readLine();
        }

        elapsed2 += stopWatch.elapsed();
        QCOMPARE(nlines1, nlines2);

    qDebug("QFile used %.2f seconds to read the file",
           elapsed1 / 1000.0);

    qDebug("QTextStream used %.2f seconds to read the file",
           elapsed2 / 1000.0);
    if (elapsed2 > elapsed1) {
        qDebug("QFile is %.2fx faster than QTextStream",
               double(elapsed2) / double(elapsed1));
    } else {
        qDebug("QTextStream is %.2fx faster than QFile",
               double(elapsed1) / double(elapsed2));
    }
}

// ------------------------------------------------------------------------------
void tst_QTextStream::hexTest_data()
{
    QTest::addColumn<qlonglong>("number");
    QTest::addColumn<QByteArray>("data");

    QTest::newRow("0") << Q_INT64_C(0) << QByteArray("0x0");
    QTest::newRow("1") << Q_INT64_C(1) << QByteArray("0x1");
    QTest::newRow("2") << Q_INT64_C(2) << QByteArray("0x2");
    QTest::newRow("3") << Q_INT64_C(3) << QByteArray("0x3");
    QTest::newRow("4") << Q_INT64_C(4) << QByteArray("0x4");
    QTest::newRow("5") << Q_INT64_C(5) << QByteArray("0x5");
    QTest::newRow("6") << Q_INT64_C(6) << QByteArray("0x6");
    QTest::newRow("7") << Q_INT64_C(7) << QByteArray("0x7");
    QTest::newRow("8") << Q_INT64_C(8) << QByteArray("0x8");
    QTest::newRow("9") << Q_INT64_C(9) << QByteArray("0x9");
    QTest::newRow("a") << Q_INT64_C(0xa) << QByteArray("0xa");
    QTest::newRow("b") << Q_INT64_C(0xb) << QByteArray("0xb");
    QTest::newRow("c") << Q_INT64_C(0xc) << QByteArray("0xc");
    QTest::newRow("d") << Q_INT64_C(0xd) << QByteArray("0xd");
    QTest::newRow("e") << Q_INT64_C(0xe) << QByteArray("0xe");
    QTest::newRow("f") << Q_INT64_C(0xf) << QByteArray("0xf");
    QTest::newRow("-1") << Q_INT64_C(-1) << QByteArray("-0x1");
    QTest::newRow("0xffffffff") << Q_INT64_C(0xffffffff) << QByteArray("0xffffffff");
    QTest::newRow("0xfffffffffffffffe") << Q_INT64_C(0xfffffffffffffffe) << QByteArray("-0x2");
    QTest::newRow("0xffffffffffffffff") << Q_INT64_C(0xffffffffffffffff) << QByteArray("-0x1");
    QTest::newRow("0x7fffffffffffffff") << Q_INT64_C(0x7fffffffffffffff) << QByteArray("0x7fffffffffffffff");
}

// ------------------------------------------------------------------------------
void tst_QTextStream::hexTest()
{
    QFETCH(qlonglong, number);
    QFETCH(QByteArray, data);

    QByteArray array;
    QTextStream stream(&array);

    stream << showbase << hex << number;
    stream.flush();
    QCOMPARE(array, data);
}

// ------------------------------------------------------------------------------
void tst_QTextStream::binTest_data()
{
    QTest::addColumn<int>("number");
    QTest::addColumn<QByteArray>("data");

    QTest::newRow("0") << 0 << QByteArray("0b0");
    QTest::newRow("1") << 1 << QByteArray("0b1");
    QTest::newRow("2") << 2 << QByteArray("0b10");
    QTest::newRow("5") << 5 << QByteArray("0b101");
    QTest::newRow("-1") << -1 << QByteArray("-0b1");
    QTest::newRow("11111111") << 0xff << QByteArray("0b11111111");
    QTest::newRow("1111111111111111") << 0xffff << QByteArray("0b1111111111111111");
    QTest::newRow("1111111011111110") << 0xfefe << QByteArray("0b1111111011111110");
}

// ------------------------------------------------------------------------------
void tst_QTextStream::binTest()
{
    QFETCH(int, number);
    QFETCH(QByteArray, data);

    QByteArray array;
    QTextStream stream(&array);

    stream << showbase << bin << number;
    stream.flush();
    QCOMPARE(array.constData(), data.constData());
}

// ------------------------------------------------------------------------------
void tst_QTextStream::octTest_data()
{
    QTest::addColumn<int>("number");
    QTest::addColumn<QByteArray>("data");

    QTest::newRow("0") << 0 << QByteArray("00");
}

// ------------------------------------------------------------------------------
void tst_QTextStream::octTest()
{
    QFETCH(int, number);
    QFETCH(QByteArray, data);

    QByteArray array;
    QTextStream stream(&array);

    stream << showbase << oct << number;
    stream.flush();
    QCOMPARE(array, data);
}

// ------------------------------------------------------------------------------
void tst_QTextStream::zeroTermination()
{
    QTextStream stream;
    char c = '@';

    QTest::ignoreMessage(QtWarningMsg, "QTextStream: No device");
    stream >> c;
    QCOMPARE(c, '\0');

    c = '@';

    QTest::ignoreMessage(QtWarningMsg, "QTextStream: No device");
    stream >> &c;
    QCOMPARE(c, '\0');
}

// ------------------------------------------------------------------------------
void tst_QTextStream::ws_manipulator()
{
    {
        QString string = "a b c d";
        QTextStream stream(&string);

        char a, b, c, d;
        stream >> a >> b >> c >> d;
        QCOMPARE(a, 'a');
        QCOMPARE(b, ' ');
        QCOMPARE(c, 'b');
        QCOMPARE(d, ' ');
    }
    {
        QString string = "a b c d";
        QTextStream stream(&string);

        char a, b, c, d;
        stream >> a >> ws >> b >> ws >> c >> ws >> d;
        QCOMPARE(a, 'a');
        QCOMPARE(b, 'b');
        QCOMPARE(c, 'c');
        QCOMPARE(d, 'd');
    }
}

// ------------------------------------------------------------------------------
void tst_QTextStream::stillOpenWhenAtEnd()
{
    int argc = 0;
    char **argv = 0;
    QCoreApplication app(argc, argv);

    QFile file(SRCDIR "tst_qtextstream.cpp");
    QVERIFY(file.open(QFile::ReadOnly));

    QTextStream stream(&file);
    while (!stream.readLine().isNull()) {}
    QVERIFY(file.isOpen());

#ifdef Q_OS_WINCE
    QSKIP("Qt/CE: Cannot test network on emulator", SkipAll);
#endif
    QTcpSocket socket;
    socket.connectToHost(QtNetworkSettings::serverName(), 143);
#if defined(Q_OS_SYMBIAN)
    // This number is determined in an arbitrary way; whatever it takes
    // to make the test pass.
    QVERIFY(socket.waitForReadyRead(30000));
#else
    QVERIFY(socket.waitForReadyRead(5000));
#endif

    QTextStream stream2(&socket);
    while (!stream2.readLine().isNull()) {}
    QVERIFY(socket.isOpen());
}

// ------------------------------------------------------------------------------
void tst_QTextStream::readNewlines_data()
{
    QTest::addColumn<QByteArray>("input");
    QTest::addColumn<QString>("output");

    QTest::newRow("empty") << QByteArray() << QString();
    QTest::newRow("\\r\\n") << QByteArray("\r\n") << QString("\n");
    QTest::newRow("\\r\\r\\n") << QByteArray("\r\r\n") << QString("\n");
    QTest::newRow("\\r\\n\\r\\n") << QByteArray("\r\n\r\n") << QString("\n\n");
    QTest::newRow("\\n") << QByteArray("\n") << QString("\n");
    QTest::newRow("\\n\\n") << QByteArray("\n\n") << QString("\n\n");
}

// ------------------------------------------------------------------------------
void tst_QTextStream::readNewlines()
{
    QFETCH(QByteArray, input);
    QFETCH(QString, output);

    QBuffer buffer(&input);
    buffer.open(QBuffer::ReadOnly | QBuffer::Text);
    QTextStream stream(&buffer);
    QCOMPARE(stream.readAll(), output);
}

// ------------------------------------------------------------------------------
void tst_QTextStream::seek()
{
    QFile file(SRCDIR "rfc3261.txt");
    QVERIFY(file.open(QFile::ReadOnly));

    QTextStream stream(&file);
    QString tmp;
    stream >> tmp;
    QCOMPARE(tmp, QString::fromLatin1("Network"));

    // QTextStream::seek(0) should both clear its internal read/write buffers
    // and seek the device.
    for (int i = 0; i < 4; ++i) {
        stream.seek(12 + i);
        stream >> tmp;
        QCOMPARE(tmp, QString("Network").mid(i));
    }
    for (int i = 0; i < 4; ++i) {
        stream.seek(16 - i);
        stream >> tmp;
        QCOMPARE(tmp, QString("Network").mid(4 - i));
    }
    stream.seek(139181);
    stream >> tmp;
    QCOMPARE(tmp, QString("information"));
    stream.seek(388683);
    stream >> tmp;
    QCOMPARE(tmp, QString("telephone"));

    // Also test this with a string
    QString words = QLatin1String("thisisa");
    QTextStream stream2(&words, QIODevice::ReadOnly);
    stream2 >> tmp;
    QCOMPARE(tmp, QString::fromLatin1("thisisa"));

    for (int i = 0; i < 4; ++i) {
        stream2.seek(i);
        stream2 >> tmp;
        QCOMPARE(tmp, QString("thisisa").mid(i));
    }
    for (int i = 0; i < 4; ++i) {
        stream2.seek(4 - i);
        stream2 >> tmp;
        QCOMPARE(tmp, QString("thisisa").mid(4 - i));
    }
}

// ------------------------------------------------------------------------------
void tst_QTextStream::pos()
{
    int argc = 1;
    QCoreApplication app(argc, 0);
    {
        // Strings
        QString str("this is a test");
        QTextStream stream(&str, QIODevice::ReadWrite);

        QCOMPARE(stream.pos(), qint64(0));
        for (int i = 0; i <= str.size(); ++i) {
            QVERIFY(stream.seek(i));
            QCOMPARE(stream.pos(), qint64(i));
        }
        for (int j = str.size(); j >= 0; --j) {
            QVERIFY(stream.seek(j));
            QCOMPARE(stream.pos(), qint64(j));
        }

        QVERIFY(stream.seek(0));

        QChar ch;
        stream >> ch;
        QCOMPARE(ch, QChar('t'));

        QCOMPARE(stream.pos(), qint64(1));
        QVERIFY(stream.seek(1));
        QCOMPARE(stream.pos(), qint64(1));
        QVERIFY(stream.seek(0));

        QString strtmp;
        stream >> strtmp;
        QCOMPARE(strtmp, QString("this"));

        QCOMPARE(stream.pos(), qint64(4));
        stream.seek(0);
        stream.seek(4);

        stream >> ch;
        QCOMPARE(ch, QChar(' '));
        QCOMPARE(stream.pos(), qint64(5));

        stream.seek(10);
        stream >> strtmp;
        QCOMPARE(strtmp, QString("test"));
        QCOMPARE(stream.pos(), qint64(14));
    }
    {
        // Latin1 device
        QFile file(SRCDIR "rfc3261.txt");
        QVERIFY(file.open(QIODevice::ReadOnly));

        QTextStream stream(&file);

        QCOMPARE(stream.pos(), qint64(0));

        for (int i = 0; i <= file.size(); i += 7) {
            QVERIFY(stream.seek(i));
            QCOMPARE(stream.pos(), qint64(i));
        }
        for (int j = file.size(); j >= 0; j -= 7) {
            QVERIFY(stream.seek(j));
            QCOMPARE(stream.pos(), qint64(j));
        }

        stream.seek(0);

        QString strtmp;
        stream >> strtmp;
        QCOMPARE(strtmp, QString("Network"));
        QCOMPARE(stream.pos(), qint64(19));

        stream.seek(2598);
        QCOMPARE(stream.pos(), qint64(2598));
        stream >> strtmp;
        QCOMPARE(stream.pos(), qint64(2607));
        QCOMPARE(strtmp, QString("locations"));
    }
    {
        // Shift-JIS device
        for (int i = 0; i < 2; ++i) {
            QFile file(SRCDIR "shift-jis.txt");
            if (i == 0)
                QVERIFY(file.open(QIODevice::ReadOnly));
            else
                QVERIFY(file.open(QIODevice::ReadOnly | QIODevice::Text));

            QTextStream stream(&file);
            stream.setCodec("Shift-JIS");
            QVERIFY(stream.codec());

            QCOMPARE(stream.pos(), qint64(0));
            for (int i = 0; i <= file.size(); i += 7) {
                QVERIFY(stream.seek(i));
                QCOMPARE(stream.pos(), qint64(i));
            }
            for (int j = file.size(); j >= 0; j -= 7) {
                QVERIFY(stream.seek(j));
                QCOMPARE(stream.pos(), qint64(j));
            }

            stream.seek(2089);
            QString strtmp;
            stream >> strtmp;
            QCOMPARE(strtmp, QString("AUnicode"));
            QCOMPARE(stream.pos(), qint64(2097));

            stream.seek(43325);
            stream >> strtmp;
            QCOMPARE(strtmp, QString("Shift-JIS"));
            stream >> strtmp;
            QCOMPARE(strtmp, QString::fromUtf8("\343\201\247\346\233\270\343\201\213\343\202\214\343\201\237"));
            QCOMPARE(stream.pos(), qint64(43345));
            stream >> strtmp;
            QCOMPARE(strtmp, QString("POD"));
            QCOMPARE(stream.pos(), qint64(43349));
        }
    }
}

// ------------------------------------------------------------------------------
void tst_QTextStream::pos2()
{
    QByteArray data("abcdef\r\nghijkl\r\n");
    QBuffer buffer(&data);
    QVERIFY(buffer.open(QIODevice::ReadOnly | QIODevice::Text));

    QTextStream stream(&buffer);

    QChar ch;

    QCOMPARE(stream.pos(), qint64(0));
    stream >> ch;
    QCOMPARE(ch, QChar('a'));
    QCOMPARE(stream.pos(), qint64(1));

    QString str;
    stream >> str;
    QCOMPARE(str, QString("bcdef"));
    QCOMPARE(stream.pos(), qint64(6));

    stream >> str;
    QCOMPARE(str, QString("ghijkl"));
    QCOMPARE(stream.pos(), qint64(14));

    // Seek back and try again
    stream.seek(1);
    QCOMPARE(stream.pos(), qint64(1));
    stream >> str;
    QCOMPARE(str, QString("bcdef"));
    QCOMPARE(stream.pos(), qint64(6));

    stream.seek(6);
    stream >> str;
    QCOMPARE(str, QString("ghijkl"));
    QCOMPARE(stream.pos(), qint64(14));
}

// ------------------------------------------------------------------------------
void tst_QTextStream::readStdin()
{
#if defined(Q_OS_WINCE) || defined(Q_OS_SYMBIAN)
    QSKIP("Qt/CE and Symbian have no stdin/out support for processes", SkipAll);
#endif
    QProcess stdinProcess;
    stdinProcess.start("stdinProcess/stdinProcess");
    stdinProcess.setReadChannel(QProcess::StandardError);

    QTextStream stream(&stdinProcess);
    stream << "1" << endl;
    stream << "2" << endl;
    stream << "3" << endl;

    stdinProcess.closeWriteChannel();

    QVERIFY(stdinProcess.waitForFinished(5000));

    int a, b, c;
    stream >> a >> b >> c;
    QCOMPARE(a, 1);
    QCOMPARE(b, 2);
    QCOMPARE(c, 3);
}

// ------------------------------------------------------------------------------
void tst_QTextStream::readAllFromStdin()
{
#if defined(Q_OS_WINCE) || defined(Q_OS_SYMBIAN)
    QSKIP("Qt/CE and Symbian have no stdin/out support for processes", SkipAll);
#endif
    QProcess stdinProcess;
    stdinProcess.start("readAllStdinProcess/readAllStdinProcess", QIODevice::ReadWrite | QIODevice::Text);
    stdinProcess.setReadChannel(QProcess::StandardError);

    QTextStream stream(&stdinProcess);
    stream.setCodec("ISO-8859-1");
    stream << "hello world" << flush;

    stdinProcess.closeWriteChannel();

    QVERIFY(stdinProcess.waitForFinished(5000));
    QChar quoteChar('"');
    QCOMPARE(stream.readAll(), QString::fromLatin1("%1hello world%2 \n").arg(quoteChar).arg(quoteChar));
}

// ------------------------------------------------------------------------------
void tst_QTextStream::readLineFromStdin()
{
#if defined(Q_OS_WINCE) || defined(Q_OS_SYMBIAN)
    QSKIP("Qt/CE and Symbian have no stdin/out support for processes", SkipAll);
#endif
    QProcess stdinProcess;
    stdinProcess.start("readLineStdinProcess/readLineStdinProcess", QIODevice::ReadWrite | QIODevice::Text);
    stdinProcess.setReadChannel(QProcess::StandardError);

    stdinProcess.write("abc\n");
    QVERIFY(stdinProcess.waitForReadyRead(5000));
    QCOMPARE(stdinProcess.readAll().data(), QByteArray("abc").data());

    stdinProcess.write("def\n");
    QVERIFY(stdinProcess.waitForReadyRead(5000));
    QCOMPARE(stdinProcess.readAll(), QByteArray("def"));

    stdinProcess.closeWriteChannel();

    QVERIFY(stdinProcess.waitForFinished(5000));
}

// ------------------------------------------------------------------------------
void tst_QTextStream::read()
{
    {
        QFile::remove("testfile");
        QFile file("testfile");
        file.open(QFile::WriteOnly);
        file.write("4.15 abc ole");
        file.close();

        QVERIFY(file.open(QFile::ReadOnly));
        QTextStream stream(&file);
        QCOMPARE(stream.read(0), QString(""));
        QCOMPARE(stream.read(4), QString("4.15"));
        QCOMPARE(stream.read(4), QString(" abc"));
        stream.seek(1);
        QCOMPARE(stream.read(4), QString(".15 "));
        stream.seek(1);
        QCOMPARE(stream.read(4), QString(".15 "));
        stream.seek(2);
        QCOMPARE(stream.read(4), QString("15 a"));
        // ### add tests for reading \r\n etc..
    }

    {
        // File larger than QTEXTSTREAM_BUFFERSIZE
        QFile::remove("testfile");
        QFile file("testfile");
        file.open(QFile::WriteOnly);
        for (int i = 0; i < 16384 / 8; ++i)
            file.write("01234567");
        file.write("0");
        file.close();

        QVERIFY(file.open(QFile::ReadOnly));
        QTextStream stream(&file);
        QCOMPARE(stream.read(10), QString("0123456701"));
        QCOMPARE(stream.read(10), QString("2345670123"));
        QCOMPARE(stream.readAll().size(), 16385-20);
    }
}

// ------------------------------------------------------------------------------
void tst_QTextStream::qbool()
{
    QString s;
    QTextStream stream(&s);
    stream << s.contains(QString("hei"));
    QCOMPARE(s, QString("0"));
}

// ------------------------------------------------------------------------------
void tst_QTextStream::forcePoint()
{
    QString str;
    QTextStream stream(&str);
    stream << fixed << forcepoint << 1.0 << " " << 1 << " " << 0 << " " << -1.0 << " " << -1;
    QCOMPARE(str, QString("1.000000 1 0 -1.000000 -1"));

    str.clear();
    stream.seek(0);
    stream << scientific << forcepoint << 1.0 << " " << 1 << " " << 0 << " " << -1.0 << " " << -1;
    QCOMPARE(str, QString("1.000000e+00 1 0 -1.000000e+00 -1"));

    str.clear();
    stream.seek(0);
    stream.setRealNumberNotation(QTextStream::SmartNotation);
    stream << forcepoint << 1.0 << " " << 1 << " " << 0 << " " << -1.0 << " " << -1;
    QCOMPARE(str, QString("1.00000 1 0 -1.00000 -1"));

}

// ------------------------------------------------------------------------------
void tst_QTextStream::forceSign()
{
    QString str;
    QTextStream stream(&str);
    stream << forcesign << 1.2 << " " << -1.2 << " " << 0;
    QCOMPARE(str, QString("+1.2 -1.2 +0"));
}

// ------------------------------------------------------------------------------
void tst_QTextStream::read0d0d0a()
{
    QFile file("task113817.txt");
    file.open(QIODevice::ReadOnly | QIODevice::Text);

    QTextStream stream(&file);
    while (!stream.atEnd())
        stream.readLine();
}

// ------------------------------------------------------------------------------

Q_DECLARE_METATYPE(QTextStreamFunction);

QTextStream &noop(QTextStream &s) { return s; }

void tst_QTextStream::numeralCase_data()
{
    QTextStreamFunction noop_ = noop;
    QTextStreamFunction bin_  = bin;
    QTextStreamFunction oct_  = oct;
    QTextStreamFunction hex_  = hex;
    QTextStreamFunction base  = showbase;
    QTextStreamFunction ucb   = uppercasebase;
    QTextStreamFunction lcb   = lowercasebase;
    QTextStreamFunction ucd   = uppercasedigits;
    QTextStreamFunction lcd   = lowercasedigits;

    QTest::addColumn<QTextStreamFunction>("func1");
    QTest::addColumn<QTextStreamFunction>("func2");
    QTest::addColumn<QTextStreamFunction>("func3");
    QTest::addColumn<QTextStreamFunction>("func4");
    QTest::addColumn<int>("value");
    QTest::addColumn<QString>("expected");
    QTest::newRow("dec 1") << noop_ << noop_ << noop_ << noop_ << 31 << "31";
    QTest::newRow("dec 2") << noop_ << base  << noop_ << noop_ << 31 << "31";

    QTest::newRow("hex 1")  << hex_  << noop_ << noop_ << noop_ << 31 << "1f";
    QTest::newRow("hex 2")  << hex_  << noop_ << noop_ << lcd   << 31 << "1f";
    QTest::newRow("hex 3")  << hex_  << noop_ << ucb   << noop_ << 31 << "1f";
    QTest::newRow("hex 4")  << hex_  << noop_ << noop_ << ucd   << 31 << "1F";
    QTest::newRow("hex 5")  << hex_  << noop_ << lcb   << ucd   << 31 << "1F";
    QTest::newRow("hex 6")  << hex_  << noop_ << ucb   << ucd   << 31 << "1F";
    QTest::newRow("hex 7")  << hex_  << base  << noop_ << noop_ << 31 << "0x1f";
    QTest::newRow("hex 8")  << hex_  << base  << lcb   << lcd   << 31 << "0x1f";
    QTest::newRow("hex 9")  << hex_  << base  << ucb   << noop_ << 31 << "0X1f";
    QTest::newRow("hex 10") << hex_  << base  << ucb   << lcd   << 31 << "0X1f";
    QTest::newRow("hex 11") << hex_  << base  << noop_ << ucd   << 31 << "0x1F";
    QTest::newRow("hex 12") << hex_  << base  << lcb   << ucd   << 31 << "0x1F";
    QTest::newRow("hex 13") << hex_  << base  << ucb   << ucd   << 31 << "0X1F";

    QTest::newRow("bin 1") << bin_  << noop_ << noop_ << noop_ << 31 << "11111";
    QTest::newRow("bin 2") << bin_  << base  << noop_ << noop_ << 31 << "0b11111";
    QTest::newRow("bin 3") << bin_  << base  << lcb   << noop_ << 31 << "0b11111";
    QTest::newRow("bin 4") << bin_  << base  << ucb   << noop_ << 31 << "0B11111";
    QTest::newRow("bin 5") << bin_  << base  << noop_ << ucd   << 31 << "0b11111";
    QTest::newRow("bin 6") << bin_  << base  << lcb   << ucd   << 31 << "0b11111";
    QTest::newRow("bin 7") << bin_  << base  << ucb   << ucd   << 31 << "0B11111";

    QTest::newRow("oct 1") << oct_  << noop_ << noop_ << noop_ << 31 << "37";
    QTest::newRow("oct 2") << oct_  << base  << noop_ << noop_ << 31 << "037";
}

// From Task 125496
void tst_QTextStream::numeralCase()
{
    QFETCH(QTextStreamFunction, func1);
    QFETCH(QTextStreamFunction, func2);
    QFETCH(QTextStreamFunction, func3);
    QFETCH(QTextStreamFunction, func4);
    QFETCH(int, value);
    QFETCH(QString, expected);

    QString str;
    QTextStream stream(&str);
    stream << func1 << func2 << func3 << func4 << value;
    QCOMPARE(str, expected);
}

// ------------------------------------------------------------------------------
void tst_QTextStream::nanInf()
{
    // Cannot use test data in this function, as comparing nans and infs isn't
    // well defined.
    QString str("nan NAN nAn +nan +NAN +nAn -nan -NAN -nAn"
                " inf INF iNf +inf +INF +iNf -inf -INF -iNf");

    QTextStream stream(&str);

    double tmpD = 0;
    stream >> tmpD; QVERIFY(qIsNaN(tmpD)); tmpD = 0;
    stream >> tmpD; QVERIFY(qIsNaN(tmpD)); tmpD = 0;
    stream >> tmpD; QVERIFY(qIsNaN(tmpD)); tmpD = 0;
    stream >> tmpD; QVERIFY(qIsNaN(tmpD)); tmpD = 0;
    stream >> tmpD; QVERIFY(qIsNaN(tmpD)); tmpD = 0;
    stream >> tmpD; QVERIFY(qIsNaN(tmpD)); tmpD = 0;
    stream >> tmpD; QVERIFY(qIsNaN(tmpD)); tmpD = 0;
    stream >> tmpD; QVERIFY(qIsNaN(tmpD)); tmpD = 0;
    stream >> tmpD; QVERIFY(qIsNaN(tmpD)); tmpD = 0;
    stream >> tmpD; QVERIFY(qIsInf(tmpD)); QVERIFY(tmpD > 0); tmpD = 0;
    stream >> tmpD; QVERIFY(qIsInf(tmpD)); QVERIFY(tmpD > 0); tmpD = 0;
    stream >> tmpD; QVERIFY(qIsInf(tmpD)); QVERIFY(tmpD > 0); tmpD = 0;
    stream >> tmpD; QVERIFY(qIsInf(tmpD)); QVERIFY(tmpD > 0); tmpD = 0;
    stream >> tmpD; QVERIFY(qIsInf(tmpD)); QVERIFY(tmpD > 0); tmpD = 0;
    stream >> tmpD; QVERIFY(qIsInf(tmpD)); QVERIFY(tmpD > 0); tmpD = 0;
    stream >> tmpD; QVERIFY(qIsInf(tmpD)); QVERIFY(tmpD < 0); tmpD = 0;
    stream >> tmpD; QVERIFY(qIsInf(tmpD)); QVERIFY(tmpD < 0); tmpD = 0;
    stream >> tmpD; QVERIFY(qIsInf(tmpD)); QVERIFY(tmpD < 0); tmpD = 0;

    stream.seek(0);

    float tmpF = 0;
    stream >> tmpF; QVERIFY(qIsNaN(tmpF)); tmpD = 0;
    stream >> tmpF; QVERIFY(qIsNaN(tmpF)); tmpD = 0;
    stream >> tmpF; QVERIFY(qIsNaN(tmpF)); tmpD = 0;
    stream >> tmpF; QVERIFY(qIsNaN(tmpF)); tmpD = 0;
    stream >> tmpF; QVERIFY(qIsNaN(tmpF)); tmpD = 0;
    stream >> tmpF; QVERIFY(qIsNaN(tmpF)); tmpD = 0;
    stream >> tmpF; QVERIFY(qIsNaN(tmpF)); tmpD = 0;
    stream >> tmpF; QVERIFY(qIsNaN(tmpF)); tmpD = 0;
    stream >> tmpF; QVERIFY(qIsNaN(tmpF)); tmpD = 0;
    stream >> tmpF; QVERIFY(qIsInf(tmpF)); QVERIFY(tmpF > 0); tmpD = 0;
    stream >> tmpF; QVERIFY(qIsInf(tmpF)); QVERIFY(tmpF > 0); tmpD = 0;
    stream >> tmpF; QVERIFY(qIsInf(tmpF)); QVERIFY(tmpF > 0); tmpD = 0;
    stream >> tmpF; QVERIFY(qIsInf(tmpF)); QVERIFY(tmpF > 0); tmpD = 0;
    stream >> tmpF; QVERIFY(qIsInf(tmpF)); QVERIFY(tmpF > 0); tmpD = 0;
    stream >> tmpF; QVERIFY(qIsInf(tmpF)); QVERIFY(tmpF > 0); tmpD = 0;
    stream >> tmpF; QVERIFY(qIsInf(tmpF)); QVERIFY(tmpF < 0); tmpD = 0;
    stream >> tmpF; QVERIFY(qIsInf(tmpF)); QVERIFY(tmpF < 0); tmpD = 0;
    stream >> tmpF; QVERIFY(qIsInf(tmpF)); QVERIFY(tmpF < 0);

    QString s;
    QTextStream out(&s);
    out << qInf() << " " << -qInf() << " " << qQNaN()
        << uppercasedigits << " "
        << qInf() << " " << -qInf() << " " << qQNaN()
        << flush;

    QCOMPARE(s, QString("inf -inf nan INF -INF NAN"));
}

// ------------------------------------------------------------------------------
void tst_QTextStream::utf8IncompleteAtBufferBoundary_data()
{
    QTest::addColumn<bool>("useLocale");

    QTest::newRow("utf8") << false;

    // is this locale UTF-8?
    if (QString(QChar::ReplacementCharacter).toLocal8Bit() == "\xef\xbf\xbd")
        QTest::newRow("locale") << true;
}

void tst_QTextStream::utf8IncompleteAtBufferBoundary()
{
    QFile::remove(TestFileName);
    QFile data(TestFileName);

    QTextCodec *utf8Codec = QTextCodec::codecForMib(106);
    QString lineContents = QString::fromUtf8("\342\200\223" // U+2013 EN DASH
                                             "\342\200\223"
                                             "\342\200\223"
                                             "\342\200\223"
                                             "\342\200\223"
                                             "\342\200\223");

    data.open(QFile::WriteOnly | QFile::Truncate);
    {
        QTextStream out(&data);
        out.setCodec(utf8Codec);
        out.setFieldWidth(3);

        for (int i = 0; i < 1000; ++i) {
            out << i << lineContents << endl;
        }
    }
    data.close();

    data.open(QFile::ReadOnly);
    QTextStream in(&data);

    QFETCH(bool, useLocale);
    if (!useLocale)
        in.setCodec(utf8Codec); // QUtf8Codec
    else
        in.setCodec(QTextCodec::codecForLocale());

    int i = 0;
    do {
        QString line = in.readLine().trimmed();
        ++i;
        QVERIFY2(line.endsWith(lineContents), QString("Line %1: %2").arg(i).arg(line).toLocal8Bit());
    } while (!in.atEnd());
}

// ------------------------------------------------------------------------------

// Make sure we don't write a BOM after seek()ing

void tst_QTextStream::writeSeekWriteNoBOM()
{

    //First with the default codec (normally either latin-1 or UTF-8)

    QBuffer out;
    out.open(QIODevice::WriteOnly);
    QTextStream stream(&out);

    int number = 0;
    QString sizeStr = QLatin1String("Size=")
        + QString::number(number).rightJustified(10, QLatin1Char('0'));
    stream << sizeStr << endl;
    stream << "Version=" << QString::number(14) << endl;
    stream << "blah blah blah" << endl;
    stream.flush();

    QCOMPARE(out.buffer().constData(), "Size=0000000000\nVersion=14\nblah blah blah\n");

    // Now overwrite the size header item
    number = 42;
    stream.seek(0);
    sizeStr = QLatin1String("Size=")
        + QString::number(number).rightJustified(10, QLatin1Char('0'));
    stream << sizeStr << endl;
    stream.flush();

    // Check buffer is still OK
    QCOMPARE(out.buffer().constData(), "Size=0000000042\nVersion=14\nblah blah blah\n");


    //Then UTF-16

    QBuffer out16;
    out16.open(QIODevice::WriteOnly);
    QTextStream stream16(&out16);
    stream16.setCodec("UTF-16");

    stream16 << "one" << "two" << QLatin1String("three");
    stream16.flush();

    // save that output
    QByteArray first = out16.buffer();

    stream16.seek(0);
    stream16 << "one";
    stream16.flush();

    QCOMPARE(out16.buffer(), first);
}



// ------------------------------------------------------------------------------
void tst_QTextStream::generateOperatorCharData(bool for_QString)
{
    QTest::addColumn<QByteArray>("input");
    QTest::addColumn<QChar>("qchar_output");
    QTest::addColumn<char>("char_output");
    QTest::addColumn<QByteArray>("write_output");

    QTest::newRow("empty") << QByteArray() << QChar('\0') << '\0' << QByteArray("\0", 1);
    QTest::newRow("a") << QByteArray("a") << QChar('a') << 'a' << QByteArray("a");
    QTest::newRow("\\na") << QByteArray("\na") << QChar('\n') << '\n' << QByteArray("\n");
    QTest::newRow("\\0") << QByteArray("\0") << QChar('\0') << '\0' << QByteArray("\0", 1);
    QTest::newRow("\\xff") << QByteArray("\xff") << QChar('\xff') << '\xff' << QByteArray("\xff");
    QTest::newRow("\\xfe") << QByteArray("\xfe") << QChar('\xfe') << '\xfe' << QByteArray("\xfe");

    if (!for_QString) {
        QTest::newRow("utf16-BE (empty)") << QByteArray("\xff\xfe", 2) << QChar('\0') << '\0' << QByteArray("\0", 1);
        QTest::newRow("utf16-BE (a)") << QByteArray("\xff\xfe\x61\x00", 4) << QChar('a') << 'a' << QByteArray("a");
        QTest::newRow("utf16-LE (empty)") << QByteArray("\xfe\xff", 2) << QChar('\0') << '\0' << QByteArray("\0", 1);
        QTest::newRow("utf16-LE (a)") << QByteArray("\xfe\xff\x00\x61", 4) << QChar('a') << 'a' << QByteArray("a");
    }
}

// ------------------------------------------------------------------------------
void tst_QTextStream::QChar_operators_FromDevice_data()
{
    generateOperatorCharData(false);
}

// ------------------------------------------------------------------------------
void tst_QTextStream::QChar_operators_FromDevice()
{
    QFETCH(QByteArray, input);
    QFETCH(QChar, qchar_output);
    QFETCH(QByteArray, write_output);

    QBuffer buf(&input);
    buf.open(QBuffer::ReadOnly);
    QTextStream stream(&buf);
    stream.setCodec(QTextCodec::codecForName("ISO-8859-1"));
    QChar tmp;
    stream >> tmp;
    QCOMPARE(tmp, qchar_output);

    QBuffer writeBuf;
    writeBuf.open(QBuffer::WriteOnly);

    QTextStream writeStream(&writeBuf);
    writeStream.setCodec(QTextCodec::codecForName("ISO-8859-1"));
    writeStream << qchar_output;
    writeStream.flush();

    QCOMPARE(writeBuf.buffer().size(), write_output.size());
    QCOMPARE(writeBuf.buffer().constData(), write_output.constData());
}

// ------------------------------------------------------------------------------
void tst_QTextStream::char_operators_FromDevice_data()
{
    generateOperatorCharData(false);
}

// ------------------------------------------------------------------------------
void tst_QTextStream::char_operators_FromDevice()
{
    QFETCH(QByteArray, input);
    QFETCH(char, char_output);
    QFETCH(QByteArray, write_output);

    QBuffer buf(&input);
    buf.open(QBuffer::ReadOnly);
    QTextStream stream(&buf);
    stream.setCodec(QTextCodec::codecForName("ISO-8859-1"));
    char tmp;
    stream >> tmp;
    QCOMPARE(tmp, char_output);

    QBuffer writeBuf;
    writeBuf.open(QBuffer::WriteOnly);

    QTextStream writeStream(&writeBuf);
    writeStream.setCodec(QTextCodec::codecForName("ISO-8859-1"));
    writeStream << char_output;
    writeStream.flush();

    QCOMPARE(writeBuf.buffer().size(), write_output.size());
    QCOMPARE(writeBuf.buffer().constData(), write_output.constData());
}

// ------------------------------------------------------------------------------
void tst_QTextStream::generateNaturalNumbersData(bool for_QString)
{
    QTest::addColumn<QByteArray>("input");
    QTest::addColumn<qulonglong>("output");

    QTest::newRow("empty") << QByteArray() << qulonglong(0);
    QTest::newRow("a") << QByteArray("a") << qulonglong(0);
    QTest::newRow(" ") << QByteArray(" ") << qulonglong(0);
    QTest::newRow("0") << QByteArray("0") << qulonglong(0);
    QTest::newRow("1") << QByteArray("1") << qulonglong(1);
    QTest::newRow("12") << QByteArray("12") << qulonglong(12);
    QTest::newRow("-12") << QByteArray("-12") << qulonglong(-12);
    QTest::newRow("-0") << QByteArray("-0") << qulonglong(0);
    QTest::newRow(" 1") << QByteArray(" 1") << qulonglong(1);
    QTest::newRow(" \\r\\n\\r\\n123") << QByteArray(" \r\n\r\n123") << qulonglong(123);

    // bit boundary tests
    QTest::newRow("127") << QByteArray("127") << qulonglong(127);
    QTest::newRow("128") << QByteArray("128") << qulonglong(128);
    QTest::newRow("129") << QByteArray("129") << qulonglong(129);
    QTest::newRow("-127") << QByteArray("-127") << qulonglong(-127);
    QTest::newRow("-128") << QByteArray("-128") << qulonglong(-128);
    QTest::newRow("-129") << QByteArray("-129") << qulonglong(-129);
    QTest::newRow("32767") << QByteArray("32767") << qulonglong(32767);
    QTest::newRow("32768") << QByteArray("32768") << qulonglong(32768);
    QTest::newRow("32769") << QByteArray("32769") << qulonglong(32769);
    QTest::newRow("-32767") << QByteArray("-32767") << qulonglong(-32767);
    QTest::newRow("-32768") << QByteArray("-32768") << qulonglong(-32768);
    QTest::newRow("-32769") << QByteArray("-32769") << qulonglong(-32769);
    QTest::newRow("65537") << QByteArray("65537") << qulonglong(65537);
    QTest::newRow("65536") << QByteArray("65536") << qulonglong(65536);
    QTest::newRow("65535") << QByteArray("65535") << qulonglong(65535);
    QTest::newRow("-65537") << QByteArray("-65537") << qulonglong(-65537);
    QTest::newRow("-65536") << QByteArray("-65536") << qulonglong(-65536);
    QTest::newRow("-65535") << QByteArray("-65535") << qulonglong(-65535);
    QTest::newRow("2147483646") << QByteArray("2147483646") << qulonglong(2147483646);
    QTest::newRow("2147483647") << QByteArray("2147483647") << qulonglong(2147483647);
    QTest::newRow("2147483648") << QByteArray("2147483648") << Q_UINT64_C(2147483648);
    QTest::newRow("-2147483646") << QByteArray("-2147483646") << qulonglong(-2147483646);
    QTest::newRow("-2147483647") << QByteArray("-2147483647") << qulonglong(-2147483647);
    QTest::newRow("-2147483648") << QByteArray("-2147483648") << Q_UINT64_C(-2147483648);
    QTest::newRow("4294967296") << QByteArray("4294967296") << Q_UINT64_C(4294967296);
    QTest::newRow("4294967297") << QByteArray("4294967297") << Q_UINT64_C(4294967297);
    QTest::newRow("4294967298") << QByteArray("4294967298") << Q_UINT64_C(4294967298);
    QTest::newRow("-4294967296") << QByteArray("-4294967296") << Q_UINT64_C(-4294967296);
    QTest::newRow("-4294967297") << QByteArray("-4294967297") << Q_UINT64_C(-4294967297);
    QTest::newRow("-4294967298") << QByteArray("-4294967298") << Q_UINT64_C(-4294967298);
    QTest::newRow("9223372036854775807") << QByteArray("9223372036854775807") << Q_UINT64_C(9223372036854775807);
    QTest::newRow("9223372036854775808") << QByteArray("9223372036854775808") << Q_UINT64_C(9223372036854775808);
    QTest::newRow("9223372036854775809") << QByteArray("9223372036854775809") << Q_UINT64_C(9223372036854775809);
    QTest::newRow("18446744073709551615") << QByteArray("18446744073709551615") << Q_UINT64_C(18446744073709551615);
    QTest::newRow("18446744073709551616") << QByteArray("18446744073709551616") << Q_UINT64_C(0);
    QTest::newRow("18446744073709551617") << QByteArray("18446744073709551617") << Q_UINT64_C(1);
    // 18446744073709551617 bytes should be enough for anyone.... ;-)

    // hex tests
    QTest::newRow("0x0") << QByteArray("0x0") << qulonglong(0);
    QTest::newRow("0x") << QByteArray("0x") << qulonglong(0);
    QTest::newRow("0x1") << QByteArray("0x1") << qulonglong(1);
    QTest::newRow("0xf") << QByteArray("0xf") << qulonglong(15);
    QTest::newRow("0xdeadbeef") << QByteArray("0xdeadbeef") << Q_UINT64_C(3735928559);
    QTest::newRow("0XDEADBEEF") << QByteArray("0XDEADBEEF") << Q_UINT64_C(3735928559);
    QTest::newRow("0xdeadbeefZzzzz") << QByteArray("0xdeadbeefZzzzz") << Q_UINT64_C(3735928559);
    QTest::newRow("  0xdeadbeefZzzzz") << QByteArray("  0xdeadbeefZzzzz") << Q_UINT64_C(3735928559);

    // oct tests
    QTest::newRow("00") << QByteArray("00") << qulonglong(0);
    QTest::newRow("0141") << QByteArray("0141") << qulonglong(97);
    QTest::newRow("01419999") << QByteArray("01419999") << qulonglong(97);
    QTest::newRow("  01419999") << QByteArray("  01419999") << qulonglong(97);

    // bin tests
    QTest::newRow("0b0") << QByteArray("0b0") << qulonglong(0);
    QTest::newRow("0b1") << QByteArray("0b1") << qulonglong(1);
    QTest::newRow("0b10") << QByteArray("0b10") << qulonglong(2);
    QTest::newRow("0B10") << QByteArray("0B10") << qulonglong(2);
    QTest::newRow("0b101010") << QByteArray("0b101010") << qulonglong(42);
    QTest::newRow("0b1010102345") << QByteArray("0b1010102345") << qulonglong(42);
    QTest::newRow("  0b1010102345") << QByteArray("  0b1010102345") << qulonglong(42);

    // utf-16 tests
    if (!for_QString) {
        QTest::newRow("utf16-BE (empty)") << QByteArray("\xfe\xff", 2) << qulonglong(0);
        QTest::newRow("utf16-BE (0xdeadbeef)")
            << QByteArray("\xfe\xff"
                          "\x00\x30\x00\x78\x00\x64\x00\x65\x00\x61\x00\x64\x00\x62\x00\x65\x00\x65\x00\x66", 22)
            << Q_UINT64_C(3735928559);
        QTest::newRow("utf16-LE (empty)") << QByteArray("\xff\xfe", 2) << Q_UINT64_C(0);
        QTest::newRow("utf16-LE (0xdeadbeef)")
            << QByteArray("\xff\xfe"
                          "\x30\x00\x78\x00\x64\x00\x65\x00\x61\x00\x64\x00\x62\x00\x65\x00\x65\x00\x66\x00", 22)
            << Q_UINT64_C(3735928559);
    }
}

// ------------------------------------------------------------------------------
#define IMPLEMENT_STREAM_RIGHT_INT_OPERATOR_TEST(texttype, type) \
    void tst_QTextStream:: texttype##_read_operator_FromDevice_data() \
    { generateNaturalNumbersData(false); } \
    void tst_QTextStream:: texttype##_read_operator_FromDevice() \
    { \
        QFETCH(QByteArray, input); \
        QFETCH(qulonglong, output); \
        type sh; \
        QTextStream stream(&input); \
        stream >> sh; \
        QCOMPARE(sh, (type)output); \
    }
IMPLEMENT_STREAM_RIGHT_INT_OPERATOR_TEST(signedShort, signed short)
IMPLEMENT_STREAM_RIGHT_INT_OPERATOR_TEST(unsignedShort, unsigned short)
IMPLEMENT_STREAM_RIGHT_INT_OPERATOR_TEST(signedInt, signed int)
IMPLEMENT_STREAM_RIGHT_INT_OPERATOR_TEST(unsignedInt, unsigned int)
IMPLEMENT_STREAM_RIGHT_INT_OPERATOR_TEST(qlonglong, qlonglong)
IMPLEMENT_STREAM_RIGHT_INT_OPERATOR_TEST(qulonglong, qulonglong)
    ;

// ------------------------------------------------------------------------------
void tst_QTextStream::generateRealNumbersData(bool for_QString)
{
    QTest::addColumn<QByteArray>("input");
    QTest::addColumn<double>("output");

    QTest::newRow("empty") << QByteArray() << 0.0;
    QTest::newRow("a") << QByteArray("a") << 0.0;
    QTest::newRow("1.0") << QByteArray("1.0") << 1.0;
    QTest::newRow(" 1") << QByteArray(" 1") << 1.0;
    QTest::newRow(" \\r\\n1.2") << QByteArray(" \r\n1.2") << 1.2;
    QTest::newRow("3.14") << QByteArray("3.14") << 3.14;
    QTest::newRow("-3.14") << QByteArray("-3.14") << -3.14;
    QTest::newRow(" -3.14") << QByteArray(" -3.14") << -3.14;
    QTest::newRow("314e-02") << QByteArray("314e-02") << 3.14;
    QTest::newRow("314E-02") << QByteArray("314E-02") << 3.14;
    QTest::newRow("314e+02") << QByteArray("314e+02") << 31400.;
    QTest::newRow("314E+02") << QByteArray("314E+02") << 31400.;

    // ### add numbers with exponents

    if (!for_QString) {
        QTest::newRow("utf16-BE (empty)") << QByteArray("\xff\xfe", 2) << 0.0;
        QTest::newRow("utf16-LE (empty)") << QByteArray("\xfe\xff", 2) << 0.0;
    }
}

// ------------------------------------------------------------------------------
#define IMPLEMENT_STREAM_RIGHT_REAL_OPERATOR_TEST(texttype, type) \
    void tst_QTextStream:: texttype##_read_operator_FromDevice_data() \
    { generateRealNumbersData(false); } \
    void tst_QTextStream:: texttype##_read_operator_FromDevice() \
    { \
        QFETCH(QByteArray, input); \
        QFETCH(double, output); \
        type sh; \
        QTextStream stream(&input); \
        stream >> sh; \
        QCOMPARE(sh, (type)output); \
    }
IMPLEMENT_STREAM_RIGHT_REAL_OPERATOR_TEST(float, float)
IMPLEMENT_STREAM_RIGHT_REAL_OPERATOR_TEST(double, double)
    ;

// ------------------------------------------------------------------------------
void tst_QTextStream::generateStringData(bool for_QString)
{
    QTest::addColumn<QByteArray>("input");
    QTest::addColumn<QByteArray>("array_output");
    QTest::addColumn<QString>("string_output");

    QTest::newRow("empty") << QByteArray() << QByteArray() << QString();
    QTest::newRow("a") << QByteArray("a") << QByteArray("a") << QString("a");
    QTest::newRow("a b") << QByteArray("a b") << QByteArray("a") << QString("a");
    QTest::newRow(" a b") << QByteArray(" a b") << QByteArray("a") << QString("a");
    QTest::newRow("a1") << QByteArray("a1") << QByteArray("a1") << QString("a1");
    QTest::newRow("a1 b1") << QByteArray("a1 b1") << QByteArray("a1") << QString("a1");
    QTest::newRow(" a1 b1") << QByteArray(" a1 b1") << QByteArray("a1") << QString("a1");
    QTest::newRow("\\n\\n\\nole i dole\\n") << QByteArray("\n\n\nole i dole\n") << QByteArray("ole") << QString("ole");

    if (!for_QString) {
        QTest::newRow("utf16-BE (empty)") << QByteArray("\xff\xfe", 2) << QByteArray() << QString();
        QTest::newRow("utf16-BE (corrupt)") << QByteArray("\xff", 1) << QByteArray("\xff") << QString("\xff");
        QTest::newRow("utf16-LE (empty)") << QByteArray("\xfe\xff", 2) << QByteArray() << QString();
        QTest::newRow("utf16-LE (corrupt)") << QByteArray("\xfe", 1) << QByteArray("\xfe") << QString("\xfe");
    }
}

// ------------------------------------------------------------------------------
void tst_QTextStream::charPtr_read_operator_FromDevice_data()
{
    generateStringData(false);
}

// ------------------------------------------------------------------------------
void tst_QTextStream::charPtr_read_operator_FromDevice()
{
    QFETCH(QByteArray, input);
    QFETCH(QByteArray, array_output);

    QBuffer buffer(&input);
    buffer.open(QBuffer::ReadOnly);
    QTextStream stream(&buffer);
    stream.setCodec(QTextCodec::codecForName("ISO-8859-1"));
    stream.setAutoDetectUnicode(true);

    char buf[1024];
    stream >> buf;

    QCOMPARE((const char *)buf, array_output.constData());
}

// ------------------------------------------------------------------------------
void tst_QTextStream::stringRef_read_operator_FromDevice_data()
{
    generateStringData(false);
}

// ------------------------------------------------------------------------------
void tst_QTextStream::stringRef_read_operator_FromDevice()
{
    QFETCH(QByteArray, input);
    QFETCH(QString, string_output);

    QBuffer buffer(&input);
    buffer.open(QBuffer::ReadOnly);
    QTextStream stream(&buffer);
    stream.setCodec(QTextCodec::codecForName("ISO-8859-1"));
    stream.setAutoDetectUnicode(true);

    QString tmp;
    stream >> tmp;

    QCOMPARE(tmp, string_output);
}

// ------------------------------------------------------------------------------
void tst_QTextStream::byteArray_read_operator_FromDevice_data()
{
    generateStringData(false);
}

// ------------------------------------------------------------------------------
void tst_QTextStream::byteArray_read_operator_FromDevice()
{
    QFETCH(QByteArray, input);
    QFETCH(QByteArray, array_output);

    QBuffer buffer(&input);
    buffer.open(QBuffer::ReadOnly);
    QTextStream stream(&buffer);
    stream.setCodec(QTextCodec::codecForName("ISO-8859-1"));
    stream.setAutoDetectUnicode(true);

    QByteArray array;
    stream >> array;

    QCOMPARE(array, array_output);
}

// ------------------------------------------------------------------------------
#define IMPLEMENT_STREAM_LEFT_INT_OPERATOR_TEST(texttype, type) \
    void tst_QTextStream:: texttype##_write_operator_ToDevice() \
    { \
        QFETCH(qulonglong, number); \
        QFETCH(QByteArray, data); \
        \
        QBuffer buffer; \
        buffer.open(QBuffer::WriteOnly); \
        QTextStream stream(&buffer); \
        stream << (type)number; \
        stream.flush(); \
        \
        QCOMPARE(buffer.data().constData(), data.constData()); \
    }

// ------------------------------------------------------------------------------
void tst_QTextStream::signedShort_write_operator_ToDevice_data()
{
    QTest::addColumn<qulonglong>("number");
    QTest::addColumn<QByteArray>("data");

    QTest::newRow("0") << Q_UINT64_C(0) << QByteArray("0");
    QTest::newRow("1") << Q_UINT64_C(1) << QByteArray("1");
    QTest::newRow("-1") << Q_UINT64_C(-1) << QByteArray("-1");
    QTest::newRow("32767") << Q_UINT64_C(32767) << QByteArray("32767");
    QTest::newRow("32768") << Q_UINT64_C(32768) << QByteArray("-32768");
    QTest::newRow("32769") << Q_UINT64_C(32769) << QByteArray("-32767");
    QTest::newRow("65535") << Q_UINT64_C(65535) << QByteArray("-1");
    QTest::newRow("65536") << Q_UINT64_C(65536) << QByteArray("0");
    QTest::newRow("65537") << Q_UINT64_C(65537) << QByteArray("1");
}
IMPLEMENT_STREAM_LEFT_INT_OPERATOR_TEST(signedShort, signed short)
    ;

// ------------------------------------------------------------------------------
void tst_QTextStream::unsignedShort_write_operator_ToDevice_data()
{
    QTest::addColumn<qulonglong>("number");
    QTest::addColumn<QByteArray>("data");

    QTest::newRow("0") << Q_UINT64_C(0) << QByteArray("0");
    QTest::newRow("1") << Q_UINT64_C(1) << QByteArray("1");
    QTest::newRow("-1") << Q_UINT64_C(-1) << QByteArray("65535");
    QTest::newRow("32767") << Q_UINT64_C(32767) << QByteArray("32767");
    QTest::newRow("32768") << Q_UINT64_C(32768) << QByteArray("32768");
    QTest::newRow("32769") << Q_UINT64_C(32769) << QByteArray("32769");
    QTest::newRow("65535") << Q_UINT64_C(65535) << QByteArray("65535");
    QTest::newRow("65536") << Q_UINT64_C(65536) << QByteArray("0");
    QTest::newRow("65537") << Q_UINT64_C(65537) << QByteArray("1");
}
IMPLEMENT_STREAM_LEFT_INT_OPERATOR_TEST(unsignedShort, unsigned short)
    ;

// ------------------------------------------------------------------------------
void tst_QTextStream::signedInt_write_operator_ToDevice_data()
{
    QTest::addColumn<qulonglong>("number");
    QTest::addColumn<QByteArray>("data");

    QTest::newRow("0") << Q_UINT64_C(0) << QByteArray("0");
    QTest::newRow("1") << Q_UINT64_C(1) << QByteArray("1");
    QTest::newRow("-1") << Q_UINT64_C(-1) << QByteArray("-1");
    QTest::newRow("32767") << Q_UINT64_C(32767) << QByteArray("32767");
    QTest::newRow("32768") << Q_UINT64_C(32768) << QByteArray("32768");
    QTest::newRow("32769") << Q_UINT64_C(32769) << QByteArray("32769");
    QTest::newRow("65535") << Q_UINT64_C(65535) << QByteArray("65535");
    QTest::newRow("65536") << Q_UINT64_C(65536) << QByteArray("65536");
    QTest::newRow("65537") << Q_UINT64_C(65537) << QByteArray("65537");
    QTest::newRow("2147483647") << Q_UINT64_C(2147483647) << QByteArray("2147483647");
    QTest::newRow("2147483648") << Q_UINT64_C(2147483648) << QByteArray("-2147483648");
    QTest::newRow("2147483649") << Q_UINT64_C(2147483649) << QByteArray("-2147483647");
    QTest::newRow("4294967295") << Q_UINT64_C(4294967295) << QByteArray("-1");
    QTest::newRow("4294967296") << Q_UINT64_C(4294967296) << QByteArray("0");
    QTest::newRow("4294967297") << Q_UINT64_C(4294967297) << QByteArray("1");
}
IMPLEMENT_STREAM_LEFT_INT_OPERATOR_TEST(signedInt, signed int)
    ;

// ------------------------------------------------------------------------------
void tst_QTextStream::unsignedInt_write_operator_ToDevice_data()
{
    QTest::addColumn<qulonglong>("number");
    QTest::addColumn<QByteArray>("data");

    QTest::newRow("0") << Q_UINT64_C(0) << QByteArray("0");
    QTest::newRow("1") << Q_UINT64_C(1) << QByteArray("1");
    QTest::newRow("-1") << Q_UINT64_C(-1) << QByteArray("4294967295");
    QTest::newRow("32767") << Q_UINT64_C(32767) << QByteArray("32767");
    QTest::newRow("32768") << Q_UINT64_C(32768) << QByteArray("32768");
    QTest::newRow("32769") << Q_UINT64_C(32769) << QByteArray("32769");
    QTest::newRow("65535") << Q_UINT64_C(65535) << QByteArray("65535");
    QTest::newRow("65536") << Q_UINT64_C(65536) << QByteArray("65536");
    QTest::newRow("65537") << Q_UINT64_C(65537) << QByteArray("65537");
    QTest::newRow("2147483647") << Q_UINT64_C(2147483647) << QByteArray("2147483647");
    QTest::newRow("2147483648") << Q_UINT64_C(2147483648) << QByteArray("2147483648");
    QTest::newRow("2147483649") << Q_UINT64_C(2147483649) << QByteArray("2147483649");
    QTest::newRow("4294967295") << Q_UINT64_C(4294967295) << QByteArray("4294967295");
    QTest::newRow("4294967296") << Q_UINT64_C(4294967296) << QByteArray("0");
    QTest::newRow("4294967297") << Q_UINT64_C(4294967297) << QByteArray("1");
}
IMPLEMENT_STREAM_LEFT_INT_OPERATOR_TEST(unsignedInt, unsigned int)
    ;

// ------------------------------------------------------------------------------
void tst_QTextStream::qlonglong_write_operator_ToDevice_data()
{
    QTest::addColumn<qulonglong>("number");
    QTest::addColumn<QByteArray>("data");

    QTest::newRow("0") << Q_UINT64_C(0) << QByteArray("0");
    QTest::newRow("1") << Q_UINT64_C(1) << QByteArray("1");
    QTest::newRow("-1") << Q_UINT64_C(-1) << QByteArray("-1");
    QTest::newRow("32767") << Q_UINT64_C(32767) << QByteArray("32767");
    QTest::newRow("32768") << Q_UINT64_C(32768) << QByteArray("32768");
    QTest::newRow("32769") << Q_UINT64_C(32769) << QByteArray("32769");
    QTest::newRow("65535") << Q_UINT64_C(65535) << QByteArray("65535");
    QTest::newRow("65536") << Q_UINT64_C(65536) << QByteArray("65536");
    QTest::newRow("65537") << Q_UINT64_C(65537) << QByteArray("65537");
    QTest::newRow("2147483647") << Q_UINT64_C(2147483647) << QByteArray("2147483647");
    QTest::newRow("2147483648") << Q_UINT64_C(2147483648) << QByteArray("2147483648");
    QTest::newRow("2147483649") << Q_UINT64_C(2147483649) << QByteArray("2147483649");
    QTest::newRow("4294967295") << Q_UINT64_C(4294967295) << QByteArray("4294967295");
    QTest::newRow("4294967296") << Q_UINT64_C(4294967296) << QByteArray("4294967296");
    QTest::newRow("4294967297") << Q_UINT64_C(4294967297) << QByteArray("4294967297");
    QTest::newRow("9223372036854775807") << Q_UINT64_C(9223372036854775807) << QByteArray("9223372036854775807");
    QTest::newRow("9223372036854775808") << Q_UINT64_C(9223372036854775808) << QByteArray("-9223372036854775808");
    QTest::newRow("9223372036854775809") << Q_UINT64_C(9223372036854775809) << QByteArray("-9223372036854775807");
    QTest::newRow("18446744073709551615") << Q_UINT64_C(18446744073709551615) << QByteArray("-1");
}
IMPLEMENT_STREAM_LEFT_INT_OPERATOR_TEST(qlonglong, qlonglong)
    ;

// ------------------------------------------------------------------------------
void tst_QTextStream::qulonglong_write_operator_ToDevice_data()
{
    QTest::addColumn<qulonglong>("number");
    QTest::addColumn<QByteArray>("data");

    QTest::newRow("0") << Q_UINT64_C(0) << QByteArray("0");
    QTest::newRow("1") << Q_UINT64_C(1) << QByteArray("1");
    QTest::newRow("-1") << Q_UINT64_C(-1) << QByteArray("18446744073709551615");
    QTest::newRow("32767") << Q_UINT64_C(32767) << QByteArray("32767");
    QTest::newRow("32768") << Q_UINT64_C(32768) << QByteArray("32768");
    QTest::newRow("32769") << Q_UINT64_C(32769) << QByteArray("32769");
    QTest::newRow("65535") << Q_UINT64_C(65535) << QByteArray("65535");
    QTest::newRow("65536") << Q_UINT64_C(65536) << QByteArray("65536");
    QTest::newRow("65537") << Q_UINT64_C(65537) << QByteArray("65537");
    QTest::newRow("2147483647") << Q_UINT64_C(2147483647) << QByteArray("2147483647");
    QTest::newRow("2147483648") << Q_UINT64_C(2147483648) << QByteArray("2147483648");
    QTest::newRow("2147483649") << Q_UINT64_C(2147483649) << QByteArray("2147483649");
    QTest::newRow("4294967295") << Q_UINT64_C(4294967295) << QByteArray("4294967295");
    QTest::newRow("4294967296") << Q_UINT64_C(4294967296) << QByteArray("4294967296");
    QTest::newRow("4294967297") << Q_UINT64_C(4294967297) << QByteArray("4294967297");
    QTest::newRow("9223372036854775807") << Q_UINT64_C(9223372036854775807) << QByteArray("9223372036854775807");
    QTest::newRow("9223372036854775808") << Q_UINT64_C(9223372036854775808) << QByteArray("9223372036854775808");
    QTest::newRow("9223372036854775809") << Q_UINT64_C(9223372036854775809) << QByteArray("9223372036854775809");
    QTest::newRow("18446744073709551615") << Q_UINT64_C(18446744073709551615) << QByteArray("18446744073709551615");
}
IMPLEMENT_STREAM_LEFT_INT_OPERATOR_TEST(qulonglong, qulonglong)
    ;


// ------------------------------------------------------------------------------
void tst_QTextStream::generateRealNumbersDataWrite()
{
    QTest::addColumn<double>("number");
    QTest::addColumn<QByteArray>("data");

    QTest::newRow("0") << 0.0 << QByteArray("0");
    QTest::newRow("3.14") << 3.14 << QByteArray("3.14");
    QTest::newRow("-3.14") << -3.14 << QByteArray("-3.14");
    QTest::newRow("1.2e+10") << 1.2e+10 << QByteArray("1.2e+10");
    QTest::newRow("-1.2e+10") << -1.2e+10 << QByteArray("-1.2e+10");
}

// ------------------------------------------------------------------------------
#define IMPLEMENT_STREAM_LEFT_REAL_OPERATOR_TEST(texttype, type) \
    void tst_QTextStream:: texttype##_write_operator_ToDevice_data() \
    { generateRealNumbersDataWrite(); } \
    void tst_QTextStream:: texttype##_write_operator_ToDevice() \
    { \
        QFETCH(double, number); \
        QFETCH(QByteArray, data); \
        \
        QBuffer buffer; \
        buffer.open(QBuffer::WriteOnly); \
        QTextStream stream(&buffer); \
        float f = (float)number; \
        stream << f; \
        stream.flush(); \
        QCOMPARE(buffer.data().constData(), data.constData()); \
    }
IMPLEMENT_STREAM_LEFT_REAL_OPERATOR_TEST(float, float)
IMPLEMENT_STREAM_LEFT_REAL_OPERATOR_TEST(double, float)
    ;

// ------------------------------------------------------------------------------
void tst_QTextStream::string_write_operator_ToDevice_data()
{
    QTest::addColumn<QByteArray>("bytedata");
    QTest::addColumn<QString>("stringdata");
    QTest::addColumn<QByteArray>("result");

    QTest::newRow("empty") << QByteArray("", 1) << QString(1, '\0') << QByteArray("", 1);
    QTest::newRow("a") << QByteArray("a") << QString("a") << QByteArray("a");
    QTest::newRow("a cow jumped over the moon")
        << QByteArray("a cow jumped over the moon")
        << QString("a cow jumped over the moon")
        << QByteArray("a cow jumped over the moon");

    // ### get the utf16-be test on its legs.
    /*
    QTest::newRow("utf16-BE (a cow jumped over the moon)")
        << QByteArray("\xff\xfe\x00\x61\x00\x20\x00\x63\x00\x6f\x00\x77\x00\x20\x00\x6a\x00\x75\x00\x6d\x00\x70\x00\x65\x00\x64\x00\x20\x00\x6f\x00\x76\x00\x65\x00\x72\x00\x20\x00\x74\x00\x68\x00\x65\x00\x20\x00\x6d\x00\x6f\x00\x6f\x00\x6e\x00\x0a", 56)
        << QString("a cow jumped over the moon")
        << QByteArray("a cow jumped over the moon");
    */
}

// ------------------------------------------------------------------------------
void tst_QTextStream::string_write_operator_ToDevice()
{
    QFETCH(QByteArray, bytedata);
    QFETCH(QString, stringdata);
    QFETCH(QByteArray, result);

    {
        // char*
        QBuffer buf;
        buf.open(QBuffer::WriteOnly);
        QTextStream stream(&buf);
        stream.setCodec(QTextCodec::codecForName("ISO-8859-1"));
        stream.setAutoDetectUnicode(true);

        stream << bytedata.constData();
        stream.flush();
        QCOMPARE(buf.buffer().constData(), result.constData());
    }
    {
        // QByteArray
        QBuffer buf;
        buf.open(QBuffer::WriteOnly);
        QTextStream stream(&buf);
        stream.setCodec(QTextCodec::codecForName("ISO-8859-1"));
        stream.setAutoDetectUnicode(true);

        stream << bytedata;
        stream.flush();
        QCOMPARE(buf.buffer().constData(), result.constData());
    }
    {
        // QString
        QBuffer buf;
        buf.open(QBuffer::WriteOnly);
        QTextStream stream(&buf);
        stream.setCodec(QTextCodec::codecForName("ISO-8859-1"));
        stream.setAutoDetectUnicode(true);

        stream << stringdata;
        stream.flush();
        QCOMPARE(buf.buffer().constData(), result.constData());
    }
}

// ------------------------------------------------------------------------------
void tst_QTextStream::useCase1()
{
    QFile::remove("testfile");
    QFile file("testfile");
    QVERIFY(file.open(QFile::ReadWrite));

    {
        QTextStream stream(&file);
        stream.setCodec(QTextCodec::codecForName("ISO-8859-1"));
        stream.setAutoDetectUnicode(true);

        stream << 4.15 << " " << QByteArray("abc") << " " << QString("ole");
    }

    file.seek(0);
    QCOMPARE(file.readAll(), QByteArray("4.15 abc ole"));
    file.seek(0);

    {
        double d;
        QByteArray a;
        QString s;
        QTextStream stream(&file);
        stream.setCodec(QTextCodec::codecForName("ISO-8859-1"));
        stream.setAutoDetectUnicode(true);

        stream >> d;
        stream >> a;
        stream >> s;

        QCOMPARE(d, 4.15);
        QCOMPARE(a, QByteArray("abc"));
        QCOMPARE(s, QString("ole"));
    }
}

// ------------------------------------------------------------------------------
void tst_QTextStream::useCase2()
{
    QFile::remove("testfile");
    QFile file("testfile");
    QVERIFY(file.open(QFile::ReadWrite));

    QTextStream stream(&file);
    stream.setCodec(QTextCodec::codecForName("ISO-8859-1"));
    stream.setAutoDetectUnicode(true);

    stream << 4.15 << " " << QByteArray("abc") << " " << QString("ole");

    file.close();
    QVERIFY(file.open(QFile::ReadWrite));

    QCOMPARE(file.readAll(), QByteArray("4.15 abc ole"));

    file.close();
    QVERIFY(file.open(QFile::ReadWrite));

    double d;
    QByteArray a;
    QString s;
    QTextStream stream2(&file);
    stream2.setCodec(QTextCodec::codecForName("ISO-8859-1"));
    stream2.setAutoDetectUnicode(true);

    stream2 >> d;
    stream2 >> a;
    stream2 >> s;

    QCOMPARE(d, 4.15);
    QCOMPARE(a, QByteArray("abc"));
    QCOMPARE(s, QString("ole"));
}

// ------------------------------------------------------------------------------
void tst_QTextStream::manipulators_data()
{
    QTest::addColumn<int>("flags");
    QTest::addColumn<int>("width");
    QTest::addColumn<double>("realNumber");
    QTest::addColumn<int>("intNumber");
    QTest::addColumn<QString>("textData");
    QTest::addColumn<QByteArray>("result");

    QTest::newRow("no flags") << 0 << 0  << 5.0 << 5 << QString("five") << QByteArray("55five");
    QTest::newRow("rightadjust") << 0 << 10 << 5.0 << 5 << QString("five") << QByteArray("         5         5      five");

    // ### FIX
//    QTest::newRow("leftadjust") << int(QTextStream::left) << 10 << 5.0 << 5 << QString("five") << QByteArray("5         5         five      ");
//    QTest::newRow("showpos") << int(QTextStream::showpos) << 10 << 5.0 << 5 << QString("five") << QByteArray("        +5        +5      five");
//    QTest::newRow("showpos2") << int(QTextStream::showpos) << 5 << 3.14 << -5 << QString("five") << QByteArray("+3.14   -5 five");
//    QTest::newRow("hex") << int(QTextStream::hex | QTextStream::showbase) << 5 << 3.14 << -5 << QString("five") << QByteArray(" 3.14 -0x5 five");
//    QTest::newRow("hex uppercase") << int(QTextStream::hex | QTextStream::uppercase | QTextStream::showbase) << 5 << 3.14 << -5 << QString("five") << QByteArray(" 3.14 -0X5 five");
}

// ------------------------------------------------------------------------------
void tst_QTextStream::manipulators()
{
//    QFETCH(int, flags);
    QFETCH(int, width);
    QFETCH(double, realNumber);
    QFETCH(int, intNumber);
    QFETCH(QString, textData);
    QFETCH(QByteArray, result);

    QBuffer buffer;
    buffer.open(QBuffer::WriteOnly);

    QTextStream stream(&buffer);
    stream.setCodec(QTextCodec::codecForName("ISO-8859-1"));
    stream.setAutoDetectUnicode(true);

//    stream.setFlags(flags);
    stream.setFieldWidth(width);
    stream << realNumber;
    stream << intNumber;
    stream << textData;
    stream.flush();

    QCOMPARE(buffer.data().constData(), result.constData());
}

void tst_QTextStream::generateBOM()
{
    QFile::remove("bom.txt");
    {
        QFile file("bom.txt");
        QVERIFY(file.open(QFile::ReadWrite | QFile::Truncate));

        QTextStream stream(&file);
        stream.setCodec(QTextCodec::codecForName("UTF-16LE"));
        stream << "Hello" << endl;

        file.close();
        QVERIFY(file.open(QFile::ReadOnly));
        QCOMPARE(file.readAll(), QByteArray("\x48\x00\x65\00\x6c\00\x6c\00\x6f\x00\x0a\x00", 12));
    }

    QFile::remove("bom.txt");
    {
        QFile file("bom.txt");
        QVERIFY(file.open(QFile::ReadWrite | QFile::Truncate));

        QTextStream stream(&file);
        stream.setCodec(QTextCodec::codecForName("UTF-16LE"));
        stream << bom << "Hello" << endl;

        file.close();
        QVERIFY(file.open(QFile::ReadOnly));
        QCOMPARE(file.readAll(), QByteArray("\xff\xfe\x48\x00\x65\00\x6c\00\x6c\00\x6f\x00\x0a\x00", 14));
    }
}

void tst_QTextStream::readBomSeekBackReadBomAgain()
{
    QFile::remove("utf8bom");
    QFile file("utf8bom");
    QVERIFY(file.open(QFile::ReadWrite));
    file.write("\xef\xbb\xbf""Andreas");
    file.seek(0);
    QCOMPARE(file.pos(), qint64(0));

    QTextStream stream(&file);
    stream.setCodec("UTF-8");
    QString Andreas;
    stream >> Andreas;
    QCOMPARE(Andreas, QString("Andreas"));
    stream.seek(0);
    stream >> Andreas;
    QCOMPARE(Andreas, QString("Andreas"));
}

// ------------------------------------------------------------------------------
// QT 3 tests
// ------------------------------------------------------------------------------
#ifdef QT3_SUPPORT

void tst_QTextStream::qt3_readLineFromString()
{
    QString data = "line 1e\nline 2e\nline 3e";
    QStringList list;
    QTextStream stream(&data, QIODevice::ReadOnly);
    stream.setCodec(QTextCodec::codecForName("ISO-8859-1"));
    stream.setAutoDetectUnicode(true);

    while (!stream.atEnd()) {
        QString line = stream.readLine();
        QCOMPARE(QChar(line[line.size()-1]), QChar('e'));
        list << line;
    }
    QVERIFY(list.count() == 3);
}

// ------------------------------------------------------------------------------
void tst_QTextStream::qt3_operator_shiftright_data()
{
    qt3_operatorShift_data( QIODevice::ReadOnly );
}

// ------------------------------------------------------------------------------
void tst_QTextStream::qt3_operator_shiftleft_data()
{
    qt3_operatorShift_data( QIODevice::WriteOnly );
}

static const double doubleDummy = 567.89;
static const int intDummy = 1234;
static const QString stringDummy = "ABCD";

struct tst_IODevice {
    char *n;
    QIODevice::OpenModeFlag modes;
    QString name() const { return QString::fromLatin1(n); }
} devices[] = {
    { "file", QIODevice::ReadWrite },
    { "bytearray", QIODevice::ReadWrite },
    { "buffer", QIODevice::ReadWrite },
    { "string", QIODevice::ReadWrite },
    { "resource", QIODevice::ReadOnly }, //See discussion (resources) below
    { 0, (QIODevice::OpenModeFlag)0 }
};

/* Resources:

   This will test QTextStream's ability to interact with resources,
   however the trouble is the resources must be created correctly and
   built into the executable (but this executable also creates the
   resource files). So there is a chicken and egg problem, to create
   resources (if the file formats change at all):

   1) p4 edit resources/...

   2) Change QIODevice::ReadOnly above in the devices[] for "resource"
      to QIODevice::WriteOnly

   3) run the test

   4) revert this file, qmake, make, and run again.

   5) Enjoy.
*/

static inline QString resourceDir()
{
    QString ret = "resources/";
    if(QSysInfo::ByteOrder == QSysInfo::BigEndian)
        ret += "big_endian/";
    else
        ret += "little_endian/";
    return ret;
}

static const char *const encodings[] = {
    "Locale",
    "Latin1",
    "Unicode",
    "UnicodeNetworkOrder",
    "UnicodeReverse",
    "RawUnicode",
    "UnicodeUTF8",
    0
};

// ------------------------------------------------------------------------------
void tst_QTextStream::qt3_operatorShift_data( QIODevice::OpenMode mode )
{
    QTest::addColumn<QString>("device");
    QTest::addColumn<QString>("encoding");
    QTest::addColumn<QString>("type");
    QTest::addColumn<double>("doubleVal");
    QTest::addColumn<int>("intVal");
    QTest::addColumn<QString>("stringVal");
    QTest::addColumn<QByteArray>("encoded");

    for ( int i=0; !devices[i].name().isNull(); i++ ) {
        tst_IODevice *device = devices+i;
        if(!(device->modes & mode))
            continue;

	/*
	  We first test each type at least once.
	*/
	QTest::newRow( device->name() + "0" ) << device->name() << QString("UnicodeUTF8") << QString("QChar")
		<< doubleDummy << (int) 'Z' << stringDummy
		<< QByteArray( QByteArray("Z") );
	QTest::newRow( device->name() + "1" ) << device->name() << QString("UnicodeUTF8") << QString("char")
		<< doubleDummy << (int) 'Z' << stringDummy
		<< QByteArray( QByteArray("Z") );
	QTest::newRow( device->name() + "2" ) << device->name() << QString("UnicodeUTF8") << QString("signed short")
		<< doubleDummy << 12345 << stringDummy
		<< QByteArray( QByteArray("12345") );
	QTest::newRow( device->name() + "3" ) << device->name() << QString("UnicodeUTF8") << QString("unsigned short")
		<< doubleDummy << 12345 << stringDummy
		<< QByteArray( QByteArray("12345") );
	QTest::newRow( device->name() + "4" ) << device->name() << QString("UnicodeUTF8") << QString("signed int")
		<< doubleDummy << 12345 << stringDummy
		<< QByteArray( QByteArray("12345") );
	QTest::newRow( device->name() + "5" ) << device->name() << QString("UnicodeUTF8") << QString("unsigned int")
		<< doubleDummy << 12345 << stringDummy
		<< QByteArray( QByteArray("12345") );
	QTest::newRow( device->name() + "6" ) << device->name() << QString("UnicodeUTF8") << QString("signed long")
		<< doubleDummy << 12345 << stringDummy
		<< QByteArray( QByteArray("12345") );
	QTest::newRow( device->name() + "7" ) << device->name() << QString("UnicodeUTF8") << QString("unsigned long")
		<< doubleDummy << 12345 << stringDummy
		<< QByteArray( QByteArray("12345") );
	QTest::newRow( device->name() + "8" ) << device->name() << QString("UnicodeUTF8") << QString("float")
		<< (double)3.1415f << intDummy << stringDummy
		<< QByteArray( QByteArray("3.1415") );
	QTest::newRow( device->name() + "9" ) << device->name() << QString("UnicodeUTF8") << QString("double")
		<< 3.1415 << intDummy << stringDummy
		<< QByteArray( QByteArray("3.1415") );
	QTest::newRow( device->name() + "10" ) << device->name() << QString("UnicodeUTF8") << QString("char *")
		<< doubleDummy << intDummy << QString("I-am-a-string")
		<< QByteArray( QByteArray("I-am-a-string") );
	QTest::newRow( device->name() + "11" ) << device->name() << QString("UnicodeUTF8") << QString("QString")
		<< doubleDummy << intDummy << QString("I-am-a-string")
		<< QByteArray( QByteArray("I-am-a-string") );
	QTest::newRow( device->name() + "12" ) << device->name() << QString("UnicodeUTF8") << QString("QByteArray")
		<< doubleDummy << intDummy << QString("I-am-a-string")
		<< QByteArray( QByteArray("I-am-a-string") );

	/*
	  Then we test some special cases that have caused problems in the past.
	*/
	QTest::newRow( device->name() + "20" ) << device->name() << QString("UnicodeUTF8") << QString("QChar")
		<< doubleDummy << 0xff8c << stringDummy
		<< QByteArray( QByteArray("\xef\xbe\x8c") );
	QTest::newRow( device->name() + "21" ) << device->name() << QString("UnicodeUTF8") << QString("QChar")
		<< doubleDummy << 0x8cff << stringDummy
		<< QByteArray( QByteArray("\xe8\xb3\xbf") );
    }
}

// ------------------------------------------------------------------------------
void tst_QTextStream::qt3_do_shiftleft( QTextStream *ts )
{
    QFETCH( QString, encoding );
    QFETCH( QString, type );
    QFETCH( double,  doubleVal );
    QFETCH( int, intVal );
    QFETCH( QString,  stringVal );

    ts->setEncoding( qt3_toEncoding(encoding) );

    if ( type == "QChar" ) {
	if ( intVal >= 0 && intVal <= 0xffff )
	    *ts << QChar( intVal );
    } else if ( type == "char" ) {
	*ts << (char) intVal;
    } else if ( type == "signed short" ) {
	*ts << (signed short) intVal;
    } else if ( type == "unsigned short" ) {
	*ts << (unsigned short) intVal;
    } else if ( type == "signed int" ) {
	*ts << (signed int) intVal;
    } else if ( type == "unsigned int" ) {
	*ts << (unsigned int) intVal;
    } else if ( type == "signed long" ) {
	*ts << (signed long) intVal;
    } else if ( type == "unsigned long" ) {
	*ts << (unsigned long) intVal;
    } else if ( type == "float" ) {
	*ts << (float) doubleVal;
    } else if ( type == "double" ) {
	*ts << (double) doubleVal;
    } else if ( type == "char *" ) {
	*ts << stringVal.latin1();
    } else if ( type == "QString" ) {
	*ts << stringVal;
    } else if ( type == "QByteArray" ) {
	*ts << QByteArray( stringVal.latin1() );
    } else {
	QWARN( "Unknown type: %s" + type );
    }

    ts->flush();
}

// ------------------------------------------------------------------------------
void tst_QTextStream::qt3_operator_shiftleft()
{
    QFETCH( QString, device );
    QFETCH( QString, encoding );
    QFETCH( QByteArray, encoded );

    if ( device == "file" ) {
	QFile outFile( "qtextstream.out" );
	QVERIFY( outFile.open( QIODevice::WriteOnly ) );
	QTextStream ts( &outFile );

	qt3_do_shiftleft( &ts );
	outFile.close();

	QFile inFile( "qtextstream.out" );
	QVERIFY( inFile.open( QIODevice::ReadOnly ) );
        QCOMPARE( inFile.readAll().constData(), encoded.constData() );
    } else if(device == "resource" ) { //ONLY TO CREATE THE RESOURCE!! NOT A TEST!
	QFile outFile( resourceDir() + "operator_shiftright_" + QTest::currentDataTag() + ".data" );
	QVERIFY( outFile.open( QIODevice::WriteOnly ) );
	QTextStream ts( &outFile );
	qt3_do_shiftleft( &ts );
	outFile.close();
    } else if ( device == "bytearray" ) {
	QByteArray outArray;
	QTextStream ts(&outArray, QIODevice::WriteOnly );

	qt3_do_shiftleft( &ts );

	QVERIFY( outArray == encoded );
    } else if ( device == "buffer" ) {
	QByteArray outArray;
	QBuffer outBuffer(&outArray);
	QVERIFY( outBuffer.open(QIODevice::WriteOnly) );
	QTextStream ts( &outBuffer );

	qt3_do_shiftleft( &ts );

	QCOMPARE( outArray, encoded );
    } else if ( device == "string" ) {
	QString outString;
	QTextStream ts( &outString, QIODevice::WriteOnly );

	qt3_do_shiftleft( &ts );

	QString decodedString = qt3_decodeString( encoded, encoding );
	QCOMPARE( outString, decodedString );
    } else {
	QWARN( "Unknown device type: " + device );
    }
}

// ------------------------------------------------------------------------------
void tst_QTextStream::qt3_operator_shiftright()
{
    QFETCH( QString, device );
    QFETCH( QString, encoding );
    QFETCH( QString, type );
    QFETCH( double,  doubleVal );
    QFETCH( int, intVal );
    QFETCH( QString,  stringVal );
    QFETCH( QByteArray, encoded );

    if ( device == "file" ) {
	QFile outFile( "qtextstream.out" );
	QVERIFY( outFile.open( QIODevice::WriteOnly ) );
	QVERIFY( (int) outFile.write(encoded) == (int) encoded.size() );
	outFile.close();

	inFile = new QFile( "qtextstream.out" );
	QVERIFY( inFile->open( QIODevice::ReadOnly ) );
	ts = new QTextStream( inFile );
    } else if(device == "resource" ) {
	inResource = new QFile( ":/tst_textstream/" + resourceDir() + "operator_shiftright_" + QTest::currentDataTag() + ".data" );
	QVERIFY( inResource->open( QIODevice::ReadOnly ) );
	ts = new QTextStream( inResource );
    } else if ( device == "bytearray" ) {
	ts = new QTextStream(&encoded, QIODevice::ReadOnly);
    } else if ( device == "buffer" ) {
	inBuffer = new QBuffer(&encoded);
	QVERIFY( inBuffer->open(QIODevice::ReadOnly) );
	ts = new QTextStream( inBuffer );
    } else if ( device == "string" ) {
	inString = new QString( qt3_decodeString(encoded, encoding) );
	ts = new QTextStream(inString, QIODevice::ReadOnly);
    } else {
	QWARN( "Unknown device type: " + device );
    }

    ts->setEncoding( qt3_toEncoding(encoding) );

    if ( type == "QChar" ) {
	QChar c;
	if ( intVal >= 0 && intVal <= 0xffff )
	    *ts >> c;
	QCOMPARE( c.toLatin1(), QChar(intVal).toLatin1() );
    } else if ( type == "char" ) {
	char c;
	*ts >> c;
	QCOMPARE( c, (char) intVal );
    } else if ( type == "signed short" ) {
	signed short h;
	*ts >> h;
	QCOMPARE( h, (signed short) intVal );
    } else if ( type == "unsigned short" ) {
	unsigned short h;
	*ts >> h;
	QCOMPARE( h, (unsigned short) intVal );
    } else if ( type == "signed int" ) {
	signed int i;
	*ts >> i;
	QCOMPARE( i, (signed int) intVal );
    } else if ( type == "unsigned int" ) {
	unsigned int i;
	*ts >> i;
	QCOMPARE( i, (unsigned int) intVal );
    } else if ( type == "signed long" ) {
	signed long ell;
	*ts >> ell;
	QCOMPARE( ell, (signed long) intVal );
    } else if ( type == "unsigned long" ) {
	unsigned long ell;
	*ts >> ell;
	QCOMPARE( ell, (unsigned long) intVal );
    } else if ( type == "float" ) {
	float f;
	*ts >> f;
	QCOMPARE( f, (float) doubleVal );
    } else if ( type == "double" ) {
	double d;
	*ts >> d;
	QCOMPARE( d, (double) doubleVal );
    } else if ( type == "char *" ) {
	char *cp = new char[2048];
	*ts >> cp;
	QVERIFY( qstrcmp(cp, stringVal.latin1()) == 0 );
	delete[] cp;
    } else if ( type == "QString" ) {
	QString s;
	*ts >> s;
	QCOMPARE( s, stringVal );
    } else if ( type == "QByteArray" ) {
	QByteArray s;
	*ts >> s;
	QCOMPARE( QString::fromLatin1(s), stringVal );
    } else {
	QWARN( "Unknown type: %s" + type );
    }
}

// ------------------------------------------------------------------------------
QTextStream::Encoding tst_QTextStream::qt3_toEncoding( const QString &str )
{
    if ( str == "Locale" )
	return QTextStream::Locale;
    else if ( str == "Latin1" )
	return QTextStream::Latin1;
    else if ( str == "Unicode" )
	return QTextStream::Unicode;
    else if ( str == "UnicodeNetworkOrder" )
	return QTextStream::UnicodeNetworkOrder;
    else if ( str == "UnicodeReverse" )
	return QTextStream::UnicodeReverse;
    else if ( str == "RawUnicode" )
	return QTextStream::RawUnicode;
    else if ( str == "UnicodeUTF8" )
	return QTextStream::UnicodeUTF8;

    QWARN( "No such encoding " + str );
    return QTextStream::Latin1;
}

// ------------------------------------------------------------------------------
QString tst_QTextStream::qt3_decodeString( QByteArray array, const QString& encoding )
{
    switch ( qt3_toEncoding(encoding) ) {
    case QTextStream::Locale:
	return QString::fromLocal8Bit( array.data(), array.size() );
    case QTextStream::Latin1:
	return QString::fromLatin1( array.data(), array.size() );
    case QTextStream::Unicode:
    case QTextStream::UnicodeNetworkOrder:
    case QTextStream::UnicodeReverse:
    case QTextStream::RawUnicode:
	QWARN( "Unicode not implemented ###" );
	return QString();
    case QTextStream::UnicodeUTF8:
	return QString::fromUtf8( array.data(), array.size() );
    default:
	QWARN( "Unhandled encoding" );
	return QString();
    }
}

// ************************************************

// ------------------------------------------------------------------------------
void tst_QTextStream::qt3_createWriteStream( QTextStream *&os )
{
    QFETCH( QString, device );

    if ( device == "file" ) {
	outFile = new QFile( "qtextstream.out" );
	QVERIFY( outFile->open( QIODevice::WriteOnly ) );
	os = new QTextStream( outFile );
        os->setCodec(QTextCodec::codecForName("ISO-8859-1"));
        os->setAutoDetectUnicode(true);
    } else if(device == "resource" ) { //ONLY TO CREATE THE RESOURCE!! NOT A TEST!
	outFile = new QFile( resourceDir() +
                             QTest::currentTestFunction() +
                             "_" + QTest::currentDataTag() + ".data" );
	QVERIFY( outFile->open( QIODevice::WriteOnly ) );
	os = new QTextStream( outFile );
        os->setCodec(QTextCodec::codecForName("ISO-8859-1"));
        os->setAutoDetectUnicode(true);
    } else if ( device == "bytearray" ) {
	inArray = new QByteArray;
	os = new QTextStream(inArray, QIODevice::WriteOnly);
        os->setCodec(QTextCodec::codecForName("ISO-8859-1"));
        os->setAutoDetectUnicode(true);
    } else if ( device == "buffer" ) {
	inBuffer = new QBuffer;
	QVERIFY( inBuffer->open(QIODevice::WriteOnly) );
	os = new QTextStream( inBuffer );
        os->setCodec(QTextCodec::codecForName("ISO-8859-1"));
        os->setAutoDetectUnicode(true);
    } else if ( device == "string" ) {
	inString = new QString;
	os = new QTextStream( inString, QIODevice::WriteOnly );
        os->setCodec(QTextCodec::codecForName("ISO-8859-1"));
        os->setAutoDetectUnicode(true);
    } else {
	QWARN( "Error creating write stream: Unknown device type '" + device + "'" );
    }

    QFETCH( QString, encoding );
    os->setEncoding( qt3_toEncoding( encoding ));
}

// ------------------------------------------------------------------------------
void tst_QTextStream::qt3_closeWriteStream( QTextStream *os )
{
    QFETCH( QString, device );

    os->flush();
    if ( os->device() )
	os->device()->close();
}

// ------------------------------------------------------------------------------
void tst_QTextStream::qt3_createReadStream( QTextStream *&is )
{
    QFETCH( QString, device );

    if ( device == "file" ) {
	inFile = new QFile( "qtextstream.out" );
	is = new QTextStream( inFile );
        is->setCodec(QTextCodec::codecForName("ISO-8859-1"));
        is->setAutoDetectUnicode(true);
	QVERIFY( inFile->open( QIODevice::ReadOnly ) );
    } else if(device == "resource") {
	inResource = new QFile( ":/tst_textstream/" + resourceDir() +
                                QTest::currentTestFunction() +
                                "_" + QTest::currentDataTag() + ".data" );
	is = new QTextStream( inResource );
        is->setCodec(QTextCodec::codecForName("ISO-8859-1"));
        is->setAutoDetectUnicode(true);
	QVERIFY( inResource->open( QIODevice::ReadOnly ) );
    } else if ( device == "bytearray" ) {
	is = new QTextStream(inArray, QIODevice::ReadOnly);
        is->setCodec(QTextCodec::codecForName("ISO-8859-1"));
        is->setAutoDetectUnicode(true);
    } else if ( device == "buffer" ) {
	QVERIFY( inBuffer->open(QIODevice::ReadOnly) );
	is = new QTextStream( inBuffer );
        is->setCodec(QTextCodec::codecForName("ISO-8859-1"));
        is->setAutoDetectUnicode(true);
    } else if ( device == "string" ) {
	is = new QTextStream( inString, QIODevice::ReadOnly );
        is->setCodec(QTextCodec::codecForName("ISO-8859-1"));
        is->setAutoDetectUnicode(true);
    } else {
	QWARN( "Error creating read stream: Unknown device type '" + device + "'" );
    }

    QFETCH( QString, encoding );
    is->setEncoding( qt3_toEncoding( encoding ));

    if (!file_is_empty) {
	QVERIFY( !is->atEnd() );
    }
}

// ------------------------------------------------------------------------------
void tst_QTextStream::qt3_closeReadStream( QTextStream *is )
{
    QVERIFY( is->atEnd() );

    if ( is->device() )
	is->device()->close();
}

// **************** QChar ****************

// ------------------------------------------------------------------------------
void tst_QTextStream::qt3_operator_shift_QChar_data()
{
    QTest::addColumn<int>("modes");
    QTest::addColumn<QString>("device");
    QTest::addColumn<QString>("encoding");
    QTest::addColumn<ushort>("qchar");

    for ( int i=0; !devices[i].name().isNull(); i++ ) {
        tst_IODevice *device = devices+i;
	for ( int e=0; encodings[e] != 0; e++ ) {
	    QString encoding = encodings[e];

	    QString tag = device->name() + "_" + encoding + "_";
	    QTest::newRow( tag + "0" ) << device->modes << device->name() << encoding << QChar( 'A' ).unicode();
	    QTest::newRow( tag + "1" ) << device->modes << device->name() << encoding << QChar( 'B' ).unicode();
	    QTest::newRow( tag + "2" ) << device->modes << device->name() << encoding << QChar( 'Z' ).unicode();
	    QTest::newRow( tag + "3" ) << device->modes << device->name() << encoding << QChar( 'z' ).unicode();
	    QTest::newRow( tag + "4" ) << device->modes << device->name() << encoding << QChar( '@' ).unicode();
	}
    }
}

// ------------------------------------------------------------------------------
void tst_QTextStream::qt3_operator_shift_QChar()
{
    QFETCH( int, modes );
    if(modes & QIODevice::WriteOnly) {
        qt3_createWriteStream( os );
        qt3_write_QChar( os );
        qt3_closeWriteStream( os );
    }
    if(modes & QIODevice::ReadOnly) {
        qt3_createReadStream( is );
        qt3_read_QChar( is );
        qt3_closeReadStream( is );
    }
}

// ------------------------------------------------------------------------------
void tst_QTextStream::qt3_read_QChar( QTextStream *s )
{
    QFETCH( ushort, qchar );
    QChar expected( qchar );
    QChar actual;
    *s >> actual;
    QCOMPARE( actual, expected );
}

// ------------------------------------------------------------------------------
void tst_QTextStream::qt3_write_QChar( QTextStream *s )
{
    QFETCH( ushort, qchar );
    QChar actual( qchar );
    *s << actual;
}

// **************** char ****************

// ------------------------------------------------------------------------------
void tst_QTextStream::qt3_operator_shift_char_data()
{
    QTest::addColumn<int>("modes");
    QTest::addColumn<QString>("device");
    QTest::addColumn<QString>("encoding");
    QTest::addColumn<int>("ch");

    for ( int i=0; !devices[i].name().isNull(); i++ ) {
        tst_IODevice *device = devices+i;
	for ( int e=0; encodings[e] != 0; e++ ) {
	    QString encoding = encodings[e];

	    QString tag = device->name() + "_" + encoding + "_";
	    QTest::newRow( tag + "0" ) << device->modes << device->name() << encoding << int('A');
	    QTest::newRow( tag + "1" ) << device->modes << device->name() << encoding << int('B');
	    QTest::newRow( tag + "2" ) << device->modes << device->name() << encoding << int('Z');
	    QTest::newRow( tag + "3" ) << device->modes << device->name() << encoding << int(14);
	    QTest::newRow( tag + "4" ) << device->modes << device->name() << encoding << int('0');
	}
    }
}

// ------------------------------------------------------------------------------
void tst_QTextStream::qt3_operator_shift_char()
{
    QFETCH( int, modes );
    if(modes & QIODevice::WriteOnly) {
        qt3_createWriteStream( os );
        qt3_write_char( os );
        qt3_closeWriteStream( os );
    }
    if(modes & QIODevice::ReadOnly) {
        qt3_createReadStream( is );
        qt3_read_char( is );
        qt3_closeReadStream( is );
    }
}

// ------------------------------------------------------------------------------
void tst_QTextStream::qt3_read_char( QTextStream *s )
{
    QFETCH( int, ch );
    char c(ch);
    char exp;
    *s >> exp;
    QCOMPARE( exp, c );
}

// ------------------------------------------------------------------------------
void tst_QTextStream::qt3_write_char( QTextStream *s )
{
    QFETCH( int, ch );
    char c(ch);
    *s << c;
}

// **************** short ****************

// ------------------------------------------------------------------------------
void tst_QTextStream::qt3_operator_shift_short_data()
{
    QTest::addColumn<int>("modes");
    QTest::addColumn<QString>("device");
    QTest::addColumn<QString>("encoding");
    QTest::addColumn<short>("ss");

    for ( int i=0; !devices[i].name().isNull(); i++ ) {
        tst_IODevice *device = devices+i;
	for ( int e=0; encodings[e] != 0; e++ ) {
	    QString encoding = encodings[e];

	    QString tag = device->name() + "_" + encoding + "_";
	    QTest::newRow( tag + "0" ) << device->modes << device->name() << encoding << short(0);
	    QTest::newRow( tag + "1" ) << device->modes << device->name() << encoding << short(-1);
	    QTest::newRow( tag + "2" ) << device->modes << device->name() << encoding << short(1);
	    QTest::newRow( tag + "3" ) << device->modes << device->name() << encoding << short(255);
	    QTest::newRow( tag + "4" ) << device->modes << device->name() << encoding << short(-254);
	}
    }
}

// ------------------------------------------------------------------------------
void tst_QTextStream::qt3_operator_shift_short()
{
    QFETCH( int, modes );
    if(modes & QIODevice::WriteOnly) {
        qt3_createWriteStream( os );
        qt3_write_short( os );
        qt3_closeWriteStream( os );
    }
    if(modes & QIODevice::ReadOnly) {
        qt3_createReadStream( is );
        qt3_read_short( is );
        qt3_closeReadStream( is );
    }
}

// ------------------------------------------------------------------------------
void tst_QTextStream::qt3_read_short( QTextStream *s )
{
    QFETCH( short, ss );
    short exp;
    QString A, B;
    *s >> A >> exp >> B;
    QCOMPARE( A, QString("A") );
    QCOMPARE( B, QString("B") );
    QCOMPARE( exp, ss );
    s->skipWhiteSpace();
}

// ------------------------------------------------------------------------------
void tst_QTextStream::qt3_write_short( QTextStream *s )
{
    QFETCH( short, ss );
    *s << " A " << ss << " B ";
}

// **************** ushort ****************

// ------------------------------------------------------------------------------
void tst_QTextStream::qt3_operator_shift_ushort_data()
{
    QTest::addColumn<int>("modes");
    QTest::addColumn<QString>("device");
    QTest::addColumn<QString>("encoding");
    QTest::addColumn<ushort>("us");

    for ( int i=0; !devices[i].name().isNull(); i++ ) {
        tst_IODevice *device = devices+i;
	for ( int e=0; encodings[e] != 0; e++ ) {
	    QString encoding = encodings[e];

	    QString tag = device->name() + "_" + encoding + "_";
	    QTest::newRow( tag + "0" ) << device->modes << device->name() << encoding << ushort(0);
	    QTest::newRow( tag + "1" ) << device->modes << device->name() << encoding << ushort(1);
	    QTest::newRow( tag + "2" ) << device->modes << device->name() << encoding << ushort(10);
	    QTest::newRow( tag + "3" ) << device->modes << device->name() << encoding << ushort(255);
	    QTest::newRow( tag + "4" ) << device->modes << device->name() << encoding << ushort(512);
	}
    }
}

// ------------------------------------------------------------------------------
void tst_QTextStream::qt3_operator_shift_ushort()
{
    QFETCH( int, modes );
    if(modes & QIODevice::WriteOnly) {
        qt3_createWriteStream( os );
        qt3_write_ushort( os );
        qt3_closeWriteStream( os );
    }
    if(modes & QIODevice::ReadOnly) {
        qt3_createReadStream( is );
        qt3_read_ushort( is );
        qt3_closeReadStream( is );
    }
}

// ------------------------------------------------------------------------------
void tst_QTextStream::qt3_read_ushort( QTextStream *s )
{
    QFETCH( ushort, us );
    ushort exp;
    QString A, B;
    *s >> A >> exp >> B;
    QCOMPARE( A, QString("A") );
    QCOMPARE( B, QString("B") );
    QCOMPARE( exp, us );
    s->skipWhiteSpace();
}

// ------------------------------------------------------------------------------
void tst_QTextStream::qt3_write_ushort( QTextStream *s )
{
    QFETCH( ushort, us );
    *s << " A " << us << " B ";
}

// **************** int ****************

// ------------------------------------------------------------------------------
void tst_QTextStream::qt3_operator_shift_int_data()
{
    QTest::addColumn<int>("modes");
    QTest::addColumn<QString>("device");
    QTest::addColumn<QString>("encoding");
    QTest::addColumn<int>("si");

    for ( int i=0; !devices[i].name().isNull(); i++ ) {
        tst_IODevice *device = devices+i;
	for ( int e=0; encodings[e] != 0; e++ ) {
	    QString encoding = encodings[e];

	    QString tag = device->name() + "_" + encoding + "_";
	    QTest::newRow( tag + "0" ) << device->modes << device->name() << encoding << int(0);
	    QTest::newRow( tag + "1" ) << device->modes << device->name() << encoding << int(1);
	    QTest::newRow( tag + "2" ) << device->modes << device->name() << encoding << int(10);
	    QTest::newRow( tag + "3" ) << device->modes << device->name() << encoding << int(255);
	    QTest::newRow( tag + "4" ) << device->modes << device->name() << encoding << int(512);
	    QTest::newRow( tag + "5" ) << device->modes << device->name() << encoding << int(-1);
	    QTest::newRow( tag + "6" ) << device->modes << device->name() << encoding << int(-10);
	    QTest::newRow( tag + "7" ) << device->modes << device->name() << encoding << int(-255);
	    QTest::newRow( tag + "8" ) << device->modes << device->name() << encoding << int(-512);
	}
    }
}

// ------------------------------------------------------------------------------
void tst_QTextStream::qt3_operator_shift_int()
{
    QFETCH( int, modes );
    if(modes & QIODevice::WriteOnly) {
        qt3_createWriteStream( os );
        qt3_write_int( os );
        qt3_closeWriteStream( os );
    }
    if(modes & QIODevice::ReadOnly) {
        qt3_createReadStream( is );
        qt3_read_int( is );
        qt3_closeReadStream( is );
    }
}

// ------------------------------------------------------------------------------
void tst_QTextStream::qt3_read_int( QTextStream *s )
{
    QFETCH( int, si );
    int exp;
    QString A, B;
    *s >> A >> exp >> B;
    QCOMPARE( A, QString("A") );
    QCOMPARE( B, QString("B") );
    QCOMPARE( exp, si );
    s->skipWhiteSpace();
}

// ------------------------------------------------------------------------------
void tst_QTextStream::qt3_write_int( QTextStream *s )
{
    QFETCH( int, si );
    *s << " A " << si << " B ";
}

// **************** uint ****************

// ------------------------------------------------------------------------------
void tst_QTextStream::qt3_operator_shift_uint_data()
{
    QTest::addColumn<int>("modes");
    QTest::addColumn<QString>("device");
    QTest::addColumn<QString>("encoding");
    QTest::addColumn<uint>("ui");

    for ( int i=0; !devices[i].name().isNull(); i++ ) {
        tst_IODevice *device = devices+i;
	for ( int e=0; encodings[e] != 0; e++ ) {
	    QString encoding = encodings[e];

	    QString tag = device->name() + "_" + encoding + "_";
	    QTest::newRow( tag + "0" ) << device->modes << device->name() << encoding << uint(0);
	    QTest::newRow( tag + "1" ) << device->modes << device->name() << encoding << uint(1);
	    QTest::newRow( tag + "2" ) << device->modes << device->name() << encoding << uint(10);
	    QTest::newRow( tag + "3" ) << device->modes << device->name() << encoding << uint(255);
	    QTest::newRow( tag + "4" ) << device->modes << device->name() << encoding << uint(512);
	}
    }
}

// ------------------------------------------------------------------------------
void tst_QTextStream::qt3_operator_shift_uint()
{
    QFETCH( int, modes );
    if(modes & QIODevice::WriteOnly) {
        qt3_createWriteStream( os );
        qt3_write_uint( os );
        qt3_closeWriteStream( os );
    }
    if(modes & QIODevice::ReadOnly) {
        qt3_createReadStream( is );
        qt3_read_uint( is );
        qt3_closeReadStream( is );
    }
}

// ------------------------------------------------------------------------------
void tst_QTextStream::qt3_read_uint( QTextStream *s )
{
    QFETCH( uint, ui );
    uint exp;
    QString A, B;
    *s >> A >> exp >> B;
    QCOMPARE( A, QString("A") );
    QCOMPARE( B, QString("B") );
    QCOMPARE( exp, ui );
    s->skipWhiteSpace();
}

// ------------------------------------------------------------------------------
void tst_QTextStream::qt3_write_uint( QTextStream *s )
{
    QFETCH( uint, ui );
    *s << " A " << ui << " B ";
}

// **************** long ****************

// ------------------------------------------------------------------------------
void tst_QTextStream::qt3_operator_shift_long_data()
{
    QTest::addColumn<int>("modes");
    QTest::addColumn<QString>("device");
    QTest::addColumn<QString>("encoding");
    QTest::addColumn<long>("sl");

    for ( int i=0; !devices[i].name().isNull(); i++ ) {
        tst_IODevice *device = devices+i;
	for ( int e=0; encodings[e] != 0; e++ ) {
	    QString encoding = encodings[e];

	    QString tag = device->name() + "_" + encoding + "_";
	    QTest::newRow( tag + "0" ) << device->modes << device->name() << encoding << long(0);
	    QTest::newRow( tag + "1" ) << device->modes << device->name() << encoding << long(1);
	    QTest::newRow( tag + "2" ) << device->modes << device->name() << encoding << long(10);
	    QTest::newRow( tag + "3" ) << device->modes << device->name() << encoding << long(255);
	    QTest::newRow( tag + "4" ) << device->modes << device->name() << encoding << long(65535);
	    QTest::newRow( tag + "5" ) << device->modes << device->name() << encoding << long(-1);
	    QTest::newRow( tag + "6" ) << device->modes << device->name() << encoding << long(-10);
	    QTest::newRow( tag + "7" ) << device->modes << device->name() << encoding << long(-255);
	    QTest::newRow( tag + "8" ) << device->modes << device->name() << encoding << long(-65534);
	}
    }
}

// ------------------------------------------------------------------------------
void tst_QTextStream::qt3_operator_shift_long()
{
    QFETCH( int, modes );
    if(modes & QIODevice::WriteOnly) {
        qt3_createWriteStream( os );
        qt3_write_long( os );
        qt3_closeWriteStream( os );
    }
    if(modes & QIODevice::ReadOnly) {
        qt3_createReadStream( is );
        qt3_read_long( is );
        qt3_closeReadStream( is );
    }
}

// ------------------------------------------------------------------------------
void tst_QTextStream::qt3_read_long( QTextStream *s )
{
    QFETCH( long, sl );
    long exp;
    QString A, B;
    *s >> A >> exp >> B;
    QCOMPARE( A, QString("A") );
    QCOMPARE( B, QString("B") );
    QCOMPARE( exp, sl );
    s->skipWhiteSpace();
}

// ------------------------------------------------------------------------------
void tst_QTextStream::qt3_write_long( QTextStream *s )
{
    QFETCH( long, sl );
    *s << " A " << sl << " B ";
}

// **************** long ****************

// ------------------------------------------------------------------------------
void tst_QTextStream::qt3_operator_shift_ulong_data()
{
    QTest::addColumn<int>("modes");
    QTest::addColumn<QString>("device");
    QTest::addColumn<QString>("encoding");
    QTest::addColumn<ulong>("ul");

    for ( int i=0; !devices[i].name().isNull(); i++ ) {
        tst_IODevice *device = devices+i;
	for ( int e=0; encodings[e] != 0; e++ ) {
	    QString encoding = encodings[e];

	    QString tag = device->name() + "_" + encoding + "_";
	    QTest::newRow( tag + "0" ) << device->modes << device->name() << encoding << ulong(0);
	    QTest::newRow( tag + "1" ) << device->modes << device->name() << encoding << ulong(1);
	    QTest::newRow( tag + "2" ) << device->modes << device->name() << encoding << ulong(10);
	    QTest::newRow( tag + "3" ) << device->modes << device->name() << encoding << ulong(255);
	    QTest::newRow( tag + "4" ) << device->modes << device->name() << encoding << ulong(65535);
	}
    }
}

// ------------------------------------------------------------------------------
void tst_QTextStream::qt3_operator_shift_ulong()
{
    QFETCH( int, modes );
    if(modes & QIODevice::WriteOnly) {
        qt3_createWriteStream( os );
        qt3_write_ulong( os );
        qt3_closeWriteStream( os );
    }
    if(modes & QIODevice::ReadOnly) {
        qt3_createReadStream( is );
        qt3_read_ulong( is );
        qt3_closeReadStream( is );
    }
}

// ------------------------------------------------------------------------------
void tst_QTextStream::qt3_read_ulong( QTextStream *s )
{
    QFETCH( ulong, ul );
    ulong exp;
    QString A, B;
    *s >> A >> exp >> B;
    QCOMPARE( A, QString("A") );
    QCOMPARE( B, QString("B") );
    QCOMPARE( exp, ul );
    s->skipWhiteSpace();
}

// ------------------------------------------------------------------------------
void tst_QTextStream::qt3_write_ulong( QTextStream *s )
{
    QFETCH( ulong, ul );
    *s << " A " << ul << " B ";
}

// **************** float ****************

// ------------------------------------------------------------------------------
void tst_QTextStream::qt3_operator_shift_float_data()
{
    QTest::addColumn<int>("modes");
    QTest::addColumn<QString>("device");
    QTest::addColumn<QString>("encoding");
    QTest::addColumn<float>("f");

    for ( int i=0; !devices[i].name().isNull(); i++ ) {
        tst_IODevice *device = devices+i;
	for ( int e=0; encodings[e] != 0; e++ ) {
	    QString encoding = encodings[e];

	    QString tag = device->name() + "_" + encoding + "_";
	    QTest::newRow( tag + "0" ) << device->modes << device->name() << encoding << float(0.0);
	    QTest::newRow( tag + "1" ) << device->modes << device->name()  << encoding << float(0.0001);
	    QTest::newRow( tag + "2" ) << device->modes << device->name()  << encoding << float(-0.0001);
	    QTest::newRow( tag + "3" ) << device->modes << device->name()  << encoding << float(3.45678);
	    QTest::newRow( tag + "4" ) << device->modes << device->name()  << encoding << float(-3.45678);
	}
    }
}

// ------------------------------------------------------------------------------
void tst_QTextStream::qt3_operator_shift_float()
{
    QFETCH( int, modes );
    if(modes & QIODevice::WriteOnly) {
        qt3_createWriteStream( os );
        qt3_write_float( os );
        qt3_closeWriteStream( os );
    }
    if(modes & QIODevice::ReadOnly) {
        qt3_createReadStream( is );
        qt3_read_float( is );
        qt3_closeReadStream( is );
    }
}

// ------------------------------------------------------------------------------
void tst_QTextStream::qt3_read_float( QTextStream *s )
{
    QFETCH( float, f );
    float exp;
    QString A, B;
    *s >> A >> exp >> B;
    QCOMPARE( A, QString("A") );
    QCOMPARE( B, QString("B") );
    QCOMPARE( exp, f );
    s->skipWhiteSpace();
}

// ------------------------------------------------------------------------------
void tst_QTextStream::qt3_write_float( QTextStream *s )
{
    QFETCH( float, f );
    *s << " A " << f << " B ";
}

// **************** double ****************

// ------------------------------------------------------------------------------
void tst_QTextStream::qt3_operator_shift_double_data()
{
    QTest::addColumn<int>("modes");
    QTest::addColumn<QString>("device");
    QTest::addColumn<QString>("encoding");
    QTest::addColumn<double>("d");

    for ( int i=0; !devices[i].name().isNull(); i++ ) {
        tst_IODevice *device = devices+i;
	for ( int e=0; encodings[e] != 0; e++ ) {
	    QString encoding = encodings[e];

	    QString tag = device->name() + "_" + encoding + "_";
	    QTest::newRow( tag + "0" ) << device->modes << device->name() << encoding << double(0.0);
	    QTest::newRow( tag + "1" ) << device->modes << device->name() << encoding << double(0.0001);
	    QTest::newRow( tag + "2" ) << device->modes << device->name() << encoding << double(-0.0001);
	    QTest::newRow( tag + "3" ) << device->modes << device->name() << encoding << double(3.45678);
	    QTest::newRow( tag + "4" ) << device->modes << device->name() << encoding << double(-3.45678);
	    QTest::newRow( tag + "5" ) << device->modes << device->name() << encoding << double(1.23456789);
	    QTest::newRow( tag + "6" ) << device->modes << device->name() << encoding << double(-1.23456789);
	}
    }
}

// ------------------------------------------------------------------------------
void tst_QTextStream::qt3_operator_shift_double()
{
    QFETCH( int, modes );
    if(modes & QIODevice::WriteOnly) {
        qt3_createWriteStream( os );
        os->precision( 10 );
        qt3_write_double( os );
        qt3_closeWriteStream( os );
    }
    if(modes & QIODevice::ReadOnly) {
        qt3_createReadStream( is );
        is->precision( 10 );
        qt3_read_double( is );
        qt3_closeReadStream( is );
    }
}

// ------------------------------------------------------------------------------
void tst_QTextStream::qt3_read_double( QTextStream *s )
{
    QFETCH( double, d );
    double exp;
    QString A, B;
    *s >> A >> exp >> B;
    QCOMPARE( A, QString("A") );
    QCOMPARE( B, QString("B") );
    QCOMPARE( exp, d );
    s->skipWhiteSpace();
}

// ------------------------------------------------------------------------------
void tst_QTextStream::qt3_write_double( QTextStream *s )
{
    QFETCH( double, d );
    *s << " A " << d << " B ";
}

// **************** QString ****************

// ------------------------------------------------------------------------------
void tst_QTextStream::qt3_operator_shift_QString_data()
{
    QTest::addColumn<int>("modes");
    QTest::addColumn<QString>("device");
    QTest::addColumn<QString>("encoding");
    QTest::addColumn<QString>("str");
    QTest::addColumn<bool>("multi_str");
    QTest::addColumn<bool>("zero_length");

    for ( int i=0; !devices[i].name().isNull(); i++ ) {
        tst_IODevice *device = devices+i;
	for ( int e=0; encodings[e] != 0; e++ ) {
	    QString encoding = encodings[e];

	    QString tag = device->name() + "_" + encoding + "_";
	    QTest::newRow( tag + "0" ) << device->modes << device->name() << encoding << QString("") << bool(FALSE) << bool(TRUE);
	    QTest::newRow( tag + "1" ) << device->modes << device->name()  << encoding << QString() << bool(FALSE) << bool(TRUE);
	    QTest::newRow( tag + "2" ) << device->modes << device->name()  << encoding << QString("foo") << bool(FALSE) << bool(FALSE);
	    QTest::newRow( tag + "3" ) << device->modes << device->name()  << encoding << QString("foo\nbar") << bool(TRUE) << bool(FALSE);
	    QTest::newRow( tag + "4" ) << device->modes << device->name()  << encoding << QString("cjacka ckha cka ckah ckac kahckadhcbkgdk vkzdfbvajef vkahv") << bool(TRUE) << bool(FALSE);
	}
    }
}

// ------------------------------------------------------------------------------
void tst_QTextStream::qt3_operator_shift_QString()
{
    QFETCH( bool, zero_length );
    file_is_empty = zero_length;

    QFETCH( int, modes );
    if(modes & QIODevice::WriteOnly) {
        qt3_createWriteStream( os );
        qt3_write_QString( os );
        qt3_closeWriteStream( os );
    }
    if(modes & QIODevice::ReadOnly) {
        qt3_createReadStream( is );
        qt3_read_QString( is );
        qt3_closeReadStream( is );
    }
}

// ------------------------------------------------------------------------------
void tst_QTextStream::qt3_read_QString( QTextStream *s )
{
    QString exp;
    QFETCH( QString, str );
    if (str.isNull())
	str = "";

    QFETCH( bool, multi_str );
    if (!multi_str) {
	*s >> exp;
	QCOMPARE( exp, str );
    } else {
	QStringList l;
	l = QStringList::split( " ", str );
	if (l.count() < 2)
	    l = QStringList::split( "\n", str );
	for (int i=0; i<l.count(); i++) {
	    *s >> exp;
	    QCOMPARE( exp, l[i] );
	}
    }
}

// ------------------------------------------------------------------------------
void tst_QTextStream::qt3_write_QString( QTextStream *s )
{
    QFETCH( QString, str );
    *s << str;
}

// **************** QByteArray ****************

// ------------------------------------------------------------------------------
void tst_QTextStream::qt3_operator_shift_QByteArray_data()
{
    QTest::addColumn<int>("modes");
    QTest::addColumn<QString>("device");
    QTest::addColumn<QString>("encoding");
    QTest::addColumn<QByteArray>("cs");
    QTest::addColumn<bool>("multi_str");
    QTest::addColumn<bool>("zero_length");

    for ( int i=0; !devices[i].name().isNull(); i++ ) {
        tst_IODevice *device = devices+i;
	for ( int e=0; encodings[e] != 0; e++ ) {
	    QString encoding = encodings[e];

	    QString tag = device->name() + "_" + encoding + "_";
	    QTest::newRow( tag + "0" ) << device->modes << device->name() << encoding << QByteArray("") << bool(FALSE) << bool(TRUE);
	    QTest::newRow( tag + "1" ) << device->modes << device->name() << encoding << QByteArray(0) << bool(FALSE) << bool(TRUE);
	    QTest::newRow( tag + "2" ) << device->modes << device->name() << encoding << QByteArray("foo") << bool(FALSE) << bool(FALSE);
	    QTest::newRow( tag + "3" ) << device->modes << device->name() << encoding << QByteArray("foo\nbar") << bool(TRUE) << bool(FALSE);
	    QTest::newRow( tag + "4" ) << device->modes << device->name() << encoding << QByteArray("cjacka ckha cka ckah ckac kahckadhcbkgdk vkzdfbvajef vkahv") << bool(TRUE) << bool(FALSE);
	}
    }
}

// ------------------------------------------------------------------------------
void tst_QTextStream::qt3_operator_shift_QByteArray()
{
    QFETCH( bool, zero_length );
    file_is_empty = zero_length;

    QFETCH( int, modes );
    if(modes & QIODevice::WriteOnly) {
        qt3_createWriteStream( os );
        qt3_write_QByteArray( os );
        qt3_closeWriteStream( os );
    }
    if(modes & QIODevice::ReadOnly) {
        qt3_createReadStream( is );
        qt3_read_QByteArray( is );
        qt3_closeReadStream( is );
    }
}

// ------------------------------------------------------------------------------
void tst_QTextStream::qt3_read_QByteArray( QTextStream *s )
{
/*
    QFETCH( QByteArray, cs );
    QByteArray exp;
    *s >> exp;
    QCOMPARE( exp, cs );
*/
    QByteArray exp;
    QFETCH( QByteArray, cs );
    if (cs.isNull())
	cs = "";

    QFETCH( bool, multi_str );
    if (!multi_str) {
	*s >> exp;
	QCOMPARE( exp.constData(), cs.constData() );
    } else {
	QStringList l;
	l = QStringList::split( " ", cs );
	if (l.count() < 2)
	    l = QStringList::split( "\n", cs );
	for (int i=0; i<l.count(); i++) {
	    *s >> exp;
	    QCOMPARE( exp, QByteArray(l[i].toAscii()) );
	}
    }
}

// ------------------------------------------------------------------------------
static void qt3_generateLineData( bool for_QString )
{
    QTest::addColumn<QByteArray>("input");
    QTest::addColumn<QStringList>("output");

    QTest::newRow("emptyer") << QByteArray() << QStringList();
    QTest::newRow("lf") << QByteArray("\n") << (QStringList() << "");
    QTest::newRow("crlf") << QByteArray("\r\n") << (QStringList() << "");
    QTest::newRow("oneline/nothing") << QByteArray("ole") << (QStringList() << "ole");
    QTest::newRow("oneline/lf") << QByteArray("ole\n") << (QStringList() << "ole");
    QTest::newRow("oneline/crlf") << QByteArray("ole\r\n") << (QStringList() << "ole");
    QTest::newRow("twolines/lf/lf") << QByteArray("ole\ndole\n") << (QStringList() << "ole" << "dole");
    QTest::newRow("twolines/crlf/crlf") << QByteArray("ole\r\ndole\r\n") << (QStringList() << "ole" << "dole");
    QTest::newRow("twolines/lf/crlf") << QByteArray("ole\ndole\r\n") << (QStringList() << "ole" << "dole");
    QTest::newRow("twolines/lf/nothing") << QByteArray("ole\ndole") << (QStringList() << "ole" << "dole");
    QTest::newRow("twolines/crlf/nothing") << QByteArray("ole\r\ndole") << (QStringList() << "ole" << "dole");
    QTest::newRow("threelines/lf/lf/lf") << QByteArray("ole\ndole\ndoffen\n") << (QStringList() << "ole" << "dole" << "doffen");
    QTest::newRow("threelines/crlf/crlf/crlf") << QByteArray("ole\r\ndole\r\ndoffen\r\n") << (QStringList() << "ole" << "dole" << "doffen");
    QTest::newRow("threelines/crlf/crlf/nothing") << QByteArray("ole\r\ndole\r\ndoffen") << (QStringList() << "ole" << "dole" << "doffen");

    if (!for_QString) {
        QTest::newRow("unicode/nothing") << QByteArray("\xfe\xff\x00\xe5\x00\x67\x00\x65", 8) << (QStringList() << "\345ge");
        QTest::newRow("unicode-little/nothing") << QByteArray("\xff\xfe\xe5\x00\x67\x00\x65\x00", 8) << (QStringList() << "\345ge");
        QTest::newRow("unicode/lf")        << QByteArray("\xfe\xff\x00\xe5\x00\x67\x00\x65\x00\x0a", 10) << (QStringList() << "\345ge");
        QTest::newRow("unicode-little/lf") << QByteArray("\xff\xfe\xe5\x00\x67\x00\x65\x00\x0a\x00", 10) << (QStringList() << "\345ge");

        QTest::newRow("unicode/twolines")  << QByteArray("\xfe\xff\x00\xe5\x00\x67\x00\x65\x00\x0a\x00\xe5\x00\x67\x00\x65\x00\x0a", 18) << (QStringList() << "\345ge" << "\345ge");
        QTest::newRow("unicode-little/twolines")  << QByteArray("\xff\xfe\xe5\x00\x67\x00\x65\x00\x0a\x00\xe5\x00\x67\x00\x65\x00\x0a\x00", 18) << (QStringList() << "\345ge" << "\345ge");

        QTest::newRow("unicode/threelines")
            << QByteArray("\xfe\xff"
                          "\x00\xe5\x00\x67\x00\x65\x00\x0a"
                          "\x00\xe5\x00\x67\x00\x65\x00\x0a"
                          "\x00\xe5\x00\x67\x00\x65\x00\x0a", 26)
            << (QStringList() << "\345ge" << "\345ge" << "\345ge");
        QTest::newRow("unicode-little/threelines")
            << QByteArray("\xff\xfe"
                          "\xe5\x00\x67\x00\x65\x00\x0a\x00"
                          "\xe5\x00\x67\x00\x65\x00\x0a\x00"
                          "\xe5\x00\x67\x00\x65\x00\x0a\x00", 26)
            << (QStringList() << "\345ge" << "\345ge" << "\345ge");
    }

    // partials
    QTest::newRow("cr") << QByteArray("\r") << (QStringList() << "");
    QTest::newRow("oneline/cr") << QByteArray("ole\r") << (QStringList() << "ole");
    if (!for_QString)
        QTest::newRow("unicode/cr") << QByteArray("\xfe\xff\x00\xe5\x00\x67\x00\x65\x00\x0d", 10) << (QStringList() << "\345ge");
}

// ------------------------------------------------------------------------------
void tst_QTextStream::qt3_writeDataToFileReadAsLines_data()
{
    qt3_generateLineData(false);
}

// ------------------------------------------------------------------------------
QByteArray pretty(const QString &input)
{
    QByteArray data;

    QByteArray arr = input.toLatin1();
    for (int i = 0; i < arr.size(); ++i) {
        char buf[64];
        memset(buf, 0, sizeof(buf));
        char ch = arr.at(i);
        sprintf(buf, "\\%2hhx", ch);
        data += buf;
    }

    return data;
}

// ------------------------------------------------------------------------------
void tst_QTextStream::qt3_writeDataToFileReadAsLines()
{
    QFETCH(QByteArray, input);
    QFETCH(QStringList, output);

    QFile::remove("eoltest");

    // Create the file
    QFile writer("eoltest");
    QVERIFY2(writer.open(QFile::WriteOnly | QFile::Truncate), ("When creating a file: " + writer.errorString()).latin1());
    QCOMPARE(writer.write(input), qlonglong(input.size()));
    writer.close();

    // Read from it using QTextStream
    QVERIFY2(writer.open(QFile::ReadOnly), ("When reading a file: " + writer.errorString()).latin1());
    QTextStream stream(&writer);
    QStringList tmp;

    while (!stream.atEnd())
        tmp << stream.readLine();

    QCOMPARE(tmp, output);
}

// ------------------------------------------------------------------------------
void tst_QTextStream::qt3_writeDataToQStringReadAsLines_data()
{
    qt3_generateLineData(true);
}

// ------------------------------------------------------------------------------
void tst_QTextStream::qt3_writeDataToQStringReadAsLines()
{
    QFETCH(QByteArray, input);
    QFETCH(QStringList, output);

    QString s = input;
    QTextStream stream(&s, QIODevice::ReadOnly);

    QStringList tmp;
    while (!stream.atEnd())
        tmp << stream.readLine();

    QCOMPARE(tmp, output);
}

// ------------------------------------------------------------------------------
void tst_QTextStream::qt3_write_QByteArray( QTextStream *s )
{
    QFETCH( QByteArray, cs );
    *s << cs;
}
#endif

// ------------------------------------------------------------------------------
void tst_QTextStream::status_real_read_data()
{
    QTest::addColumn<QString>("input");
    QTest::addColumn<double>("expected_f");
    QTest::addColumn<QString>("expected_w");
    QTest::addColumn<QList<int> >("results");

    QTest::newRow("1.23 abc   ") << QString("1.23 abc   ") << 1.23 << QString("abc")
                              << (QList<int>()
                                  << (int)QTextStream::Ok
                                  << (int)QTextStream::ReadCorruptData
                                  << (int)QTextStream::Ok
                                  << (int)QTextStream::Ok
                                  << (int)QTextStream::ReadPastEnd);
}

void tst_QTextStream::status_real_read()
{
    QFETCH(QString, input);
    QFETCH(double, expected_f);
    QFETCH(QString, expected_w);
    QFETCH(QList<int>, results);

    QTextStream s(&input);
    double f = 0.0;
    QString w;
    s >> f;
    QCOMPARE((int)s.status(), results.at(0));
    QCOMPARE(f, expected_f);
    s >> f;
    QCOMPARE((int)s.status(), results.at(1));
    s.resetStatus();
    QCOMPARE((int)s.status(), results.at(2));
    s >> w;
    QCOMPARE((int)s.status(), results.at(3));
    QCOMPARE(w, expected_w);
    s >> f;
    QCOMPARE((int)s.status(), results.at(4));
}

void tst_QTextStream::status_integer_read()
{
#ifdef Q_OS_WINCE
    QString text = QLatin1String("123 abc   ");
    QTextStream s(&text);
#else
    QTextStream s("123 abc   ");
#endif
    int i;
    QString w;
    s >> i;
    QCOMPARE(s.status(), QTextStream::Ok);
    s >> i;
    QCOMPARE(s.status(), QTextStream::ReadCorruptData);
    s.resetStatus();
    QCOMPARE(s.status(), QTextStream::Ok);
    s >> w;
    QCOMPARE(s.status(), QTextStream::Ok);
    QCOMPARE(w, QString("abc"));
    s >> i;
    QCOMPARE(s.status(), QTextStream::ReadPastEnd);
}

void tst_QTextStream::status_word_read()
{
#ifdef Q_OS_WINCE
    QString text = QLatin1String("abc ");
    QTextStream s(&text);
#else
    QTextStream s("abc ");
#endif
    QString w;
    s >> w;
    QCOMPARE(s.status(), QTextStream::Ok);
    s >> w;
    QCOMPARE(s.status(), QTextStream::ReadPastEnd);
}

void tst_QTextStream::task180679_alignAccountingStyle()
{
    {
    QString result;
    QTextStream out(&result);
    out.setFieldAlignment(QTextStream::AlignAccountingStyle);
    out.setFieldWidth(4);
    out.setPadChar('0');
    out << -1;
    QCOMPARE(result, QLatin1String("-001"));
    }

    {
    QString result;
    QTextStream out(&result);
    out.setFieldAlignment(QTextStream::AlignAccountingStyle);
    out.setFieldWidth(4);
    out.setPadChar('0');
    out << "-1";
    QCOMPARE(result, QLatin1String("00-1"));
    }

    {
    QString result;
    QTextStream out(&result);
    out.setFieldAlignment(QTextStream::AlignAccountingStyle);
    out.setFieldWidth(6);
    out.setPadChar('0');
    out << -1.2;
    QCOMPARE(result, QLatin1String("-001.2"));
    }

    {
    QString result;
    QTextStream out(&result);
    out.setFieldAlignment(QTextStream::AlignAccountingStyle);
    out.setFieldWidth(6);
    out.setPadChar('0');
    out << "-1.2";
    QCOMPARE(result, QLatin1String("00-1.2"));
    }
}

void tst_QTextStream::task178772_setCodec()
{
    QByteArray ba("\xe5 v\xe6r\n\xc3\xa5 v\xc3\xa6r\n");
    QString res = QLatin1String("\xe5 v\xe6r");

    QTextStream stream(ba);
    stream.setCodec("ISO 8859-1");
    QCOMPARE(stream.readLine(),res);
    stream.setCodec("UTF-8");
    QCOMPARE(stream.readLine(),res);
}

void tst_QTextStream::double_write_with_flags_data()
{
    QTest::addColumn<double>("number");
    QTest::addColumn<QString>("output");
    QTest::addColumn<int>("numberFlags");
    QTest::addColumn<int>("realNumberNotation");

    QTest::newRow("-ForceSign") << -1.23 << QString("-1.23") << (int)QTextStream::ForceSign << 0;
    QTest::newRow("+ForceSign") << 1.23 << QString("+1.23") << (int)QTextStream::ForceSign << 0;
    QTest::newRow("inf") << qInf() << QString("inf") << 0 << 0;
    QTest::newRow("-inf") << -qInf() << QString("-inf") << 0 << 0;
    QTest::newRow("inf uppercase") << qInf() << QString("INF") << (int)QTextStream::UppercaseDigits << 0;
    QTest::newRow("-inf uppercase") << -qInf() << QString("-INF") << (int)QTextStream::UppercaseDigits << 0;
    QTest::newRow("nan") << qQNaN() << QString("nan") << 0 << 0;
    QTest::newRow("nan") << qQNaN() << QString("NAN") << (int)QTextStream::UppercaseDigits << 0;
    QTest::newRow("scientific") << 1.234567e+02 << QString("1.234567e+02") << 0  << (int)QTextStream::ScientificNotation;
    QTest::newRow("scientific2") << 1.234567e+02 << QString("1.234567e+02") << (int)QTextStream::UppercaseBase << (int)QTextStream::ScientificNotation;
    QTest::newRow("scientific uppercase") << 1.234567e+02 << QString("1.234567E+02") << (int)QTextStream::UppercaseDigits << (int)QTextStream::ScientificNotation;
}

void tst_QTextStream::double_write_with_flags()
{
    QFETCH(double, number);
    QFETCH(QString, output);
    QFETCH(int, numberFlags);
    QFETCH(int, realNumberNotation);

    QString buf;
    QTextStream stream(&buf);
    if (numberFlags)
        stream.setNumberFlags(QTextStream::NumberFlag(numberFlags));
    if (realNumberNotation)
        stream.setRealNumberNotation(QTextStream::RealNumberNotation(realNumberNotation));
    stream << number;
    QCOMPARE(buf, output);
}

void tst_QTextStream::double_write_with_precision_data()
{
    QTest::addColumn<int>("precision");
    QTest::addColumn<double>("value");
    QTest::addColumn<QString>("result");

    QTest::ignoreMessage(QtWarningMsg, "QTextStream::setRealNumberPrecision: Invalid precision (-1)");
    QTest::newRow("-1") << -1 << 3.14159 << QString("3.14159");
    QTest::newRow("0") << 0 << 3.14159 << QString("3");
    QTest::newRow("1") << 1 << 3.14159 << QString("3");
    QTest::newRow("2") << 2 << 3.14159 << QString("3.1");
    QTest::newRow("3") << 3 << 3.14159 << QString("3.14");
    QTest::newRow("5") << 5 << 3.14159 << QString("3.1416");
    QTest::newRow("6") << 6 << 3.14159 << QString("3.14159");
    QTest::newRow("7") << 7 << 3.14159 << QString("3.14159");
    QTest::newRow("10") << 10 << 3.14159 << QString("3.14159");
}

void tst_QTextStream::double_write_with_precision()
{
    QFETCH(int, precision);
    QFETCH(double, value);
    QFETCH(QString, result);

    QString buf;
    QTextStream stream(&buf);
    stream.setRealNumberPrecision(precision);
    stream << value;
    QCOMPARE(buf, result);
}

void tst_QTextStream::int_read_with_locale_data()
{
    QTest::addColumn<QString>("locale");
    QTest::addColumn<QString>("input");
    QTest::addColumn<int>("output");

    QTest::newRow("C -123") << QString("C") << QString("-123") << -123;
    QTest::newRow("C +123") << QString("C") << QString("+123") << 123;
    QTest::newRow("C 12345") << QString("C") << QString("12345") << 12345;
    QTest::newRow("C 12,345") << QString("C") << QString("12,345") << 12;
    QTest::newRow("C 12.345") << QString("C") << QString("12.345") << 12;

    QTest::newRow("de_DE -123") << QString("de_DE") << QString("-123") << -123;
    QTest::newRow("de_DE +123") << QString("de_DE") << QString("+123") << 123;
    QTest::newRow("de_DE 12345") << QString("de_DE") << QString("12345") << 12345;
    QTest::newRow("de_DE 12.345") << QString("de_DE") << QString("12.345") << 12345;
    QTest::newRow("de_DE .12345") << QString("de_DE") << QString(".12345") << 0;
}

void tst_QTextStream::int_read_with_locale()
{
    QFETCH(QString, locale);
    QFETCH(QString, input);
    QFETCH(int, output);

    QTextStream stream(&input);
    stream.setLocale(locale);
    int result;
    stream >> result;
    QCOMPARE(result, output);
}

void tst_QTextStream::int_write_with_locale_data()
{
    QTest::addColumn<QString>("locale");
    QTest::addColumn<int>("numberFlags");
    QTest::addColumn<int>("input");
    QTest::addColumn<QString>("output");

    QTest::newRow("C -123") << QString("C") << 0 << -123 << QString("-123");
    QTest::newRow("C +123") << QString("C") << (int)QTextStream::ForceSign << 123 << QString("+123");
    QTest::newRow("C 12345") << QString("C") << 0 << 12345 << QString("12345");

    QTest::newRow("de_DE -123") << QString("de_DE") << 0 << -123 << QString("-123");
    QTest::newRow("de_DE +123") << QString("de_DE") << (int)QTextStream::ForceSign << 123 << QString("+123");
    QTest::newRow("de_DE 12345") << QString("de_DE") << 0 << 12345 << QString("12.345");
}

void tst_QTextStream::int_write_with_locale()
{
    QFETCH(QString, locale);
    QFETCH(int, numberFlags);
    QFETCH(int, input);
    QFETCH(QString, output);

    QString result;
    QTextStream stream(&result);
    stream.setLocale(locale);
    if (numberFlags)
        stream.setNumberFlags(QTextStream::NumberFlags(numberFlags));
    stream << input;
    QCOMPARE(result, output);
}

// ------------------------------------------------------------------------------

// like QTEST_APPLESS_MAIN, but initialising the locale on Unix
#if defined (Q_OS_UNIX) && !defined (Q_OS_SYMBIAN)
QT_BEGIN_NAMESPACE
extern bool qt_locale_initialized;
QT_END_NAMESPACE
#endif

int main(int argc, char *argv[])
{
#if defined (Q_OS_UNIX) && !defined (Q_OS_SYMBIAN)
    ::setlocale(LC_ALL, "");
    qt_locale_initialized = true;
#endif
    tst_QTextStream tc;
    return QTest::qExec(&tc, argc, argv);
}

#include "tst_qtextstream.moc"

