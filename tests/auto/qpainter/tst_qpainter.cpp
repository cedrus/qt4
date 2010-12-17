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
#include "../../shared/util.h"

#include <qpainter.h>
#include <qapplication.h>
#include <qwidget.h>
#include <qfontmetrics.h>
#include <qbitmap.h>
#include <qimage.h>
#include <limits.h>
#if !defined(Q_OS_WINCE) && !defined(Q_OS_SYMBIAN)
#include <qprinter.h>
#include <math.h>
#ifdef QT3_SUPPORT
#include <q3painter.h>
#endif
#endif
#include <qpaintengine.h>
#include <qdesktopwidget.h>
#include <qpixmap.h>

#include <qpainter.h>

#include <qlabel.h>

#include <qqueue.h>

#include <qgraphicsview.h>
#include <qgraphicsscene.h>
#include <qgraphicsproxywidget.h>
#include <qlayout.h>

#if defined(Q_OS_SYMBIAN)
# define SRCDIR "."
#endif

Q_DECLARE_METATYPE(QLine)
Q_DECLARE_METATYPE(QRect)
Q_DECLARE_METATYPE(QSize)
Q_DECLARE_METATYPE(QPoint)
Q_DECLARE_METATYPE(QPainterPath)

//TESTED_CLASS=
//TESTED_FILES=

class tst_QPainter : public QObject
{
Q_OBJECT

public:
    tst_QPainter();
    virtual ~tst_QPainter();


public slots:
    void init();
    void cleanup();
private slots:
    void getSetCheck();
    void qt_format_text_clip();
    void qt_format_text_boundingRect();
    void drawPixmap_comp_data();
    void drawPixmap_comp();
    void saveAndRestore_data();
    void saveAndRestore();

    void drawBorderPixmap();
    void drawPixmapFragments();

    void drawLine_data();
    void drawLine();
    void drawLine_clipped();
    void drawLine_task121143();
    void drawLine_task216948();

    void drawLine_task190634();
    void drawLine_task229459();
    void drawLine_task234891();

    void drawRect_data() { fillData(); }
    void drawRect();
    void drawRect2();

    void fillRect();
    void fillRect2();
    void fillRect3();
    void fillRect4();

    void drawEllipse_data();
    void drawEllipse();
    void drawClippedEllipse_data();
    void drawClippedEllipse();

    void drawPath_data();
    void drawPath();
    void drawPath2();
    void drawPath3();

    void drawRoundRect_data() { fillData(); }
    void drawRoundRect();

    void qimageFormats_data();
    void qimageFormats();
    void textOnTransparentImage();

    void initFrom();

    void setWindow();

    void combinedMatrix();
    void renderHints();

    void disableEnableClipping();
    void setClipRect();
    void setEqualClipRegionAndPath_data();
    void setEqualClipRegionAndPath();

    void clipRectSaveRestore();

    void clippedFillPath_data();
    void clippedFillPath();
    void clippedLines_data();
    void clippedLines();
    void clippedPolygon_data();
    void clippedPolygon();

    void clippedText();

    void setOpacity_data();
    void setOpacity();

    void drawhelper_blend_untransformed_data();
    void drawhelper_blend_untransformed();
    void drawhelper_blend_tiled_untransformed_data();
    void drawhelper_blend_tiled_untransformed();

    void porterDuff_warning();

    void drawhelper_blend_color();

    void childWidgetViewport();

    void fillRect_objectBoundingModeGradient();
    void fillRect_stretchToDeviceMode();
    void monoImages();

    void linearGradientSymmetry();
    void gradientInterpolation();

    void fpe_pixmapTransform();
    void fpe_zeroLengthLines();
    void fpe_divByZero();

    void fpe_steepSlopes_data();
    void fpe_steepSlopes();
    void fpe_rasterizeLine_task232012();

    void fpe_radialGradients();

    void rasterizer_asserts();
    void rasterizer_negativeCoords();

    void blendOverFlow_data();
    void blendOverFlow();

    void largeImagePainting_data();
    void largeImagePainting();

    void imageScaling_task206785();

    void outlineFillConsistency();

    void drawImage_task217400_data();
    void drawImage_task217400();
    void drawImage_1x1();
    void drawImage_task258776();
    void drawRect_task215378();
    void drawRect_task247505();

    void drawImage_data();
    void drawImage();

    void clippedImage();

    void stateResetBetweenQPainters();

    void imageCoordinateLimit();
    void imageBlending_data();
    void imageBlending();
    void imageBlending_clipped();

    void paintOnNullPixmap();
    void checkCompositionMode();

    void drawPolygon();

    void inactivePainter();

    void extendedBlendModes();

    void zeroOpacity();
    void clippingBug();
    void emptyClip();

    void taskQT4444_dontOverflowDashOffset();

    void painterBegin();
    void setPenColorOnImage();
    void setPenColorOnPixmap();

    void QTBUG5939_attachPainterPrivate();

    void drawPointScaled();

private:
    void fillData();
    void setPenColor(QPainter& p);
    QColor baseColor( int k, int intensity=255 );
    QImage getResImage( const QString &dir, const QString &addition, const QString &extension );
    QBitmap getBitmap( const QString &dir, const QString &filename, bool mask );
};

// Testing get/set functions
void tst_QPainter::getSetCheck()
{
    QImage img(QSize(10, 10), QImage::Format_ARGB32_Premultiplied);
    QPainter obj1;
    obj1.begin(&img);
    // CompositionMode QPainter::compositionMode()
    // void QPainter::setCompositionMode(CompositionMode)
    obj1.setCompositionMode(QPainter::CompositionMode(QPainter::CompositionMode_SourceOver));
    QCOMPARE(QPainter::CompositionMode(QPainter::CompositionMode_SourceOver), obj1.compositionMode());
    obj1.setCompositionMode(QPainter::CompositionMode(QPainter::CompositionMode_DestinationOver));
    QCOMPARE(QPainter::CompositionMode(QPainter::CompositionMode_DestinationOver), obj1.compositionMode());
    obj1.setCompositionMode(QPainter::CompositionMode(QPainter::CompositionMode_Clear));
    QCOMPARE(QPainter::CompositionMode(QPainter::CompositionMode_Clear), obj1.compositionMode());
    obj1.setCompositionMode(QPainter::CompositionMode(QPainter::CompositionMode_Source));
    QCOMPARE(QPainter::CompositionMode(QPainter::CompositionMode_Source), obj1.compositionMode());
    obj1.setCompositionMode(QPainter::CompositionMode(QPainter::CompositionMode_Destination));
    QCOMPARE(QPainter::CompositionMode(QPainter::CompositionMode_Destination), obj1.compositionMode());
    obj1.setCompositionMode(QPainter::CompositionMode(QPainter::CompositionMode_SourceIn));
    QCOMPARE(QPainter::CompositionMode(QPainter::CompositionMode_SourceIn), obj1.compositionMode());
    obj1.setCompositionMode(QPainter::CompositionMode(QPainter::CompositionMode_DestinationIn));
    QCOMPARE(QPainter::CompositionMode(QPainter::CompositionMode_DestinationIn), obj1.compositionMode());
    obj1.setCompositionMode(QPainter::CompositionMode(QPainter::CompositionMode_SourceOut));
    QCOMPARE(QPainter::CompositionMode(QPainter::CompositionMode_SourceOut), obj1.compositionMode());
    obj1.setCompositionMode(QPainter::CompositionMode(QPainter::CompositionMode_DestinationOut));
    QCOMPARE(QPainter::CompositionMode(QPainter::CompositionMode_DestinationOut), obj1.compositionMode());
    obj1.setCompositionMode(QPainter::CompositionMode(QPainter::CompositionMode_SourceAtop));
    QCOMPARE(QPainter::CompositionMode(QPainter::CompositionMode_SourceAtop), obj1.compositionMode());
    obj1.setCompositionMode(QPainter::CompositionMode(QPainter::CompositionMode_DestinationAtop));
    QCOMPARE(QPainter::CompositionMode(QPainter::CompositionMode_DestinationAtop), obj1.compositionMode());
    obj1.setCompositionMode(QPainter::CompositionMode(QPainter::CompositionMode_Xor));
    QCOMPARE(QPainter::CompositionMode(QPainter::CompositionMode_Xor), obj1.compositionMode());

    // const QPen & QPainter::pen()
    // void QPainter::setPen(const QPen &)
    QPen var3(Qt::red);
    obj1.setPen(var3);
    QCOMPARE(var3, obj1.pen());
    obj1.setPen(QPen());
    QCOMPARE(QPen(), obj1.pen());

    // const QBrush & QPainter::brush()
    // void QPainter::setBrush(const QBrush &)
    QBrush var4(Qt::red);
    obj1.setBrush(var4);
    QCOMPARE(var4, obj1.brush());
    obj1.setBrush(QBrush());
    QCOMPARE(QBrush(), obj1.brush());

    // const QBrush & QPainter::background()
    // void QPainter::setBackground(const QBrush &)
    QBrush var5(Qt::yellow);
    obj1.setBackground(var5);
    QCOMPARE(var5, obj1.background());
    obj1.setBackground(QBrush());
    QCOMPARE(QBrush(), obj1.background());

    // bool QPainter::matrixEnabled()
    // void QPainter::setMatrixEnabled(bool)
    obj1.setMatrixEnabled(false);
    QCOMPARE(false, obj1.matrixEnabled());
    obj1.setMatrixEnabled(true);
    QCOMPARE(true, obj1.matrixEnabled());

    // bool QPainter::viewTransformEnabled()
    // void QPainter::setViewTransformEnabled(bool)
    obj1.setViewTransformEnabled(false);
    QCOMPARE(false, obj1.viewTransformEnabled());
    obj1.setViewTransformEnabled(true);
    QCOMPARE(true, obj1.viewTransformEnabled());
}

Q_DECLARE_METATYPE(QPixmap)
Q_DECLARE_METATYPE(QPolygon)
Q_DECLARE_METATYPE(QBrush)
Q_DECLARE_METATYPE(QPen)
Q_DECLARE_METATYPE(QFont)
Q_DECLARE_METATYPE(QColor)
Q_DECLARE_METATYPE(QRegion)

tst_QPainter::tst_QPainter()
{
    // QtTestCase sets this to false, but this turns off alpha pixmaps on Unix.
    QApplication::setDesktopSettingsAware(TRUE);
}

tst_QPainter::~tst_QPainter()
{
}

void tst_QPainter::init()
{
}

void tst_QPainter::cleanup()
{
}

/* tests the clipping operations in qt_format_text, making sure
   the clip rectangle after the call is the same as before
*/
void tst_QPainter::qt_format_text_clip()
{
    QVERIFY(1);
    QSKIP( "Needs fixing...", SkipAll);

    QWidget *w = new QWidget( 0 );

    int modes[] = { Qt::AlignVCenter|Qt::TextSingleLine,
		   Qt::AlignVCenter|Qt::TextSingleLine|Qt::TextDontClip,
		   Qt::AlignVCenter|Qt::TextWordWrap,
		   Qt::AlignVCenter|Qt::TextWordWrap|Qt::TextDontClip,
		   0
    };

    int *m = modes;
    while( *m ) {
	{
	    QPainter p( w );
	    QRegion clipreg = p.clipRegion();
	    bool hasClipping = p.hasClipping();
	    qreal tx = p.matrix().dx();
	    qreal ty = p.matrix().dy();

	    p.drawText( 10, 10, 100, 100, *m,
			"fooo" );

	    QVERIFY( clipreg == p.clipRegion() );
	    QVERIFY( hasClipping == p.hasClipping() );
	    QCOMPARE( tx, p.matrix().dx() );
	    QCOMPARE( ty, p.matrix().dy() );

	    p.setClipRect( QRect( 5, 5, 50, 50 ) );
	    clipreg = p.clipRegion();
	    hasClipping = p.hasClipping();

	    p.drawText( 10, 10, 100, 100, *m,
			"fooo" );

	    QVERIFY( clipreg == p.clipRegion() );
	    QVERIFY( hasClipping == p.hasClipping() );
	    QCOMPARE( tx, p.matrix().dx() );
	    QCOMPARE( ty, p.matrix().dy() );
	}
	{
	    QPainter p( w );
	    p.setMatrix( QMatrix( 2, 1, 3, 4, 5, 6 ) );
	    QRegion clipreg = p.clipRegion();
	    bool hasClipping = p.hasClipping();
	    qreal tx = p.matrix().dx();
	    qreal ty = p.matrix().dy();

	    p.drawText( 10, 10, 100, 100, *m,
			"fooo" );

	    QVERIFY( clipreg == p.clipRegion() );
	    QVERIFY( hasClipping == p.hasClipping() );
	    QCOMPARE( tx, p.matrix().dx() );
	    QCOMPARE( ty, p.matrix().dy() );

	    p.setClipRect( QRect( 5, 5, 50, 50 ) );
	    clipreg = p.clipRegion();
	    hasClipping = p.hasClipping();

	    p.drawText( 10, 10, 100, 100, *m,
			"fooo" );

	    QVERIFY( clipreg == p.clipRegion() );
	    QVERIFY( hasClipping == p.hasClipping() );
	    QCOMPARE( tx, p.matrix().dx() );
	    QCOMPARE( ty, p.matrix().dy() );
	}
	{
	    QPainter p( w );
	    QRegion clipreg = p.clipRegion();
	    bool hasClipping = p.hasClipping();
	    qreal tx = p.matrix().dx();
	    qreal ty = p.matrix().dy();

	    p.drawText( 10, 10, 100, 100, *m,
			"fooo" );

	    QVERIFY( clipreg == p.clipRegion() );
	    QVERIFY( hasClipping == p.hasClipping() );
	    QCOMPARE( tx, p.matrix().dx() );
	    QCOMPARE( ty, p.matrix().dy() );

	    p.setClipRect( QRect( 5, 5, 50, 50 ));
	    clipreg = p.clipRegion();
	    hasClipping = p.hasClipping();

	    p.drawText( 10, 10, 100, 100, *m,
			"fooo" );

	    QVERIFY( clipreg == p.clipRegion() );
	    QVERIFY( hasClipping == p.hasClipping() );
	    QCOMPARE( tx, p.matrix().dx() );
	    QCOMPARE( ty, p.matrix().dy() );
	}
	{
	    QPainter p( w );
	    p.setMatrix( QMatrix( 2, 1, 3, 4, 5, 6 ) );
	    QRegion clipreg = p.clipRegion();
	    bool hasClipping = p.hasClipping();
	    qreal tx = p.matrix().dx();
	    qreal ty = p.matrix().dy();

	    p.drawText( 10, 10, 100, 100, *m,
			"fooo" );

	    QVERIFY( clipreg == p.clipRegion() );
	    QVERIFY( hasClipping == p.hasClipping() );
	    QCOMPARE( tx, p.matrix().dx() );
	    QCOMPARE( ty, p.matrix().dy() );

	    p.setClipRect(QRect( 5, 5, 50, 50 ));
	    clipreg = p.clipRegion();
	    hasClipping = p.hasClipping();

	    p.drawText( 10, 10, 100, 100, *m,
			"fooo" );

	    QVERIFY( clipreg == p.clipRegion() );
	    QVERIFY( hasClipping == p.hasClipping() );
	    QCOMPARE( tx, p.matrix().dx() );
	    QCOMPARE( ty, p.matrix().dy() );
	}
	++m;
    }
    delete w;
}

/* tests the bounding rect calculations in qt_format_text, making sure
   the bounding rect has a reasonable value.
*/
void tst_QPainter::qt_format_text_boundingRect()
{
    QVERIFY(1);
    QSKIP( "Needs fixing...", SkipAll);

    {
	const char * strings[] = {
	    "a\n\nb",
	    "abc",
	    "a\n \nb",
	    "this is a longer string",
	    "\327\222\327\223\327\233\327\223\327\222\327\233\327\222\327\223\327\233",
	    "aa\327\222\327\233aa",
	    "\327\222\327\223\327\233\327\223\327\222\327\233\327\222\327\223\327\233",
	    "\327\222\327\233aa",
	    "linebreakatend\n",
	    "some text longer than 30 chars with a line break at the end\n",
	    "some text\nwith line breaks\nin the middle\nand at the end\n",
	    "foo\n\n\nfoo",
	    0
	};

	int modes[] = { Qt::AlignVCenter|Qt::TextSingleLine,
			Qt::AlignVCenter|Qt::TextSingleLine|Qt::TextDontClip,
			Qt::AlignVCenter|Qt::TextWordWrap,
			Qt::AlignVCenter|Qt::TextWordWrap|Qt::TextDontClip,
			Qt::AlignLeft,
			Qt::AlignCenter,
			Qt::AlignRight,
			0
	};

	QFont f;
	for(int i = 5; i < 15; ++i) {
	    f.setPointSize(i);
	    QFontMetrics fm(f);
	    const char **str = strings;
	    while( *str ) {
		int *m = modes;
		while( *m ) {
		    QRect br = fm.boundingRect( 0, 0, 2000, 100, *m, QString::fromUtf8( *str ) );
		    QVERIFY( br.width() < 800 );

		    QRect br2 = fm.boundingRect( br.x(), br.y(), br.width(), br.height(), *m, QString::fromUtf8( *str ) );
		    QCOMPARE( br, br2 );
#if 0
		    {
			QPrinter printer;
			printer.setOutputToFile(TRUE);
			printer.setOutputFileName("tmp.prn");
			QPainter p(&printer);
			QRect pbr = p.fontMetrics().boundingRect( 0, 0, 2000, 100, *m, QString::fromUtf8( *str ) );
			QCOMPARE(pbr, br);
		    }
#endif
#if !defined(Q_OS_WINCE) && !defined(Q_OS_SYMBIAN)
		    {
			QPrinter printer(QPrinter::HighResolution);
			if (printer.printerName().isEmpty()) {
			    QSKIP( "No printers installed, skipping bounding rect test",
				  SkipSingle );
			    break;
			}

			printer.setOutputFileName("tmp.prn");
			QPainter p(&printer);
			QRect pbr = p.fontMetrics().boundingRect( 0, 0, 12000, 600, *m, QString::fromUtf8( *str ) );
			QVERIFY(pbr.width() > 2*br.width());
			QVERIFY(pbr.height() > 2*br.height());
		    }
#endif
		    ++m;
		}
		++str;
	    }
	}
    }

    {
	const char * strings[] = {
	    "a",
	    "a\nb",
	    "a\n\nb",
	    "abc",
//	    "a\n \nb",
	    "this is a longer string",
//	    "\327\222\327\223\327\233\327\223\327\222\327\233\327\222\327\223\327\233",
//	    "aa\327\222\327\233aa",
//	    "\327\222\327\223\327\233\327\223\327\222\327\233\327\222\327\223\327\233",
//	    "\327\222\327\233aa",
//	    "linebreakatend\n",
//	    "some text longer than 30 chars with a line break at the end\n",
//	    "some text\nwith line breaks\nin the middle\nand at the end\n",
	    "foo\n\n\nfoo",
	    "a\n\n\n\n\nb",
	    "a\n\n\n\n\n\nb",
//	    "\347\231\273\351\214\262\346\203\205\345\240\261\343\201\214\350\246\213\343\201\244\343\201\213\343\202\211\343\201\252\343\201\204\343\201\213\347\204\241\345\212\271\343\201\252\343\201\237\343\202\201\343\200\201\nPhotoshop Album \343\202\222\350\265\267\345\213\225\343\201\247\343\201\215\343\201\276\343\201\233\343\202\223\343\200\202\345\206\215\343\202\244\343\203\263\343\202\271\343\203\210\343\203\274\343\203\253\343\201\227\343\201\246\343\201\217\343\201\240\343\201\225\343\201\204\343\200\202"
//	    "\347\231\273\351\214\262\346\203\205\345\240\261\343\201\214\350\246\213\343\201\244\343\201\213\343\202\211\343\201\252\343\201\204\343\201\213\347\204\241\345\212\271\343\201\252\343\201\237\343\202\201\343\200\201\n\343\202\222\350\265\267\345\213\225\343\201\247\343\201\215\343\201\276\343\201\233\343\202\223\343\200\202\345\206\215\343\202\244\343\203\263\343\202\271\343\203\210\343\203\274\343\203\253\343\201\227\343\201\246\343\201\217\343\201\240\343\201\225\343\201\204\343\200\202",
	    0
	};

	int modes[] = { Qt::AlignVCenter,
			Qt::AlignLeft,
			Qt::AlignCenter,
			Qt::AlignRight,
			0
	};


	QFont f;
	for(int i = 5; i < 15; ++i) {
	    f.setPointSize(i);
	    QFontMetrics fm(f);
	    const char **str = strings;
	    while( *str ) {
		int *m = modes;
		while( *m ) {
		    QString s = QString::fromUtf8(*str);
		    QRect br = fm.boundingRect(0, 0, 1000, 1000, *m, s );
		    int lines =
		    s.count("\n");
		    int expectedHeight = fm.height()+lines*fm.lineSpacing();
		    QCOMPARE(br.height(), expectedHeight);
		    ++m;
		}
		++str;
	    }
	    QRect br = fm.boundingRect(0, 0, 100, 0, Qt::TextWordWrap,
		 "A paragraph with gggggggggggggggggggggggggggggggggggg in the middle.");
	    QVERIFY(br.height() >= fm.height()+2*fm.lineSpacing());
	}
    }
}


