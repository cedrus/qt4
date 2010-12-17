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

#include <QBuffer>
#include <QDebug>
#include <QFile>
#include <QImage>
#include <QImageReader>
#include <QImageWriter>
#include <QLabel>
#include <QPixmap>
#include <QSet>
#include <QTcpSocket>
#include <QTcpServer>
#include <QTimer>

#if defined(Q_OS_SYMBIAN)
# define SRCDIR "."
#endif

typedef QMap<QString, QString> QStringMap;
typedef QList<int> QIntList;
Q_DECLARE_METATYPE(QImage)
Q_DECLARE_METATYPE(QRect)
Q_DECLARE_METATYPE(QSize)
Q_DECLARE_METATYPE(QColor)
Q_DECLARE_METATYPE(QStringMap)
Q_DECLARE_METATYPE(QIntList)
Q_DECLARE_METATYPE(QIODevice *)
Q_DECLARE_METATYPE(QImage::Format)

//TESTED_FILES=

class tst_QImageReader : public QObject
{
    Q_OBJECT

public:
    tst_QImageReader();
    virtual ~tst_QImageReader();

public slots:
    void init();
    void cleanup();

private slots:
    void getSetCheck();
    void readImage_data();
    void readImage();
    void jpegRgbCmyk();

    void setScaledSize_data();
    void setScaledSize();

    void setClipRect_data();
    void setClipRect();

    void setScaledClipRect_data();
    void setScaledClipRect();

    void imageFormat_data();
    void imageFormat();

    void blackXPM();
    void transparentXPM();
    void multiWordNamedColorXPM();

    void supportedFormats();

    void readFromDevice_data();
    void readFromDevice();

    void readFromFileAfterJunk_data();
    void readFromFileAfterJunk();

    void devicePosition_data();
    void devicePosition();

    void setBackgroundColor_data();
    void setBackgroundColor();

    void supportsAnimation_data();
    void supportsAnimation();

    void readFromResources_data();
    void readFromResources();

    void dotsPerMeter_data();
    void dotsPerMeter();

    void physicalDpi_data();
    void physicalDpi();

    void sizeBeforeRead_data();
    void sizeBeforeRead();

    void sizeBeforeFormat_data();
    void sizeBeforeFormat();

    void imageFormatBeforeRead_data();
    void imageFormatBeforeRead();

#if defined QTEST_HAVE_GIF
    void gifHandlerBugs();
    void animatedGif();
    void gifImageCount();
    void gifLoopCount();
#endif

    void readCorruptImage_data();
    void readCorruptImage();
    void readCorruptBmp();

    void supportsOption_data();
    void supportsOption();

#if defined QTEST_HAVE_TIFF
    void tiffCompression_data();
    void tiffCompression();
    void tiffEndianness();

    void tiffOrientation_data();
    void tiffOrientation();

    void tiffGrayscale();
#endif

    void autoDetectImageFormat();
    void fileNameProbing();

    void pixelCompareWithBaseline_data();
    void pixelCompareWithBaseline();

    void task255627_setNullScaledSize_data();
    void task255627_setNullScaledSize();

    void testIgnoresFormatAndExtension_data();
    void testIgnoresFormatAndExtension();

};

static const QLatin1String prefix(SRCDIR "/images/");

// Testing get/set functions
void tst_QImageReader::getSetCheck()
{
    QImageReader obj1;
    // QIODevice * QImageReader::device()
    // void QImageReader::setDevice(QIODevice *)
    QFile *var1 = new QFile;
    obj1.setDevice(var1);

    //A bit strange but that's the only way to compile under windows.
    QCOMPARE((QIODevice *) var1, obj1.device());
    obj1.setDevice((QIODevice *)0);
    QCOMPARE((QIODevice *) 0,
             obj1.device());
    delete var1;
}

tst_QImageReader::tst_QImageReader()
{
}

tst_QImageReader::~tst_QImageReader()
{

}

void tst_QImageReader::init()
{
}

void tst_QImageReader::cleanup()
{
}

void tst_QImageReader::readImage_data()
{
    QTest::addColumn<QString>("fileName");
    QTest::addColumn<bool>("success");
    QTest::addColumn<QByteArray>("format");

    QTest::newRow("empty") << QString() << false << QByteArray();
    QTest::newRow("BMP: colorful") << QString("colorful.bmp") << true << QByteArray("bmp");
    QTest::newRow("BMP: font") << QString("font.bmp") << true << QByteArray("bmp");
    QTest::newRow("BMP: signed char") << QString("crash-signed-char.bmp") << true << QByteArray("bmp");
    QTest::newRow("BMP: 4bpp RLE") << QString("4bpp-rle.bmp") << true << QByteArray("bmp");
    QTest::newRow("BMP: 4bpp uncompressed") << QString("tst7.bmp") << true << QByteArray("bmp");
    QTest::newRow("BMP: 16bpp") << QString("16bpp.bmp") << true << QByteArray("bmp");
    QTest::newRow("BMP: negative height") << QString("negativeheight.bmp") << true << QByteArray("bmp");
    QTest::newRow("XPM: marble") << QString("marble.xpm") << true << QByteArray("xpm");
    QTest::newRow("PNG: kollada") << QString("kollada.png") << true << QByteArray("png");
    QTest::newRow("PPM: teapot") << QString("teapot.ppm") << true << QByteArray("ppm");
    QTest::newRow("PPM: runners") << QString("runners.ppm") << true << QByteArray("ppm");
    QTest::newRow("PPM: test") << QString("test.ppm") << true << QByteArray("ppm");
    QTest::newRow("XBM: gnus") << QString("gnus.xbm") << true << QByteArray("xbm");

#if defined QTEST_HAVE_JPEG
    QTest::newRow("JPEG: beavis") << QString("beavis.jpg") << true << QByteArray("jpeg");
    QTest::newRow("JPEG: qtbug13653") << QString("qtbug13653-no_eoi.jpg") << true << QByteArray("jpeg");
#endif
#if defined QTEST_HAVE_GIF
    QTest::newRow("GIF: earth") << QString("earth.gif") << true << QByteArray("gif");
    QTest::newRow("GIF: trolltech") << QString("trolltech.gif") << true << QByteArray("gif");
#endif
#if defined QTEST_HAVE_MNG
    QTest::newRow("MNG: ball") << QString("ball.mng") << true << QByteArray("mng");
    QTest::newRow("MNG: fire") << QString("fire.mng") << true << QByteArray("mng");
#endif
#if defined QTEST_HAVE_SVG
    QTest::newRow("SVG: rect") << QString("rect.svg") << true << QByteArray("svg");
    QTest::newRow("SVGZ: rect") << QString("rect.svgz") << true << QByteArray("svgz");
#endif
}

void tst_QImageReader::readImage()
{
    QFETCH(QString, fileName);
    QFETCH(bool, success);
    QFETCH(QByteArray, format);

    for (int i = 0; i < 2; ++i) {
        QImageReader io(prefix + fileName, i ? QByteArray() : format);
        if (success) {
            if (!io.supportsAnimation())
                QVERIFY(io.imageCount() > 0);
        } else {
            QCOMPARE(io.imageCount(), -1);
        }
        QImage image = io.read();
        if (!success) {
            QVERIFY(image.isNull());
            return;
        }

        QVERIFY2(!image.isNull(), io.errorString().toLatin1().constData());

        // No format
        QImageReader io2(prefix + fileName);
        QVERIFY2(!io2.read().isNull(), io.errorString().toLatin1().constData());

        // No extension, no format
        QImageReader io3(prefix + fileName.left(fileName.lastIndexOf(QLatin1Char('.'))));
        QVERIFY2(!io3.read().isNull(), io.errorString().toLatin1().constData());

        // Read into \a image2
        QImage image2;
        QImageReader image2Reader(prefix + fileName, i ? QByteArray() : format);
        QCOMPARE(image2Reader.format(), format);
        QVERIFY(image2Reader.read(&image2));
        if (image2Reader.canRead()) {
            if (i)
                QVERIFY(!image2Reader.format().isEmpty());
            else
                QCOMPARE(image2Reader.format(), format);
        } else {
            if (i)
                QVERIFY(image2Reader.format().isEmpty());
            else
                QVERIFY(!image2Reader.format().isEmpty());
        }
        QCOMPARE(image, image2);
        do {
            QVERIFY2(!image.isNull(), io.errorString().toLatin1().constData());
        } while (!(image = io.read()).isNull());
    }
}

void tst_QImageReader::jpegRgbCmyk()
{
    QImage image1(prefix + QLatin1String("YCbCr_cmyk.jpg"));
    QImage image2(prefix + QLatin1String("YCbCr_cmyk.png"));

    // first, do some obvious tests
    QCOMPARE(image1.height(), image2.height());
    QCOMPARE(image1.width(), image2.width());
    QCOMPARE(image1.format(), image2.format());
    QCOMPARE(image1.format(), QImage::Format_RGB32);

    // compare all the pixels with a slack of 3. This ignores rounding errors in libjpeg/libpng
    for (int h = 0; h < image1.height(); ++h) {
        const uchar *s1 = image1.constScanLine(h);
        const uchar *s2 = image2.constScanLine(h);
        for (int w = 0; w < image1.width() * 4; ++w) {
            if (*s1 != *s2) {
                QVERIFY2(qAbs(*s1 - *s2) <= 3, qPrintable(QString("images differ in line %1, col %2 (image1: %3, image2: %4)").arg(h).arg(w).arg(*s1, 0, 16).arg(*s2, 0, 16)));
            }
            s1++;
            s2++;
        }
    }
}

