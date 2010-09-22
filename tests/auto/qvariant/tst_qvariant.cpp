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

#include <qvariant.h>
#include <qbitarray.h>
#include <qhostaddress.h>
#include <qdatetime.h>
#include <qmap.h>
#include <qiodevice.h>
#include <qurl.h>
#include <qlocale.h>
#ifdef QT3_SUPPORT
#  include <q3cstring.h>
#endif
#include <qkeysequence.h>
#include <qbitmap.h>
#include <qcursor.h>
#include <qimage.h>
#include <qicon.h>
#include <qmatrix.h>
#include <qmatrix4x4.h>
#include <qpen.h>
#include <qpolygon.h>
#include <qtransform.h>
#include <qvector2d.h>
#include <qvector3d.h>
#include <qvector4d.h>
#include <qquaternion.h>

#include <limits.h>

Q_DECLARE_METATYPE(qlonglong)
Q_DECLARE_METATYPE(qulonglong)
Q_DECLARE_METATYPE(QPointF)
Q_DECLARE_METATYPE(QRectF)
Q_DECLARE_METATYPE(QSize)
Q_DECLARE_METATYPE(QSizeF)
Q_DECLARE_METATYPE(QLine)
Q_DECLARE_METATYPE(QLineF)
Q_DECLARE_METATYPE(QPoint)
Q_DECLARE_METATYPE(QRect)
Q_DECLARE_METATYPE(QPixmap)
Q_DECLARE_METATYPE(QBrush)
Q_DECLARE_METATYPE(QFont)
Q_DECLARE_METATYPE(QColor)
Q_DECLARE_METATYPE(QKeySequence)

//TESTED_CLASS=
//TESTED_FILES=

class tst_QVariant : public QObject
{
    Q_OBJECT

public:
    tst_QVariant();

private slots:
    void constructor();
    void copy_constructor();
    void isNull();

    void canConvert_data();
    void canConvert();

    void toSize_data();
    void toSize();

    void toSizeF_data();
    void toSizeF();

    void toPoint_data();
    void toPoint();

    void toRect_data();
    void toRect();

    void toChar_data();
    void toChar();

    void toLine_data();
    void toLine();

    void toLineF_data();
    void toLineF();

    void toInt_data();
    void toInt();

    void toUInt_data();
    void toUInt();

    void toBool_data();
    void toBool();

    void toLongLong_data();
    void toLongLong();

    void toULongLong_data();
    void toULongLong();

    void asType_data();
    void asType();

    void toByteArray_data();
    void toByteArray();

    void toString_data();
    void toString();

    void toCString_data();
    void toCString();

    void toDate_data();
    void toDate();

    void toTime_data();
    void toTime();

    void toDateTime_data();
    void toDateTime();

    void toDouble_data();
    void toDouble();

    void toPointF_data();
    void toPointF();

    void toFont_data();
    void toFont();

    void toKeySequence_data();
    void toKeySequence();

    void toRectF_data();
    void toRectF();

    void toColor_data();
    void toColor();

    void toPixmap_data();
    void toPixmap();

    void toImage_data();
    void toImage();

    void toBrush_data();
    void toBrush();

    void qvariant_cast_QObject_data();
    void qvariant_cast_QObject();

    void toLocale();

    void toRegExp();

    void matrix();

    void transform();

    void matrix4x4();
    void vector2D();
    void vector3D();
    void vector4D();
    void quaternion();

    void url();

    void userType();
    void basicUserType();

    void variant_to();

    void writeToReadFromDataStream_data();
    void writeToReadFromDataStream();
    void writeToReadFromOldDataStream();
    void checkDataStream();

    void operator_eq_eq_data();
    void operator_eq_eq();

    void operator_eq_eq_rhs();

    void typeName_data();
    void typeName();
    void typeToName();

    void streamInvalidVariant();

    void podUserType();

    void nullAsType();

    void data_(); // data is virtual function in QtTestCase
    void constData();

    void saveLoadCustomTypes();

    void globalColor();

    void variantMap();
    void variantHash();

    void invalidAsByteArray();
    void convertToQUint8() const;
    void invalidQColor() const;
    void comparePointers() const;
    void voidStar() const;
    void dataStar() const;
    void canConvertQStringList() const;
    void canConvertQStringList_data() const;
    void canConvertMetaTypeToInt() const;
    void variantToDateTimeWithoutWarnings() const;
    void invalidDateTime() const;

    void loadUnknownUserType();
    void loadBrokenUserType();

    void task172061_invalidDate() const;
    void compareCustomTypes() const;
    void timeToDateTime() const;
    void copyingUserTypes() const;
    void convertBoolToByteArray() const;
    void convertBoolToByteArray_data() const;
    void convertByteArrayToBool() const;
    void convertByteArrayToBool_data() const;
    void toIntFromQString() const;
    void toIntFromDouble() const;
    void task256984_setValue();

    void numericalConvert();
    void moreCustomTypes();
    void variantInVariant();

    void colorInteger();
};

Q_DECLARE_METATYPE(QDate)
Q_DECLARE_METATYPE(QTime)
Q_DECLARE_METATYPE(QDateTime)
Q_DECLARE_METATYPE(QVariant)
#ifdef QT3_SUPPORT
Q_DECLARE_METATYPE(Q3CString)
#endif

const qlonglong intMax1 = (qlonglong)INT_MAX + 1;
const qulonglong uintMax1 = (qulonglong)UINT_MAX + 1;

tst_QVariant::tst_QVariant()
{
}

void tst_QVariant::constructor()
{
    QVariant variant;
    QVERIFY( !variant.isValid() );
    QVERIFY( variant.isNull() );

    QVariant var2(variant);
    QVERIFY( !var2.isValid() );
    QVERIFY( variant.isNull() );

    QVariant varll(intMax1);
    QVariant varll2(varll);
    QCOMPARE(varll2, varll);

    QVariant var3(QVariant::String);
    QCOMPARE(var3.typeName(), "QString");
    QVERIFY(var3.isNull());
    QVERIFY(var3.isValid());

    QVariant var4(QVariant::Invalid);
    QCOMPARE(var4.type(), QVariant::Invalid);
    QVERIFY(var4.isNull());
    QVERIFY(!var4.isValid());

    QVariant var5(QLatin1String("hallo"));
    QCOMPARE(var5.type(), QVariant::String);
    QCOMPARE(var5.typeName(), "QString");

    QVariant var6(qlonglong(0));
    QCOMPARE(var6.type(), QVariant::LongLong);
    QCOMPARE(var6.typeName(), "qlonglong");

    QVariant var7 = 5;
    QVERIFY(var7.isValid());
    QVERIFY(!var7.isNull());
    QVariant var8;
    var8.setValue<int>(5);
    QVERIFY(var8.isValid());
    QVERIFY(!var8.isNull());
}

void tst_QVariant::copy_constructor()
{
    QVariant var7(QVariant::Int);
    QVariant var8(var7);
    QCOMPARE(var8.type(), QVariant::Int);
    QVERIFY(var8.isNull());
}

void tst_QVariant::isNull()
{
    QVariant var;
    QVERIFY( var.isNull() );

    QVariant var2( QString::null );
    QVERIFY( var2.isNull() );

    QVariant var3( QString( "blah" ) );
    QVERIFY( !var3.isNull() );

    QVariant var4( 0 );
    QVERIFY( !var4.isNull() );

    QVariant var5 = QString();
    QVERIFY( var5.isNull() );

    QVariant var6( QString( "blah" ) );
    QVERIFY( !var6.isNull() );
    var6 = QVariant();
    QVERIFY( var6.isNull() );
    var6.convert( QVariant::String );
    QVERIFY( var6.isNull() );
    QVariant varLL( (qlonglong)0 );
    QVERIFY( !varLL.isNull() );
    QVariant var7(QString::null);
#ifdef QT3_SUPPORT
    QCOMPARE(var7.asInt(), 0);
#endif
    QVERIFY(var7.isNull());
}

void tst_QVariant::canConvert_data()
{
    QTest::addColumn<QVariant>("val");
    QTest::addColumn<bool>("BitArrayCast");
    QTest::addColumn<bool>("BitmapCast");
    QTest::addColumn<bool>("BoolCast");
    QTest::addColumn<bool>("BrushCast");
    QTest::addColumn<bool>("ByteArrayCast");
    QTest::addColumn<bool>("CStringCast");
    QTest::addColumn<bool>("ColorCast");
    QTest::addColumn<bool>("ColorGroupCast");
    QTest::addColumn<bool>("CursorCast");
    QTest::addColumn<bool>("DateCast");
    QTest::addColumn<bool>("DateTimeCast");
    QTest::addColumn<bool>("DoubleCast");
    QTest::addColumn<bool>("FontCast");
    QTest::addColumn<bool>("IconSetCast");
    QTest::addColumn<bool>("ImageCast");
    QTest::addColumn<bool>("IntCast");
    QTest::addColumn<bool>("InvalidCast");
    QTest::addColumn<bool>("KeySequenceCast");
    QTest::addColumn<bool>("ListCast");
    QTest::addColumn<bool>("LongLongCast");
    QTest::addColumn<bool>("MapCast");
    QTest::addColumn<bool>("PaletteCast");
    QTest::addColumn<bool>("PenCast");
    QTest::addColumn<bool>("PixmapCast");
    QTest::addColumn<bool>("PointArrayCast");
    QTest::addColumn<bool>("PointCast");
    QTest::addColumn<bool>("RectCast");
    QTest::addColumn<bool>("RegionCast");
    QTest::addColumn<bool>("SizeCast");
    QTest::addColumn<bool>("SizePolicyCast");
    QTest::addColumn<bool>("StringCast");
    QTest::addColumn<bool>("StringListCast");
    QTest::addColumn<bool>("TimeCast");
    QTest::addColumn<bool>("UIntCast");
    QTest::addColumn<bool>("ULongLongCast");


#ifdef Y
#undef Y
#endif
#ifdef N
#undef N
#endif
#define Y true
#define N false
    //            bita bitm bool brsh byta cstr col  colg curs date dt   dbl  font icon img  int  inv  kseq list ll   map  pal  pen  pix  pnta pnt  rect reg  size sp   str  strl time uint ull


    QVariant var(QBitArray(0));
    QTest::newRow("BitArray")
        << var << Y << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N;
    var = qVariantFromValue(QBitmap());
    QTest::newRow("Bitmap")
        << var << N << Y << N << N << N << N << N << N << N << N << N << N << N << N << Y << N << N << N << N << N << N << N << N << Y << N << N << N << N << N << N << N << N << N << N << N;
#ifdef QT3_SUPPORT
    var = QVariant(true, 0);
    QTest::newRow("Bool")
        << var << N << N << Y << N << Y << Y << N << N << N << N << N << Y << N << N << N << Y << N << N << N << Y << N << N << N << N << N << N << N << N << N << N << Y << N << N << Y << Y;
#endif
    var = qVariantFromValue(QBrush());
    QTest::newRow("Brush")
        << var << N << N << N << Y << N << N << Y << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << Y << N << N << N << N << N << N << N << N << N << N << N;
    var = QVariant(QByteArray());
    QTest::newRow("ByteArray")
        << var << N << N << Y << N << Y << Y << Y << N << N << N << N << Y << N << N << N << Y << N << N << N << Y << N << N << N << N << N << N << N << N << N << N << Y << N << N << Y << Y;
#ifdef QT3_SUPPORT
    var = QVariant(Q3CString("cstring"));
    QTest::newRow("CString")
        << var << N << N << Y << N << Y << Y << Y << N << N << N << N << Y << N << N << N << Y << N << N << N << Y << N << N << N << N << N << N << N << N << N << N << Y << N << N << Y << Y;
#endif
    var = qVariantFromValue(QColor());
    QTest::newRow("Color")
        << var << N << N << N << Y << Y << Y << Y << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << Y << N << N << N << N;
#ifdef QT3_SUPPORT
    var = qVariantFromValue(QColorGroup());
    QTest::newRow("ColorGroup")
        << var << N << N << N << N << N << N << N << Y << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N;
#endif
#ifndef QT_NO_CURSOR
    var = qVariantFromValue(QCursor());
    QTest::newRow("Cursor")
        << var << N << N << N << N << N << N << N << N << Y << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N;
#endif
    var = QVariant(QDate());
    QTest::newRow("Date")
        << var << N << N << N << N << N << N << N << N << N << Y << Y << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << Y << N << N << N << N;
    var = QVariant(QDateTime());
    QTest::newRow("DateTime")
        << var << N << N << N << N << N << N << N << N << N << Y << Y << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << Y << N << Y << N << N;
    var = QVariant((double)0.1);
    QTest::newRow("Double")
        << var << N << N << Y << N << Y << Y << N << N << N << N << N << Y << N << N << N << Y << N << N << N << Y << N << N << N << N << N << N << N << N << N << N << Y << N << N << Y << Y;
    var = QVariant(0.1f);
    QTest::newRow("Float")
        << var << N << N << Y << N << Y << Y << N << N << N << N << N << Y << N << N << N << Y << N << N << N << Y << N << N << N << N << N << N << N << N << N << N << Y << N << N << Y << Y;
    var = qVariantFromValue(QFont());
    QTest::newRow("Font")
        << var << N << N << N << N << N << N << N << N << N << N << N << N << Y << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << Y << N << N << N << N;
    var = qVariantFromValue(QIcon());
    QTest::newRow("Icon")
        << var << N << N << N << N << N << N << N << N << N << N << N << N << N << Y << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N;
    var = qVariantFromValue(QImage());
    QTest::newRow("Image")
        << var << N << Y << N << N << N << N << N << N << N << N << N << N << N << N << Y << N << N << N << N << N << N << N << N << Y << N << N << N << N << N << N << N << N << N << N << N;
    var = QVariant((int)1);
    QTest::newRow("Int")
        << var << N << N << Y << N << Y << Y << N << N << N << N << N << Y << N << N << N << Y << N << Y << N << Y << N << N << N << N << N << N << N << N << N << N << Y << N << N << Y << Y;
    var = QVariant();
    QTest::newRow("Invalid")
        << var << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << Y << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N;
    var = qVariantFromValue(QKeySequence());
    QTest::newRow("KeySequence")
        << var << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << Y << N << Y << N << N << N << N << N << N << N << N << N << N << N << N << Y << N << N << N << N;
    var = QVariant(QList<QVariant>());
    QTest::newRow("List")
        << var << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << Y << N << N << N << N << N << N << N << N << N << N << N << N << Y << N << N << N;
    var = QVariant((qlonglong)1);
    QTest::newRow("LongLong")
        << var << N << N << Y << N << Y << Y << N << N << N << N << N << Y << N << N << N << Y << N << N << N << Y << N << N << N << N << N << N << N << N << N << N << Y << N << N << Y << Y;
    var = QVariant(QMap<QString,QVariant>());
    QTest::newRow("Map")
        << var << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << Y << N << N << N << N << N << N << N << N << N << N << N << N << N << N;
    var = qVariantFromValue(QPalette());
    QTest::newRow("Palette")
        << var << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << Y << N << N << N << N << N << N << N << N << N << N << N << N << N;
    var = qVariantFromValue(QPen());
    QTest::newRow("Pen")
        << var << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << Y << N << N << N << N << N << N << N << N << N << N << N << N;
    var = qVariantFromValue(QPixmap());
    QTest::newRow("Pixmap")
        << var << N << Y << N << Y << N << N << N << N << N << N << N << N << N << N << Y << N << N << N << N << N << N << N << N << Y << N << N << N << N << N << N << N << N << N << N << N;
    var = qVariantFromValue(QPolygon());
    QTest::newRow("PointArray")
        << var << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << Y << N << N << N << N << N << N << N << N << N << N;
    var = QVariant(QPoint());
    QTest::newRow("Point")
        << var << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << Y << N << N << N << N << N << N << N << N << N;
    var = QVariant(QRect());
    QTest::newRow("Rect")
        << var << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << Y << N << N << N << N << N << N << N << N;
    var = qVariantFromValue(QRegion());
    QTest::newRow("Region")
        << var << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << Y << N << N << N << N << N << N << N;
    var = QVariant(QSize());
    QTest::newRow("Size")
        << var << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << Y << N << N << N << N << N << N;
    var = qVariantFromValue(QSizePolicy());
    QTest::newRow("SizePolicy")
        << var << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << Y << N << N << N << N << N;
    var = QVariant(QString());
    QTest::newRow("String")
        << var << N << N << Y << N << Y << Y << Y << N << N << Y << Y << Y << Y << N << N << Y << N << Y << N << Y << N << N << N << N << N << N << N << N << N << N << Y << Y << Y << Y << Y;
   var = QVariant(QStringList("entry"));
    QTest::newRow("StringList")
        << var << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << Y << N << N << N << N << N << N << N << N << N << N << N << Y << Y << N << N << N;
    var = QVariant(QTime());
    QTest::newRow("Time")
        << var << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << N << Y << N << Y << N << N;
    var = QVariant((uint)1);
    QTest::newRow("UInt")
        << var << N << N << Y << N << Y << Y << N << N << N << N << N << Y << N << N << N << Y << N << N << N << Y << N << N << N << N << N << N << N << N << N << N << Y << N << N << Y << Y;
    var = QVariant((qulonglong)1);
    QTest::newRow("ULongLong")
        << var << N << N << Y << N << Y << Y << N << N << N << N << N << Y << N << N << N << Y << N << N << N << Y << N << N << N << N << N << N << N << N << N << N << Y << N << N << Y << Y;

#undef N
#undef Y
}