static const char* const maskSource_data[] = {
"16 13 6 1",
". c None",
"d c #000000",
"# c #999999",
"c c #cccccc",
"b c #ffff00",
"a c #ffffff",
"...#####........",
"..#aaaaa#.......",
".#abcbcba######.",
".#acbcbcaaaaaa#d",
".#abcbcbcbcbcb#d",
"#############b#d",
"#aaaaaaaaaa##c#d",
"#abcbcbcbcbbd##d",
".#abcbcbcbcbcd#d",
".#acbcbcbcbcbd#d",
"..#acbcbcbcbb#dd",
"..#############d",
"...ddddddddddddd"};

static const char* const maskResult_data[] = {
"16 13 6 1",
". c #ff0000",
"d c #000000",
"# c #999999",
"c c #cccccc",
"b c #ffff00",
"a c #ffffff",
"...#####........",
"..#aaaaa#.......",
".#abcbcba######.",
".#acbcbcaaaaaa#d",
".#abcbcbcbcbcb#d",
"#############b#d",
"#aaaaaaaaaa##c#d",
"#abcbcbcbcbbd##d",
".#abcbcbcbcbcd#d",
".#acbcbcbcbcbd#d",
"..#acbcbcbcbb#dd",
"..#############d",
"...ddddddddddddd"};


void tst_QPainter::drawPixmap_comp_data()
{
    if (qApp->desktop()->depth() < 24) {
        QSKIP("Test only works on 32 bit displays", SkipAll);
        return;
    }

    QTest::addColumn<uint>("dest");
    QTest::addColumn<uint>("source");

    QTest::newRow("0% on 0%, 1")           << 0x00000000u<< 0x00000000u;
    QTest::newRow("0% on 0%, 2")           << 0x00007fffu << 0x00ff007fu;

    QTest::newRow("50% on a=0%")           << 0x00000000u << 0x7fff0000u;
    QTest::newRow("50% on a=50%")          << 0x7f000000u << 0x7fff0000u;
    QTest::newRow("50% on deadbeef")      << 0xdeafbeefu <<  0x7fff0000u;
    QTest::newRow("deadbeef on a=0%")      << 0x00000000u << 0xdeadbeefu;
    QTest::newRow("deadbeef on a=50%")     << 0x7f000000u << 0xdeadbeefu;
    QTest::newRow("50% blue on 50% red")   << 0x7fff0000u << 0x7f0000ffu;
    QTest::newRow("50% blue on 50% green") << 0x7f00ff00u << 0x7f0000ffu;
    QTest::newRow("50% red on 50% green")  << 0x7f00ff00u << 0x7fff0000u;
    QTest::newRow("0% on 50%")             << 0x7fff00ffu << 0x00ffffffu;
    QTest::newRow("100% on deadbeef")      << 0xdeafbeefu << 0xffabcdefu;
    QTest::newRow("100% on a=0%")           << 0x00000000u << 0xffabcdefu;
}

QRgb qt_compose_alpha(QRgb source, QRgb dest)
{
    int r1 = qRed(dest), g1 = qGreen(dest), b1 = qBlue(dest), a1 = qAlpha(dest);
    int r2 = qRed(source), g2 = qGreen(source), b2 = qBlue(source), a2 = qAlpha(source);

    int alpha = qMin(a2 + ((255 - a2) * a1 + 127) / 255, 255);
    if (alpha == 0)
        return qRgba(0, 0, 0, 0);

    return qRgba(
        qMin((r2 * a2 + (255 - a2) * r1 * a1 / 255) / alpha, 255),
        qMin((g2 * a2 + (255 - a2) * g1 * a1 / 255) / alpha, 255),
        qMin((b2 * a2 + (255 - a2) * b1 * a1 / 255) / alpha, 255),
        alpha);
}

/* Tests that drawing masked pixmaps works
*/
void tst_QPainter::drawPixmap_comp()
{
#ifdef Q_WS_MAC
    QSKIP("Mac has other ideas about alpha composition", SkipAll);
#endif

    QFETCH(uint, dest);
    QFETCH(uint, source);

    QRgb expected = qt_compose_alpha(source, dest);

    QColor c1(qRed(dest), qGreen(dest), qBlue(dest), qAlpha(dest));
    QColor c2(qRed(source), qGreen(source), qBlue(source), qAlpha(source));

    QPixmap destPm(10, 10), srcPm(10, 10);
    destPm.fill(c1);
    srcPm.fill(c2);

#if defined(Q_WS_X11)
    if (!destPm.x11PictureHandle())
        QSKIP("Requires XRender support", SkipAll);
#endif

    QPainter p(&destPm);
    p.drawPixmap(0, 0, srcPm);
    p.end();

    QImage result = destPm.toImage().convertToFormat(QImage::Format_ARGB32);
    bool different = false;
    for (int y=0; y<result.height(); ++y)
        for (int x=0; x<result.width(); ++x) {
	    bool diff;
            if (qAlpha(expected) == 0) {
                diff = qAlpha(result.pixel(x, y)) != 0;
            } else {
                // Compensate for possible roundoff / platform fudge
                int off = 1;
                QRgb pix = result.pixel(x, y);
                diff = (qAbs(qRed(pix) - qRed(expected)) > off)
                             || (qAbs(qGreen(pix) - qGreen(expected)) > off)
                             || (qAbs(qBlue(pix) - qBlue(expected)) > off)
                             || (qAbs(qAlpha(pix) - qAlpha(expected)) > off);
            }
	    if (diff && !different)
		qDebug( "Different at %d,%d pixel [%d,%d,%d,%d] expected [%d,%d,%d,%d]", x, y,
                        qRed(result.pixel(x, y)), qGreen(result.pixel(x, y)),
                        qBlue(result.pixel(x, y)), qAlpha(result.pixel(x, y)),
                        qRed(expected), qGreen(expected), qBlue(expected), qAlpha(expected));
	    different |= diff;
        }

    QVERIFY(!different);
}

void tst_QPainter::saveAndRestore_data()
{
    QVERIFY(1);

    QTest::addColumn<QFont>("font");
    QTest::addColumn<QPen>("pen");
    QTest::addColumn<QBrush>("brush");
    QTest::addColumn<QColor>("backgroundColor");
    QTest::addColumn<int>("backgroundMode");
    QTest::addColumn<QPoint>("brushOrigin");
    QTest::addColumn<QRegion>("clipRegion");
    QTest::addColumn<QRect>("window");
    QTest::addColumn<QRect>("viewport");

    QPixmap pixmap(1, 1);
    QPainter p(&pixmap);
    QFont font = p.font();
    QPen pen = p.pen();
    QBrush brush = p.brush();
    QColor backgroundColor = p.background().color();
    Qt::BGMode backgroundMode = p.backgroundMode();
    QPoint brushOrigin = p.brushOrigin();
    QRegion clipRegion = p.clipRegion();
    QRect window = p.window();
    QRect viewport = p.viewport();

    QTest::newRow("Original") << font << pen << brush << backgroundColor << int(backgroundMode)
	    << brushOrigin << clipRegion << window << viewport;

    QFont font2 = font;
    font2.setPointSize( 24 );
    QTest::newRow("Modified font.pointSize, brush, backgroundColor, backgroundMode")
            << font2 << pen << QBrush(Qt::red) << QColor(Qt::blue) << int(Qt::TransparentMode)
	    << brushOrigin << clipRegion << window << viewport;

    font2 = font;
    font2.setPixelSize( 20 );
    QTest::newRow("Modified font.pixelSize, brushOrigin, pos")
            << font2 << pen << brush << backgroundColor << int(backgroundMode)
	    << QPoint( 50, 32 ) << clipRegion << window << viewport;

    QTest::newRow("Modified clipRegion, window, viewport")
            << font << pen << brush << backgroundColor << int(backgroundMode)
	    << brushOrigin << clipRegion.subtracted(QRect(10,10,50,30))
	    << QRect(-500, -500, 500, 500 ) << QRect( 0, 0, 50, 50 );
}

void tst_QPainter::saveAndRestore()
{
    QFETCH( QFont, font );
    QFETCH( QPen, pen );
    QFETCH( QBrush, brush );
    QFETCH( QColor, backgroundColor );
    QFETCH( int, backgroundMode );
    QFETCH( QPoint, brushOrigin );
    QFETCH( QRegion, clipRegion );
    QFETCH( QRect, window );
    QFETCH( QRect, viewport );

    QPixmap pixmap(1, 1);
    QPainter painter(&pixmap);

    QFont font_org = painter.font();
    QPen pen_org = painter.pen();
    QBrush brush_org = painter.brush();
    QColor backgroundColor_org = painter.background().color();
    Qt::BGMode backgroundMode_org = painter.backgroundMode();
    QPoint brushOrigin_org = painter.brushOrigin();
    QRegion clipRegion_org = painter.clipRegion();
    QRect window_org = painter.window();
    QRect viewport_org = painter.viewport();

    painter.save();
    painter.setFont( font );
    painter.setPen( QPen(pen) );
    painter.setBrush( brush );
    painter.setBackground( backgroundColor );
    painter.setBackgroundMode( (Qt::BGMode)backgroundMode );
    painter.setBrushOrigin( brushOrigin );
    painter.setClipRegion( clipRegion );
    painter.setWindow( window );
    painter.setViewport( viewport );
    painter.restore();

    QCOMPARE( painter.font(), font_org );
    QCOMPARE( painter.font().pointSize(), font_org.pointSize() );
    QCOMPARE( painter.font().pixelSize(), font_org.pixelSize() );
    QCOMPARE( painter.pen(), pen_org );
    QCOMPARE( painter.brush(), brush_org );
    QCOMPARE( painter.background().color(), backgroundColor_org );
    QCOMPARE( painter.backgroundMode(), backgroundMode_org );
    QCOMPARE( painter.brushOrigin(), brushOrigin_org );
    QCOMPARE( painter.clipRegion(), clipRegion_org );
    QCOMPARE( painter.window(), window_org );
    QCOMPARE( painter.viewport(), viewport_org );
}

/*
   Helper functions
*/

QColor tst_QPainter::baseColor( int k, int intensity )
{
    int r = ( k & 1 ) * intensity;
    int g = ( (k>>1) & 1 ) * intensity;
    int b = ( (k>>2) & 1 ) * intensity;
    return QColor( r, g, b );
}

QImage tst_QPainter::getResImage( const QString &dir, const QString &addition, const QString &extension )
{
    QImage res;
    QString resFilename  = dir + QString( "/res_%1." ).arg( addition ) + extension;
    if ( !res.load( resFilename ) ) {
        QWARN(QString("Could not load result data %s %1").arg(resFilename).toLatin1());
        return QImage();
    }
    return res;
}

QBitmap tst_QPainter::getBitmap( const QString &dir, const QString &filename, bool mask )
{
    QBitmap bm;
    QString bmFilename = dir + QString( "/%1.xbm" ).arg( filename );
    if ( !bm.load( bmFilename ) ) {
        QWARN(QString("Could not load bitmap '%1'").arg(bmFilename).toLatin1());
        return QBitmap();
    }
    if ( mask ) {
	QBitmap mask;
	QString maskFilename = dir + QString( "/%1-mask.xbm" ).arg( filename );
	if ( !mask.load( maskFilename ) ) {
        QWARN(QString("Could not load mask '%1'").arg(maskFilename).toLatin1());
        return QBitmap();
	}
	bm.setMask( mask );
    }
    return bm;
}

static int getPaintedPixels(const QImage &image, const QColor &background)
{
    uint color = background.rgba();

    int pixels = 0;

    for (int y = 0; y < image.height(); ++y)
        for (int x = 0; x < image.width(); ++x)
            if (image.pixel(x, y) != color)
                ++pixels;

    return pixels;
}

static QRect getPaintedSize(const QImage &image, const QColor &background)
{
    // not the fastest but at least it works..
    int xmin = image.width() + 1;
    int xmax = -1;
    int ymin = image.height() +1;
    int ymax = -1;

    uint color = background.rgba();

    for ( int y = 0; y < image.height(); ++y ) {
	for ( int x = 0; x < image.width(); ++x ) {
            QRgb pixel = image.pixel( x, y );
	    if ( pixel != color && x < xmin )
		xmin = x;
	    if ( pixel != color && x > xmax )
		xmax = x;
	    if ( pixel != color && y < ymin )
		ymin = y;
	    if ( pixel != color && y > ymax )
		ymax = y;
	}
    }

    return QRect(xmin, ymin, xmax - xmin + 1, ymax - ymin + 1);
}

static QRect getPaintedSize(const QPixmap &pm, const QColor &background)
{
    return getPaintedSize(pm.toImage(), background);
}

void tst_QPainter::initFrom()
{
    QWidget *widget = new QWidget();
    QPalette pal = widget->palette();
    pal.setColor(QPalette::Foreground, QColor(255, 0, 0));
    pal.setBrush(QPalette::Background, QColor(0, 255, 0));
    widget->setPalette(pal);

    QFont font = widget->font();
    font.setPointSize(26);
    font.setItalic(true);
    widget->setFont(font);

    QPixmap pm(100, 100);
    QPainter p(&pm);
    p.initFrom(widget);

    QCOMPARE(p.font(), font);
    QCOMPARE(p.pen().color(), pal.color(QPalette::Foreground));
    QCOMPARE(p.background(), pal.background());

    delete widget;
}

void tst_QPainter::drawBorderPixmap()
{
    QPixmap src(79,79);
    src.fill(Qt::transparent);

    QImage pm(200,200,QImage::Format_RGB32);
    QPainter p(&pm);
    p.setTransform(QTransform(-1,0,0,-1,173.5,153.5));
    qDrawBorderPixmap(&p, QRect(0,0,75,105), QMargins(39,39,39,39), src, QRect(0,0,79,79), QMargins(39,39,39,39),
                       QTileRules(Qt::StretchTile,Qt::StretchTile), 0);
}

void tst_QPainter::drawPixmapFragments()
{
    QPixmap origPixmap(20, 20);
    QPixmap resPixmap(20, 20);
    QPainter::PixmapFragment fragments[4] = { {15, 15,  0,  0, 10, 10, 1, 1, 0, 1},
                                              { 5, 15, 10,  0, 10, 10, 1, 1, 0, 1},
                                              {15,  5,  0, 10, 10, 10, 1, 1, 0, 1},
                                              { 5,  5, 10, 10, 10, 10, 1, 1, 0, 1} };
    {
        QPainter p(&origPixmap);
        p.fillRect(0, 0, 10, 10, Qt::red);
        p.fillRect(10, 0, 10, 10, Qt::green);
        p.fillRect(0, 10, 10, 10, Qt::blue);
        p.fillRect(10, 10, 10, 10, Qt::yellow);
    }
    {
        QPainter p(&resPixmap);
        p.drawPixmapFragments(fragments, 4, origPixmap);
    }

    QImage origImage = origPixmap.toImage().convertToFormat(QImage::Format_ARGB32);
    QImage resImage = resPixmap.toImage().convertToFormat(QImage::Format_ARGB32);

    QVERIFY(resImage.size() == resPixmap.size());
    QVERIFY(resImage.pixel(5, 5) == origImage.pixel(15, 15));
    QVERIFY(resImage.pixel(5, 15) == origImage.pixel(15, 5));
    QVERIFY(resImage.pixel(15, 5) == origImage.pixel(5, 15));
    QVERIFY(resImage.pixel(15, 15) == origImage.pixel(5, 5));


    QPainter::PixmapFragment fragment = QPainter::PixmapFragment::create(QPointF(20, 20), QRectF(30, 30, 2, 2));
    QVERIFY(fragment.x == 20);
    QVERIFY(fragment.y == 20);
    QVERIFY(fragment.sourceLeft == 30);
    QVERIFY(fragment.sourceTop == 30);
    QVERIFY(fragment.width == 2);
    QVERIFY(fragment.height == 2);
    QVERIFY(fragment.scaleX == 1);
    QVERIFY(fragment.scaleY == 1);
    QVERIFY(fragment.rotation == 0);
    QVERIFY(fragment.opacity == 1);
}

void tst_QPainter::drawLine_data()
{
    QTest::addColumn<QLine>("line");

    QTest::newRow("0-45") << QLine(0, 20, 100, 0);
    QTest::newRow("45-90") << QLine(0, 100, 20, 0);
    QTest::newRow("90-135") << QLine(20, 100, 0, 0);
    QTest::newRow("135-180") << QLine(100, 20, 0, 0);
    QTest::newRow("180-225") << QLine(100, 0, 0, 20);
    QTest::newRow("225-270") << QLine(20, 0, 0, 100);
    QTest::newRow("270-315") << QLine(0, 0, 20, 100);
    QTest::newRow("315-360") << QLine(0, 0, 100, 20);
}

void tst_QPainter::drawLine()
{
    const int offset = 5;
    const int epsilon = 1; // allow for one pixel difference

    QFETCH(QLine, line);

    QPixmap pixmapUnclipped(qMin(line.x1(), line.x2())
                            + 2*offset + qAbs(line.dx()),
                            qMin(line.y1(), line.y2())
                            + 2*offset + qAbs(line.dy()));

    { // unclipped
        pixmapUnclipped.fill(Qt::white);
        QPainter p(&pixmapUnclipped);
        p.translate(offset, offset);
        p.setPen(QPen(Qt::black));
        p.drawLine(line);
        p.end();

        const QRect painted = getPaintedSize(pixmapUnclipped, Qt::white);

        QLine l = line;
        l.translate(offset, offset);
        QVERIFY(qAbs(painted.width() - qAbs(l.dx())) <= epsilon);
        QVERIFY(qAbs(painted.height() - qAbs(l.dy())) <= epsilon);
        QVERIFY(qAbs(painted.top() - qMin(l.y1(), l.y2())) <= epsilon);
        QVERIFY(qAbs(painted.left() - qMin(l.x1(), l.x2())) <= epsilon);
        QVERIFY(qAbs(painted.bottom() - qMax(l.y1(), l.y2())) <= epsilon);
        QVERIFY(qAbs(painted.right() - qMax(l.x1(), l.x2())) <= epsilon);
    }

    QPixmap pixmapClipped(qMin(line.x1(), line.x2())
                          + 2*offset + qAbs(line.dx()),
                          qMin(line.y1(), line.y2())
                          + 2*offset + qAbs(line.dy()));
    { // clipped
        const QRect clip = QRect(line.p1(), line.p2()).normalized();

        pixmapClipped.fill(Qt::white);
        QPainter p(&pixmapClipped);
        p.translate(offset, offset);
        p.setClipRect(clip);
        p.setPen(QPen(Qt::black));
        p.drawLine(line);
        p.end();
    }

    const QImage unclipped = pixmapUnclipped.toImage();
    const QImage clipped = pixmapClipped.toImage();
    QCOMPARE(unclipped, clipped);
}

