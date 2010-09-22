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

#include <qimage.h>
#include <qimagereader.h>
#include <qlist.h>
#include <qmatrix.h>
#include <stdio.h>

#include <qpainter.h>
#include <private/qdrawhelper_p.h>


//TESTED_CLASS=
//TESTED_FILES=
#if defined(Q_OS_SYMBIAN)
# define SRCDIR ""
#endif

Q_DECLARE_METATYPE(QImage::Format)

class tst_QImage : public QObject
{
    Q_OBJECT

public:
    tst_QImage();

private slots:
    void create();
    void createInvalidXPM();
    void createFromUChar();
    void convertBitOrder();
    void formatHandlersInput_data();
    void formatHandlersInput();

    void setAlphaChannel_data();
    void setAlphaChannel();

    void alphaChannel();

    void convertToFormat_data();
    void convertToFormat();

    void createAlphaMask_data();
    void createAlphaMask();
#ifndef QT_NO_IMAGE_HEURISTIC_MASK
    void createHeuristicMask();
#endif

    void dotsPerMeterZero();

    void convertToFormatPreserveDotsPrMeter();
#ifndef QT_NO_IMAGE_TEXT
    void convertToFormatPreserveText();
#endif

    void rotate_data();
    void rotate();

    void copy();

    void setPixel_data();
    void setPixel();

    void setColorCount();
    void setColor();

    void rasterClipping();

    void pointOverloads();
    void destructor();
    void cacheKey();

    void smoothScale();
    void smoothScale2();
    void smoothScale3();

    void smoothScaleBig();
    void smoothScaleAlpha();

    void transformed_data();
    void transformed();
    void transformed2();

    void scaled();

    void paintEngine();
    void setAlphaChannelWhilePainting();

    void smoothScaledSubImage();

    void nullSize_data();
    void nullSize();

    void premultipliedAlphaConsistency();

    void compareIndexed();
};

tst_QImage::tst_QImage()

{
}

// Test if QImage (or any functions called from QImage) throws an
// exception when creating an extremely large image.
// QImage::create() should return "false" in this case.
void tst_QImage::create()
{
    bool cr = true;
#if !defined(Q_WS_QWS) && !defined(Q_OS_WINCE)
    try {
#endif
	//QImage image(7000000, 7000000, 8, 256, QImage::IgnoreEndian);
    QImage image(7000000, 7000000, QImage::Format_Indexed8);
    image.setColorCount(256);
        cr = !image.isNull();
#if !defined(Q_WS_QWS) && !defined(Q_OS_WINCE)
    } catch (...) {
    }
#endif
    QVERIFY( !cr );
}

void tst_QImage::createInvalidXPM()
{
    QTest::ignoreMessage(QtWarningMsg, "QImage::QImage(), XPM is not supported");
    const char *xpm[] = {""};
    QImage invalidXPM(xpm);
    QVERIFY(invalidXPM.isNull());
}

void tst_QImage::createFromUChar()
{
    uchar data[] = {
#if Q_BYTE_ORDER == Q_BIG_ENDIAN
        0xFF,
#endif
        1,1,1, 0xFF, 2,2,2, 0xFF, 3,3,3, 0xFF, 4,4,4,
#if Q_BYTE_ORDER != Q_BIG_ENDIAN
        0xFF,
#endif
    };

    // When the data is const, nothing you do to the image will change the source data.
    QImage i1((const uchar*)data, 2, 2, 8, QImage::Format_RGB32);
    QCOMPARE(i1.pixel(0,0), 0xFF010101U);
    QCOMPARE(i1.pixel(1,0), 0xFF020202U);
    QCOMPARE(i1.pixel(0,1), 0xFF030303U);
    QCOMPARE(i1.pixel(1,1), 0xFF040404U);
    {
        QImage i(i1);
        i.setPixel(0,0,5);
    }
    QCOMPARE(i1.pixel(0,0), 0xFF010101U);
    QCOMPARE(*(QRgb*)data, 0xFF010101U);
    *((QRgb*)i1.bits()) = 7U;
    QCOMPARE(i1.pixel(0,0), 7U);
    QCOMPARE(*(QRgb*)data, 0xFF010101U);

    // Changing copies should not change the original image or data.
    {
        QImage i(i1);
        i.setPixel(0,0,5);
        QCOMPARE(*(QRgb*)data, 0xFF010101U);
        i1.setPixel(0,0,9);
        QCOMPARE(i1.pixel(0,0), 0xFF000009U);
        QCOMPARE(i.pixel(0,0), 0xFF000005U);
    }
    QCOMPARE(i1.pixel(0,0), 0xFF000009U);

    // When the data is non-const, nothing you do to copies of the image will change the source data,
    // but changing the image (here via bits()) will change the source data.
    QImage i2((uchar*)data, 2, 2, 8, QImage::Format_RGB32);
    QCOMPARE(i2.pixel(0,0), 0xFF010101U);
    QCOMPARE(i2.pixel(1,0), 0xFF020202U);
    QCOMPARE(i2.pixel(0,1), 0xFF030303U);
    QCOMPARE(i2.pixel(1,1), 0xFF040404U);
    {
        QImage i(i2);
        i.setPixel(0,0,5);
    }
    QCOMPARE(i2.pixel(0,0), 0xFF010101U);
    QCOMPARE(*(QRgb*)data, 0xFF010101U);
    *((QRgb*)i2.bits()) = 7U;
    QCOMPARE(i2.pixel(0,0), 7U);
    QCOMPARE(*(QRgb*)data, 7U);

    // Changing the data will change the image in either case.
    QImage i3((uchar*)data, 2, 2, 8, QImage::Format_RGB32);
    QImage i4((const uchar*)data, 2, 2, 8, QImage::Format_RGB32);
    *(QRgb*)data = 6U;
    QCOMPARE(i3.pixel(0,0), 6U);
    QCOMPARE(i4.pixel(0,0), 6U);
}

void tst_QImage::convertBitOrder()
{
#if !defined(QT3_SUPPORT)
    QSKIP("Qt compiled without Qt3Support", SkipAll);
#else
    QImage i(9,5,1,2,QImage::LittleEndian);
    qMemSet(i.bits(), 0, i.byteCount());

    i.setDotsPerMeterX(9);
    i.setDotsPerMeterY(5);
    i.fill(0x12345678);
    QVERIFY(!i.isNull());

    QImage ni = i.convertBitOrder(QImage::BigEndian);
    QVERIFY(!ni.isNull());
    QVERIFY(ni.bitOrder() == QImage::BigEndian);

    // A bunch of verifies to make sure that nothing was lost
    QVERIFY(i.dotsPerMeterX() == ni.dotsPerMeterX());
    QVERIFY(i.dotsPerMeterY() == ni.dotsPerMeterY());
    QVERIFY(i.depth() == ni.depth());
    QVERIFY(i.size() == ni.size());
    QVERIFY(i.colorCount() == ni.colorCount());
#endif
}

void tst_QImage::formatHandlersInput_data()
{
    QTest::addColumn<QString>("testFormat");
    QTest::addColumn<QString>("testFile");
    #ifdef Q_OS_SYMBIAN
    const QString prefix = QLatin1String(SRCDIR) + "images/";
    #else
    const QString prefix = QLatin1String(SRCDIR) + "/images/";
    #endif

    // add a new line here when a file is added
    QTest::newRow("ICO") << "ICO" << prefix + "image.ico";
    QTest::newRow("PNG") << "PNG" << prefix + "image.png";
    QTest::newRow("GIF") << "GIF" << prefix + "image.gif";
    QTest::newRow("BMP") << "BMP" << prefix + "image.bmp";
    QTest::newRow("JPEG") << "JPEG" << prefix + "image.jpg";
    QTest::newRow("PBM") << "PBM" << prefix + "image.pbm";
    QTest::newRow("PGM") << "PGM" << prefix + "image.pgm";
    QTest::newRow("PPM") << "PPM" << prefix + "image.ppm";
    QTest::newRow("XBM") << "XBM" << prefix + "image.xbm";
    QTest::newRow("XPM") << "XPM" << prefix + "image.xpm";
#if defined QTEST_HAVE_TIFF
    QTest::newRow("TIFF") << "TIFF" << prefix + "image.tif";
#endif
}

void tst_QImage::formatHandlersInput()
{
    QFETCH(QString, testFormat);
    QFETCH(QString, testFile);
    QList<QByteArray> formats = QImageReader::supportedImageFormats();
   // qDebug("Image input formats : %s", formats.join(" | ").latin1());

    bool formatSupported = false;
    for (QList<QByteArray>::Iterator it = formats.begin(); it != formats.end(); ++it) {
        if (*it == testFormat.toLower()) {
	    formatSupported = true;
	    break;
	}
    }
    if (formatSupported) {
//     qDebug(QImage::imageFormat(testFile));
	QCOMPARE(testFormat.toLatin1().toLower(), QImageReader::imageFormat(testFile));
    } else {
	QString msg = "Format not supported : ";
	QSKIP(QString(msg + testFormat).toLatin1(), SkipSingle);
    }
}

