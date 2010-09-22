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
#include <qpixmap.h>
#include <qbitmap.h>
#include <qimage.h>
#include <qimagereader.h>
#include <qmatrix.h>
#include <qdesktopwidget.h>
#include <qpaintengine.h>
#include <qtreewidget.h>
#include <qsplashscreen.h>

#include <private/qpixmapdata_p.h>

#include <QSet>

#ifdef Q_WS_WIN
#include <windows.h>
#endif

#ifdef Q_WS_QWS
#include <qscreen_qws.h>
#endif

#ifdef Q_OS_SYMBIAN
#include <e32std.h>
#include <fbs.h>
#include <gdi.h>
#include <bitdev.h>
#endif

#ifdef Q_WS_X11
#include <QX11Info>
#endif

//TESTED_CLASS=
//TESTED_FILES=
#if defined(Q_OS_SYMBIAN)
# define SRCDIR ""
#endif
Q_DECLARE_METATYPE(QImage::Format)

class tst_QPixmap : public QObject
{
    Q_OBJECT

public:
    tst_QPixmap();
    virtual ~tst_QPixmap();


public slots:
    void init();
    void cleanup();

private slots:
    void setAlphaChannel_data();
    void setAlphaChannel();

    void fromImage_data();
    void fromImage();

    void fromUninitializedImage_data();
    void fromUninitializedImage();

    void convertFromImage_data();
    void convertFromImage();

    void testMetrics();

    void scroll_data();
    void scroll();

    void fill_data();
    void fill();
    void fill_transparent();

    void createMaskFromColor();

    void mask();
    void bitmapMask();
    void setGetMask_data();
    void setGetMask();
    void cacheKey();
    void drawBitmap();
    void grabWidget();
    void grabWindow();
    void isNull();
    void task_246446();

#ifdef Q_WS_QWS
    void convertFromImageNoDetach();
#endif

    void convertFromImageDetach();

#if defined(Q_WS_WIN)
    void toWinHBITMAP_data();
    void toWinHBITMAP();
    void fromWinHBITMAP_data();
    void fromWinHBITMAP();

    void toWinHICON_data();
    void toWinHICON();
    void fromWinHICON_data();
    void fromWinHICON();
#endif

#if defined(Q_OS_SYMBIAN)
    void fromSymbianCFbsBitmap_data();
    void fromSymbianCFbsBitmap();
    void toSymbianCFbsBitmap_data();
    void toSymbianCFbsBitmap();
#endif

    void onlyNullPixmapsOutsideGuiThread();
    void refUnref();

    void copy();
    void depthOfNullObjects();

#ifdef QT3_SUPPORT
    void resize();
    void resizePreserveMask();
#endif

    void transformed();
    void transformed2();

    void fromImage_crash();

    void fromData();
    void loadFromDataNullValues();

    void loadFromDataImage_data();
    void loadFromDataImage();

    void fromImageReader_data();
    void fromImageReader();

    void fromImageReaderAnimatedGif();

    void preserveDepth();
    void splash_crash();

    void loadAsBitmapOrPixmap();
};

static bool lenientCompare(const QPixmap &actual, const QPixmap &expected)
{
    QImage expectedImage = expected.toImage().convertToFormat(QImage::Format_RGB32);
    QImage actualImage = actual.toImage().convertToFormat(QImage::Format_RGB32);

    if (expectedImage.size() != actualImage.size())
        return false;

    int size = actual.width() * actual.height();

    int threshold = 2;
#ifdef Q_WS_X11
    if (QX11Info::appDepth() == 16)
        threshold = 10;
#endif

    QRgb *a = (QRgb *)actualImage.bits();
    QRgb *e = (QRgb *)expectedImage.bits();
    for (int i = 0; i < size; ++i) {
        QColor ca(a[i]);
        QColor ce(e[i]);

        bool result = true;

        if (qAbs(ca.red() - ce.red()) > threshold)
            result = false;
        if (qAbs(ca.green() - ce.green()) > threshold)
            result = false;
        if (qAbs(ca.blue() - ce.blue()) > threshold)
            result = false;

        if (!result)
            return false;
    }

    return true;
}

Q_DECLARE_METATYPE(QImage)
Q_DECLARE_METATYPE(QPixmap)
Q_DECLARE_METATYPE(QMatrix)
Q_DECLARE_METATYPE(QBitmap)

tst_QPixmap::tst_QPixmap()
{
}

tst_QPixmap::~tst_QPixmap()
{
}

void tst_QPixmap::init()
{
}

void tst_QPixmap::cleanup()
{
}

void tst_QPixmap::setAlphaChannel_data()
{
    QTest::addColumn<int>("red");
    QTest::addColumn<int>("green");
    QTest::addColumn<int>("blue");
    QTest::addColumn<int>("alpha");

    QTest::newRow("red 0") << 255 << 0 << 0 << 0;
    QTest::newRow("red 24") << 255 << 0 << 0 << 24;
    QTest::newRow("red 124") << 255 << 0 << 0 << 124;
    QTest::newRow("red 255") << 255 << 0 << 0 << 255;

    QTest::newRow("green 0") << 0 << 255 << 0 << 0;
    QTest::newRow("green 24") << 0 << 255 << 0 << 24;
    QTest::newRow("green 124") << 0 << 255 << 0 << 124;
    QTest::newRow("green 255") << 0 << 255 << 0 << 255;

    QTest::newRow("blue 0") << 0 << 0 << 255 << 0;
    QTest::newRow("blue 24") << 0 << 0 << 255 << 24;
    QTest::newRow("blue 124") << 0 << 0 << 255 << 124;
    QTest::newRow("blue 255") << 0 << 0 << 255 << 255;
}

void tst_QPixmap::setAlphaChannel()
{
    QFETCH(int, red);
    QFETCH(int, green);
    QFETCH(int, blue);
    QFETCH(int, alpha);

    int width = 100;
    int height = 100;

    QPixmap pixmap(width, height);
    pixmap.fill(QColor(red, green, blue));

    QPixmap alphaChannel(width, height);
    alphaChannel.fill(QColor(alpha, alpha, alpha));
    pixmap.setAlphaChannel(alphaChannel);

#ifdef Q_WS_X11
    if (pixmap.pixmapData()->classId() == QPixmapData::X11Class && !pixmap.x11PictureHandle())
        QSKIP("Requires XRender support", SkipAll);
#endif

    QImage result;
    bool ok = true;

    QPixmap outAlpha = pixmap.alphaChannel();
    QCOMPARE(outAlpha.size(), pixmap.size());

    result = outAlpha.toImage().convertToFormat(QImage::Format_ARGB32);;
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            ok &= qGray(result.pixel(x, y)) == alpha;
        }
    }
    QVERIFY(ok);

    result = pixmap.toImage().convertToFormat(QImage::Format_ARGB32);
    QRgb expected = alpha == 0 ? 0 : qRgba(red, green, blue, alpha);
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if (result.colorCount() > 0) {
                ok &= result.pixelIndex(x, y) == expected;
            } else {
                ok &= result.pixel(x, y) == expected;
            }
        }
    }
    QVERIFY(ok);
}

void tst_QPixmap::fromImage_data()
{
    bool is16bit = false;
#ifdef Q_WS_X11
    if (QX11Info::appDepth() == 16)
        is16bit = true;
#endif

    QTest::addColumn<QImage::Format>("format");

    QTest::newRow("Format_Mono") << QImage::Format_Mono;
    QTest::newRow("Format_MonoLSB") << QImage::Format_MonoLSB;
//    QTest::newRow("Format_Indexed8") << QImage::Format_Indexed8;
    if (!is16bit)
        QTest::newRow("Format_RGB32") << QImage::Format_RGB32;
    QTest::newRow("Format_ARGB32") << QImage::Format_ARGB32;
    QTest::newRow("Format_ARGB32_Premultiplied") << QImage::Format_ARGB32_Premultiplied;
    if (!is16bit)
        QTest::newRow("Format_RGB16") << QImage::Format_RGB16;
}