void tst_QPainter::drawLine_clipped()
{
    QImage image(16, 1, QImage::Format_ARGB32_Premultiplied);
    image.fill(0x0);

    QPainter p(&image);
    p.setPen(QPen(Qt::black, 10));

    // this should fill the whole image
    p.drawLine(-1, -1, 17, 1);
    p.end();

    for (int x = 0; x < 16; ++x)
        QCOMPARE(image.pixel(x, 0), 0xff000000);
}

void tst_QPainter::drawLine_task121143()
{
    QPen pen(Qt::black);

    QImage image(5, 5, QImage::Format_ARGB32_Premultiplied);
    image.fill(0xffffffff);
    QPainter p(&image);
    p.setPen(pen);
    p.drawLine(QLine(0, 0+4, 0+4, 0));
    p.end();

    QImage expected(5, 5, QImage::Format_ARGB32_Premultiplied);
    expected.fill(0xffffffff);
    for (int x = 0; x < 5; ++x)
        expected.setPixel(x, 5-x-1, pen.color().rgb());

    QCOMPARE(image, expected);
}

void tst_QPainter::drawLine_task190634()
{
    QPen pen(Qt::black, 3);

    QImage image(32, 32, QImage::Format_ARGB32_Premultiplied);
    QPainter p(&image);
    p.fillRect(0, 0, image.width(), image.height(), Qt::white);

    p.setPen(pen);
    p.drawLine(QLineF(2, -1.6, 10, -1.6));
    p.end();

    const uint *data = reinterpret_cast<uint *>(image.bits());

    for (int i = 0; i < image.width() * image.height(); ++i)
        QCOMPARE(data[i], 0xffffffff);

    p.begin(&image);
    p.fillRect(0, 0, image.width(), image.height(), Qt::white);

    p.setPen(pen);
    p.drawLine(QLineF(-1.6, 2, -1.6, 10));
    p.end();

    data = reinterpret_cast<uint *>(image.bits());

    for (int i = 0; i < image.width() * image.height(); ++i)
        QCOMPARE(data[i], 0xffffffff);

    p.begin(&image);
    p.fillRect(0, 0, image.width(), image.height(), Qt::white);

    p.setPen(pen);
    p.drawLine( QPoint(2,-2), QPoint(3,-5) );
    p.end();

    data = reinterpret_cast<uint *>(image.bits());

    for (int i = 0; i < image.width() * image.height(); ++i)
        QCOMPARE(data[i], 0xffffffff);
}

void tst_QPainter::drawLine_task229459()
{
    QImage image(32, 32, QImage::Format_ARGB32_Premultiplied);
    image.fill(0x0);
    QPen pen(Qt::black, 64);

    QPainter p(&image);
    p.setPen(pen);
    p.drawLine(-8, -8, 10000000, 10000000);
    p.end();

    QImage expected = image;
    expected.fill(0xff000000);

    QCOMPARE(image, expected);
}

void tst_QPainter::drawLine_task234891()
{
    QImage img(100, 1000, QImage::Format_ARGB32_Premultiplied);
    img.fill(0x0);
    QImage expected = img;

    QPainter p(&img);
    p.setPen(QPen(QBrush(QColor(255,0,0)), 6));
    p.drawLine(QPointF(25000,100),QPointF(30000,105));

    p.setPen(QPen(QBrush(QColor(0,255,0)), 6));
    p.drawLine(QPointF(30000,150),QPointF(35000,155));

    p.setPen(QPen(QBrush(QColor(0,0,255)), 6));
    p.drawLine(QPointF(65000,200),QPointF(66000,205));

    QCOMPARE(expected, img);
}

void tst_QPainter::drawLine_task216948()
{
    QImage img(1, 10, QImage::Format_ARGB32_Premultiplied);
    img.fill(0x0);

    QPainter p(&img);
    QLine line(10, 0, 10, 10);
    p.translate(-10, 0);
    p.drawLine(line);
    p.end();

    for (int i = 0; i < img.height(); ++i)
        QCOMPARE(img.pixel(0, i), QColor(Qt::black).rgba());
}

void tst_QPainter::drawRect()
{
    QFETCH(QRect, rect);
    QFETCH(bool, usePen);

    QPixmap pixmap(rect.x() + rect.width() + 10,
                   rect.y() + rect.height() + 10);
    {
        pixmap.fill(Qt::white);
        QPainter p(&pixmap);
        p.setPen(usePen ? QPen(Qt::black) : QPen(Qt::NoPen));
        p.setBrush(Qt::black);
        p.drawRect(rect);
        p.end();

        int increment = usePen ? 1 : 0;

        const QRect painted = getPaintedSize(pixmap, Qt::white);
        QCOMPARE(painted.width(), rect.width() + increment);
        QCOMPARE(painted.height(), rect.height() + increment);
    }

#ifdef QT3_SUPPORT
    {
        if (usePen && (rect.width() < 2 || rect.height() < 2))
            return;
        pixmap.fill(Qt::white);
        Q3Painter p(&pixmap);
        p.setPen(usePen ? QPen(Qt::black) : QPen(Qt::NoPen));
        p.setBrush(Qt::black);
        p.drawRect(rect);
        p.end();

        const QRect painted = getPaintedSize(pixmap, Qt::white);

        QCOMPARE(painted.width(), rect.width());
        QCOMPARE(painted.height(), rect.height());
    }
#endif
}

void tst_QPainter::drawRect2()
{
    QImage image(64, 64, QImage::Format_ARGB32_Premultiplied);
    {
        image.fill(0xffffffff);

        QTransform transform(0.368567, 0, 0, 0, 0.368567, 0, 0.0289, 0.0289, 1);

        QPainter p(&image);
        p.setTransform(transform);
        p.setBrush(Qt::red);
        p.setPen(Qt::NoPen);
        p.drawRect(QRect(14, 14, 39, 39));
        p.end();

        QRect fill = getPaintedSize(image, Qt::white);
        image.fill(0xffffffff);

        p.begin(&image);
        p.setTransform(transform);
        p.drawRect(QRect(14, 14, 39, 39));
        p.end();

        QRect stroke = getPaintedSize(image, Qt::white);
        QCOMPARE(stroke.adjusted(1, 1, 0, 0), fill.adjusted(0, 0, 1, 1));
    }
}

void tst_QPainter::fillRect()
{
    QImage image(100, 100, QImage::Format_ARGB32_Premultiplied);
    image.fill(QColor(0, 0, 0, 0).rgba());

    QPainter p(&image);

    p.fillRect(0, 0, 100, 100, QColor(255, 0, 0, 127));

//    pixmap.save("bla1.png", "PNG");
    QCOMPARE(getPaintedSize(image, QColor(0, 0, 0, 0)),
             QRect(0, 0, 100, 100));
    QCOMPARE(getPaintedSize(image, QColor(127, 0, 0, 127)).isValid(),
             QRect().isValid());

    p.setCompositionMode(QPainter::CompositionMode_SourceIn);
    p.fillRect(50, 0, 50, 100, QColor(0, 0, 255, 255));

    QCOMPARE(getPaintedSize(image, QColor(127, 0, 0, 127)),
             QRect(50, 0, 50, 100));
    QCOMPARE(getPaintedSize(image, QColor(0, 0, 127, 127)),
             QRect(0, 0, 50, 100));
}

void tst_QPainter::fillRect2()
{
    QRgb background = 0x0;

    QImage img(1, 20, QImage::Format_ARGB32_Premultiplied);
    img.fill(background);

    QPainter p(&img);

    QRectF rect(0, 1, 1.2, 18);
    p.fillRect(rect, Qt::black);

    p.end();

    QCOMPARE(img.pixel(0, 0), background);
    QCOMPARE(img.pixel(0, img.height() - 1), background);

    QCOMPARE(img.pixel(0, 1), img.pixel(0, 2));
    QCOMPARE(img.pixel(0, img.height() - 2), img.pixel(0, img.height() - 3));
}

void tst_QPainter::fillRect3()
{
    QImage img(1, 1, QImage::Format_ARGB32_Premultiplied);
    img.fill(QColor(Qt::black).rgba());

    QPainter p(&img);
    p.setCompositionMode(QPainter::CompositionMode_Source);
    p.fillRect(img.rect(), Qt::transparent);
    p.end();

    QCOMPARE(img.pixel(0, 0), 0U);
}

void tst_QPainter::fillRect4()
{
    QImage image(100, 1, QImage::Format_ARGB32_Premultiplied);
    image.fill(0x0);

    QImage expected = image;
    expected.fill(0xffffffff);

    QPainter p(&image);
    p.scale(1.1, 1);
    p.setPen(Qt::NoPen);

    for (int i = 0; i < 33; ++i)
        p.fillRect(QRectF(3 * i, 0, 3, 1), Qt::white);

    p.end();

    QCOMPARE(image, expected);
}

void tst_QPainter::drawPath_data()
{
    QTest::addColumn<QPainterPath>("path");
    QTest::addColumn<QRect>("expectedBounds");
    QTest::addColumn<int>("expectedPixels");

    {
        QPainterPath p;
        p.addRect(2, 2, 10, 10);
        QTest::newRow("int-aligned rect") << p << QRect(2, 2, 10, 10) << 10 * 10;
    }

    {
        QPainterPath p;
        p.addRect(2.25, 2.25, 10, 10);
        QTest::newRow("non-aligned rect") << p << QRect(3, 3, 10, 10) << 10 * 10;
    }

    {
        QPainterPath p;
        p.addRect(2.25, 2.25, 10.5, 10.5);
        QTest::newRow("non-aligned rect 2") << p << QRect(3, 3, 10, 10) << 10 * 10;
    }

    {
        QPainterPath p;
        p.addRect(2.5, 2.5, 10, 10);
        QTest::newRow("non-aligned rect 3") << p << QRect(3, 3, 10, 10) << 10 * 10;
    }

    {
        QPainterPath p;
        p.addRect(2, 2, 10, 10);
        p.addRect(4, 4, 6, 6);
        QTest::newRow("rect-in-rect") << p << QRect(2, 2, 10, 10) << 10 * 10 - 6 * 6;
    }

    {
        QPainterPath p;
        p.addRect(2, 2, 10, 10);
        p.addRect(4, 4, 6, 6);
        p.addRect(6, 6, 2, 2);
        QTest::newRow("rect-in-rect-in-rect") << p << QRect(2, 2, 10, 10) << 10 * 10 - 6 * 6 + 2 * 2;
    }
}

void tst_QPainter::drawPath()
{
    QFETCH(QPainterPath, path);
    QFETCH(QRect, expectedBounds);
    QFETCH(int, expectedPixels);

    const int offset = 2;

    QImage image(expectedBounds.width() + 2 * offset, expectedBounds.height() + 2 * offset,
                 QImage::Format_ARGB32_Premultiplied);
    image.fill(QColor(Qt::white).rgb());

    QPainter p(&image);
    p.setPen(Qt::NoPen);
    p.setBrush(Qt::black);
    p.translate(offset - expectedBounds.left(), offset - expectedBounds.top());
    p.drawPath(path);
    p.end();

    const QRect paintedBounds = getPaintedSize(image, Qt::white);

    QCOMPARE(paintedBounds.x(), offset);
    QCOMPARE(paintedBounds.y(), offset);
    QCOMPARE(paintedBounds.width(), expectedBounds.width());
    QCOMPARE(paintedBounds.height(), expectedBounds.height());

    if (expectedPixels != -1) {
        int paintedPixels = getPaintedPixels(image, Qt::white);
        QCOMPARE(paintedPixels, expectedPixels);
    }
}

void tst_QPainter::drawPath2()
{
    const int w = 50;

    for (int h = 5; h < 200; ++h) {
        QPainterPath p1, p2;
        p1.lineTo(w, 0);
        p1.lineTo(w, h);

        p2.lineTo(w, h);
        p2.lineTo(0, h);

        const int offset = 2;

        QImage image(w + 2 * offset, h + 2 * offset,
                     QImage::Format_ARGB32_Premultiplied);
        image.fill(QColor(Qt::white).rgb());

        QPainter p(&image);
        p.setPen(Qt::NoPen);
        p.setBrush(Qt::black);
        p.translate(offset, offset);
        p.drawPath(p1);
        p.end();

        const int p1Pixels = getPaintedPixels(image, Qt::white);

        image.fill(QColor(Qt::white).rgb());
        p.begin(&image);
        p.setPen(Qt::NoPen);
        p.setBrush(Qt::black);
        p.translate(offset, offset);
        p.drawPath(p2);
        p.end();

        const int p2Pixels = getPaintedPixels(image, Qt::white);

        QCOMPARE(p1Pixels + p2Pixels, w * h);
    }
}

void tst_QPainter::drawPath3()
{
#if !defined(Q_OS_WINCE) && !defined(Q_OS_SYMBIAN)
    QImage imgA(400, 400, QImage::Format_RGB32);
#else
    QImage imgA(100, 100, QImage::Format_RGB32);
#endif
    imgA.fill(0xffffff);
    QImage imgB = imgA;

    QPainterPath path;
    for (int y = 0; y < imgA.height(); ++y) {
        for (int x = 0; x < imgA.width(); ++x) {
            if ((x + y) & 1) {
                imgA.setPixel(x, y, 0);
                path.addRect(x, y, 1, 1);
            }
        }
    }

    QPainter p(&imgB);
    p.setPen(Qt::NoPen);
    p.setBrush(Qt::black);

    p.drawPath(path);
    p.end();

    QVERIFY(imgA == imgB);

    imgA.invertPixels();
    imgB.fill(0xffffff);

    p.begin(&imgB);
    p.setPen(Qt::NoPen);
    p.setBrush(Qt::black);

    QRectF rect(0, 0, imgA.width(), imgA.height());
    path.addRect(rect.adjusted(-10, -10, 10, 10));
    p.drawPath(path);
    p.end();

    QVERIFY(imgA == imgB);

    path.setFillRule(Qt::WindingFill);
    imgB.fill(0xffffff);

    p.begin(&imgB);
    p.setPen(Qt::NoPen);
    p.setBrush(Qt::black);
    QRect clip = rect.adjusted(10, 10, -10, -10).toRect();
    p.setClipRect(clip);
    p.drawPath(path);
    p.end();

    QCOMPARE(getPaintedPixels(imgB, Qt::white), clip.width() * clip.height());
}

void tst_QPainter::drawEllipse_data()
{
    QTest::addColumn<QSize>("size");
    QTest::addColumn<bool>("usePen");

    // The current drawEllipse algorithm (drawEllipse_midpoint_i in
    // qpaintengine_raster.cpp) draws ellipses that are too wide if the
    // ratio between width and hight is too large/small (task 114874). Those
    // ratios are therefore currently avoided.
    for (int w = 10; w < 128; w += 7) {
        for (int h = w/2; h < qMin(2*w, 128); h += 13) {
            QString s = QString("%1x%2").arg(w).arg(h);
            QTest::newRow(QString("%1 with pen").arg(s).toLatin1()) << QSize(w, h) << true;
            QTest::newRow(QString("%1 no pen").arg(s).toLatin1()) << QSize(w, h) << false;
        }
    }
}

void tst_QPainter::drawEllipse()
{
    QFETCH(QSize, size);
    QFETCH(bool, usePen);

    const int offset = 10;
    QRect rect(QPoint(offset, offset), size);

    QImage image(size.width() + 2 * offset, size.height() + 2 * offset,
                 QImage::Format_ARGB32_Premultiplied);
    image.fill(QColor(Qt::white).rgb());

    QPainter p(&image);
    p.setPen(usePen ? QPen(Qt::black) : QPen(Qt::NoPen));
    p.setBrush(Qt::black);
    p.drawEllipse(rect);
    p.end();

    QPixmap pixmap = QPixmap::fromImage(image);

    const QRect painted = getPaintedSize(pixmap, Qt::white);

    QCOMPARE(painted.x(), rect.x());
    QCOMPARE(painted.y(), rect.y() + (usePen ? 0 : 1));
    QCOMPARE(painted.width(), size.width() + (usePen ? 1 : 0));
    QCOMPARE(painted.height(), size.height() + (usePen ? 1 : -1));
}

void tst_QPainter::drawClippedEllipse_data()
{
    QTest::addColumn<QRect>("rect");

    for (int w = 20; w < 128; w += 7) {
        for (int h = w/2; h < qMin(2*w, 128); h += 13) {
            QString s = QString("%1x%2").arg(w).arg(h);
            QTest::newRow(QString("%1 top").arg(s).toLatin1()) << QRect(0, -h/2, w, h);
            QTest::newRow(QString("%1 topright").arg(s).toLatin1()) << QRect(w/2, -h/2, w, h);
            QTest::newRow(QString("%1 right").arg(s).toLatin1()) << QRect(w/2, 0, w, h);
            QTest::newRow(QString("%1 bottomright").arg(s).toLatin1()) << QRect(w/2, h/2, w, h);
            QTest::newRow(QString("%1 bottom").arg(s).toLatin1()) << QRect(0, h/2, w, h);
            QTest::newRow(QString("%1 bottomleft").arg(s).toLatin1()) << QRect(-w/2, h/2, w, h);
            QTest::newRow(QString("%1 left").arg(s).toLatin1()) << QRect(-w/2, 0, w, h);
            QTest::newRow(QString("%1 topleft").arg(s).toLatin1()) << QRect(-w/2, -h/2, w, h);
        }
    }
}

void tst_QPainter::drawClippedEllipse()
{
    QFETCH(QRect, rect);
    if (sizeof(qreal) != sizeof(double))
        QSKIP("Test only works for qreal==double", SkipAll);
    QImage image(rect.width() + 1, rect.height() + 1,
                 QImage::Format_ARGB32_Premultiplied);
    QRect expected = QRect(rect.x(), rect.y(), rect.width()+1, rect.height()+1)
                     & QRect(0, 0, image.width(), image.height());


    image.fill(QColor(Qt::white).rgb());
    QPainter p(&image);
    p.drawEllipse(rect);
    p.end();

    QPixmap pixmap = QPixmap::fromImage(image);
    const QRect painted = getPaintedSize(pixmap, Qt::white);

    QCOMPARE(painted.x(), expected.x());
    QCOMPARE(painted.y(), expected.y());
    QCOMPARE(painted.width(), expected.width());
    QCOMPARE(painted.height(), expected.height());

}

void tst_QPainter::drawRoundRect()
{
    QFETCH(QRect, rect);
    QFETCH(bool, usePen);

#ifdef Q_WS_MAC
    if (QTest::currentDataTag() == QByteArray("rect(6, 12, 3, 14) with pen") ||
        QTest::currentDataTag() == QByteArray("rect(6, 17, 3, 25) with pen") ||
        QTest::currentDataTag() == QByteArray("rect(10, 6, 10, 3) with pen") ||
        QTest::currentDataTag() == QByteArray("rect(10, 12, 10, 14) with pen") ||
        QTest::currentDataTag() == QByteArray("rect(13, 45, 17, 80) with pen") ||
        QTest::currentDataTag() == QByteArray("rect(13, 50, 17, 91) with pen") ||
        QTest::currentDataTag() == QByteArray("rect(17, 6, 24, 3) with pen") ||
        QTest::currentDataTag() == QByteArray("rect(24, 12, 38, 14) with pen"))
        QSKIP("The Mac paint engine is off-by-one on certain rect sizes", SkipSingle);
#endif
    QPixmap pixmap(rect.x() + rect.width() + 10,
                   rect.y() + rect.height() + 10);
    {
        pixmap.fill(Qt::white);
        QPainter p(&pixmap);
        p.setPen(usePen ? QPen(Qt::black) : QPen(Qt::NoPen));
        p.setBrush(Qt::black);
        p.drawRoundRect(rect);
        p.end();

        int increment = usePen ? 1 : 0;

        const QRect painted = getPaintedSize(pixmap, Qt::white);
        QCOMPARE(painted.width(), rect.width() + increment);
        QCOMPARE(painted.height(), rect.height() + increment);
    }

#ifdef QT3_SUPPORT
    {
        pixmap.fill(Qt::white);
        Q3Painter p(&pixmap);
        p.setPen(usePen ? QPen(Qt::black) : QPen(Qt::NoPen));
        p.setBrush(Qt::black);
        p.drawRoundRect(rect);
        p.end();

        const QRect painted = getPaintedSize(pixmap, Qt::white);

        QCOMPARE(painted.width(), rect.width());
        QCOMPARE(painted.height(), rect.height());
    }
#endif
}