void tst_QVariant::canConvert()
{
    QFETCH(QVariant, val);
    QFETCH(bool, BitArrayCast);
    QFETCH(bool, BitmapCast);
    QFETCH(bool, BoolCast);
    QFETCH(bool, BrushCast);
    QFETCH(bool, ByteArrayCast);
    QFETCH(bool, CStringCast);
    QFETCH(bool, ColorCast);
    QFETCH(bool, ColorGroupCast);
    QFETCH(bool, CursorCast);
    QFETCH(bool, DateCast);
    QFETCH(bool, DateTimeCast);
    QFETCH(bool, DoubleCast);
    QFETCH(bool, FontCast);
    QFETCH(bool, IconSetCast);
    QFETCH(bool, ImageCast);
    QFETCH(bool, IntCast);
    QFETCH(bool, InvalidCast);
    QFETCH(bool, KeySequenceCast);
    QFETCH(bool, ListCast);
    QFETCH(bool, LongLongCast);
    QFETCH(bool, MapCast);
    QFETCH(bool, PaletteCast);
    QFETCH(bool, PenCast);
    QFETCH(bool, PixmapCast);
    QFETCH(bool, PointArrayCast);
    QFETCH(bool, PointCast);
    QFETCH(bool, RectCast);
    QFETCH(bool, RegionCast);
    QFETCH(bool, SizeCast);
    QFETCH(bool, SizePolicyCast);
    QFETCH(bool, StringCast);
    QFETCH(bool, StringListCast);
    QFETCH(bool, TimeCast);
    QFETCH(bool, UIntCast);
    QFETCH(bool, ULongLongCast);

    QCOMPARE(val.canConvert(QVariant::BitArray), BitArrayCast);
    QCOMPARE(val.canConvert(QVariant::Bitmap), BitmapCast);
    QCOMPARE(val.canConvert(QVariant::Bool), BoolCast);
    QCOMPARE(val.canConvert(QVariant::Brush), BrushCast);
    QCOMPARE(val.canConvert(QVariant::ByteArray), ByteArrayCast);
#ifdef QT3_SUPPORT
    QCOMPARE(val.canConvert(QVariant::CString), CStringCast);
#else
    Q_UNUSED(CStringCast);
#endif
    QCOMPARE(val.canConvert(QVariant::Color), ColorCast);
#ifdef QT3_SUPPORT
    QCOMPARE(val.canConvert(QVariant::ColorGroup), ColorGroupCast);
#else
    Q_UNUSED(ColorGroupCast);
#endif
    QCOMPARE(val.canConvert(QVariant::Cursor), CursorCast);
    QCOMPARE(val.canConvert(QVariant::Date), DateCast);
    QCOMPARE(val.canConvert(QVariant::DateTime), DateTimeCast);
    QCOMPARE(val.canConvert(QVariant::Double), DoubleCast);
    QCOMPARE(val.canConvert(QVariant::Type(QMetaType::Float)), DoubleCast);
    QCOMPARE(val.canConvert(QVariant::Font), FontCast);
#ifdef QT3_SUPPORT
    QCOMPARE(val.canConvert(QVariant::IconSet), IconSetCast);
#else
    Q_UNUSED(IconSetCast);
#endif
    QCOMPARE(val.canConvert(QVariant::Image), ImageCast);
    QCOMPARE(val.canConvert(QVariant::Int), IntCast);
    QCOMPARE(val.canConvert(QVariant::Invalid), InvalidCast);
    QCOMPARE(val.canConvert(QVariant::KeySequence), KeySequenceCast);
    QCOMPARE(val.canConvert(QVariant::List), ListCast);
    QCOMPARE(val.canConvert(QVariant::LongLong), LongLongCast);
    QCOMPARE(val.canConvert(QVariant::Map), MapCast);
    QCOMPARE(val.canConvert(QVariant::Palette), PaletteCast);
    QCOMPARE(val.canConvert(QVariant::Pen), PenCast);
    QCOMPARE(val.canConvert(QVariant::Pixmap), PixmapCast);
#ifdef QT3_SUPPORT
    QCOMPARE(val.canConvert(QVariant::PointArray), PointArrayCast);
#else
    Q_UNUSED(PointArrayCast);
#endif
    QCOMPARE(val.canConvert(QVariant::Point), PointCast);
    QCOMPARE(val.canConvert(QVariant::Rect), RectCast);
    QCOMPARE(val.canConvert(QVariant::Region), RegionCast);
    QCOMPARE(val.canConvert(QVariant::Size), SizeCast);
    QCOMPARE(val.canConvert(QVariant::SizePolicy), SizePolicyCast);
    QCOMPARE(val.canConvert(QVariant::String), StringCast);
    QCOMPARE(val.canConvert(QVariant::StringList), StringListCast);
    QCOMPARE(val.canConvert(QVariant::Time), TimeCast);
    QCOMPARE(val.canConvert(QVariant::UInt), UIntCast);
    QCOMPARE(val.canConvert(QVariant::ULongLong), ULongLongCast);
}

void tst_QVariant::toInt_data()
{
    QTest::addColumn<QVariant>("value");
    QTest::addColumn<int>("result");
    QTest::addColumn<bool>("valueOK");

    QTest::newRow( "invalid" ) << QVariant()  << 0 << false;
    QTest::newRow( "int" ) << QVariant( 123 ) << 123 << true;
    QTest::newRow( "double" ) << QVariant( 3.1415927 ) << 3 << true;
    QTest::newRow( "float" ) << QVariant( 3.1415927f ) << 3 << true;
    QTest::newRow( "uint" ) << QVariant( 123u ) << 123 << true;
#ifdef QT3_SUPPORT
    QTest::newRow( "bool" ) << QVariant( true, 42 ) << 1 << true;
#endif
    QTest::newRow( "int-string" ) << QVariant( QString("123") ) << 123 << true;
    QTest::newRow( "string" ) << QVariant( QString("Unicode String") ) << 0 << false;
    QTest::newRow( "longlong0" ) << QVariant( (qlonglong)34 ) << 34 << true;
    QTest::newRow( "longlong1" ) << QVariant( intMax1 ) << (int)INT_MIN << true;
    QTest::newRow( "ulonglong0" ) << QVariant( (qulonglong)34 ) << 34 << true;
    QTest::newRow( "ulonglong1" ) << QVariant( uintMax1 ) << 0 << true;
    QTest::newRow( "signedint" ) << QVariant( -123 ) << -123 << true;
    QTest::newRow( "signeddouble" ) << QVariant( -3.1415927 ) << -3 << true;
    QTest::newRow( "signedfloat" ) << QVariant( -3.1415927f ) << -3 << true;
    QTest::newRow( "signedint-string" ) << QVariant( QString("-123") ) << -123 << true;
    QTest::newRow( "signedlonglong0" ) << QVariant( (qlonglong)-34 ) << -34 << true;
    QTest::newRow( "QChar" ) << QVariant(QChar('a')) << int('a') << true;
    QTest::newRow( "keysequence" ) << qVariantFromValue( QKeySequence( Qt::Key_A ) ) << 65 << true;
    QByteArray bytearray(4, ' ');
    bytearray[0] = 'T';
    bytearray[1] = 'e';
    bytearray[2] = 's';
    bytearray[3] = 't';
//    bytearray[4] = '\0';
    QTest::newRow( "QByteArray1" ) << QVariant( bytearray ) << 0 << false;
    bytearray[0] = '4';
    bytearray[1] = '5';
    bytearray[2] = '0';
    bytearray[3] = '0';
    QTest::newRow( "QByteArray2" ) << QVariant( bytearray ) << 4500 << true;
}

void tst_QVariant::toInt()
{
    QFETCH( QVariant, value );
    QFETCH( int, result );
    QFETCH( bool, valueOK );
//    QEXPECT_FAIL("QByteArray", "Expected to not yet be able to convert QByteArray to int", Abort);
    QVERIFY( value.isValid() == value.canConvert( QVariant::Int ) );
    bool ok;
    int i = value.toInt( &ok );
    QCOMPARE( i, result );
    QVERIFY( ok == valueOK );
}

void tst_QVariant::toUInt_data()
{
    QTest::addColumn<QVariant>("value");
    QTest::addColumn<uint>("result");
    QTest::addColumn<bool>("valueOK");

    QTest::newRow( "int" ) << QVariant( 123 ) << (uint)123 << true;
    QTest::newRow( "double" ) << QVariant( 3.1415927 ) << (uint)3 << true;
    QTest::newRow( "float" ) << QVariant( 3.1415927f ) << (uint)3 << true;
    QTest::newRow( "uint" ) << QVariant( 123u ) << (uint)123 << true;
#ifdef QT3_SUPPORT
    QTest::newRow( "bool" ) << QVariant( true, 42 ) << (uint)1 << true;
#endif
    QTest::newRow( "int-string" ) << QVariant( QString("123") ) << (uint)123 << true;
    QTest::newRow( "string" ) << QVariant( QString("Unicode String") ) << (uint)0 << false;
    QTest::newRow( "string2" ) << QVariant( QString("4") ) << (uint)4 << true;
    QTest::newRow( "longlong0" ) << QVariant( (qlonglong)34 ) << (uint)34 << true;
    QTest::newRow( "longlong1" ) << QVariant( intMax1 ) << (uint)INT_MIN << true;
    QTest::newRow( "ulonglong0" ) << QVariant( (qulonglong)34 ) << (uint)34 << true;
    QTest::newRow( "ulonglong1" ) << QVariant( uintMax1 ) << (uint)0 << true;
    QTest::newRow( "negativeint" ) << QVariant( -123 ) << (uint)-123 << true;
    QTest::newRow( "negativedouble" ) << QVariant( -3.1415927 ) << (uint)-3 << true;
    QTest::newRow( "negativefloat" ) << QVariant( -3.1415927f ) << (uint)-3 << true;
    QTest::newRow( "negativeint-string" ) << QVariant( QString("-123") ) << (uint)0 << false;
    QTest::newRow( "negativelonglong0" ) << QVariant( (qlonglong)-34 ) << (uint)-34 << true;
    QTest::newRow( "QChar" ) << QVariant(QChar('a')) << uint('a') << true;
    QByteArray bytearray(4, ' ');
    bytearray[0] = '4';
    bytearray[1] = '3';
    bytearray[2] = '2';
    bytearray[3] = '1';
    QTest::newRow( "QByteArray" ) << QVariant( bytearray ) << (uint)4321 << true;
}

void tst_QVariant::toUInt()
{
    QFETCH( QVariant, value );
    QFETCH( uint, result );
    QFETCH( bool, valueOK );
    QVERIFY( value.isValid() );
    QVERIFY( value.canConvert( QVariant::UInt ) );

    bool ok;
    uint i = value.toUInt( &ok );
    QVERIFY( ok == valueOK );
    QCOMPARE( i, result );
}


void tst_QVariant::toSize_data()
{
    QTest::addColumn<QVariant>("value");
    QTest::addColumn<QSize>("result");
    QTest::newRow( "qsizef4" ) << QVariant( QSizeF(4, 2) ) << QSize(4, 2);
    QTest::newRow( "qsizef1" ) << QVariant( QSizeF(0, 0) ) << QSize(0, 0);
    QTest::newRow( "qsizef2" ) << QVariant( QSizeF(-5, -1) ) << QSize(-5, -1);
    QTest::newRow( "qsizef3" ) << QVariant( QSizeF() ) << QSize();
}

void tst_QVariant::toSize()
{
    QFETCH( QVariant, value );
    QFETCH( QSize, result );
    QVERIFY( value.isValid() );
    QVERIFY( value.canConvert( QVariant::Size ) );

    QSize i = value.toSize();
    QCOMPARE( i, result );
}

void tst_QVariant::toSizeF_data()
{
    QTest::addColumn<QVariant>("value");
    QTest::addColumn<QSizeF>("result");
    QTest::newRow( "qsize1" ) << QVariant( QSize(0, 0) ) << QSizeF(0, 0);
    QTest::newRow( "qsize2" ) << QVariant( QSize(-5, -1) ) << QSizeF(-5, -1);
     QTest::newRow( "qsize3" ) << QVariant( QSize() ) << QSizeF();
    QTest::newRow( "qsize4" ) << QVariant(QSize(4,2)) << QSizeF(4,2);
}

void tst_QVariant::toSizeF()
{
    QFETCH( QVariant, value );
    QFETCH( QSizeF, result );
    QVERIFY( value.isValid() );
    QVERIFY( value.canConvert( QVariant::SizeF ) );

    QSizeF i = value.toSizeF();
    QCOMPARE( i, result );
}

void tst_QVariant::toLine_data()
{
    QTest::addColumn<QVariant>("value");
    QTest::addColumn<QLine>("result");
    QTest::newRow( "linef1" ) << QVariant( QLineF(1, 2, 3, 4) ) << QLine(1, 2, 3, 4);
    QTest::newRow( "linef2" ) << QVariant( QLineF(-1, -2, -3, -4) ) << QLine(-1, -2, -3, -4);
    QTest::newRow( "linef3" ) << QVariant( QLineF(0, 0, 0, 0) ) << QLine(0, 0, 0, 0);
    QTest::newRow( "linef4" ) << QVariant( QLineF() ) << QLine();
}

void tst_QVariant::toLine()
{
    QFETCH( QVariant, value );
    QFETCH( QLine, result );
    QVERIFY( value.isValid() );
    QVERIFY( value.canConvert( QVariant::Line ) );

    QLine i = value.toLine();
    QCOMPARE( i, result );
}

void tst_QVariant::toLineF_data()
{
    QTest::addColumn<QVariant>("value");
    QTest::addColumn<QLineF>("result");
    QTest::newRow( "line1" ) << QVariant( QLine(-1, -2, -3, -4) ) << QLineF(-1, -2, -3, -4);
    QTest::newRow( "line2" ) << QVariant( QLine(0, 0, 0, 0) ) << QLineF(0, 0, 0, 0);
    QTest::newRow( "line3" ) << QVariant( QLine() ) << QLineF();
    QTest::newRow( "line4" ) << QVariant( QLine(1, 2, 3, 4) ) << QLineF(1, 2, 3, 4);
}

void tst_QVariant::toLineF()
{
    QFETCH( QVariant, value );
    QFETCH( QLineF, result );
    QVERIFY( value.isValid() );
    QVERIFY( value.canConvert( QVariant::LineF ) );

    QLineF i = value.toLineF();
    QCOMPARE( i, result );
}

void tst_QVariant::toPoint_data()
{
    QTest::addColumn<QVariant>("value");
    QTest::addColumn<QPoint>("result");
    QTest::newRow( "pointf1" ) << QVariant( QPointF(4, 2) ) << QPoint(4, 2);
    QTest::newRow( "pointf2" ) << QVariant( QPointF(0, 0) ) << QPoint(0, 0);
    QTest::newRow( "pointf3" ) << QVariant( QPointF(-4, -2) ) << QPoint(-4, -2);
    QTest::newRow( "pointf4" ) << QVariant( QPointF() ) << QPoint();
    QTest::newRow( "pointf5" ) << QVariant( QPointF(-4.2f, -2.3f) ) << QPoint(-4, -2);
}