void tst_QImage::setAlphaChannel_data()
{
    QTest::addColumn<int>("red");
    QTest::addColumn<int>("green");
    QTest::addColumn<int>("blue");
    QTest::addColumn<int>("alpha");
    QTest::addColumn<bool>("gray");

    QTest::newRow("red at 0%, gray") << 255 << 0 << 0 << 0 << true;
    QTest::newRow("red at 25%, gray") << 255 << 0 << 0 << 63 << true;
    QTest::newRow("red at 50%, gray") << 255 << 0 << 0 << 127 << true;
    QTest::newRow("red at 100%, gray") << 255 << 0 << 0 << 191 << true;
    QTest::newRow("red at 0%, 32bit") << 255 << 0 << 0 << 0 << false;
    QTest::newRow("red at 25%, 32bit") << 255 << 0 << 0 << 63 << false;
    QTest::newRow("red at 50%, 32bit") << 255 << 0 << 0 << 127 << false;
    QTest::newRow("red at 100%, 32bit") << 255 << 0 << 0 << 191 << false;

    QTest::newRow("green at 0%, gray") << 0 << 255 << 0 << 0 << true;
    QTest::newRow("green at 25%, gray") << 0 << 255 << 0 << 63 << true;
    QTest::newRow("green at 50%, gray") << 0 << 255 << 0 << 127 << true;
    QTest::newRow("green at 100%, gray") << 0 << 255 << 0 << 191 << true;
    QTest::newRow("green at 0%, 32bit") << 0 << 255 << 0 << 0 << false;
    QTest::newRow("green at 25%, 32bit") << 0 << 255 << 0 << 63 << false;
    QTest::newRow("green at 50%, 32bit") << 0 << 255 << 0 << 127 << false;
    QTest::newRow("green at 100%, 32bit") << 0 << 255 << 0 << 191 << false;

    QTest::newRow("blue at 0%, gray") << 0 << 0 << 255 << 0 << true;
    QTest::newRow("blue at 25%, gray") << 0 << 0 << 255 << 63 << true;
    QTest::newRow("blue at 50%, gray") << 0 << 0 << 255 << 127 << true;
    QTest::newRow("blue at 100%, gray") << 0 << 0 << 255 << 191 << true;
    QTest::newRow("blue at 0%, 32bit") << 0 << 0 << 255 << 0 << false;
    QTest::newRow("blue at 25%, 32bit") << 0 << 0 << 255 << 63 << false;
    QTest::newRow("blue at 50%, 32bit") << 0 << 0 << 255 << 127 << false;
    QTest::newRow("blue at 100%, 32bit") << 0 << 0 << 255 << 191 << false;
}

void tst_QImage::setAlphaChannel()
{
    QFETCH(int, red);
    QFETCH(int, green);
    QFETCH(int, blue);
    QFETCH(int, alpha);
    QFETCH(bool, gray);

    int width = 100;
    int height = 100;

    QImage image(width, height, QImage::Format_RGB32);
    image.fill(qRgb(red, green, blue));

    // Create the alpha channel
    QImage alphaChannel;
    if (gray) {
        alphaChannel = QImage(width, height, QImage::Format_Indexed8);
        alphaChannel.setColorCount(256);
        for (int i=0; i<256; ++i)
            alphaChannel.setColor(i, qRgb(i, i, i));
        alphaChannel.fill(alpha);
    } else {
        alphaChannel = QImage(width, height, QImage::Format_ARGB32);
        alphaChannel.fill(qRgb(alpha, alpha, alpha));
    }

    image.setAlphaChannel(alphaChannel);
    image = image.convertToFormat(QImage::Format_ARGB32);
    QVERIFY(image.format() == QImage::Format_ARGB32);

    // alpha of 0 becomes black at a=0 due to premultiplication
    QRgb pixel = alpha == 0 ? 0 : qRgba(red, green, blue, alpha);
    bool allPixelsOK = true;
    for (int y=0; y<height; ++y) {
        for (int x=0; x<width; ++x) {
            allPixelsOK &= image.pixel(x, y) == pixel;
        }
    }
    QVERIFY(allPixelsOK);

    QImage outAlpha = image.alphaChannel();
    QCOMPARE(outAlpha.size(), image.size());

    bool allAlphaOk = true;
    for (int y=0; y<height; ++y) {
        for (int x=0; x<width; ++x) {
            allAlphaOk &= outAlpha.pixelIndex(x, y) == alpha;
        }
    }
    QVERIFY(allAlphaOk);

}

void tst_QImage::alphaChannel()
{
    QImage img(10, 10, QImage::Format_Mono);
    img.setColor(0, Qt::transparent);
    img.setColor(1, Qt::black);
    img.fill(0);

    QPainter p(&img);
    p.fillRect(2, 2, 6, 6, Qt::black);
    p.end();

    QCOMPARE(img.alphaChannel(), img.convertToFormat(QImage::Format_ARGB32).alphaChannel());
}