Q_DECLARE_METATYPE(QImage::Format)

void tst_QPainter::qimageFormats_data()
{
    QTest::addColumn<QImage::Format>("format");
    QTest::newRow("QImage::Format_RGB32") << QImage::Format_RGB32;
    QTest::newRow("QImage::Format_ARGB32") << QImage::Format_ARGB32;
    QTest::newRow("QImage::Format_ARGB32_Premultiplied") << QImage::Format_ARGB32_Premultiplied;
    QTest::newRow("QImage::Format_RGB16") << QImage::Format_RGB16;
    QTest::newRow("Qimage::Format_ARGB8565_Premultiplied") << QImage::Format_ARGB8565_Premultiplied;
    QTest::newRow("Qimage::Format_RGB666") << QImage::Format_RGB666;
    QTest::newRow("Qimage::Format_RGB555") << QImage::Format_RGB555;
    QTest::newRow("Qimage::Format_ARGB8555_Premultiplied") << QImage::Format_ARGB8555_Premultiplied;
    QTest::newRow("Qimage::Format_RGB888") << QImage::Format_RGB888;
}

/*
    Tests that QPainter can paint on various QImage formats.
*/
void tst_QPainter::qimageFormats()
{
    QFETCH(QImage::Format, format);

    const QSize size(100, 100);
    QImage image(size, format);
    image.fill(0);

    const QColor testColor(Qt::red);
    QPainter p(&image);
    QVERIFY(p.isActive());
    p.setBrush(QBrush(testColor));
    p.drawRect(QRect(QPoint(0,0), size));
    QCOMPARE(image.pixel(50, 50), testColor.rgb());
}

void tst_QPainter::fillData()
{
    QTest::addColumn<QRect>("rect");
    QTest::addColumn<bool>("usePen");

    for (int w = 3; w < 50; w += 7) {
        for (int h = 3; h < 50; h += 11) {
            int x = w/2 + 5;
            int y = h/2 + 5;
            QTest::newRow(QString("rect(%1, %2, %3, %4) with pen").arg(x).arg(y).arg(w).arg(h).toLatin1())
                << QRect(x, y, w, h) << true;
            QTest::newRow(QString("rect(%1, %2, %3, %4) no pen").arg(x).arg(y).arg(w).arg(h).toLatin1())
                << QRect(x, y, w, h) << false;
        }
    }
}

/*
    Test that drawline works properly after setWindow has been called.
*/
void tst_QPainter::setWindow()
{
    QPixmap pixmap(600, 600);
    pixmap.fill(QColor(Qt::white));

    QPainter painter(&pixmap);
    painter.setWindow(0, 0, 3, 3);
    painter.drawLine(1, 1, 2, 2);

    const QRect painted = getPaintedSize(pixmap, Qt::white);
    QVERIFY(195 < painted.y() && painted.y() < 205); // correct value is around 200
    QVERIFY(195 < painted.height() && painted.height() < 205); // correct value is around 200
}

void tst_QPainter::combinedMatrix()
{
    QPixmap pm(64, 64);

    QPainter p(&pm);
    p.setWindow(0, 0, 1, 1);
    p.setViewport(32, 0, 32, 32);

    p.translate(0.5, 0.5);

    QMatrix cm = p.combinedMatrix();

    QPointF pt = QPointF(0, 0) * cm;

    QCOMPARE(pt.x(), 48.0);
    QCOMPARE(pt.y(), 16.0);
}

void tst_QPainter::textOnTransparentImage()
{
    bool foundPixel = false;
    QImage image(10, 10, QImage::Format_ARGB32_Premultiplied);
    image.fill(qRgba(0, 0, 0, 0)); // transparent
    {
        QPainter painter(&image);
        painter.setPen(QColor(255, 255, 255));
        painter.drawText(0, 10, "W");
    }
    for (int x = 0; x < image.width(); ++x)
        for (int y = 0; y < image.height(); ++y)
            if (image.pixel(x, y) != 0)
                foundPixel = true;
    QVERIFY(foundPixel);
}

void tst_QPainter::renderHints()
{
    QImage img(1, 1, QImage::Format_RGB32);

    QPainter p(&img);

    // Turn off all...
    p.setRenderHints(QPainter::RenderHints(0xffffffff), false);
    QCOMPARE(p.renderHints(), QPainter::RenderHints(0));

    // Single set/get
    p.setRenderHint(QPainter::Antialiasing);
    QVERIFY(p.renderHints() & QPainter::Antialiasing);

    p.setRenderHint(QPainter::Antialiasing, false);
    QVERIFY(!(p.renderHints() & QPainter::Antialiasing));

    // Multi set/get
    p.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    QVERIFY(p.renderHints() & (QPainter::Antialiasing | QPainter::SmoothPixmapTransform));

    p.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform, false);
    QVERIFY(!(p.renderHints() & (QPainter::Antialiasing | QPainter::SmoothPixmapTransform)));
}

int countPixels(const QImage &img, const QRgb &color)
{
    int count = 0;
    for (int y = 0; y < img.height(); ++y) {
        for (int x = 0; x < img.width(); ++x) {
            count += ((img.pixel(x, y) & 0xffffff) == color);
        }
    }
    return count;
}

template <typename T>
void testClipping(QImage &img)
{
    img.fill(0x0);
    QPainterPath a, b;
    a.addRect(QRect(2, 2, 4, 4));
    b.addRect(QRect(4, 4, 4, 4));

    QPainter p(&img);
    p.setClipPath(a);
    p.setClipPath(b, Qt::UniteClip);

    p.setClipping(false);
    p.setPen(Qt::NoPen);
    p.setBrush(QColor(0xff0000));
    p.drawRect(T(0, 0, 10, 10));

    p.setClipping(true);
    p.setBrush(QColor(0x00ff00));
    p.drawRect(T(0, 0, 10, 10));

    QCOMPARE(countPixels(img, 0xff0000), 72);
    QCOMPARE(countPixels(img, 0x00ff00), 28);

    p.end();
    img.fill(0x0);
    p.begin(&img);
    p.setClipPath(a);
    p.setClipPath(b, Qt::IntersectClip);

    p.setClipping(false);
    p.setPen(Qt::NoPen);
    p.setBrush(QColor(0xff0000));
    p.drawRect(T(0, 0, 10, 10));

    p.setClipping(true);
    p.setBrush(QColor(0x00ff00));
    p.drawRect(T(0, 0, 10, 10));

    QCOMPARE(countPixels(img, 0xff0000), 96);
    QCOMPARE(countPixels(img, 0x00ff00), 4);
}

void tst_QPainter::disableEnableClipping()
{
    QImage img(10, 10, QImage::Format_RGB32);

    testClipping<QRectF>(img);
    testClipping<QRect>(img);
}

void tst_QPainter::setClipRect()
{
    QImage img(10, 10, QImage::Format_RGB32);
    // simple test to let valgrind check for buffer overflow
    {
        QPainter p(&img);
        p.setClipRect(-10, -10, 100, 100);
        p.fillRect(-10, -10, 100, 100, QBrush(QColor(Qt::red)));
    }

    // rects with negative width/height
    {
        QPainter p(&img);
        p.setClipRect(QRect(10, 10, -10, 10));
        QVERIFY(p.clipRegion().isEmpty());
        p.setClipRect(QRect(10, 10, 10, -10));
        QVERIFY(p.clipRegion().isEmpty());
        p.setClipRect(QRectF(10.5, 10.5, -10.5, 10.5));
        QVERIFY(p.clipRegion().isEmpty());
        p.setClipRect(QRectF(10.5, 10.5, 10.5, -10.5));
        QVERIFY(p.clipRegion().isEmpty());
    }
}

/*
    This tests the two different clipping approaches in QRasterPaintEngine,
    one when using a QRegion and one when using a QPainterPath. They should
    give equal results.
*/
void tst_QPainter::setEqualClipRegionAndPath_data()
{
    QTest::addColumn<QSize>("deviceSize");
    QTest::addColumn<QRegion>("region");

    QTest::newRow("empty") << QSize(100, 100) << QRegion();
    QTest::newRow("simple rect") << QSize(100, 100)
                                 << QRegion(QRect(5, 5, 10, 10));

    QVector<QRect> rects;
    QRegion region;

    rects << QRect(5, 5, 10, 10) << QRect(20, 20, 10, 10);
    region.setRects(rects.constData(), rects.size());
    QTest::newRow("two rects") << QSize(100, 100) << region;

    rects.clear();
    rects << QRect(5, 5, 10, 10) << QRect(20, 5, 10, 10);
    region.setRects(rects.constData(), rects.size());
    QTest::newRow("two x-adjacent rects") << QSize(100, 100) << region;

    rects.clear();
    rects << QRect(0, 0, 10, 100) << QRect(12, 0, 10, 100);
    region.setRects(rects.constData(), rects.size());
    QTest::newRow("two x-adjacent rects 2") << QSize(100, 100) << region;

    rects.clear();
    rects << QRect(0, 0, 10, 100) << QRect(12, 0, 10, 100);
    region.setRects(rects.constData(), rects.size());
    QTest::newRow("two x-adjacent rects 3") << QSize(50, 50) << region;

    rects.clear();
    rects << QRect(0, 0, 10, 100) << QRect(12, 0, 10, 100);
    region.setRects(rects.constData(), rects.size());
    QTest::newRow("two x-adjacent rects 4") << QSize(101, 101) << region;

    region = QRegion(QRect(0, 0, 200, 200), QRegion::Ellipse);

    QTest::newRow("ellipse") << QSize(190, 200) << region;

    region ^= QRect(50, 50, 50, 50);
    QTest::newRow("ellipse 2") << QSize(200, 200) << region;
}

void tst_QPainter::setEqualClipRegionAndPath()
{
    QFETCH(QSize, deviceSize);
    QFETCH(QRegion, region);

    QPainterPath path;
    path.addRegion(region);

    QImage img1(deviceSize.width(), deviceSize.height(),
                QImage::Format_ARGB32);
    QImage img2(deviceSize.width(), deviceSize.height(),
                QImage::Format_ARGB32);
    img1.fill(0x12345678);
    img2.fill(0x12345678);

    {
        QPainter p(&img1);
        p.setClipRegion(region);
        p.fillRect(0, 0, img1.width(), img1.height(), QColor(Qt::red));
    }
    {
        QPainter p(&img2);
        p.setClipPath(path);
        p.fillRect(0, 0, img2.width(), img2.height(), QColor(Qt::red));
    }

#if 0
    if (img1 != img2) {
        img1.save("setEqualClipRegionAndPath_1.xpm", "XPM");
        img2.save("setEqualClipRegionAndPath_2.xpm", "XPM");
    }
#endif
    QCOMPARE(img1, img2);

#if 0
    // rotated
    img1.fill(0x12345678);
    img2.fill(0x12345678);

    {
        QPainter p(&img1);
        p.rotate(25);
        p.setClipRegion(region);
        p.fillRect(0, 0, img1.width(), img1.height(), QColor(Qt::red));
    }
    {
        QPainter p(&img2);
        p.rotate(25);
        p.setClipPath(path);
        p.fillRect(0, 0, img2.width(), img2.height(), QColor(Qt::red));
    }

#if 1
    if (img1 != img2) {
        img1.save("setEqualClipRegionAndPath_1.xpm", "XPM");
        img2.save("setEqualClipRegionAndPath_2.xpm", "XPM");
    }
#endif
    QCOMPARE(img1, img2);
#endif

    // simple uniteclip
    img1.fill(0x12345678);
    img2.fill(0x12345678);
    {
        QPainter p(&img1);
        p.setClipRegion(region);
        p.setClipRegion(region, Qt::UniteClip);
        p.fillRect(0, 0, img1.width(), img1.height(), QColor(Qt::red));
    }
    {
        QPainter p(&img2);
        p.setClipPath(path);
        p.setClipPath(path, Qt::UniteClip);
        p.fillRect(0, 0, img2.width(), img2.height(), QColor(Qt::red));
    }
    QCOMPARE(img1, img2);
    img1.fill(0x12345678);
    img2.fill(0x12345678);
    {
        QPainter p(&img1);
        p.setClipPath(path);
        p.setClipRegion(region, Qt::UniteClip);
        p.fillRect(0, 0, img1.width(), img1.height(), QColor(Qt::red));
    }
    {
        QPainter p(&img2);
        p.setClipRegion(region);
        p.setClipPath(path, Qt::UniteClip);
        p.fillRect(0, 0, img2.width(), img2.height(), QColor(Qt::red));
    }
#if 0
    if (img1 != img2) {
        img1.save("setEqualClipRegionAndPath_1.xpm", "XPM");
        img2.save("setEqualClipRegionAndPath_2.xpm", "XPM");
    }
#endif
    QCOMPARE(img1, img2);

    // simple intersectclip
    img1.fill(0x12345678);
    img2.fill(0x12345678);
    {
        QPainter p(&img1);
        p.setClipRegion(region);
        p.setClipRegion(region, Qt::IntersectClip);
        p.fillRect(0, 0, img1.width(), img1.height(), QColor(Qt::red));
    }
    {
        QPainter p(&img2);
        p.setClipPath(path);
        p.setClipPath(path, Qt::IntersectClip);
        p.fillRect(0, 0, img2.width(), img2.height(), QColor(Qt::red));
    }
#if 0
    if (img1 != img2) {
        img1.save("setEqualClipRegionAndPath_1.png", "PNG");
        img2.save("setEqualClipRegionAndPath_2.png", "PNG");
    }
#endif
    QCOMPARE(img1, img2);

    img1.fill(0x12345678);
    img2.fill(0x12345678);
    {
        QPainter p(&img1);
        p.setClipPath(path);
        p.setClipRegion(region, Qt::IntersectClip);
        p.fillRect(0, 0, img1.width(), img1.height(), QColor(Qt::red));
    }
    {
        QPainter p(&img2);
        p.setClipRegion(region);
        p.setClipPath(path, Qt::IntersectClip);
        p.fillRect(0, 0, img2.width(), img2.height(), QColor(Qt::red));
    }
#if 0
    if (img1 != img2) {
        img1.save("setEqualClipRegionAndPath_1.xpm", "XPM");
        img2.save("setEqualClipRegionAndPath_2.xpm", "XPM");
    }
#endif
    QCOMPARE(img1, img2);

}

void tst_QPainter::clippedFillPath_data()
{
    QTest::addColumn<QSize>("imageSize");
    QTest::addColumn<QPainterPath>("path");
    QTest::addColumn<QRect>("clipRect");
    QTest::addColumn<QBrush>("brush");
    QTest::addColumn<QPen>("pen");

    QLinearGradient gradient(QPoint(0, 0), QPoint(100, 100));
    gradient.setColorAt(0, Qt::red);
    gradient.setColorAt(1, Qt::blue);


    QPen pen2(QColor(223, 223, 0, 223));
    pen2.setWidth(2);

    QPainterPath path;
    path.addRect(QRect(15, 15, 50, 50));
    QTest::newRow("simple rect 0") << QSize(100, 100) << path
                                   << QRect(15, 15, 49, 49)
                                   << QBrush(Qt::NoBrush)
                                   << QPen(Qt::black);
    QTest::newRow("simple rect 1") << QSize(100, 100) << path
                                   << QRect(15, 15, 50, 50)
                                   << QBrush(Qt::NoBrush)
                                   << QPen(Qt::black);
    QTest::newRow("simple rect 2") << QSize(100, 100) << path
                                   << QRect(15, 15, 51, 51)
                                   << QBrush(Qt::NoBrush)
                                   << QPen(Qt::black);
    QTest::newRow("simple rect 3") << QSize(100, 100) << path
                                   << QRect(15, 15, 51, 51)
                                   << QBrush(QColor(Qt::blue))
                                   << QPen(Qt::NoPen);
    QTest::newRow("simple rect 4") << QSize(100, 100) << path
                                   << QRect(15, 15, 51, 51)
                                   << QBrush(gradient)
                                   << pen2;

    path = QPainterPath();
    path.addEllipse(QRect(15, 15, 50, 50));
    QTest::newRow("ellipse 0") << QSize(100, 100) << path
                               << QRect(15, 15, 49, 49)
                               << QBrush(Qt::NoBrush)
                               << QPen(Qt::black);
    QTest::newRow("ellipse 1") << QSize(100, 100) << path
                               << QRect(15, 15, 50, 50)
                               << QBrush(Qt::NoBrush)
                               << QPen(Qt::black);
    QTest::newRow("ellipse 2") << QSize(100, 100) << path
                               << QRect(15, 15, 51, 51)
                               << QBrush(Qt::NoBrush)
                               << QPen(Qt::black);
    QTest::newRow("ellipse 3") << QSize(100, 100) << path
                               << QRect(15, 15, 51, 51)
                               << QBrush(QColor(Qt::blue))
                               << QPen(Qt::NoPen);
    QTest::newRow("ellipse 4") << QSize(100, 100) << path
                               << QRect(15, 15, 51, 51)
                               << QBrush(gradient)
                               << pen2;

    path = QPainterPath();
    path.addRoundRect(QRect(15, 15, 50, 50), 20);
    QTest::newRow("round rect 0") << QSize(100, 100) << path
                                  << QRect(15, 15, 49, 49)
                                  << QBrush(Qt::NoBrush)
                                  << QPen(Qt::black);
    QTest::newRow("round rect 1") << QSize(100, 100) << path
                                  << QRect(15, 15, 50, 50)
                                  << QBrush(Qt::NoBrush)
                                  << QPen(Qt::black);
    QTest::newRow("round rect 2") << QSize(100, 100) << path
                                  << QRect(15, 15, 51, 51)
                                  << QBrush(Qt::NoBrush)
                                  << QPen(Qt::black);
    QTest::newRow("round rect 3") << QSize(100, 100) << path
                                  << QRect(15, 15, 51, 51)
                                  << QBrush(QColor(Qt::blue))
                                  << QPen(Qt::NoPen);
    QTest::newRow("round rect 4") << QSize(100, 100) << path
                                  << QRect(15, 15, 51, 51)
                                  << QBrush(gradient)
                                  << pen2;

    path = QPainterPath();
    path.moveTo(15, 50);
    path.cubicTo(40, 50, 40, 15, 65, 50);
    path.lineTo(15, 50);
    QTest::newRow("cubic 0") << QSize(100, 100) << path
                             << QRect(15, 15, 49, 49)
                             << QBrush(Qt::NoBrush)
                             << QPen(Qt::black);
    QTest::newRow("cubic 1") << QSize(100, 100) << path
                             << QRect(15, 15, 50, 50)
                             << QBrush(Qt::NoBrush)
                             << QPen(Qt::black);
    QTest::newRow("cubic 2") << QSize(100, 100) << path
                             << QRect(15, 15, 51, 51)
                             << QBrush(Qt::NoBrush)
                             << QPen(Qt::black);
    QTest::newRow("cubic 3") << QSize(100, 100) << path
                             << QRect(15, 15, 51, 51)
                             << QBrush(QColor(Qt::blue))
                             << QPen(Qt::NoPen);
    QTest::newRow("cubic 4") << QSize(100, 100) << path
                             << QRect(15, 15, 51, 51)
                             << QBrush(gradient)
                             << pen2;
}