void tst_QImageReader::setScaledSize_data()
{
    QTest::addColumn<QString>("fileName");
    QTest::addColumn<QSize>("newSize");
    QTest::addColumn<QByteArray>("format");

    QTest::newRow("BMP: colorful") << "colorful" << QSize(200, 200) << QByteArray("bmp");
    QTest::newRow("BMP: font") << "font" << QSize(200, 200) << QByteArray("bmp");
    QTest::newRow("XPM: marble") << "marble" << QSize(200, 200) << QByteArray("xpm");
    QTest::newRow("PNG: kollada") << "kollada" << QSize(200, 200) << QByteArray("png");
    QTest::newRow("PPM: teapot") << "teapot" << QSize(200, 200) << QByteArray("ppm");
    QTest::newRow("PPM: runners") << "runners.ppm" << QSize(400, 400) << QByteArray("ppm");
    QTest::newRow("PPM: test") << "test.ppm" << QSize(10, 10) << QByteArray("ppm");
    QTest::newRow("XBM: gnus") << "gnus" << QSize(200, 200) << QByteArray("xbm");
#ifdef QTEST_HAVE_JPEG
    QTest::newRow("JPEG: beavis A") << "beavis" << QSize(200, 200) << QByteArray("jpeg");
    QTest::newRow("JPEG: beavis B") << "beavis" << QSize(175, 175) << QByteArray("jpeg");
    QTest::newRow("JPEG: beavis C") << "beavis" << QSize(100, 100) << QByteArray("jpeg");
    QTest::newRow("JPEG: beavis D") << "beavis" << QSize(100, 200) << QByteArray("jpeg");
    QTest::newRow("JPEG: beavis E") << "beavis" << QSize(200, 100) << QByteArray("jpeg");
    QTest::newRow("JPEG: beavis F") << "beavis" << QSize(87, 87) << QByteArray("jpeg");
    QTest::newRow("JPEG: beavis G") << "beavis" << QSize(50, 45) << QByteArray("jpeg");
    QTest::newRow("JPEG: beavis H") << "beavis" << QSize(43, 43) << QByteArray("jpeg");
    QTest::newRow("JPEG: beavis I") << "beavis" << QSize(25, 25) << QByteArray("jpeg");
#endif // QTEST_HAVE_JPEG
#ifdef QTEST_HAVE_GIF
    QTest::newRow("GIF: earth") << "earth" << QSize(200, 200) << QByteArray("gif");
    QTest::newRow("GIF: trolltech") << "trolltech" << QSize(200, 200) << QByteArray("gif");
#endif // QTEST_HAVE_GIF
#ifdef QTEST_HAVE_MNG
    QTest::newRow("MNG: ball") << "ball" << QSize(200, 200) << QByteArray("mng");
    QTest::newRow("MNG: fire") << "fire" << QSize(200, 200) << QByteArray("mng");
#endif // QTEST_HAVE_MNG
#if defined QTEST_HAVE_SVG
    QTest::newRow("SVG: rect") << "rect" << QSize(200, 200) << QByteArray("svg");
    QTest::newRow("SVGZ: rect") << "rect" << QSize(200, 200) << QByteArray("svgz");
#endif
}

void tst_QImageReader::setScaledSize()
{
    QFETCH(QString, fileName);
    QFETCH(QSize, newSize);
    QFETCH(QByteArray, format);

    if (!format.isEmpty() && !QImageReader::supportedImageFormats().contains(format))
        QSKIP("Qt does not support reading the \"" + format + "\" format", SkipSingle);

    QImageReader reader(prefix + fileName);
    reader.setScaledSize(newSize);
    QImage image = reader.read();
    QVERIFY(!image.isNull());

    QCOMPARE(image.size(), newSize);
}

void tst_QImageReader::task255627_setNullScaledSize_data()
{
    setScaledSize_data();
}

void tst_QImageReader::task255627_setNullScaledSize()
{
    QFETCH(QString, fileName);
    QFETCH(QByteArray, format);

    if (!format.isEmpty() && !QImageReader::supportedImageFormats().contains(format))
        QSKIP("Qt does not support reading the \"" + format + "\" format", SkipSingle);

    QImageReader reader(prefix + fileName);

    // set a null size
    reader.setScaledSize(QSize(0, 0));
    reader.setQuality(0);
    QImage image = reader.read();
    QVERIFY(image.isNull());
    QCOMPARE(image.size(), QSize(0, 0));
}

void tst_QImageReader::setClipRect_data()
{
    QTest::addColumn<QString>("fileName");
    QTest::addColumn<QRect>("newRect");
    QTest::addColumn<QByteArray>("format");
    QTest::newRow("BMP: colorful") << "colorful" << QRect(0, 0, 50, 50) << QByteArray("bmp");
    QTest::newRow("BMP: font") << "font" << QRect(0, 0, 50, 50) << QByteArray("bmp");
    QTest::newRow("BMP: 4bpp uncompressed") << "tst7.bmp" << QRect(0, 0, 31, 31) << QByteArray("bmp");
    QTest::newRow("XPM: marble") << "marble" << QRect(0, 0, 50, 50) << QByteArray("xpm");
    QTest::newRow("PNG: kollada") << "kollada" << QRect(0, 0, 50, 50) << QByteArray("png");
    QTest::newRow("PPM: teapot") << "teapot" << QRect(0, 0, 50, 50) << QByteArray("ppm");
    QTest::newRow("PPM: runners") << "runners.ppm" << QRect(0, 0, 50, 50) << QByteArray("ppm");
    QTest::newRow("PPM: test") << "test.ppm" << QRect(0, 0, 50, 50) << QByteArray("ppm");
    QTest::newRow("XBM: gnus") << "gnus" << QRect(0, 0, 50, 50) << QByteArray("xbm");
#ifdef QTEST_HAVE_JPEG
    QTest::newRow("JPEG: beavis") << "beavis" << QRect(0, 0, 50, 50) << QByteArray("jpeg");
#endif // QTEST_HAVE_JPEG
#ifdef QTEST_HAVE_GIF
    QTest::newRow("GIF: earth") << "earth" << QRect(0, 0, 50, 50) << QByteArray("gif");
    QTest::newRow("GIF: trolltech") << "trolltech" << QRect(0, 0, 50, 50) << QByteArray("gif");
#endif // QTEST_HAVE_GIF
#ifdef QTEST_HAVE_MNG
    QTest::newRow("MNG: ball") << "ball" << QRect(0, 0, 50, 50) << QByteArray("mng");
    QTest::newRow("MNG: fire") << "fire" << QRect(0, 0, 50, 50) << QByteArray("mng");
#endif // QTEST_HAVE_MNG
#if defined QTEST_HAVE_SVG
    QTest::newRow("SVG: rect") << "rect" << QRect(0, 0, 50, 50) << QByteArray("svg");
    QTest::newRow("SVGZ: rect") << "rect" << QRect(0, 0, 50, 50) << QByteArray("svgz");
#endif
}

void tst_QImageReader::setClipRect()
{
    QFETCH(QString, fileName);
    QFETCH(QRect, newRect);
    QFETCH(QByteArray, format);

    if (!format.isEmpty() && !QImageReader::supportedImageFormats().contains(format))
        QSKIP("Qt does not support reading the \"" + format + "\" format", SkipSingle);

    QImageReader reader(prefix + fileName);
    reader.setClipRect(newRect);
    QImage image = reader.read();
    QVERIFY(!image.isNull());
    QCOMPARE(image.rect(), newRect);

    QImageReader originalReader(prefix + fileName);
    QImage originalImage = originalReader.read();
    QCOMPARE(originalImage.copy(newRect), image);
}

void tst_QImageReader::setScaledClipRect_data()
{
    QTest::addColumn<QString>("fileName");
    QTest::addColumn<QRect>("newRect");
    QTest::addColumn<QByteArray>("format");

    QTest::newRow("BMP: colorful") << "colorful" << QRect(0, 0, 50, 50) << QByteArray("bmp");
    QTest::newRow("BMP: font") << "font" << QRect(0, 0, 50, 50) << QByteArray("bmp");
    QTest::newRow("XPM: marble") << "marble" << QRect(0, 0, 50, 50) << QByteArray("xpm");
    QTest::newRow("PNG: kollada") << "kollada" << QRect(0, 0, 50, 50) << QByteArray("png");
    QTest::newRow("PPM: teapot") << "teapot" << QRect(0, 0, 50, 50) << QByteArray("ppm");
    QTest::newRow("PPM: runners") << "runners.ppm" << QRect(0, 0, 50, 50) << QByteArray("ppm");
    QTest::newRow("PPM: test") << "test.ppm" << QRect(0, 0, 50, 50) << QByteArray("ppm");
    QTest::newRow("XBM: gnus") << "gnus" << QRect(0, 0, 50, 50) << QByteArray("xbm");
#ifdef QTEST_HAVE_JPEG
    QTest::newRow("JPEG: beavis") << "beavis" << QRect(0, 0, 50, 50) << QByteArray("jpeg");
#endif // QTEST_HAVE_JPEG
#ifdef QTEST_HAVE_GIF
    QTest::newRow("GIF: earth") << "earth" << QRect(0, 0, 50, 50) << QByteArray("gif");
    QTest::newRow("GIF: trolltech") << "trolltech" << QRect(0, 0, 50, 50) << QByteArray("gif");
#endif // QTEST_HAVE_GIF
#ifdef QTEST_HAVE_MNG
    QTest::newRow("MNG: ball") << "ball" << QRect(0, 0, 50, 50) << QByteArray("mng");
    QTest::newRow("MNG: fire") << "fire" << QRect(0, 0, 50, 50) << QByteArray("mng");
#endif // QTEST_HAVE_MNG
#if defined QTEST_HAVE_SVG
    QTest::newRow("SVG: rect") << "rect" << QRect(0, 0, 50, 50) << QByteArray("svg");
    QTest::newRow("SVGZ: rect") << "rect" << QRect(0, 0, 50, 50) << QByteArray("svgz");
#endif
}

void tst_QImageReader::setScaledClipRect()
{
    QFETCH(QString, fileName);
    QFETCH(QRect, newRect);
    QFETCH(QByteArray, format);

    if (!format.isEmpty() && !QImageReader::supportedImageFormats().contains(format))
        QSKIP("Qt does not support reading the \"" + format + "\" format", SkipSingle);

    QImageReader reader(prefix + fileName);
    reader.setScaledSize(QSize(300, 300));
    reader.setScaledClipRect(newRect);
    QImage image = reader.read();
    QVERIFY(!image.isNull());
    QCOMPARE(image.rect(), newRect);

    QImageReader originalReader(prefix + fileName);
    originalReader.setScaledSize(QSize(300, 300));
    QImage originalImage = originalReader.read();
    QCOMPARE(originalImage.copy(newRect), image);
}