void tst_QImage::convertToFormat_data()
{
    QTest::addColumn<int>("inFormat");
    QTest::addColumn<uint>("inPixel");
    QTest::addColumn<int>("resFormat");
    QTest::addColumn<uint>("resPixel");

    QTest::newRow("red rgb32 -> argb32") << int(QImage::Format_RGB32) << 0xffff0000
                                      << int(QImage::Format_ARGB32) << 0xffff0000;
    QTest::newRow("green rgb32 -> argb32") << int(QImage::Format_RGB32) << 0xff00ff00
                                        << int(QImage::Format_ARGB32) << 0xff00ff00;
    QTest::newRow("blue rgb32 -> argb32") << int(QImage::Format_RGB32) << 0xff0000ff
                                       << int(QImage::Format_ARGB32) << 0xff0000ff;

    QTest::newRow("red rgb32 -> rgb16") << int(QImage::Format_RGB32) << 0xffff0000
                                      << int(QImage::Format_RGB16) << 0xffff0000;
    QTest::newRow("green rgb32 -> rgb16") << int(QImage::Format_RGB32) << 0xff00ff00
                                        << int(QImage::Format_RGB16) << 0xff00ff00;
    QTest::newRow("blue rgb32 -> rgb16") << int(QImage::Format_RGB32) << 0xff0000ff
                                       << int(QImage::Format_RGB16) << 0xff0000ff;
    QTest::newRow("funky rgb32 -> rgb16") << int(QImage::Format_RGB32) << 0xfff0c080
                                       << int(QImage::Format_RGB16) << 0xfff7c384;

    QTest::newRow("red rgb32 -> argb32_pm") << int(QImage::Format_RGB32) << 0xffff0000
                                         << int(QImage::Format_ARGB32_Premultiplied) << 0xffff0000;
    QTest::newRow("green rgb32 -> argb32_pm") << int(QImage::Format_RGB32) << 0xff00ff00
                                           << int(QImage::Format_ARGB32_Premultiplied) <<0xff00ff00;
    QTest::newRow("blue rgb32 -> argb32_pm") << int(QImage::Format_RGB32) << 0xff0000ff
                                          << int(QImage::Format_ARGB32_Premultiplied) << 0xff0000ff;

    QTest::newRow("semired argb32 -> pm") << int(QImage::Format_ARGB32) << 0x7fff0000u
                                       << int(QImage::Format_ARGB32_Premultiplied) << 0x7f7f0000u;
    QTest::newRow("semigreen argb32 -> pm") << int(QImage::Format_ARGB32) << 0x7f00ff00u
                                         << int(QImage::Format_ARGB32_Premultiplied) << 0x7f007f00u;
    QTest::newRow("semiblue argb32 -> pm") << int(QImage::Format_ARGB32) << 0x7f0000ffu
                                        << int(QImage::Format_ARGB32_Premultiplied) << 0x7f00007fu;
    QTest::newRow("semiwhite argb32 -> pm") << int(QImage::Format_ARGB32) << 0x7fffffffu
                                         << int(QImage::Format_ARGB32_Premultiplied) << 0x7f7f7f7fu;
    QTest::newRow("semiblack argb32 -> pm") << int(QImage::Format_ARGB32) << 0x7f000000u
                                         << int(QImage::Format_ARGB32_Premultiplied) << 0x7f000000u;

    QTest::newRow("semired pm -> argb32") << int(QImage::Format_ARGB32_Premultiplied) << 0x7f7f0000u
                                       << int(QImage::Format_ARGB32) << 0x7fff0000u;
    QTest::newRow("semigreen pm -> argb32") << int(QImage::Format_ARGB32_Premultiplied) << 0x7f007f00u
                                         << int(QImage::Format_ARGB32) << 0x7f00ff00u;
    QTest::newRow("semiblue pm -> argb32") << int(QImage::Format_ARGB32_Premultiplied) << 0x7f00007fu
                                        << int(QImage::Format_ARGB32) << 0x7f0000ffu;
    QTest::newRow("semiwhite pm -> argb32") << int(QImage::Format_ARGB32_Premultiplied) << 0x7f7f7f7fu
                                         << int(QImage::Format_ARGB32) << 0x7fffffffu;
    QTest::newRow("semiblack pm -> argb32") << int(QImage::Format_ARGB32_Premultiplied) << 0x7f000000u
                                         << int(QImage::Format_ARGB32) << 0x7f000000u;

    QTest::newRow("semired pm -> rgb32") << int(QImage::Format_ARGB32_Premultiplied) << 0x7f7f0000u
                                       << int(QImage::Format_RGB32) << 0xffff0000u;
    QTest::newRow("semigreen pm -> rgb32") << int(QImage::Format_ARGB32_Premultiplied) << 0x7f007f00u
                                         << int(QImage::Format_RGB32) << 0xff00ff00u;
    QTest::newRow("semiblue pm -> rgb32") << int(QImage::Format_ARGB32_Premultiplied) << 0x7f00007fu
                                        << int(QImage::Format_RGB32) << 0xff0000ffu;
    QTest::newRow("semiwhite pm -> rgb32") << int(QImage::Format_ARGB32_Premultiplied) << 0x7f7f7f7fu
                                         << int(QImage::Format_RGB32) << 0xffffffffu;
    QTest::newRow("semiblack pm -> rgb32") << int(QImage::Format_ARGB32_Premultiplied) << 0x7f000000u
                                         << int(QImage::Format_RGB32) << 0xff000000u;

    QTest::newRow("semired argb32 -> rgb32") << int(QImage::Format_ARGB32) << 0x7fff0000u
                                             << int(QImage::Format_RGB32) << 0xffff0000u;
    QTest::newRow("semigreen argb32 -> rgb32") << int(QImage::Format_ARGB32) << 0x7f00ff00u
                                               << int(QImage::Format_RGB32) << 0xff00ff00u;
    QTest::newRow("semiblue argb32 -> rgb32") << int(QImage::Format_ARGB32) << 0x7f0000ffu
                                              << int(QImage::Format_RGB32) << 0xff0000ffu;
    QTest::newRow("semiwhite argb -> rgb32") << int(QImage::Format_ARGB32) << 0x7fffffffu
                                             << int(QImage::Format_RGB32) << 0xffffffffu;
    QTest::newRow("semiblack argb -> rgb32") << int(QImage::Format_ARGB32) << 0x7f000000u
                                             << int(QImage::Format_RGB32) << 0xff000000u;

    QTest::newRow("black mono -> rgb32") << int(QImage::Format_Mono) << 0x00000000u
                                         << int(QImage::Format_RGB32) << 0xff000000u;

    QTest::newRow("white mono -> rgb32") << int(QImage::Format_Mono) << 0x00000001u
                                         << int(QImage::Format_RGB32) << 0xffffffffu;
    QTest::newRow("red rgb16 -> argb32") << int(QImage::Format_RGB16) << 0xffff0000
                                         << int(QImage::Format_ARGB32) << 0xffff0000;
    QTest::newRow("green rgb16 -> argb32") << int(QImage::Format_RGB16) << 0xff00ff00
                                           << int(QImage::Format_ARGB32) << 0xff00ff00;
    QTest::newRow("blue rgb16 -> argb32") << int(QImage::Format_RGB16) << 0xff0000ff
                                          << int(QImage::Format_ARGB32) << 0xff0000ff;
    QTest::newRow("red rgb16 -> rgb16") << int(QImage::Format_RGB32) << 0xffff0000
                                         << int(QImage::Format_RGB16) << 0xffff0000;
    QTest::newRow("green rgb16 -> rgb16") << int(QImage::Format_RGB32) << 0xff00ff00
                                           << int(QImage::Format_RGB16) << 0xff00ff00;
    QTest::newRow("blue rgb16 -> rgb16") << int(QImage::Format_RGB32) << 0xff0000ff
                                          << int(QImage::Format_RGB16) << 0xff0000ff;
    QTest::newRow("semired argb32 -> rgb16") << int(QImage::Format_ARGB32) << 0x7fff0000u
                                             << int(QImage::Format_RGB16) << 0xffff0000;
    QTest::newRow("semigreen argb32 -> rgb16") << int(QImage::Format_ARGB32) << 0x7f00ff00u
                                               << int(QImage::Format_RGB16) << 0xff00ff00;
    QTest::newRow("semiblue argb32 -> rgb16") << int(QImage::Format_ARGB32) << 0x7f0000ffu
                                              << int(QImage::Format_RGB16) << 0xff0000ff;
    QTest::newRow("semired pm -> rgb16") << int(QImage::Format_ARGB32_Premultiplied) << 0x7f7f0000u
                                       << int(QImage::Format_RGB16) << 0xffff0000u;

    QTest::newRow("semigreen pm -> rgb16") << int(QImage::Format_ARGB32_Premultiplied) << 0x7f007f00u
                                         << int(QImage::Format_RGB16) << 0xff00ff00u;
    QTest::newRow("semiblue pm -> rgb16") << int(QImage::Format_ARGB32_Premultiplied) << 0x7f00007fu
                                        << int(QImage::Format_RGB16) << 0xff0000ffu;
    QTest::newRow("semiwhite pm -> rgb16") << int(QImage::Format_ARGB32_Premultiplied) << 0x7f7f7f7fu
                                         << int(QImage::Format_RGB16) << 0xffffffffu;
    QTest::newRow("semiblack pm -> rgb16") << int(QImage::Format_ARGB32_Premultiplied) << 0x7f000000u
                                         << int(QImage::Format_RGB16) << 0xff000000u;

    QTest::newRow("mono -> mono lsb") << int(QImage::Format_Mono) << 1u
                                      << int(QImage::Format_MonoLSB) << 0xffffffffu;
    QTest::newRow("mono lsb -> mono") << int(QImage::Format_MonoLSB) << 1u
                                      << int(QImage::Format_Mono) << 0xffffffffu;

    QTest::newRow("red rgb32 -> rgb666") << int(QImage::Format_RGB32) << 0xffff0000
                                        << int(QImage::Format_RGB666) << 0xffff0000;
    QTest::newRow("green rgb32 -> rgb666") << int(QImage::Format_RGB32) << 0xff00ff00
                                          << int(QImage::Format_RGB666) << 0xff00ff00;
    QTest::newRow("blue rgb32 -> rgb666") << int(QImage::Format_RGB32) << 0xff0000ff
                                         << int(QImage::Format_RGB666) << 0xff0000ff;

    QTest::newRow("red rgb16 -> rgb666") << int(QImage::Format_RGB16) << 0xffff0000
                                        << int(QImage::Format_RGB666) << 0xffff0000;
    QTest::newRow("green rgb16 -> rgb666") << int(QImage::Format_RGB16) << 0xff00ff00
                                          << int(QImage::Format_RGB666) << 0xff00ff00;
    QTest::newRow("blue rgb16 -> rgb666") << int(QImage::Format_RGB16) << 0xff0000ff
                                         << int(QImage::Format_RGB666) << 0xff0000ff;

    QTest::newRow("red rgb32 -> rgb15") << int(QImage::Format_RGB32) << 0xffff0000
                                        << int(QImage::Format_RGB555) << 0xffff0000;
    QTest::newRow("green rgb32 -> rgb15") << int(QImage::Format_RGB32) << 0xff00ff00
                                          << int(QImage::Format_RGB555) << 0xff00ff00;
    QTest::newRow("blue rgb32 -> rgb15") << int(QImage::Format_RGB32) << 0xff0000ff
                                         << int(QImage::Format_RGB555) << 0xff0000ff;
    QTest::newRow("funky rgb32 -> rgb15") << int(QImage::Format_RGB32) << 0xfff0c080
                                          << int(QImage::Format_RGB555) << 0xfff7c684;

    QTest::newRow("red rgb16 -> rgb15") << int(QImage::Format_RGB16) << 0xffff0000
                                        << int(QImage::Format_RGB555) << 0xffff0000;
    QTest::newRow("green rgb16 -> rgb15") << int(QImage::Format_RGB16) << 0xff00ff00
                                          << int(QImage::Format_RGB555) << 0xff00ff00;
    QTest::newRow("blue rgb16 -> rgb15") << int(QImage::Format_RGB16) << 0xff0000ff
                                         << int(QImage::Format_RGB555) << 0xff0000ff;
    QTest::newRow("funky rgb16 -> rgb15") << int(QImage::Format_RGB16) << 0xfff0c080
                                          << int(QImage::Format_RGB555) << 0xfff7c684;

    QTest::newRow("red rgb32 -> argb8565") << int(QImage::Format_RGB32) << 0xffff0000
                                           << int(QImage::Format_ARGB8565_Premultiplied) << 0xffff0000;
    QTest::newRow("green rgb32 -> argb8565") << int(QImage::Format_RGB32) << 0xff00ff00
                                             << int(QImage::Format_ARGB8565_Premultiplied) << 0xff00ff00;
    QTest::newRow("blue rgb32 -> argb8565") << int(QImage::Format_RGB32) << 0xff0000ff
                                            << int(QImage::Format_ARGB8565_Premultiplied) << 0xff0000ff;

    QTest::newRow("red rgb16 -> argb8565") << int(QImage::Format_RGB16) << 0xffff0000
                                           << int(QImage::Format_ARGB8565_Premultiplied) << 0xffff0000;
    QTest::newRow("green rgb16 -> argb8565") << int(QImage::Format_RGB16) << 0xff00ff00
                                             << int(QImage::Format_ARGB8565_Premultiplied) << 0xff00ff00;
    QTest::newRow("blue rgb16 -> argb8565") << int(QImage::Format_RGB16) << 0xff0000ff
                                            << int(QImage::Format_ARGB8565_Premultiplied) << 0xff0000ff;

    QTest::newRow("red argb8565 -> argb32") << int(QImage::Format_ARGB8565_Premultiplied) << 0xffff0000
                                            << int(QImage::Format_ARGB32) << 0xffff0000;
    QTest::newRow("green argb8565 -> argb32") << int(QImage::Format_ARGB8565_Premultiplied) << 0xff00ff00
                                              << int(QImage::Format_ARGB32) << 0xff00ff00;
    QTest::newRow("blue argb8565 -> argb32") << int(QImage::Format_ARGB8565_Premultiplied) << 0xff0000ff
                                             << int(QImage::Format_ARGB32) << 0xff0000ff;

    QTest::newRow("semired argb32 -> argb8565") << int(QImage::Format_ARGB32) << 0x7fff0000u
                                                << int(QImage::Format_ARGB8565_Premultiplied) << 0x7f7b0000u;
    QTest::newRow("semigreen argb32 -> argb8565") << int(QImage::Format_ARGB32) << 0x7f00ff00u
                                                  << int(QImage::Format_ARGB8565_Premultiplied) << 0x7f007d00u;
    QTest::newRow("semiblue argb32 -> argb8565") << int(QImage::Format_ARGB32) << 0x7f0000ffu
                                                 << int(QImage::Format_ARGB8565_Premultiplied) << 0x7f00007bu;

    QTest::newRow("semired pm -> argb8565") << int(QImage::Format_ARGB32_Premultiplied) << 0x7f7f0000u
                                            << int(QImage::Format_ARGB8565_Premultiplied) << 0x7f7b0000u;
    QTest::newRow("semigreen pm -> argb8565") << int(QImage::Format_ARGB32_Premultiplied) << 0x7f007f00u
                                              << int(QImage::Format_ARGB8565_Premultiplied) << 0x7f007d00u;
    QTest::newRow("semiblue pm -> argb8565") << int(QImage::Format_ARGB32_Premultiplied) << 0x7f00007fu
                                             << int(QImage::Format_ARGB8565_Premultiplied) << 0x7f00007bu;
    QTest::newRow("semiwhite pm -> argb8565") << int(QImage::Format_ARGB32_Premultiplied) << 0x7f7f7f7fu
                                              << int(QImage::Format_ARGB8565_Premultiplied) << 0x7f7b7d7bu;
    QTest::newRow("semiblack pm -> argb8565") << int(QImage::Format_ARGB32_Premultiplied) << 0x7f000000u
                                              << int(QImage::Format_ARGB8565_Premultiplied) << 0x7f000000u;

    QTest::newRow("red rgb666 -> argb32") << int(QImage::Format_RGB666) << 0xffff0000
                                         << int(QImage::Format_ARGB32) << 0xffff0000;
    QTest::newRow("green rgb666 -> argb32") << int(QImage::Format_RGB666) << 0xff00ff00
                                           << int(QImage::Format_ARGB32) << 0xff00ff00;
    QTest::newRow("blue rgb666 -> argb32") << int(QImage::Format_RGB666) << 0xff0000ff
                                          << int(QImage::Format_ARGB32) << 0xff0000ff;

    QTest::newRow("semired argb32 -> rgb666") << int(QImage::Format_ARGB32) << 0x7fff0000u
                                             << int(QImage::Format_RGB666) << 0xffff0000;
    QTest::newRow("semigreen argb32 -> rgb666") << int(QImage::Format_ARGB32) << 0x7f00ff00u
                                               << int(QImage::Format_RGB666) << 0xff00ff00;
    QTest::newRow("semiblue argb32 -> rgb666") << int(QImage::Format_ARGB32) << 0x7f0000ffu
                                              << int(QImage::Format_RGB666) << 0xff0000ff;

    QTest::newRow("semired pm -> rgb666") << int(QImage::Format_ARGB32_Premultiplied) << 0x7f7f0000u
                                         << int(QImage::Format_RGB666) << 0xffff0000u;
    QTest::newRow("semigreen pm -> rgb666") << int(QImage::Format_ARGB32_Premultiplied) << 0x7f007f00u
                                           << int(QImage::Format_RGB666) << 0xff00ff00u;
    QTest::newRow("semiblue pm -> rgb666") << int(QImage::Format_ARGB32_Premultiplied) << 0x7f00007fu
                                          << int(QImage::Format_RGB666) << 0xff0000ffu;
    QTest::newRow("semiwhite pm -> rgb666") << int(QImage::Format_ARGB32_Premultiplied) << 0x7f7f7f7fu
                                           << int(QImage::Format_RGB666) << 0xffffffffu;
    QTest::newRow("semiblack pm -> rgb666") << int(QImage::Format_ARGB32_Premultiplied) << 0x7f000000u
                                           << int(QImage::Format_RGB666) << 0xff000000u;

    QTest::newRow("red rgb15 -> argb32") << int(QImage::Format_RGB555) << 0xffff0000
                                         << int(QImage::Format_ARGB32) << 0xffff0000;
    QTest::newRow("green rgb15 -> argb32") << int(QImage::Format_RGB555) << 0xff00ff00
                                           << int(QImage::Format_ARGB32) << 0xff00ff00;
    QTest::newRow("blue rgb15 -> argb32") << int(QImage::Format_RGB555) << 0xff0000ff
                                          << int(QImage::Format_ARGB32) << 0xff0000ff;

    QTest::newRow("semired argb32 -> rgb15") << int(QImage::Format_ARGB32) << 0x7fff0000u
                                             << int(QImage::Format_RGB555) << 0xffff0000;
    QTest::newRow("semigreen argb32 -> rgb15") << int(QImage::Format_ARGB32) << 0x7f00ff00u
                                               << int(QImage::Format_RGB555) << 0xff00ff00;
    QTest::newRow("semiblue argb32 -> rgb15") << int(QImage::Format_ARGB32) << 0x7f0000ffu
                                              << int(QImage::Format_RGB555) << 0xff0000ff;

    QTest::newRow("semired pm -> rgb15") << int(QImage::Format_ARGB32_Premultiplied) << 0x7f7f0000u
                                         << int(QImage::Format_RGB555) << 0xffff0000u;
    QTest::newRow("semigreen pm -> rgb15") << int(QImage::Format_ARGB32_Premultiplied) << 0x7f007f00u
                                           << int(QImage::Format_RGB555) << 0xff00ff00u;
    QTest::newRow("semiblue pm -> rgb15") << int(QImage::Format_ARGB32_Premultiplied) << 0x7f00007fu
                                          << int(QImage::Format_RGB555) << 0xff0000ffu;
    QTest::newRow("semiwhite pm -> rgb15") << int(QImage::Format_ARGB32_Premultiplied) << 0x7f7f7f7fu
                                           << int(QImage::Format_RGB555) << 0xffffffffu;
    QTest::newRow("semiblack pm -> rgb15") << int(QImage::Format_ARGB32_Premultiplied) << 0x7f000000u
                                           << int(QImage::Format_RGB555) << 0xff000000u;


    QTest::newRow("red rgb32 -> argb8555") << int(QImage::Format_RGB32) << 0xffff0000
                                           << int(QImage::Format_ARGB8555_Premultiplied) << 0xffff0000;
    QTest::newRow("green rgb32 -> argb8555") << int(QImage::Format_RGB32) << 0xff00ff00
                                             << int(QImage::Format_ARGB8555_Premultiplied) << 0xff00ff00;
    QTest::newRow("blue rgb32 -> argb8555") << int(QImage::Format_RGB32) << 0xff0000ff
                                            << int(QImage::Format_ARGB8555_Premultiplied) << 0xff0000ff;

    QTest::newRow("red rgb16 -> argb8555") << int(QImage::Format_RGB16) << 0xffff0000
                                           << int(QImage::Format_ARGB8555_Premultiplied) << 0xffff0000;
    QTest::newRow("green rgb16 -> argb8555") << int(QImage::Format_RGB16) << 0xff00ff00
                                             << int(QImage::Format_ARGB8555_Premultiplied) << 0xff00ff00;
    QTest::newRow("blue rgb16 -> argb8555") << int(QImage::Format_RGB16) << 0xff0000ff
                                            << int(QImage::Format_ARGB8555_Premultiplied) << 0xff0000ff;

    QTest::newRow("red argb8555 -> argb32") << int(QImage::Format_ARGB8555_Premultiplied) << 0xffff0000
                                            << int(QImage::Format_ARGB32) << 0xffff0000;
    QTest::newRow("green argb8555 -> argb32") << int(QImage::Format_ARGB8555_Premultiplied) << 0xff00ff00
                                              << int(QImage::Format_ARGB32) << 0xff00ff00;
    QTest::newRow("blue argb8555 -> argb32") << int(QImage::Format_ARGB8555_Premultiplied) << 0xff0000ff
                                             << int(QImage::Format_ARGB32) << 0xff0000ff;

    QTest::newRow("semired argb32 -> argb8555") << int(QImage::Format_ARGB32) << 0x7fff0000u
                                                << int(QImage::Format_ARGB8555_Premultiplied) << 0x7f7b0000u;
    QTest::newRow("semigreen argb32 -> argb8555") << int(QImage::Format_ARGB32) << 0x7f00ff00u
                                                  << int(QImage::Format_ARGB8555_Premultiplied) << 0x7f007b00u;
    QTest::newRow("semiblue argb32 -> argb8555") << int(QImage::Format_ARGB32) << 0x7f0000ffu
                                                 << int(QImage::Format_ARGB8555_Premultiplied) << 0x7f00007bu;

    QTest::newRow("semired pm -> argb8555") << int(QImage::Format_ARGB32_Premultiplied) << 0x7f7f0000u
                                            << int(QImage::Format_ARGB8555_Premultiplied) << 0x7f7b0000u;
    QTest::newRow("semigreen pm -> argb8555") << int(QImage::Format_ARGB32_Premultiplied) << 0x7f007f00u
                                              << int(QImage::Format_ARGB8555_Premultiplied) << 0x7f007b00u;
    QTest::newRow("semiblue pm -> argb8555") << int(QImage::Format_ARGB32_Premultiplied) << 0x7f00007fu
                                             << int(QImage::Format_ARGB8555_Premultiplied) << 0x7f00007bu;
    QTest::newRow("semiwhite pm -> argb8555") << int(QImage::Format_ARGB32_Premultiplied) << 0x7f7f7f7fu
                                              << int(QImage::Format_ARGB8555_Premultiplied) << 0x7f7b7b7bu;
    QTest::newRow("semiblack pm -> argb8555") << int(QImage::Format_ARGB32_Premultiplied) << 0x7f000000u
                                              << int(QImage::Format_ARGB8555_Premultiplied) << 0x7f000000u;

    QTest::newRow("red rgb32 -> rgb888") << int(QImage::Format_RGB32) << 0xffff0000
                                         << int(QImage::Format_RGB888) << 0xffff0000;
    QTest::newRow("green rgb32 -> rgb888") << int(QImage::Format_RGB32) << 0xff00ff00
                                           << int(QImage::Format_RGB888) << 0xff00ff00;
    QTest::newRow("blue rgb32 -> rgb888") << int(QImage::Format_RGB32) << 0xff0000ff
                                          << int(QImage::Format_RGB888) << 0xff0000ff;

    QTest::newRow("red rgb16 -> rgb888") << int(QImage::Format_RGB16) << 0xffff0000
                                         << int(QImage::Format_RGB888) << 0xffff0000;
    QTest::newRow("green rgb16 -> rgb888") << int(QImage::Format_RGB16) << 0xff00ff00
                                           << int(QImage::Format_RGB888) << 0xff00ff00;
    QTest::newRow("blue rgb16 -> rgb888") << int(QImage::Format_RGB16) << 0xff0000ff
                                          << int(QImage::Format_RGB888) << 0xff0000ff;

    QTest::newRow("red rgb888 -> argb32") << int(QImage::Format_RGB888) << 0xffff0000
                                          << int(QImage::Format_ARGB32) << 0xffff0000;
    QTest::newRow("green rgb888 -> argb32") << int(QImage::Format_RGB888) << 0xff00ff00
                                            << int(QImage::Format_ARGB32) << 0xff00ff00;
    QTest::newRow("blue rgb888 -> argb32") << int(QImage::Format_RGB888) << 0xff0000ff
                                           << int(QImage::Format_ARGB32) << 0xff0000ff;

    QTest::newRow("semired argb32 -> rgb888") << int(QImage::Format_ARGB32) << 0x7fff0000u
                                              << int(QImage::Format_RGB888) << 0xffff0000;
    QTest::newRow("semigreen argb32 -> rgb888") << int(QImage::Format_ARGB32) << 0x7f00ff00u
                                                << int(QImage::Format_RGB888) << 0xff00ff00;
    QTest::newRow("semiblue argb32 -> rgb888") << int(QImage::Format_ARGB32) << 0x7f0000ffu
                                               << int(QImage::Format_RGB888) << 0xff0000ff;

    QTest::newRow("semired pm -> rgb888") << int(QImage::Format_ARGB32_Premultiplied) << 0x7f7f0000u
                                          << int(QImage::Format_RGB888) << 0xffff0000u;
    QTest::newRow("semigreen pm -> rgb888") << int(QImage::Format_ARGB32_Premultiplied) << 0x7f007f00u
                                            << int(QImage::Format_RGB888) << 0xff00ff00u;
    QTest::newRow("semiblue pm -> rgb888") << int(QImage::Format_ARGB32_Premultiplied) << 0x7f00007fu
                                           << int(QImage::Format_RGB888) << 0xff0000ffu;
    QTest::newRow("semiwhite pm -> rgb888") << int(QImage::Format_ARGB32_Premultiplied) << 0x7f7f7f7fu
                                            << int(QImage::Format_RGB888) << 0xffffffffu;
    QTest::newRow("semiblack pm -> rgb888") << int(QImage::Format_ARGB32_Premultiplied) << 0x7f000000u
                                            << int(QImage::Format_RGB888) << 0xff000000u;
}