void tst_QPainter::clippedFillPath()
{
    QFETCH(QSize, imageSize);
    QFETCH(QPainterPath, path);
    QFETCH(QRect, clipRect);
    QPainterPath clipPath;
    clipPath.addRect(clipRect);
    QFETCH(QBrush, brush);
    QFETCH(QPen, pen);

    const int width = imageSize.width();
    const int height = imageSize.height();

    QImage clippedRect(width, height, QImage::Format_ARGB32_Premultiplied);
    clippedRect.fill(0x12345678);
    {
        QPainter painter(&clippedRect);
        painter.setPen(pen);
        painter.setBrush(brush);
        painter.setClipRect(clipRect);
        painter.drawPath(path);
    }

    QImage clippedPath(width, height, QImage::Format_ARGB32_Premultiplied);
    clippedPath.fill(0x12345678);
    {
        QPainter painter(&clippedPath);
        painter.setPen(pen);
        painter.setBrush(brush);
        painter.setClipPath(clipPath);
        painter.drawPath(path);
    }

#if 0
    if (clippedRect != clippedPath) {
        clippedRect.save(QString("clippedRect.png"), "PNG");
        clippedPath.save(QString("clippedPath.png"), "PNG");
    }
#endif
    QCOMPARE(clippedRect, clippedPath);

    // repeat with antialiasing

    clippedRect.fill(0x12345678);
    {
        QPainter painter(&clippedRect);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setPen(pen);
        painter.setBrush(brush);
        painter.setClipRect(clipRect);
        painter.drawPath(path);
    }

    clippedPath.fill(0x12345678);
    {
        QPainter painter(&clippedPath);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setPen(pen);
        painter.setBrush(brush);
        painter.setClipPath(clipPath);
        painter.drawPath(path);
    }

#if 1
    if (clippedRect != clippedPath) {
        clippedRect.save(QString("clippedRect.png"), "PNG");
        clippedPath.save(QString("clippedPath.png"), "PNG");
    }
#endif
    QCOMPARE(clippedRect, clippedPath);

}

void tst_QPainter::clippedLines_data()
{
    QTest::addColumn<QSize>("imageSize");
    QTest::addColumn<QLineF>("line");
    QTest::addColumn<QRect>("clipRect");
    QTest::addColumn<QPen>("pen");

    QPen pen2(QColor(223, 223, 0, 223));
    pen2.setWidth(2);

    QVector<QLineF> lines;
    lines << QLineF(15, 15, 65, 65)
          << QLineF(14, 14, 66, 66)
          << QLineF(16, 16, 64, 64)
          << QLineF(65, 65, 15, 15)
          << QLineF(66, 66, 14, 14)
          << QLineF(64, 64, 14, 14)
          << QLineF(15, 50, 15, 64)
          << QLineF(15, 50, 15, 65)
          << QLineF(15, 50, 15, 66)
          << QLineF(15, 50, 64, 50)
          << QLineF(15, 50, 65, 50)
          << QLineF(15, 50, 66, 50);

    foreach (QLineF line, lines) {
        QString desc = QString("line (%1, %2, %3, %4) %5").arg(line.x1())
                       .arg(line.y1()).arg(line.x2()).arg(line.y2());
        QTest::newRow(qPrintable(desc.arg(0))) << QSize(100, 100) << line
                                   << QRect(15, 15, 49, 49)
                                   << QPen(Qt::black);
        QTest::newRow(qPrintable(desc.arg(1))) << QSize(100, 100) << line
                                   << QRect(15, 15, 50, 50)
                                   << QPen(Qt::black);
        QTest::newRow(qPrintable(desc.arg(2))) << QSize(100, 100) << line
                                   << QRect(15, 15, 51, 51)
                                   << QPen(Qt::black);
        QTest::newRow(qPrintable(desc.arg(3))) << QSize(100, 100) << line
                                   << QRect(15, 15, 51, 51)
                                   << QPen(Qt::NoPen);
        QTest::newRow(qPrintable(desc.arg(4))) << QSize(100, 100) << line
                                   << QRect(15, 15, 51, 51)
                                   << pen2;
    }
}

void tst_QPainter::clippedLines()
{
    QFETCH(QSize, imageSize);
    QFETCH(QLineF, line);
    QFETCH(QRect, clipRect);
    QPainterPath clipPath;
    clipPath.addRect(clipRect);
    QFETCH(QPen, pen);

    const int width = imageSize.width();
    const int height = imageSize.height();

    QImage clippedRect(width, height, QImage::Format_ARGB32_Premultiplied);
    clippedRect.fill(0x12345678);
    {
        QPainter painter(&clippedRect);
        painter.setPen(pen);
        painter.setClipRect(clipRect);
        painter.drawLine(line);
        painter.drawLine(line.toLine());
    }

    QImage clippedPath(width, height, QImage::Format_ARGB32_Premultiplied);
    clippedPath.fill(0x12345678);
    {
        QPainter painter(&clippedPath);
        painter.setPen(pen);
        painter.setClipPath(clipPath);
        painter.drawLine(line);
        painter.drawLine(line.toLine());
    }

#if 0
    if (clippedRect != clippedPath) {
        clippedRect.save(QString("clippedRect.png"), "PNG");
        clippedPath.save(QString("clippedPath.png"), "PNG");
    }
#endif
    QCOMPARE(clippedRect, clippedPath);

    // repeat with antialiasing
    clippedRect.fill(0x12345678);
    {
        QPainter painter(&clippedRect);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setPen(pen);
        painter.setClipRect(clipRect);
        painter.drawLine(line);
        painter.drawLine(line.toLine());
    }

    clippedPath.fill(0x12345678);
    {
        QPainter painter(&clippedPath);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setPen(pen);
        painter.setClipPath(clipPath);
        painter.drawLine(line);
        painter.drawLine(line.toLine());
    }

#if 0
    if (clippedRect != clippedPath) {
        clippedRect.save(QString("clippedRect.png"), "PNG");
        clippedPath.save(QString("clippedPath.png"), "PNG");
    }
#endif
    QCOMPARE(clippedRect, clippedPath);
}

void tst_QPainter::clippedPolygon_data()
{
    clippedFillPath_data();
};

void tst_QPainter::clippedPolygon()
{
    QFETCH(QSize, imageSize);
    QFETCH(QPainterPath, path);
    QPolygonF polygon = path.toFillPolygon();
    QFETCH(QRect, clipRect);
    QPainterPath clipPath;
    clipPath.addRect(clipRect);
    QFETCH(QPen, pen);
    QFETCH(QBrush, brush);

    const int width = imageSize.width();
    const int height = imageSize.height();

    QImage clippedRect(width, height, QImage::Format_ARGB32_Premultiplied);
    clippedRect.fill(0x12345678);
    {
        QPainter painter(&clippedRect);
        painter.setPen(pen);
        painter.setBrush(brush);
        painter.setClipRect(clipRect);
        painter.drawPolygon(polygon);
        painter.drawPolygon(polygon.toPolygon());
    }

    QImage clippedPath(width, height, QImage::Format_ARGB32_Premultiplied);
    clippedPath.fill(0x12345678);
    {
        QPainter painter(&clippedPath);
        painter.setPen(pen);
        painter.setBrush(brush);
        painter.setClipRect(clipRect);
        painter.drawPolygon(polygon);
        painter.drawPolygon(polygon.toPolygon());
    }

#if 0
    if (clippedRect != clippedPath) {
        clippedRect.save(QString("clippedRect.png"), "PNG");
        clippedPath.save(QString("clippedPath.png"), "PNG");
    }
#endif
    QCOMPARE(clippedRect, clippedPath);

    // repeat with antialiasing

    clippedRect.fill(0x12345678);
    {
        QPainter painter(&clippedRect);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setPen(pen);
        painter.setBrush(brush);
        painter.setClipRect(clipRect);
        painter.drawPolygon(polygon);
        painter.drawPolygon(polygon.toPolygon());
    }

    clippedPath.fill(0x12345678);
    {
        QPainter painter(&clippedPath);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setPen(pen);
        painter.setBrush(brush);
        painter.setClipRect(clipRect);
        painter.drawPolygon(polygon);
        painter.drawPolygon(polygon.toPolygon());
    }

#if 0
    if (clippedRect != clippedPath) {
        clippedRect.save(QString("clippedRect.png"), "PNG");
        clippedPath.save(QString("clippedPath.png"), "PNG");
    }
#endif
    QCOMPARE(clippedRect, clippedPath);
}

// this just draws some text that should be clipped in the raster
// paint engine.
void tst_QPainter::clippedText()
{
    for (char ch = 'A'; ch < 'Z'; ++ch) {
        //qDebug() << ch;
        QFont f;
        f.setPixelSize(24);
        QFontMetrics metrics(f);
        QRect textRect = metrics.boundingRect(QChar(ch));

        if (textRect.width() <= 8)
            continue;
        if (textRect.height() <= 8)
            continue;

        QRect imageRect = textRect.adjusted(4, 4, -4, -4);

        QImage image(imageRect.size(), QImage::Format_ARGB32_Premultiplied);

        image.fill(qRgba(255, 255, 255, 255));
        {
            QPainter painter(&image);
            painter.setFont(f);
            painter.setPen(Qt::black);

            painter.drawText(0, 0, QChar(ch));
        }

        image.fill(qRgba(255, 255, 255, 255));
        {
            QPainter painter(&image);
            painter.setFont(f);
            painter.setPen(Qt::black);

            painter.drawText(-imageRect.topLeft(), QChar(ch));
        }

        bool foundPixel = false;
        for (int x = 0; x < image.width(); ++x)
            for (int y = 0; y < image.height(); ++y)
                if (image.pixel(x, y) != 0)
                    foundPixel = true;
        // can't QVERIFY(foundPixel) as sometimes all pixels are clipped
        // away. For example for 'O'
        // just call /some/ function to prevent the compiler from optimizing
        // foundPixel away
        QString::number(foundPixel);

        //image.save(QString("debug") + ch + ".xpm");
    }

    QVERIFY(true); // reached, don't trigger any valgrind errors
}

void tst_QPainter::setOpacity_data()
{
    QTest::addColumn<QImage::Format>("destFormat");
    QTest::addColumn<QImage::Format>("srcFormat");

    QTest::newRow("ARGB32P on ARGB32P") << QImage::Format_ARGB32_Premultiplied
                                        << QImage::Format_ARGB32_Premultiplied;

    QTest::newRow("ARGB32 on ARGB32") << QImage::Format_ARGB32
                                      << QImage::Format_ARGB32;

    QTest::newRow("RGB32 on RGB32") << QImage::Format_RGB32
                                    << QImage::Format_RGB32;

    QTest::newRow("RGB16 on RGB16") << QImage::Format_RGB16
                                    << QImage::Format_RGB16;

    QTest::newRow("ARGB8565_Premultiplied on ARGB8565_Premultiplied") << QImage::Format_ARGB8565_Premultiplied
                                                                      << QImage::Format_ARGB8565_Premultiplied;

    QTest::newRow("RGB555 on RGB555") << QImage::Format_RGB555
                                      << QImage::Format_RGB555;

    QTest::newRow("RGB666 on RGB666") << QImage::Format_RGB666
                                      << QImage::Format_RGB666;

    QTest::newRow("ARGB8555_Premultiplied on ARGB8555_Premultiplied") << QImage::Format_ARGB8555_Premultiplied
                                                                      << QImage::Format_ARGB8555_Premultiplied;

    QTest::newRow("RGB888 on RGB888") << QImage::Format_RGB888
                                      << QImage::Format_RGB888;

    QTest::newRow("RGB32 on RGB16") << QImage::Format_RGB16
                                    << QImage::Format_RGB32;

    QTest::newRow("RGB32 on ARGB8565_Premultiplied") << QImage::Format_ARGB8565_Premultiplied
                                                     << QImage::Format_RGB32;

    QTest::newRow("RGB32 on RGB666") << QImage::Format_RGB666
                                     << QImage::Format_RGB32;

    QTest::newRow("RGB32 on RGB555") << QImage::Format_RGB555
                                     << QImage::Format_RGB32;

    QTest::newRow("RGB32 on ARGB8555_Premultiplied") << QImage::Format_ARGB8555_Premultiplied
                                                     << QImage::Format_RGB32;

    QTest::newRow("RGB32 on RGB888") << QImage::Format_RGB888
                                     << QImage::Format_RGB32;

    QTest::newRow("RGB16 on RGB32") << QImage::Format_RGB32
                                    << QImage::Format_RGB16;

    QTest::newRow("ARGB8565_Premultiplied on RGB32") << QImage::Format_RGB32
                                                     << QImage::Format_ARGB8565_Premultiplied;

    QTest::newRow("RGB666 on RGB32") << QImage::Format_RGB32
                                     << QImage::Format_RGB666;

    QTest::newRow("RGB555 on RGB32") << QImage::Format_RGB32
                                     << QImage::Format_RGB555;

    QTest::newRow("ARGB8555_Premultiplied on RGB32") << QImage::Format_RGB32
                                                     << QImage::Format_ARGB8555_Premultiplied;

    QTest::newRow("RGB888 on RGB32") << QImage::Format_RGB32
                                     << QImage::Format_RGB888;

    QTest::newRow("RGB555 on RGB888") << QImage::Format_RGB888
                                      << QImage::Format_RGB555;

    QTest::newRow("RGB666 on RGB888") << QImage::Format_RGB888
                                      << QImage::Format_RGB666;

    QTest::newRow("RGB444 on RGB444") << QImage::Format_RGB444
                                      << QImage::Format_RGB444;
}

void tst_QPainter::setOpacity()
{
    QFETCH(QImage::Format, destFormat);
    QFETCH(QImage::Format, srcFormat);

    const QSize imageSize(12, 12);
    const QRect imageRect(QPoint(0, 0), imageSize);
    QColor destColor = Qt::black;
    QColor srcColor = Qt::white;

    QImage dest(imageSize, destFormat);
    QImage src(imageSize, srcFormat);

    QPainter p;
    p.begin(&dest);
    p.fillRect(imageRect, destColor);
    p.end();

    p.begin(&src);
    p.fillRect(imageRect, srcColor);
    p.end();

    p.begin(&dest);
    p.setOpacity(0.5);
    p.drawImage(imageRect, src, imageRect);
    p.end();

    QImage actual = dest.convertToFormat(QImage::Format_RGB32);

    for (int y = 0; y < actual.height(); ++y) {
        QRgb *p = (QRgb *)actual.scanLine(y);
        for (int x = 0; x < actual.width(); ++x) {
            QVERIFY(qAbs(qRed(p[x]) - 127) <= 0xf);
            QVERIFY(qAbs(qGreen(p[x]) - 127) <= 0xf);
            QVERIFY(qAbs(qBlue(p[x]) - 127) <= 0xf);
        }
    }
}

void tst_QPainter::drawhelper_blend_untransformed_data()
{
    setOpacity_data();
}

void tst_QPainter::drawhelper_blend_untransformed()
{
    QFETCH(QImage::Format, destFormat);
    QFETCH(QImage::Format, srcFormat);

    const int size = 128;
    const QSize imageSize(size, size);
    const QRect paintRect(1, 0, size - 2, size); // needs alignment and tailing

    QColor destColor(127, 127, 127);
    QColor srcColor(Qt::white);

    QImage dest(imageSize, destFormat);
    QImage src(imageSize, srcFormat);

    QPainter p;
    p.begin(&src);
    p.fillRect(paintRect, srcColor);
    p.end();

    QList<qreal> opacities = (QList<qreal>() << 0.0 << 0.1  << 0.01 << 0.4
                              << 0.5 << 0.6 << 0.9 << 1.0);
    foreach (qreal opacity, opacities) {
        p.begin(&dest);
        p.fillRect(paintRect, destColor);

        p.setOpacity(opacity);
        p.drawImage(paintRect, src, paintRect);
        p.end();

        // sanity check: make sure all pixels are equal
        QImage expected(size - 2, size, destFormat);
        p.begin(&expected);
        p.fillRect(0, 0, expected.width(), expected.height(),
                   QColor(dest.pixel(1, 0)));
        p.end();

        const QImage subDest(dest.bits() + dest.depth() / 8,
                             dest.width() - 2, dest.height(),
                             dest.bytesPerLine(), dest.format());

        if (dest.format() == QImage::Format_ARGB8565_Premultiplied ||
            dest.format() == QImage::Format_ARGB8555_Premultiplied) {
            // Test skipped due to rounding errors...
            continue;
        }
#if 0
        if (subDest != expected) {
            qDebug() << "size" << size << "opacity" << opacity;
            for (int j = 0; j < expected.height(); ++j) {
                for (int i = 0; i < expected.width(); ++i) {
                    if (expected.pixel(i,j) != subDest.pixel(i,j))
                        qDebug() << i << j << hex << expected.pixel(i, j)
                                 << subDest.pixel(i, j);
                }
            }
        }
#endif
        QCOMPARE(subDest, expected);
    }
}

void tst_QPainter::drawhelper_blend_tiled_untransformed_data()
{
    setOpacity_data();
}

void tst_QPainter::drawhelper_blend_tiled_untransformed()
{
    QFETCH(QImage::Format, destFormat);
    QFETCH(QImage::Format, srcFormat);

    const int size = 128;
    const QSize imageSize(size, size);
    const QRect paintRect(1, 0, size - 2, size); // needs alignment and tailing

    QColor destColor(127, 127, 127);
    QColor srcColor(Qt::white);

    QImage dest(imageSize, destFormat);
    QImage src(imageSize / 2, srcFormat);

    QPainter p;
    p.begin(&src);
    p.fillRect(QRect(QPoint(0, 0), imageSize/ 2), srcColor);
    p.end();

    const QBrush brush(src);

    QList<qreal> opacities = (QList<qreal>() << 0.0 << 0.1  << 0.01 << 0.4
                              << 0.5 << 0.6 << 0.9 << 1.0);
    foreach (qreal opacity, opacities) {
        p.begin(&dest);
        p.fillRect(paintRect, destColor);

        p.setOpacity(opacity);
        p.fillRect(paintRect, brush);
        p.end();

        // sanity check: make sure all pixels are equal
        QImage expected(size - 2, size, destFormat);
        p.begin(&expected);
        p.fillRect(0, 0, expected.width(), expected.height(),
                   QColor(dest.pixel(1, 0)));
        p.end();

        const QImage subDest(dest.bits() + dest.depth() / 8,
                             dest.width() - 2, dest.height(),
                             dest.bytesPerLine(), dest.format());

        if (dest.format() == QImage::Format_ARGB8565_Premultiplied ||
            dest.format() == QImage::Format_ARGB8555_Premultiplied) {
            // Skipping test due to rounding errors. Test needs rewrite
            continue;
        }
#if 0
        if (subDest != expected) {
            qDebug() << "size" << size << "opacity" << opacity;
            for (int j = 0; j < expected.height(); ++j) {
                for (int i = 0; i < expected.width(); ++i) {
                    if (expected.pixel(i,j) != subDest.pixel(i,j))
                        qDebug() << i << j << hex << expected.pixel(i, j)
                                 << subDest.pixel(i, j);
                }
            }
        }
#endif
        QCOMPARE(subDest, expected);
    }
}

static QPaintEngine::PaintEngineFeatures no_porter_duff()
{
    QPaintEngine::PaintEngineFeatures features = QPaintEngine::AllFeatures;
    return features & ~QPaintEngine::PorterDuff;
}

class DummyPaintEngine : public QPaintEngine, public QPaintDevice
{
public:
    DummyPaintEngine() : QPaintEngine(no_porter_duff()) {}
    virtual bool begin(QPaintDevice *) { return true; }
    virtual bool end() { return true; }

    virtual void updateState(const QPaintEngineState &) {}
    virtual void drawPixmap(const QRectF &, const QPixmap &, const QRectF &) {}

    virtual Type type() const { return User; }

    virtual QPaintEngine *paintEngine() const { return (QPaintEngine *)this; }

    virtual int metric(PaintDeviceMetric metric) const { Q_UNUSED(metric); return 0; };
};

static bool success;

void porterDuff_warningChecker(QtMsgType type, const char *msg)
{
    if (type == QtWarningMsg && msg == QLatin1String("QPainter::setCompositionMode: PorterDuff modes not supported on device"))
        success = false;
}

void tst_QPainter::porterDuff_warning()
{
    QtMsgHandler old = qInstallMsgHandler(porterDuff_warningChecker);
    DummyPaintEngine dummy;
    QPainter p(&dummy);

    success = true;
    p.setCompositionMode(QPainter::CompositionMode_Source);
    QVERIFY(success);

    success = true;
    p.setCompositionMode(QPainter::CompositionMode_SourceOver);
    QVERIFY(success);

    success = true;
    p.setCompositionMode(QPainter::CompositionMode_DestinationOver);
    QVERIFY(!success);

    QVERIFY(qInstallMsgHandler(old) == porterDuff_warningChecker);
}