void tst_QImageReader::imageFormat_data()
{
    QTest::addColumn<QString>("fileName");
    QTest::addColumn<QByteArray>("format");
    QTest::addColumn<QImage::Format>("imageFormat");

    QTest::newRow("pbm") << QString("image.pbm") << QByteArray("pbm") << QImage::Format_Mono;
    QTest::newRow("pgm") << QString("image.pgm") << QByteArray("pgm") << QImage::Format_Indexed8;
    QTest::newRow("ppm-1") << QString("image.ppm") << QByteArray("ppm") << QImage::Format_RGB32;
    QTest::newRow("ppm-2") << QString("teapot.ppm") << QByteArray("ppm") << QImage::Format_RGB32;
    QTest::newRow("ppm-3") << QString("runners.ppm") << QByteArray("ppm") << QImage::Format_RGB32;
    QTest::newRow("ppm-4") << QString("test.ppm") << QByteArray("ppm") << QImage::Format_RGB32;
#ifdef QTEST_HAVE_JPEG
    QTest::newRow("jpeg-1") << QString("beavis.jpg") << QByteArray("jpeg") << QImage::Format_Indexed8;
    QTest::newRow("jpeg-2") << QString("YCbCr_cmyk.jpg") << QByteArray("jpeg") << QImage::Format_RGB32;
    QTest::newRow("jpeg-3") << QString("YCbCr_rgb.jpg") << QByteArray("jpeg") << QImage::Format_RGB32;
#endif
#if defined QTEST_HAVE_GIF
    QTest::newRow("gif-1") << QString("earth.gif") << QByteArray("gif") << QImage::Format_Invalid;
    QTest::newRow("gif-2") << QString("trolltech.gif") << QByteArray("gif") << QImage::Format_Invalid;
#endif
    QTest::newRow("xbm") << QString("gnus.xbm") << QByteArray("xbm") << QImage::Format_MonoLSB;
    QTest::newRow("xpm") << QString("marble.xpm") << QByteArray("xpm") << QImage::Format_Indexed8;
    QTest::newRow("bmp-1") << QString("colorful.bmp") << QByteArray("bmp") << QImage::Format_Indexed8;
    QTest::newRow("bmp-2") << QString("font.bmp") << QByteArray("bmp") << QImage::Format_Indexed8;
    QTest::newRow("png") << QString("kollada.png") << QByteArray("png") << QImage::Format_ARGB32;
    QTest::newRow("png-2") << QString("YCbCr_cmyk.png") << QByteArray("png") << QImage::Format_RGB32;
    QTest::newRow("mng-1") << QString("ball.mng") << QByteArray("mng") << QImage::Format_Invalid;
    QTest::newRow("mng-2") << QString("fire.mng") << QByteArray("mng") << QImage::Format_Invalid;
    QTest::newRow("svg") << QString("rect.svg") << QByteArray("svg") << QImage::Format_ARGB32_Premultiplied;
    QTest::newRow("svgz") << QString("rect.svgz") << QByteArray("svgz") << QImage::Format_ARGB32_Premultiplied;
}

void tst_QImageReader::imageFormat()
{
    QFETCH(QString, fileName);
    QFETCH(QByteArray, format);
    QFETCH(QImage::Format, imageFormat);

    if (QImageReader::imageFormat(prefix + fileName).isEmpty()) {
        if (QByteArray("jpeg") == format)
#ifndef QTEST_HAVE_JPEG
            return;
#endif // !QTEST_HAVE_JPEG
        if (QByteArray("gif") == format)
#ifndef QTEST_HAVE_GIF
            return;
#endif // !QTEST_HAVE_GIF
        if (QByteArray("mng") == format)
#ifndef QTEST_HAVE_MNG
            return;
#endif // !QTEST_HAVE_MNG
        if (QByteArray("svg") == format || QByteArray("svgz") == format)
#ifndef QTEST_HAVE_SVG
            return;
#endif // !QTEST_HAVE_SVG
        QSKIP(("Qt does not support the " + format + " format.").constData(), SkipSingle);
    } else {
        QCOMPARE(QImageReader::imageFormat(prefix + fileName), format);
    }
    QImageReader reader(prefix + fileName);
    QCOMPARE(reader.imageFormat(), imageFormat);
}

void tst_QImageReader::blackXPM()
{
    QImage image(prefix + QLatin1String("black.xpm"));
    QImage image2(prefix + QLatin1String("black.png"));
    QCOMPARE(image.pixel(25, 25), qRgb(190, 190, 190));
    QCOMPARE(image.pixel(25, 25), image2.pixel(25, 25));
}

void tst_QImageReader::transparentXPM()
{
    QImage image(prefix + QLatin1String("nontransparent.xpm"));
    QImage image2(prefix + QLatin1String("transparent.xpm"));
    QCOMPARE(image.format(), QImage::Format_RGB32);
    QCOMPARE(image2.format(), QImage::Format_ARGB32);
}

void tst_QImageReader::multiWordNamedColorXPM()
{
    QImage image(prefix + QLatin1String("namedcolors.xpm"));
    QCOMPARE(image.pixel(0, 0), qRgb(102, 139, 139)); // pale turquoise 4
    QCOMPARE(image.pixel(0, 1), qRgb(250, 250, 210)); // light golden rod yellow
    QCOMPARE(image.pixel(0, 2), qRgb(255, 250, 205)); // lemon chiffon
}

void tst_QImageReader::supportedFormats()
{
    QList<QByteArray> formats = QImageReader::supportedImageFormats();
    QList<QByteArray> sortedFormats = formats;
    qSort(sortedFormats);

    // check that the list is sorted
    QCOMPARE(formats, sortedFormats);

    QSet<QByteArray> formatSet;
    foreach (QByteArray format, formats)
        formatSet << format;

    // check that the list does not contain duplicates
    QCOMPARE(formatSet.size(), formats.size());
}

void tst_QImageReader::setBackgroundColor_data()
{
    QTest::addColumn<QString>("fileName");
    QTest::addColumn<QColor>("color");

    QTest::newRow("BMP: colorful") << QString("colorful.bmp") << QColor(Qt::white);
    QTest::newRow("BMP: font") << QString("font.bmp") << QColor(Qt::black);
    QTest::newRow("BMP: signed char") << QString("crash-signed-char.bmp") << QColor(Qt::red);
    QTest::newRow("XPM: marble") << QString("marble.xpm") << QColor(Qt::darkRed);
    QTest::newRow("PNG: kollada") << QString("kollada.png") << QColor(Qt::green);
    QTest::newRow("PPM: teapot") << QString("teapot.ppm") << QColor(Qt::darkGreen);
    QTest::newRow("PPM: runners") << QString("runners.ppm") << QColor(Qt::red);
    QTest::newRow("PPM: test") << QString("test.ppm") << QColor(Qt::white);
    QTest::newRow("XBM: gnus") << QString("gnus.xbm") << QColor(Qt::blue);
#if defined QTEST_HAVE_JPEG
    QTest::newRow("JPEG: beavis") << QString("beavis.jpg") << QColor(Qt::darkBlue);
#endif
#if defined QTEST_HAVE_GIF
    QTest::newRow("GIF: earth") << QString("earth.gif") << QColor(Qt::cyan);
    QTest::newRow("GIF: trolltech") << QString("trolltech.gif") << QColor(Qt::magenta);
#endif
#if defined QTEST_HAVE_MNG
    QTest::newRow("MNG: ball") << QString("ball.mng") << QColor(Qt::yellow);
    QTest::newRow("MNG: fire") << QString("fire.mng") << QColor(Qt::gray);
#endif
#if defined QTEST_HAVE_SVG
    QTest::newRow("SVG: rect") << QString("rect.svg") << QColor(Qt::darkGreen);
    QTest::newRow("SVGZ: rect") << QString("rect.svgz") << QColor(Qt::darkGreen);
#endif
}

void tst_QImageReader::setBackgroundColor()
{
    QFETCH(QString, fileName);
    QFETCH(QColor, color);
    QImageReader io("images/" + fileName);
    io.setBackgroundColor(color);
    if (io.backgroundColor().isValid())
        QCOMPARE(io.backgroundColor(), color);
}

void tst_QImageReader::supportsAnimation_data()
{
    QTest::addColumn<QString>("fileName");
    QTest::addColumn<bool>("success");

    QTest::newRow("BMP: colorful") << QString("colorful.bmp") << false;
    QTest::newRow("BMP: font") << QString("font.bmp") << false;
    QTest::newRow("BMP: signed char") << QString("crash-signed-char.bmp") << false;
    QTest::newRow("XPM: marble") << QString("marble.xpm") << false;
    QTest::newRow("PNG: kollada") << QString("kollada.png") << false;
    QTest::newRow("PPM: teapot") << QString("teapot.ppm") << false;
    QTest::newRow("PPM: teapot") << QString("teapot.ppm") << false;
    QTest::newRow("PPM: runners") << QString("runners.ppm") << false;
    QTest::newRow("XBM: gnus") << QString("gnus.xbm") << false;
#if defined QTEST_HAVE_JPEG
    QTest::newRow("JPEG: beavis") << QString("beavis.jpg") << false;
#endif
#if defined QTEST_HAVE_GIF
    QTest::newRow("GIF: earth") << QString("earth.gif") << true;
    QTest::newRow("GIF: trolltech") << QString("trolltech.gif") << true;
#endif
#if defined QTEST_HAVE_MNG
    QTest::newRow("MNG: ball") << QString("ball.mng") << true;
    QTest::newRow("MNG: fire") << QString("fire.mng") << true;
#endif
#if defined QTEST_HAVE_SVG
    QTest::newRow("SVG: rect") << QString("rect.svg") << false;
    QTest::newRow("SVGZ: rect") << QString("rect.svgz") << false;
#endif
}

void tst_QImageReader::supportsAnimation()
{
    QFETCH(QString, fileName);
    QFETCH(bool, success);
    QImageReader io(prefix + fileName);
    QCOMPARE(io.supportsAnimation(), success);
}

void tst_QImageReader::sizeBeforeRead_data()
{
    imageFormat_data();
}