void tst_QImage::convertToFormat()
{
    QFETCH(int, inFormat);
    QFETCH(uint, inPixel);
    QFETCH(int, resFormat);
    QFETCH(uint, resPixel);

    QImage src(32, 32, QImage::Format(inFormat));

    if (inFormat == QImage::Format_Mono) {
        src.setColor(0, qRgba(0,0,0,0xff));
        src.setColor(1, qRgba(255,255,255,0xff));
    }

    for (int y=0; y<src.height(); ++y)
        for (int x=0; x<src.width(); ++x)
            src.setPixel(x, y, inPixel);

    QImage result = src.convertToFormat(QImage::Format(resFormat));

    QCOMPARE(src.width(), result.width());
    QCOMPARE(src.height(), result.height());

    bool same = true;
    for (int y=0; y<result.height(); ++y) {
        for (int x=0; x<result.width(); ++x) {
            QRgb pixel = result.pixel(x, y);
            same &= (pixel == resPixel);
            if (!same) {
                printf("expect=%08x, result=%08x\n", resPixel, pixel);
                y = 100000;
                break;
            }

        }
    }
    QVERIFY(same);

    // repeat tests converting from an image with nonstandard stride

    int dp = (src.depth() < 8 || result.depth() < 8) ? 8 : 1;
    QImage src2(src.bits() + (dp*src.depth())/8,
                src.width() - dp*2,
                src.height(), src.bytesPerLine(),
                src.format());
    if (src.depth() < 8)
        src2.setColorTable(src.colorTable());

    const QImage result2 = src2.convertToFormat(QImage::Format(resFormat));

    QCOMPARE(src2.width(), result2.width());
    QCOMPARE(src2.height(), result2.height());

    QImage expected2(result.bits() + (dp*result.depth())/8,
                     result.width() - dp*2,
                     result.height(), result.bytesPerLine(),
                     result.format());
    if (result.depth() < 8)
        expected2.setColorTable(result.colorTable());

    result2.save("result2.xpm", "XPM");
    expected2.save("expected2.xpm", "XPM");

    QCOMPARE(result2, expected2);
    QFile::remove(QLatin1String("result2.xpm"));
    QFile::remove(QLatin1String("expected2.xpm"));
}