class quint24
{
public:
    inline quint24(quint32 v)
    {
        data[0] = qBlue(v);
        data[1] = qGreen(v);
        data[2] = qRed(v);
    }

    inline operator quint32 ()
    {
        return qRgb(data[2], data[1], data[0]);
    }

    inline bool operator==(const quint24 &v) const {
        return (data[0] == v.data[0] && data[1] == v.data[1] && data[2] == v.data[2]);
    }

    uchar data[3];
} Q_PACKED;

void tst_QPainter::drawhelper_blend_color()
{
    QImage dest(32, 32, QImage::Format_ARGB8555_Premultiplied);
    dest.fill(0xff000000);

    {
        QPainter p(&dest);
        p.fillRect(0, 0, dest.width(), dest.height(), QColor(255, 0, 0, 127));
    }

    QImage expected(32, 32, QImage::Format_ARGB8555_Premultiplied);
    expected.fill(0xff3c007f);

    QCOMPARE(dest.pixel(1, 1), expected.pixel(1, 1));
    QCOMPARE(dest, expected);
}

class ViewportTestWidget : public QWidget
{
public:
    ViewportTestWidget(QWidget *parent = 0) : QWidget(parent), hasPainted(false) {}
    QSize sizeHint() const {
        return QSize(100, 100);
    }

    QRect viewport;
    bool hasPainted;

protected:
    void paintEvent(QPaintEvent *) {
        hasPainted = true;
        QPainter p(this);
        viewport = p.viewport();
    }
};

void tst_QPainter::childWidgetViewport()
{
    QWidget parent;
    parent.setAutoFillBackground(true);
    parent.resize(200, 200);
    ViewportTestWidget child(&parent);
    child.setAutoFillBackground(true);
    parent.show();
    parent.update();
    qApp->processEvents();

    if (child.hasPainted) {
        QCOMPARE(child.viewport, QRect(QPoint(0, 0), child.sizeHint()));
    } else {
        qWarning("Failed to ensure that paintEvent has been run. Could not run test.");
    }
}

void tst_QPainter::fillRect_objectBoundingModeGradient()
{
    QImage a(10, 10, QImage::Format_ARGB32_Premultiplied);
    a.fill(0x0);
    QImage b = a;

    QLinearGradient g(QPoint(0, 0), QPoint(0, 1));
    g.setColorAt(0, Qt::red);
    g.setColorAt(1, Qt::blue);
    g.setCoordinateMode(QGradient::ObjectBoundingMode);

    QPainter p(&a);
    p.fillRect(QRect(0, 0, a.width(), a.height()), g);
    p.end();

    QPainterPath path;
    path.addRect(0, 0, a.width(), a.height());

    p.begin(&b);
    p.fillPath(path, g);
    p.end();

    QCOMPARE(a, b);
}

void tst_QPainter::fillRect_stretchToDeviceMode()
{
    QImage img(64, 64, QImage::Format_ARGB32_Premultiplied);

    QLinearGradient g(QPoint(0, 0), QPoint(0, 1));
    g.setCoordinateMode(QGradient::StretchToDeviceMode);

    QPainter p(&img);
    p.fillRect(img.rect(), g);
    p.end();

    for (int i = 1; i < img.height(); ++i)
        QVERIFY(img.pixel(0, i) != img.pixel(0, i-1));
}

void tst_QPainter::monoImages()
{
    Qt::GlobalColor colorPairs[][2] = {
        { Qt::white, Qt::black },
        { Qt::color0, Qt::color1 },
        { Qt::red, Qt::blue }
    };

    const int numColorPairs = sizeof(colorPairs) / sizeof(QRgb[2]);

    QImage transparent(2, 2, QImage::Format_ARGB32_Premultiplied);
    transparent.fill(0x0);

    for (int i = 1; i < QImage::NImageFormats; ++i) {
        for (int j = 0; j < numColorPairs; ++j) {
            const QImage::Format format = QImage::Format(i);
            if (format == QImage::Format_Indexed8)
                continue;

            QImage img(2, 2, format);

            if (img.colorCount() > 0) {
                img.setColor(0, QColor(colorPairs[j][0]).rgba());
                img.setColor(1, QColor(colorPairs[j][1]).rgba());
            }

            img.fill(0x0);
            QPainter p(&img);
            p.fillRect(0, 0, 2, 2, colorPairs[j][0]);
            p.fillRect(0, 0, 1, 1, colorPairs[j][1]);
            p.fillRect(1, 1, 1, 1, colorPairs[j][1]);
            p.end();

            QImage original = img;

            p.begin(&img);
            p.drawImage(0, 0, transparent);
            p.end();

            // drawing a transparent image on top of another image
            // should not change the image
            QCOMPARE(original, img);

            if (img.colorCount() == 0)
                continue;

            for (int k = 0; k < 2; ++k) {
                QPainter p(&img);
                p.fillRect(0, 0, 2, 2, colorPairs[j][k]);
                p.end();

                QImage argb32p(2, 2, QImage::Format_ARGB32_Premultiplied);
                p.begin(&argb32p);
                p.fillRect(0, 0, 2, 2, colorPairs[j][k]);
                p.end();

                QCOMPARE(argb32p, img.convertToFormat(argb32p.format()));

                // drawing argb32p image on mono image
                p.begin(&img);
                p.drawImage(0, 0, argb32p);
                p.end();

                QCOMPARE(argb32p, img.convertToFormat(argb32p.format()));

                // drawing mono image on argb32p image
                p.begin(&argb32p);
                p.drawImage(0, 0, img);
                p.end();

                QCOMPARE(argb32p, img.convertToFormat(argb32p.format()));
            }
        }
    }
}

#if !defined(Q_OS_IRIX) && !defined(Q_OS_AIX) && !defined(Q_CC_MSVC) && !defined(Q_OS_SOLARIS) && !defined(Q_OS_SYMBIAN)
#include <fenv.h>

static const QString fpeExceptionString(int exception)
{
#ifdef FE_INEXACT
    if (exception & FE_INEXACT)
        return QLatin1String("Inexact result");
#endif
    if (exception & FE_UNDERFLOW)
        return QLatin1String("Underflow");
    if (exception & FE_OVERFLOW)
        return QLatin1String("Overflow");
    if (exception & FE_DIVBYZERO)
        return QLatin1String("Divide by zero");
    if (exception & FE_INVALID)
        return QLatin1String("Invalid operation");
    return QLatin1String("No exception");
}

class FpExceptionChecker
{
public:
    FpExceptionChecker(int exceptionMask)
        : m_exceptionMask(exceptionMask)
    {
        feclearexcept(m_exceptionMask);
    }

    ~FpExceptionChecker()
    {
        const int exceptions = fetestexcept(m_exceptionMask);
        QVERIFY2(!exceptions, qPrintable(QLatin1String("Floating point exception: ") + fpeExceptionString(exceptions)));
    }

private:
    int m_exceptionMask;
};

void fpe_rasterizeLine_task232012()
{
    FpExceptionChecker checker(FE_UNDERFLOW | FE_OVERFLOW | FE_INVALID | FE_DIVBYZERO);
    QImage img(128, 128, QImage::Format_ARGB32_Premultiplied);
    img.fill(0x0);
    QPainter p(&img);

    p.setBrush(Qt::black);
    p.drawRect(QRectF(0, 0, 5, 0));
    p.drawRect(QRectF(0, 0, 0, 5));
}

void fpe_pixmapTransform()
{
    FpExceptionChecker checker(FE_UNDERFLOW | FE_OVERFLOW | FE_INVALID | FE_DIVBYZERO);

    QImage img(128, 128, QImage::Format_ARGB32_Premultiplied);

    QPainter p(&img);

    const qreal scaleFactor = 0.001;
    const int translateDistance = 1000000;

    p.setPen(Qt::red);
    p.setBrush(QBrush(Qt::red,Qt::Dense6Pattern));

    for (int i = 0; i < 2; ++i) {
        p.setRenderHint(QPainter::SmoothPixmapTransform, i);

        p.resetTransform();
        p.scale(1.1, 1.1);
        p.translate(translateDistance, 0);
        p.drawRect(-translateDistance, 0, 100, 100);

        p.resetTransform();
        p.scale(scaleFactor, scaleFactor);
        p.drawRect(QRectF(0, 0, 1 / scaleFactor, 1 / scaleFactor));
    }
}

void fpe_zeroLengthLines()
{
    FpExceptionChecker checker(FE_UNDERFLOW | FE_OVERFLOW | FE_INVALID | FE_DIVBYZERO);

    QImage img(128, 128, QImage::Format_ARGB32_Premultiplied);

    QPainter p(&img);

    p.setPen(QPen(Qt::black, 3));
    p.drawLine(64, 64, 64, 64);
}

void fpe_divByZero()
{
    FpExceptionChecker checker(FE_UNDERFLOW | FE_OVERFLOW | FE_INVALID | FE_DIVBYZERO);

    QImage img(128, 128, QImage::Format_ARGB32_Premultiplied);

    QPainter p(&img);

    p.setRenderHint(QPainter::Antialiasing);

    p.drawRect(QRectF(10, 10, 100, 0));
    p.drawRect(QRectF(10, 10, 0, 100));

    p.drawRect(QRect(10, 10, 100, 0));
    p.drawRect(QRect(10, 10, 0, 100));

    p.fillRect(QRectF(10, 10, 100, 0), Qt::black);
    p.fillRect(QRectF(10, 10, 0, 100), Qt::black);

    p.fillRect(QRect(10, 10, 100, 0), Qt::black);
    p.fillRect(QRect(10, 10, 0, 100), Qt::black);
}

void fpe_steepSlopes()
{
    FpExceptionChecker checker(FE_UNDERFLOW | FE_OVERFLOW | FE_INVALID | FE_DIVBYZERO);

    QImage img(1024, 1024, QImage::Format_ARGB32_Premultiplied);

    QFETCH(QTransform, transform);
    QFETCH(QLineF, line);
    QFETCH(bool, antialiased);

    QPainter p(&img);

    p.setPen(QPen(Qt::black, 1));
    p.setRenderHint(QPainter::Antialiasing, antialiased);
    p.setTransform(transform);

    p.drawLine(line);
}

void fpe_radialGradients()
{
    FpExceptionChecker checker(FE_UNDERFLOW | FE_OVERFLOW | FE_INVALID | FE_DIVBYZERO);

    QImage img(21, 21, QImage::Format_ARGB32_Premultiplied);
    img.fill(0);

    double m = img.width() * 0.5;

    QPainter p(&img);
    p.setRenderHints(QPainter::Antialiasing);
    p.setPen(Qt::NoPen);
    p.setBrush(QRadialGradient(m, m, m));
    p.drawEllipse(img.rect());
}

#define FPE_TEST(x) \
void tst_QPainter::x() \
{ \
    ::x(); \
}
#else
#define FPE_TEST(x) \
void tst_QPainter::x() \
{ \
    QSKIP("Floating point exception checking (fenv.h) not available", SkipAll); \
}
#endif

FPE_TEST(fpe_rasterizeLine_task232012)
FPE_TEST(fpe_pixmapTransform)
FPE_TEST(fpe_zeroLengthLines)
FPE_TEST(fpe_divByZero)
FPE_TEST(fpe_steepSlopes)
FPE_TEST(fpe_radialGradients)

void tst_QPainter::fpe_steepSlopes_data()
{
    QTest::addColumn<QTransform>("transform");
    QTest::addColumn<QLineF>("line");
    QTest::addColumn<bool>("antialiased");

    {
        const qreal dsin = 0.000014946676875461832484392500630665523431162000633776187896728515625;
        const qreal dcos = 0.9999999998882984630910186751862056553363800048828125;

        const QTransform transform = QTransform(QMatrix(dcos, dsin, -dsin, dcos, 64, 64));
        const QLineF line(2, 2, 2, 6);

        QTest::newRow("task 207147 aa") << transform << line << true;
        QTest::newRow("task 207147 no aa") << transform << line << false;
    }

    {
        QTransform transform;
        transform.rotate(0.0000001);
        const QLineF line(5, 5, 10, 5);

        QTest::newRow("task 166702 aa") << transform << line << true;
        QTest::newRow("task 166702 no aa") << transform << line << false;
    }

    {
        const QTransform transform;
        const QLineF line(2.5, 2.5, 2.5 + 1/256., 60000.5);

        QTest::newRow("steep line aa") << transform << line << true;
        QTest::newRow("steep line no aa") << transform << line << false;
    }

    {
        const QTransform transform;
        const QLineF line(2.5, 2.5, 2.5 + 1/256., 1024);

        QTest::newRow("steep line 2 aa") << transform << line << true;
        QTest::newRow("steep line 2 no aa") << transform << line << false;
    }

    {
        const QTransform transform;
        const QLineF line(2.5, 2.5, 2.5 + 1/64., 1024);

        QTest::newRow("steep line 3 aa") << transform << line << true;
        QTest::newRow("steep line 3 no aa") << transform << line << false;
    }
}

qreal randf()
{
    return rand() / (RAND_MAX + 1.0);
}

QPointF randInRect(const QRectF &rect)
{
    const qreal x = rect.left() + rect.width() * randf();
    const qreal y = rect.top() + rect.height() * randf();

    return QPointF(x, y);
}

void tst_QPainter::rasterizer_asserts()
{
    QImage img(64, 64, QImage::Format_ARGB32_Premultiplied);

    QRectF middle(QPointF(0, 0), img.size());
    QRectF left = middle.translated(-middle.width(), 0);
    QRectF right = middle.translated(middle.width(), 0);

    QPainter p(&img);
    img.fill(Qt::white);
    p.setCompositionMode(QPainter::CompositionMode_Destination);
    for (int i = 0; i < 100000; ++i) {
        QPainterPath path;
        path.moveTo(randInRect(middle));
        path.lineTo(randInRect(left));
        path.lineTo(randInRect(right));

        p.fillPath(path, Qt::black);
    }
}

void tst_QPainter::rasterizer_negativeCoords()
{
    QImage img(64, 64, QImage::Format_ARGB32_Premultiplied);
    img.fill(0x0);

    QImage original = img;

    QPainter p(&img);
    p.rotate(90);
    p.fillRect(0, 0, 70, 50, Qt::black);

    // image should not have changed
    QCOMPARE(img.pixel(0, 0), 0x0U);
    QCOMPARE(img, original);
}

void tst_QPainter::blendOverFlow_data()
{
    QTest::addColumn<QImage::Format>("format");
    QTest::addColumn<int>("width");
    QTest::addColumn<int>("height");

    QImage::Format format = QImage::Format_ARGB8555_Premultiplied;
    QTest::newRow("555,1,1") << format << 1 << 1;
    QTest::newRow("555,2,2") << format << 2 << 2;
    QTest::newRow("555,10,10") << format << 10 << 10;

    format = QImage::Format_ARGB8565_Premultiplied;
    QTest::newRow("565,1,1") << format << 1 << 1;
    QTest::newRow("565,2,2") << format << 2 << 2;
    QTest::newRow("565,10,10") << format << 10 << 10;
}

void tst_QPainter::blendOverFlow()
{
    QFETCH(QImage::Format, format);
    QFETCH(int, width);
    QFETCH(int, height);

    QImage dest(width, height, format);
    QImage src(width, height, format);

    {
        QPainter p(&dest);
        p.fillRect(0, 0, width, height, Qt::green);
    }
    QImage expected = dest;

    {
        QPainter p(&src);
        p.setCompositionMode(QPainter::CompositionMode_Source);
        p.fillRect(0, 0, width, height, QColor(0, 255, 0, 6));
    }

    {
        QPainter p(&dest);
        p.drawImage(0, 0, src);
    }

    QCOMPARE(dest.pixel(0, 0), expected.pixel(0, 0));
    QCOMPARE(dest, expected);
}

void tst_QPainter::largeImagePainting_data()
{
    QTest::addColumn<int>("width");
    QTest::addColumn<int>("height");
    QTest::addColumn<bool>("antialiased");

    QTest::newRow("tall") << 1 << 32767 << false;
    QTest::newRow("tall aa") << 1 << 32767 << true;
    QTest::newRow("wide") << 32767 << 1 << false;
    QTest::newRow("wide aa") << 32767 << 1 << true;
}

void tst_QPainter::largeImagePainting()
{
    QPainterPath path;
    path.addRect(0, 0, 1, 1);
    path.addRect(2, 0, 1, 1);
    path.addRect(0, 2, 1, 1);

    QFETCH(int, width);
    QFETCH(int, height);
    QFETCH(bool, antialiased);

    QImage img(width, height, QImage::Format_ARGB32_Premultiplied);
    img.fill(0x0);

    QPainter p(&img);
    p.setPen(Qt::NoPen);
    p.setBrush(Qt::white);

    p.setRenderHint(QPainter::Antialiasing, antialiased);

    for (int i = 0; i < img.width(); i += 4) {
        p.drawPath(path);
        p.translate(4, 0);
    }

    p.resetMatrix();

    for (int i = 4; i < img.height(); i += 4) {
        p.translate(0, 4);
        p.drawPath(path);
    }

    for (int i = 0; i < img.width(); ++i) {
        if (i % 2)
            QCOMPARE(img.pixel(i, 0), 0x0U);
        else
            QCOMPARE(img.pixel(i, 0), 0xffffffffU);
    }

    for (int i = 1; i < img.height(); ++i) {
        if (i % 2)
            QCOMPARE(img.pixel(0, i), 0x0U);
        else
            QCOMPARE(img.pixel(0, i), 0xffffffffU);
    }
}

void tst_QPainter::imageScaling_task206785()
{
    QImage src(32, 2, QImage::Format_ARGB32_Premultiplied);
    src.fill(0xffffffff);

    QImage dst(128, 128, QImage::Format_ARGB32_Premultiplied);

    QImage expected(128, 128, QImage::Format_ARGB32_Premultiplied);
    expected.fill(0xffffffff);

    for (int i = 1; i < 5; ++i) {
        qreal scale = i / qreal(5);

        dst.fill(0xff000000);

        QPainter p(&dst);
        p.scale(dst.width() / qreal(src.width()), scale);

        for (int y = 0; y * scale < dst.height(); ++y)
            p.drawImage(0, y, src);

        p.end();

        QCOMPARE(dst, expected);
    }
}

#define FOR_EACH_NEIGHBOR_8 for (int dx = -1; dx <= 1; ++dx) for (int dy = -1; dy <= 1; ++dy) if (dx != 0 || dy != 0)
#define FOR_EACH_NEIGHBOR_4 for (int dx = -1; dx <= 1; ++dx) for (int dy = -1; dy <= 1; ++dy) if ((dx == 0) != (dy == 0))

uint qHash(const QPoint &point)
{
    return qHash(qMakePair(point.x(), point.y()));
}

bool verifyOutlineFillConsistency(const QImage &img, QRgb outside, QRgb inside, QRgb outline)
{
    if (img.pixel(img.width() / 2, img.height() / 2) != inside)
        return false;

    int x = img.width() / 2;
    int y = img.height() / 2;

    while (img.pixel(++x, y) == inside)
        ;

    if (img.pixel(x, y) != outline)
        return false;

    QQueue<QPoint> discovered;
    discovered.enqueue(QPoint(x, y));

    QVector<bool> visited(img.width() * img.height());
    visited.fill(false);

    while (!discovered.isEmpty()) {
        QPoint p = discovered.dequeue();
        QRgb pixel = img.pixel(p.x(), p.y());

        bool &v = visited[p.y() * img.width() + p.x()];
        if (v)
            continue;
        v = true;

        if (pixel == outline) {
            FOR_EACH_NEIGHBOR_8 {
                QPoint x(p.x() + dx, p.y() + dy);
                discovered.enqueue(x);
            }
        } else {
            FOR_EACH_NEIGHBOR_4 {
                if ((dx == 0) == (dy == 0))
                    continue;
                QRgb neighbor = img.pixel(p.x() + dx, p.y() + dy);
                if ((pixel == inside && neighbor == outside) ||
                    (pixel == outside && neighbor == inside))
                    return false;
            }
        }
    }

    return true;
}