void tst_QImageReader::sizeBeforeRead()
{
    QFETCH(QString, fileName);
    QFETCH(QByteArray, format);
    QImageReader reader(prefix + fileName);
    QVERIFY(reader.canRead());
    if (format == "mng") {
        QCOMPARE(reader.size(), QSize());
        return;
    }

    QSize size = reader.size();
    QVERIFY(reader.canRead());
    QImage image = reader.read();
    QVERIFY(!image.isNull());
    QCOMPARE(size, image.size());
}

void tst_QImageReader::sizeBeforeFormat_data()
{
    imageFormat_data();
}

void tst_QImageReader::sizeBeforeFormat()
{
    QFETCH(QString, fileName);

    QByteArray formatA, formatB;

    {
        QImageReader reader(prefix + fileName);
        formatA = reader.format();
    }

    {
        QImageReader reader(prefix + fileName);
        QSize size = reader.size();
        formatB = reader.format();
    }

    QCOMPARE(formatA, formatB);
}

void tst_QImageReader::imageFormatBeforeRead_data()
{
    imageFormat_data();
}

void tst_QImageReader::imageFormatBeforeRead()
{
    QFETCH(QString, fileName);
    QFETCH(QImage::Format, imageFormat);
    QImageReader reader(fileName);
    if (reader.supportsOption(QImageIOHandler::ImageFormat)) {
        QImage::Format fileFormat = reader.imageFormat();
        QCOMPARE(fileFormat, imageFormat);
        QSize size = reader.size();
        QImage image(size, fileFormat);
        QVERIFY(reader.read(&image));
        QCOMPARE(image.format(), fileFormat);
    }
}

#if defined QTEST_HAVE_GIF
void tst_QImageReader::gifHandlerBugs()
{
    {
        QImageReader io(prefix + "trolltech.gif");
        QVERIFY(io.loopCount() != 1);
        int count=0;
        for (; io.canRead(); io.read(), ++count) ;
        QVERIFY(count == 34);
    }

    // Task 95166
    {
        QImageReader io1(prefix + "bat1.gif");
        QImageReader io2(prefix + "bat2.gif");
        QVERIFY(io1.canRead());
        QVERIFY(io2.canRead());
        QImage im1 = io1.read();
        QImage im2 = io2.read();
        QVERIFY(!im1.isNull());
        QVERIFY(!im2.isNull());
        QCOMPARE(im1, im2);
    }

    // Task 9994
    {
        QImageReader io1(prefix + "noclearcode.gif");
        QImageReader io2(prefix + "noclearcode.bmp");
        QVERIFY(io1.canRead());  QVERIFY(io2.canRead());
        QImage im1 = io1.read(); QImage im2 = io2.read();
        QVERIFY(!im1.isNull());  QVERIFY(!im2.isNull());
        QCOMPARE(im1.convertToFormat(QImage::Format_ARGB32), im2.convertToFormat(QImage::Format_ARGB32));
    }

    // Check the undocumented feature.
    {
        QImageReader io(prefix + "endless-anim.gif");
        QVERIFY(io.canRead());
        QCOMPARE(io.loopCount(), -1);
    }
}

void tst_QImageReader::animatedGif()
{
    QImageReader io(":images/qt.gif");
    QImage image = io.read();
    QVERIFY(!image.isNull());
    int i = 0;
    while(!image.isNull()){
        QString frameName = QString(":images/qt%1.gif").arg(++i);
        QCOMPARE(image, QImage(frameName));
        image = io.read();
    }
}

// http://bugreports.qt.nokia.com/browse/QTBUG-6696
// Check the count of images in various call orders...
void tst_QImageReader::gifImageCount()
{
    // just read every frame... and see how much we got..
    {
        QImageReader io(":images/four-frames.gif");

        QVERIFY(io.canRead());
        QImage blackFrame = io.read();

        QVERIFY(io.canRead());
        QImage whiteFrame = io.read();

        QVERIFY(io.canRead());
        QImage greenFrame = io.read();

        QVERIFY(io.imageCount() == 4);

        QVERIFY(io.canRead());
        QImage blueFrame = io.read();

        QVERIFY(!io.canRead());
        QImage emptyFrame = io.read();

        QVERIFY(!io.canRead());
        QCOMPARE(blackFrame.pixel(0,0), qRgb(0, 0, 0));
        QCOMPARE(blackFrame.size(), QSize(64,64));

        QCOMPARE(whiteFrame.pixel(0,0), qRgb(0xff, 0xff, 0xff));
        QCOMPARE(whiteFrame.size(), QSize(64,64));

        QCOMPARE(greenFrame.pixel(0,0), qRgb(0x0, 0xff, 0x0));
        QCOMPARE(greenFrame.size(), QSize(64,64));

        QCOMPARE(blueFrame.pixel(0,0), qRgb(0x0, 0x0, 0xff));
        QCOMPARE(blueFrame.size(), QSize(64,64));
        QVERIFY(emptyFrame.isNull());
    }

    // Read and get the size
    {
        QImageReader io(":images/four-frames.gif");

        QVERIFY(io.canRead());
        QCOMPARE(io.size(), QSize(64,64));

        QVERIFY(io.canRead());
        QCOMPARE(io.size(), QSize(64,64));
        QCOMPARE(io.size(), QSize(64,64));
        QVERIFY(io.canRead());
        QImage blackFrame = io.read();

        QVERIFY(io.canRead());
        QCOMPARE(io.size(), QSize(64,64));
        QCOMPARE(io.size(), QSize(64,64));
        QVERIFY(io.canRead());
        QImage whiteFrame = io.read();

        QVERIFY(io.canRead());
        QCOMPARE(io.size(), QSize(64,64));
        QCOMPARE(io.size(), QSize(64,64));
        QVERIFY(io.canRead());
        QImage greenFrame = io.read();

        QVERIFY(io.canRead());
        QCOMPARE(io.size(), QSize(64,64));
        QCOMPARE(io.size(), QSize(64,64));
        QVERIFY(io.canRead());
        QImage blueFrame = io.read();

        QVERIFY(!io.canRead());
        QCOMPARE(io.size(), QSize());
        QCOMPARE(io.size(), QSize());
        QVERIFY(!io.canRead());
        QImage emptyFrame = io.read();

        QVERIFY(!io.canRead());
        QCOMPARE(blackFrame.pixel(0,0), qRgb(0, 0, 0));
        QCOMPARE(blackFrame.size(), QSize(64,64));

        QCOMPARE(whiteFrame.pixel(0,0), qRgb(0xff, 0xff, 0xff));
        QCOMPARE(whiteFrame.size(), QSize(64,64));

        QCOMPARE(greenFrame.pixel(0,0), qRgb(0x0, 0xff, 0x0));
        QCOMPARE(greenFrame.size(), QSize(64,64));

        QCOMPARE(blueFrame.pixel(0,0), qRgb(0x0, 0x0, 0xff));
        QCOMPARE(blueFrame.size(), QSize(64,64));
        QVERIFY(emptyFrame.isNull());
    }

    // Do a Size query as substitute for canRead
    {
        QImageReader io(":images/four-frames.gif");

        QCOMPARE(io.size(), QSize(64,64));
        QCOMPARE(io.size(), QSize(64,64));
        QImage blackFrame = io.read();

        QCOMPARE(io.size(), QSize(64,64));
        QCOMPARE(io.size(), QSize(64,64));
        QImage whiteFrame = io.read();

        QCOMPARE(io.size(), QSize(64,64));
        QCOMPARE(io.size(), QSize(64,64));
        QImage greenFrame = io.read();

        QCOMPARE(io.size(), QSize(64,64));
        QCOMPARE(io.size(), QSize(64,64));
        QImage blueFrame = io.read();

        QCOMPARE(io.size(), QSize());
        QVERIFY(!io.canRead());
        QImage emptyFrame = io.read();

        QVERIFY(!io.canRead());
        QCOMPARE(blackFrame.pixel(0,0), qRgb(0, 0, 0));
        QCOMPARE(blackFrame.size(), QSize(64,64));

        QCOMPARE(whiteFrame.pixel(0,0), qRgb(0xff, 0xff, 0xff));
        QCOMPARE(whiteFrame.size(), QSize(64,64));

        QCOMPARE(greenFrame.pixel(0,0), qRgb(0x0, 0xff, 0x0));
        QCOMPARE(greenFrame.size(), QSize(64,64));

        QCOMPARE(blueFrame.pixel(0,0), qRgb(0x0, 0x0, 0xff));
        QCOMPARE(blueFrame.size(), QSize(64,64));
        QVERIFY(emptyFrame.isNull());
    }
    {
        QImageReader io(":images/trolltech.gif");
        QVERIFY(io.imageCount() == 34);
        QVERIFY(io.size() == QSize(128,64));
    }
}

void tst_QImageReader::gifLoopCount()
{
    {
        QImageReader io(":images/qt-gif-anim.gif");
        QCOMPARE(io.loopCount(), -1); // infinite loop
    }
    {
        QImageReader io(":images/qt-gif-noanim.gif");
        QCOMPARE(io.loopCount(), 0); // no loop
    }
}

#endif

class Server : public QObject
{
    Q_OBJECT
public:
    Server(const QByteArray &data) :serverSocket(0)
    {
        connect(&server, SIGNAL(newConnection()), this, SLOT(acceptNewConnection()));
        server.listen();
        this->data = data;
    }

public slots:
    void runTest()
    {
        connect(&clientSocket, SIGNAL(connected()), this, SLOT(connected()));
        clientSocket.connectToHost(QHostAddress::LocalHost, server.serverPort());
    }

public:
    inline QTcpSocket *socket() const { return serverSocket; }

signals:
    void ready();

private slots:
    void acceptNewConnection()
    {
        serverSocket = server.nextPendingConnection();
        connect(serverSocket, SIGNAL(error(QAbstractSocket::SocketError)),
                this, SLOT(remoteHostClosed()));
    }

    void connected()
    {
        clientSocket.write(data);
        clientSocket.disconnectFromHost();
    }

    void remoteHostClosed()
    {
        emit ready();
    }

private:
    QTcpServer server;
    QTcpSocket clientSocket;
    QTcpSocket *serverSocket;
    QByteArray data;
};