void tst_QImage::createAlphaMask_data()
{
    QTest::addColumn<int>("x");
    QTest::addColumn<int>("y");
    QTest::addColumn<int>("alpha1");
    QTest::addColumn<int>("alpha2");

    int alphas[] = { 0, 127, 255 };

    for (unsigned a1 = 0; a1 < sizeof(alphas) / sizeof(int); ++a1) {
        for (unsigned a2 = 0; a2 < sizeof(alphas) / sizeof(int); ++a2) {
            if (a1 == a2)
                continue;
            for (int x=10; x<18; x+=3) {
                for (int y=100; y<108; y+=3) {
                    QTest::newRow(qPrintable(QString::fromLatin1("x=%1, y=%2, a1=%3, a2=%4").arg(x).arg(y).arg(alphas[a1]).arg(alphas[a2])))
                        << x << y << alphas[a1] << alphas[a2];
                }
            }
        }
    }
}

void tst_QImage::createAlphaMask()
{
    QFETCH(int, x);
    QFETCH(int, y);
    QFETCH(int, alpha1);
    QFETCH(int, alpha2);

    QSize size(255, 255);
    int pixelsInLines = size.width() + size.height() - 1;
    int pixelsOutofLines = size.width() * size.height() - pixelsInLines;

    // Generate an white image with two lines, horizontal at y and vertical at x.
    // Lines have alpha of alpha2, rest has alpha of alpha1
    QImage image(size, QImage::Format_ARGB32);
    for (int cy=0; cy<image.height(); ++cy) {
        for (int cx=0; cx<image.width(); ++cx) {
            int alpha = (y == cy || x == cx) ? alpha2 : alpha1;
            image.setPixel(cx, cy, qRgba(255, 255, 255, alpha));
        }
    }

    QImage mask = image.createAlphaMask(Qt::OrderedAlphaDither);

    // Sanity check...
    QCOMPARE(mask.width(), image.width());
    QCOMPARE(mask.height(), image.height());

    // Sum up the number of pixels set for both lines and other area
    int sumAlpha1 = 0;
    int sumAlpha2 = 0;
    for (int cy=0; cy<image.height(); ++cy) {
        for (int cx=0; cx<image.width(); ++cx) {
            int *alpha = (y == cy || x == cx) ? &sumAlpha2 : &sumAlpha1;
            *alpha += mask.pixelIndex(cx, cy);
        }
    }

    // Compare the set bits to whats expected for that alpha.
    const int threshold = 5;
    QVERIFY(qAbs(sumAlpha1 * 255 / pixelsOutofLines - alpha1) < threshold);
    QVERIFY(qAbs(sumAlpha2 * 255 / pixelsInLines - alpha2) < threshold);
}

void tst_QImage::dotsPerMeterZero()
{
    QImage img(100, 100, QImage::Format_RGB32);
    QVERIFY(!img.isNull());

    int defaultDpmX = img.dotsPerMeterX();
    int defaultDpmY = img.dotsPerMeterY();
    QVERIFY(defaultDpmX != 0);
    QVERIFY(defaultDpmY != 0);

    img.setDotsPerMeterX(0);
    img.setDotsPerMeterY(0);

    QCOMPARE(img.dotsPerMeterX(), defaultDpmX);
    QCOMPARE(img.dotsPerMeterY(), defaultDpmY);
}

void tst_QImage::rotate_data()
{
    QTest::addColumn<QImage::Format>("format");
    QTest::addColumn<int>("degrees");

    QVector<int> degrees;
    degrees << 0 << 90 << 180 << 270;

    foreach (int d, degrees) {
        QString title = QString("%1 %2").arg(d);
        QTest::newRow(qPrintable(title.arg("Format_RGB32")))
            << QImage::Format_RGB32 << d;
        QTest::newRow(qPrintable(title.arg("Format_ARGB32")))
            << QImage::Format_ARGB32 << d;
        QTest::newRow(qPrintable(title.arg("Format_ARGB32_Premultiplied")))
            << QImage::Format_ARGB32_Premultiplied << d;
        QTest::newRow(qPrintable(title.arg("Format_RGB16")))
            << QImage::Format_RGB16 << d;
        QTest::newRow(qPrintable(title.arg("Format_ARGB8565_Premultiplied")))
            << QImage::Format_ARGB8565_Premultiplied << d;
        QTest::newRow(qPrintable(title.arg("Format_RGB666")))
            << QImage::Format_RGB666 << d;
        QTest::newRow(qPrintable(title.arg("Format_RGB555")))
            << QImage::Format_RGB555 << d;
        QTest::newRow(qPrintable(title.arg("Format_ARGB8555_Premultiplied")))
            << QImage::Format_ARGB8555_Premultiplied << d;
        QTest::newRow(qPrintable(title.arg("Format_RGB888")))
            << QImage::Format_RGB888 << d;
        QTest::newRow(qPrintable(title.arg("Format_Indexed8")))
            << QImage::Format_Indexed8 << d;
    }
}

void tst_QImage::rotate()
{
    QFETCH(QImage::Format, format);
    QFETCH(int, degrees);

    // test if rotate90 is lossless
    int w = 54;
    int h = 13;
    QImage original(w, h, format);
    original.fill(qRgb(255,255,255));

    if (format == QImage::Format_Indexed8) {
        original.setColorCount(256);
        for (int i = 0; i < 255; ++i)
            original.setColor(i, qRgb(0, i, i));
    }

    if (original.colorTable().isEmpty()) {
        for (int x = 0; x < w; ++x) {
            original.setPixel(x,0, qRgb(x,0,128));
            original.setPixel(x,h - 1, qRgb(0,255 - x,128));
        }
        for (int y = 0; y < h; ++y) {
            original.setPixel(0, y, qRgb(y,0,255));
            original.setPixel(w - 1, y, qRgb(0,255 - y,255));
        }
    } else {
        const int n = original.colorTable().size();
        for (int x = 0; x < w; ++x) {
            original.setPixel(x, 0, x % n);
            original.setPixel(x, h - 1, n - (x % n) - 1);
        }
        for (int y = 0; y < h; ++y) {
            original.setPixel(0, y, y % n);
            original.setPixel(w - 1, y, n - (y % n) - 1);
        }
    }

    // original.save("rotated90_original.png", "png");

    // Initialize the matrix manually (do not use rotate) to avoid rounding errors
    QMatrix matRotate90;
    matRotate90.rotate(degrees);
    QImage dest = original;
    // And rotate it 4 times, then the image should be identical to the original
    for (int i = 0; i < 4 ; ++i) {
        dest = dest.transformed(matRotate90);
    }

    // Make sure they are similar in format before we compare them.
    dest = dest.convertToFormat(format);

    // dest.save("rotated90_result.png","png");
    QCOMPARE(original, dest);

    // Test with QMatrix::rotate 90 also, since we trust that now
    matRotate90.rotate(degrees);
    dest = original;
    // And rotate it 4 times, then the image should be identical to the original
    for (int i = 0; i < 4 ; ++i) {
        dest = dest.transformed(matRotate90);
    }

    // Make sure they are similar in format before we compare them.
    dest = dest.convertToFormat(format);

    QCOMPARE(original, dest);
}