void tst_QVariant::toPoint()
{
    QFETCH( QVariant, value );
    QFETCH( QPoint, result );
    QVERIFY( value.isValid() );
    QVERIFY( value.canConvert( QVariant::Point ) );
    QPoint i = value.toPoint();
    QCOMPARE( i, result );
}

void tst_QVariant::toRect_data()
{
    QTest::addColumn<QVariant>("value");
    QTest::addColumn<QRect>("result");
    QTest::newRow( "rectf1" ) << QVariant(QRectF(1, 2, 3, 4)) << QRect(1, 2, 3, 4);
    QTest::newRow( "rectf2" ) << QVariant(QRectF(0, 0, 0, 0)) << QRect(0, 0, 0, 0);
    QTest::newRow( "rectf3" ) << QVariant(QRectF(-1, -2, -3, -4)) << QRect(-1, -2, -3, -4);
    QTest::newRow( "rectf4" ) << QVariant(QRectF(-1.3f, 0, 3.9f, -4.0)) << QRect(-1, 0, 4, -4);
    QTest::newRow( "rectf5" ) << QVariant(QRectF()) << QRect();
}

void tst_QVariant::toRect()
{
    QFETCH( QVariant, value );
    QFETCH( QRect, result );
    QVERIFY( value.isValid() );
    QVERIFY( value.canConvert( QVariant::Rect ) );
    QRect i = value.toRect();
    QCOMPARE( i, result );
}

void tst_QVariant::toChar_data()
{
    QTest::addColumn<QVariant>("value");
    QTest::addColumn<QChar>("result");
    QTest::newRow( "longlong" ) << QVariant(qlonglong('6')) << QChar('6');
    QTest::newRow( "ulonglong" ) << QVariant(qulonglong('7')) << QChar('7');
}

void tst_QVariant::toChar()
{
    QFETCH( QVariant, value );
    QFETCH( QChar, result );
    QVERIFY( value.isValid() );
    QVERIFY( value.canConvert( QVariant::Char ) );

    QChar i = value.toChar();
    QCOMPARE( i, result );
}

void tst_QVariant::toBool_data()
{
    QTest::addColumn<QVariant>("value");
    QTest::addColumn<bool>("result");

    QTest::newRow( "int0" ) << QVariant( 0 ) << false;
    QTest::newRow( "int1" ) << QVariant( 123 ) << true;
    QTest::newRow( "uint0" ) << QVariant( 0u ) << false;
    QTest::newRow( "uint1" ) << QVariant( 123u ) << true;
    QTest::newRow( "double0" ) << QVariant( 0.0 ) << false;
    QTest::newRow( "float0" ) << QVariant( 0.0f ) << false;
    QTest::newRow( "double1" ) << QVariant( 3.1415927 ) << true;
    QTest::newRow( "float1" ) << QVariant( 3.1415927f ) << true;
#ifdef QT3_SUPPORT
    QTest::newRow( "bool0" ) << QVariant( false, 42 ) << false;
    QTest::newRow( "bool1" ) << QVariant( true, 42 ) << true;
#endif
    QTest::newRow( "string0" ) << QVariant( QString("3") ) << true;
    QTest::newRow( "string1" ) << QVariant( QString("true") ) << true;
    QTest::newRow( "string2" ) << QVariant( QString("0") ) << false;
    QTest::newRow( "string3" ) << QVariant( QString("fAlSe") ) << false;
    QTest::newRow( "longlong0" ) << QVariant( (qlonglong)0 ) << false;
    QTest::newRow( "longlong1" ) << QVariant( (qlonglong)1 ) << true;
    QTest::newRow( "ulonglong0" ) << QVariant( (qulonglong)0 ) << false;
    QTest::newRow( "ulonglong1" ) << QVariant( (qulonglong)1 ) << true;
    QTest::newRow( "QChar" ) << QVariant(QChar('a')) << true;
    QTest::newRow( "Null_QChar" ) << QVariant(QChar(0)) << false;
}

void tst_QVariant::toBool()
{
    QFETCH( QVariant, value );
    QFETCH( bool, result );
    QVERIFY( value.isValid() );
    QVERIFY( value.canConvert( QVariant::Bool ) );

    bool i = value.toBool();
    QCOMPARE( i, result );
}

void tst_QVariant::toPointF_data()
{
    QTest::addColumn<QVariant>("value");
    QTest::addColumn<QPointF>("result");

    QTest::newRow( "QPoint" ) << QVariant( QPointF( 19, 84) ) << QPointF( 19, 84 );
}

void tst_QVariant::toPointF()
{
    QFETCH( QVariant, value );
    QFETCH( QPointF, result );
    QVERIFY( value.isValid() );
    QVERIFY( value.canConvert( QVariant::PointF ) );
    QPointF d = value.toPointF();
    QCOMPARE( d, result );
}

void tst_QVariant::toRectF_data()
{
    QTest::addColumn<QVariant>("value");
    QTest::addColumn<QRectF>("result");

    QRect r( 1, 9, 8, 4 );
    QRectF rf( 1.0, 9.0, 8.0, 4.0 );
    QTest::newRow( "QRect" ) << QVariant( r ) << rf;
}

void tst_QVariant::toRectF()
{
    QFETCH( QVariant, value );
    QFETCH( QRectF, result );
    QVERIFY( value.isValid() );
    QVERIFY( value.canConvert( QVariant::RectF ) );
    QRectF d = value.toRectF();
    QCOMPARE( d, result );
}

void tst_QVariant::toColor_data()
{
    QTest::addColumn<QVariant>("value");
    QTest::addColumn<QColor>("result");

    QColor c("red");
    QTest::newRow( "string" ) << QVariant( QString( "red" ) ) << c;
    QTest::newRow( "solid brush" ) << QVariant( QBrush(c) ) << c;
}

void tst_QVariant::toColor()
{
    QFETCH( QVariant, value );
    QFETCH( QColor, result );
    QVERIFY( value.isValid() );
    QVERIFY( value.canConvert( QVariant::Color ) );
    QColor d = qVariantValue<QColor>(value);
    QCOMPARE( d, result );
}

void tst_QVariant::toPixmap_data()
{
    QTest::addColumn<QVariant>("value");
    QTest::addColumn<QPixmap>("result");

    QPixmap pm(30, 30);
    pm.fill(Qt::red);
    QTest::newRow( "image" ) << QVariant( pm ) << pm;

    QBitmap bm(30, 30);
    bm.fill(Qt::color1);
    QTest::newRow( "bitmap" ) << QVariant( bm ) << QPixmap(bm);
}

void tst_QVariant::toPixmap()
{
    QFETCH( QVariant, value );
    QFETCH( QPixmap, result );
    QVERIFY( value.isValid() );
    QVERIFY( value.canConvert( QVariant::Pixmap ) );
    QPixmap d = qVariantValue<QPixmap>(value);
    QCOMPARE( d, result );
}

void tst_QVariant::toImage_data()
{
    QTest::addColumn<QVariant>("value");
    QTest::addColumn<QImage>("result");

    QImage im(30, 30, QImage::Format_ARGB32);
    im.fill(0x7fff0000);
    QTest::newRow( "image" ) << QVariant( im ) << im;
}

void tst_QVariant::toImage()
{
    QFETCH( QVariant, value );
    QFETCH( QImage, result );
    QVERIFY( value.isValid() );
    QVERIFY( value.canConvert( QVariant::Image ) );
    QImage d = qVariantValue<QImage>(value);
    QCOMPARE( d, result );
}

void tst_QVariant::toBrush_data()
{
    QTest::addColumn<QVariant>("value");
    QTest::addColumn<QBrush>("result");

    QColor c(Qt::red);
    QTest::newRow( "color" ) << QVariant( c ) << QBrush(c);
    QPixmap pm(30, 30);
    pm.fill(c);
    QTest::newRow( "pixmap" ) << QVariant( pm ) << QBrush(pm);
}

void tst_QVariant::toBrush()
{
    QFETCH( QVariant, value );
    QFETCH( QBrush, result );
    QVERIFY( value.isValid() );
    QVERIFY( value.canConvert( QVariant::Brush ) );
    QBrush d = qVariantValue<QBrush>(value);
    QCOMPARE( d, result );
}

void tst_QVariant::toFont_data()
{
    QTest::addColumn<QVariant>("value");
    QTest::addColumn<QFont>("result");

    QFont f("times",12,-1,false);
    QTest::newRow( "string" ) << QVariant( QString( "times,12,-1,5,50,0,0,0,0,0" ) ) << f;
}

void tst_QVariant::toFont()
{
    QFETCH( QVariant, value );
    QFETCH( QFont, result );
    QVERIFY( value.isValid() );
    QVERIFY( value.canConvert( QVariant::Font ) );
    QFont d = qVariantValue<QFont>(value);
    QCOMPARE( d, result );
}

void tst_QVariant::toKeySequence_data()
{
    QTest::addColumn<QVariant>("value");
    QTest::addColumn<QKeySequence>("result");


    QTest::newRow( "int" ) << QVariant( 67108929 ) << QKeySequence( Qt::CTRL + Qt::Key_A );


    QTest::newRow( "qstring" )
        << QVariant( QString( "Ctrl+A" ) )
        << QKeySequence( Qt::CTRL + Qt::Key_A );
}

void tst_QVariant::toKeySequence()
{
    QFETCH( QVariant, value );
    QFETCH( QKeySequence, result );
    QVERIFY( value.isValid() );
    QVERIFY( value.canConvert( QVariant::KeySequence ) );
    QKeySequence d = qVariantValue<QKeySequence>(value);
    QCOMPARE( d, result );
}

void tst_QVariant::toDouble_data()
{
    QTest::addColumn<QVariant>("value");
    QTest::addColumn<double>("result");
    QTest::addColumn<bool>("valueOK");

    QByteArray bytearray(4, ' ');
    bytearray[0] = '3';
    bytearray[1] = '2';
    bytearray[2] = '.';
    bytearray[3] = '1';
    QTest::newRow( "bytearray" ) << QVariant( bytearray ) << 32.1 << true;
}

void tst_QVariant::toDouble()
{
    QFETCH( QVariant, value );
    QFETCH( double, result );
    QFETCH( bool, valueOK );
    QVERIFY( value.isValid() );
    QVERIFY( value.canConvert( QVariant::Double ) );
    bool ok;
    double d = value.toDouble( &ok );
    QCOMPARE( d, result );
    QVERIFY( ok == valueOK );
}

void tst_QVariant::toLongLong_data()
{
    QTest::addColumn<QVariant>("value");
    QTest::addColumn<qlonglong>("result");
    QTest::addColumn<bool>("valueOK");

    QTest::newRow( "int0" ) << QVariant( 123 ) << (qlonglong)123 << true;
    QTest::newRow( "double" ) << QVariant( 3.1415927 ) << (qlonglong)3 << true;
    QTest::newRow( "float" ) << QVariant( 3.1415927f ) << (qlonglong)3 << true;
    QTest::newRow( "uint" ) << QVariant( 123u ) << (qlonglong)123 << true;
#ifdef QT3_SUPPORT
    QTest::newRow( "bool" ) << QVariant( true, 42 ) << (qlonglong)1 << true;
#endif
    QTest::newRow( "int-string" ) << QVariant( QString("123") )
                               << (qlonglong)123 << true;
    QTest::newRow( "string" ) << QVariant( QString("Unicode fun") ) << (qlonglong)0
                           << false;
    QTest::newRow( "longlong" ) << QVariant( intMax1 ) << intMax1 << true;
    QTest::newRow( "ulonglong" ) << QVariant( uintMax1 ) << (qlonglong)uintMax1 << true;
    QTest::newRow( "QChar" ) << QVariant(QChar('a')) << qlonglong('a') << true;
    QByteArray bytearray(4, ' ');
    bytearray[0] = '3';
    bytearray[1] = '2';
    bytearray[2] = '0';
    bytearray[3] = '0';
    QTest::newRow( "QByteArray" ) << QVariant( bytearray ) << (qlonglong) 3200 << true;
}

void tst_QVariant::toLongLong()
{
    QFETCH( QVariant, value );
    QFETCH( qlonglong, result );
    QFETCH( bool, valueOK );
    QVERIFY( value.isValid() );
    QVERIFY( value.canConvert( QVariant::LongLong ) );
    bool ok;
    qlonglong ll = value.toLongLong( &ok );
    QCOMPARE( ll, result );
    QVERIFY( ok == valueOK );
}

void tst_QVariant::toULongLong_data()
{
    QTest::addColumn<QVariant>("value");
    QTest::addColumn<qulonglong>("result");
    QTest::addColumn<bool>("valueOK");

    QTest::newRow( "int0" ) << QVariant( 123 ) << (qulonglong)123 << true;
    QTest::newRow( "double" ) << QVariant( 3.1415927 ) << (qulonglong)3 << true;
    QTest::newRow( "float" ) << QVariant( 3.1415927f ) << (qulonglong)3 << true;
    QTest::newRow( "uint" ) << QVariant( 123u ) << (qulonglong)123 << true;
#ifdef QT3_SUPPORT
    QTest::newRow( "bool" ) << QVariant( true, 42 ) << (qulonglong)1 << true;
#endif
    QTest::newRow( "int-string" ) << QVariant( QString("123") )
                               << (qulonglong)123 << true;
    QTest::newRow( "string" ) << QVariant( QString("Unicode fun") ) << (qulonglong)0
                           << false;
    QTest::newRow( "ulonglong-string" ) << QVariant( QString("18446744073709551615") )
                                     << Q_UINT64_C(18446744073709551615)
                                     << true;
    QTest::newRow( "bytaa-string" ) << QVariant( QString("18446744073709551615") )
                                     << Q_UINT64_C(18446744073709551615)
                                     << true;
    QTest::newRow( "longlong" ) << QVariant( intMax1 ) << (qulonglong)intMax1 << true;
    QTest::newRow( "ulonglong" ) << QVariant( uintMax1 ) << uintMax1 << true;
    QTest::newRow( "QChar" ) << QVariant(QChar('a')) << qulonglong('a') << true;
    QByteArray bytearray(4, ' ');
    bytearray[0] = '3';
    bytearray[1] = '2';
    bytearray[2] = '0';
    bytearray[3] = '1';
    QTest::newRow( "QByteArray" ) << QVariant( bytearray ) << (qulonglong) 3201 << true;
}

void tst_QVariant::toULongLong()
{
    QFETCH( QVariant, value );
    QFETCH( qulonglong, result );
    QFETCH( bool, valueOK );
    QVERIFY( value.isValid() );
    QVERIFY( value.canConvert( QVariant::ULongLong ) );
    bool ok;
    qulonglong ll = value.toULongLong( &ok );
    QCOMPARE( ll, result );
    QVERIFY( ok == valueOK );
}

void tst_QVariant::asType_data()
{
    QTest::addColumn<QVariant>("value");

    QTest::newRow( "string" ) << QVariant( QString( "1.0" ) );
}

void tst_QVariant::asType()
{
#ifndef QT3_SUPPORT
    QSKIP("Qt compiled without Qt3Support", SkipAll);
#else
    QFETCH( QVariant, value );
    QVariant::Type type = value.type();

    QVariant copy = value;
    copy.asDouble();
    QCOMPARE( value.type(), type );

    copy = value;
    copy.asList();
    QCOMPARE( value.type(), type );

    copy = value;
    copy.asMap();
    QCOMPARE( value.type(), type );

    copy = value;
    QCOMPARE( value.type(), type );
#endif
}

void tst_QVariant::toByteArray_data()
{
    QTest::addColumn<QVariant>("value");
    QTest::addColumn<QByteArray>("result");

    QByteArray ba(5, ' ');
    ba[0] = 'T';
    ba[1] = 'e';
    ba[2] = 's';
    ba[3] = 't';
    ba[4] = '\0';

    QByteArray variantBa = ba;

    QTest::newRow( "qbytearray" ) << QVariant( variantBa ) << ba;
    QTest::newRow( "int" ) << QVariant( -123 ) << QByteArray( "-123" );
    QTest::newRow( "uint" ) << QVariant( (uint)123 ) << QByteArray( "123" );
    QTest::newRow( "double" ) << QVariant( 123.456 ) << QByteArray( "123.456" );
    QTest::newRow( "float" ) << QVariant( 123.456f ) << QByteArray( "123.456" );
    QTest::newRow( "longlong" ) << QVariant( (qlonglong)34 ) << QByteArray( "34" );
    QTest::newRow( "ulonglong" ) << QVariant( (qulonglong)34 ) << QByteArray( "34" );
}