void tst_QImageReader::readFromDevice_data()
{
    QTest::addColumn<QString>("fileName");
    QTest::addColumn<QByteArray>("format");

    QTest::newRow("pbm") << QString("image.pbm") << QByteArray("pbm");
    QTest::newRow("pgm") << QString("image.pgm") << QByteArray("pgm");
    QTest::newRow("ppm-1") << QString("image.ppm") << QByteArray("ppm");
    QTest::newRow("ppm-2") << QString("teapot.ppm") << QByteArray("ppm");
    QTest::newRow("ppm-3") << QString("teapot.ppm") << QByteArray("ppm");
    QTest::newRow("ppm-4") << QString("runners.ppm") << QByteArray("ppm");
#ifdef QTEST_HAVE_JPEG
    QTest::newRow("jpeg-1") << QString("beavis.jpg") << QByteArray("jpeg");
    QTest::newRow("jpeg-2") << QString("YCbCr_cmyk.jpg") << QByteArray("jpeg");
    QTest::newRow("jpeg-3") << QString("YCbCr_rgb.jpg") << QByteArray("jpeg");
    QTest::newRow("jpeg-4") << QString("qtbug13653-no_eoi.jpg") << QByteArray("jpeg");
#endif // QTEST_HAVE_JPEG
#ifdef QTEST_HAVE_GIF
    QTest::newRow("gif-1") << QString("earth.gif") << QByteArray("gif");
    QTest::newRow("gif-2") << QString("trolltech.gif") << QByteArray("gif");
#endif // QTEST_HAVE_GIF
    QTest::newRow("xbm") << QString("gnus.xbm") << QByteArray("xbm");
    QTest::newRow("xpm") << QString("marble.xpm") << QByteArray("xpm");
    QTest::newRow("bmp-1") << QString("colorful.bmp") << QByteArray("bmp");
    QTest::newRow("bmp-2") << QString("font.bmp") << QByteArray("bmp");
    QTest::newRow("png") << QString("kollada.png") << QByteArray("png");
#ifdef QTEST_HAVE_MNG
    QTest::newRow("mng-1") << QString("ball.mng") << QByteArray("mng");
    QTest::newRow("mng-2") << QString("fire.mng") << QByteArray("mng");
#endif // QTEST_HAVE_MNG
#if defined QTEST_HAVE_SVG
    QTest::newRow("svg") << QString("rect.svg") << QByteArray("svg");
    QTest::newRow("svgz") << QString("rect.svgz") << QByteArray("svgz");
#endif
}

void tst_QImageReader::readFromDevice()
{
    QFETCH(QString, fileName);
    QFETCH(QByteArray, format);

    QImage expectedImage(prefix + fileName, format);

    QFile file(prefix + fileName);
    QVERIFY(file.open(QFile::ReadOnly));
    QByteArray imageData = file.readAll();
    QVERIFY(!imageData.isEmpty());
    {
        QBuffer buffer;
        buffer.setData(imageData);
        QVERIFY(buffer.open(QBuffer::ReadOnly));

        QImageReader reader(&buffer);
        QVERIFY(reader.canRead());
        QImage imageReaderImage = reader.read();

        QCOMPARE(imageReaderImage, expectedImage);

        buffer.seek(0);

        QImage image1;
        QVERIFY(image1.loadFromData((const uchar *)buffer.data().data(),
                                    buffer.data().size(), format.data()));
        QCOMPARE(image1, expectedImage);

        QByteArray throughBase64 = QByteArray::fromBase64(imageData.toBase64());
        QVERIFY(image1.loadFromData((const uchar *)throughBase64.data(),
                                    throughBase64.size(), format.data()));
        QCOMPARE(image1, expectedImage);
    }

    Server server(imageData);
    QEventLoop loop;
    connect(&server, SIGNAL(ready()), &loop, SLOT(quit()));
    QTimer::singleShot(0, &server, SLOT(runTest()));
    QTimer::singleShot(5000, &loop, SLOT(quit()));
    loop.exec();

    QImageReader reader(server.socket(), format == "xbm" ? "xbm" : "");
    if (format == "xbm")
        QVERIFY(!reader.canRead());
    else
        QVERIFY(reader.canRead());
    QImage imageReaderImage = reader.read();
    QCOMPARE(imageReaderImage, expectedImage);
}

void tst_QImageReader::readFromFileAfterJunk_data()
{
    QTest::addColumn<QString>("fileName");
    QTest::addColumn<QByteArray>("format");

    QTest::newRow("pbm") << QString("image.pbm") << QByteArray("pbm");
    QTest::newRow("pgm") << QString("image.pgm") << QByteArray("pgm");
    QTest::newRow("ppm-1") << QString("image.ppm") << QByteArray("ppm");
    QTest::newRow("ppm-2") << QString("teapot.ppm") << QByteArray("ppm");
    QTest::newRow("ppm-3") << QString("teapot.ppm") << QByteArray("ppm");
    QTest::newRow("ppm-4") << QString("runners.ppm") << QByteArray("ppm");
#ifdef QTEST_HAVE_JPEG
    QTest::newRow("jpeg-1") << QString("beavis.jpg") << QByteArray("jpeg");
    QTest::newRow("jpeg-2") << QString("YCbCr_cmyk.jpg") << QByteArray("jpeg");
    QTest::newRow("jpeg-3") << QString("YCbCr_rgb.jpg") << QByteArray("jpeg");
#endif
#if defined QTEST_HAVE_GIF
//    QTest::newRow("gif-1") << QString("images/earth.gif") << QByteArray("gif");
//    QTest::newRow("gif-2") << QString("images/trolltech.gif") << QByteArray("gif");
#endif
    QTest::newRow("xbm") << QString("gnus.xbm") << QByteArray("xbm");
    QTest::newRow("xpm") << QString("marble.xpm") << QByteArray("xpm");
    QTest::newRow("bmp-1") << QString("colorful.bmp") << QByteArray("bmp");
    QTest::newRow("bmp-2") << QString("font.bmp") << QByteArray("bmp");
    QTest::newRow("png") << QString("kollada.png") << QByteArray("png");
//    QTest::newRow("mng-1") << QString("images/ball.mng") << QByteArray("mng");
//    QTest::newRow("mng-2") << QString("images/fire.mng") << QByteArray("mng");
#if defined QTEST_HAVE_SVG
    QTest::newRow("svg") << QString("rect.svg") << QByteArray("svg");
    QTest::newRow("svgz") << QString("rect.svgz") << QByteArray("svgz");
#endif
}

void tst_QImageReader::readFromFileAfterJunk()
{
    QFETCH(QString, fileName);
    QFETCH(QByteArray, format);

    if (!QImageReader::supportedImageFormats().contains(format)) {
        QString cause = QString("Skipping %1; no %2 support").arg(fileName).arg(QString(format));
        QSKIP(qPrintable(cause), SkipSingle);
    }

    QFile::remove("junk");
    QFile junkFile("junk");
    QVERIFY(junkFile.open(QFile::WriteOnly));

    QFile imageFile(prefix + fileName);
    QVERIFY(imageFile.open(QFile::ReadOnly));
    QByteArray imageData = imageFile.readAll();
    QVERIFY(!imageData.isNull());

    int iterations = 3;
    if (format == "ppm" || format == "pbm" || format == "pgm" || format == "svg" || format == "svgz")
        iterations = 1;

    if (format == "mng" || !QImageWriter::supportedImageFormats().contains(format)) {
        for (int i = 0; i < iterations; ++i) {
            junkFile.write("deadbeef", 9);
            QCOMPARE(junkFile.write(imageData), qint64(imageData.size()));
        }
    } else {
        for (int i = 0; i < iterations; ++i) {
            QImageWriter writer(&junkFile, format);
            junkFile.write("deadbeef", 9);
            QVERIFY(writer.write(QImage(prefix + fileName)));
        }
    }
    junkFile.close();
    junkFile.open(QFile::ReadOnly);

    for (int i = 0; i < iterations; ++i) {
        QByteArray ole = junkFile.read(9);
        junkFile.ungetChar(ole[ole.size() - 1]);
        char c;
        junkFile.getChar(&c);
        QImageReader reader(&junkFile);
        QVERIFY(reader.canRead());
        QVERIFY(!reader.read().isNull());
    }
}

void tst_QImageReader::devicePosition_data()
{
    QTest::addColumn<QString>("fileName");
    QTest::addColumn<QByteArray>("format");

    QTest::newRow("pbm") << QString("image.pbm") << QByteArray("pbm");
    QTest::newRow("pgm") << QString("image.pgm") << QByteArray("pgm");
    QTest::newRow("ppm-1") << QString("image.ppm") << QByteArray("ppm");
#ifdef QTEST_HAVE_JPEG
    QTest::newRow("jpeg-1") << QString("beavis.jpg") << QByteArray("jpeg");
    QTest::newRow("jpeg-2") << QString("YCbCr_cmyk.jpg") << QByteArray("jpeg");
    QTest::newRow("jpeg-3") << QString("YCbCr_rgb.jpg") << QByteArray("jpeg");
#endif
#if defined QTEST_HAVE_GIF
    QTest::newRow("gif-1") << QString("earth.gif") << QByteArray("gif");
#endif
    QTest::newRow("xbm") << QString("gnus.xbm") << QByteArray("xbm");
    QTest::newRow("xpm") << QString("marble.xpm") << QByteArray("xpm");
    QTest::newRow("bmp-1") << QString("colorful.bmp") << QByteArray("bmp");
    QTest::newRow("bmp-2") << QString("font.bmp") << QByteArray("bmp");
    QTest::newRow("png") << QString("kollada.png") << QByteArray("png");
//    QTest::newRow("mng-1") << QString("images/ball.mng") << QByteArray("mng");
//    QTest::newRow("mng-2") << QString("images/fire.mng") << QByteArray("mng");
#if defined QTEST_HAVE_SVG
    QTest::newRow("svg") << QString("rect.svg") << QByteArray("svg");
    QTest::newRow("svgz") << QString("rect.svgz") << QByteArray("svgz");
#endif
}