void tst_QImage::copy()
{
    // Task 99250
    {
        QImage img(16,16,QImage::Format_ARGB32);
        img.copy(QRect(1000,1,1,1));
    }
}

void tst_QImage::setPixel_data()
{
    QTest::addColumn<int>("format");
    QTest::addColumn<uint>("value");
    QTest::addColumn<uint>("expected");

    QTest::newRow("ARGB32 red") << int(QImage::Format_ARGB32)
                                << 0xffff0000 << 0xffff0000;
    QTest::newRow("ARGB32 green") << int(QImage::Format_ARGB32)
                                  << 0xff00ff00 << 0xff00ff00;
    QTest::newRow("ARGB32 blue") << int(QImage::Format_ARGB32)
                                 << 0xff0000ff << 0xff0000ff;
    QTest::newRow("RGB16 red") << int(QImage::Format_RGB16)
                               << 0xffff0000 << 0xf800u;
    QTest::newRow("RGB16 green") << int(QImage::Format_RGB16)
                                 << 0xff00ff00 << 0x07e0u;
    QTest::newRow("RGB16 blue") << int(QImage::Format_RGB16)
                                << 0xff0000ff << 0x001fu;
    QTest::newRow("ARGB8565_Premultiplied red") << int(QImage::Format_ARGB8565_Premultiplied)
                                  << 0xffff0000 << 0xffff0000;
    QTest::newRow("ARGB8565_Premultiplied green") << int(QImage::Format_ARGB8565_Premultiplied)
                                    << 0xff00ff00 << 0xff00ff00;
    QTest::newRow("ARGB8565_Premultiplied blue") << int(QImage::Format_ARGB8565_Premultiplied)
                                   << 0xff0000ff << 0xff0000ff;
    QTest::newRow("RGB666 red") << int(QImage::Format_RGB666)
                                << 0xffff0000 << 0xffff0000;
    QTest::newRow("RGB666 green") << int(QImage::Format_RGB666)
                                  << 0xff00ff00 << 0xff00ff00;;
    QTest::newRow("RGB666 blue") << int(QImage::Format_RGB666)
                                 << 0xff0000ff << 0xff0000ff;
    QTest::newRow("RGB555 red") << int(QImage::Format_RGB555)
                                << 0xffff0000 << 0x7c00u;
    QTest::newRow("RGB555 green") << int(QImage::Format_RGB555)
                                  << 0xff00ff00 << 0x03e0u;
    QTest::newRow("RGB555 blue") << int(QImage::Format_RGB555)
                                 << 0xff0000ff << 0x001fu;
    QTest::newRow("ARGB8555_Premultiplied red") << int(QImage::Format_ARGB8555_Premultiplied)
                                  << 0xffff0000 << 0xffff0000;
    QTest::newRow("ARGB8555_Premultiplied green") << int(QImage::Format_ARGB8555_Premultiplied)
                                    << 0xff00ff00 << 0xff00ff00;
    QTest::newRow("ARGB8555_Premultiplied blue") << int(QImage::Format_ARGB8555_Premultiplied)
                                   << 0xff0000ff << 0xff0000ff;
    QTest::newRow("RGB888 red") << int(QImage::Format_RGB888)
                                << 0xffff0000 << 0xffff0000;
    QTest::newRow("RGB888 green") << int(QImage::Format_RGB888)
                                  << 0xff00ff00 << 0xff00ff00;
    QTest::newRow("RGB888 blue") << int(QImage::Format_RGB888)
                                 << 0xff0000ff << 0xff0000ff;
}

void tst_QImage::setPixel()
{
    QFETCH(int, format);
    QFETCH(uint, value);
    QFETCH(uint, expected);

    const int w = 13;
    const int h = 15;

    QImage img(w, h, QImage::Format(format));

    // fill image
    for (int y = 0; y < h; ++y)
        for (int x = 0; x < w; ++x)
            img.setPixel(x, y, value);

    // check pixel values
    switch (format) {
    case int(QImage::Format_RGB32):
    case int(QImage::Format_ARGB32):
    case int(QImage::Format_ARGB32_Premultiplied):
    {
        for (int y = 0; y < h; ++y) {
            const quint32 *row = (const quint32*)(img.scanLine(y));
            for (int x = 0; x < w; ++x) {
                quint32 result = row[x];
                if (result != expected)
                    printf("[x,y]: %d,%d, expected=%08x, result=%08x\n",
                           x, y, expected, result);
                QCOMPARE(uint(result), expected);
            }
        }
        break;
    }
    case int(QImage::Format_RGB555):
    case int(QImage::Format_RGB16):
    {
        for (int y = 0; y < h; ++y) {
            const quint16 *row = (const quint16*)(img.scanLine(y));
            for (int x = 0; x < w; ++x) {
                quint16 result = row[x];
                if (result != expected)
                    printf("[x,y]: %d,%d, expected=%04x, result=%04x\n",
                           x, y, expected, result);
                QCOMPARE(uint(result), expected);
            }
        }
        break;
    }
    case int(QImage::Format_RGB666):
    {
        for (int y = 0; y < h; ++y) {
            const qrgb666 *row = (const qrgb666*)(img.scanLine(y));
            for (int x = 0; x < w; ++x) {
                quint32 result = row[x];
                if (result != expected)
                    printf("[x,y]: %d,%d, expected=%04x, result=%04x\n",
                           x, y, expected, result);
                QCOMPARE(result, expected);
            }
        }
        break;
    }
    case int(QImage::Format_ARGB8565_Premultiplied):
    {
        for (int y = 0; y < h; ++y) {
            const qargb8565 *row = (const qargb8565*)(img.scanLine(y));
            for (int x = 0; x < w; ++x) {
                quint32 result = row[x];
                if (result != expected)
                    printf("[x,y]: %d,%d, expected=%04x, result=%04x\n",
                           x, y, expected, result);
                QCOMPARE(result, expected);
            }
        }
        break;
    }
    case int(QImage::Format_ARGB8555_Premultiplied):
    {
        for (int y = 0; y < h; ++y) {
            const qargb8555 *row = (const qargb8555*)(img.scanLine(y));
            for (int x = 0; x < w; ++x) {
                quint32 result = row[x];
                if (result != expected)
                    printf("[x,y]: %d,%d, expected=%04x, result=%04x\n",
                           x, y, expected, result);
                QCOMPARE(result, expected);
            }
        }
        break;
    }
    case int(QImage::Format_RGB888):
    {
        for (int y = 0; y < h; ++y) {
            const qrgb888 *row = (const qrgb888*)(img.scanLine(y));
            for (int x = 0; x < w; ++x) {
                qrgb888 result = row[x];
                if (result != expected)
                    printf("[x,y]: %d,%d, expected=%04x, result=%04x\n",
                           x, y, expected, quint32(result));
                QCOMPARE(uint(result), expected);
            }
        }
        break;
    }
    default:
        qFatal("Test not implemented for format %d", format);
    }
}

void tst_QImage::convertToFormatPreserveDotsPrMeter()
{
    QImage img(100, 100, QImage::Format_ARGB32_Premultiplied);

    int dpmx = 123;
    int dpmy = 234;
    img.setDotsPerMeterX(dpmx);
    img.setDotsPerMeterY(dpmy);
    img.fill(0x12345678);

    img = img.convertToFormat(QImage::Format_RGB32);

    QCOMPARE(img.dotsPerMeterX(), dpmx);
    QCOMPARE(img.dotsPerMeterY(), dpmy);
}

#ifndef QT_NO_IMAGE_TEXT
void tst_QImage::convertToFormatPreserveText()
{
    QImage img(100, 100, QImage::Format_ARGB32_Premultiplied);

    img.setText("foo", "bar");
    img.setText("foo2", "bar2");
    img.fill(0x12345678);

    QStringList listResult;
    listResult << "foo" << "foo2";
    QString result = "foo: bar\n\nfoo2: bar2";

    QImage imgResult1 = img.convertToFormat(QImage::Format_RGB32);
    QCOMPARE(imgResult1.text(), result);
    QCOMPARE(imgResult1.textKeys(), listResult);

    QVector<QRgb> colorTable(4);
    for (int i = 0; i < 4; ++i)
        colorTable[i] = QRgb(42);
    QImage imgResult2 = img.convertToFormat(QImage::Format_MonoLSB,
                                            colorTable);
    QCOMPARE(imgResult2.text(), result);
    QCOMPARE(imgResult2.textKeys(), listResult);
}
#endif // QT_NO_IMAGE_TEXT

void tst_QImage::setColorCount()
{
    QImage img(0, 0, QImage::Format_Indexed8);
    QTest::ignoreMessage(QtWarningMsg, "QImage::setColorCount: null image");
    img.setColorCount(256);
    QCOMPARE(img.colorCount(), 0);
}

void tst_QImage::setColor()
{
    QImage img(0, 0, QImage::Format_Indexed8);
    img.setColor(0, qRgba(18, 219, 108, 128));
    QCOMPARE(img.colorCount(), 0);

    QImage img2(1, 1, QImage::Format_Indexed8);
    img2.setColor(0, qRgba(18, 219, 108, 128));
    QCOMPARE(img2.colorCount(), 1);
}

/* Just some sanity checking that we don't draw outside the buffer of
 * the image. Hopefully this will create crashes or at least some
 * random test fails when broken.
 */