void tst_QVariant::toByteArray()
{
    QFETCH( QVariant, value );
    QFETCH( QByteArray, result );
    QVERIFY( value.isValid() );
    QVERIFY( value.canConvert( QVariant::ByteArray ) );
    QByteArray ba = value.toByteArray();
    QCOMPARE( ba, result );
}

void tst_QVariant::toString_data()
{
    QTest::addColumn<QVariant>("value");
    QTest::addColumn<QString>("result");

    QTest::newRow( "qstring" ) << QVariant( QString( "Test" ) ) << QString( "Test" );
    QTest::newRow( "charstar" ) << QVariant(QLatin1String("Test")) << QString("Test");
    QTest::newRow( "qbytearray") << QVariant( QByteArray( "Test\0" ) ) << QString( "Test" );
    QTest::newRow( "int" ) << QVariant( -123 ) << QString( "-123" );
    QTest::newRow( "uint" ) << QVariant( (uint)123 ) << QString( "123" );
    QTest::newRow( "double" ) << QVariant( 123.456 ) << QString( "123.456" );
    QTest::newRow( "float" ) << QVariant( 123.456f ) << QString( "123.456" );
#ifdef QT3_SUPPORT
    QTest::newRow( "bool" ) << QVariant( true, 0 ) << QString( "true" );
#else
    QTest::newRow( "bool" ) << QVariant( true ) << QString( "true" );
#endif
    QTest::newRow( "qdate" ) << QVariant( QDate( 2002, 1, 1 ) ) << QString( "2002-01-01" );
    QTest::newRow( "qtime" ) << QVariant( QTime( 12, 34, 56 ) ) << QString( "12:34:56" );
    QTest::newRow( "qdatetime" ) << QVariant( QDateTime( QDate( 2002, 1, 1 ), QTime( 12, 34, 56 ) ) ) << QString( "2002-01-01T12:34:56" );
    QTest::newRow( "qkeysequence" ) << qVariantFromValue( QKeySequence( Qt::CTRL + Qt::Key_A ) )
#ifndef Q_WS_MAC
        << QString( "Ctrl+A" );
#else
        << QString(QChar(0x2318)) + "A";
#endif

    QFont font( "times", 12 );
    QTest::newRow( "qfont" ) << qVariantFromValue( font ) << QString("times,12,-1,5,50,0,0,0,0,0");
    QTest::newRow( "qcolor" ) << qVariantFromValue( QColor( 10, 10, 10 ) ) << QString( "#0a0a0a" );
    QTest::newRow( "llong" ) << QVariant( (qlonglong)Q_INT64_C(123456789012) ) <<
        QString( "123456789012" );
}

void tst_QVariant::toString()
{
    QFETCH( QVariant, value );
    QFETCH( QString, result );
    QVERIFY( value.isValid() );
    QVERIFY( value.canConvert( QVariant::String ) );
    QString str = value.toString();
    QCOMPARE( str, result );
}

void tst_QVariant::toCString_data()
{
#ifdef QT3_SUPPORT
    QTest::addColumn<QVariant>("value");
    QTest::addColumn<Q3CString>("result");

    QTest::newRow( "qstring" ) << QVariant( Q3CString( "Test" ) ) << Q3CString( "Test" );
    QTest::newRow( "qcstring") << QVariant( Q3CString( "Test\0" ) ) << Q3CString( "Test" );
#endif
}

void tst_QVariant::toCString()
{
#ifdef QT3_SUPPORT
    QFETCH( QVariant, value );
    QFETCH( Q3CString, result );
    QVERIFY( value.isValid() );

    Q3CString str = value.toCString();

    QCOMPARE( str, result );
#else
    QSKIP("Qt not build with Qt3Support", SkipAll);
#endif
}

void tst_QVariant::toDate_data()
{
    QTest::addColumn<QVariant>("value");
    QTest::addColumn<QDate>("result");

    QTest::newRow( "qdate" ) << QVariant( QDate( 2002, 10, 10 ) ) << QDate( 2002, 10, 10 );
    QTest::newRow( "qdatetime" ) << QVariant( QDateTime( QDate( 2002, 10, 10 ), QTime( 12, 34, 56 ) ) ) << QDate( 2002, 10, 10 );
    QTest::newRow( "qstring" ) << QVariant( QString( "2002-10-10" ) ) << QDate( 2002, 10, 10 );
}

void tst_QVariant::toDate()
{
    QFETCH( QVariant, value );
    QFETCH( QDate, result );
    QVERIFY( value.isValid() );
    QVERIFY( value.canConvert( QVariant::Date ) );
    QCOMPARE( value.toDate(), result );
}

void tst_QVariant::toTime_data()
{
    QTest::addColumn<QVariant>("value");
    QTest::addColumn<QTime>("result");

    QTest::newRow( "qtime" ) << QVariant( QTime( 12, 34, 56 ) ) << QTime( 12, 34, 56 );
    QTest::newRow( "qdatetime" ) << QVariant( QDateTime( QDate( 2002, 10, 10 ), QTime( 12, 34, 56 ) ) ) << QTime( 12, 34, 56 );
    QTest::newRow( "qstring" ) << QVariant( QString( "12:34:56" ) ) << QTime( 12, 34, 56 );
}

void tst_QVariant::toTime()
{
    QFETCH( QVariant, value );
    QFETCH( QTime, result );
    QVERIFY( value.isValid() );
    QVERIFY( value.canConvert( QVariant::Time ) );
    QCOMPARE( value.toTime(), result );
}

void tst_QVariant::toDateTime_data()
{
    QTest::addColumn<QVariant>("value");
    QTest::addColumn<QDateTime>("result");

    QTest::newRow( "qdatetime" ) << QVariant( QDateTime( QDate( 2002, 10, 10 ), QTime( 12, 34, 56 ) ) )
        << QDateTime( QDate( 2002, 10, 10 ), QTime( 12, 34, 56 ) );
    QTest::newRow( "qdate" ) << QVariant( QDate( 2002, 10, 10 ) ) << QDateTime( QDate( 2002, 10, 10 ), QTime( 0, 0, 0 ) );
    QTest::newRow( "qstring" ) << QVariant( QString( "2002-10-10T12:34:56" ) ) << QDateTime( QDate( 2002, 10, 10 ), QTime( 12, 34, 56 ) );
}

void tst_QVariant::toDateTime()
{
    QFETCH( QVariant, value );
    QFETCH( QDateTime, result );
    QVERIFY( value.isValid() );
    QVERIFY( value.canConvert( QVariant::DateTime ) );
    QCOMPARE( value.toDateTime(), result );
}

void tst_QVariant::toLocale()
{
    QVariant variant;
    QLocale loc = variant.toLocale();
    variant = QLocale::system();
    loc = variant.toLocale();
}

void tst_QVariant::toRegExp()
{
    QVariant variant;
    QRegExp rx = variant.toRegExp();
    variant = QRegExp("foo");
    rx = variant.toRegExp();
}

void tst_QVariant::matrix()
{
    QVariant variant;
    QMatrix matrix = qVariantValue<QMatrix>(variant);
    QVERIFY(matrix.isIdentity());
    qVariantSetValue(variant, QMatrix().rotate(90));
    QCOMPARE(QMatrix().rotate(90), qVariantValue<QMatrix>(variant));

    void *mmatrix = QMetaType::construct(QVariant::Matrix, 0);
    QVERIFY(mmatrix);
    QMetaType::destroy(QVariant::Matrix, mmatrix);
}

void tst_QVariant::matrix4x4()
{
    QVariant variant;
    QMatrix4x4 matrix = qVariantValue<QMatrix4x4>(variant);
    QVERIFY(matrix.isIdentity());
    QMatrix4x4 m;
    m.scale(2.0f);
    qVariantSetValue(variant, m);
    QCOMPARE(m, qVariantValue<QMatrix4x4>(variant));

    void *mmatrix = QMetaType::construct(QVariant::Matrix4x4, 0);
    QVERIFY(mmatrix);
    QMetaType::destroy(QVariant::Matrix4x4, mmatrix);
}

void tst_QVariant::transform()
{
    QVariant variant;
    QTransform matrix = qVariantValue<QTransform>(variant);
    QVERIFY(matrix.isIdentity());
    qVariantSetValue(variant, QTransform().rotate(90));
    QCOMPARE(QTransform().rotate(90), qVariantValue<QTransform>(variant));

    void *mmatrix = QMetaType::construct(QVariant::Transform, 0);
    QVERIFY(mmatrix);
    QMetaType::destroy(QVariant::Transform, mmatrix);
}


void tst_QVariant::vector2D()
{
    QVariant variant;
    QVector2D vector = qVariantValue<QVector2D>(variant);
    QVERIFY(vector.isNull());
    qVariantSetValue(variant, QVector2D(0.1, 0.2));
    QCOMPARE(QVector2D(0.1, 0.2), qVariantValue<QVector2D>(variant));

    void *pvector = QMetaType::construct(QVariant::Vector2D, 0);
    QVERIFY(pvector);
    QMetaType::destroy(QVariant::Vector2D, pvector);
}

void tst_QVariant::vector3D()
{
    QVariant variant;
    QVector3D vector = qVariantValue<QVector3D>(variant);
    QVERIFY(vector.isNull());
    qVariantSetValue(variant, QVector3D(0.1, 0.2, 0.3));
    QCOMPARE(QVector3D(0.1, 0.2, 0.3), qVariantValue<QVector3D>(variant));

    void *pvector = QMetaType::construct(QVariant::Vector3D, 0);
    QVERIFY(pvector);
    QMetaType::destroy(QVariant::Vector3D, pvector);
}

void tst_QVariant::vector4D()
{
    QVariant variant;
    QVector4D vector = qVariantValue<QVector4D>(variant);
    QVERIFY(vector.isNull());
    qVariantSetValue(variant, QVector4D(0.1, 0.2, 0.3, 0.4));
    QCOMPARE(QVector4D(0.1, 0.2, 0.3, 0.4), qVariantValue<QVector4D>(variant));

    void *pvector = QMetaType::construct(QVariant::Vector4D, 0);
    QVERIFY(pvector);
    QMetaType::destroy(QVariant::Vector4D, pvector);
}

void tst_QVariant::quaternion()
{
    QVariant variant;
    QQuaternion quaternion = qVariantValue<QQuaternion>(variant);
    QVERIFY(quaternion.isIdentity());
    qVariantSetValue(variant, QQuaternion(0.1, 0.2, 0.3, 0.4));
    QCOMPARE(QQuaternion(0.1, 0.2, 0.3, 0.4), qVariantValue<QQuaternion>(variant));

    void *pquaternion = QMetaType::construct(QVariant::Quaternion, 0);
    QVERIFY(pquaternion);
    QMetaType::destroy(QVariant::Quaternion, pquaternion);
}

void tst_QVariant::writeToReadFromDataStream_data()
{

    QTest::addColumn<QVariant>("writeVariant");
    QTest::addColumn<bool>("isNull");
    {
        typedef QList<QVariant> variantsList;
        variantsList valuelist;
        valuelist << QVariant( 1 ) << QVariant( QString("Two") ) << QVariant( 3.45 );
        QVariant var(valuelist);
        QTest::newRow( "list_valid" ) << var << false;
    }

    QTest::newRow( "invalid" ) << QVariant() << true;
    QTest::newRow( "bitarray_invalid" ) << QVariant( QBitArray() ) << true;
    QBitArray bitarray( 3 );
    bitarray[0] = 0;
    bitarray[1] = 1;
    bitarray[2] = 0;
    QTest::newRow( "bitarray_valid" ) << QVariant( bitarray ) << false;
    QTest::newRow( "bytearray_invalid" ) << QVariant( QByteArray() ) << true;
    QTest::newRow( "int_invalid") << QVariant(QVariant::Int) << true;
    QByteArray bytearray(5, ' ');
    bytearray[0] = 'T';
    bytearray[1] = 'e';
    bytearray[2] = 's';
    bytearray[3] = 't';
    bytearray[4] = '\0';
    QTest::newRow( "bytearray_valid" ) << QVariant( bytearray ) << false;
    QTest::newRow( "bitmap_invalid" ) << qVariantFromValue( QBitmap() ) << true;
    QBitmap bitmap( 10, 10 );
    bitmap.fill( Qt::red );
    QTest::newRow( "bitmap_valid" ) << qVariantFromValue( bitmap ) << false;
    QTest::newRow( "brush_valid" ) << qVariantFromValue( QBrush( Qt::red ) ) << false;
    QTest::newRow( "color_valid" ) << qVariantFromValue( QColor( Qt::red ) ) << false;
#ifdef QT3_SUPPORT
    QTest::newRow( "colorgroup_valid" ) << qVariantFromValue(QPalette(QColor("turquoise")).active()) << false;
#endif
#ifndef QT_NO_CURSOR
    QTest::newRow( "cursor_valid" ) << qVariantFromValue( QCursor( Qt::PointingHandCursor ) ) << false;
#endif
#ifdef QT3_SUPPORT
    QTest::newRow( "bool_valid" ) << QVariant( true, 0 ) << false;
#endif
    QTest::newRow( "date_invalid" ) << QVariant( QDate() ) << true;
    QTest::newRow( "date_valid" ) << QVariant( QDate( 2002, 07, 06 ) ) << false;
    QTest::newRow( "datetime_invalid" ) << QVariant( QDateTime() ) << true;
    QTest::newRow( "datetime_valid" ) << QVariant( QDateTime( QDate( 2002, 07, 06 ), QTime( 14, 0, 0 ) ) ) << false;
    QTest::newRow( "double_valid" ) << QVariant( 123.456 ) << false;
    QTest::newRow( "float_valid" ) << QVariant( 123.456f ) << false;
    QTest::newRow( "font_valid" ) << qVariantFromValue( QFont( "times", 12 ) ) << false;
    QTest::newRow( "pixmap_invalid" ) << qVariantFromValue( QPixmap() ) << true;
    QPixmap pixmap( 10, 10 );
    pixmap.fill( Qt::red );
    QTest::newRow( "pixmap_valid" ) << qVariantFromValue( pixmap ) << false;
#ifdef QT3_SUPPORT
    QTest::newRow( "iconset_invalid" ) << qVariantFromValue( QIcon() ) << true;
#endif
//    QTest::newRow( "iconset_valid" ) << QVariant( QIcon( pixmap ) ) << false;
    QTest::newRow( "image_invalid" ) << qVariantFromValue( QImage() ) << true;
#ifdef QT3_SUPPORT
    QImage image( 10, 10, 32 );
    image.fill( QColor( Qt::red ).pixel() );
    QTest::newRow( "image_valid" ) << qVariantFromValue( image ) << false;
#endif
    QTest::newRow( "keysequence_valid" ) << qVariantFromValue( QKeySequence( Qt::CTRL + Qt::Key_A ) ) << false;
    QTest::newRow( "int_valid" ) << QVariant( -123 ) << false;
    typedef QList<QVariant> variantsList;
//     variantsList valuelist;
//     valuelist << QVariant( 1 ) << QVariant( QString("Two") ) << QVariant( 3.45 );
//     QTest::newRow( "list_valid" ) << QVariant( valuelist ) << false;
    typedef QMap<QString, QVariant> variantsMap;
    variantsMap vMap;
    vMap.insert( "int", QVariant( 1 ) );
    vMap.insert( "string", QVariant( QString("Two") ) );
    vMap.insert( "double", QVariant( 3.45 ) );
    vMap.insert( "float", QVariant( 3.45f ) );
    QTest::newRow( "map_valid" ) << QVariant( vMap ) << false;
    QTest::newRow( "palette_valid" ) << qVariantFromValue(QPalette(QColor("turquoise"))) << false;
    QTest::newRow( "pen_valid" ) << qVariantFromValue( QPen( Qt::red ) ) << false;
    QTest::newRow( "pointarray_invalid" ) << qVariantFromValue( QPolygon() ) << true;
    QTest::newRow( "pointarray_valid" ) << qVariantFromValue( QPolygon( QRect( 10, 10, 20, 20 ) ) ) << false;
    QTest::newRow( "region_invalid" ) << qVariantFromValue( QRegion() ) << true;
    QTest::newRow( "region_valid" ) << qVariantFromValue( QRegion( 10, 10, 20, 20 ) ) << false;
    QTest::newRow( "sizepolicy_valid" ) << qVariantFromValue( QSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed ) ) << false;
    QTest::newRow( "point_invalid" ) << qVariantFromValue( QPoint() ) << true;
    QTest::newRow( "point_valid" ) << qVariantFromValue( QPoint( 10, 10 ) ) << false;
    QTest::newRow( "rect_invalid" ) << QVariant( QRect() ) << true;
    QTest::newRow( "rect_valid" ) << QVariant( QRect( 10, 10, 20, 20 ) ) << false;
    QTest::newRow( "size_invalid" ) << QVariant( QSize( 0, 0 ) ) << true;
    QTest::newRow( "size_valid" ) << QVariant( QSize( 10, 10 ) ) << false;
    QTest::newRow( "string_invalid" ) << QVariant( QString() ) << true;
    QTest::newRow( "string_valid" ) << QVariant( QString( "Test" ) ) << false;