void tst_QImageReader::devicePosition()
{
    QFETCH(QString, fileName);
    QFETCH(QByteArray, format);

    QImage expected(prefix + fileName);
    QVERIFY(!expected.isNull());

    QFile imageFile(prefix + fileName);
    QVERIFY(imageFile.open(QFile::ReadOnly));
    QByteArray imageData = imageFile.readAll();
    QVERIFY(!imageData.isNull());
    int imageDataSize = imageData.size();

    const char *preStr = "prebeef\n";
    int preLen = qstrlen(preStr);
    imageData.prepend(preStr);
    if (format != "svg" && format != "svgz") // Doesn't handle trailing data
        imageData.append("\npostbeef");
    QBuffer buf(&imageData);
    buf.open(QIODevice::ReadOnly);
    buf.seek(preLen);
    QImageReader reader(&buf, format);
    QCOMPARE(expected, reader.read());
    if (format != "ppm" && format != "gif")  // Known not to work
        QCOMPARE(buf.pos(), qint64(preLen+imageDataSize));
}


void tst_QImageReader::readFromResources_data()
{
    QTest::addColumn<QString>("fileName");
    QTest::addColumn<QByteArray>("format");
    QTest::addColumn<QSize>("size");
    QTest::addColumn<QString>("message");

    QTest::newRow("corrupt.bmp") << QString("corrupt.bmp")
                                        << QByteArray("bmp") << QSize(0, 0)
                                        << QString("");
    QTest::newRow("negativeheight.bmp") << QString("negativeheight.bmp")
                                               << QByteArray("bmp") << QSize(127, 64)
                                               << QString("");
    QTest::newRow("font.bmp") << QString("font.bmp")
                                     << QByteArray("bmp") << QSize(240, 8)
                                     << QString("");
    QTest::newRow("noclearcode.bmp") << QString("noclearcode.bmp")
                                            << QByteArray("bmp") << QSize(29, 18)
                                            << QString("");
    QTest::newRow("colorful.bmp") << QString("colorful.bmp")
                                         << QByteArray("bmp") << QSize(320, 200)
                                         << QString("");
    QTest::newRow("16bpp.bmp") << QString("16bpp.bmp")
                                      << QByteArray("bmp") << QSize(320, 240)
                                      << QString("");
    QTest::newRow("crash-signed-char.bmp") << QString("crash-signed-char.bmp")
                                                  << QByteArray("bmp") << QSize(360, 280)
                                                  << QString("");
    QTest::newRow("4bpp-rle.bmp") << QString("4bpp-rle.bmp")
                                         << QByteArray("bmp") << QSize(640, 480)
                                         << QString("");
#ifdef QTEST_HAVE_GIF
    QTest::newRow("corrupt.gif") << QString("corrupt.gif")
                                        << QByteArray("gif") << QSize(0, 0)
                                        << QString("");
    QTest::newRow("trolltech.gif") << QString("trolltech.gif")
                                          << QByteArray("gif") << QSize(128, 64)
                                          << QString("");
    QTest::newRow("noclearcode.gif") << QString("noclearcode.gif")
                                            << QByteArray("gif") << QSize(29, 18)
                                            << QString("");
    QTest::newRow("earth.gif") << QString("earth.gif")
                                      << QByteArray("gif") << QSize(320, 200)
                                      << QString("");
    QTest::newRow("bat1.gif") << QString("bat1.gif")
                                     << QByteArray("gif") << QSize(32, 32)
                                     << QString("");
    QTest::newRow("bat2.gif") << QString("bat2.gif")
                                     << QByteArray("gif") << QSize(32, 32)
                                     << QString("");
#endif
#ifdef QTEST_HAVE_JPEG
    QTest::newRow("corrupt.jpg") << QString("corrupt.jpg")
                                        << QByteArray("jpg") << QSize(0, 0)
                                        << QString("JPEG datastream contains no image");
    QTest::newRow("beavis.jpg") << QString("beavis.jpg")
                                       << QByteArray("jpg") << QSize(350, 350)
                                       << QString("");
    QTest::newRow("YCbCr_cmyk.jpg") << QString("YCbCr_cmyk.jpg")
                                           << QByteArray("jpg") << QSize(75, 50)
                                           << QString("");
    QTest::newRow("YCbCr_rgb.jpg") << QString("YCbCr_rgb.jpg")
                                          << QByteArray("jpg") << QSize(75, 50)
                                          << QString("");
    QTest::newRow("qtbug13653-no_eoi.jpg") << QString("qtbug13653-no_eoi.jpg")
                                        << QByteArray("jpg") << QSize(240, 180)
                                        << QString("");
#endif
#ifdef QTEST_HAVE_MNG
    QTest::newRow("corrupt.mng") << QString("corrupt.mng")
                                        << QByteArray("mng") << QSize(0, 0)
                                        << QString("MNG error 901: Application signalled I/O error; chunk IHDR; subcode 0:0");
    QTest::newRow("fire.mng") << QString("fire.mng")
                                     << QByteArray("mng") << QSize(30, 60)
                                     << QString("");
    QTest::newRow("ball.mng") << QString("ball.mng")
                                     << QByteArray("mng") << QSize(32, 32)
                                     << QString("");
#endif
#ifdef QTEST_HAVE_SVG
    QTest::newRow("rect.svg") << QString("rect.svg")
                                     << QByteArray("svg") << QSize(105, 137)
                                     << QString("");
    QTest::newRow("rect.svgz") << QString("rect.svgz")
                                     << QByteArray("svgz") << QSize(105, 137)
                                     << QString("");
    QTest::newRow("corrupt.svg") << QString("corrupt.svg")
                                     << QByteArray("svg") << QSize(0, 0)
                                     << QString("");
    QTest::newRow("corrupt.svgz") << QString("corrupt.svgz")
                                     << QByteArray("svgz") << QSize(0, 0)
                                     << QString("");
#endif
    QTest::newRow("image.pbm") << QString("image.pbm")
                                      << QByteArray("pbm") << QSize(16, 6)
                                      << QString("");
    QTest::newRow("image.pgm") << QString("image.pgm")
                                      << QByteArray("pgm") << QSize(24, 7)
                                      << QString("");
    QTest::newRow("corrupt.png") << QString("corrupt.png")
                                        << QByteArray("png") << QSize(0, 0)
                                        << QString("");
    QTest::newRow("away.png") << QString("away.png")
                                     << QByteArray("png") << QSize(16, 16)
                                     << QString("");
    QTest::newRow("image.png") << QString("image.png")
                                      << QByteArray("png") << QSize(22, 22)
                                      << QString("");
    QTest::newRow("kollada.png") << QString("kollada.png")
                                        << QByteArray("png") << QSize(436, 160)
                                        << QString("");
    QTest::newRow("black.png") << QString("black.png")
                                      << QByteArray("png") << QSize(48, 48)
                                      << QString("");
    QTest::newRow("YCbCr_cmyk.png") << QString("YCbCr_cmyk.png")
                                           << QByteArray("png") << QSize(75, 50)
                                           << QString("");
    QTest::newRow("teapot.ppm") << QString("teapot.ppm")
                                       << QByteArray("ppm") << QSize(256, 256)
                                       << QString("");
    QTest::newRow("image.ppm") << QString("image.ppm")
                                      << QByteArray("ppm") << QSize(4, 4)
                                      << QString("");
    QTest::newRow("runners.ppm") << QString("runners.ppm")
                                        << QByteArray("ppm") << QSize(400, 400)
                                        << QString("");
    QTest::newRow("test.ppm") << QString("test.ppm")
                                     << QByteArray("ppm") << QSize(10, 10)
                                     << QString("");
//    QTest::newRow("corrupt.xbm") << QString("corrupt.xbm") << QByteArray("xbm") << QSize(0, 0);
    QTest::newRow("gnus.xbm") << QString("gnus.xbm")
                                     << QByteArray("xbm") << QSize(271, 273)
                                     << QString("");
    QTest::newRow("corrupt-colors.xpm") << QString("corrupt-colors.xpm")
                                               << QByteArray("xpm") << QSize(0, 0)
                                               << QString("QImage: XPM color specification is missing: bla9an.n#x");
    QTest::newRow("corrupt-pixels.xpm") << QString("corrupt-pixels.xpm")
                                               << QByteArray("xpm") << QSize(0, 0)
                                               << QString("QImage: XPM pixels missing on image line 3");
    QTest::newRow("marble.xpm") << QString("marble.xpm")
                                       << QByteArray("xpm") << QSize(240, 240)
                                       << QString("");
    QTest::newRow("test.xpm") << QString("test.xpm")
                                     << QByteArray("xpm") << QSize(256, 256)
                                     << QString("");
    QTest::newRow("black.xpm") << QString("black.xpm")
                                      << QByteArray("xpm") << QSize(48, 48)
                                      << QString("");
    QTest::newRow("namedcolors.xpm") << QString("namedcolors.xpm")
                                            << QByteArray("xpm") << QSize(8, 8)
                                            << QString("");
    QTest::newRow("nontransparent.xpm") << QString("nontransparent.xpm")
                                               << QByteArray("xpm") << QSize(8, 8)
                                               << QString("");
    QTest::newRow("transparent.xpm") << QString("transparent.xpm")
                                            << QByteArray("xpm") << QSize(8, 8)
                                            << QString("");
}