void tst_QImage::rasterClipping()
{
    QImage image(10, 10, QImage::Format_RGB32);
    image.fill(0xffffffff);

    QPainter p(&image);

    p.drawLine(-1000, 5, 5, 5);
    p.drawLine(-1000, 5, 1000, 5);
    p.drawLine(5, 5, 1000, 5);

    p.drawLine(5, -1000, 5, 5);
    p.drawLine(5, -1000, 5, 1000);
    p.drawLine(5, 5, 5, 1000);

    p.setBrush(Qt::red);

    p.drawEllipse(3, 3, 4, 4);
    p.drawEllipse(-100, -100, 210, 210);

    p.drawEllipse(-1000, 0, 2010, 2010);
    p.drawEllipse(0, -1000, 2010, 2010);
    p.drawEllipse(-2010, -1000, 2010, 2010);
    p.drawEllipse(-1000, -2010, 2010, 2010);
    QVERIFY(true);
}

// Tests the new QPoint overloads in QImage in Qt 4.2
void tst_QImage::pointOverloads()
{
    QImage image(100, 100, QImage::Format_RGB32);
    image.fill(0xff00ff00);

    // IsValid
    QVERIFY(image.valid(QPoint(0, 0)));
    QVERIFY(image.valid(QPoint(99, 0)));
    QVERIFY(image.valid(QPoint(0, 99)));
    QVERIFY(image.valid(QPoint(99, 99)));

    QVERIFY(!image.valid(QPoint(50, -1))); // outside on the top
    QVERIFY(!image.valid(QPoint(50, 100))); // outside on the bottom
    QVERIFY(!image.valid(QPoint(-1, 50))); // outside on the left
    QVERIFY(!image.valid(QPoint(100, 50))); // outside on the right

    // Test the pixel setter
    image.setPixel(QPoint(10, 10), 0xff0000ff);
    QCOMPARE(image.pixel(10, 10), 0xff0000ff);

    // pixel getter
    QCOMPARE(image.pixel(QPoint(10, 10)), 0xff0000ff);

    // pixelIndex()
    QImage indexed = image.convertToFormat(QImage::Format_Indexed8);
    QCOMPARE(indexed.pixelIndex(10, 10), indexed.pixelIndex(QPoint(10, 10)));
}

void tst_QImage::destructor()
{
    QPolygon poly(6);
    poly.setPoint(0,-1455, 1435);

    QImage image(100, 100, QImage::Format_RGB32);
    QPainter ptPix(&image);
    ptPix.setPen(Qt::black);
    ptPix.setBrush(Qt::black);
    ptPix.drawPolygon(poly, Qt::WindingFill);
    ptPix.end();

}


/* XPM */
static const char *monoPixmap[] = {
/* width height ncolors chars_per_pixel */
"4 4 2 1",
"x c #000000",
". c #ffffff",
/* pixels */
"xxxx",
"x..x",
"x..x",
"xxxx"
};


#ifndef QT_NO_IMAGE_HEURISTIC_MASK
void tst_QImage::createHeuristicMask()
{
    QImage img(monoPixmap);
    img = img.convertToFormat(QImage::Format_MonoLSB);
    QImage mask = img.createHeuristicMask();
    QImage newMask = mask.convertToFormat(QImage::Format_ARGB32_Premultiplied);

    // line 2
    QVERIFY(newMask.pixel(0,1) != newMask.pixel(1,1));
    QVERIFY(newMask.pixel(1,1) == newMask.pixel(2,1));
    QVERIFY(newMask.pixel(2,1) != newMask.pixel(3,1));

    // line 3
    QVERIFY(newMask.pixel(0,2) != newMask.pixel(1,2));
    QVERIFY(newMask.pixel(1,2) == newMask.pixel(2,2));
    QVERIFY(newMask.pixel(2,2) != newMask.pixel(3,2));
}
#endif

void tst_QImage::cacheKey()
{
    QImage image1(2, 2, QImage::Format_RGB32);
    qint64 image1_key = image1.cacheKey();
    QImage image2 = image1;

    QVERIFY(image2.cacheKey() == image1.cacheKey());
    image2.detach();
    QVERIFY(image2.cacheKey() != image1.cacheKey());
    QVERIFY(image1.cacheKey() == image1_key);
}