#ifdef QT3_SUPPORT
    QTest::newRow( "cstring_invalid" ) << QVariant( Q3CString() ) << true;
    QTest::newRow( "cstring_valid" ) << QVariant( Q3CString( "Test" ) ) << false;
#endif
    QStringList stringlist;
    stringlist << "One" << "Two" << "Three";
    QTest::newRow( "stringlist_valid" ) << QVariant( stringlist ) << false;
    QTest::newRow( "time_invalid" ) << QVariant( QTime() ) << true;
    QTest::newRow( "time_valid" ) << QVariant( QTime( 14, 0, 0 ) ) << false;
    QTest::newRow( "uint_valid" ) << QVariant( (uint)123 ) << false;
    QTest::newRow( "qchar" ) << QVariant(QChar('a')) << false;
    QTest::newRow( "qchar_null" ) << QVariant(QChar(0)) << true;
    QTest::newRow( "regexp" ) << QVariant(QRegExp("foo", Qt::CaseInsensitive)) << false;
    QTest::newRow( "regexp_empty" ) << QVariant(QRegExp()) << false;

    // types known to QMetaType, but not part of QVariant::Type
    QTest::newRow("QMetaType::Long invalid") << QVariant(QMetaType::Long, (void *) 0) << false;
    long longInt = -1l;
    QTest::newRow("QMetaType::Long") << QVariant(QMetaType::Long, &longInt) << false;
    QTest::newRow("QMetaType::Short invalid") << QVariant(QMetaType::Short, (void *) 0) << false;
    short shortInt = 1;
    QTest::newRow("QMetaType::Short") << QVariant(QMetaType::Short, &shortInt) << false;
    QTest::newRow("QMetaType::Char invalid") << QVariant(QMetaType::Char, (void *) 0) << false;
    char ch = 'c';
    QTest::newRow("QMetaType::Char") << QVariant(QMetaType::Char, &ch) << false;
    QTest::newRow("QMetaType::ULong invalid") << QVariant(QMetaType::ULong, (void *) 0) << false;
    ulong ulongInt = 1ul;
    QTest::newRow("QMetaType::ULong") << QVariant(QMetaType::ULong, &ulongInt) << false;
    QTest::newRow("QMetaType::UShort invalid") << QVariant(QMetaType::UShort, (void *) 0) << false;
    ushort ushortInt = 1u;
    QTest::newRow("QMetaType::UShort") << QVariant(QMetaType::UShort, &ushortInt) << false;
    QTest::newRow("QMetaType::UChar invalid") << QVariant(QMetaType::UChar, (void *) 0) << false;
    uchar uch = 0xf0;
    QTest::newRow("QMetaType::UChar") << QVariant(QMetaType::UChar, &uch) << false;
    QTest::newRow("QMetaType::Float invalid") << QVariant(QMetaType::Float, (void *) 0) << false;
    float f = 1.234f;
    QTest::newRow("QMetaType::Float") << QVariant(QMetaType::Float, &f) << false;
}