void tst_QImageReader::readFromResources()
{
    QFETCH(QString, fileName);
    QFETCH(QByteArray, format);
    QFETCH(QSize, size);
    QFETCH(QString, message);
    for (int i = 0; i < 2; ++i) {
        QString file = i ? (":/images/" + fileName) : (prefix + fileName);
        {
            // suppress warnings if we expect them
            if (!message.isEmpty()) {
                for (int j = 0; j < 5; ++j)
                    QTest::ignoreMessage(QtWarningMsg, message.toLatin1());
            }

            // 1) full filename, no format
            QImageReader reader(file);
            QImage image = reader.read();
            if (size.isNull())
                QVERIFY(image.isNull());
            else
                QVERIFY(!image.isNull());
            QCOMPARE(image.size(), size);
        }
        {
            // 2) full filename, with format
            QImageReader reader(file, format);
            QImage image = reader.read();
            if (size.isNull())
                QVERIFY(image.isNull());
            else
                QVERIFY(!image.isNull());
            QCOMPARE(image.size(), size);
        }
        {
            // 3) full filename, with uppercase format
            QImageReader reader(file, format.toUpper());
            QImage image = reader.read();
            if (size.isNull())
                QVERIFY(image.isNull());
            else
                QVERIFY(!image.isNull());
            QCOMPARE(image.size(), size);
        }
        {
            // 4) chopped filename, with format
            QImageReader reader(file.left(file.lastIndexOf(QLatin1Char('.'))), format);
            QImage image = reader.read();
            if (size.isNull())
                QVERIFY(image.isNull());
            else
                QVERIFY(!image.isNull());
            QCOMPARE(image.size(), size);
        }
        {
            // 5) chopped filename, with uppercase format
            QImageReader reader(file.left(file.lastIndexOf(QLatin1Char('.'))), format.toUpper());
            QImage image = reader.read();
            if (size.isNull())
                QVERIFY(image.isNull());
            else
                QVERIFY(!image.isNull());
            QCOMPARE(image.size(), size);
        }
    }

    // Check that the results are identical
    if (!message.isEmpty()) {
        QTest::ignoreMessage(QtWarningMsg, message.toLatin1());
        QTest::ignoreMessage(QtWarningMsg, message.toLatin1());
    }
    QCOMPARE(QImageReader(prefix + fileName).read(), QImageReader(":/images/" + fileName).read());
}

void tst_QImageReader::readCorruptImage_data()
{
    QTest::addColumn<QString>("fileName");
    QTest::addColumn<bool>("shouldFail");
    QTest::addColumn<QString>("message");
#if defined QTEST_HAVE_JPEG
    QTest::newRow("corrupt jpeg") << QString("corrupt.jpg") << true
                                  << QString("JPEG datastream contains no image");
#endif
#if defined QTEST_HAVE_GIF
    QTest::newRow("corrupt gif") << QString("corrupt.gif") << true << QString("");
#endif
#ifdef QTEST_HAVE_MNG
    QTest::newRow("corrupt mng") << QString("corrupt.mng") << true
                                 << QString("MNG error 901: Application signalled I/O error; chunk IHDR; subcode 0:0");
#endif
    QTest::newRow("corrupt png") << QString("corrupt.png") << true << QString("");
    QTest::newRow("corrupt bmp") << QString("corrupt.bmp") << true << QString("");
    QTest::newRow("corrupt xpm (colors)") << QString("corrupt-colors.xpm") << true
                                          << QString("QImage: XPM color specification is missing: bla9an.n#x");
    QTest::newRow("corrupt xpm (pixels)") << QString("corrupt-pixels.xpm") << true
                                          << QString("QImage: XPM pixels missing on image line 3");
    QTest::newRow("corrupt xbm") << QString("corrupt.xbm") << false << QString("");
#if defined QTEST_HAVE_TIFF
    QTest::newRow("corrupt tiff") << QString("corrupt-data.tif") << true << QString("");
#endif
#if defined QTEST_HAVE_SVG
    QTest::newRow("corrupt svg") << QString("corrupt.svg") << true << QString("");
    QTest::newRow("corrupt svgz") << QString("corrupt.svgz") << true << QString("");
#endif
}

void tst_QImageReader::readCorruptImage()
{
    QFETCH(QString, fileName);
    QFETCH(bool, shouldFail);
    QFETCH(QString, message);

    if (!message.isEmpty())
        QTest::ignoreMessage(QtWarningMsg, message.toLatin1());
    QImageReader reader(prefix + fileName);
    QVERIFY(reader.canRead());
    QCOMPARE(reader.read().isNull(), shouldFail);
}

void tst_QImageReader::readCorruptBmp()
{
    QCOMPARE(QImage(prefix + "tst7.bmp").convertToFormat(QImage::Format_ARGB32_Premultiplied), QImage(prefix + "tst7.png").convertToFormat(QImage::Format_ARGB32_Premultiplied));
}

void tst_QImageReader::supportsOption_data()
{
    QTest::addColumn<QString>("fileName");
    QTest::addColumn<QIntList>("options");

    QTest::newRow("png") << QString("black.png")
                         << (QIntList() << QImageIOHandler::Gamma
                              << QImageIOHandler::Description
                              << QImageIOHandler::Quality
                              << QImageIOHandler::Size);
}

void tst_QImageReader::supportsOption()
{
    QFETCH(QString, fileName);
    QFETCH(QIntList, options);

    QSet<QImageIOHandler::ImageOption> allOptions;
    allOptions << QImageIOHandler::Size
               << QImageIOHandler::ClipRect
               << QImageIOHandler::Description
               << QImageIOHandler::ScaledClipRect
               << QImageIOHandler::ScaledSize
               << QImageIOHandler::CompressionRatio
               << QImageIOHandler::Gamma
               << QImageIOHandler::Quality
               << QImageIOHandler::Name
               << QImageIOHandler::SubType
               << QImageIOHandler::IncrementalReading
               << QImageIOHandler::Endianness
               << QImageIOHandler::Animation
               << QImageIOHandler::BackgroundColor;

    QImageReader reader(prefix + fileName);
    for (int i = 0; i < options.size(); ++i) {
        QVERIFY(reader.supportsOption(QImageIOHandler::ImageOption(options.at(i))));
        allOptions.remove(QImageIOHandler::ImageOption(options.at(i)));
    }

    foreach (QImageIOHandler::ImageOption option, allOptions)
        QVERIFY(!reader.supportsOption(option));
}

#if defined QTEST_HAVE_TIFF
void tst_QImageReader::tiffCompression_data()
{
    QTest::addColumn<QString>("uncompressedFile");
    QTest::addColumn<QString>("compressedFile");

    QTest::newRow("TIFF: adobedeflate") << "rgba_nocompression_littleendian.tif"
                                        << "rgba_adobedeflate_littleendian.tif";
    QTest::newRow("TIFF: lzw") << "rgba_nocompression_littleendian.tif"
                               << "rgba_lzw_littleendian.tif";
    QTest::newRow("TIFF: packbits") << "rgba_nocompression_littleendian.tif"
                                    << "rgba_packbits_littleendian.tif";
    QTest::newRow("TIFF: zipdeflate") << "rgba_nocompression_littleendian.tif"
                                      << "rgba_zipdeflate_littleendian.tif";
}

void tst_QImageReader::tiffCompression()
{
    QFETCH(QString, uncompressedFile);
    QFETCH(QString, compressedFile);

    QImage uncompressedImage(prefix + uncompressedFile);
    QImage compressedImage(prefix + compressedFile);

    QCOMPARE(uncompressedImage, compressedImage);
}

void tst_QImageReader::tiffEndianness()
{
    QImage littleEndian(prefix + "rgba_nocompression_littleendian.tif");
    QImage bigEndian(prefix + "rgba_nocompression_bigendian.tif");

    QCOMPARE(littleEndian, bigEndian);
}

void tst_QImageReader::tiffOrientation_data()
{
    QTest::addColumn<QString>("expected");
    QTest::addColumn<QString>("oriented");
    QTest::newRow("Indexed TIFF, orientation1") << "tiff_oriented/original_indexed.tiff" << "tiff_oriented/indexed_orientation_1.tiff";
    QTest::newRow("Indexed TIFF, orientation2") << "tiff_oriented/original_indexed.tiff" << "tiff_oriented/indexed_orientation_2.tiff";
    QTest::newRow("Indexed TIFF, orientation3") << "tiff_oriented/original_indexed.tiff" << "tiff_oriented/indexed_orientation_3.tiff";
    QTest::newRow("Indexed TIFF, orientation4") << "tiff_oriented/original_indexed.tiff" << "tiff_oriented/indexed_orientation_4.tiff";
    QTest::newRow("Indexed TIFF, orientation5") << "tiff_oriented/original_indexed.tiff" << "tiff_oriented/indexed_orientation_5.tiff";
    QTest::newRow("Indexed TIFF, orientation6") << "tiff_oriented/original_indexed.tiff" << "tiff_oriented/indexed_orientation_6.tiff";
    QTest::newRow("Indexed TIFF, orientation7") << "tiff_oriented/original_indexed.tiff" << "tiff_oriented/indexed_orientation_7.tiff";
    QTest::newRow("Indexed TIFF, orientation8") << "tiff_oriented/original_indexed.tiff" << "tiff_oriented/indexed_orientation_8.tiff";

    QTest::newRow("Mono TIFF, orientation1") << "tiff_oriented/original_mono.tiff" << "tiff_oriented/mono_orientation_1.tiff";
    QTest::newRow("Mono TIFF, orientation2") << "tiff_oriented/original_mono.tiff" << "tiff_oriented/mono_orientation_2.tiff";
    QTest::newRow("Mono TIFF, orientation3") << "tiff_oriented/original_mono.tiff" << "tiff_oriented/mono_orientation_3.tiff";
    QTest::newRow("Mono TIFF, orientation4") << "tiff_oriented/original_mono.tiff" << "tiff_oriented/mono_orientation_4.tiff";
    QTest::newRow("Mono TIFF, orientation5") << "tiff_oriented/original_mono.tiff" << "tiff_oriented/mono_orientation_5.tiff";
    QTest::newRow("Mono TIFF, orientation6") << "tiff_oriented/original_mono.tiff" << "tiff_oriented/mono_orientation_6.tiff";
    QTest::newRow("Mono TIFF, orientation7") << "tiff_oriented/original_mono.tiff" << "tiff_oriented/mono_orientation_7.tiff";
    QTest::newRow("Mono TIFF, orientation8") << "tiff_oriented/original_mono.tiff" << "tiff_oriented/mono_orientation_8.tiff";

    QTest::newRow("RGB TIFF, orientation1") << "tiff_oriented/original_rgb.tiff" << "tiff_oriented/rgb_orientation_1.tiff";
    QTest::newRow("RGB TIFF, orientation2") << "tiff_oriented/original_rgb.tiff" << "tiff_oriented/rgb_orientation_2.tiff";
    QTest::newRow("RGB TIFF, orientation3") << "tiff_oriented/original_rgb.tiff" << "tiff_oriented/rgb_orientation_3.tiff";
    QTest::newRow("RGB TIFF, orientation4") << "tiff_oriented/original_rgb.tiff" << "tiff_oriented/rgb_orientation_4.tiff";
    QTest::newRow("RGB TIFF, orientation5") << "tiff_oriented/original_rgb.tiff" << "tiff_oriented/rgb_orientation_5.tiff";
    QTest::newRow("RGB TIFF, orientation6") << "tiff_oriented/original_rgb.tiff" << "tiff_oriented/rgb_orientation_6.tiff";
    QTest::newRow("RGB TIFF, orientation7") << "tiff_oriented/original_rgb.tiff" << "tiff_oriented/rgb_orientation_7.tiff";
    QTest::newRow("RGB TIFF, orientation8") << "tiff_oriented/original_rgb.tiff" << "tiff_oriented/rgb_orientation_8.tiff";
}