#undef FOR_EACH_NEIGHBOR_8
#undef FOR_EACH_NEIGHBOR_4

void tst_QPainter::outlineFillConsistency()
{
    QImage dst(256, 256, QImage::Format_ARGB32_Premultiplied);

    QPolygonF poly;
    poly << QPointF(5, -100) << QPointF(-70, 20) << QPointF(95, 25);

    QPen pen(Qt::red);
    QBrush brush(Qt::black);

    QRgb background = 0xffffffff;
    for (int i = 0; i < 360; ++i) {
        dst.fill(background);

        QPainter p(&dst);
        p.translate(dst.width() / 2, dst.height() / 2);

        QPolygonF copy = poly;
        for (int j = 0; j < copy.size(); ++j)
            copy[j] = QTransform().rotate(i).map(copy[j]);

        p.setPen(pen);
        p.setBrush(brush);
        p.drawPolygon(copy);
        p.end();

#if 0
        if (!verifyOutlineFillConsistency(dst, background, p.brush().color().rgba(), p.pen().color().rgba()))
            dst.save(QString("outlineFillConsistency-%1.png").arg(i));
#endif

        QVERIFY(verifyOutlineFillConsistency(dst, background, brush.color().rgba(), pen.color().rgba()));
    }
}

void tst_QPainter::drawImage_task217400_data()
{
    QTest::addColumn<QImage::Format>("format");

    QTest::newRow("444") << QImage::Format_ARGB4444_Premultiplied;
    QTest::newRow("555") << QImage::Format_ARGB8555_Premultiplied;
    QTest::newRow("565") << QImage::Format_ARGB8565_Premultiplied;
//    QTest::newRow("666") << QImage::Format_ARGB6666_Premultiplied;
    QTest::newRow("888p") << QImage::Format_ARGB32_Premultiplied;
    QTest::newRow("888") << QImage::Format_ARGB32;
}

void tst_QPainter::drawImage_task217400()
{
    QFETCH(QImage::Format, format);

    const QImage src = QImage(QString(SRCDIR) + "/task217400.png")
                       .convertToFormat(format);
    QVERIFY(!src.isNull());

    QImage expected(src.size(), format);
    {
        QPainter p(&expected);
        p.fillRect(0, 0, expected.width(), expected.height(), Qt::white);
        p.drawImage(0, 0, src);
    }

    for (int i = 1; i <= 4; ++i) {
        QImage dest(src.width() + i, src.height(), format);
        {
            QPainter p(&dest);
            p.fillRect(0, 0, dest.width(), dest.height(), Qt::white);
            p.drawImage(i, 0, src);
        }

        const QImage result = dest.copy(i, 0, src.width(), src.height());

#if 0
        if (result != expected) {
            qDebug("i=%i", i);
            result.save("result.png");
            expected.save("expected.png");
        }
#endif
        QCOMPARE(result, expected);
    }
}

void tst_QPainter::drawImage_task258776()
{
    QImage src(16, 16, QImage::Format_RGB888);
    QImage dest(33, 33, QImage::Format_RGB888);
    src.fill(0x00ff00);
    dest.fill(0x0000ff);

    QPainter painter(&dest);
    painter.drawImage(QRectF(0.499, 0.499, 32, 32), src, QRectF(0, 0, 16, 16));
    painter.end();

    QImage expected(33, 33, QImage::Format_RGB32);
    expected.fill(0xff0000);

    painter.begin(&expected);
    painter.drawImage(QRectF(0, 0, 32, 32), src);
    painter.end();

    dest = dest.convertToFormat(QImage::Format_RGB32);

    dest.save("dest.png");
    expected.save("expected.png");
    QCOMPARE(dest, expected);
}

void tst_QPainter::clipRectSaveRestore()
{
    QImage img(64, 64, QImage::Format_ARGB32_Premultiplied);
    img.fill(0x0);

    QPainter p(&img);
    p.setClipRect(QRect(0, 0, 10, 10));
    p.save();
    p.setClipRect(QRect(5, 5, 5, 5), Qt::IntersectClip);
    p.restore();
    p.fillRect(0, 0, 64, 64, Qt::black);
    p.end();

    QCOMPARE(img.pixel(0, 0), QColor(Qt::black).rgba());
}

void tst_QPainter::clippedImage()
{
    QImage img(16, 16, QImage::Format_ARGB32_Premultiplied);
    img.fill(0x0);

    QImage src(16, 16, QImage::Format_RGB32);
    src.fill(QColor(Qt::red).rgba());

    QPainter p(&img);
    p.setClipRect(QRect(1, 1, 14, 14));
    p.drawImage(0, 0, src);
    p.end();

    QCOMPARE(img.pixel(0, 0), 0x0U);
    QCOMPARE(img.pixel(1, 1), src.pixel(1, 1));
}

void tst_QPainter::stateResetBetweenQPainters()
{
    QImage img(16, 16, QImage::Format_ARGB32);

    {
        QPainter p(&img);
        p.setCompositionMode(QPainter::CompositionMode_Source);
        p.fillRect(0, 0, 16, 16, Qt::red);
    }

    {
        QPainter p2(&img);
        p2.fillRect(0, 0, 16, 16, QColor(0, 0, 255, 63));
    }

    img.save("foo.png");

    QVERIFY(img.pixel(0, 0) != qRgba(0, 0, 255, 63));
    QVERIFY(qRed(img.pixel(0, 0)) > 0); // We didn't erase the red channel...
    QVERIFY(qBlue(img.pixel(0, 0)) < 255); // We blended the blue channel
}

void tst_QPainter::drawRect_task215378()
{
    QImage img(11, 11, QImage::Format_ARGB32_Premultiplied);
    img.fill(QColor(Qt::white).rgba());

    QPainter p(&img);
    p.setPen(QColor(127, 127, 127, 127));
    p.drawRect(0, 0, 10, 10);
    p.end();

    QCOMPARE(img.pixel(0, 0), img.pixel(1, 0));
    QCOMPARE(img.pixel(0, 0), img.pixel(0, 1));
    QVERIFY(img.pixel(0, 0) != img.pixel(1, 1));
}

void tst_QPainter::drawRect_task247505()
{
    QImage a(10, 10, QImage::Format_ARGB32_Premultiplied);
    a.fill(0);
    QImage b = a;

    QPainter p(&a);
    p.setPen(Qt::NoPen);
    p.setBrush(Qt::black);
    p.drawRect(QRectF(10, 0, -10, 10));
    p.end();
    p.begin(&b);
    p.setPen(Qt::NoPen);
    p.setBrush(Qt::black);
    p.drawRect(QRectF(0, 0, 10, 10));
    p.end();

    QCOMPARE(a, b);
}

void tst_QPainter::drawImage_data()
{
    QTest::addColumn<int>("x");
    QTest::addColumn<int>("y");
    QTest::addColumn<int>("w");
    QTest::addColumn<int>("h");
    QTest::addColumn<QImage::Format>("srcFormat");
    QTest::addColumn<QImage::Format>("dstFormat");

    for (int srcFormat = QImage::Format_Mono; srcFormat < QImage::NImageFormats; ++srcFormat) {
        for (int dstFormat = QImage::Format_Mono; dstFormat < QImage::NImageFormats; ++dstFormat) {
            if (dstFormat == QImage::Format_Indexed8)
                continue;
            for (int odd_x = 0; odd_x <= 1; ++odd_x) {
                for (int odd_width = 0; odd_width <= 1; ++odd_width) {
                    QString description =
                        QString("srcFormat %1, dstFormat %2, odd x: %3, odd width: %4")
                            .arg(srcFormat).arg(dstFormat).arg(odd_x).arg(odd_width);

                    QTest::newRow(qPrintable(description)) << (10 + odd_x) << 10 << (20 + odd_width) << 20
                        << QImage::Format(srcFormat)
                        << QImage::Format(dstFormat);
                }
            }
        }
    }
}

bool verifyImage(const QImage &img, int x, int y, int w, int h, uint background)
{
    int imgWidth = img.width();
    int imgHeight = img.height();
    for (int i = 0; i < imgHeight; ++i) {
        for (int j = 0; j < imgWidth; ++j) {
            uint pixel = img.pixel(j, i);
            bool outside = j < x || j >= (x + w) || i < y || i >= (y + h);
            if (outside != (pixel == background)) {
                //printf("%d %d, expected %x, got %x, outside: %d\n", x, y, background, pixel, outside);
                return false;
            }
        }
    }

    return true;
}

void tst_QPainter::drawImage()
{
    QFETCH(int, x);
    QFETCH(int, y);
    QFETCH(int, w);
    QFETCH(int, h);
    QFETCH(QImage::Format, srcFormat);
    QFETCH(QImage::Format, dstFormat);

    QImage dst(40, 40, QImage::Format_RGB32);
    dst.fill(0xffffffff);

    dst = dst.convertToFormat(dstFormat);
    uint background = dst.pixel(0, 0);

    QImage src(w, h, QImage::Format_RGB32);
    src.fill(0xff000000);
    src = src.convertToFormat(srcFormat);

    QPainter p(&dst);
    p.drawImage(x, y, src);
    p.end();

    QVERIFY(verifyImage(dst, x, y, w, h, background));
}

void tst_QPainter::imageCoordinateLimit()
{
    QImage img(64, 40000, QImage::Format_MonoLSB);
    QPainter p(&img);
    p.drawText(10, 36000, QLatin1String("foo"));
    p.setPen(QPen(Qt::black, 2));
    p.drawLine(10, 0, 60, 40000);

    p.setRenderHint(QPainter::Antialiasing);
    p.drawLine(10, 0, 60, 40000);
}


void tst_QPainter::imageBlending_data()
{
    QTest::addColumn<QImage::Format>("sourceFormat");
    QTest::addColumn<QImage::Format>("destFormat");
    QTest::addColumn<int>("error");

    int error_rgb565 = ((1<<3) + (1<<2) + (1<<3));
    QTest::newRow("rgb565_on_rgb565") << QImage::Format_RGB16
                                      << QImage::Format_RGB16
                                      << 0;
    QTest::newRow("argb8565_on_rgb565") << QImage::Format_ARGB8565_Premultiplied
                                        << QImage::Format_RGB16
                                        << error_rgb565;

    QTest::newRow("rgb32_on_rgb565") << QImage::Format_RGB32
                                     << QImage::Format_RGB16
                                     << error_rgb565;

    QTest::newRow("argb32pm_on_rgb565") << QImage::Format_ARGB32_Premultiplied
                                        << QImage::Format_RGB16
                                        << error_rgb565;
}

int diffColor(quint32 ap, quint32 bp)
{
    int a = qAlpha(ap) - qAlpha(bp);
    int r = qRed(ap) - qRed(bp);
    int b = qBlue(ap) - qBlue(bp);
    int g = qBlue(ap) - qBlue(bp);

    return qAbs(a) + qAbs(r) + qAbs(g) + qAbs(b);
}

// this test assumes premultiplied pixels...

void tst_QPainter::imageBlending()
{
    QFETCH(QImage::Format, sourceFormat);
    QFETCH(QImage::Format, destFormat);
    QFETCH(int, error);

    QImage dest;
    {
        QImage orig_dest(6, 6, QImage::Format_ARGB32_Premultiplied);
        orig_dest.fill(0);
        QPainter p(&orig_dest);
        p.fillRect(0, 0, 6, 3, QColor::fromRgbF(1, 0, 0));
        p.fillRect(3, 0, 3, 6, QColor::fromRgbF(0, 0, 1, 0.5));
        p.end();
        dest = orig_dest.convertToFormat(destFormat);

        // An image like this: (r = red, m = magenta, b = light alpha blue, 0 = transparent)
        // r r r m m m
        // r r r m m m
        // r r r m m m
        // 0 0 0 b b b
        // 0 0 0 b b b
        // 0 0 0 b b b
    }

    QImage source;
    {
        QImage orig_source(6, 6, QImage::Format_ARGB32_Premultiplied);
        orig_source.fill(0);
        QPainter p(&orig_source);
        p.fillRect(1, 1, 4, 4, QColor::fromRgbF(0, 1, 0, 0.5));
        p.fillRect(2, 2, 2, 2, QColor::fromRgbF(0, 1, 0));
        p.end();
        source = orig_source.convertToFormat(sourceFormat);

        // An image like this: (0 = transparent, . = green at 0.5 alpha, g = opaque green.
        // 0 0 0 0 0 0
        // 0 . . . . 0
        // 0 . g g . 0
        // 0 . g g . 0
        // 0 . . . . 0
        // 0 0 0 0 0 0
    }

    QPainter p(&dest);
    p.drawImage(0, 0, source);
    p.end();

    // resulting image:
    // r  r  r  m  m  m
    // r  r. r. m. m. m
    // r  r. g  g  m. m
    // 0  .  g  g  b. b
    // 0  .  .  b. b. b
    // 0  0  0  b  b  b

    // the g pixels, always green..
    QVERIFY(diffColor(dest.pixel(2, 2), 0xff00ff00) <= error); // g

    if (source.hasAlphaChannel()) {
        QVERIFY(diffColor(dest.pixel(0, 0), 0xffff0000) <= error); // r
        QVERIFY(diffColor(dest.pixel(5, 0), 0xff7f007f) <= error); // m
        QVERIFY(diffColor(dest.pixel(1, 1), 0xff7f7f00) <= error); // r.
        QVERIFY(diffColor(dest.pixel(4, 1), 0xff3f7f3f) <= error); // m.
        if (dest.hasAlphaChannel()) {
            QVERIFY(diffColor(dest.pixel(1, 3), 0x7f007f00) <= error); // .
            QVERIFY(diffColor(dest.pixel(4, 3), 0x7f007f3f) <= error); // b.
            QVERIFY(diffColor(dest.pixel(4, 3), 0x7f007f3f) <= error); // b.
            QVERIFY(diffColor(dest.pixel(4, 4), 0x7f00007f) <= error); // b
            QVERIFY(diffColor(dest.pixel(4, 0), 0) <= 0); // 0
       }
    } else {
        QVERIFY(diffColor(dest.pixel(0, 0), 0xff000000) <= 0);
        QVERIFY(diffColor(dest.pixel(1, 1), 0xff007f00) <= error);
    }
}

void tst_QPainter::imageBlending_clipped()
{
    QImage src(20, 20, QImage::Format_RGB16);
    QPainter p(&src);
    p.fillRect(src.rect(), Qt::red);
    p.end();

    QImage dst(40, 20, QImage::Format_RGB16);
    p.begin(&dst);
    p.fillRect(dst.rect(), Qt::white);
    p.end();

    QImage expected = dst;

    p.begin(&dst);
    p.setClipRect(QRect(23, 0, 20, 20));

    // should be completely clipped
    p.drawImage(QRectF(3, 0, 20, 20), src);
    p.end();

    // dst should be left unchanged
    QCOMPARE(dst, expected);
}

void tst_QPainter::paintOnNullPixmap()
{
    QPixmap pix(16, 16);

    QPixmap textPixmap;
    QPainter p(&textPixmap);
    p.drawPixmap(10, 10, pix);
    p.end();

    QPixmap textPixmap2(16,16);
    p.begin(&textPixmap2);
    p.end();
}

void tst_QPainter::checkCompositionMode()
{
    QImage refImage(50,50,QImage::Format_ARGB32);
    QPainter painter(&refImage);
    painter.fillRect(QRect(0,0,50,50),Qt::blue);

    QImage testImage(50,50,QImage::Format_ARGB32);
    QPainter p(&testImage);
    p.fillRect(QRect(0,0,50,50),Qt::red);
    p.save();
    p.setCompositionMode(QPainter::CompositionMode_SourceOut);
    p.restore();
    p.fillRect(QRect(0,0,50,50),Qt::blue);

    QCOMPARE(refImage.pixel(20,20),testImage.pixel(20,20));
}

static QLinearGradient inverseGradient(QLinearGradient g)
{
    QLinearGradient g2 = g;

    QGradientStops stops = g.stops();

    QGradientStops inverse;
    foreach (QGradientStop stop, stops)
        inverse << QGradientStop(1 - stop.first, stop.second);

    g2.setStops(inverse);
    return g2;
}

void tst_QPainter::linearGradientSymmetry()
{
    QImage a(64, 8, QImage::Format_ARGB32_Premultiplied);
    QImage b(64, 8, QImage::Format_ARGB32_Premultiplied);

    a.fill(0);
    b.fill(0);

    QLinearGradient gradient(QRectF(b.rect()).topLeft(), QRectF(b.rect()).topRight());
    gradient.setColorAt(0.0, Qt::blue);
    gradient.setColorAt(0.2, QColor(220, 220, 220, 0));
    gradient.setColorAt(0.6, Qt::red);
    gradient.setColorAt(0.9, QColor(220, 220, 220, 255));
    gradient.setColorAt(1.0, Qt::black);

    QPainter pa(&a);
    pa.fillRect(a.rect(), gradient);
    pa.end();

    QPainter pb(&b);
    pb.fillRect(b.rect(), inverseGradient(gradient));
    pb.end();

    b = b.mirrored(true);
    QCOMPARE(a, b);
}

void tst_QPainter::gradientInterpolation()
{
    QImage image(256, 8, QImage::Format_ARGB32_Premultiplied);
    QPainter painter;

    QLinearGradient gradient(QRectF(image.rect()).topLeft(), QRectF(image.rect()).topRight());
    gradient.setColorAt(0.0, QColor(255, 0, 0, 0));
    gradient.setColorAt(1.0, Qt::blue);

    image.fill(0);
    painter.begin(&image);
    painter.fillRect(image.rect(), gradient);
    painter.end();

    const QRgb *line = reinterpret_cast<QRgb *>(image.scanLine(3));

    for (int i = 0; i < 256; ++i) {
        QCOMPARE(qAlpha(line[i]), qBlue(line[i])); // bright blue
        QVERIFY(qAbs(qAlpha(line[i]) - i) < 3); // linear alpha
        QCOMPARE(qRed(line[i]), 0); // no red component
        QCOMPARE(qGreen(line[i]), 0); // no green component
    }

    gradient.setInterpolationMode(QGradient::ComponentInterpolation);

    image.fill(0);
    painter.begin(&image);
    painter.fillRect(image.rect(), gradient);
    painter.end();

    for (int i = 1; i < 256; ++i) {
        if (i < 128) {
            QVERIFY(qRed(line[i]) >= qBlue(line[i])); // red is dominant
        } else {
            QVERIFY(qRed(line[i]) <= qBlue(line[i])); // blue is dominant
        }
        QVERIFY((qRed(line[i]) - 0.5) * (qAlpha(line[i - 1]) - 0.5) <= (qRed(line[i - 1]) + 0.5) * (qAlpha(line[i]) + 0.5)); // decreasing red
        QVERIFY((qBlue(line[i]) + 0.5) * (qAlpha(line[i - 1]) + 0.5) >= (qBlue(line[i - 1]) - 0.5) * (qAlpha(line[i]) - 0.5)); // increasing blue
        QVERIFY(qAbs(qAlpha(line[i]) - i) < 3); // linear alpha
        QCOMPARE(qGreen(line[i]), 0); // no green component
    }
}

void tst_QPainter::drawPolygon()
{
    QImage img(128, 128, QImage::Format_ARGB32_Premultiplied);

    QPainterPathStroker stroker;
    stroker.setWidth(1.5);

    QPainterPath path;
    path.moveTo(2, 34);
    path.lineTo(34, 2);

    QPolygonF poly = stroker.createStroke(path).toFillPolygon();

    img.fill(0xffffffff);
    QPainter p(&img);
    p.setRenderHint(QPainter::Antialiasing);
    p.setBrush(Qt::red);
    p.setPen(Qt::NoPen);
    p.drawPolygon(poly);
    p.translate(64, 64);
    p.drawPolygon(poly);
    p.end();

    QImage a = img.copy();

    img.fill(0xffffffff);
    p.begin(&img);
    p.setRenderHint(QPainter::Antialiasing);
    p.setBrush(Qt::red);
    p.setPen(Qt::NoPen);
    p.translate(64, 64);
    p.drawPolygon(poly);
    p.resetTransform();
    p.drawPolygon(poly);
    p.end();

    QCOMPARE(a, img);
}