void tst_QVariant::writeToReadFromDataStream()
{
    // See #15831 for more information on the bug

    QFETCH( QVariant, writeVariant );
    QFETCH( bool, isNull );
    QByteArray data;

    QDataStream writeStream( &data, QIODevice::WriteOnly );
    writeStream << writeVariant;

    QVariant readVariant;
    QDataStream readStream( &data, QIODevice::ReadOnly );
    readStream >> readVariant;
    QVERIFY( readVariant.isNull() == isNull );
    // Best way to confirm the readVariant contains the same data?
    // Since only a few won't match since the serial numbers are different
    // I won't bother adding another bool in the data test.
    QVariant::Type writeType = writeVariant.type();
#ifdef QT3_SUPPORT
    if ( writeType != QVariant::Invalid && writeType != QVariant::Bitmap && writeType != QVariant::Pixmap
        && writeType != QVariant::Image && writeType != QVariant::IconSet ) {
#else
    if ( writeType != QVariant::Invalid && writeType != QVariant::Bitmap && writeType != QVariant::Pixmap
        && writeType != QVariant::Image) {
#endif
        switch (writeType) {
        default:
            QCOMPARE( readVariant, writeVariant );
            break;

        // compare types know by QMetaType but not QVariant (QVariant::operator==() knows nothing about them)
        case QMetaType::Long:
            QCOMPARE(qvariant_cast<long>(readVariant), qvariant_cast<long>(writeVariant));
            break;
        case QMetaType::ULong:
            QCOMPARE(qvariant_cast<ulong>(readVariant), qvariant_cast<ulong>(writeVariant));
            break;
        case QMetaType::Short:
            QCOMPARE(qvariant_cast<short>(readVariant), qvariant_cast<short>(writeVariant));
            break;
        case QMetaType::UShort:
            QCOMPARE(qvariant_cast<ushort>(readVariant), qvariant_cast<ushort>(writeVariant));
            break;
        case QMetaType::Char:
            QCOMPARE(qvariant_cast<char>(readVariant), qvariant_cast<char>(writeVariant));
            break;
        case QMetaType::UChar:
            QCOMPARE(qvariant_cast<uchar>(readVariant), qvariant_cast<uchar>(writeVariant));
            break;
        case QMetaType::Float:
            {
                // the uninitialized float can be NaN (observed on Windows Mobile 5 ARMv4i)
                float readFloat = qvariant_cast<float>(readVariant);
                float writtenFloat = qvariant_cast<float>(writeVariant);
                QVERIFY(qIsNaN(readFloat) == qIsNaN(writtenFloat));
                if (!qIsNaN(readFloat))
                    QVERIFY(readFloat == writtenFloat);
            }
            break;
        }
    }
}

void tst_QVariant::writeToReadFromOldDataStream()
{
    QVariant writeVariant = QString("hello");
    QByteArray data;

    QDataStream writeStream(&data, QIODevice::WriteOnly);
    writeStream.setVersion(QDataStream::Qt_2_1);
    writeStream << writeVariant;

    QVariant readVariant;
    QDataStream readStream(&data, QIODevice::ReadOnly);
    readStream.setVersion(QDataStream::Qt_2_1);
    readStream >> readVariant;

    QCOMPARE(writeVariant.userType(), readVariant.userType());
    QCOMPARE(writeVariant, readVariant);
}

void tst_QVariant::checkDataStream()
{
    // test fix for task 215610
    const QByteArray settingsHex("0000002effffffffff");
    const QByteArray settings = QByteArray::fromHex(settingsHex);
    QDataStream in(settings);
    QVariant v;
    in >> v;
    // the line below has been left out for now since the data stream
    // is not necessarily considered corrupt when an invalid QVariant is
    // constructed. However, it might be worth considering changing that behavior
    // in the future.
//    QCOMPARE(in.status(), QDataStream::ReadCorruptData);
    QCOMPARE(v.type(), QVariant::Invalid);
}

void tst_QVariant::operator_eq_eq_data()
{
    QTest::addColumn<QVariant>("left");
    QTest::addColumn<QVariant>("right");
    QTest::addColumn<bool>("equal"); // left == right ?

    QVariant inv;
    QVariant i0( int(0) );
    QVariant i1( int(1) );
    // Invalid
    QTest::newRow( "invinv" ) << inv << inv << true;
    // Int
    QTest::newRow( "int1int1" ) << i1 << i1 << true;
    QTest::newRow( "int1int0" ) << i1 << i0 << false;
    QTest::newRow( "nullint" ) << i0 << QVariant(QVariant::Int) << false;

    // LongLong and ULongLong
    QVariant ll1( (qlonglong)1 );
    QVariant lln2( (qlonglong)-2 );
    QVariant ull1( (qulonglong)1 );
    QVariant ull3( (qulonglong)3 );
    QTest::newRow( "ll1ll1" ) << ll1 << ll1 << true;
    QTest::newRow( "ll1lln2" ) << ll1 << lln2 << false;
    QTest::newRow( "ll1ull1" ) << ull1 << ull1 << true;
    QTest::newRow( "ll1i1" ) << ull1 << i1 << true;
    QTest::newRow( "ull1ull1" ) << ull1 << ull1 << true;
    QTest::newRow( "ull1i1" ) << ull1 << ull1 << true;

    QVariant mInt(-42);
    QVariant mIntString(QByteArray("-42"));
    QVariant mIntQString(QString("-42"));

    QVariant mUInt(42u);
    QVariant mUIntString(QByteArray("42"));
    QVariant mUIntQString(QString("42"));

    QVariant mDouble(42.11);
    QVariant mDoubleString(QByteArray("42.11"));
    QVariant mDoubleQString(QString("42.11"));

    QVariant mFloat(42.11f);
    QVariant mFloatString(QByteArray("42.11"));
    QVariant mFloatQString(QString("42.11"));

    QVariant mLongLong((qlonglong)-42);
    QVariant mLongLongString(QByteArray("-42"));
    QVariant mLongLongQString(QString("-42"));

    QVariant mULongLong((qulonglong)42);
    QVariant mULongLongString(QByteArray("42"));
    QVariant mULongLongQString(QString("42"));

#ifdef QT3_SUPPORT
    QVariant mBool(false, 0);
#else
    QVariant mBool(false);
#endif
    QVariant mBoolString(QByteArray("false"));
    QVariant mBoolQString(QString("false"));

    QTest::newRow( "double_int" ) << QVariant(42.0) << QVariant(42) << true;
    QTest::newRow( "float_int" ) << QVariant(42.f) << QVariant(42) << true;
    QTest::newRow( "mInt_mIntString" ) << mInt << mIntString << true;
    QTest::newRow( "mIntString_mInt" ) << mIntString << mInt << true;
    QTest::newRow( "mInt_mIntQString" ) << mInt << mIntQString << true;
    QTest::newRow( "mIntQString_mInt" ) << mIntQString << mInt << true;

    QTest::newRow( "mUInt_mUIntString" ) << mUInt << mUIntString << true;
    QTest::newRow( "mUIntString_mUInt" ) << mUIntString << mUInt << true;
    QTest::newRow( "mUInt_mUIntQString" ) << mUInt << mUIntQString << true;
    QTest::newRow( "mUIntQString_mUInt" ) << mUIntQString << mUInt << true;

    QTest::newRow( "mDouble_mDoubleString" ) << mDouble << mDoubleString << true;
    QTest::newRow( "mDoubleString_mDouble" ) << mDoubleString << mDouble << true;
    QTest::newRow( "mDouble_mDoubleQString" ) << mDouble << mDoubleQString << true;
    QTest::newRow( "mDoubleQString_mDouble" ) << mDoubleQString << mDouble << true;

    QTest::newRow( "mFloat_mFloatString" ) << mFloat << mFloatString << true;
    QTest::newRow( "mFloatString_mFloat" ) << mFloatString << mFloat << true;
    QTest::newRow( "mFloat_mFloatQString" ) << mFloat << mFloatQString << true;
    QTest::newRow( "mFloatQString_mFloat" ) << mFloatQString << mFloat << true;

    QTest::newRow( "mLongLong_mLongLongString" ) << mLongLong << mLongLongString << true;
    QTest::newRow( "mLongLongString_mLongLong" ) << mLongLongString << mLongLong << true;
    QTest::newRow( "mLongLong_mLongLongQString" ) << mLongLong << mLongLongQString << true;
    QTest::newRow( "mLongLongQString_mLongLong" ) << mLongLongQString << mLongLong << true;

    QTest::newRow( "mULongLong_mULongLongString" ) << mULongLong << mULongLongString << true;
    QTest::newRow( "mULongLongString_mULongLong" ) << mULongLongString << mULongLong << true;
    QTest::newRow( "mULongLong_mULongLongQString" ) << mULongLong << mULongLongQString << true;
    QTest::newRow( "mULongLongQString_mULongLong" ) << mULongLongQString << mULongLong << true;

    QTest::newRow( "mBool_mBoolString" ) << mBool << mBoolString << true;
    QTest::newRow( "mBoolString_mBool" ) << mBoolString << mBool << true;
    QTest::newRow( "mBool_mBoolQString" ) << mBool << mBoolQString << true;
    QTest::newRow( "mBoolQString_mBool" ) << mBoolQString << mBool << true;

    QTest::newRow("ba2qstring") << QVariant(QLatin1String("hallo")) << QVariant(QString("hallo")) << true;
    QTest::newRow("qstring2ba") << QVariant(QString("hallo")) << QVariant(QLatin1String("hallo")) << true;
    QTest::newRow("char_char") << QVariant(QChar('a')) << QVariant(QChar('a')) << true;
    QTest::newRow("char_char2") << QVariant(QChar('a')) << QVariant(QChar('b')) << false;

    QTest::newRow("invalidConversion") << QVariant(QString("bubu")) << QVariant(0) << false;
    QTest::newRow("invalidConversionR") << QVariant(0) << QVariant(QString("bubu")) << false;
    // ### many other combinations missing

    {
        QMap<QString, QVariant> map1;
        map1.insert( "X", 1 );

        QMap<QString, QVariant> map2;
        map2.insert( "Y", 1 );

        QTest::newRow("TwoItemsInEqual") << QVariant(map1) << QVariant(map2) << false;

    }

    {
        QMap<QString, QVariant> map1;
        map1.insert( "X", 1 );

        QMap<QString, QVariant> map2;
        map2.insert( "X", 1 );

        QTest::newRow("TwoItemsEqual") << QVariant(map1) << QVariant(map2) << true;
    }

    {
        QMap<QString, QVariant> map1;
        map1.insert( "X", 1 );

        QMap<QString, QVariant> map2;

        QTest::newRow("PopulatedEmptyMap") << QVariant(map1) << QVariant(map2) << false;
    }

    {
        QMap<QString, QVariant> map1;

        QMap<QString, QVariant> map2;
        map2.insert( "X", 1 );

        QTest::newRow("EmptyPopulatedMap") << QVariant(map1) << QVariant(map2) << false;
    }

    {
        QMap<QString, QVariant> map1;
        map1.insert( "X", 1 );
        map1.insert( "Y", 1 );

        QMap<QString, QVariant> map2;
        map2.insert( "X", 1 );

        QTest::newRow("FirstLarger") << QVariant(map1) << QVariant(map2) << false;
    }

    {
        QMap<QString, QVariant> map1;
        map1.insert( "X", 1 );

        QMap<QString, QVariant> map2;
        map2.insert( "X", 1 );
        map2.insert( "Y", 1 );

        QTest::newRow("SecondLarger") << QVariant(map1) << QVariant(map2) << false;
    }

    // same thing with hash
    {
        QHash<QString, QVariant> hash1;
        hash1.insert( "X", 1 );

        QHash<QString, QVariant> hash2;
        hash2.insert( "Y", 1 );

        QTest::newRow("HashTwoItemsInEqual") << QVariant(hash1) << QVariant(hash2) << false;

    }

    {
        QHash<QString, QVariant> hash1;
        hash1.insert( "X", 1 );

        QHash<QString, QVariant> hash2;
        hash2.insert( "X", 1 );

        QTest::newRow("HashTwoItemsEqual") << QVariant(hash1) << QVariant(hash2) << true;
    }

    {
        QHash<QString, QVariant> hash1;
        hash1.insert( "X", 1 );

        QHash<QString, QVariant> hash2;

        QTest::newRow("HashPopulatedEmptyHash") << QVariant(hash1) << QVariant(hash2) << false;
    }

    {
        QHash<QString, QVariant> hash1;

        QHash<QString, QVariant> hash2;
        hash2.insert( "X", 1 );

        QTest::newRow("EmptyPopulatedHash") << QVariant(hash1) << QVariant(hash2) << false;
    }

    {
        QHash<QString, QVariant> hash1;
        hash1.insert( "X", 1 );
        hash1.insert( "Y", 1 );

        QHash<QString, QVariant> hash2;
        hash2.insert( "X", 1 );

        QTest::newRow("HashFirstLarger") << QVariant(hash1) << QVariant(hash2) << false;
    }

    {
        QHash<QString, QVariant> hash1;
        hash1.insert( "X", 1 );

        QHash<QString, QVariant> hash2;
        hash2.insert( "X", 1 );
        hash2.insert( "Y", 1 );

        QTest::newRow("HashSecondLarger") << QVariant(hash1) << QVariant(hash2) << false;
    }

    QTest::newRow( "UserType" ) << QVariant(QVariant::UserType) << QVariant(QVariant::UserType) << true;
    QVariant mUserType(QVariant::UserType);
    QTest::newRow( "Shared UserType" ) << mUserType << mUserType << true;
}

void tst_QVariant::operator_eq_eq()
{
    QFETCH( QVariant, left );
    QFETCH( QVariant, right );
    QFETCH( bool, equal );
    QEXPECT_FAIL("nullint", "See task 118496", Continue);
    QCOMPARE( left == right, equal );
}

void tst_QVariant::operator_eq_eq_rhs()
{
    QVariant v = 42;

    QVERIFY(v == 42);
    QVERIFY(42 == v);

#if 0
    /* This should _not_ compile */
    QStringList list;
    QDateTime dt;

    QVERIFY(dt == list);
#endif
}

void tst_QVariant::typeName_data()
{
    QTest::addColumn<int>("type");
    QTest::addColumn<QByteArray>("res");
    QTest::newRow("0") << int(QVariant::Invalid) << QByteArray("");
    QTest::newRow("1") << int(QVariant::Map) << QByteArray("QVariantMap");
    QTest::newRow("2") << int(QVariant::List) << QByteArray("QVariantList");
    QTest::newRow("3") << int(QVariant::String) << QByteArray("QString");
    QTest::newRow("4") << int(QVariant::StringList) << QByteArray("QStringList");
    QTest::newRow("5") << int(QVariant::Font) << QByteArray("QFont");
    QTest::newRow("6") << int(QVariant::Pixmap) << QByteArray("QPixmap");
    QTest::newRow("7") << int(QVariant::Brush) << QByteArray("QBrush");
    QTest::newRow("8") << int(QVariant::Rect) << QByteArray("QRect");
    QTest::newRow("9") << int(QVariant::Size) << QByteArray("QSize");
    QTest::newRow("10") << int(QVariant::Color) << QByteArray("QColor");
    QTest::newRow("11") << int(QVariant::Palette) << QByteArray("QPalette");
#ifdef QT3_SUPPORT
    QTest::newRow("12") << int(QVariant::ColorGroup) << QByteArray("QColorGroup");
    QTest::newRow("13") << int(QVariant::IconSet) << QByteArray("QIcon");
#endif
    QTest::newRow("14") << int(QVariant::Point) << QByteArray("QPoint");
    QTest::newRow("15") << int(QVariant::Image) << QByteArray("QImage");
    QTest::newRow("16") << int(QVariant::Int) << QByteArray("int");
    QTest::newRow("17") << int(QVariant::UInt) << QByteArray("uint");
    QTest::newRow("18") << int(QVariant::Bool) << QByteArray("bool");
    QTest::newRow("19") << int(QVariant::Double) << QByteArray("double");
    QTest::newRow("20") << int(QMetaType::Float) << QByteArray("float");
    QTest::newRow("21") << int(QVariant::Polygon) << QByteArray("QPolygon");
    QTest::newRow("22") << int(QVariant::Region) << QByteArray("QRegion");
    QTest::newRow("23") << int(QVariant::Bitmap) << QByteArray("QBitmap");
    QTest::newRow("24") << int(QVariant::Cursor) << QByteArray("QCursor");
    QTest::newRow("25") << int(QVariant::SizePolicy) << QByteArray("QSizePolicy");
    QTest::newRow("26") << int(QVariant::Date) << QByteArray("QDate");
    QTest::newRow("27") << int(QVariant::Time) << QByteArray("QTime");
    QTest::newRow("28") << int(QVariant::DateTime) << QByteArray("QDateTime");
    QTest::newRow("29") << int(QVariant::ByteArray) << QByteArray("QByteArray");
    QTest::newRow("30") << int(QVariant::BitArray) << QByteArray("QBitArray");
    QTest::newRow("31") << int(QVariant::KeySequence) << QByteArray("QKeySequence");
    QTest::newRow("32") << int(QVariant::Pen) << QByteArray("QPen");
    QTest::newRow("33") << int(QVariant::LongLong) << QByteArray("qlonglong");
    QTest::newRow("34") << int(QVariant::ULongLong) << QByteArray("qulonglong");
    QTest::newRow("35") << int(QVariant::Char) << QByteArray("QChar");
    QTest::newRow("36") << int(QVariant::Url) << QByteArray("QUrl");
    QTest::newRow("37") << int(QVariant::TextLength) << QByteArray("QTextLength");
    QTest::newRow("38") << int(QVariant::TextFormat) << QByteArray("QTextFormat");
    QTest::newRow("39") << int(QVariant::Locale) << QByteArray("QLocale");
    QTest::newRow("40") << int(QVariant::LineF) << QByteArray("QLineF");
    QTest::newRow("41") << int(QVariant::RectF) << QByteArray("QRectF");
    QTest::newRow("42") << int(QVariant::PointF) << QByteArray("QPointF");
    QTest::newRow("43") << int(QVariant::RegExp) << QByteArray("QRegExp");
    QTest::newRow("44") << int(QVariant::UserType) << QByteArray("UserType");
    QTest::newRow("45") << int(QVariant::Matrix) << QByteArray("QMatrix");
    QTest::newRow("46") << int(QVariant::Transform) << QByteArray("QTransform");
    QTest::newRow("47") << int(QVariant::Hash) << QByteArray("QVariantHash");
    QTest::newRow("48") << int(QVariant::Matrix4x4) << QByteArray("QMatrix4x4");
    QTest::newRow("49") << int(QVariant::Vector2D) << QByteArray("QVector2D");
    QTest::newRow("50") << int(QVariant::Vector3D) << QByteArray("QVector3D");
    QTest::newRow("51") << int(QVariant::Vector4D) << QByteArray("QVector4D");
    QTest::newRow("52") << int(QVariant::Quaternion) << QByteArray("QQuaternion");
}

void tst_QVariant::typeName()
{
    QFETCH( int, type );
    QFETCH( QByteArray, res );
    QCOMPARE(QString::fromLatin1(QVariant::typeToName((QVariant::Type)type)),
            QString::fromLatin1(res.constData()));
}

// test nameToType as well
void tst_QVariant::typeToName()
{
    QVariant v;
    QCOMPARE( QVariant::typeToName( v.type() ), (const char*)0 ); // Invalid
    // assumes that QVariant::Type contains consecutive values

    int max = QVariant::Quaternion;
    for ( int t = 1; t <= max; t++ ) {
        const char *n = QVariant::typeToName( (QVariant::Type)t );
        if (n)
            QCOMPARE( int(QVariant::nameToType( n )), t );

    }
    QCOMPARE(QVariant::typeToName(QVariant::Int), "int");
    // not documented but we return 0 if the type is out of range
    // by testing this we catch cases where QVariant is extended
    // but type_map is not updated accordingly
    QCOMPARE( QVariant::typeToName( QVariant::Type(max+1) ), (char*)0 );
    // invalid type names
    QVERIFY( QVariant::nameToType( 0 ) == QVariant::Invalid );
    QVERIFY( QVariant::nameToType( "" ) == QVariant::Invalid );
    QVERIFY( QVariant::nameToType( "foo" ) == QVariant::Invalid );
    QCOMPARE(QVariant::nameToType("QIconSet"), QVariant::Icon);
#ifdef QT3_SUPPORT
    QCOMPARE(QVariant::nameToType("Q3CString"), QVariant::ByteArray);
#endif
}

void tst_QVariant::streamInvalidVariant()
{
    // I wasn't sure where this test belonged, so it's here
    // See #17423 for more details

    int writeX = 1;
    int writeY = 2;
    int readX;
    int readY;
    QVariant writeVariant;
    QVariant readVariant;

    QVERIFY( writeVariant.type() == QVariant::Invalid );

    QByteArray data;
    QDataStream writeStream( &data, QIODevice::WriteOnly );
    writeStream << writeX << writeVariant << writeY;

    QDataStream readStream( &data, QIODevice::ReadOnly );
    readStream >> readX >> readVariant >> readY;

    QVERIFY( readX == writeX );
    // Two invalid QVariant's aren't necessarily the same, so == will
    // return false if one is invalid, so check the type() instead
    QVERIFY( readVariant.type() == QVariant::Invalid );
    QVERIFY( readY == writeY );
}

void tst_QVariant::nullAsType()
{
#ifdef QT3_SUPPORT
    QVariant null;
    QVERIFY(null.isNull());

    null.asInt();
    QVERIFY(null.isNull());

    null = QVariant(QString::null);
    QVERIFY(null.isNull());

    null.asInt();
    QVERIFY(null.isNull());

    int type = QVariant::Invalid;
    while (type < (int)QVariant::ULongLong) {
        null = QVariant();
        QVERIFY(null.isNull());

        type++;
        if (type == 20)
            continue;
        if (type == QVariant::Size)
            // QSize has its own ideas of ::isNull
            continue;

        if (!null.convert((QVariant::Type)type))
            continue;
        QCOMPARE((int)null.type(), type);

        QVERIFY2(null.isNull(), qPrintable(QString("'null.isNull()' failed for type: %1").arg(null.typeName())));
    }

    null = QVariant(QLatin1String(static_cast<const char *>(0)));
    QVERIFY(null.isNull());
#endif
}

static int instanceCount = 0;

struct MyType
{
    MyType(int n = 0, const char *t=0): number(n), text(t)
    {
        ++instanceCount;
    }
    MyType(const MyType &other)
        : number(other.number), text(other.text)
    {
        ++instanceCount;
    }
    ~MyType()
    {
        --instanceCount;
    }
    int number;
    const char *text;
};

Q_DECLARE_METATYPE(MyType)
Q_DECLARE_METATYPE(MyType*)

void tst_QVariant::userType()
{
    {
        QVariant userVariant(QVariant::UserType);

        QVERIFY(userVariant.isValid());
        QVERIFY(userVariant.isNull());
    }

    {
        MyType data(1, "eins");
        MyType data2(2, "zwei");

        {
            QVariant userVar;
            qVariantSetValue(userVar, data);

            QCOMPARE(userVar.type(), QVariant::UserType);
            QCOMPARE(userVar.typeName(), "MyType");
            QVERIFY(!userVar.isNull());
            QVERIFY(!userVar.canConvert(QVariant::String));
            QVERIFY(!userVar.canConvert(QVariant::UserType));

            QVariant userVar2(userVar);
            QVERIFY(userVar == userVar2);

            qVariantSetValue(userVar2, data2);
            QVERIFY(userVar != userVar2);

            const MyType *varData = static_cast<const MyType *>(userVar.constData());
            QVERIFY(varData);
            QCOMPARE(varData->number, data.number);
            QCOMPARE(varData->text, data.text);

            QVariant userVar3;
            qVariantSetValue(userVar3, data2);
            QVERIFY(userVar2 != userVar3);
            userVar3 = userVar2;
            QVERIFY(userVar2 == userVar3);
        }
        {
            QVariant userVar;
            qVariantSetValue(userVar, &data);

            QCOMPARE(userVar.type(), QVariant::UserType);
            QCOMPARE(userVar.typeName(), "MyType*");
            QVERIFY(!userVar.isNull());
            QVERIFY(!userVar.canConvert(QVariant::String));
            QVERIFY(!userVar.canConvert(QVariant::UserType));

            QVariant userVar2(userVar);
            QVERIFY(userVar == userVar2);

            qVariantSetValue(userVar2, &data2);
            QVERIFY(userVar != userVar2);

            MyType * const*varData = reinterpret_cast<MyType *const *>(userVar.constData());
            QVERIFY(varData);
            QCOMPARE(*varData, &data);

            QVariant userVar3;
            qVariantSetValue(userVar3, &data2);

            /* This check is correct now. userVar2 contains a pointer to data2 and so
             * does userVar3. */
            QVERIFY(userVar2 == userVar3);

            userVar3 = userVar2;
            QVERIFY(userVar2 == userVar3);
        }

        QCOMPARE(instanceCount, 2);
        QVariant myCarrier;
        qVariantSetValue(myCarrier, data);
        QCOMPARE(instanceCount, 3);

        {
            QVariant second = myCarrier;
            QCOMPARE(instanceCount, 3);
            second.detach();
            QCOMPARE(instanceCount, 4);
        }
        QCOMPARE(instanceCount, 3);

        MyType data3(0, "null");
        data3 = qVariantValue<MyType>(myCarrier);
        QCOMPARE(data3.number, 1);
        QCOMPARE(data3.text, (const char *)"eins");
#ifndef Q_CC_SUN
        QCOMPARE(instanceCount, 4);
#endif

    }

    {
        const MyType data(3, "drei");
        QVariant myCarrier;

        qVariantSetValue(myCarrier, data);
        QCOMPARE(myCarrier.typeName(), "MyType");

        const MyType data2 = qvariant_cast<MyType>(myCarrier);
        QCOMPARE(data2.number, 3);
        QCOMPARE(data2.text, (const char *)"drei");
    }

    {
        short s = 42;
        QVariant myCarrier;

        qVariantSetValue(myCarrier, s);
        QCOMPARE((int)qvariant_cast<short>(myCarrier), 42);
    }

    {
        qlonglong ll = Q_INT64_C(42);
        QVariant myCarrier;

        qVariantSetValue(myCarrier, ll);
        QCOMPARE(qvariant_cast<int>(myCarrier), 42);
    }

    QCOMPARE(instanceCount, 0);
}

struct MyTypePOD
{
    int a;
    int b;
};
Q_DECLARE_METATYPE(MyTypePOD)

void tst_QVariant::podUserType()
{
    MyTypePOD pod;
    pod.a = 10;
    pod.b = 20;

    QVariant pod_as_variant = qVariantFromValue(pod);
    MyTypePOD pod2 = qvariant_cast<MyTypePOD>(pod_as_variant);

    QCOMPARE(pod.a, pod2.a);
    QCOMPARE(pod.b, pod2.b);

    qVariantSetValue(pod_as_variant, pod);
    pod2 = qVariantValue<MyTypePOD>(pod_as_variant);

    QCOMPARE(pod.a, pod2.a);
    QCOMPARE(pod.b, pod2.b);
}

void tst_QVariant::basicUserType()
{
    QVariant v;
    {
        int i = 7;
        v = QVariant(QMetaType::Int, &i);
    }
    QCOMPARE(v.type(), QVariant::Int);
    QCOMPARE(v.toInt(), 7);

    {
        QString s("foo");
        v = QVariant(QMetaType::QString, &s);
    }
    QCOMPARE(v.type(), QVariant::String);
    QCOMPARE(v.toString(), QString("foo"));

    {
        double d = 4.4;
        v = QVariant(QMetaType::Double, &d);
    }
    QCOMPARE(v.type(), QVariant::Double);
    QCOMPARE(v.toDouble(), 4.4);

    {
        float f = 4.5f;
        v = QVariant(QMetaType::Float, &f);
    }
    QCOMPARE(v.userType(), int(QMetaType::Float));
    QCOMPARE(v.toDouble(), 4.5);

    {
        QByteArray ba("bar");
        v = QVariant(QMetaType::QByteArray, &ba);
    }
    QCOMPARE(v.type(), QVariant::ByteArray);
    QCOMPARE(v.toByteArray(), QByteArray("bar"));
}

void tst_QVariant::data_()
{
    QVariant v;

    QVariant i = 1;
    QVariant d = 1.12;
    QVariant f = 1.12f;
    QVariant ll = (qlonglong)2;
    QVariant ull = (qulonglong)3;
    QVariant s(QString("hallo"));
    QVariant r(QRect(1,2,3,4));

    v = i;
    QVERIFY(v.data());
    QCOMPARE(*static_cast<int *>(v.data()), i.toInt());

    v = d;
    QVERIFY(v.data());
    QCOMPARE(*static_cast<double *>(v.data()), d.toDouble());

    v = f;
    QVERIFY(v.data());
    QCOMPARE(*static_cast<float *>(v.data()), qVariantValue<float>(v));

    v = ll;
    QVERIFY(v.data());
    QCOMPARE(*static_cast<qlonglong *>(v.data()), ll.toLongLong());

    v = ull;
    QVERIFY(v.data());
    QCOMPARE(*static_cast<qulonglong *>(v.data()), ull.toULongLong());

    v = s;
    QVERIFY(v.data());
    QCOMPARE(*static_cast<QString *>(v.data()), s.toString());

    v = r;
    QVERIFY(v.data());
    QCOMPARE(*static_cast<QRect *>(v.data()), r.toRect());
}

void tst_QVariant::constData()
{
    QVariant v;

    int i = 1;
    double d = 1.12;
    float f = 1.12f;
    qlonglong ll = 2;
    qulonglong ull = 3;
    QString s("hallo");
    QRect r(1,2,3,4);

    v = QVariant(i);
    QVERIFY(v.constData());
    QCOMPARE(*static_cast<const int *>(v.constData()), i);

    v = QVariant(d);
    QVERIFY(v.constData());
    QCOMPARE(*static_cast<const double *>(v.constData()), d);

    v = QVariant(f);
    QVERIFY(v.constData());
    QCOMPARE(*static_cast<const float *>(v.constData()), f);

    v = QVariant(ll);
    QVERIFY(v.constData());
    QCOMPARE(*static_cast<const qlonglong *>(v.constData()), ll);

    v = QVariant(ull);
    QVERIFY(v.constData());
    QCOMPARE(*static_cast<const qulonglong *>(v.constData()), ull);

    v = QVariant(s);
    QVERIFY(v.constData());
    QCOMPARE(*static_cast<const QString *>(v.constData()), s);

    v = QVariant(r);
    QVERIFY(v.constData());
    QCOMPARE(*static_cast<const QRect *>(v.constData()), r);
}

struct Foo
{
    Foo(): i(0) {}
    int i;
};

Q_DECLARE_METATYPE(Foo)

void tst_QVariant::variant_to()
{
    QVariant v1(4.2);
    QVariant v2(5);

    QVariant v3;
    QVariant v4;

    QStringList sl;
    sl << QLatin1String("blah");

    qVariantSetValue(v3, sl);

    Foo foo;
    foo.i = 42;

    qVariantSetValue(v4, foo);

    QCOMPARE(qvariant_cast<double>(v1), 4.2);
    QCOMPARE(qvariant_cast<float>(v1), 4.2f);
    QCOMPARE(qvariant_cast<int>(v2), 5);
    QCOMPARE(qvariant_cast<QStringList>(v3), sl);
    QCOMPARE(qvariant_cast<QString>(v3), QString::fromLatin1("blah"));

    QCOMPARE(qvariant_cast<Foo>(v4).i, 42);

    QVariant v5;
    QCOMPARE(qvariant_cast<Foo>(v5).i, 0);

    QCOMPARE(qvariant_cast<int>(v1), 4);

    QVariant n = qVariantFromValue<short>(42);
    QCOMPARE(qvariant_cast<int>(n), 42);
    QCOMPARE(qvariant_cast<uint>(n), 42u);
    QCOMPARE(qvariant_cast<double>(n), 42.0);
    QCOMPARE(qvariant_cast<float>(n), 42.f);
    QCOMPARE(qvariant_cast<short>(n), short(42));
    QCOMPARE(qvariant_cast<ushort>(n), ushort(42));

    n = qVariantFromValue(43l);
    QCOMPARE(qvariant_cast<int>(n), 43);
    QCOMPARE(qvariant_cast<uint>(n), 43u);
    QCOMPARE(qvariant_cast<double>(n), 43.0);
    QCOMPARE(qvariant_cast<float>(n), 43.f);
    QCOMPARE(qvariant_cast<long>(n), 43l);

    n = QLatin1String("44");
    QCOMPARE(qvariant_cast<int>(n), 44);
    QCOMPARE(qvariant_cast<ulong>(n), 44ul);
    QCOMPARE(qvariant_cast<float>(n), 44.0f);

    QCOMPARE(qVariantFromValue(0.25f).toDouble(), 0.25);
}

struct Blah { int i; };

QDataStream& operator>>(QDataStream& s, Blah& c)
{ return (s >> c.i); }

QDataStream& operator<<(QDataStream& s, const Blah& c)
{ return (s << c.i); }

void tst_QVariant::saveLoadCustomTypes()
{
    QByteArray data;

    Blah i = { 42 };
    int tp = qRegisterMetaType<Blah>("Blah");
    QVariant v = QVariant(tp, &i);

    qRegisterMetaTypeStreamOperators<Blah>("Blah");

    QCOMPARE(v.userType(), tp);
    QVERIFY(v.type() == QVariant::UserType);
    {
        QDataStream stream(&data, QIODevice::WriteOnly);
        stream << v;
    }

    v = QVariant();

    {
        QDataStream stream(data);
        stream >> v;
    }

    QCOMPARE(int(v.userType()), QMetaType::type("Blah"));
    int value = *(int*)v.constData();
    QCOMPARE(value, 42);
}

void tst_QVariant::url()
{
    QString str("http://qt.nokia.com");
    QUrl url(str);

    QVariant v(url); //built with a QUrl

    QVariant v2 = v;

    QVariant v3(str); //built with a QString

    QCOMPARE(v2.toUrl(), url);
    QVERIFY(qVariantCanConvert<QUrl>(v3));
    QCOMPARE(v2.toUrl(), v3.toUrl());

    QVERIFY(qVariantCanConvert<QString>(v2));
    QCOMPARE(v2.toString(), str);
    QCOMPARE(v3.toString(), str);
}

void tst_QVariant::globalColor()
{
    QVariant variant(Qt::blue);
    QVERIFY(variant.type() == QVariant::Color);
    QVERIFY(qVariantValue<QColor>(variant) == QColor(Qt::blue));
}

void tst_QVariant::variantMap()
{
    QMap<QString, QVariant> map;
    map["test"] = 42;

    QVariant v = map;
    QVariantMap map2 = qvariant_cast<QVariantMap>(v);

    QCOMPARE(map2.value("test").toInt(), 42);

    QVariant v2 = QVariant(QMetaType::type("QVariantMap"), &map);
    QCOMPARE(qvariant_cast<QVariantMap>(v2).value("test").toInt(), 42);

    QVariant v3 = QVariant(QMetaType::type("QMap<QString, QVariant>"), &map);
    QCOMPARE(qvariant_cast<QVariantMap>(v3).value("test").toInt(), 42);
}

void tst_QVariant::variantHash()
{
    QHash<QString, QVariant> hash;
    hash["test"] = 42;

    QVariant v = hash;
    QVariantHash hash2 = qvariant_cast<QVariantHash>(v);

    QCOMPARE(hash2.value("test").toInt(), 42);

    QVariant v2 = QVariant(QMetaType::type("QVariantHash"), &hash);
    QCOMPARE(qvariant_cast<QVariantHash>(v2).value("test").toInt(), 42);

    QVariant v3 = QVariant(QMetaType::type("QHash<QString, QVariant>"), &hash);
    QCOMPARE(qvariant_cast<QVariantHash>(v3).value("test").toInt(), 42);
}

void tst_QVariant::invalidAsByteArray()
{
#ifdef QT3_SUPPORT
    QVariant v;
    QByteArray &a = v.asByteArray();
    a.resize(2);
    a[0] = 'a';
    a[1] = 'b';
    QCOMPARE(v, QVariant(QByteArray("ab")));
#else
    QSKIP("Qt compiled without Qt3Support",SkipAll);
#endif
}

void tst_QVariant::invalidQColor() const
{
    QVariant va("An invalid QColor::name() value.");
    QTest::ignoreMessage(QtWarningMsg, "QColor::setNamedColor: Unknown color name 'An invalid QColor::name() value.'");
    QVERIFY(va.canConvert(QVariant::Color));

    QVERIFY(!va.convert(QVariant::Color));

    QVERIFY(!qvariant_cast<QColor>(va).isValid());
}

void tst_QVariant::qvariant_cast_QObject_data() {

    QTest::addColumn<QVariant>("data");
    QTest::addColumn<bool>("success");
    QTest::newRow("from QObject") << QVariant(QMetaType::QObjectStar, new QObject(this)) << true;
    QTest::newRow("from String") << QVariant(QLatin1String("1, 2, 3")) << false;
    QTest::newRow("from int") << QVariant((int) 123) << false;
}


void tst_QVariant::qvariant_cast_QObject() {
    QFETCH(QVariant, data);
    QFETCH(bool, success);

    QObject *o = qvariant_cast<QObject *>(data);
    QCOMPARE(o != 0, success);
}

Q_DECLARE_METATYPE(qint8);

void tst_QVariant::convertToQUint8() const
{
    // See task 158470.

    /* qint8. */
    {
        const qint8 anInt = 32;

        /* QVariant(int) gets invoked here so the QVariant has nothing with qint8 to do.
         * It's of type QVariant::Int. */
        const QVariant v0 = anInt;

        QVERIFY(qVariantCanConvert<qint8>(v0));
        QCOMPARE(int(qvariant_cast<qint8>(v0)), 32);
        QCOMPARE(int(v0.toInt()), 32);
        QCOMPARE(v0.toString(), QString("32"));

        QCOMPARE(int(qvariant_cast<qlonglong>(v0)), 32);
        QCOMPARE(int(qvariant_cast<char>(v0)),      32);
        QCOMPARE(int(qvariant_cast<short>(v0)),     32);
        QCOMPARE(int(qvariant_cast<long>(v0)),      32);
        QCOMPARE(int(qvariant_cast<float>(v0)),     32);
        QCOMPARE(int(qvariant_cast<double>(v0)),    32);
    }

    /* quint8. */
    {
        const quint8 anInt = 32;
        const QVariant v0 = anInt;

        QVERIFY(qVariantCanConvert<quint8>(v0));
        QCOMPARE(int(qvariant_cast<quint8>(v0)), 32);
        QCOMPARE(int(v0.toUInt()), 32);
        QCOMPARE(v0.toString(), QString("32"));
    }

    /* qint16. */
    {
        const qint16 anInt = 32;
        const QVariant v0 = anInt;

        QVERIFY(qVariantCanConvert<qint16>(v0));
        QCOMPARE(int(qvariant_cast<qint16>(v0)), 32);
        QCOMPARE(int(v0.toInt()), 32);
        QCOMPARE(v0.toString(), QString("32"));
    }

    /* quint16. */
    {
        const quint16 anInt = 32;
        const QVariant v0 = anInt;

        QVERIFY(qVariantCanConvert<quint16>(v0));
        QCOMPARE(int(qvariant_cast<quint16>(v0)), 32);
        QCOMPARE(int(v0.toUInt()), 32);
        QCOMPARE(v0.toString(), QString("32"));
    }
}

void tst_QVariant::comparePointers() const
{
    class MyClass
    {
    };

    MyClass myClass;

    QVariant v  = qVariantFromValue<void *>(&myClass);
    QVariant v2 = qVariantFromValue<void *>(&myClass);

    QCOMPARE(v, v2);
}

struct Data {};
Q_DECLARE_METATYPE(Data*)

void tst_QVariant::voidStar() const
{
    char c;
    void *p1 = &c;
    void *p2 = p1;

    QVariant v1, v2;
    v1 = qVariantFromValue(p1);
    v2 = v1;
    QVERIFY(v1 == v2);

    v2 = qVariantFromValue(p2);
    QVERIFY(v1 == v2);

    p2 = 0;
    v2 = qVariantFromValue(p2);
    QVERIFY(v1 != v2);
}

void tst_QVariant::dataStar() const
{
    qRegisterMetaType<Data*>();
    Data *p1 = new Data;

    QVariant v1 = qVariantFromValue(p1);
    QCOMPARE(v1.userType(), qMetaTypeId<Data*>());
    QCOMPARE(qvariant_cast<Data*>(v1), p1);

    QVariant v2 = v1;
    QVERIFY(v1 == v2);

    v2 = qVariantFromValue(p1);
    QVERIFY(v1 == v2);
    delete p1;
}

void tst_QVariant::canConvertQStringList() const
{
    QFETCH(bool, canConvert);
    QFETCH(QStringList, input);
    QFETCH(QString, result);

    QVariant v(input);

    QCOMPARE(v.canConvert(QVariant::String), canConvert);
    QCOMPARE(v.toString(), result);
}

void tst_QVariant::canConvertQStringList_data() const
{
    QTest::addColumn<bool>("canConvert");
    QTest::addColumn<QStringList>("input");
    QTest::addColumn<QString>("result");

    QTest::newRow("An empty list") << false << QStringList() << QString();
    QTest::newRow("A single item") << true << QStringList(QLatin1String("foo")) << QString::fromLatin1("foo");
    QTest::newRow("A single, but empty item") << true << QStringList(QString()) << QString();

    QStringList l;
    l << "a" << "b";

    QTest::newRow("Two items") << false << l << QString();

    l << "c";
    QTest::newRow("Three items") << false << l << QString();
}

template<typename T> void convertMetaType()
{
    QVERIFY(qVariantFromValue<T>(10).isValid());
    QVERIFY(qVariantFromValue<T>(10).canConvert(QVariant::Int));
    QCOMPARE(qVariantFromValue<T>(10).toInt(), 10);
    QCOMPARE(qVariantFromValue<T>(10), qVariantFromValue<T>(10));
}

void tst_QVariant::canConvertMetaTypeToInt() const
{
    convertMetaType<long>();
    convertMetaType<short>();
    convertMetaType<short>();
    convertMetaType<unsigned short>();
    convertMetaType<ushort>();
    convertMetaType<ulong>();
    convertMetaType<unsigned long>();
    convertMetaType<uchar>();
    convertMetaType<unsigned char>();
    convertMetaType<char>();
    convertMetaType<uint>();
    convertMetaType<unsigned int>();
}

/*!
 These calls should not produce any warnings.
 */
void tst_QVariant::variantToDateTimeWithoutWarnings() const
{
    {
        const QVariant variant(QLatin1String("An invalid QDateTime string"));
        const QDateTime dateTime(variant.toDateTime());
        QVERIFY(!dateTime.isValid());
    }

    {
        QVariant v1(QLatin1String("xyz"));
        v1.convert(QVariant::DateTime);

        QVariant v2(QLatin1String("xyz"));
        QDateTime dt1(v2.toDateTime());

        const QVariant v3(QLatin1String("xyz"));
        const QDateTime dt2(v3.toDateTime());
    }
}

void tst_QVariant::invalidDateTime() const
{
    QVariant variant(QString::fromLatin1("Invalid date time string"));
    QVERIFY(!variant.toDateTime().isValid());
    QVERIFY(!variant.convert(QVariant::DateTime));
}

struct MyClass
{
    MyClass() : myValue(0) {}
    int myValue;
};

Q_DECLARE_METATYPE( MyClass )

void tst_QVariant::loadUnknownUserType()
{
    qRegisterMetaType<MyClass>("MyClass");
    char data[] = {0, 0, 0, 127, 0, 0, 0, 0, 8, 77, 121, 67, 108, 97, 115, 115, 0};

    QByteArray ba(data, sizeof(data));
    QDataStream ds(&ba, QIODevice::ReadOnly);
    QVariant var;
    var.load(ds);
    QCOMPARE(ds.status(), QDataStream::ReadCorruptData);
}

void tst_QVariant::loadBrokenUserType()
{
    char data[] = {0, 0, 0, 127, 0, 112 };

    QByteArray ba(data, sizeof(data));
    QDataStream ds(&ba, QIODevice::ReadOnly);
    QVariant var;
    var.load(ds);
    QCOMPARE(ds.status(), QDataStream::ReadPastEnd);
}

void tst_QVariant::task172061_invalidDate() const
{
    QString foo("Hello");
    QVariant variant(foo);
    QVERIFY(!variant.convert(QVariant::Date));

    variant = foo;
    QVERIFY(!variant.convert(QVariant::DateTime));

    variant = foo;
    QVERIFY(!variant.convert(QVariant::Time));

    variant = foo;
    QVERIFY(!variant.convert(QVariant::Int));

    variant = foo;
    QVERIFY(!variant.convert(QVariant::Double));

    variant = foo;
    QVERIFY(!variant.convert(QVariant::Type(QMetaType::Float)));
}

struct WontCompare
{
    int x;
};
Q_DECLARE_METATYPE(WontCompare);

void tst_QVariant::compareCustomTypes() const
{
    qRegisterMetaType<WontCompare>("WontCompare");

    WontCompare f1;
    f1.x = 0;
    const QVariant variant1(qVariantFromValue(f1));

    WontCompare f2;
    f2.x = 0;
    const QVariant variant2(qVariantFromValue(f2));

    /* We compare pointers. */
    QVERIFY(variant1 != variant2);
    QVERIFY(variant1 == variant1);
    QVERIFY(variant2 == variant2);
}

void tst_QVariant::timeToDateTime() const
{
    const QVariant val(QTime::currentTime());
    QVERIFY(!val.canConvert(QVariant::DateTime));
    QVERIFY(!val.toDateTime().isValid());
}

Q_DECLARE_METATYPE(QHostAddress)

void tst_QVariant::copyingUserTypes() const
{
    QVariant var;
    QVariant var3;
    const QHostAddress ha("127.0.0.1");
    qVariantSetValue(var, ha);
    var3 = var;

    QCOMPARE(qVariantValue<QHostAddress>(var3), ha);
}

void tst_QVariant::convertBoolToByteArray() const
{
    QFETCH(QByteArray, input);
    QFETCH(bool, canConvert);
    QFETCH(bool, value);

    const QVariant variant(input);

    QCOMPARE(qVariantCanConvert<bool>(variant), canConvert);

    if(canConvert) {
        /* Just call this function so we run the code path. */
        QCOMPARE(variant.toBool(), value);
    }
}

void tst_QVariant::convertBoolToByteArray_data() const
{
    QTest::addColumn<QByteArray>("input");
    QTest::addColumn<bool>("canConvert");
    QTest::addColumn<bool>("value");

    QTest::newRow("false")
        << QByteArray("false")
        << true
        << false;

    QTest::newRow("FALSE")
        << QByteArray("FALSE")
        << true
        << false;

    QTest::newRow("falSE")
        << QByteArray("FALSE")
        << true
        << false;

    QTest::newRow("")
        << QByteArray("")
        << true
        << false;

    QTest::newRow("null QByteArray")
        << QByteArray()
        << true
        << false;

    QTest::newRow("any-content")
        << QByteArray("any-content")
        << true
        << true;

    QTest::newRow("true")
        << QByteArray("true")
        << true
        << true;

    QTest::newRow("TRUE")
        << QByteArray("TRUE")
        << true
        << true;

    QTest::newRow("trUE")
        << QByteArray("trUE")
        << true
        << true;
}

void tst_QVariant::convertByteArrayToBool() const
{
    QFETCH(bool, input);
    QFETCH(QByteArray, output);

    const QVariant variant(input);
    QCOMPARE(variant.type(), QVariant::Bool);
    QCOMPARE(variant.toBool(), input);
    QVERIFY(qVariantCanConvert<bool>(variant));

    QCOMPARE(variant.toByteArray(), output);
}

void tst_QVariant::convertByteArrayToBool_data() const
{
    QTest::addColumn<bool>("input");
    QTest::addColumn<QByteArray>("output");

    QTest::newRow("false")
        << false
        << QByteArray("false");

    QTest::newRow("true")
        << true
        << QByteArray("true");
}

/*!
  We verify that:
    1. Converting the string "9.9" to int fails. This is the behavior of
       toLongLong() and hence also QVariant, since it uses it.
    2. Converting the QVariant containing the double 9.9 to int works.

  Rationale: "9.9" is not a valid int. However, doubles are by definition not
  ints and therefore it makes more sense to perform conversion for those.

  See task 237252.
 */
void tst_QVariant::toIntFromQString() const
{
    QVariant first("9.9");
    bool ok;
    QCOMPARE(first.toInt(&ok), 0);
    QVERIFY(!ok);

    QCOMPARE(QString("9.9").toLongLong(&ok), qlonglong(0));
    QVERIFY(!ok);

    QVariant v(9.9);
    QCOMPARE(v.toInt(&ok), 10);
    QVERIFY(ok);
}

/*!
  We verify that:
    1. Conversion from (64 bit) double to int works (no overflow).
    2. Same conversion works for QVariant::convert.

  Rationale: if 2147483630 is set in float and then converted to int,
  there will be overflow and the result will be -2147483648.

  See task 250267.
 */
void tst_QVariant::toIntFromDouble() const
{
    double d = 2147483630;  // max int 2147483647
    QVERIFY((int)d == 2147483630);

    QVariant var(d);
    QVERIFY( var.canConvert( QVariant::Int ) );

    bool ok;
    int result = var.toInt(&ok);

    QVERIFY( ok == true );
    QCOMPARE(result, 2147483630);
}

void tst_QVariant::task256984_setValue()
{
    QTransform t; //we just take a value so that we're sure that it will be shared
    QVariant v1 = t;
    QVERIFY( v1.isDetached() );
    QVariant v2 = v1;
    QVERIFY( !v1.isDetached() );
    QVERIFY( !v2.isDetached() );

    qVariantSetValue(v2, 3); //set an integer value

    QVERIFY( v1.isDetached() );
    QVERIFY( v2.isDetached() );
}

void tst_QVariant::numericalConvert()
{
    QVariant vfloat(float(5.3));
    QVariant vdouble(double(5.3));
    QVariant vreal(qreal(5.3));
    QVariant vint(int(5));
    QVariant vuint(uint(5));
    QVariant vshort(short(5));
    QVariant vlonglong(quint64(5));
    QVariant vstringint(QString::fromLatin1("5"));
    QVariant vstring(QString::fromLatin1("5.3"));

    QVector<QVariant *> vect;
    vect << &vfloat << &vdouble << &vreal << &vint << &vuint << &vshort<< &vlonglong << &vstringint << &vstring;

    for(int i = 0; i < vect.size(); i++) {
        double num = 5.3;
        if (i >= 3 && i <= 7)
            num = 5;
        QVariant *v = vect.at(i);
        QCOMPARE(v->toFloat() , float(num));
        QCOMPARE(float(v->toReal()) , float(num));
        QCOMPARE(float(v->toDouble()) , float(num));
        if(i != 8) {
            QCOMPARE(v->toInt() , int(num));
            QCOMPARE(v->toUInt() , uint(num));
            QCOMPARE(v->toULongLong() , quint64(num));
        }
        QCOMPARE(v->toString() , QString::number(num));
    }
}


template<class T> void playWithVariant(const T &orig, bool isNull, const QString &toString, double toDouble, bool toBool)
{
    QVariant v = QVariant::fromValue(orig);
    QVERIFY(v.isValid());
    QCOMPARE(v.isNull(), isNull);
    QCOMPARE(v.toString(), toString);
    QCOMPARE(v.toDouble(), toDouble);
    QCOMPARE(v.toBool(), toBool);
    QCOMPARE(qvariant_cast<T>(v), orig);

    {
        QVariant v2 = v;
        QCOMPARE(v2, v);
        QVERIFY(v2.isValid());
        QCOMPARE(v2.isNull(), isNull);
        QCOMPARE(v2.toString(), toString);
        QCOMPARE(v2.toDouble(), toDouble);
        QCOMPARE(v2.toBool(), toBool);
        QCOMPARE(qvariant_cast<T>(v2), orig);

        QVariant v3;
        v = QVariant();
        QCOMPARE(v3, v);
        v = v2;
        QCOMPARE(v, v2);
        QCOMPARE(qvariant_cast<T>(v2), qvariant_cast<T>(v));
        QCOMPARE(v2.toString(), toString);
        v3 = qVariantFromValue(orig);

        QVERIFY(v3.isValid());
        QCOMPARE(v3.isNull(), isNull);
        QCOMPARE(v3.toString(), toString);
        QCOMPARE(v3.toDouble(), toDouble);
        QCOMPARE(v3.toBool(), toBool);
        QCOMPARE(qvariant_cast<T>(v3), qvariant_cast<T>(v));
    }

    QVERIFY(v.isValid());
    QCOMPARE(v.isNull(), isNull);
    QCOMPARE(v.toString(), toString);
    QCOMPARE(v.toDouble(), toDouble);
    QCOMPARE(v.toBool(), toBool);
    QCOMPARE(qvariant_cast<T>(v), orig);

    if (qMetaTypeId<T>() != qMetaTypeId<QVariant>()) {
        QCOMPARE(v.userType(), qMetaTypeId<T>());
        QCOMPARE(QVariant::typeToName(QVariant::Type(v.userType())), QMetaType::typeName(qMetaTypeId<T>()));
    }
}


struct MyPrimitive
{
    char x, y;
    bool operator==(const MyPrimitive &o) const
    {
        return x == o.x && y == o.y;
    }
};

QT_BEGIN_NAMESPACE
Q_DECLARE_TYPEINFO(MyPrimitive, Q_PRIMITIVE_TYPE);
QT_END_NAMESPACE

struct MyData
{
    void *ptr;
    MyData() : ptr(this) {}
    ~MyData() { Q_ASSERT(ptr == this); }
    MyData(const MyData& o) : ptr(this) { Q_ASSERT(o.ptr == &o); }
    MyData &operator=(const MyData &o)
    {
        Q_ASSERT(ptr == this);
        Q_ASSERT(o.ptr == &o);
        return *this;
    }
    bool operator==(const MyData &o) const
    {
        Q_ASSERT(ptr == this);
        Q_ASSERT(o.ptr == &o);
        return true;
    }
};

struct MyMovable
{
    static int count;
    int v;
    MyMovable() { v = count++; }
    ~MyMovable() { count--; }
    MyMovable(const MyMovable &o) : v(o.v) { count++; }

    bool operator==(const MyMovable &o) const
    {
        return v == o.v;
    }
};

int MyMovable::count  = 0;


QT_BEGIN_NAMESPACE
Q_DECLARE_TYPEINFO(MyMovable, Q_MOVABLE_TYPE);
QT_END_NAMESPACE

Q_DECLARE_METATYPE(QList<QSize>)
Q_DECLARE_METATYPE(MyPrimitive)
Q_DECLARE_METATYPE(MyData)
Q_DECLARE_METATYPE(MyMovable)
Q_DECLARE_METATYPE(QList<MyPrimitive>)
Q_DECLARE_METATYPE(QList<MyData>)
Q_DECLARE_METATYPE(QList<MyMovable>)
Q_DECLARE_METATYPE(MyPrimitive *)
Q_DECLARE_METATYPE(MyData *)
Q_DECLARE_METATYPE(MyMovable *)


void tst_QVariant::moreCustomTypes()
{
    {
        QList<QSize> listSize;
        playWithVariant(listSize, false, QString(), 0, false);
        listSize << QSize(4,5) << QSize(89,23) << QSize(5,6);
        playWithVariant(listSize, false, QString(), 0, false);
    }

    {
        QString str;
        playWithVariant(str, true, QString(), 0, false);
        str = QString::fromLatin1("123456789.123");
        playWithVariant(str, false, str, 123456789.123, true);
    }

    {
        QSize size;
        playWithVariant(size, false, QString(), 0, false);
        playWithVariant(QSize(45,78), false, QString(), 0, false);
    }

    {
        MyData d;
        playWithVariant(d, false, QString(), 0, false);
        playWithVariant(&d, false, QString(), 0, false);
        QList<MyData> l;
        playWithVariant(l, false, QString(), 0, false);
        l << MyData() << MyData();
        playWithVariant(l, false, QString(), 0, false);
    }

    {
        MyPrimitive d = { 4, 5 };
        playWithVariant(d, false, QString(), 0, false);
        playWithVariant(&d, false, QString(), 0, false);
        QList<MyPrimitive> l;
        playWithVariant(l, false, QString(), 0, false);
        l << d;
        playWithVariant(l, false, QString(), 0, false);
    }

    {
        MyMovable d;
        playWithVariant(d, false, QString(), 0, false);
        playWithVariant(&d, false, QString(), 0, false);
        QList<MyMovable> l;
        playWithVariant(l, false, QString(), 0, false);
        l << MyMovable() << d;
        playWithVariant(l, false, QString(), 0, false);
    }
    QCOMPARE(MyMovable::count, 0);

    {
        playWithVariant(12.12, false, "12.12", 12.12, true);
        playWithVariant(12.12f, false, "12.12", 12.12f, true);
        playWithVariant('a', false, "a", 'a', true);
        playWithVariant((unsigned char)('a'), false, "a", 'a', true);
        playWithVariant( quint8(12), false, "\xc", 12, true);
        playWithVariant(  qint8(13), false, "\xd", 13, true);
        playWithVariant(quint16(14), false, "14", 14, true);
        playWithVariant( qint16(15), false, "15", 15, true);
        playWithVariant(quint32(16), false, "16", 16, true);
        playWithVariant( qint32(17), false, "17", 17, true);
        playWithVariant(quint64(18), false, "18", 18, true);
        playWithVariant( qint64(19), false, "19", 19, true);
        playWithVariant(  qint8(-12), false, "\xf4", -12, true);
        playWithVariant( qint16(-13), false, "-13", -13, true);
        playWithVariant( qint32(-14), false, "-14", -14, true);
        playWithVariant( qint64(-15), false, "-15", -15, true);
        playWithVariant(quint64(0), false, "0", 0, false);
        playWithVariant( true, false, "true", 1, true);
        playWithVariant( false, false, "false", 0, false);

        playWithVariant(QString("hello\n"), false, "hello\n", 0, true);
    }

    {
        int i = 5;
        playWithVariant((void *)(&i), false, QString(), 0, false);
        playWithVariant((void *)(0), false, QString(), 0, false);
    }

    {
        QVariant v1 = QVariant::fromValue(5);
        QVariant v2 = QVariant::fromValue(5.0);
        QVariant v3 = QVariant::fromValue(quint16(5));
        QVariant v4 = 5;
        QVariant v5 = QVariant::fromValue(MyPrimitive());
        QVariant v6 = QVariant::fromValue(MyMovable());
        QVariant v7 = QVariant::fromValue(MyData());
        playWithVariant(v1, false, "5", 5, true);
        playWithVariant(v2, false, "5", 5, true);
        playWithVariant(v3, false, "5", 5, true);
        playWithVariant(v4, false, "5", 5, true);

        playWithVariant(v5, false, QString(), 0, false);
    }
}


void tst_QVariant::variantInVariant()
{
    QVariant var1 = 5;
    QCOMPARE(var1.type(), QVariant::Int);
    QVariant var2 = var1;
    QCOMPARE(var2, var1);
    QCOMPARE(var2.type(), QVariant::Int);
    QVariant var3 = QVariant::fromValue(var1);
    QCOMPARE(var3, var1);
    QCOMPARE(var3.type(), QVariant::Int);
    QVariant var4 = qvariant_cast<QVariant>(var1);
    QCOMPARE(var4, var1);
    QCOMPARE(var4.type(), QVariant::Int);
    QVariant var5;
    var5 = var1;
    QCOMPARE(var5, var1);
    QCOMPARE(var5.type(), QVariant::Int);
    QVariant var6;
    var6.setValue(var1);
    QCOMPARE(var6, var1);
    QCOMPARE(var6.type(), QVariant::Int);

    QCOMPARE(QVariant::fromValue(var1), QVariant::fromValue(var2));
    QCOMPARE(qvariant_cast<QVariant>(var3), QVariant::fromValue(var4));
    QCOMPARE(qvariant_cast<QVariant>(var5), qvariant_cast<QVariant>(var6));

    QString str("hello");
    QVariant var8 = qvariant_cast<QVariant>(QVariant::fromValue(QVariant::fromValue(str)));
    QCOMPARE((int)var8.type(), (int)QVariant::String);
    QCOMPARE(qvariant_cast<QString>(QVariant(qvariant_cast<QVariant>(var8))), str);

    QVariant var9(qMetaTypeId<QVariant>(), &var1);
    QCOMPARE(var9.userType(), qMetaTypeId<QVariant>());
    QCOMPARE(qvariant_cast<QVariant>(var9), var1);
}

void tst_QVariant::colorInteger()
{
    QVariant v = QColor(Qt::red);
    QCOMPARE(v.type(), QVariant::Color);
    QCOMPARE(v.value<QColor>(), QColor(Qt::red));

    v.setValue(1000);
    QCOMPARE(v.type(), QVariant::Int);
    QCOMPARE(v.toInt(), 1000);

    v.setValue(QColor(Qt::yellow));
    QCOMPARE(v.type(), QVariant::Color);
    QCOMPARE(v.value<QColor>(), QColor(Qt::yellow));
}

QTEST_MAIN(tst_QVariant)
#include "tst_qvariant.moc"