void tst_QImageReader::tiffOrientation()
{
    QFETCH(QString, expected);
    QFETCH(QString, oriented);

    QImage expectedImage(prefix + expected);
    QImage orientedImage(prefix + oriented);
    QCOMPARE(expectedImage, orientedImage);
}

void tst_QImageReader::tiffGrayscale()
{
    QImage actualImage(prefix + "grayscale.tif");
    QImage expectedImage(prefix + "grayscale-ref.tif");

    QCOMPARE(expectedImage, actualImage.convertToFormat(expectedImage.format()));
}
#endif

void tst_QImageReader::dotsPerMeter_data()
{
    QTest::addColumn<QString>("fileName");
    QTest::addColumn<int>("expectedDotsPerMeterX");
    QTest::addColumn<int>("expectedDotsPerMeterY");
#if defined QTEST_HAVE_TIFF
    QTest::newRow("TIFF: 72 dpi") << ("rgba_nocompression_littleendian.tif") << qRound(72 * (100 / 2.54)) << qRound(72 * (100 / 2.54));
    QTest::newRow("TIFF: 100 dpi") << ("image_100dpi.tif") << qRound(100 * (100 / 2.54)) << qRound(100 * (100 / 2.54));
#endif
}

void tst_QImageReader::dotsPerMeter()
{
    QFETCH(QString, fileName);
    QFETCH(int, expectedDotsPerMeterX);
    QFETCH(int, expectedDotsPerMeterY);

    QImage image(prefix + fileName);

    QCOMPARE(image.dotsPerMeterX(), expectedDotsPerMeterX);
    QCOMPARE(image.dotsPerMeterY(), expectedDotsPerMeterY);
}

void tst_QImageReader::physicalDpi_data()
{
    QTest::addColumn<QString>("fileName");
    QTest::addColumn<int>("expectedPhysicalDpiX");
    QTest::addColumn<int>("expectedPhysicalDpiY");
#if defined QTEST_HAVE_TIFF
    QTest::newRow("TIFF: 72 dpi") << "rgba_nocompression_littleendian.tif" << 72 << 72;
    QTest::newRow("TIFF: 100 dpi") << "image_100dpi.tif" << 100 << 100;
#endif
}

void tst_QImageReader::physicalDpi()
{
    QFETCH(QString, fileName);
    QFETCH(int, expectedPhysicalDpiX);
    QFETCH(int, expectedPhysicalDpiY);

    QImage image(prefix + fileName);

    QCOMPARE(image.physicalDpiX(), expectedPhysicalDpiX);
    QCOMPARE(image.physicalDpiY(), expectedPhysicalDpiY);
}

void tst_QImageReader::autoDetectImageFormat()
{
    // Assume PNG is supported :-)
    {
        // Disables file name extension probing
        QImageReader reader(prefix + "kollada");
        reader.setAutoDetectImageFormat(false);
        QVERIFY(!reader.canRead());
        QVERIFY(reader.read().isNull());
        reader.setAutoDetectImageFormat(true);
        QVERIFY(reader.canRead());
        QVERIFY(!reader.read().isNull());
    }
    {
        // Disables detection based on suffix
        QImageReader reader(prefix + "kollada.png");
        reader.setAutoDetectImageFormat(false);
        QVERIFY(!reader.canRead());
        QVERIFY(reader.read().isNull());
        reader.setAutoDetectImageFormat(true);
        QVERIFY(reader.canRead());
        QVERIFY(!reader.read().isNull());
    }
    {
        // Disables detection based on content
        QImageReader reader(prefix + "kollada-noext");
        reader.setAutoDetectImageFormat(false);
        QVERIFY(!reader.canRead());
        QVERIFY(reader.read().isNull());
        reader.setAutoDetectImageFormat(true);
        QVERIFY(reader.canRead());
        QVERIFY(!reader.read().isNull());
    }

#ifdef QTEST_HAVE_JPEG
    {
        QImageReader io(prefix + "YCbCr_rgb.jpg");
        io.setAutoDetectImageFormat(false);
        // This should fail since no format string is given
        QImage image;
        QVERIFY(!io.read(&image));
    }
    {
        QImageReader io(prefix + "YCbCr_rgb.jpg", "jpg");
        io.setAutoDetectImageFormat(false);
        QImage image;
        QVERIFY(io.read(&image));
    }
#endif
    {
        QImageReader io(prefix + "tst7.png");
        io.setAutoDetectImageFormat(false);
        // This should fail since no format string is given
        QImage image;
        QVERIFY(!io.read(&image));
    }
    {
        QImageReader io(prefix + "tst7.png", "png");
        io.setAutoDetectImageFormat(false);
        QImage image;
        QVERIFY(io.read(&image));
    }
}

void tst_QImageReader::fileNameProbing()
{
    QString name("doesnotexist.png");
    QImageReader r;
    r.setFileName(name); // non-existing / non-readable file
    QCOMPARE(r.fileName(), name);

    r.size();
    QCOMPARE(r.fileName(), name);
    r.read();
    QCOMPARE(r.fileName(), name);
}

void tst_QImageReader::pixelCompareWithBaseline_data()
{
    QTest::addColumn<QString>("fileName");

    QTest::newRow("floppy (16px,32px - 16 colors)") << "35floppy.ico";
    QTest::newRow("semitransparent") << "semitransparent.ico";
    QTest::newRow("slightlybrokenBMPHeader") << "kde_favicon.ico";
    QTest::newRow("sightlybrokenIconHeader") << "connect.ico";
}

void tst_QImageReader::pixelCompareWithBaseline()
{
    QFETCH(QString, fileName);

    static int enteredCount = 0;    // Used for better error diagnostics if something fails. We
    static int loadFailCount = 0;   // don't know if the reason load() fails is that the plugin
                                    // does not exist or because of a bug in the plugin. But if at
                                    // least one file succeeded we know that the plugin was built.
                                    // The other failures are then real failures.
    QImage icoImg;
    const QString inputFileName(QString::fromAscii("images/%1").arg(fileName));
    QFileInfo fi(inputFileName);

    ++enteredCount;
    // might fail if the plugin does not exist, which is ok.
    if (icoImg.load(inputFileName)) {
        icoImg = icoImg.convertToFormat(QImage::Format_ARGB32_Premultiplied);
        const QString baselineFileName(QString::fromAscii("baseline/%1.png").arg(fi.baseName()));
#if 0
        icoImg.save(baselineFileName);
#else
        QImage baseImg;
        QVERIFY(baseImg.load(baselineFileName));
        baseImg = baseImg.convertToFormat(QImage::Format_ARGB32_Premultiplied);
        QCOMPARE(int(baseImg.format()), int(icoImg.format()));
        QCOMPARE(baseImg, icoImg);
#endif
    } else {
        ++loadFailCount;
        if (enteredCount != loadFailCount) {
            QFAIL("Plugin is built, but some did not load properly");
        } else {
            qWarning("loading failed, check if ico plugin is built");
        }
    }
}


void tst_QImageReader::testIgnoresFormatAndExtension_data()
{
    QTest::addColumn<QString>("name");
    QTest::addColumn<QString>("extension");
    QTest::addColumn<QString>("expected");

    QTest::newRow("black.png") << "black" << "png" << "png";
    QTest::newRow("black.xpm") << "black" << "xpm" << "xpm";
    QTest::newRow("colorful.bmp") << "colorful" << "bmp" << "bmp";
    QTest::newRow("image.ppm") << "image" << "ppm" << "ppm";
    QTest::newRow("image.pbm") << "image" << "pbm" << "pbm";
    QTest::newRow("image.pgm") << "image" << "pgm" << "pgm";

#if defined QTEST_HAVE_GIF
    QTest::newRow("bat1.gif") << "bat1" << "gif" << "gif";
#endif

#if defined QTEST_HAVE_JPEG
    QTest::newRow("beavis.jpg") << "beavis" << "jpg" << "jpeg";
#endif

#if defined QTEST_HAVE_MNG
    QTest::newRow("fire.mng") << "fire" << "mng" << "mng";
#endif

#if defined QTEST_HAVE_TIFF
    QTest::newRow("image_100dpi.tif") << "image_100dpi" << "tif" << "tiff";
#endif

#if defined QTEST_HAVE_SVG
    QTest::newRow("rect.svg") << "rect" << "svg" << "svg";
    QTest::newRow("rect.svgz") << "rect" << "svgz" << "svgz";
#endif
}


void tst_QImageReader::testIgnoresFormatAndExtension()
{
    QFETCH(QString, name);
    QFETCH(QString, extension);
    QFETCH(QString, expected);

    QList<QByteArray> formats = QImageReader::supportedImageFormats();
    QString fileNameBase = prefix + name + ".";

    foreach (const QByteArray &f, formats) {
        if (f == extension)
            continue;
        QFile tmp(QDir::tempPath() + "/" + name + "_" + expected + "." + f);

        QVERIFY(QFile::copy(fileNameBase + extension, QFileInfo(tmp).absoluteFilePath()));

        QString format;
        QImage image;
        {
            // image reader needs to be scoped for the remove() to work..
            QImageReader r;
            r.setFileName(QFileInfo(tmp).absoluteFilePath());
            r.setDecideFormatFromContent(true);
            format = r.format();
            r.read(&image);
        }

        tmp.remove();

        QVERIFY(!image.isNull());
        QCOMPARE(format, expected);
    }
}

QTEST_MAIN(tst_QImageReader)
#include "tst_qimagereader.moc"