void tst_QPixmap::fromImage()
{
    QFETCH(QImage::Format, format);

    QImage image(37, 16, format);

    if (image.colorCount() == 2) {
        image.setColor(0, QColor(Qt::color0).rgba());
        image.setColor(1, QColor(Qt::color1).rgba());
    }
    image.fill(0x7f7f7f7f);

    const QPixmap pixmap = QPixmap::fromImage(image);
#ifdef Q_WS_X11
    if (pixmap.pixmapData()->classId() == QPixmapData::X11Class && !pixmap.x11PictureHandle())
        QSKIP("Requires XRender support", SkipAll);
#endif
    const QImage result = pixmap.toImage();
    image = image.convertToFormat(result.format());
    QCOMPARE(result, image);
}


void tst_QPixmap::fromUninitializedImage_data()
{
    QTest::addColumn<QImage::Format>("format");

    QTest::newRow("Format_Mono") << QImage::Format_Mono;
    QTest::newRow("Format_MonoLSB") << QImage::Format_MonoLSB;
    QTest::newRow("Format_Indexed8") << QImage::Format_Indexed8;
    QTest::newRow("Format_RGB32") << QImage::Format_RGB32;
    QTest::newRow("Format_ARGB32") << QImage::Format_ARGB32;
    QTest::newRow("Format_ARGB32_Premultiplied") << QImage::Format_ARGB32_Premultiplied;
    QTest::newRow("Format_RGB16") << QImage::Format_RGB16;
}

void tst_QPixmap::fromUninitializedImage()
{
    QFETCH(QImage::Format, format);

    QImage image(100, 100, format);
    QPixmap pix = QPixmap::fromImage(image);

    // it simply shouldn't crash...
    QVERIFY(true);

}

void tst_QPixmap::convertFromImage_data()
{
    QTest::addColumn<QImage>("img1");
    QTest::addColumn<QImage>("img2");
#ifdef Q_OS_SYMBIAN
    const QString prefix = QLatin1String(SRCDIR) + "convertFromImage";
#else
    const QString prefix = QLatin1String(SRCDIR) + "/convertFromImage";
#endif
    {
        QImage img1;
        QImage img2;
        QVERIFY(img1.load(prefix + "/task31722_0/img1.png"));
        QVERIFY(img2.load(prefix + "/task31722_0/img2.png"));
        QVERIFY(img1.load(prefix + "/task31722_0/img1.png"));
        QVERIFY(img2.load(prefix + "/task31722_0/img2.png"));
        QTest::newRow("Task 31722 0") << img1 << img2;
    }
    {
        QImage img1;
        QImage img2;
        QVERIFY(img1.load(prefix + "/task31722_1/img1.png"));
        QVERIFY(img2.load(prefix + "/task31722_1/img2.png"));
        QTest::newRow("Task 31722 1") << img1 << img2;
    }
}

void tst_QPixmap::convertFromImage()
{
    QFETCH(QImage, img1);
    QFETCH(QImage, img2);

    QPixmap pix = QPixmap::fromImage(img1);
    pix = QPixmap::fromImage(img2);

    QPixmap res = QPixmap::fromImage(img2);
    QVERIFY( pixmapsAreEqual(&pix, &res) );
}

void tst_QPixmap::scroll_data()
{
    QTest::addColumn<QImage>("input");
    QTest::addColumn<int>("dx");
    QTest::addColumn<int>("dy");
    QTest::addColumn<QRect>("rect");
    QTest::addColumn<QRegion>("exposed");
    QTest::addColumn<bool>("newPix");

    QImage input(":/images/designer.png");

    // Noop tests
    QTest::newRow("null") << QImage() << 0 << 0 << QRect() << QRegion() << false;
    QTest::newRow("dx_0_dy_0_null") << input << 0 << 0 << QRect() << QRegion() << false;
    QTest::newRow("dx_1_dy_0_null") << input << 1 << 0 << QRect() << QRegion() << false;
    QTest::newRow("dx_0_dy_1_null") << input << 0 << 1 << QRect() << QRegion() << false;
    QTest::newRow("dx_0_dy_0_x_y_w_h") << input << 0 << 0 << input.rect() << QRegion() << false;

    QRegion r;
    // Scroll whole pixmap
    r = QRegion(); r += QRect(0, 0, 128, 10);
    QTest::newRow("dx_0_dy_10_x_y_w_h") << input << 0 << 10 << input.rect() << r << true;
    r = QRegion(); r += QRect(0, 0, 10, 128);
    QTest::newRow("dx_10_dy_0_x_y_w_h") << input << 10 << 0 << input.rect() << r << true;
    r = QRegion(); r += QRect(0, 0, 128, 10); r += QRect(0, 10, 10, 118);
    QTest::newRow("dx_10_dy_10_x_y_w_h") << input << 10 << 10 << input.rect() << r << true;
    r = QRegion(); r += QRect(118, 0, 10, 128);
    QTest::newRow("dx_-10_dy_0_x_y_w_h") << input << -10 << 0 << input.rect() << r << true;
    r = QRegion(); r += QRect(0, 118, 128, 10);
    QTest::newRow("dx_0_dy_-10_x_y_w_h") << input << 0 << -10 << input.rect() << r << true;
    r = QRegion(); r += QRect(118, 0, 10, 118); r += QRect(0, 118, 128, 10);
    QTest::newRow("dx_-10_dy_-10_x_y_w_h") << input << -10 << -10 << input.rect() << r << true;

    // Scroll part of pixmap
    QTest::newRow("dx_0_dy_0_50_50_100_100") << input << 0 << 0 << QRect(50, 50, 100, 100) << QRegion() << false;
    r = QRegion(); r += QRect(50, 50, 10, 78);
    QTest::newRow("dx_10_dy_0_50_50_100_100") << input << 10 << 0 << QRect(50, 50, 100, 100) << r << true;
    r = QRegion(); r += QRect(50, 50, 78, 10);
    QTest::newRow("dx_0_dy_10_50_50_100_100") << input << 0 << 10 << QRect(50, 50, 100, 100) << r << true;
    r = QRegion(); r += QRect(50, 50, 78, 10); r += QRect(50, 60, 10, 68);
    QTest::newRow("dx_10_dy_10_50_50_100_100") << input << 10 << 10 << QRect(50, 50, 100, 100) << r << true;
    r = QRegion(); r += QRect(118, 50, 10, 78);
    QTest::newRow("dx_-10_dy_0_50_50_100_100") << input << -10 << 0 << QRect(50, 50, 100, 100) << r << true;
    r = QRegion(); r += QRect(50, 118, 78, 10);
    QTest::newRow("dx_0_dy_-10_50_50_100_100") << input << 0 << -10 << QRect(50, 50, 100, 100) << r << true;
    r = QRegion(); r += QRect(118, 50, 10, 68); r += QRect(50, 118, 78, 10);
    QTest::newRow("dx_-10_dy_-10_50_50_100_100") << input << -10 << -10 << QRect(50, 50, 100, 100) << r << true;

    // Scroll away the whole pixmap
    r = input.rect();
    QTest::newRow("dx_128_dy_0_x_y_w_h") << input << 128 << 0 << input.rect() << r << false;
    QTest::newRow("dx_0_dy_128_x_y_w_h") << input << 0 << 128 << input.rect() << r << false;
    QTest::newRow("dx_128_dy_128_x_y_w_h") << input << 128 << 128 << input.rect() << r << false;
    QTest::newRow("dx_-128_dy_0_x_y_w_h") << input << -128 << 0 << input.rect() << r << false;
    QTest::newRow("dx_0_dy_-128_x_y_w_h") << input << 0 << -128 << input.rect() << r << false;
    QTest::newRow("dx_-128_dy_-128_x_y_w_h") << input << -128 << -128 << input.rect() << r << false;

    // Scroll away part of the pixmap
    r = QRegion(); r += QRect(64, 64, 64, 64);
    QTest::newRow("dx_128_dy_128_64_64_128_128") << input << 128 << 128 << QRect(64, 64, 128, 128) << r << false;
}