void tst_QPainter::inactivePainter()
{
    // This test succeeds if it doesn't segfault.

    QPainter p;
    QPainterPath path;
    QRegion region(QRect(20, 20, 60, 40));
    QPolygonF polygon(QVector<QPointF>() << QPointF(0, 0) << QPointF(12, 0) << QPointF(8, 6));
    path.addPolygon(polygon);

    p.save();
    p.restore();

    p.background();
    p.setBackground(QBrush(Qt::blue));

    p.brush();
    p.setBrush(Qt::red);
    p.setBrush(Qt::NoBrush);
    p.setBrush(QBrush(Qt::white, Qt::DiagCrossPattern));

    p.backgroundMode();
    p.setBackgroundMode(Qt::OpaqueMode);

    p.boundingRect(QRectF(0, 0, 100, 20), Qt::AlignCenter, QLatin1String("Hello, World!"));
    p.boundingRect(QRect(0, 0, 100, 20), Qt::AlignCenter, QLatin1String("Hello, World!"));

    p.brushOrigin();
    p.setBrushOrigin(QPointF(12, 34));
    p.setBrushOrigin(QPoint(12, 34));

    p.clipPath();
    p.clipRegion();
    p.hasClipping();
    p.setClipPath(path);
    p.setClipRect(QRectF(42, 42, 42, 42));
    p.setClipRect(QRect(42, 42, 42, 42));
    p.setClipRegion(region);
    p.setClipping(true);

    p.combinedMatrix();
    p.combinedTransform();

    p.compositionMode();
    p.setCompositionMode(QPainter::CompositionMode_Plus);

    p.device();
    p.deviceMatrix();
    p.deviceTransform();

    p.font();
    p.setFont(QFont(QLatin1String("Times"), 24));

    p.fontInfo();
    p.fontMetrics();

    p.layoutDirection();
    p.setLayoutDirection(Qt::RightToLeft);

    p.opacity();
    p.setOpacity(0.75);

    p.pen();
    p.setPen(QPen(Qt::red));
    p.setPen(Qt::green);
    p.setPen(Qt::NoPen);

    p.renderHints();
    p.setRenderHint(QPainter::Antialiasing, true);
    p.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform, false);

    p.resetMatrix();
    p.resetTransform();
    p.rotate(1);
    p.scale(2, 2);
    p.shear(-1, 1);
    p.translate(3, 14);

    p.viewTransformEnabled();
    p.setViewTransformEnabled(true);

    p.viewport();
    p.setViewport(QRect(10, 10, 620, 460));

    p.window();
    p.setWindow(QRect(10, 10, 620, 460));

    p.worldMatrix();
    p.setWorldMatrix(QMatrix().translate(43, 21), true);
    p.setWorldMatrixEnabled(true);

    p.transform();
    p.setTransform(QTransform().translate(12, 34), true);

    p.worldTransform();
    p.setWorldTransform(QTransform().scale(0.5, 0.5), true);
}

bool testCompositionMode(int src, int dst, int expected, QPainter::CompositionMode op, qreal opacity = 1.0)
{
    // The test image needs to be large enough to test SIMD code
    const QSize imageSize(100, 100);

    QImage actual(imageSize, QImage::Format_ARGB32_Premultiplied);
    actual.fill(QColor(dst, dst, dst).rgb());

    QPainter p(&actual);
    p.setCompositionMode(op);
    p.setOpacity(opacity);
    p.fillRect(QRect(QPoint(), imageSize), QColor(src, src, src));
    p.end();

    if (qRed(actual.pixel(0, 0)) != expected) {
        qDebug("Fail: mode %d, src[%d] dst [%d] actual [%d] expected [%d]", op,
               src, dst, qRed(actual.pixel(0, 0)), expected);
        return false;
    } else {
        QImage refImage(imageSize, QImage::Format_ARGB32_Premultiplied);
        refImage.fill(QColor(expected, expected, expected).rgb());
        return actual == refImage;
    }
}

void tst_QPainter::extendedBlendModes()
{
    QVERIFY(testCompositionMode(255, 255, 255, QPainter::CompositionMode_Plus));
    QVERIFY(testCompositionMode(  0,   0,   0, QPainter::CompositionMode_Plus));
    QVERIFY(testCompositionMode(127, 128, 255, QPainter::CompositionMode_Plus));
    QVERIFY(testCompositionMode(127,   0, 127, QPainter::CompositionMode_Plus));
    QVERIFY(testCompositionMode(  0, 127, 127, QPainter::CompositionMode_Plus));
    QVERIFY(testCompositionMode(255,   0, 255, QPainter::CompositionMode_Plus));
    QVERIFY(testCompositionMode(  0, 255, 255, QPainter::CompositionMode_Plus));
    QVERIFY(testCompositionMode(128, 128, 255, QPainter::CompositionMode_Plus));

    QVERIFY(testCompositionMode(255, 255, 255, QPainter::CompositionMode_Plus, 0.3));
    QVERIFY(testCompositionMode(  0,   0,   0, QPainter::CompositionMode_Plus, 0.3));
    QVERIFY(testCompositionMode(127, 128, 165, QPainter::CompositionMode_Plus, 0.3));
    QVERIFY(testCompositionMode(127,   0,  37, QPainter::CompositionMode_Plus, 0.3));
    QVERIFY(testCompositionMode(  0, 127, 127, QPainter::CompositionMode_Plus, 0.3));
    QVERIFY(testCompositionMode(255,   0,  75, QPainter::CompositionMode_Plus, 0.3));
    QVERIFY(testCompositionMode(  0, 255, 255, QPainter::CompositionMode_Plus, 0.3));
    QVERIFY(testCompositionMode(128, 128, 166, QPainter::CompositionMode_Plus, 0.3));
    QVERIFY(testCompositionMode(186, 200, 255, QPainter::CompositionMode_Plus, 0.3));

    QVERIFY(testCompositionMode(255, 255, 255, QPainter::CompositionMode_Multiply));
    QVERIFY(testCompositionMode(  0,   0,   0, QPainter::CompositionMode_Multiply));
    QVERIFY(testCompositionMode(127, 255, 127, QPainter::CompositionMode_Multiply));
    QVERIFY(testCompositionMode(255, 127, 127, QPainter::CompositionMode_Multiply));
    QVERIFY(testCompositionMode( 63, 255,  63, QPainter::CompositionMode_Multiply));
    QVERIFY(testCompositionMode(255,  63,  63, QPainter::CompositionMode_Multiply));
    QVERIFY(testCompositionMode(127, 127,  63, QPainter::CompositionMode_Multiply));

    QVERIFY(testCompositionMode(255, 255, 255, QPainter::CompositionMode_Screen));
    QVERIFY(testCompositionMode(  0,   0,   0, QPainter::CompositionMode_Screen));
    QVERIFY(testCompositionMode( 63, 255, 255, QPainter::CompositionMode_Screen));
    QVERIFY(testCompositionMode(255,  63, 255, QPainter::CompositionMode_Screen));
    QVERIFY(testCompositionMode( 63,   0,  63, QPainter::CompositionMode_Screen));
    QVERIFY(testCompositionMode(  0,  63,  63, QPainter::CompositionMode_Screen));
    QVERIFY(testCompositionMode(127, 127, 191, QPainter::CompositionMode_Screen));

    QVERIFY(testCompositionMode(255, 255, 255, QPainter::CompositionMode_Overlay));
    QVERIFY(testCompositionMode(  0,   0,   0, QPainter::CompositionMode_Overlay));
    QVERIFY(testCompositionMode( 63,  63,  31, QPainter::CompositionMode_Overlay));
    QVERIFY(testCompositionMode( 63, 255, 255, QPainter::CompositionMode_Overlay));

    QVERIFY(testCompositionMode(255, 255, 255, QPainter::CompositionMode_Darken));
    QVERIFY(testCompositionMode(  0,   0,   0, QPainter::CompositionMode_Darken));
    QVERIFY(testCompositionMode( 63,  63,  63, QPainter::CompositionMode_Darken));
    QVERIFY(testCompositionMode( 63, 255,  63, QPainter::CompositionMode_Darken));
    QVERIFY(testCompositionMode(255,  63,  63, QPainter::CompositionMode_Darken));
    QVERIFY(testCompositionMode( 63, 127,  63, QPainter::CompositionMode_Darken));
    QVERIFY(testCompositionMode(127,  63,  63, QPainter::CompositionMode_Darken));

    QVERIFY(testCompositionMode(255, 255, 255, QPainter::CompositionMode_Lighten));
    QVERIFY(testCompositionMode(  0,   0,   0, QPainter::CompositionMode_Lighten));
    QVERIFY(testCompositionMode( 63,  63,  63, QPainter::CompositionMode_Lighten));
    QVERIFY(testCompositionMode( 63, 255, 255, QPainter::CompositionMode_Lighten));
    QVERIFY(testCompositionMode(255,  63, 255, QPainter::CompositionMode_Lighten));
    QVERIFY(testCompositionMode( 63, 127, 127, QPainter::CompositionMode_Lighten));
    QVERIFY(testCompositionMode(127,  63, 127, QPainter::CompositionMode_Lighten));

    QVERIFY(testCompositionMode(255, 255, 255, QPainter::CompositionMode_ColorDodge));
    QVERIFY(testCompositionMode(  0,   0,   0, QPainter::CompositionMode_ColorDodge));
    QVERIFY(testCompositionMode( 63, 127, 169, QPainter::CompositionMode_ColorDodge));
    QVERIFY(testCompositionMode(191, 127, 255, QPainter::CompositionMode_ColorDodge));
    QVERIFY(testCompositionMode(127, 191, 255, QPainter::CompositionMode_ColorDodge));

    QVERIFY(testCompositionMode(255, 255, 255, QPainter::CompositionMode_ColorBurn));
    QVERIFY(testCompositionMode(  0,   0,   0, QPainter::CompositionMode_ColorBurn));
    QVERIFY(testCompositionMode(127, 127,   0, QPainter::CompositionMode_ColorBurn));
    QVERIFY(testCompositionMode(128, 128,   2, QPainter::CompositionMode_ColorBurn));
    QVERIFY(testCompositionMode(191, 127,  84, QPainter::CompositionMode_ColorBurn));

    QVERIFY(testCompositionMode(255, 255, 255, QPainter::CompositionMode_HardLight));
    QVERIFY(testCompositionMode(  0,   0,   0, QPainter::CompositionMode_HardLight));
    QVERIFY(testCompositionMode(127, 127, 127, QPainter::CompositionMode_HardLight));
    QVERIFY(testCompositionMode( 63,  63,  31, QPainter::CompositionMode_HardLight));
    QVERIFY(testCompositionMode(127,  63,  63, QPainter::CompositionMode_HardLight));

    QVERIFY(testCompositionMode(255, 255, 255, QPainter::CompositionMode_SoftLight));
    QVERIFY(testCompositionMode(  0,   0,   0, QPainter::CompositionMode_SoftLight));
    QVERIFY(testCompositionMode(127, 127, 126, QPainter::CompositionMode_SoftLight));
    QVERIFY(testCompositionMode( 63,  63,  39, QPainter::CompositionMode_SoftLight));
    QVERIFY(testCompositionMode(127,  63,  62, QPainter::CompositionMode_SoftLight));

    QVERIFY(testCompositionMode(255, 255,   0, QPainter::CompositionMode_Difference));
    QVERIFY(testCompositionMode(  0,   0,   0, QPainter::CompositionMode_Difference));
    QVERIFY(testCompositionMode(255,   0, 255, QPainter::CompositionMode_Difference));
    QVERIFY(testCompositionMode(127, 127,   0, QPainter::CompositionMode_Difference));
    QVERIFY(testCompositionMode(127, 128,   1, QPainter::CompositionMode_Difference));
    QVERIFY(testCompositionMode(127,  63,  64, QPainter::CompositionMode_Difference));
    QVERIFY(testCompositionMode(  0, 127, 127, QPainter::CompositionMode_Difference));

    QVERIFY(testCompositionMode(255, 255,   0, QPainter::CompositionMode_Exclusion));
    QVERIFY(testCompositionMode(  0,   0,   0, QPainter::CompositionMode_Exclusion));
    QVERIFY(testCompositionMode(255,   0, 255, QPainter::CompositionMode_Exclusion));
    QVERIFY(testCompositionMode(127, 127, 127, QPainter::CompositionMode_Exclusion));
    QVERIFY(testCompositionMode( 63, 127, 127, QPainter::CompositionMode_Exclusion));
    QVERIFY(testCompositionMode( 63,  63,  95, QPainter::CompositionMode_Exclusion));
    QVERIFY(testCompositionMode(191, 191,  96, QPainter::CompositionMode_Exclusion));
}

void tst_QPainter::zeroOpacity()
{
    QImage source(1, 1, QImage::Format_ARGB32_Premultiplied);
    source.fill(0xffffffff);

    QImage target(1, 1, QImage::Format_RGB32);
    target.fill(0xff000000);

    QPainter p(&target);
    p.setOpacity(0.0);
    p.drawImage(0, 0, source);
    p.end();

    QCOMPARE(target.pixel(0, 0), 0xff000000);
}

void tst_QPainter::clippingBug()
{
    QImage img(32, 32, QImage::Format_ARGB32_Premultiplied);
    img.fill(0);

    QImage expected = img;
    QPainter p(&expected);
    p.fillRect(1, 1, 30, 30, Qt::red);
    p.end();

    QPainterPath path;
    path.addRect(1, 1, 30, 30);
    path.addRect(1, 1, 30, 30);
    path.addRect(1, 1, 30, 30);

    p.begin(&img);
    p.setClipPath(path);
    p.fillRect(0, 0, 32, 32, Qt::red);
    p.end();

    QCOMPARE(img, expected);
}

void tst_QPainter::emptyClip()
{
    QImage img(64, 64, QImage::Format_ARGB32_Premultiplied);
    QPainter p(&img);
    p.setRenderHints(QPainter::Antialiasing);
    p.setClipRect(0, 32, 64, 0);
    p.fillRect(0, 0, 64, 64, Qt::white);

    QPainterPath path;
    path.lineTo(64, 0);
    path.lineTo(64, 64);
    path.lineTo(40, 64);
    path.lineTo(40, 80);
    path.lineTo(0, 80);

    p.fillPath(path, Qt::green);
}

void tst_QPainter::drawImage_1x1()
{
    QImage source(1, 1, QImage::Format_ARGB32_Premultiplied);
    source.fill(0xffffffff);

    QImage img(32, 32, QImage::Format_ARGB32_Premultiplied);
    img.fill(0xff000000);
    QPainter p(&img);
    p.drawImage(QRectF(0.9, 0.9, 32, 32), source);
    p.end();

    QImage expected = img;
    expected.fill(0xff000000);
    p.begin(&expected);
    p.fillRect(1, 1, 31, 31, Qt::white);
    p.end();

    QCOMPARE(img, expected);
}

void tst_QPainter::taskQT4444_dontOverflowDashOffset()
{
    QPainter p;

    QPen pen;
    pen.setWidth(2);
    pen.setStyle(Qt::DashDotLine);

    QPointF point[4];
    point[0] = QPointF(182.50868749707968,347.78457234212630);
    point[1] = QPointF(182.50868749707968,107.22501998401277);
    point[2] = QPointF(182.50868749707968,107.22501998401277);
    point[3] = QPointF(520.46600762283651,107.22501998401277);

    QImage crashImage(QSize(1000, 120), QImage::Format_ARGB32_Premultiplied);
    p.begin(&crashImage);
    p.setPen(pen);
    p.drawLines(point, 2);
    p.end();

    QVERIFY(true); // Don't crash
}

void tst_QPainter::painterBegin()
{
    QImage nullImage;
    QImage indexed8Image(16, 16, QImage::Format_Indexed8);
    QImage rgb32Image(16, 16, QImage::Format_RGB32);
    QImage argb32Image(16, 16, QImage::Format_ARGB32_Premultiplied);

    QPainter p;

    // Painting on null image should fail.
    QVERIFY(!p.begin(&nullImage));

    // Check that the painter is not messed up by using it on another image.
    QVERIFY(p.begin(&rgb32Image));
    QVERIFY(p.end());

    // If painting on indexed8 image fails, the painter state should still be OK.
    if (p.begin(&indexed8Image))
        QVERIFY(p.end());
    QVERIFY(p.begin(&rgb32Image));
    QVERIFY(p.end());

    // Try opening a painter on the two different images.
    QVERIFY(p.begin(&rgb32Image));
    QVERIFY(!p.begin(&argb32Image));
    QVERIFY(p.end());

    // Try opening two painters on the same image.
    QVERIFY(p.begin(&rgb32Image));
    QPainter q;
    QVERIFY(!q.begin(&rgb32Image));
    QVERIFY(!q.end());
    QVERIFY(p.end());

    // Try ending an inactive painter.
    QVERIFY(!p.end());
}

void tst_QPainter::setPenColor(QPainter& p)
{
    p.setPen(Qt::NoPen);

    // Setting color, then style
    // Should work even though the pen is "NoPen with color", temporarily.
    QPen newPen(p.pen());
    newPen.setColor(Qt::red);
    QCOMPARE(p.pen().style(), newPen.style());
    QCOMPARE(p.pen().style(), Qt::NoPen);
    p.setPen(newPen);

    QCOMPARE(p.pen().color().name(), QString("#ff0000"));

    QPen newPen2(p.pen());
    newPen2.setStyle(Qt::SolidLine);
    p.setPen(newPen2);

    QCOMPARE(p.pen().color().name(), QString("#ff0000"));
}

void tst_QPainter::setPenColorOnImage()
{
    QImage img(QSize(10, 10), QImage::Format_ARGB32_Premultiplied);
    QPainter p(&img);
    setPenColor(p);
}

void tst_QPainter::setPenColorOnPixmap()
{
    QPixmap pix(10, 10);
    QPainter p(&pix);
    setPenColor(p);
}

class TestProxy : public QGraphicsProxyWidget
{
public:
    TestProxy() : QGraphicsProxyWidget() {}
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
    {
        QGraphicsProxyWidget::paint(painter, option, widget);
        deviceTransform = painter->deviceTransform();
    }
    QTransform deviceTransform;
};

class TestWidget : public QWidget
{
Q_OBJECT
public:
    TestWidget() : QWidget(), painted(false) {}
    void paintEvent(QPaintEvent *)
    {
        QPainter p(this);
        deviceTransform = p.deviceTransform();
        worldTransform = p.worldTransform();
        painted = true;
    }
    QTransform deviceTransform;
    QTransform worldTransform;
    bool painted;
};

void tst_QPainter::QTBUG5939_attachPainterPrivate()
{
    QWidget *w = new QWidget();
    QGraphicsScene *scene = new QGraphicsScene();
    QGraphicsView *view = new QGraphicsView(scene, w);
    view->move(50 ,50);
    TestProxy *proxy = new TestProxy();
    TestWidget *widget = new TestWidget();
    proxy->setWidget(widget);
    scene->addItem(proxy);
    proxy->rotate(45);
    w->resize(scene->sceneRect().size().toSize());

    w->show();
    QTRY_VERIFY(widget->painted);

    QVERIFY(widget->worldTransform.isIdentity());
    QCOMPARE(widget->deviceTransform, proxy->deviceTransform);
}

void tst_QPainter::drawPointScaled()
{
    QImage image(32, 32, QImage::Format_RGB32);
    image.fill(0xffffffff);

    QPainter p(&image);

    p.scale(0.1, 0.1);

    QPen pen;
    pen.setWidth(1000);
    pen.setColor(Qt::red);

    p.setPen(pen);
    p.drawPoint(0, 0);
    p.end();

    QCOMPARE(image.pixel(16, 16), 0xffff0000);
}

QTEST_MAIN(tst_QPainter)

#include "tst_qpainter.moc"