void tst_QImage::smoothScale()
{
    unsigned int data[2] = { qRgba(0, 0, 0, 0), qRgba(128, 128, 128, 128) };

    QImage imgX((unsigned char *)data, 2, 1, QImage::Format_ARGB32_Premultiplied);
    QImage imgY((unsigned char *)data, 1, 2, QImage::Format_ARGB32_Premultiplied);

    QImage scaledX = imgX.scaled(QSize(4, 1), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    QImage scaledY = imgY.scaled(QSize(1, 4), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

    uint *scaled[2] = {
        (unsigned int *)scaledX.bits(),
        (unsigned int *)scaledY.bits()
    };

    int expected[4] = { 0, 32, 96, 128 };
    for (int image = 0; image < 2; ++image) {
        for (int index = 0; index < 4; ++index) {
            for (int component = 0; component < 4; ++component) {
                int pixel = scaled[image][index];
                int val = (pixel >> (component * 8)) & 0xff;

                QCOMPARE(val, expected[index]);
            }
        }
    }
}

// test area sampling
void tst_QImage::smoothScale2()
{
    int sizes[] = { 2, 4, 8, 10, 16, 20, 32, 40, 64, 100, 101, 128, 0 };
    QImage::Format formats[] = { QImage::Format_ARGB32, QImage::Format_RGB32, QImage::Format_Invalid };
    for (int i = 0; sizes[i] != 0; ++i) {
        for (int j = 0; formats[j] != QImage::Format_Invalid; ++j) {
            int size = sizes[i];

            QRgb expected = formats[j] == QImage::Format_ARGB32 ? qRgba(63, 127, 255, 255) : qRgb(63, 127, 255);

            QImage img(size, size, formats[j]);
            img.fill(expected);

            // scale x down, y down
            QImage scaled = img.scaled(QSize(1, 1), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
            QRgb pixel = scaled.pixel(0, 0);
            QCOMPARE(qAlpha(pixel), qAlpha(expected));
            QCOMPARE(qRed(pixel), qRed(expected));
            QCOMPARE(qGreen(pixel), qGreen(expected));
            QCOMPARE(qBlue(pixel), qBlue(expected));

            // scale x down, y up
            scaled = img.scaled(QSize(1, size * 2), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
            for (int y = 0; y < scaled.height(); ++y) {
                pixel = scaled.pixel(0, y);
                QCOMPARE(qAlpha(pixel), qAlpha(expected));
                QCOMPARE(qRed(pixel), qRed(expected));
                QCOMPARE(qGreen(pixel), qGreen(expected));
                QCOMPARE(qBlue(pixel), qBlue(expected));
            }

            // scale x up, y down
            scaled = img.scaled(QSize(size * 2, 1), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
            for (int x = 0; x < scaled.width(); ++x) {
                pixel = scaled.pixel(x, 0);
                QCOMPARE(qAlpha(pixel), qAlpha(expected));
                QCOMPARE(qRed(pixel), qRed(expected));
                QCOMPARE(qGreen(pixel), qGreen(expected));
                QCOMPARE(qBlue(pixel), qBlue(expected));
            }

            // scale x up, y up
            scaled = img.scaled(QSize(size * 2, size * 2), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
            for (int y = 0; y < scaled.height(); ++y) {
                for (int x = 0; x < scaled.width(); ++x) {
                    pixel = scaled.pixel(x, y);
                    QCOMPARE(qAlpha(pixel), qAlpha(expected));
                    QCOMPARE(qRed(pixel), qRed(expected));
                    QCOMPARE(qGreen(pixel), qGreen(expected));
                    QCOMPARE(qBlue(pixel), qBlue(expected));
                }
            }
        }
    }
}

static inline int rand8()
{
    return int(256. * (qrand() / (RAND_MAX + 1.0)));
}

static inline bool compare(int a, int b, int tolerance)
{
    return qAbs(a - b) <= tolerance;
}

// compares img.scale against the bilinear filtering used by QPainter
void tst_QImage::smoothScale3()
{
    QImage img(128, 128, QImage::Format_RGB32);
    for (int y = 0; y < img.height(); ++y) {
        for (int x = 0; x < img.width(); ++x) {
            const int red = rand8();
            const int green = rand8();
            const int blue = rand8();
            const int alpha = 255;

            img.setPixel(x, y, qRgba(red, green, blue, alpha));
        }
    }

    qreal scales[2] = { .5, 2 };

    for (int i = 0; i < 2; ++i) {
        QImage a = img.scaled(img.size() * scales[i], Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        QImage b(a.size(), a.format());
        b.fill(0x0);

        QPainter p(&b);
        p.setRenderHint(QPainter::SmoothPixmapTransform);
        p.scale(scales[i], scales[i]);
        p.drawImage(0, 0, img);
        p.end();

        for (int y = 0; y < a.height(); ++y) {
            for (int x = 0; x < a.width(); ++x) {
                QRgb ca = a.pixel(x, y);
                QRgb cb = b.pixel(x, y);

                // tolerate a little bit of rounding errors
                QVERIFY(compare(qRed(ca), qRed(cb), 3));
                QVERIFY(compare(qGreen(ca), qGreen(cb), 3));
                QVERIFY(compare(qBlue(ca), qBlue(cb), 3));
            }
        }
    }
}

void tst_QImage::smoothScaleBig()
{
#if defined(Q_OS_WINCE)
    int bigValue = 2000;
#elif defined(Q_OS_SYMBIAN)
    int bigValue = 2000;
#else
    int bigValue = 200000;
#endif
    QImage tall(4, bigValue, QImage::Format_ARGB32);
    tall.fill(0x0);

    QImage wide(bigValue, 4, QImage::Format_ARGB32);
    wide.fill(0x0);

    QImage tallScaled = tall.scaled(4, tall.height() / 4, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    QImage wideScaled = wide.scaled(wide.width() / 4, 4, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

    QCOMPARE(tallScaled.pixel(0, 0), QRgb(0x0));
    QCOMPARE(wideScaled.pixel(0, 0), QRgb(0x0));
}

void tst_QImage::smoothScaleAlpha()
{
    QImage src(128, 128, QImage::Format_ARGB32_Premultiplied);
    src.fill(0x0);

    QPainter srcPainter(&src);
    srcPainter.setPen(Qt::NoPen);
    srcPainter.setBrush(Qt::white);
    srcPainter.drawEllipse(QRect(QPoint(), src.size()));
    srcPainter.end();

    QImage dst(32, 32, QImage::Format_ARGB32_Premultiplied);
    dst.fill(0xffffffff);
    QImage expected = dst;

    QPainter dstPainter(&dst);
    dstPainter.drawImage(0, 0, src.scaled(dst.size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    dstPainter.end();

    QCOMPARE(dst, expected);
}

static int count(const QImage &img, int x, int y, int dx, int dy, QRgb pixel)
{
    int i = 0;
    while (x >= 0 && x < img.width() && y >= 0 && y < img.height()) {
        i += (img.pixel(x, y) == pixel);
        x += dx;
        y += dy;
    }
    return i;
}

const int transformed_image_width = 128;
const int transformed_image_height = 128;

void tst_QImage::transformed_data()
{
    QTest::addColumn<QTransform>("transform");

    {
        QTransform transform;
        transform.translate(10.4, 10.4);
        QTest::newRow("Translate") << transform;
    }
    {
        QTransform transform;
        transform.scale(1.5, 1.5);
        QTest::newRow("Scale") << transform;
    }
    {
        QTransform transform;
        transform.rotate(30);
        QTest::newRow("Rotate 30") << transform;
    }
    {
        QTransform transform;
        transform.rotate(90);
        QTest::newRow("Rotate 90") << transform;
    }
    {
        QTransform transform;
        transform.rotate(180);
        QTest::newRow("Rotate 180") << transform;
    }
    {
        QTransform transform;
        transform.rotate(270);
        QTest::newRow("Rotate 270") << transform;
    }
    {
        QTransform transform;
        transform.translate(transformed_image_width/2, transformed_image_height/2);
        transform.rotate(155, Qt::XAxis);
        transform.translate(-transformed_image_width/2, -transformed_image_height/2);
        QTest::newRow("Perspective 1") << transform;
    }
    {
        QTransform transform;
        transform.rotate(155, Qt::XAxis);
        transform.translate(-transformed_image_width/2, -transformed_image_height/2);
        QTest::newRow("Perspective 2") << transform;
    }
}

void tst_QImage::transformed()
{
    QFETCH(QTransform, transform);

    QImage img(transformed_image_width, transformed_image_height, QImage::Format_ARGB32_Premultiplied);
    QPainter p(&img);
    p.fillRect(0, 0, img.width(), img.height(), Qt::red);
    p.drawRect(0, 0, img.width()-1, img.height()-1);
    p.end();

    QImage transformed = img.transformed(transform, Qt::SmoothTransformation);

    // all borders should have touched pixels

    QVERIFY(count(transformed, 0, 0, 1, 0, 0x0) < transformed.width());
    QVERIFY(count(transformed, 0, 0, 0, 1, 0x0) < transformed.height());

    QVERIFY(count(transformed, 0, img.height() - 1, 1, 0, 0x0) < transformed.width());
    QVERIFY(count(transformed, img.width() - 1, 0, 0, 1, 0x0) < transformed.height());

    QImage transformedPadded(transformed.width() + 2, transformed.height() + 2, img.format());
    transformedPadded.fill(0x0);

    p.begin(&transformedPadded);
    p.setRenderHint(QPainter::SmoothPixmapTransform);
    p.setRenderHint(QPainter::Antialiasing);
    p.setTransform(transformed.trueMatrix(transform, img.width(), img.height()) * QTransform().translate(1, 1));
    p.drawImage(0, 0, img);
    p.end();

    // no borders should have touched pixels since we have a one-pixel padding

    QCOMPARE(count(transformedPadded, 0, 0, 1, 0, 0x0), transformedPadded.width());
    QCOMPARE(count(transformedPadded, 0, 0, 0, 1, 0x0), transformedPadded.height());

    QCOMPARE(count(transformedPadded, 0, transformedPadded.height() - 1, 1, 0, 0x0), transformedPadded.width());
    QCOMPARE(count(transformedPadded, transformedPadded.width() - 1, 0, 0, 1, 0x0), transformedPadded.height());
}

void tst_QImage::transformed2()
{
    QImage img(3, 3, QImage::Format_Mono);
    QPainter p(&img);
    p.fillRect(0, 0, 3, 3, Qt::white);
    p.fillRect(0, 0, 3, 3, Qt::Dense4Pattern);
    p.end();

    QTransform transform;
    transform.scale(3, 3);

    QImage expected(9, 9, QImage::Format_Mono);
    p.begin(&expected);
    p.fillRect(0, 0, 9, 9, Qt::white);
    p.setBrush(Qt::black);
    p.setPen(Qt::NoPen);
    p.drawRect(3, 0, 3, 3);
    p.drawRect(0, 3, 3, 3);
    p.drawRect(6, 3, 3, 3);
    p.drawRect(3, 6, 3, 3);
    p.end();

    {
        QImage actual = img.transformed(transform);

        QCOMPARE(actual.format(), expected.format());
        QCOMPARE(actual.size(), expected.size());
        QCOMPARE(actual, expected);
    }

    {
        transform.rotate(-90);
        QImage actual = img.transformed(transform);

        QCOMPARE(actual.convertToFormat(QImage::Format_ARGB32_Premultiplied),
                 expected.convertToFormat(QImage::Format_ARGB32_Premultiplied));
    }
}

void tst_QImage::scaled()
{
    QImage img(102, 3, QImage::Format_Mono);
    QPainter p(&img);
    p.fillRect(0, 0, img.width(), img.height(), Qt::white);
    p.end();

    QImage scaled = img.scaled(1994, 10);

    QImage expected(1994, 10, QImage::Format_Mono);
    p.begin(&expected);
    p.fillRect(0, 0, expected.width(), expected.height(), Qt::white);
    p.end();

    QCOMPARE(scaled, expected);
}

void tst_QImage::paintEngine()
{
    QImage img;

    QPaintEngine *engine;
    {
        QImage temp(100, 100, QImage::Format_RGB32);
        temp.fill(0xff000000);

        QPainter p(&temp);
        p.fillRect(80,80,10,10,Qt::blue);
        p.end();

        img = temp;

        engine = temp.paintEngine();
    }

    {
        QPainter p(&img);

        p.fillRect(80,10,10,10,Qt::yellow);
        p.end();
    }

    QImage expected(100, 100, QImage::Format_RGB32);
    expected.fill(0xff000000);

    QPainter p(&expected);
    p.fillRect(80,80,10,10,Qt::blue);
    p.fillRect(80,10,10,10,Qt::yellow);
    p.end();

    QCOMPARE(engine, img.paintEngine());
    QCOMPARE(img, expected);
}

void tst_QImage::setAlphaChannelWhilePainting()
{
    QImage image(100, 100, QImage::Format_ARGB32);
    image.fill(Qt::black);
    QPainter p(&image);

    image.setAlphaChannel(image.createMaskFromColor(QColor(Qt::black).rgb(), Qt::MaskInColor));
}


// See task 240047 for details
void tst_QImage::smoothScaledSubImage()
{
    QImage original(128, 128, QImage::Format_RGB32);
    QPainter p(&original);
    p.fillRect(0, 0, 64, 128, Qt::black);
    p.fillRect(64, 0, 64, 128, Qt::white);
    p.end();

    QImage subimage(((const QImage &) original).bits(), 32, 32, original.bytesPerLine(), QImage::Format_RGB32); // only in the black part of the source...

    QImage scaled = subimage.scaled(8, 8, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

    for (int y=0; y<scaled.height(); ++y)
        for (int x=0; x<scaled.width(); ++x)
            QCOMPARE(scaled.pixel(x, y), 0xff000000);
}

void tst_QImage::nullSize_data()
{
    QTest::addColumn<QImage>("image");
    QTest::newRow("null image") << QImage();
    QTest::newRow("zero-size image") << QImage(0, 0, QImage::Format_RGB32);
}

void tst_QImage::nullSize()
{
    QFETCH(QImage, image);
    QCOMPARE(image.isNull(), true);
    QCOMPARE(image.width(), image.size().width());
    QCOMPARE(image.height(), image.size().height());
}

void tst_QImage::premultipliedAlphaConsistency()
{
    QImage img(256, 1, QImage::Format_ARGB32);
    for (int x = 0; x < 256; ++x)
        img.setPixel(x, 0, (x << 24) | 0xffffff);

    QImage converted = img.convertToFormat(QImage::Format_ARGB8565_Premultiplied);
    QImage pm32 = converted.convertToFormat(QImage::Format_ARGB32_Premultiplied);

    for (int i = 0; i < pm32.width(); ++i) {
        QRgb pixel = pm32.pixel(i, 0);
        QVERIFY(qRed(pixel) <= qAlpha(pixel));
        QVERIFY(qGreen(pixel) <= qAlpha(pixel));
        QVERIFY(qBlue(pixel) <= qAlpha(pixel));
    }
}

void tst_QImage::compareIndexed()
{
    QImage img(256, 1, QImage::Format_Indexed8);

    QVector<QRgb> colorTable(256);
    for (int i = 0; i < 256; ++i)
        colorTable[i] = qRgb(i, i, i);
    img.setColorTable(colorTable);

    for (int i = 0; i < 256; ++i) {
        img.setPixel(i, 0, i);
    }

    QImage imgInverted(256, 1, QImage::Format_Indexed8);
    QVector<QRgb> invertedColorTable(256);
    for (int i = 0; i < 256; ++i)
        invertedColorTable[255-i] = qRgb(i, i, i);
    imgInverted.setColorTable(invertedColorTable);

    for (int i = 0; i < 256; ++i) {
        imgInverted.setPixel(i, 0, (255-i));
    }

    QCOMPARE(img, imgInverted);
}

QTEST_MAIN(tst_QImage)
#include "tst_qimage.moc"