void tst_QPixmap::scroll()
{
    QFETCH(QImage, input);
    QFETCH(int, dx);
    QFETCH(int, dy);
    QFETCH(QRect, rect);
    QFETCH(QRegion, exposed);
    QFETCH(bool, newPix);

    QPixmap pixmap = QPixmap::fromImage(input);
    QRegion exp;
    qint64 oldKey = pixmap.cacheKey();
    pixmap.scroll(dx, dy, rect, &exp);
    if (!newPix)
        QCOMPARE(pixmap.cacheKey(), oldKey);
    else
        QVERIFY(pixmap.cacheKey() != oldKey);

#if 0
    // Remember to add to resources.
    QString fileName = QString("images/%1.png").arg(QTest::currentDataTag());
    pixmap.toImage().save(fileName);
#else
    QString fileName = QString(":/images/%1.png").arg(QTest::currentDataTag());
#endif
    QPixmap output(fileName);
    QVERIFY(input.isNull() == output.isNull());
    QVERIFY(lenientCompare(pixmap, output));
    QCOMPARE(exp, exposed);
}

void tst_QPixmap::fill_data()
{
    QTest::addColumn<uint>("pixel");
    QTest::addColumn<bool>("syscolor");
    QTest::addColumn<bool>("bitmap");
    for (int color = Qt::black; color < Qt::darkYellow; ++color)
        QTest::newRow(QString("syscolor_%1").arg(color).toLatin1())
            << uint(color) << true << false;

#ifdef Q_WS_QWS
    if (QScreen::instance()->depth() >= 24) {
#elif defined (Q_WS_X11)
    QPixmap pm(1, 1);
    if (pm.x11PictureHandle()) {
#elif defined (Q_OS_WINCE)
    QPixmap pixmap(1,1);
    if (QPixmap::grabWidget(QApplication::desktop()).depth() >= 24) {
#else
    QPixmap pixmap(1, 1); {
#endif
        QTest::newRow("alpha_7f_red")   << 0x7fff0000u << false << false;
        QTest::newRow("alpha_3f_blue")  << 0x3f0000ffu << false << false;
        QTest::newRow("alpha_b7_green") << 0xbf00ff00u << false << false;
        QTest::newRow("alpha_7f_white") << 0x7fffffffu << false << false;
        QTest::newRow("alpha_3f_white") << 0x3fffffffu << false << false;
        QTest::newRow("alpha_b7_white") << 0xb7ffffffu << false << false;
        QTest::newRow("alpha_7f_black") << 0x7f000000u << false << false;
        QTest::newRow("alpha_3f_black") << 0x3f000000u << false << false;
        QTest::newRow("alpha_b7_black") << 0xbf000000u << false << false;
    }

    QTest::newRow("bitmap_color0") << uint(Qt::color0) << true << true;
    QTest::newRow("bitmap_color1") << uint(Qt::color1) << true << true;
}

void tst_QPixmap::fill()
{
    QFETCH(uint, pixel);
    QFETCH(bool, syscolor);
    QFETCH(bool, bitmap);

    QColor color;

    if (syscolor)
        color = QColor(Qt::GlobalColor(pixel));
    else
        color = QColor(qRed(pixel), qGreen(pixel), qBlue(pixel), qAlpha(pixel));

    QColor compareColor = color;
    if (bitmap && syscolor) {
        // special case color0 and color1 for bitmaps.
        if (pixel == Qt::color0)
            compareColor.setRgb(255, 255, 255);
        else
            compareColor.setRgb(0, 0, 0);
    }

    QPixmap pm;

    if (bitmap)
        pm = QBitmap(400, 400);
    else
        pm = QPixmap(400, 400);

#if defined(Q_WS_X11)
    if (!bitmap && pm.pixmapData()->classId() == QPixmapData::X11Class && !pm.x11PictureHandle())
        QSKIP("Requires XRender support", SkipSingle);
#endif

    pm.fill(color);
    if (syscolor && !bitmap && pm.depth() < 24) {
        QSKIP("Test does not work on displays without true color", SkipSingle);
    }

    QImage image = pm.toImage();
    if (bitmap && syscolor) {
        int pixelindex = (pixel == Qt::color0) ? 0 : 1;
        QCOMPARE(image.pixelIndex(0,0), pixelindex);
    }
    QImage::Format format = compareColor.alpha() != 255
                            ? QImage::Format_ARGB32
                            : QImage::Format_RGB32;
    image = image.convertToFormat(format);


    QImage shouldBe(400, 400, format);
    shouldBe.fill(compareColor.rgba());

    QCOMPARE(image, shouldBe);
}

void tst_QPixmap::fill_transparent()
{
    QPixmap pixmap(10, 10);
#ifdef Q_WS_X11
    if (pixmap.pixmapData()->classId() == QPixmapData::X11Class && !pixmap.x11PictureHandle())
        QSKIP("Requires XRender support", SkipAll);
#endif
    pixmap.fill(Qt::transparent);
    QVERIFY(pixmap.hasAlphaChannel());
}

void tst_QPixmap::mask()
{
    QPixmap pm(100, 100);
    QBitmap bm(100, 100);

    pm.fill();
    bm.fill();

    QVERIFY(!pm.isNull());
    QVERIFY(!bm.isNull());
    // hw: todo: this will fail if the default pixmap format is
    // argb32_premultiplied. The mask will be all 1's
    QVERIFY(pm.mask().isNull());

    QImage img = bm.toImage();
    QVERIFY(img.format() == QImage::Format_MonoLSB
            || img.format() == QImage::Format_Mono);

    pm.setMask(bm);
    QVERIFY(!pm.mask().isNull());

    bm = QBitmap();
    // Invalid format here, since isNull() == true
    QVERIFY(bm.toImage().isNull());
    QCOMPARE(bm.toImage().format(), QImage::Format_Invalid);
    pm.setMask(bm);
    QVERIFY(pm.mask().isNull());

    bm = QBitmap(100, 100);
    bm.fill();
    pm.setMask(bm);
    QVERIFY(!pm.mask().isNull());
}

void tst_QPixmap::bitmapMask()
{
    QImage image(3, 3, QImage::Format_Mono);
    image.setColor(0, Qt::color0);
    image.setColor(1, Qt::color1);
    image.fill(Qt::color0);
    image.setPixel(1, 1, Qt::color1);
    image.setPixel(0, 0, Qt::color1);

    QImage image_mask(3, 3, QImage::Format_Mono);
    image_mask.setColor(0, Qt::color0);
    image_mask.setColor(1, Qt::color1);
    image_mask.fill(Qt::color0);
    image_mask.setPixel(1, 1, Qt::color1);
    image_mask.setPixel(2, 0, Qt::color1);

    QBitmap pm = QBitmap::fromImage(image);
    QBitmap pm_mask = QBitmap::fromImage(image_mask);
    pm.setMask(pm_mask);

    image = pm.toImage();
    image.setColor(0, Qt::color0);
    image.setColor(1, Qt::color1);
    image_mask = pm_mask.toImage();
    image_mask.setColor(0, Qt::color0);
    image_mask.setColor(1, Qt::color1);

    QVERIFY(!image.pixel(0, 0));
    QVERIFY(!image.pixel(2, 0));
    QVERIFY(image.pixel(1, 1));
}

void tst_QPixmap::setGetMask_data()
{
    QTest::addColumn<QPixmap>("pixmap");
    QTest::addColumn<QBitmap>("mask");
    QTest::addColumn<QBitmap>("expected");

    QPixmap pixmap(10, 10);
    QBitmap mask(10, 10);
    QPainter p;

    p.begin(&pixmap);
    p.fillRect(0, 0, 10, 10, QColor(Qt::black));
    p.end();

    QTest::newRow("nullmask 0") << QPixmap() << QBitmap() << QBitmap();
    QTest::newRow("nullmask 1") << pixmap << QBitmap() << QBitmap();
    mask.clear();
    QTest::newRow("nullmask 2") << pixmap << mask << mask;
    QTest::newRow("nullmask 3") << QPixmap(QBitmap()) << QBitmap() << QBitmap();

    p.begin(&mask);
    p.fillRect(1, 1, 5, 5, QColor(Qt::color1));
    p.end();
    QTest::newRow("simple mask 0") << pixmap << mask << mask;
}

void tst_QPixmap::setGetMask()
{
    QFETCH(QPixmap, pixmap);
    QFETCH(QBitmap, mask);
    QFETCH(QBitmap, expected);

    pixmap.setMask(mask);
    QBitmap result = pixmap.mask();

    QImage resultImage = result.toImage();
    QImage expectedImage = expected.toImage();
    QCOMPARE(resultImage.convertToFormat(expectedImage.format()),
             expectedImage);
}

void tst_QPixmap::testMetrics()
{
    QPixmap pixmap(100, 100);

    QCOMPARE(pixmap.width(), 100);
    QCOMPARE(pixmap.height(), 100);
    QVERIFY(pixmap.depth() >= QPixmap::defaultDepth());

    QBitmap bitmap(100, 100);

    QCOMPARE(bitmap.width(), 100);
    QCOMPARE(bitmap.height(), 100);
    QCOMPARE(bitmap.depth(), 1);
}

void tst_QPixmap::createMaskFromColor()
{
    QImage image(3, 3, QImage::Format_Indexed8);
    image.setColorCount(10);
    image.setColor(0, 0xffffffff);
    image.setColor(1, 0xff000000);
    image.setColor(2, 0xffff0000);
    image.setColor(3, 0xff0000ff);
    image.fill(0);
    image.setPixel(1, 0, 1);
    image.setPixel(0, 1, 2);
    image.setPixel(1, 1, 3);

    QImage im_mask = image.createMaskFromColor(0xffff0000);
    QCOMPARE((uint) im_mask.pixel(0, 1), QColor(Qt::color0).rgba());
    QCOMPARE((uint) im_mask.pixel(0, 1), QColor(Qt::color0).rgba());

    QPixmap pixmap = QPixmap::fromImage(image);
    QBitmap mask = pixmap.createMaskFromColor(Qt::red);
    QBitmap inv_mask = pixmap.createMaskFromColor(Qt::red, Qt::MaskOutColor);
    QCOMPARE((uint) mask.toImage().pixel(0, 1), QColor(Qt::color0).rgba());
    QCOMPARE((uint) inv_mask.toImage().pixel(0, 1), QColor(Qt::color1).rgba());
}


void tst_QPixmap::cacheKey()
{
    QPixmap pixmap1(1, 1);
    QPixmap pixmap2(1, 1);
    qint64 pixmap1_key = pixmap1.cacheKey();

    QVERIFY(pixmap1.cacheKey() != pixmap2.cacheKey());

    pixmap2 = pixmap1;
    QVERIFY(pixmap2.cacheKey() == pixmap1.cacheKey());

    pixmap2.detach();
    QVERIFY(pixmap2.cacheKey() != pixmap1.cacheKey());
    QVERIFY(pixmap1.cacheKey() == pixmap1_key);
}

// Test drawing a bitmap on a pixmap.
void tst_QPixmap::drawBitmap()
{
    QBitmap bitmap(10,10);
    bitmap.fill(Qt::color1);

    QPixmap pixmap(10,10);
    QPainter painter2(&pixmap);
    painter2.fillRect(0,0,10,10, QBrush(Qt::green));
    painter2.setPen(Qt::red);
    painter2.drawPixmap(0,0,10,10, bitmap);
    painter2.end();

    QPixmap expected(10, 10);
    expected.fill(Qt::red);

    QVERIFY(lenientCompare(pixmap, expected));
}

void tst_QPixmap::grabWidget()
{
    QWidget widget;
    QImage image(128, 128, QImage::Format_ARGB32_Premultiplied);
    for (int row = 0; row < image.height(); ++row) {
        QRgb *line = reinterpret_cast<QRgb *>(image.scanLine(row));
        for (int col = 0; col < image.width(); ++col)
            line[col] = qRgb(rand() & 255, row, col);
    }

    QPalette pal = widget.palette();
    pal.setBrush(QPalette::Window, QBrush(image));
    widget.setPalette(pal);
    widget.resize(128, 128);

    QPixmap expected = QPixmap::fromImage(QImage(image.scanLine(64) + 64 * 4, 64, 64, image.bytesPerLine(), image.format()));
    QPixmap actual = QPixmap::grabWidget(&widget, QRect(64, 64, 64, 64));
    QVERIFY(lenientCompare(actual, expected));

    actual = QPixmap::grabWidget(&widget, 64, 64);
    QVERIFY(lenientCompare(actual, expected));

    // Make sure a widget that is not yet shown is grabbed correctly.
    QTreeWidget widget2;
    actual = QPixmap::grabWidget(&widget2);
    widget2.show();
    expected = QPixmap::grabWidget(&widget2);

    QVERIFY(lenientCompare(actual, expected));
}

void tst_QPixmap::grabWindow()
{
#ifdef Q_OS_WINCE
    // We get out of memory, if the desktop itself is too big.
    if (QApplication::desktop()->width() <= 480)
#endif
    QVERIFY(QPixmap::grabWindow(QApplication::desktop()->winId()).isNull() == false);
    QWidget w;
    w.resize(640, 480);
    w.show();
    QTest::qWait(100);
#ifdef Q_WS_X11
    qt_x11_wait_for_window_manager(&w);
#endif
    QVERIFY(QPixmap::grabWindow(w.winId()).isNull() == false);

    QWidget child(&w);
    child.setGeometry(50, 50, 100, 100);
    child.setPalette(Qt::red);
    child.setAutoFillBackground(true);
    child.show();
    QTest::qWait(100);
#ifdef Q_WS_X11
    qt_x11_wait_for_window_manager(&child);
#endif

    QPixmap grabWindowPixmap = QPixmap::grabWindow(child.winId());
    QPixmap grabWidgetPixmap = QPixmap::grabWidget(&child);
    lenientCompare(grabWindowPixmap, grabWidgetPixmap);
}

void tst_QPixmap::isNull()
{
    {
        QPixmap pixmap(1,1);
        QVERIFY(pixmap.isNull() == false);
    }
    {
        QPixmap pixmap(0,0);
        QVERIFY(pixmap.isNull());
    }

    {
        QPixmap pixmap(0,1);
        QVERIFY(pixmap.isNull());
    }
    {
        QPixmap pixmap(1,0);
        QVERIFY(pixmap.isNull());
    }
    {
        QPixmap pixmap(-1,-1);
        QVERIFY(pixmap.isNull());
    }
    {
        QPixmap pixmap(-1,5);
        QVERIFY(pixmap.isNull());
    }
}

#ifdef Q_WS_QWS
void tst_QPixmap::convertFromImageNoDetach()
{
    //first get the screen format
    QPixmap randomPixmap(10,10);
    QImage::Format screenFormat = randomPixmap.toImage().format();
    QVERIFY(screenFormat != QImage::Format_Invalid);

    QImage orig(100,100, screenFormat);

    QPixmap pix = QPixmap::fromImage(orig);
    QImage copy = pix.toImage();

    QVERIFY(copy.format() == screenFormat);

    const QImage constOrig = orig;
    const QImage constCopy = copy;
    QVERIFY(constOrig.bits() == constCopy.bits());
}
#endif //Q_WS_QWS

void tst_QPixmap::convertFromImageDetach()
{
    QImage img(10,10, QImage::Format_RGB32);
    img.fill(0);
    QVERIFY(!img.isNull());
    QPixmap p = QPixmap::fromImage(img);
    QVERIFY(p.isDetached());
    QPixmap copy = p;
    QVERIFY(!copy.isDetached());
    QVERIFY(!p.isDetached());
    img.fill(1);
    p = QPixmap::fromImage(img);
    QVERIFY(copy.isDetached());
}

#if defined(Q_WS_WIN)
void tst_QPixmap::toWinHBITMAP_data()
{
    QTest::addColumn<int>("red");
    QTest::addColumn<int>("green");
    QTest::addColumn<int>("blue");

    QTest::newRow("red")   << 255 << 0 << 0;
    QTest::newRow("green") << 0 << 255 << 0;
    QTest::newRow("blue")  << 0 << 0 << 255;
}

void tst_QPixmap::toWinHBITMAP()
{
    QFETCH(int, red);
    QFETCH(int, green);
    QFETCH(int, blue);

    QPixmap pm(100, 100);
    pm.fill(QColor(red, green, blue));

    HBITMAP bitmap = pm.toWinHBITMAP();

    QVERIFY(bitmap != 0);

    // Verify size
    BITMAP bitmap_info;
    memset(&bitmap_info, 0, sizeof(BITMAP));

    int res = GetObject(bitmap, sizeof(BITMAP), &bitmap_info);
    QVERIFY(res);

    QCOMPARE(100, (int) bitmap_info.bmWidth);
    QCOMPARE(100, (int) bitmap_info.bmHeight);

    HDC display_dc = GetDC(0);
    HDC bitmap_dc = CreateCompatibleDC(display_dc);

    HBITMAP null_bitmap = (HBITMAP) SelectObject(bitmap_dc, bitmap);

    COLORREF pixel = GetPixel(bitmap_dc, 0, 0);
    QCOMPARE((int)GetRValue(pixel), red);
    QCOMPARE((int)GetGValue(pixel), green);
    QCOMPARE((int)GetBValue(pixel), blue);

    // Clean up
    SelectObject(bitmap_dc, null_bitmap);
    DeleteObject(bitmap);
    DeleteDC(bitmap_dc);
    ReleaseDC(0, display_dc);

}

void tst_QPixmap::fromWinHBITMAP_data()
{
    toWinHBITMAP_data();
}

void tst_QPixmap::fromWinHBITMAP()
{
    QFETCH(int, red);
    QFETCH(int, green);
    QFETCH(int, blue);

    HDC display_dc = GetDC(0);
    HDC bitmap_dc = CreateCompatibleDC(display_dc);
    HBITMAP bitmap = CreateCompatibleBitmap(display_dc, 100, 100);
    SelectObject(bitmap_dc, bitmap);

    SelectObject(bitmap_dc, GetStockObject(NULL_PEN));
    HGDIOBJ old_brush = SelectObject(bitmap_dc, CreateSolidBrush(RGB(red, green, blue)));
    Rectangle(bitmap_dc, 0, 0, 100, 100);

#ifdef Q_OS_WINCE //the device context has to be deleted before QPixmap::fromWinHBITMAP()
    DeleteDC(bitmap_dc);
#endif
    QPixmap pixmap = QPixmap::fromWinHBITMAP(bitmap);
    QCOMPARE(pixmap.width(), 100);
    QCOMPARE(pixmap.height(), 100);

    QImage image = pixmap.toImage();
    QRgb pixel = image.pixel(0, 0);
    QCOMPARE(qRed(pixel), red);
    QCOMPARE(qGreen(pixel), green);
    QCOMPARE(qBlue(pixel), blue);

    DeleteObject(SelectObject(bitmap_dc, old_brush));
    DeleteObject(SelectObject(bitmap_dc, bitmap));
#ifndef Q_OS_WINCE
    DeleteDC(bitmap_dc);
#endif
    ReleaseDC(0, display_dc);
}

static void compareImages(const QImage &image1, const QImage &image2)
{
    QCOMPARE(image1.width(), image2.width());
    QCOMPARE(image1.height(), image2.height());
    QCOMPARE(image1.format(), image2.format());

    static const int fuzz = 1;

    for (int y = 0; y < image1.height(); y++)
    {
        for (int x = 0; x < image2.width(); x++)
        {
            QRgb p1 = image1.pixel(x, y);
            QRgb p2 = image2.pixel(x, y);

            bool pixelMatches =
                qAbs(qRed(p1) - qRed(p2)) <= fuzz
                && qAbs(qGreen(p1) - qGreen(p2)) <= fuzz
                && qAbs(qBlue(p1) - qBlue(p2)) <= fuzz
                && qAbs(qAlpha(p1) - qAlpha(p2)) <= fuzz;

            QVERIFY(pixelMatches);
        }
    }
}

void tst_QPixmap::toWinHICON_data()
{
    QTest::addColumn<QString>("image");
    QTest::addColumn<int>("width");
    QTest::addColumn<int>("height");

    const QString prefix = QLatin1String(SRCDIR) + "/convertFromToHICON";

    QTest::newRow("32bpp_16x16") << prefix + QLatin1String("/icon_32bpp") << 16 << 16;
    QTest::newRow("32bpp_32x32") << prefix + QLatin1String("/icon_32bpp") << 32 << 32;
    QTest::newRow("32bpp_48x48") << prefix + QLatin1String("/icon_32bpp") << 48 << 48;
    QTest::newRow("32bpp_256x256") << prefix + QLatin1String("/icon_32bpp") << 256 << 256;

    QTest::newRow("8bpp_16x16") << prefix + QLatin1String("/icon_8bpp") << 16 << 16;
    QTest::newRow("8bpp_32x32") << prefix + QLatin1String("/icon_8bpp") << 32 << 32;
    QTest::newRow("8bpp_48x48") << prefix + QLatin1String("/icon_8bpp") << 48 << 48;
}

void tst_QPixmap::toWinHICON()
{
#ifdef Q_OS_WINCE
    QSKIP("Test shall be enabled for Windows CE shortly.", SkipAll);
#endif

    QFETCH(int, width);
    QFETCH(int, height);
    QFETCH(QString, image);

    QPixmap empty(width, height);
    empty.fill(Qt::transparent);

    HDC display_dc = GetDC(0);
    HDC bitmap_dc = CreateCompatibleDC(display_dc);
    HBITMAP bitmap = empty.toWinHBITMAP(QPixmap::Alpha);
    SelectObject(bitmap_dc, bitmap);

    QImage imageFromFile(image + QString(QLatin1String("_%1x%2.png")).arg(width).arg(height));
    imageFromFile = imageFromFile.convertToFormat(QImage::Format_ARGB32_Premultiplied);

    HICON icon = QPixmap::fromImage(imageFromFile).toWinHICON();

    DrawIconEx(bitmap_dc, 0, 0, icon, width, height, 0, 0, DI_NORMAL);

    DestroyIcon(icon);
    DeleteDC(bitmap_dc);

    QImage imageFromHICON = QPixmap::fromWinHBITMAP(bitmap, QPixmap::Alpha).toImage();

    ReleaseDC(0, display_dc);

    // fuzzy comparison must be used, as the pixel values change slightly during conversion
    // between QImage::Format_ARGB32 and QImage::Format_ARGB32_Premultiplied, or elsewhere

    // QVERIFY(imageFromHICON == imageFromFile);
    compareImages(imageFromHICON, imageFromFile);
}

void tst_QPixmap::fromWinHICON_data()
{
    toWinHICON_data();
}

void tst_QPixmap::fromWinHICON()
{
#ifdef Q_OS_WINCE
    QSKIP("Test shall be enabled for Windows CE shortly.", SkipAll);

#else
    QFETCH(int, width);
    QFETCH(int, height);
    QFETCH(QString, image);

    HICON icon = (HICON)LoadImage(0, (wchar_t*)(image + QLatin1String(".ico")).utf16(), IMAGE_ICON, width, height, LR_LOADFROMFILE);
    QImage imageFromHICON = QPixmap::fromWinHICON(icon).toImage();
    DestroyIcon(icon);

    QImage imageFromFile(image + QString(QLatin1String("_%1x%2.png")).arg(width).arg(height));
    imageFromFile = imageFromFile.convertToFormat(QImage::Format_ARGB32_Premultiplied);

    // fuzzy comparison must be used, as the pixel values change slightly during conversion
    // between QImage::Format_ARGB32 and QImage::Format_ARGB32_Premultiplied, or elsewhere

    // QVERIFY(imageFromHICON == imageFromFile);
    compareImages(imageFromHICON, imageFromFile);
#endif
}

#endif // Q_WS_WIN

#if defined(Q_OS_SYMBIAN)
Q_DECLARE_METATYPE(TDisplayMode)

void tst_QPixmap::fromSymbianCFbsBitmap_data()
{
    QTest::addColumn<TDisplayMode>("format");
    QTest::addColumn<int>("width");
    QTest::addColumn<int>("height");
    QTest::addColumn<QColor>("color");

    const int smallWidth = 20;
    const int smallHeight = 20;
    const int largeWidth = 240;
    const int largeHeight = 320;

    // Indexed Color Formats - Disabled since images seem to be blank -> no palette?
//    QTest::newRow("EGray2 small") << EGray2 << smallWidth << smallHeight << QColor(Qt::black);
//    QTest::newRow("EGray2 big") << EGray2 << largeWidth << largeHeight << QColor(Qt::black);
//    QTest::newRow("EGray256 small") << EGray256 << smallWidth << smallHeight << QColor(Qt::blue);
//    QTest::newRow("EGray256 big") << EGray256 << largeWidth << largeHeight << QColor(Qt::blue);
//    QTest::newRow("EColor256 small") << EColor256 << smallWidth << smallHeight << QColor(Qt::red);
//    QTest::newRow("EColor256 big") << EColor256 << largeWidth << largeHeight << QColor(Qt::red);

    // Direct Color Formats
    QTest::newRow("EColor4K small") << EColor4K << smallWidth << smallHeight << QColor(Qt::red);
    QTest::newRow("EColor4K big") << EColor4K << largeWidth << largeHeight << QColor(Qt::red);
    QTest::newRow("EColor64K small") << EColor64K << smallWidth << smallHeight << QColor(Qt::green);
    QTest::newRow("EColor64K big") << EColor64K << largeWidth << largeHeight << QColor(Qt::green);
    QTest::newRow("EColor16M small") << EColor16M << smallWidth << smallHeight << QColor(Qt::yellow);
    QTest::newRow("EColor16M big") << EColor16M << largeWidth << largeHeight << QColor(Qt::yellow);
    QTest::newRow("EColor16MU small") << EColor16MU << smallWidth << smallHeight << QColor(Qt::red);
    QTest::newRow("EColor16MU big") << EColor16MU << largeWidth << largeHeight << QColor(Qt::red);
    QTest::newRow("EColor16MA small opaque") << EColor16MA << smallWidth << smallHeight << QColor(255, 255, 0);
    QTest::newRow("EColor16MA big opaque") << EColor16MA << largeWidth << largeHeight << QColor(255, 255, 0);

    // Semi-transparent Colors - Disabled for now, since the QCOMPARE fails, but visually confirmed to work
//    QTest::newRow("EColor16MA small semi") << EColor16MA << smallWidth << smallHeight << QColor(255, 255, 0, 127);
//    QTest::newRow("EColor16MA big semi") << EColor16MA << largeWidth << largeHeight << QColor(255, 255, 0, 127);
//    QTest::newRow("EColor16MA small trans") << EColor16MA << smallWidth << smallHeight << QColor(255, 255, 0, 0);
//    QTest::newRow("EColor16MA big trans") << EColor16MA << largeWidth << largeHeight << QColor(255, 255, 0, 0);

#if !defined(__SERIES60_31__)
    QTest::newRow("EColor16MAP small") << EColor16MAP << smallWidth << smallHeight << QColor(Qt::red);
    QTest::newRow("EColor16MAP big") << EColor16MAP << largeWidth << largeHeight << QColor(Qt::red);
#endif
}

void tst_QPixmap::fromSymbianCFbsBitmap()
{
    QFETCH(TDisplayMode, format);
    QFETCH(int, width);
    QFETCH(int, height);
    QFETCH(QColor, color);
    int expectedDepth = TDisplayModeUtils::NumDisplayModeBitsPerPixel(format);

    CFbsBitmap *nativeBitmap = 0;
    CFbsBitmapDevice *bitmapDevice = 0;
    CBitmapContext *bitmapContext = 0;

    nativeBitmap = new (ELeave) CFbsBitmap();
    TInt err = nativeBitmap->Create(TSize(width, height), format);
    CleanupStack::PushL(nativeBitmap);
    QVERIFY(err == KErrNone);
    bitmapDevice = CFbsBitmapDevice::NewL(nativeBitmap);
    CleanupStack::PushL(bitmapDevice);

    err = bitmapDevice->CreateBitmapContext(bitmapContext);
    CleanupStack::PushL(bitmapContext);
    QVERIFY(err == KErrNone);
    TRgb symbianColor = TRgb(color.red(), color.green(), color.blue(), color.alpha());
    bitmapContext->SetBrushColor(symbianColor);
    bitmapContext->Clear();

    __UHEAP_MARK;
    { // Test the null case
        CFbsBitmap *bitmap = 0;
        QPixmap pixmap = QPixmap::fromSymbianCFbsBitmap(bitmap);
        QVERIFY(pixmap.isNull());
    }
    __UHEAP_MARKEND;

    __UHEAP_MARK;
    { // Test the normal case
        QPixmap pixmap = QPixmap::fromSymbianCFbsBitmap(nativeBitmap);
//        QCOMPARE(pixmap.depth(), expectedDepth); // Depth is not preserved now
        QCOMPARE(pixmap.width(), width);
        QCOMPARE(pixmap.height(), height);
        QImage image = pixmap.toImage();

        QColor actualColor(image.pixel(1, 1));
        QCOMPARE(actualColor, color);
    }
    __UHEAP_MARKEND;

    CleanupStack::PopAndDestroy(3);
}

void tst_QPixmap::toSymbianCFbsBitmap_data()
{
    QTest::addColumn<int>("red");
    QTest::addColumn<int>("green");
    QTest::addColumn<int>("blue");

    QTest::newRow("red")   << 255 << 0 << 0;
    QTest::newRow("green") << 0 << 255 << 0;
    QTest::newRow("blue")  << 0 << 0 << 255;
}

void tst_QPixmap::toSymbianCFbsBitmap()
{
    QFETCH(int, red);
    QFETCH(int, green);
    QFETCH(int, blue);

    QPixmap pm(100, 100);
    pm.fill(QColor(red, green, blue));

    CFbsBitmap *bitmap = pm.toSymbianCFbsBitmap();

    QVERIFY(bitmap != 0);

    // Verify size
    QCOMPARE(100, (int) bitmap->SizeInPixels().iWidth);
    QCOMPARE(100, (int) bitmap->SizeInPixels().iHeight);

    // Verify pixel color
    TRgb pixel;
    bitmap->GetPixel(pixel, TPoint(0,0));
    QCOMPARE((int)pixel.Red(), red);
    QCOMPARE((int)pixel.Green(), green);
    QCOMPARE((int)pixel.Blue(), blue);

    // Clean up
    delete bitmap;
}
#endif

void tst_QPixmap::onlyNullPixmapsOutsideGuiThread()
{
#if !defined(Q_WS_WIN)
    class Thread : public QThread
    {
    public:
        void run()
        {
            QTest::ignoreMessage(QtWarningMsg,
                                 "QPixmap: It is not safe to use pixmaps outside the GUI thread");
            QPixmap pixmap;
            QVERIFY(pixmap.isNull());

            QTest::ignoreMessage(QtWarningMsg,
                                 "QPixmap: It is not safe to use pixmaps outside the GUI thread");
            QPixmap pixmap1(100, 100);
            QVERIFY(pixmap1.isNull());

            QTest::ignoreMessage(QtWarningMsg,
                                 "QPixmap: It is not safe to use pixmaps outside the GUI thread");
            QPixmap pixmap2(pixmap1);
            QVERIFY(pixmap2.isNull());
        }
    };
    Thread thread;
#if defined(Q_OS_SYMBIAN)
    thread.setStackSize(0x10000);
#endif
    thread.start();
#if defined(Q_OS_SYMBIAN)
    QVERIFY(thread.wait(10000));
#else
    thread.wait();
#endif

#endif // !defined(Q_WS_WIN)
}

void tst_QPixmap::refUnref()
{
    // Simple ref/unref
    {
        QPixmap p;
    }
    {
        QBitmap b;
    }

    // Get a copy of a pixmap that goes out of scope
    {
        QPixmap b;
        {
            QPixmap a(10, 10);
            a.fill(Qt::color0);
            b = a;
        }
    }
    {
        QBitmap mask;
        {
            QBitmap bitmap(10, 10);
            bitmap.fill(Qt::color1);
            mask = bitmap.mask();
        }
        mask.fill(Qt::color0);
    }

}

void tst_QPixmap::copy()
{
    QPixmap src(32, 32);
    {
        QPainter p(&src);
        p.fillRect(0, 0, 32, 32, Qt::red);
        p.fillRect(10, 10, 10, 10, Qt::blue);
    }

    QPixmap dest = src.copy(10, 10, 10, 10);

    QPixmap expected(10, 10);
    expected.fill(Qt::blue);
    QVERIFY(lenientCompare(dest, expected));

    QPixmap trans;
    trans.fill(Qt::transparent);

    QPixmap transCopy = trans.copy();
    QVERIFY(pixmapsAreEqual(&trans, &transCopy));
}

#ifdef QT3_SUPPORT
void tst_QPixmap::resize()
{
    QPixmap p1(10, 10);
    p1.fill(Qt::red);

    QPixmap p2 = p1;
    QPixmap p3(50, 50);
    p3.fill(Qt::red);

    p1.resize(p3.size());
    p1.resize(p2.size());
    p3.resize(p2.size());
    QCOMPARE(p1.toImage(), p2.toImage());
    QCOMPARE(p1.toImage(), p3.toImage());

    QBitmap b1;
    b1.resize(10, 10);
    QVERIFY(b1.depth() == 1);
    QPixmap p4;
    p4.resize(10, 10);
    QVERIFY(p4.depth() != 0);
}

void tst_QPixmap::resizePreserveMask()
{
    QPixmap pm(100, 100);
    pm.fill(Qt::transparent);
    QPainter p(&pm);
    p.fillRect(10, 10, 80, 80, Qt::red);
    p.drawRect(0, 0, 99, 99);
    p.end();

    QBitmap mask = pm.mask();
    pm.resize(50, 50);

    QCOMPARE(pm.mask().toImage(), mask.toImage().copy(0, 0, 50, 50));

    pm = QPixmap(100, 100);
    pm.fill(Qt::red);
    pm.setMask(mask);
    pm.resize(50, 50);

    QCOMPARE(pm.mask().toImage(), mask.toImage().copy(0, 0, 50, 50));
}
#endif

void tst_QPixmap::depthOfNullObjects()
{
    QBitmap b1;
    QVERIFY(b1.depth() == 0);
    QPixmap p4;
    QVERIFY(p4.depth() == 0);
}

void tst_QPixmap::transformed()
{
    QPixmap p1(20, 10);
    p1.fill(Qt::red);
    {
        QPainter p(&p1);
        p.drawRect(0, 0, p1.width() - 1, p1.height() - 1);
    }

    QPixmap p2(10, 20);
    {
        QPainter p(&p2);
        p.rotate(90);
        p.drawPixmap(0, -p1.height(), p1);
    }

    QPixmap p3(20, 10);
    {
        QPainter p(&p3);
        p.rotate(180);
        p.drawPixmap(-p1.width(), -p1.height(), p1);
    }

    QPixmap p4(10, 20);
    {
        QPainter p(&p4);
        p.rotate(270);
        p.drawPixmap(-p1.width(), 0, p1);
    }

    QPixmap p1_90 = p1.transformed(QTransform().rotate(90));
    QPixmap p1_180 = p1.transformed(QTransform().rotate(180));
    QPixmap p1_270 = p1.transformed(QTransform().rotate(270));

    QVERIFY(lenientCompare(p1_90, p2));
    QVERIFY(lenientCompare(p1_180, p3));
    QVERIFY(lenientCompare(p1_270, p4));
}

void tst_QPixmap::transformed2()
{
    QPixmap pm(3, 3);
    pm.fill(Qt::red);
    QPainter p(&pm);
    p.fillRect(0, 0, 3, 3, QBrush(Qt::Dense4Pattern));
    p.end();

    QTransform transform;
    transform.rotate(-90);
    transform.scale(3, 3);

    QPixmap actual = pm.transformed(transform);

    QPixmap expected(9, 9);
    expected.fill(Qt::red);
    p.begin(&expected);
    p.setBrush(Qt::black);
    p.setPen(Qt::NoPen);
    p.drawRect(3, 0, 3, 3);
    p.drawRect(0, 3, 3, 3);
    p.drawRect(6, 3, 3, 3);
    p.drawRect(3, 6, 3, 3);
    p.end();

    QVERIFY(lenientCompare(actual, expected));
}

void tst_QPixmap::fromImage_crash()
{
    QImage *img = new QImage(64, 64, QImage::Format_ARGB32_Premultiplied);

    QPixmap pm = QPixmap::fromImage(*img);
    QPainter painter(&pm);

    delete img;
}

//This is testing QPixmapData::createCompatiblePixmapData - see QTBUG-5977
void tst_QPixmap::splash_crash()
{
    QPixmap pix;
    pix = QPixmap(":/images/designer.png");
    QSplashScreen splash(pix);
    splash.show();
    QCoreApplication::processEvents();
    splash.close();
}

void tst_QPixmap::fromData()
{
    unsigned char bits[] = { 0xaa, 0x55 };

    QBitmap bm = QBitmap::fromData(QSize(8, 2), bits);
    QImage img = bm.toImage();

    QSet<QRgb> colors;
    for (int y = 0; y < img.height(); ++y)
        for (int x = 0; x < img.width(); ++x)
            colors << img.pixel(x, y);

    QCOMPARE(colors.size(), 2);

    QCOMPARE(img.pixel(0, 0), QRgb(0xffffffff));
    QCOMPARE(img.pixel(0, 1), QRgb(0xff000000));
}

void tst_QPixmap::loadFromDataNullValues()
{
    {
    QPixmap pixmap;
    pixmap.loadFromData(QByteArray());
    QVERIFY(pixmap.isNull());
    }
    {
    QPixmap pixmap;
    pixmap.loadFromData(0, 123);
    QVERIFY(pixmap.isNull());
    }
    {
    QPixmap pixmap;
    const uchar bla[] = "bla";
    pixmap.loadFromData(bla, 0);
    QVERIFY(pixmap.isNull());
    }
}

void tst_QPixmap::loadFromDataImage_data()
{
    QTest::addColumn<QString>("imagePath");
#ifdef Q_OS_SYMBIAN
    const QString prefix = QLatin1String(SRCDIR) + "loadFromData";
#else
    const QString prefix = QLatin1String(SRCDIR) + "/loadFromData";
#endif
    QTest::newRow("designer_argb32.png") << prefix + "/designer_argb32.png";
    QTest::newRow("designer_indexed8_no_alpha.png") << prefix + "/designer_indexed8_no_alpha.png";
    QTest::newRow("designer_indexed8_with_alpha.png") << prefix + "/designer_indexed8_with_alpha.png";
    QTest::newRow("designer_rgb32.png") << prefix + "/designer_rgb32.png";
    QTest::newRow("designer_indexed8_no_alpha.gif") << prefix + "/designer_indexed8_no_alpha.gif";
    QTest::newRow("designer_indexed8_with_alpha.gif") << prefix + "/designer_indexed8_with_alpha.gif";
    QTest::newRow("designer_rgb32.jpg") << prefix + "/designer_rgb32.jpg";
}

void tst_QPixmap::loadFromDataImage()
{
    QFETCH(QString, imagePath);

    QImage imageRef(imagePath);
    QPixmap pixmapWithCopy = QPixmap::fromImage(imageRef);

    QFile file(imagePath);
    file.open(QIODevice::ReadOnly);
    QByteArray rawData = file.readAll();

    QPixmap directLoadingPixmap;
    directLoadingPixmap.loadFromData(rawData);

    QVERIFY(pixmapsAreEqual(&pixmapWithCopy, &directLoadingPixmap));
}

void tst_QPixmap::fromImageReader_data()
{
    QTest::addColumn<QString>("imagePath");
#ifdef Q_OS_SYMBIAN
    const QString prefix = QLatin1String(SRCDIR) + "loadFromData";
#else
    const QString prefix = QLatin1String(SRCDIR) + "/loadFromData";
#endif
    QTest::newRow("designer_argb32.png") << prefix + "/designer_argb32.png";
    QTest::newRow("designer_indexed8_no_alpha.png") << prefix + "/designer_indexed8_no_alpha.png";
    QTest::newRow("designer_indexed8_with_alpha.png") << prefix + "/designer_indexed8_with_alpha.png";
    QTest::newRow("designer_rgb32.png") << prefix + "/designer_rgb32.png";
    QTest::newRow("designer_indexed8_no_alpha.gif") << prefix + "/designer_indexed8_no_alpha.gif";
    QTest::newRow("designer_indexed8_with_alpha.gif") << prefix + "/designer_indexed8_with_alpha.gif";
    QTest::newRow("designer_rgb32.jpg") << prefix + "/designer_rgb32.jpg";
}

void tst_QPixmap::fromImageReader()
{
    QFETCH(QString, imagePath);

    QImage imageRef(imagePath);
    QPixmap pixmapWithCopy = QPixmap::fromImage(imageRef);

    QImageReader imageReader(imagePath);

    QPixmap directLoadingPixmap = QPixmap::fromImageReader(&imageReader);

    QVERIFY(pixmapsAreEqual(&pixmapWithCopy, &directLoadingPixmap));
}

void tst_QPixmap::fromImageReaderAnimatedGif()
{
#ifdef Q_OS_SYMBIAN
    const QString prefix = QLatin1String(SRCDIR) + "loadFromData";
#else
    const QString prefix = QLatin1String(SRCDIR) + "/loadFromData";
#endif
    const QString path = prefix + QString::fromLatin1("/designer_indexed8_with_alpha_animated.gif");

    QImageReader referenceReader(path);
    QImageReader pixmapReader(path);

    Q_ASSERT(referenceReader.canRead());
    Q_ASSERT(referenceReader.imageCount() > 1);

    for (int i = 0; i < referenceReader.imageCount(); ++i) {
        QImage refImage = referenceReader.read();
        QPixmap refPixmap = QPixmap::fromImage(refImage);

        QPixmap directLoadingPixmap = QPixmap::fromImageReader(&pixmapReader);
        QVERIFY(pixmapsAreEqual(&refPixmap, &directLoadingPixmap));
    }
}

void tst_QPixmap::task_246446()
{
    // This crashed without the bugfix in 246446
    QPixmap pm(10, 10);
    pm.fill(Qt::transparent); // force 32-bit depth
    QBitmap bm;
    pm.setMask(bm);
    {
        QPixmap pm2(pm);
    }
    QVERIFY(pm.width() == 10);
    QVERIFY(pm.mask().isNull());
}

void tst_QPixmap::preserveDepth()
{
    QPixmap target(64, 64);
    target.fill(Qt::transparent);

    QPixmap source(64, 64);
    source.fill(Qt::white);

    int depth = source.depth();

    QPainter painter(&target);
    painter.setBrush(source);
    painter.drawRect(target.rect());
    painter.end();

    QCOMPARE(depth, source.depth());
}

void tst_QPixmap::loadAsBitmapOrPixmap()
{
    QImage tmp(10, 10, QImage::Format_RGB32);
    tmp.save("tmp.png");

    bool ok;

    // Check that we can load the pixmap as a pixmap and that it then turns into a pixmap
    QPixmap pixmap("tmp.png");
    QVERIFY(!pixmap.isNull());
    QVERIFY(pixmap.depth() > 1);
    QVERIFY(!pixmap.isQBitmap());

    pixmap = QPixmap();
    ok = pixmap.load("tmp.png");
    QVERIFY(ok);
    QVERIFY(!pixmap.isNull());
    QVERIFY(pixmap.depth() > 1);
    QVERIFY(!pixmap.isQBitmap());

    // The do the same check for bitmaps..
    QBitmap bitmap("tmp.png");
    QVERIFY(!bitmap.isNull());
    QVERIFY(bitmap.depth() == 1);
    QVERIFY(bitmap.isQBitmap());

    bitmap = QBitmap();
    ok = bitmap.load("tmp.png");
    QVERIFY(ok);
    QVERIFY(!bitmap.isNull());
    QVERIFY(bitmap.depth() == 1);
    QVERIFY(bitmap.isQBitmap());
}



QTEST_MAIN(tst_QPixmap)
#include "tst_qpixmap.moc"
