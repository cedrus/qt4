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


#if defined(QT3_SUPPORT)
#include <q3hbox.h>
#include <q3textedit.h>
#endif
#include <qboxlayout.h>
#include <qapplication.h>
#include <qbitmap.h>
#include <qdebug.h>
#include <qeventloop.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qlineedit.h>
#include <qlistview.h>
#include <qmessagebox.h>
#include <qpainter.h>
#include <qpoint.h>
#include <qpushbutton.h>
#include <qstyle.h>
#include <qwidget.h>
#include <qwindowsstyle.h>
#include <qinputcontext.h>
#include <qdesktopwidget.h>
#include <private/qwidget_p.h>
#include <private/qapplication_p.h>
#include <qcalendarwidget.h>
#include <qmainwindow.h>
#include <qdockwidget.h>
#include <qtoolbar.h>
#include <QtGui/qpaintengine.h>
#include <private/qbackingstore_p.h>

#include <QtGui/QGraphicsView>
#include <QtGui/QGraphicsProxyWidget>

#include "../../shared/util.h"


#ifdef Q_WS_S60
#include <avkon.hrh>                // EEikStatusPaneUidTitle
#include <akntitle.h>               // CAknTitlePane
#include <akncontext.h>             // CAknContextPane
#include <eikspane.h>               // CEikStatusPane
#include <eikbtgpc.h>               // CEikButtonGroupContainer
#endif

#ifdef Q_WS_QWS
# include <qscreen_qws.h>
#endif

// I *MUST* have QtTest afterwards or this test won't work with newer headers
#if defined(Q_WS_MAC)
# include <private/qt_mac_p.h>
#undef verify
#include "tst_qwidget_mac_helpers.h"  // Abstract the ObjC stuff out so not everyone must run an ObjC++ compile.
#endif

#include <QtTest/QtTest>

#if defined(Q_WS_WIN)
#  include <qt_windows.h>
#  if !defined(Q_OS_WINCE)
#define Q_CHECK_PAINTEVENTS \
    if (::SwitchDesktop(::GetThreadDesktop(::GetCurrentThreadId())) == 0) \
        QSKIP("desktop is not visible, this test would fail", SkipSingle);
#  else
#    define Q_CHECK_PAINTEVENTS
#  endif
#elif defined(Q_WS_X11)
#  include <private/qt_x11_p.h>
#  include <qx11info_x11.h>
#elif defined(Q_WS_QWS)
# include <qwindowsystem_qws.h>
#endif

#if !defined(Q_WS_WIN)
#define Q_CHECK_PAINTEVENTS
#endif

#if defined(Q_OS_WINCE_WM)
#include <qguifunctions_wince.h>
// taken from qguifunctions_wce.cpp
#define SPI_GETPLATFORMTYPE 257
bool qt_wince_is_platform(const QString &platformString) {
    wchar_t tszPlatform[64];
    if (SystemParametersInfo(SPI_GETPLATFORMTYPE,
                             sizeof(tszPlatform)/sizeof(*tszPlatform),tszPlatform,0))
      if (0 == _tcsicmp(reinterpret_cast<const wchar_t *> (platformString.utf16()), tszPlatform))
            return true;
    return false;
}
bool qt_wince_is_smartphone() {
       return qt_wince_is_platform(QString::fromLatin1("Smartphone"));
}
#endif

#ifdef Q_WS_MAC
#include <Security/AuthSession.h>
bool macHasAccessToWindowsServer()
{
    SecuritySessionId mySession;
    SessionAttributeBits sessionInfo;
    SessionGetInfo(callerSecuritySession, &mySession, &sessionInfo);
    return (sessionInfo & sessionHasGraphicAccess);
}
#endif


#if defined(Bool)
#undef Bool
#endif

//TESTED_CLASS=
//TESTED_FILES=

class tst_QWidget : public QObject
{
    Q_OBJECT

public:
    tst_QWidget();
    virtual ~tst_QWidget();


public slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();
private slots:
    void getSetCheck();
    void fontPropagation();
    void fontPropagation2();
    void palettePropagation();
    void palettePropagation2();
    void enabledPropagation();
    void acceptDropsPropagation();
    void isEnabledTo();
    void visible();
    void visible_setWindowOpacity();
    void isVisibleTo();
    void isHidden();
    void fonts();
    void mapToGlobal();
    void mapFromAndTo_data();
    void mapFromAndTo();
    void checkFocus();
    void focusChainOnHide();
    void focusChainOnReparent();
    void setTabOrder();
    void activation();
    void reparent();
    void windowState();
    void showMaximized();
    void showFullScreen();
    void showMinimized();
    void showMinimizedKeepsFocus();
    void icon();
    void hideWhenFocusWidgetIsChild();
    void normalGeometry();
    void setGeometry();
    void windowOpacity();
    void raise();
    void lower();
    void stackUnder();
    void testContentsPropagation();
    void saveRestoreGeometry();

    void restoreVersion1Geometry_data();
    void restoreVersion1Geometry();

    void windowTitle();
    void windowModified();
    void windowIconText();

    void widgetAt();
#ifdef Q_WS_MAC
    void retainHIView();
    void sheetOpacity();
    void setMask();
#endif
    void optimizedResizeMove();
    void optimizedResize_topLevel();
    void resizeEvent();
    void task110173();

    void testDeletionInEventHandlers();

    void childDeletesItsSibling();

    void setMinimumSize();
    void setMaximumSize();
    void setFixedSize();

    void ensureCreated();
    void winIdChangeEvent();
#ifdef Q_OS_SYMBIAN
    void reparentCausesChildWinIdChange();
#else
    void persistentWinId();
#endif
    void showNativeChild();
    void qobject_castInDestroyedSlot();

    void showHideEvent_data();
    void showHideEvent();

    void lostUpdatesOnHide();

    void update();
    void isOpaque();

#ifndef Q_WS_MAC
    void scroll();
#endif

    // tests QWidget::setGeometry() on windows only
    void setWindowGeometry_data();
    void setWindowGeometry();

    // tests QWidget::move() and resize() on windows only
    void windowMoveResize_data();
    void windowMoveResize();

    void moveChild_data();
    void moveChild();
    void showAndMoveChild();

    void subtractOpaqueSiblings();

#ifdef Q_WS_WIN
    void getDC();
#ifndef Q_OS_WINCE
    void setGeometry_win();
#endif
#endif

    void setLocale();
    void deleteStyle();
    void multipleToplevelFocusCheck();
    void setFocus();
    void setCursor();
    void setToolTip();
    void testWindowIconChangeEventPropagation();
#ifdef Q_WS_X11
    void minAndMaxSizeWithX11BypassWindowManagerHint();
    void showHideShow();
    void clean_qt_x11_enforce_cursor();
#endif

    void compatibilityChildInsertedEvents();
    void render();
    void renderInvisible();
    void renderWithPainter();
    void render_task188133();
    void render_task211796();
    void render_task217815();
    void render_windowOpacity();
    void render_systemClip();
    void render_systemClip2_data();
    void render_systemClip2();
    void render_systemClip3_data();
    void render_systemClip3();
    void render_task252837();
    void render_worldTransform();

    void setContentsMargins();

    void moveWindowInShowEvent_data();
    void moveWindowInShowEvent();

    void repaintWhenChildDeleted();
    void hideOpaqueChildWhileHidden();
    void updateWhileMinimized();
#if defined(Q_WS_WIN) || defined(Q_WS_X11)
    void alienWidgets();
#endif
    void adjustSize();
    void adjustSize_data();
    void updateGeometry();
    void updateGeometry_data();
    void sendUpdateRequestImmediately();
    void painterRedirection();
    void doubleRepaint();
#ifndef Q_WS_MAC
    void resizeInPaintEvent();
    void opaqueChildren();
#endif

    void setMaskInResizeEvent();
    void moveInResizeEvent();

#if defined(Q_WS_WIN) || defined(Q_WS_X11)
    // We don't support immediate repaint right after show on
    // other platforms. Must be compatible with Qt 4.3.
    void immediateRepaintAfterShow();
    void immediateRepaintAfterInvalidateBuffer();
#endif
    void effectiveWinId();
    void customDpi();
    void customDpiProperty();

    void quitOnCloseAttribute();
    void moveRect();

#if defined (Q_WS_WIN)
    void gdiPainting();
    void paintOnScreenPossible();
#endif
    void reparentStaticWidget();
    void QTBUG6883_reparentStaticWidget2();
#ifdef Q_WS_QWS
    void updateOutsideSurfaceClip();
#endif
    void translucentWidget();

    void setClearAndResizeMask();
    void maskedUpdate();
#if defined(Q_WS_WIN) || defined(Q_WS_X11) || defined(Q_WS_QWS)
    void syntheticEnterLeave();
    void taskQTBUG_4055_sendSyntheticEnterLeave();
#endif
    void windowFlags();
    void initialPosForDontShowOnScreenWidgets();
#ifdef Q_WS_X11
    void paintOutsidePaintEvent();
#endif
    void updateOnDestroyedSignal();
    void toplevelLineEditFocus();
    void inputFocus_task257832();

    void focusWidget_task254563();
    void rectOutsideCoordinatesLimit_task144779();
    void setGraphicsEffect();

    void destroyBackingStore();
    void destroyBackingStoreWhenHidden();

    void activateWindow();

    void openModal_taskQTBUG_5804();

#ifdef Q_OS_SYMBIAN
    void cbaVisibility();
    void fullScreenWindowModeTransitions();
    void maximizedWindowModeTransitions();
    void minimizedWindowModeTransitions();
    void normalWindowModeTransitions();
#endif

    void focusProxyAndInputMethods();
    void scrollWithoutBackingStore();

    void taskQTBUG_7532_tabOrderWithFocusProxy();
    void movedAndResizedAttributes();
    void childAt();
#ifdef Q_WS_MAC
    void childAt_unifiedToolBar();
#ifdef QT_MAC_USE_COCOA
    void taskQTBUG_11373();
#endif // QT_MAC_USE_COCOA
#endif

private:
    bool ensureScreenSize(int width, int height);
    QWidget *testWidget;
};

bool tst_QWidget::ensureScreenSize(int width, int height)
{
    QSize available;
#ifdef Q_WS_QWS
    available = QDesktopWidget().availableGeometry().size();
    if (available.width() < width || available.height() < height) {
        QScreen *screen = QScreen::instance();
        if (!screen)
            return false;
        screen->setMode(width, height, screen->depth());
    }
#endif // Q_WS_QWS

    available = QDesktopWidget().availableGeometry().size();
    return (available.width() >= width && available.height() >= height);
}

class MyInputContext : public QInputContext
{
public:
    MyInputContext() : QInputContext() {}
    QString identifierName() { return QString("NoName"); }
    QString language() { return QString("NoLanguage"); }
    void reset() {}
    bool isComposing() const { return false; }
};

// Testing get/set functions
void tst_QWidget::getSetCheck()
{
    QWidget obj1;
    QWidget child1(&obj1);
    // QStyle * QWidget::style()
    // void QWidget::setStyle(QStyle *)
    QWindowsStyle *var1 = new QWindowsStyle;
    obj1.setStyle(var1);
    QCOMPARE(static_cast<QStyle *>(var1), obj1.style());
    obj1.setStyle((QStyle *)0);
    QVERIFY(var1 != obj1.style());
    QVERIFY(0 != obj1.style()); // style can never be 0 for a widget

    // int QWidget::minimumWidth()
    // void QWidget::setMinimumWidth(int)
    obj1.setMinimumWidth(0);
    QCOMPARE(obj1.minimumWidth(), 0);
    obj1.setMinimumWidth(INT_MIN);
    QCOMPARE(obj1.minimumWidth(), 0); // A widgets width can never be less than 0
    obj1.setMinimumWidth(INT_MAX);
#ifndef Q_WS_QWS  //QWS doesn't allow toplevels to be bigger than the screen
#if defined(Q_CC_MSVC) && !defined(Q_CC_MSVC_NET)
    QCOMPARE((long)obj1.minimumWidth(), QWIDGETSIZE_MAX); // The largest minimum size should only be as big as the maximium
#else
    QCOMPARE(obj1.minimumWidth(), QWIDGETSIZE_MAX); // The largest minimum size should only be as big as the maximium
#endif
#endif

    child1.setMinimumWidth(0);
    QCOMPARE(child1.minimumWidth(), 0);
    child1.setMinimumWidth(INT_MIN);
    QCOMPARE(child1.minimumWidth(), 0); // A widgets width can never be less than 0
    child1.setMinimumWidth(INT_MAX);
#if defined(Q_CC_MSVC) && !defined(Q_CC_MSVC_NET)
    QCOMPARE((long)child1.minimumWidth(), QWIDGETSIZE_MAX); // The largest minimum size should only be as big as the maximium
#else
    QCOMPARE(child1.minimumWidth(), QWIDGETSIZE_MAX); // The largest minimum size should only be as big as the maximium
#endif

    // int QWidget::minimumHeight()
    // void QWidget::setMinimumHeight(int)
    obj1.setMinimumHeight(0);
    QCOMPARE(obj1.minimumHeight(), 0);
    obj1.setMinimumHeight(INT_MIN);
    QCOMPARE(obj1.minimumHeight(), 0); // A widgets height can never be less than 0
    obj1.setMinimumHeight(INT_MAX);
#ifndef Q_WS_QWS    //QWS doesn't allow toplevels to be bigger than the screen
#if defined(Q_CC_MSVC) && !defined(Q_CC_MSVC_NET)
    QCOMPARE((long)obj1.minimumHeight(), QWIDGETSIZE_MAX); // The largest minimum size should only be as big as the maximium
#else
    QCOMPARE(obj1.minimumHeight(), QWIDGETSIZE_MAX); // The largest minimum size should only be as big as the maximium
#endif
#endif

    child1.setMinimumHeight(0);
    QCOMPARE(child1.minimumHeight(), 0);
    child1.setMinimumHeight(INT_MIN);
    QCOMPARE(child1.minimumHeight(), 0); // A widgets height can never be less than 0
    child1.setMinimumHeight(INT_MAX);
#if defined(Q_CC_MSVC) && !defined(Q_CC_MSVC_NET)
    QCOMPARE((long)child1.minimumHeight(), QWIDGETSIZE_MAX); // The largest minimum size should only be as big as the maximium
#else
    QCOMPARE(child1.minimumHeight(), QWIDGETSIZE_MAX); // The largest minimum size should only be as big as the maximium
#endif



// int QWidget::maximumWidth()
    // void QWidget::setMaximumWidth(int)
    obj1.setMaximumWidth(0);
    QCOMPARE(obj1.maximumWidth(), 0);
    obj1.setMaximumWidth(INT_MIN);
    QCOMPARE(obj1.maximumWidth(), 0); // A widgets width can never be less than 0
    obj1.setMaximumWidth(INT_MAX);
#if defined(Q_CC_MSVC) && !defined(Q_CC_MSVC_NET)
    QCOMPARE((long)obj1.maximumWidth(), QWIDGETSIZE_MAX); // QWIDGETSIZE_MAX is the abs max, not INT_MAX
#else
    QCOMPARE(obj1.maximumWidth(), QWIDGETSIZE_MAX); // QWIDGETSIZE_MAX is the abs max, not INT_MAX
#endif

    // int QWidget::maximumHeight()
    // void QWidget::setMaximumHeight(int)
    obj1.setMaximumHeight(0);
    QCOMPARE(obj1.maximumHeight(), 0);
    obj1.setMaximumHeight(INT_MIN);
    QCOMPARE(obj1.maximumHeight(), 0); // A widgets height can never be less than 0
    obj1.setMaximumHeight(INT_MAX);
#if defined(Q_CC_MSVC) && !defined(Q_CC_MSVC_NET)
    QCOMPARE((long)obj1.maximumHeight(), QWIDGETSIZE_MAX); // QWIDGETSIZE_MAX is the abs max, not INT_MAX
#else
    QCOMPARE(obj1.maximumHeight(), QWIDGETSIZE_MAX); // QWIDGETSIZE_MAX is the abs max, not INT_MAX
#endif

    // back to normal
    obj1.setMinimumWidth(0);
    obj1.setMinimumHeight(0);
    obj1.setMaximumWidth(QWIDGETSIZE_MAX);
    obj1.setMaximumHeight(QWIDGETSIZE_MAX);

    // const QPalette & QWidget::palette()
    // void QWidget::setPalette(const QPalette &)
    QPalette var6;
    obj1.setPalette(var6);
    QCOMPARE(var6, obj1.palette());
    obj1.setPalette(QPalette());
    QCOMPARE(QPalette(), obj1.palette());

    // const QFont & QWidget::font()
    // void QWidget::setFont(const QFont &)
    QFont var7;
    obj1.setFont(var7);
    QCOMPARE(var7, obj1.font());
    obj1.setFont(QFont());
    QCOMPARE(QFont(), obj1.font());

    // qreal QWidget::windowOpacity()
    // void QWidget::setWindowOpacity(qreal)
    obj1.setWindowOpacity(0.0);
    QCOMPARE(0.0, obj1.windowOpacity());
    obj1.setWindowOpacity(1.1f);
    QCOMPARE(1.0, obj1.windowOpacity()); // 1.0 is the fullest opacity possible

    // QWidget * QWidget::focusProxy()
    // void QWidget::setFocusProxy(QWidget *)
    QWidget *var9 = new QWidget();
    obj1.setFocusProxy(var9);
    QCOMPARE(var9, obj1.focusProxy());
    obj1.setFocusProxy((QWidget *)0);
    QCOMPARE((QWidget *)0, obj1.focusProxy());
    delete var9;

    // const QRect & QWidget::geometry()
    // void QWidget::setGeometry(const QRect &)
    qApp->processEvents();
    QRect var10(10, 10, 100, 100);
    obj1.setGeometry(var10);
    qApp->processEvents();
    qDebug() << obj1.geometry();
    QCOMPARE(var10, obj1.geometry());
    obj1.setGeometry(QRect(0,0,0,0));
    qDebug() << obj1.geometry();
    QCOMPARE(QRect(0,0,0,0), obj1.geometry());

    // QLayout * QWidget::layout()
    // void QWidget::setLayout(QLayout *)
    QBoxLayout *var11 = new QBoxLayout(QBoxLayout::LeftToRight);
    obj1.setLayout(var11);
    QCOMPARE(static_cast<QLayout *>(var11), obj1.layout());
    obj1.setLayout((QLayout *)0);
    QCOMPARE(static_cast<QLayout *>(var11), obj1.layout()); // You cannot set a 0-pointer layout, that keeps the current
    delete var11; // This will remove the layout from the widget
    QCOMPARE((QLayout *)0, obj1.layout());

    // bool QWidget::acceptDrops()
    // void QWidget::setAcceptDrops(bool)
    obj1.setAcceptDrops(false);
    QCOMPARE(false, obj1.acceptDrops());
    obj1.setAcceptDrops(true);
    QCOMPARE(true, obj1.acceptDrops());

    // QInputContext * QWidget::inputContext()
    // void QWidget::setInputContext(QInputContext *)
    MyInputContext *var13 = new MyInputContext;
    obj1.setInputContext(var13);
    QCOMPARE((QInputContext *)0, obj1.inputContext()); // The widget by default doesn't have the WA_InputMethodEnabled attribute
    obj1.setAttribute(Qt::WA_InputMethodEnabled);
    obj1.setInputContext(var13);
    QCOMPARE(static_cast<QInputContext *>(var13), obj1.inputContext());
    // QWidget takes ownership, so check parent
    QCOMPARE(var13->parent(), static_cast<QObject *>(&obj1));
    // Check self assignment
    obj1.setInputContext(obj1.inputContext());
    QCOMPARE(static_cast<QInputContext *>(var13), obj1.inputContext());
    obj1.setInputContext((QInputContext *)0);
    QCOMPARE(qApp->inputContext(), obj1.inputContext());
    QVERIFY(qApp->inputContext() != var13);

    // bool QWidget::autoFillBackground()
    // void QWidget::setAutoFillBackground(bool)
    obj1.setAutoFillBackground(false);
    QCOMPARE(false, obj1.autoFillBackground());
    obj1.setAutoFillBackground(true);
    QCOMPARE(true, obj1.autoFillBackground());

    delete var1;
#if defined (Q_WS_WIN) && !defined(Q_OS_WINCE)
    obj1.setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint);
    HWND handle = obj1.winId();
    long flags = GetWindowLong(handle, GWL_STYLE);
    QVERIFY(flags & WS_POPUP);
#endif
}

tst_QWidget::tst_QWidget()
{
    QFont font;
    font.setBold(true);
    font.setPointSize(42);
    qApp->setFont(font, "QPropagationTestWidget");

    QPalette palette;
    palette.setColor(QPalette::ToolTipBase, QColor(12, 13, 14));
    palette.setColor(QPalette::Text, QColor(21, 22, 23));
    qApp->setPalette(palette, "QPropagationTestWidget");

    testWidget = 0;
}

tst_QWidget::~tst_QWidget()
{
}

class BezierViewer : public QWidget {
public:
    BezierViewer( QWidget* parent=0, const char* name=0 );
    void paintEvent( QPaintEvent* );
    void setPoints( const QPolygonF& poly );
private:
    QPolygonF points;

};

void tst_QWidget::initTestCase()
{
#ifdef Q_OS_WINCE //disable magic for WindowsCE
    qApp->setAutoMaximizeThreshold(-1);
#endif
  // Create the test class
    testWidget = new BezierViewer( 0, "testObject");
    testWidget->resize(200,200);
#ifdef QT3_SUPPORT
    qApp->setMainWidget(testWidget);
#endif
    testWidget->show();
    QTest::qWaitForWindowShown(testWidget);
    QTest::qWait(50);
}

void tst_QWidget::cleanupTestCase()
{
    delete testWidget;
    testWidget = 0;
}

void tst_QWidget::init()
{
// TODO: Add initialization code here.
// This will be executed immediately before each test is run.
    testWidget->setFont(QFont());
    testWidget->setPalette(QPalette());
}

void tst_QWidget::cleanup()
{
}

// Helper class...

BezierViewer::BezierViewer( QWidget* parent, const char* name )
	: QWidget( parent )
{
    setObjectName(name);
    QPalette pal;
    pal.setColor(backgroundRole(), Qt::white);
    setPalette(pal);
}


void BezierViewer::setPoints( const QPolygonF& a )
{
    points = a;
}

#include "private/qbezier_p.h"
void BezierViewer::paintEvent( QPaintEvent* )
{
    if ( points.size() != 4 ) {
#if defined(QT_CHECK_RANGE)
	qWarning( "QPolygon::bezier: The array must have 4 control points" );
#endif
	return;
    }

    /* Calculate Bezier curve */
    QPolygonF bezier = QBezier::fromPoints(points.at(0),points.at(1),points.at(2),points.at(3)).toPolygon();

    QPainter painter( this );

    /* Calculate scale to fit in window */
    QRectF br = bezier.boundingRect() | points.boundingRect();
    QRectF pr = rect();
    int scl = qMax( qMin(pr.width()/br.width(), pr.height()/br.height()), qreal(1.) );
    int border = scl-1;

    /* Scale Bezier curve vertices */
    for ( QPolygonF::Iterator it = bezier.begin(); it != bezier.end(); ++it ) {
	it->setX( (it->x()-br.x()) * scl + border );
	it->setY( (it->y()-br.y()) * scl + border );
    }

    /* Draw grid */
    painter.setPen( Qt::lightGray );
	int i;
	for ( i = border; i <= pr.width(); i += scl ) {
		painter.drawLine( i, 0, i, pr.height() );
    }
    for ( int j = border; j <= pr.height(); j += scl ) {
	painter.drawLine( 0, j, pr.width(), j );
    }

    /* Write number of vertices */
    painter.setPen( Qt::red );
    painter.setFont( QFont("Helvetica", 14, QFont::DemiBold, TRUE ) );
    QString caption;
    caption.setNum( bezier.size() );
    caption += QString::fromLatin1( " vertices" );
    painter.drawText( 10, pr.height()-10, caption );

    /* Draw Bezier curve */
    painter.setPen( Qt::black );
    painter.drawPolyline( bezier );

    /* Scale and draw control points */
    painter.setPen( Qt::darkGreen );
    for ( QPolygonF::Iterator p1 = points.begin(); p1 != points.end(); ++p1 ) {
	int x = (p1->x()-br.x()) * scl + border;
	int y = (p1->y()-br.y()) * scl + border;
	painter.drawLine( x-4, y-4, x+4, y+4 );
	painter.drawLine( x+4, y-4, x-4, y+4 );
    }

    /* Draw vertices */
    painter.setPen( Qt::red );
    painter.setBrush( Qt::red );
    for ( QPolygonF::Iterator p2 = bezier.begin(); p2 != bezier.end(); ++p2 )
	painter.drawEllipse( p2->x()-1, p2->y()-1, 3, 3 );
}

void tst_QWidget::fontPropagation()
{
    QFont font = testWidget->font();
    QWidget* childWidget = new QWidget( testWidget );
    childWidget->show();
    QCOMPARE( font, childWidget->font() );

    font.setBold( TRUE );
    testWidget->setFont( font );
    QCOMPARE( font, testWidget->font() );
    QCOMPARE( font, childWidget->font() );

    QFont newFont = font;
    newFont.setItalic( TRUE );
    childWidget->setFont( newFont );
    QWidget* grandChildWidget = new QWidget( childWidget );
    QCOMPARE( font, testWidget->font() );
    QCOMPARE( newFont, grandChildWidget->font() );

    font.setUnderline( TRUE );
    testWidget->setFont( font );

    // the child and grand child should now have merged bold and
    // underline
    newFont.setUnderline( TRUE );

    QCOMPARE( newFont, childWidget->font() );
    QCOMPARE( newFont, grandChildWidget->font() );

    // make sure font propagation continues working after reparenting
    font = testWidget->font();
    font.setPointSize(font.pointSize() + 2);
    testWidget->setFont(font);

    QWidget *one   = new QWidget(testWidget);
    QWidget *two   = new QWidget(one);
    QWidget *three = new QWidget(two);
    QWidget *four  = new QWidget(two);

    four->setParent(three);
    four->move(QPoint(0,0));

    font.setPointSize(font.pointSize() + 2);
    testWidget->setFont(font);

    QCOMPARE(testWidget->font(), one->font());
    QCOMPARE(one->font(), two->font());
    QCOMPARE(two->font(), three->font());
    QCOMPARE(three->font(), four->font());

    QVERIFY(testWidget->testAttribute(Qt::WA_SetFont));
    QVERIFY(! one->testAttribute(Qt::WA_SetFont));
    QVERIFY(! two->testAttribute(Qt::WA_SetFont));
    QVERIFY(! three->testAttribute(Qt::WA_SetFont));
    QVERIFY(! four->testAttribute(Qt::WA_SetFont));

    font.setPointSize(font.pointSize() + 2);
    one->setFont(font);

    QCOMPARE(one->font(), two->font());
    QCOMPARE(two->font(), three->font());
    QCOMPARE(three->font(), four->font());

    QVERIFY(one->testAttribute(Qt::WA_SetFont));
    QVERIFY(! two->testAttribute(Qt::WA_SetFont));
    QVERIFY(! three->testAttribute(Qt::WA_SetFont));
    QVERIFY(! four->testAttribute(Qt::WA_SetFont));

    font.setPointSize(font.pointSize() + 2);
    two->setFont(font);

    QCOMPARE(two->font(), three->font());
    QCOMPARE(three->font(), four->font());

    QVERIFY(two->testAttribute(Qt::WA_SetFont));
    QVERIFY(! three->testAttribute(Qt::WA_SetFont));
    QVERIFY(! four->testAttribute(Qt::WA_SetFont));

    font.setPointSize(font.pointSize() + 2);
    three->setFont(font);

    QCOMPARE(three->font(), four->font());

    QVERIFY(three->testAttribute(Qt::WA_SetFont));
    QVERIFY(! four->testAttribute(Qt::WA_SetFont));

    font.setPointSize(font.pointSize() + 2);
    four->setFont(font);

    QVERIFY(four->testAttribute(Qt::WA_SetFont));
}

class QPropagationTestWidget : public QWidget
{
    Q_OBJECT
public:
    QPropagationTestWidget(QWidget *parent = 0)
        : QWidget(parent)
    { }
};

void tst_QWidget::fontPropagation2()
{
    // ! Note, the code below is executed in tst_QWidget's constructor.
    // QFont font;
    // font.setBold(true);
    // font.setPointSize(42);
    // qApp->setFont(font, "QPropagationTestWidget");

    QWidget *root = new QWidget;
    QWidget *child0 = new QWidget(root);
    QWidget *child1 = new QWidget(child0);
    QWidget *child2 = new QPropagationTestWidget(child1);
    QWidget *child3 = new QWidget(child2);
    QWidget *child4 = new QWidget(child3);
    QWidget *child5 = new QWidget(child4);
    root->show();

    // Check that only the application fonts apply.
    QCOMPARE(root->font(), QApplication::font());
    QCOMPARE(child0->font(), QApplication::font());
    QCOMPARE(child1->font(), QApplication::font());
    QCOMPARE(child2->font().pointSize(), 42);
    QVERIFY(child2->font().bold());
    QCOMPARE(child3->font().pointSize(), 42);
    QVERIFY(child3->font().bold());
    QCOMPARE(child4->font().pointSize(), 42);
    QVERIFY(child4->font().bold());
    QCOMPARE(child5->font().pointSize(), 42);
    QVERIFY(child5->font().bold());

    // Set child0's font size to 15, and remove bold on child4.
    QFont font;
    font.setPointSize(15);
    child0->setFont(font);
    QFont unboldFont;
    unboldFont.setBold(false);
    child4->setFont(unboldFont);

    // Check that the above settings propagate correctly.
    QCOMPARE(root->font(), QApplication::font());
    QCOMPARE(child0->font().pointSize(), 15);
    QVERIFY(!child0->font().bold());
    QCOMPARE(child1->font().pointSize(), 15);
    QVERIFY(!child1->font().bold());
    QCOMPARE(child2->font().pointSize(), 15);
    QVERIFY(child2->font().bold());
    QCOMPARE(child3->font().pointSize(), 15);
    QVERIFY(child3->font().bold());
    QCOMPARE(child4->font().pointSize(), 15);
    QVERIFY(!child4->font().bold());
    QCOMPARE(child5->font().pointSize(), 15);
    QVERIFY(!child5->font().bold());

    // Replace the app font for child2. Italic should propagate
    // but the size should still be ignored. The previous bold
    // setting is gone.
    QFont italicSizeFont;
    italicSizeFont.setItalic(true);
    italicSizeFont.setPointSize(33);
    qApp->setFont(italicSizeFont, "QPropagationTestWidget");

    // Check that this propagates correctly.
    QCOMPARE(root->font(), QApplication::font());
    QCOMPARE(child0->font().pointSize(), 15);
    QVERIFY(!child0->font().bold());
    QVERIFY(!child0->font().italic());
    QCOMPARE(child1->font().pointSize(), 15);
    QVERIFY(!child1->font().bold());
    QVERIFY(!child1->font().italic());
    QCOMPARE(child2->font().pointSize(), 15);
    QVERIFY(!child2->font().bold());
    QVERIFY(child2->font().italic());
    QCOMPARE(child3->font().pointSize(), 15);
    QVERIFY(!child3->font().bold());
    QVERIFY(child3->font().italic());
    QCOMPARE(child4->font().pointSize(), 15);
    QVERIFY(!child4->font().bold());
    QVERIFY(child4->font().italic());
    QCOMPARE(child5->font().pointSize(), 15);
    QVERIFY(!child5->font().bold());
    QVERIFY(child5->font().italic());
}

void tst_QWidget::palettePropagation()
{
    QPalette palette = testWidget->palette();
    QWidget* childWidget = new QWidget( testWidget );
    childWidget->show();
    QCOMPARE( palette, childWidget->palette() );

    palette.setColor( QPalette::Base, Qt::red );
    testWidget->setPalette( palette );
    QCOMPARE( palette, testWidget->palette() );
    QCOMPARE( palette, childWidget->palette() );

    QPalette newPalette = palette;
    newPalette.setColor( QPalette::Highlight, Qt::green );
    childWidget->setPalette( newPalette );
    QWidget* grandChildWidget = new QWidget( childWidget );
    QCOMPARE( palette, testWidget->palette() );
    QCOMPARE( newPalette, grandChildWidget->palette() );

    palette.setColor( QPalette::Text, Qt::blue );
    testWidget->setPalette( palette );

    // the child and grand child should now have merged green
    // highlight and blue text
    newPalette.setColor( QPalette::Text, Qt::blue);

    QCOMPARE( newPalette, childWidget->palette() );
    QCOMPARE( newPalette, grandChildWidget->palette() );
}

void tst_QWidget::palettePropagation2()
{
    // ! Note, the code below is executed in tst_QWidget's constructor.
    // QPalette palette;
    // font.setColor(QPalette::ToolTipBase, QColor(12, 13, 14));
    // font.setColor(QPalette::Text, QColor(21, 22, 23));
    // qApp->setPalette(palette, "QPropagationTestWidget");

    QWidget *root = new QWidget;
    QWidget *child0 = new QWidget(root);
    QWidget *child1 = new QWidget(child0);
    QWidget *child2 = new QPropagationTestWidget(child1);
    QWidget *child3 = new QWidget(child2);
    QWidget *child4 = new QWidget(child3);
    QWidget *child5 = new QWidget(child4);
    root->show();
    QTest::qWait(100);

    // These colors are unlikely to be imposed on the default palette of
    // QWidget ;-).
    QColor sysPalText(21, 22, 23);
    QColor sysPalToolTipBase(12, 13, 14);
    QColor overridePalText(42, 43, 44);
    QColor overridePalToolTipBase(45, 46, 47);
    QColor sysPalButton(99, 98, 97);

    // Check that only the application fonts apply.
    QPalette appPal = QApplication::palette();
    QCOMPARE(root->palette(), appPal);
    QCOMPARE(child0->palette(), appPal);
    QCOMPARE(child1->palette(), appPal);
    QCOMPARE(child2->palette().color(QPalette::ToolTipBase), sysPalToolTipBase);
    QCOMPARE(child2->palette().color(QPalette::Text), sysPalText);
    QCOMPARE(child2->palette().color(QPalette::ToolTipText), appPal.color(QPalette::ToolTipText));
    QCOMPARE(child3->palette().color(QPalette::ToolTipBase), sysPalToolTipBase);
    QCOMPARE(child3->palette().color(QPalette::Text), sysPalText);
    QCOMPARE(child3->palette().color(QPalette::ToolTipText), appPal.color(QPalette::ToolTipText));
    QCOMPARE(child4->palette().color(QPalette::ToolTipBase), sysPalToolTipBase);
    QCOMPARE(child4->palette().color(QPalette::Text), sysPalText);
    QCOMPARE(child4->palette().color(QPalette::ToolTipText), appPal.color(QPalette::ToolTipText));
    QCOMPARE(child5->palette().color(QPalette::ToolTipBase), sysPalToolTipBase);
    QCOMPARE(child5->palette().color(QPalette::Text), sysPalText);
    QCOMPARE(child5->palette().color(QPalette::ToolTipText), appPal.color(QPalette::ToolTipText));

    // Set child0's Text, and set ToolTipBase on child4.
    QPalette textPalette;
    textPalette.setColor(QPalette::Text, overridePalText);
    child0->setPalette(textPalette);
    QPalette toolTipPalette;
    toolTipPalette.setColor(QPalette::ToolTipBase, overridePalToolTipBase);
    child4->setPalette(toolTipPalette);

    // Check that the above settings propagate correctly.
    QCOMPARE(root->palette(), appPal);
    QCOMPARE(child0->palette().color(QPalette::Text), overridePalText);
    QCOMPARE(child0->palette().color(QPalette::ToolTipBase), appPal.color(QPalette::ToolTipBase));
    QCOMPARE(child0->palette().color(QPalette::ToolTipText), appPal.color(QPalette::ToolTipText));
    QCOMPARE(child1->palette().color(QPalette::Text), overridePalText);
    QCOMPARE(child1->palette().color(QPalette::ToolTipBase), appPal.color(QPalette::ToolTipBase));
    QCOMPARE(child1->palette().color(QPalette::ToolTipText), appPal.color(QPalette::ToolTipText));
    QCOMPARE(child2->palette().color(QPalette::Text), overridePalText);
    QCOMPARE(child2->palette().color(QPalette::ToolTipBase), sysPalToolTipBase);
    QCOMPARE(child2->palette().color(QPalette::ToolTipText), appPal.color(QPalette::ToolTipText));
    QCOMPARE(child3->palette().color(QPalette::Text), overridePalText);
    QCOMPARE(child3->palette().color(QPalette::ToolTipBase), sysPalToolTipBase);
    QCOMPARE(child3->palette().color(QPalette::ToolTipText), appPal.color(QPalette::ToolTipText));
    QCOMPARE(child4->palette().color(QPalette::Text), overridePalText);
    QCOMPARE(child4->palette().color(QPalette::ToolTipBase), overridePalToolTipBase);
    QCOMPARE(child4->palette().color(QPalette::ToolTipText), appPal.color(QPalette::ToolTipText));
    QCOMPARE(child5->palette().color(QPalette::Text), overridePalText);
    QCOMPARE(child5->palette().color(QPalette::ToolTipBase), overridePalToolTipBase);
    QCOMPARE(child5->palette().color(QPalette::ToolTipText), appPal.color(QPalette::ToolTipText));

    // Replace the app palette for child2. Button should propagate but Text
    // should still be ignored. The previous ToolTipBase setting is gone.
    QPalette buttonPalette;
    buttonPalette.setColor(QPalette::ToolTipText, sysPalButton);
    qApp->setPalette(buttonPalette, "QPropagationTestWidget");

    // Check that the above settings propagate correctly.
    QCOMPARE(root->palette(), appPal);
    QCOMPARE(child0->palette().color(QPalette::Text), overridePalText);
    QCOMPARE(child0->palette().color(QPalette::ToolTipBase), appPal.color(QPalette::ToolTipBase));
    QCOMPARE(child0->palette().color(QPalette::ToolTipText), appPal.color(QPalette::ToolTipText));
    QCOMPARE(child1->palette().color(QPalette::Text), overridePalText);
    QCOMPARE(child1->palette().color(QPalette::ToolTipBase), appPal.color(QPalette::ToolTipBase));
    QCOMPARE(child1->palette().color(QPalette::ToolTipText), appPal.color(QPalette::ToolTipText));
    QCOMPARE(child2->palette().color(QPalette::Text), overridePalText);
    QCOMPARE(child2->palette().color(QPalette::ToolTipBase), appPal.color(QPalette::ToolTipBase));
    QCOMPARE(child2->palette().color(QPalette::ToolTipText), sysPalButton);
    QCOMPARE(child3->palette().color(QPalette::Text), overridePalText);
    QCOMPARE(child3->palette().color(QPalette::ToolTipBase), appPal.color(QPalette::ToolTipBase));
    QCOMPARE(child3->palette().color(QPalette::ToolTipText), sysPalButton);
    QCOMPARE(child4->palette().color(QPalette::Text), overridePalText);
    QCOMPARE(child4->palette().color(QPalette::ToolTipBase), overridePalToolTipBase);
    QCOMPARE(child4->palette().color(QPalette::ToolTipText), sysPalButton);
    QCOMPARE(child5->palette().color(QPalette::Text), overridePalText);
    QCOMPARE(child5->palette().color(QPalette::ToolTipBase), overridePalToolTipBase);
    QCOMPARE(child5->palette().color(QPalette::ToolTipText), sysPalButton);
}

void tst_QWidget::enabledPropagation()
{
    QWidget* childWidget = new QWidget( testWidget );
    childWidget->show();
    QVERIFY( testWidget->isEnabled() );
    QVERIFY( childWidget->isEnabled() );

    testWidget->setEnabled( FALSE );
    QVERIFY( !testWidget->isEnabled() );
    QVERIFY( !childWidget->isEnabled() );

    testWidget->setDisabled( FALSE );
    QVERIFY( testWidget->isEnabled() );
    QVERIFY( childWidget->isEnabled() );

    QWidget* grandChildWidget = new QWidget( childWidget );
    QVERIFY( grandChildWidget->isEnabled() );

    testWidget->setDisabled( TRUE );
    QVERIFY( !testWidget->isEnabled() );
    QVERIFY( !childWidget->isEnabled() );
    QVERIFY( !grandChildWidget->isEnabled() );

    grandChildWidget->setEnabled( FALSE );
    testWidget->setEnabled( TRUE );
    QVERIFY( testWidget->isEnabled() );
    QVERIFY( childWidget->isEnabled() );
    QVERIFY( !grandChildWidget->isEnabled() );

    grandChildWidget->setEnabled( TRUE );
    testWidget->setEnabled( FALSE );
    childWidget->setDisabled( TRUE );
    testWidget->setEnabled( TRUE );
    QVERIFY( testWidget->isEnabled() );
    QVERIFY( !childWidget->isEnabled() );
    QVERIFY( !grandChildWidget->isEnabled() );
}

void tst_QWidget::acceptDropsPropagation()
{
#ifdef QT_NO_DRAGANDDROP
    QSKIP("Drag'n drop disabled in this build", SkipAll);
#else
    QWidget *childWidget = new QWidget(testWidget);
    childWidget->show();
    QVERIFY(!testWidget->acceptDrops());
    QVERIFY(!childWidget->acceptDrops());

    testWidget->setAcceptDrops(true);
    QVERIFY(testWidget->acceptDrops());
    QVERIFY(!childWidget->acceptDrops());
    QVERIFY(childWidget->testAttribute(Qt::WA_DropSiteRegistered));

    testWidget->setAcceptDrops(false);
    QVERIFY(!testWidget->acceptDrops());
    QVERIFY(!childWidget->acceptDrops());
    QVERIFY(!childWidget->testAttribute(Qt::WA_DropSiteRegistered));

    QWidget *grandChildWidget = new QWidget(childWidget);
    QVERIFY(!grandChildWidget->acceptDrops());
    QVERIFY(!grandChildWidget->testAttribute(Qt::WA_DropSiteRegistered));

    testWidget->setAcceptDrops(true);
    QVERIFY(testWidget->acceptDrops());
    QVERIFY(!childWidget->acceptDrops());
    QVERIFY(childWidget->testAttribute(Qt::WA_DropSiteRegistered));
    QVERIFY(!grandChildWidget->acceptDrops());
    QVERIFY(grandChildWidget->testAttribute(Qt::WA_DropSiteRegistered));

    grandChildWidget->setAcceptDrops(true);
    testWidget->setAcceptDrops(false);
    QVERIFY(!testWidget->acceptDrops());
    QVERIFY(!childWidget->acceptDrops());
    QVERIFY(grandChildWidget->acceptDrops());
    QVERIFY(grandChildWidget->testAttribute(Qt::WA_DropSiteRegistered));


    grandChildWidget->setAcceptDrops(false);
    QVERIFY(!grandChildWidget->testAttribute(Qt::WA_DropSiteRegistered));
    testWidget->setAcceptDrops(true);
    childWidget->setAcceptDrops(true);
    testWidget->setAcceptDrops(false);
    QVERIFY(!testWidget->acceptDrops());
    QVERIFY(childWidget->acceptDrops());
    QVERIFY(!grandChildWidget->acceptDrops());
    QVERIFY(grandChildWidget->testAttribute(Qt::WA_DropSiteRegistered));
#endif
}

void tst_QWidget::isEnabledTo()
{
    QWidget* childWidget = new QWidget( testWidget );
    QWidget* grandChildWidget = new QWidget( childWidget );

    QVERIFY( childWidget->isEnabledTo( testWidget ) );
    QVERIFY( grandChildWidget->isEnabledTo( testWidget ) );

    childWidget->setEnabled( FALSE );
    QVERIFY( !childWidget->isEnabledTo( testWidget ) );
    QVERIFY( grandChildWidget->isEnabledTo( childWidget ) );
    QVERIFY( !grandChildWidget->isEnabledTo( testWidget ) );
}

void tst_QWidget::visible()
{
    // Ensure that the testWidget is hidden for this test at the
    // start

    testWidget->hide();
    QVERIFY( !testWidget->isVisible() );
    QWidget* childWidget = new QWidget( testWidget );
    QVERIFY( !childWidget->isVisible() );

    testWidget->show();
    QVERIFY( testWidget->isVisible() );
    QVERIFY( childWidget->isVisible() );

    QWidget* grandChildWidget = new QWidget( childWidget );
    QVERIFY( !grandChildWidget->isVisible() );
    grandChildWidget->show();
    QVERIFY( grandChildWidget->isVisible() );

    grandChildWidget->hide();
    testWidget->hide();
    testWidget->show();
    QVERIFY( !grandChildWidget->isVisible() );
    QVERIFY( testWidget->isVisible() );
    QVERIFY( childWidget->isVisible() );

    grandChildWidget->show();
    childWidget->hide();
    testWidget->hide();
    testWidget->show();
    QVERIFY( testWidget->isVisible() );
    QVERIFY( !childWidget->isVisible() );
    QVERIFY( !grandChildWidget->isVisible() );

    grandChildWidget->show();
    QVERIFY( !grandChildWidget->isVisible() );
}

void tst_QWidget::setLocale()
{
    QWidget w;
    QCOMPARE(w.locale(), QLocale());

    w.setLocale(QLocale::Italian);
    QCOMPARE(w.locale(), QLocale(QLocale::Italian));

    QWidget child1(&w);
    QCOMPARE(child1.locale(), QLocale(QLocale::Italian));

    w.unsetLocale();
    QCOMPARE(w.locale(), QLocale());
    QCOMPARE(child1.locale(), QLocale());

    w.setLocale(QLocale::French);
    QCOMPARE(w.locale(), QLocale(QLocale::French));
    QCOMPARE(child1.locale(), QLocale(QLocale::French));

    child1.setLocale(QLocale::Italian);
    QCOMPARE(w.locale(), QLocale(QLocale::French));
    QCOMPARE(child1.locale(), QLocale(QLocale::Italian));

    child1.unsetLocale();
    QCOMPARE(w.locale(), QLocale(QLocale::French));
    QCOMPARE(child1.locale(), QLocale(QLocale::French));

    QWidget child2;
    QCOMPARE(child2.locale(), QLocale());
    child2.setParent(&w);
    QCOMPARE(child2.locale(), QLocale(QLocale::French));
}

void tst_QWidget::visible_setWindowOpacity()
{
    testWidget->hide();
    QVERIFY( !testWidget->isVisible() );
    testWidget->setWindowOpacity(0.5);
#ifdef Q_OS_WIN
    QVERIFY(::IsWindowVisible(testWidget->winId()) ==  FALSE);
#endif
    testWidget->setWindowOpacity(1.0);
}

void tst_QWidget::isVisibleTo()
{
    // Ensure that the testWidget is hidden for this test at the
    // start

    testWidget->hide();
    QWidget* childWidget = new QWidget( testWidget );
    QVERIFY( childWidget->isVisibleTo( testWidget ) );
    childWidget->hide();
    QVERIFY( !childWidget->isVisibleTo( testWidget ) );

    QWidget* grandChildWidget = new QWidget( childWidget );
    QVERIFY( !grandChildWidget->isVisibleTo( testWidget ) );
    QVERIFY( grandChildWidget->isVisibleTo( childWidget ) );

    testWidget->show();
    childWidget->show();

    QVERIFY( childWidget->isVisibleTo( testWidget ) );
    grandChildWidget->hide();
    QVERIFY( !grandChildWidget->isVisibleTo( childWidget ) );
    QVERIFY( !grandChildWidget->isVisibleTo( testWidget ) );

}

void tst_QWidget::isHidden()
{
    // Ensure that the testWidget is hidden for this test at the
    // start

    testWidget->hide();
    QVERIFY( testWidget->isHidden() );
    QWidget* childWidget = new QWidget( testWidget );
    QVERIFY( !childWidget->isHidden() );

    testWidget->show();
    QVERIFY( !testWidget->isHidden() );
    QVERIFY( !childWidget->isHidden() );

    QWidget* grandChildWidget = new QWidget( childWidget );
    QVERIFY( grandChildWidget->isHidden() );
    grandChildWidget->show();
    QVERIFY( !grandChildWidget->isHidden() );

    grandChildWidget->hide();
    testWidget->hide();
    testWidget->show();
    QVERIFY( grandChildWidget->isHidden() );
    QVERIFY( !testWidget->isHidden() );
    QVERIFY( !childWidget->isHidden() );

    grandChildWidget->show();
    childWidget->hide();
    testWidget->hide();
    testWidget->show();
    QVERIFY( !testWidget->isHidden() );
    QVERIFY( childWidget->isHidden() );
    QVERIFY( !grandChildWidget->isHidden() );

    grandChildWidget->show();
    QVERIFY( !grandChildWidget->isHidden() );
}

void tst_QWidget::fonts()
{
    // Tests setFont(), ownFont() and unsetFont()
    QWidget* cleanTestWidget = new QWidget( testWidget );
    QFont originalFont = cleanTestWidget->font();

    QVERIFY( !cleanTestWidget->testAttribute(Qt::WA_SetFont) );
    cleanTestWidget->setFont(QFont());
    QVERIFY( !cleanTestWidget->testAttribute(Qt::WA_SetFont) );

    QFont newFont( "times", 18 );
    cleanTestWidget->setFont( newFont );
    newFont = newFont.resolve( testWidget->font() );

    QVERIFY( cleanTestWidget->testAttribute(Qt::WA_SetFont) );
    QVERIFY( cleanTestWidget->font() == newFont );

    cleanTestWidget->setFont(QFont());
    QVERIFY( !cleanTestWidget->testAttribute(Qt::WA_SetFont) );
    QVERIFY( cleanTestWidget->font() == originalFont );
}

void tst_QWidget::mapToGlobal()
{
#if !defined(QT3_SUPPORT)
    QSKIP("No Qt3 Support", SkipAll);
#else
    QPoint vis = testWidget->mapToGlobal(QPoint(0,0));
    testWidget->hide();
    QCOMPARE(testWidget->mapToGlobal(QPoint(0,0)), vis);
    testWidget->show();

    // test in a layout and witha move
    Q3HBox * qhb = new Q3HBox(testWidget);
    QWidget * qw = new QWidget(qhb);
    qw->move(6,12);
    QPoint wVis = qw->mapToGlobal(QPoint(0,0));
    qw->hide();
    QCOMPARE(qw->mapToGlobal(QPoint(0,0)), wVis);
    delete qhb;
#endif // QT3_SUPPORT
}

void tst_QWidget::mapFromAndTo_data()
{
    QTest::addColumn<bool>("windowHidden");
    QTest::addColumn<bool>("subWindow1Hidden");
    QTest::addColumn<bool>("subWindow2Hidden");
    QTest::addColumn<bool>("subSubWindowHidden");
    QTest::addColumn<bool>("windowMinimized");
    QTest::addColumn<bool>("subWindow1Minimized");

    QTest::newRow("window 1 sub1 1 sub2 1 subsub 1") << false << false << false << false << false << false;
    QTest::newRow("window 0 sub1 1 sub2 1 subsub 1") << true << false << false << false << false << false;
    QTest::newRow("window 1 sub1 0 sub2 1 subsub 1") << false << true << false << false << false << false;
    QTest::newRow("window 0 sub1 0 sub2 1 subsub 1") << true << true << false << false << false << false;
    QTest::newRow("window 1 sub1 1 sub2 0 subsub 1") << false << false << true << false << false << false;
    QTest::newRow("window 0 sub1 1 sub2 0 subsub 1") << true << false << true << false << false << false;
    QTest::newRow("window 1 sub1 0 sub2 0 subsub 1") << false << true << true << false << false << false;
    QTest::newRow("window 0 sub1 0 sub2 0 subsub 1") << true << true << true << false << false << false;
    QTest::newRow("window 1 sub1 1 sub2 1 subsub 0") << false << false << false << true << false << false;
    QTest::newRow("window 0 sub1 1 sub2 1 subsub 0") << true << false << false << true << false << false;
    QTest::newRow("window 1 sub1 0 sub2 1 subsub 0") << false << true << false << true << false << false;
    QTest::newRow("window 0 sub1 0 sub2 1 subsub 0") << true << true << false << true << false << false;
    QTest::newRow("window 1 sub1 1 sub2 0 subsub 0") << false << false << true << true << false << false;
    QTest::newRow("window 0 sub1 1 sub2 0 subsub 0") << true << false << true << true << false << false;
    QTest::newRow("window 1 sub1 0 sub2 0 subsub 0") << false << true << true << true << false << false;
    QTest::newRow("window 0 sub1 0 sub2 0 subsub 0") << true << true << true << true << false << false;
    QTest::newRow("window 1 sub1 1 sub2 1 subsub 1 windowMinimized") << false << false << false << false << true << false;
    QTest::newRow("window 0 sub1 1 sub2 1 subsub 1 windowMinimized") << true << false << false << false << true << false;
    QTest::newRow("window 1 sub1 0 sub2 1 subsub 1 windowMinimized") << false << true << false << false << true << false;
    QTest::newRow("window 0 sub1 0 sub2 1 subsub 1 windowMinimized") << true << true << false << false << true << false;
    QTest::newRow("window 1 sub1 1 sub2 0 subsub 1 windowMinimized") << false << false << true << false << true << false;
    QTest::newRow("window 0 sub1 1 sub2 0 subsub 1 windowMinimized") << true << false << true << false << true << false;
    QTest::newRow("window 1 sub1 0 sub2 0 subsub 1 windowMinimized") << false << true << true << false << true << false;
    QTest::newRow("window 0 sub1 0 sub2 0 subsub 1 windowMinimized") << true << true << true << false << true << false;
    QTest::newRow("window 1 sub1 1 sub2 1 subsub 0 windowMinimized") << false << false << false << true << true << false;
    QTest::newRow("window 0 sub1 1 sub2 1 subsub 0 windowMinimized") << true << false << false << true << true << false;
    QTest::newRow("window 1 sub1 0 sub2 1 subsub 0 windowMinimized") << false << true << false << true << true << false;
    QTest::newRow("window 0 sub1 0 sub2 1 subsub 0 windowMinimized") << true << true << false << true << true << false;
    QTest::newRow("window 1 sub1 1 sub2 0 subsub 0 windowMinimized") << false << false << true << true << true << false;
    QTest::newRow("window 0 sub1 1 sub2 0 subsub 0 windowMinimized") << true << false << true << true << true << false;
    QTest::newRow("window 1 sub1 0 sub2 0 subsub 0 windowMinimized") << false << true << true << true << true << false;
    QTest::newRow("window 0 sub1 0 sub2 0 subsub 0 windowMinimized") << true << true << true << true << true << false;
    QTest::newRow("window 1 sub1 1 sub2 1 subsub 1 subWindow1Minimized") << false << false << false << false << false << true;
    QTest::newRow("window 0 sub1 1 sub2 1 subsub 1 subWindow1Minimized") << true << false << false << false << false << true;
    QTest::newRow("window 1 sub1 0 sub2 1 subsub 1 subWindow1Minimized") << false << true << false << false << false << true;
    QTest::newRow("window 0 sub1 0 sub2 1 subsub 1 subWindow1Minimized") << true << true << false << false << false << true;
    QTest::newRow("window 1 sub1 1 sub2 0 subsub 1 subWindow1Minimized") << false << false << true << false << false << true;
    QTest::newRow("window 0 sub1 1 sub2 0 subsub 1 subWindow1Minimized") << true << false << true << false << false << true;
    QTest::newRow("window 1 sub1 0 sub2 0 subsub 1 subWindow1Minimized") << false << true << true << false << false << true;
    QTest::newRow("window 0 sub1 0 sub2 0 subsub 1 subWindow1Minimized") << true << true << true << false << false << true;
    QTest::newRow("window 1 sub1 1 sub2 1 subsub 0 subWindow1Minimized") << false << false << false << true << false << true;
    QTest::newRow("window 0 sub1 1 sub2 1 subsub 0 subWindow1Minimized") << true << false << false << true << false << true;
    QTest::newRow("window 1 sub1 0 sub2 1 subsub 0 subWindow1Minimized") << false << true << false << true << false << true;
    QTest::newRow("window 0 sub1 0 sub2 1 subsub 0 subWindow1Minimized") << true << true << false << true << false << true;
    QTest::newRow("window 1 sub1 1 sub2 0 subsub 0 subWindow1Minimized") << false << false << true << true << false << true;
    QTest::newRow("window 0 sub1 1 sub2 0 subsub 0 subWindow1Minimized") << true << false << true << true << false << true;
    QTest::newRow("window 1 sub1 0 sub2 0 subsub 0 subWindow1Minimized") << false << true << true << true << false << true;
    QTest::newRow("window 0 sub1 0 sub2 0 subsub 0 subWindow1Minimized") << true << true << true << true << false << true;


}

void tst_QWidget::mapFromAndTo()
{
    QFETCH(bool, windowHidden);
    QFETCH(bool, subWindow1Hidden);
    QFETCH(bool, subWindow2Hidden);
    QFETCH(bool, subSubWindowHidden);
    QFETCH(bool, windowMinimized);
    QFETCH(bool, subWindow1Minimized);

    // create a toplevel and two overlapping siblings
    QWidget window;
    window.setWindowFlags(window.windowFlags() | Qt::X11BypassWindowManagerHint);
    QWidget *subWindow1 = new QWidget(&window);
    QWidget *subWindow2 = new QWidget(&window);
    QWidget *subSubWindow = new QWidget(subWindow1);

    // set their geometries
    window.setGeometry(100, 100, 100, 100);
    subWindow1->setGeometry(50, 50, 100, 100);
    subWindow2->setGeometry(75, 75, 100, 100);
    subSubWindow->setGeometry(10, 10, 10, 10);

#if !defined (Q_OS_WINCE) && !defined(Q_OS_SYMBIAN) //still no proper minimizing
    //update visibility
    if (windowMinimized) {
        if (!windowHidden) {
            window.showMinimized();
            QVERIFY(window.isMinimized());
        }
    } else {
        window.setVisible(!windowHidden);
    }
    if (subWindow1Minimized) {
        subWindow1->hide();
        subWindow1->showMinimized();
        QVERIFY(subWindow1->isMinimized());
    } else {
        subWindow1->setVisible(!subWindow1Hidden);
    }
#else
    Q_UNUSED(windowHidden);
    Q_UNUSED(subWindow1Hidden);
    Q_UNUSED(windowMinimized);
    Q_UNUSED(subWindow1Minimized);
#endif

    subWindow2->setVisible(!subWindow2Hidden);
    subSubWindow->setVisible(!subSubWindowHidden);

    // window
    QCOMPARE(window.mapToGlobal(QPoint(0, 0)), QPoint(100, 100));
    QCOMPARE(window.mapToGlobal(QPoint(10, 0)), QPoint(110, 100));
    QCOMPARE(window.mapToGlobal(QPoint(0, 10)), QPoint(100, 110));
    QCOMPARE(window.mapToGlobal(QPoint(-10, 0)), QPoint(90, 100));
    QCOMPARE(window.mapToGlobal(QPoint(0, -10)), QPoint(100, 90));
    QCOMPARE(window.mapToGlobal(QPoint(100, 100)), QPoint(200, 200));
    QCOMPARE(window.mapToGlobal(QPoint(110, 100)), QPoint(210, 200));
    QCOMPARE(window.mapToGlobal(QPoint(100, 110)), QPoint(200, 210));
    QCOMPARE(window.mapFromGlobal(QPoint(100, 100)), QPoint(0, 0));
    QCOMPARE(window.mapFromGlobal(QPoint(110, 100)), QPoint(10, 0));
    QCOMPARE(window.mapFromGlobal(QPoint(100, 110)), QPoint(0, 10));
    QCOMPARE(window.mapFromGlobal(QPoint(90, 100)), QPoint(-10, 0));
    QCOMPARE(window.mapFromGlobal(QPoint(100, 90)), QPoint(0, -10));
    QCOMPARE(window.mapFromGlobal(QPoint(200, 200)), QPoint(100, 100));
    QCOMPARE(window.mapFromGlobal(QPoint(210, 200)), QPoint(110, 100));
    QCOMPARE(window.mapFromGlobal(QPoint(200, 210)), QPoint(100, 110));
    QCOMPARE(window.mapToParent(QPoint(0, 0)), QPoint(100, 100));
    QCOMPARE(window.mapToParent(QPoint(10, 0)), QPoint(110, 100));
    QCOMPARE(window.mapToParent(QPoint(0, 10)), QPoint(100, 110));
    QCOMPARE(window.mapToParent(QPoint(-10, 0)), QPoint(90, 100));
    QCOMPARE(window.mapToParent(QPoint(0, -10)), QPoint(100, 90));
    QCOMPARE(window.mapToParent(QPoint(100, 100)), QPoint(200, 200));
    QCOMPARE(window.mapToParent(QPoint(110, 100)), QPoint(210, 200));
    QCOMPARE(window.mapToParent(QPoint(100, 110)), QPoint(200, 210));
    QCOMPARE(window.mapFromParent(QPoint(100, 100)), QPoint(0, 0));
    QCOMPARE(window.mapFromParent(QPoint(110, 100)), QPoint(10, 0));
    QCOMPARE(window.mapFromParent(QPoint(100, 110)), QPoint(0, 10));
    QCOMPARE(window.mapFromParent(QPoint(90, 100)), QPoint(-10, 0));
    QCOMPARE(window.mapFromParent(QPoint(100, 90)), QPoint(0, -10));
    QCOMPARE(window.mapFromParent(QPoint(200, 200)), QPoint(100, 100));
    QCOMPARE(window.mapFromParent(QPoint(210, 200)), QPoint(110, 100));
    QCOMPARE(window.mapFromParent(QPoint(200, 210)), QPoint(100, 110));

    // first subwindow
    QCOMPARE(subWindow1->mapToGlobal(QPoint(0, 0)), QPoint(150, 150));
    QCOMPARE(subWindow1->mapToGlobal(QPoint(10, 0)), QPoint(160, 150));
    QCOMPARE(subWindow1->mapToGlobal(QPoint(0, 10)), QPoint(150, 160));
    QCOMPARE(subWindow1->mapToGlobal(QPoint(-10, 0)), QPoint(140, 150));
    QCOMPARE(subWindow1->mapToGlobal(QPoint(0, -10)), QPoint(150, 140));
    QCOMPARE(subWindow1->mapToGlobal(QPoint(100, 100)), QPoint(250, 250));
    QCOMPARE(subWindow1->mapToGlobal(QPoint(110, 100)), QPoint(260, 250));
    QCOMPARE(subWindow1->mapToGlobal(QPoint(100, 110)), QPoint(250, 260));
    QCOMPARE(subWindow1->mapFromGlobal(QPoint(150, 150)), QPoint(0, 0));
    QCOMPARE(subWindow1->mapFromGlobal(QPoint(160, 150)), QPoint(10, 0));
    QCOMPARE(subWindow1->mapFromGlobal(QPoint(150, 160)), QPoint(0, 10));
    QCOMPARE(subWindow1->mapFromGlobal(QPoint(140, 150)), QPoint(-10, 0));
    QCOMPARE(subWindow1->mapFromGlobal(QPoint(150, 140)), QPoint(0, -10));
    QCOMPARE(subWindow1->mapFromGlobal(QPoint(250, 250)), QPoint(100, 100));
    QCOMPARE(subWindow1->mapFromGlobal(QPoint(260, 250)), QPoint(110, 100));
    QCOMPARE(subWindow1->mapFromGlobal(QPoint(250, 260)), QPoint(100, 110));
    QCOMPARE(subWindow1->mapToParent(QPoint(0, 0)), QPoint(50, 50));
    QCOMPARE(subWindow1->mapToParent(QPoint(10, 0)), QPoint(60, 50));
    QCOMPARE(subWindow1->mapToParent(QPoint(0, 10)), QPoint(50, 60));
    QCOMPARE(subWindow1->mapToParent(QPoint(-10, 0)), QPoint(40, 50));
    QCOMPARE(subWindow1->mapToParent(QPoint(0, -10)), QPoint(50, 40));
    QCOMPARE(subWindow1->mapToParent(QPoint(100, 100)), QPoint(150, 150));
    QCOMPARE(subWindow1->mapToParent(QPoint(110, 100)), QPoint(160, 150));
    QCOMPARE(subWindow1->mapToParent(QPoint(100, 110)), QPoint(150, 160));
    QCOMPARE(subWindow1->mapFromParent(QPoint(50, 50)), QPoint(0, 0));
    QCOMPARE(subWindow1->mapFromParent(QPoint(60, 50)), QPoint(10, 0));
    QCOMPARE(subWindow1->mapFromParent(QPoint(50, 60)), QPoint(0, 10));
    QCOMPARE(subWindow1->mapFromParent(QPoint(40, 50)), QPoint(-10, 0));
    QCOMPARE(subWindow1->mapFromParent(QPoint(50, 40)), QPoint(0, -10));
    QCOMPARE(subWindow1->mapFromParent(QPoint(150, 150)), QPoint(100, 100));
    QCOMPARE(subWindow1->mapFromParent(QPoint(160, 150)), QPoint(110, 100));
    QCOMPARE(subWindow1->mapFromParent(QPoint(150, 160)), QPoint(100, 110));

    // subsubwindow
    QCOMPARE(subSubWindow->mapToGlobal(QPoint(0, 0)), QPoint(160, 160));
    QCOMPARE(subSubWindow->mapToGlobal(QPoint(10, 0)), QPoint(170, 160));
    QCOMPARE(subSubWindow->mapToGlobal(QPoint(0, 10)), QPoint(160, 170));
    QCOMPARE(subSubWindow->mapToGlobal(QPoint(-10, 0)), QPoint(150, 160));
    QCOMPARE(subSubWindow->mapToGlobal(QPoint(0, -10)), QPoint(160, 150));
    QCOMPARE(subSubWindow->mapToGlobal(QPoint(100, 100)), QPoint(260, 260));
    QCOMPARE(subSubWindow->mapToGlobal(QPoint(110, 100)), QPoint(270, 260));
    QCOMPARE(subSubWindow->mapToGlobal(QPoint(100, 110)), QPoint(260, 270));
    QCOMPARE(subSubWindow->mapFromGlobal(QPoint(160, 160)), QPoint(0, 0));
    QCOMPARE(subSubWindow->mapFromGlobal(QPoint(170, 160)), QPoint(10, 0));
    QCOMPARE(subSubWindow->mapFromGlobal(QPoint(160, 170)), QPoint(0, 10));
    QCOMPARE(subSubWindow->mapFromGlobal(QPoint(150, 160)), QPoint(-10, 0));
    QCOMPARE(subSubWindow->mapFromGlobal(QPoint(160, 150)), QPoint(0, -10));
    QCOMPARE(subSubWindow->mapFromGlobal(QPoint(260, 260)), QPoint(100, 100));
    QCOMPARE(subSubWindow->mapFromGlobal(QPoint(270, 260)), QPoint(110, 100));
    QCOMPARE(subSubWindow->mapFromGlobal(QPoint(260, 270)), QPoint(100, 110));
    QCOMPARE(subSubWindow->mapToParent(QPoint(0, 0)), QPoint(10, 10));
    QCOMPARE(subSubWindow->mapToParent(QPoint(10, 0)), QPoint(20, 10));
    QCOMPARE(subSubWindow->mapToParent(QPoint(0, 10)), QPoint(10, 20));
    QCOMPARE(subSubWindow->mapToParent(QPoint(-10, 0)), QPoint(0, 10));
    QCOMPARE(subSubWindow->mapToParent(QPoint(0, -10)), QPoint(10, 0));
    QCOMPARE(subSubWindow->mapToParent(QPoint(100, 100)), QPoint(110, 110));
    QCOMPARE(subSubWindow->mapToParent(QPoint(110, 100)), QPoint(120, 110));
    QCOMPARE(subSubWindow->mapToParent(QPoint(100, 110)), QPoint(110, 120));
    QCOMPARE(subSubWindow->mapFromParent(QPoint(10, 10)), QPoint(0, 0));
    QCOMPARE(subSubWindow->mapFromParent(QPoint(20, 10)), QPoint(10, 0));
    QCOMPARE(subSubWindow->mapFromParent(QPoint(10, 20)), QPoint(0, 10));
    QCOMPARE(subSubWindow->mapFromParent(QPoint(0, 10)), QPoint(-10, 0));
    QCOMPARE(subSubWindow->mapFromParent(QPoint(10, 0)), QPoint(0, -10));
    QCOMPARE(subSubWindow->mapFromParent(QPoint(110, 110)), QPoint(100, 100));
    QCOMPARE(subSubWindow->mapFromParent(QPoint(120, 110)), QPoint(110, 100));
    QCOMPARE(subSubWindow->mapFromParent(QPoint(110, 120)), QPoint(100, 110));
}

void tst_QWidget::focusChainOnReparent()
{
    QWidget window;
    QWidget *child1 = new QWidget(&window);
    QWidget *child2 = new QWidget(&window);
    QWidget *child3 = new QWidget(&window);
    QWidget *child21 = new QWidget(child2);
    QWidget *child22 = new QWidget(child2);
    QWidget *child4 = new QWidget(&window);

    QWidget *expectedOriginalChain[8] = {&window, child1,  child2,  child3,  child21, child22, child4, &window};
    QWidget *w = &window;
    for (int i = 0; i <8; ++i) {
        QCOMPARE(w, expectedOriginalChain[i]);
        w = w->nextInFocusChain();
    }
    for (int i = 7; i >= 0; --i) {
        w = w->previousInFocusChain();
        QCOMPARE(w, expectedOriginalChain[i]);
    }

    QWidget window2;
    child2->setParent(&window2);

    QWidget *expectedNewChain[5] = {&window2, child2,  child21, child22, &window2};
    w = &window2;
    for (int i = 0; i <5; ++i) {
        QCOMPARE(w, expectedNewChain[i]);
        w = w->nextInFocusChain();
    }
    for (int i = 4; i >= 0; --i) {
        w = w->previousInFocusChain();
        QCOMPARE(w, expectedNewChain[i]);
    }

    QWidget *expectedOldChain[5] = {&window, child1,  child3, child4, &window};
    w = &window;
    for (int i = 0; i <5; ++i) {
        QCOMPARE(w, expectedOldChain[i]);
        w = w->nextInFocusChain();
    }
    for (int i = 4; i >= 0; --i) {
        w = w->previousInFocusChain();
        QCOMPARE(w, expectedOldChain[i]);
    }
}


void tst_QWidget::focusChainOnHide()
{
    testWidget->hide(); // We do not want to get disturbed by other widgets
    // focus should move to the next widget in the focus chain when we hide it.
    QWidget *parent = new QWidget();
    parent->setObjectName(QLatin1String("Parent"));
    parent->setFocusPolicy(Qt::StrongFocus);
    QWidget *child = new QWidget(parent);
    child->setObjectName(QLatin1String("child"));
    child->setFocusPolicy(Qt::StrongFocus);
    QWidget::setTabOrder(child, parent);

    parent->show();
    qApp->setActiveWindow(parent->window());
    child->activateWindow();
    child->setFocus();
    qApp->processEvents();

    QTRY_COMPARE(child->hasFocus(), true);
    child->hide();
    qApp->processEvents();

    QTRY_COMPARE(parent->hasFocus(), true);
    QCOMPARE(parent, qApp->focusWidget());

    delete parent;
    testWidget->show(); //don't disturb later tests
}

void tst_QWidget::checkFocus()
{
#if !defined(QT3_SUPPORT)
    QSKIP("No Qt3 Support", SkipAll);
#else
    // This is a very specific test for a specific bug, the bug was
    // that when setEnabled(FALSE) then setEnabled(TRUE) was called on
    // the parent of a child widget which had focus while hidden, then
    // when the widget was shown, the focus would be in the wrong place.

    Q3HBox widget;
    QLineEdit *focusWidget = new QLineEdit( &widget );
    new QLineEdit( &widget );
    new QPushButton( &widget );
    focusWidget->setFocus();
    widget.setEnabled( FALSE );
    widget.setEnabled( TRUE );
    widget.show();
#ifdef Q_WS_X11
    qt_x11_wait_for_window_manager(&widget);
#endif
    QTest::qWait( 100 );
    widget.activateWindow();
    // next call is necessary since the window manager may not give the focus to the widget when
    // it is shown, which causes the QVERIFY to fail
    QApplication::setActiveWindow(&widget);
    QVERIFY( qApp->focusWidget() == focusWidget );
#endif // QT3_SUPPORT
}

class Container : public QWidget
{
public:
    QVBoxLayout* box;

    Container()
    {
        box = new QVBoxLayout(this);
	//(new QVBoxLayout(this))->setAutoAdd(true);
    }

    void tab()
    {
	focusNextPrevChild(TRUE);
    }

    void backTab()
    {
	focusNextPrevChild(FALSE);
    }
};

class Composite : public QFrame
{
public:
    Composite(QWidget* parent = 0, const char* name = 0)
        : QFrame(parent)
    {
        setObjectName(name);
        //QHBoxLayout* hbox = new QHBoxLayout(this, 2, 0);
        //hbox->setAutoAdd(true);
        QHBoxLayout* hbox = new QHBoxLayout(this);

        lineEdit = new QLineEdit(this);
        hbox->addWidget(lineEdit);

        button = new QPushButton(this);
        hbox->addWidget(button);
        button->setFocusPolicy( Qt::NoFocus );

        setFocusProxy( lineEdit );
        setFocusPolicy( Qt::StrongFocus );

	setTabOrder(lineEdit, button);
    }

private:
    QLineEdit* lineEdit;
    QPushButton* button;
};

#define NUM_WIDGETS 4

void tst_QWidget::setTabOrder()
{
    QTest::qWait(100);

    Container container;

    Composite* comp[NUM_WIDGETS];

    QLineEdit *firstEdit = new QLineEdit(&container);
    container.box->addWidget(firstEdit);

    int i = 0;
    for(i = 0; i < NUM_WIDGETS; i++) {
        comp[i] = new Composite(&container);
        container.box->addWidget(comp[i]);
    }

    QLineEdit *lastEdit = new QLineEdit(&container);
    container.box->addWidget(lastEdit);

    container.setTabOrder(lastEdit, comp[NUM_WIDGETS-1]);
    for(i = NUM_WIDGETS-1; i > 0; i--) {
        container.setTabOrder(comp[i], comp[i-1]);
    }
    container.setTabOrder(comp[0], firstEdit);

    int current = NUM_WIDGETS-1;
    lastEdit->setFocus();

    container.show();
    container.activateWindow();
    qApp->setActiveWindow(&container);
#ifdef Q_WS_X11
    QTest::qWaitForWindowShown(&container);
    QTest::qWait(50);
#endif

    QTest::qWait(100);

    QTRY_VERIFY(lastEdit->hasFocus());
    container.tab();
    do {
	QVERIFY(comp[current]->focusProxy()->hasFocus());
	container.tab();
	current--;
    } while (current >= 0);

    QVERIFY(firstEdit->hasFocus());
}

void tst_QWidget::activation()
{
#if !defined(Q_WS_WIN)
    QSKIP("This test is Windows-only.", SkipAll);
#endif
    Q_CHECK_PAINTEVENTS

#if defined(Q_OS_WINCE)
    int waitTime = 1000;
#else
    int waitTime = 100;
#endif

#ifdef Q_OS_WINCE
    qApp->processEvents();
#endif
    QWidget widget1;
    widget1.setWindowTitle("Widget1");

    QWidget widget2;
    widget2.setWindowTitle("Widget2");

    widget1.show();
    widget2.show();

    QTest::qWait(waitTime);
    QVERIFY(qApp->activeWindow() == &widget2);
    widget2.showMinimized();
    QTest::qWait(waitTime);

    QVERIFY(qApp->activeWindow() == &widget1);
    widget2.showMaximized();
    QTest::qWait(waitTime);
    QVERIFY(qApp->activeWindow() == &widget2);
    widget2.showMinimized();
    QTest::qWait(waitTime);
    QVERIFY(qApp->activeWindow() == &widget1);
    widget2.showNormal();
    QTest::qWait(waitTime);
#if defined(Q_WS_WIN) && !defined(Q_OS_WINCE)
    if (QSysInfo::WindowsVersion >= QSysInfo::WV_VISTA)
        QEXPECT_FAIL("", "MS introduced new behavior after XP", Continue);
#endif
    QTest::qWait(waitTime);
    QVERIFY(qApp->activeWindow() == &widget2);
    widget2.hide();
    QTest::qWait(waitTime);
    QVERIFY(qApp->activeWindow() == &widget1);
}

void tst_QWidget::windowState()
{
#ifdef Q_WS_X11
    QSKIP("Many window managers do not support window state properly, which causes this "
         "test to fail.", SkipAll);
#else
#ifdef Q_OS_WINCE_WM
    QPoint pos(500, 500);
    QSize size(200, 200);
    if (qt_wince_is_smartphone()) { //small screen
        pos = QPoint(10,10);
        size = QSize(100,100);
    }
#elif defined(Q_WS_S60)
    QPoint pos = QPoint(10,10);
    QSize size = QSize(100,100);
#else
    const QPoint pos(500, 500);
    const QSize size(200, 200);
#endif

    QWidget widget1;
    widget1.move(pos);
    widget1.resize(size);
    QCOMPARE(widget1.pos(), pos);
    QCOMPARE(widget1.size(), size);
    QTest::qWait(100);
    widget1.setWindowTitle("Widget1");
    QCOMPARE(widget1.pos(), pos);
    QCOMPARE(widget1.size(), size);

#define VERIFY_STATE(s) QCOMPARE(int(widget1.windowState() & stateMask), int(s))

    const int stateMask = Qt::WindowMaximized|Qt::WindowMinimized|Qt::WindowFullScreen;

    widget1.setWindowState(Qt::WindowMaximized);
    QTest::qWait(100);
    VERIFY_STATE(Qt::WindowMaximized);

    widget1.show();
    QTest::qWait(100);
    VERIFY_STATE(Qt::WindowMaximized);

    widget1.setWindowState(widget1.windowState() ^ Qt::WindowMaximized);
    QTest::qWait(100);
    QVERIFY(!(widget1.windowState() & Qt::WindowMaximized));
    QCOMPARE(widget1.pos(), pos);

    widget1.setWindowState(Qt::WindowMinimized);
    QTest::qWait(100);
    VERIFY_STATE(Qt::WindowMinimized);

    widget1.setWindowState(widget1.windowState() | Qt::WindowMaximized);
    QTest::qWait(100);
    VERIFY_STATE((Qt::WindowMinimized|Qt::WindowMaximized));

    widget1.setWindowState(widget1.windowState() ^ Qt::WindowMinimized);
    QTest::qWait(100);
    VERIFY_STATE(Qt::WindowMaximized);

    widget1.setWindowState(widget1.windowState() ^ Qt::WindowMaximized);
    QTest::qWait(100);
    QVERIFY(!(widget1.windowState() & (Qt::WindowMinimized|Qt::WindowMaximized)));
    QCOMPARE(widget1.pos(), pos);

    widget1.setWindowState(Qt::WindowFullScreen);
    QTest::qWait(100);
    VERIFY_STATE(Qt::WindowFullScreen);

    widget1.setWindowState(widget1.windowState() ^ Qt::WindowMinimized);
    QTest::qWait(100);
    VERIFY_STATE((Qt::WindowFullScreen|Qt::WindowMinimized));

    widget1.setWindowState(widget1.windowState() ^ Qt::WindowMinimized);
    QTest::qWait(100);
    VERIFY_STATE(Qt::WindowFullScreen);

    widget1.setWindowState(widget1.windowState() ^ Qt::WindowMaximized);
    QTest::qWait(100);
    VERIFY_STATE((Qt::WindowFullScreen|Qt::WindowMaximized));

    widget1.setWindowState(widget1.windowState() ^ Qt::WindowMinimized);
    QTest::qWait(100);
    VERIFY_STATE((Qt::WindowFullScreen|Qt::WindowMaximized|Qt::WindowMinimized));

    widget1.setWindowState(widget1.windowState() ^ Qt::WindowMinimized);
    QTest::qWait(100);
    VERIFY_STATE((Qt::WindowFullScreen|Qt::WindowMaximized));

    widget1.setWindowState(widget1.windowState() ^ Qt::WindowFullScreen);
    QTest::qWait(100);
    VERIFY_STATE(Qt::WindowMaximized);

    widget1.setWindowState(widget1.windowState() ^ Qt::WindowMaximized);
    QTest::qWait(100);
    QVERIFY(!(widget1.windowState() & stateMask));

    QCOMPARE(widget1.pos(), pos);
    QCOMPARE(widget1.size(), size);
#endif
}

void tst_QWidget::showMaximized()
{
    QWidget plain;
    QHBoxLayout *layout;
    layout = new QHBoxLayout;
    QWidget layouted;
    QLineEdit le;
    QLineEdit le2;
    QLineEdit le3;

    layout->addWidget(&le);
    layout->addWidget(&le2);
    layout->addWidget(&le3);

    layouted.setLayout(layout);

    plain.showMaximized();
    QVERIFY(plain.windowState() & Qt::WindowMaximized);

    plain.showNormal();
    QVERIFY(!(plain.windowState() & Qt::WindowMaximized));

    layouted.showMaximized();
    QVERIFY(layouted.windowState() & Qt::WindowMaximized);

    layouted.showNormal();
    QVERIFY(!(layouted.windowState() & Qt::WindowMaximized));

#if !defined(Q_WS_QWS) && !defined(Q_OS_WINCE) && !defined(Q_WS_S60)
//embedded may choose a different size to fit on the screen.
    QCOMPARE(layouted.size(), layouted.sizeHint());
#endif
    layouted.showMaximized();
    QVERIFY(layouted.isMaximized());
    QVERIFY(layouted.isVisible());

    layouted.hide();
    QVERIFY(layouted.isMaximized());
    QVERIFY(!layouted.isVisible());

    layouted.showMaximized();
    QVERIFY(layouted.isMaximized());
    QVERIFY(layouted.isVisible());

    layouted.showMinimized();
    QVERIFY(layouted.isMinimized());
    QVERIFY(layouted.isMaximized());

    layouted.showMaximized();
    QVERIFY(!layouted.isMinimized());
    QVERIFY(layouted.isMaximized());
    QVERIFY(layouted.isVisible());

    layouted.showMinimized();
    QVERIFY(layouted.isMinimized());
    QVERIFY(layouted.isMaximized());

    layouted.showMaximized();
    QVERIFY(!layouted.isMinimized());
    QVERIFY(layouted.isMaximized());
    QVERIFY(layouted.isVisible());

    {
        QWidget frame;
        QWidget widget(&frame);
        widget.showMaximized();
        QVERIFY(widget.isMaximized());
    }

    {
        QWidget widget;
        widget.setGeometry(0, 0, 10, 10);
        widget.showMaximized();
        QTRY_VERIFY(widget.size().width() > 20 && widget.size().height() > 20);
    }

#ifdef QT3_SUPPORT
#if !defined(Q_WS_QWS)
//embedded respects max/min sizes by design -- maybe wrong design, but that's the way it is now.
    {
        Q3HBox box;
        QWidget widget(&box);
        widget.setMinimumSize(500, 500);
        box.showMaximized();
        QVERIFY(box.isMaximized());
    }

    {
        Q3HBox box;
        QWidget widget(&box);
        widget.setMaximumSize(500, 500);

        box.showMaximized();
        QVERIFY(box.isMaximized());
    }
#endif
#endif // QT3_SUPPORT
}

void tst_QWidget::showFullScreen()
{
    QWidget plain;
    QHBoxLayout *layout;
    QWidget layouted;
    QLineEdit le;
    QLineEdit le2;
    QLineEdit le3;
    layout = new QHBoxLayout;

    layout->addWidget(&le);
    layout->addWidget(&le2);
    layout->addWidget(&le3);

    layouted.setLayout(layout);

    plain.showFullScreen();
    QVERIFY(plain.windowState() & Qt::WindowFullScreen);

    plain.showNormal();
    QVERIFY(!(plain.windowState() & Qt::WindowFullScreen));

    layouted.showFullScreen();
    QVERIFY(layouted.windowState() & Qt::WindowFullScreen);

    layouted.showNormal();
    QVERIFY(!(layouted.windowState() & Qt::WindowFullScreen));

#if !defined(Q_WS_QWS) && !defined(Q_OS_WINCE) && !defined (Q_WS_S60)
//embedded may choose a different size to fit on the screen.
    QCOMPARE(layouted.size(), layouted.sizeHint());
#endif

    layouted.showFullScreen();
    QVERIFY(layouted.isFullScreen());
    QVERIFY(layouted.isVisible());

    layouted.hide();
    QVERIFY(layouted.isFullScreen());
    QVERIFY(!layouted.isVisible());

    layouted.showFullScreen();
    QVERIFY(layouted.isFullScreen());
    QVERIFY(layouted.isVisible());

    layouted.showMinimized();
    QVERIFY(layouted.isMinimized());
    QVERIFY(layouted.isFullScreen());

    layouted.showFullScreen();
    QVERIFY(!layouted.isMinimized());
    QVERIFY(layouted.isFullScreen());
    QVERIFY(layouted.isVisible());

    layouted.showMinimized();
    QVERIFY(layouted.isMinimized());
    QVERIFY(layouted.isFullScreen());

    layouted.showFullScreen();
    QVERIFY(!layouted.isMinimized());
    QVERIFY(layouted.isFullScreen());
    QVERIFY(layouted.isVisible());

    {
        QWidget frame;
        QWidget widget(&frame);
        widget.showFullScreen();
        QVERIFY(widget.isFullScreen());
    }

#ifdef QT3_SUPPORT
#if !defined(Q_WS_QWS)
//embedded respects max/min sizes by design -- maybe wrong design, but that's the way it is now.
    {
        Q3HBox box;
        QWidget widget(&box);
        widget.setMinimumSize(500, 500);
        box.showFullScreen();
        QVERIFY(box.isFullScreen());
    }

    {
        Q3HBox box;
        QWidget widget(&box);
        widget.setMaximumSize(500, 500);

        box.showFullScreen();
        QVERIFY(box.isFullScreen());
    }
#endif
#endif // QT3_SUPPORT
}

class ResizeWidget : public QWidget {
public:
    ResizeWidget(QWidget *p = 0) : QWidget(p)
    {
        m_resizeEventCount = 0;
    }
protected:
    void resizeEvent(QResizeEvent *e){
        QCOMPARE(size(), e->size());
        ++m_resizeEventCount;
    }

public:
    int m_resizeEventCount;
};

void tst_QWidget::resizeEvent()
{
    {
        QWidget wParent;
        ResizeWidget wChild(&wParent);
        wParent.show();
        QCOMPARE (wChild.m_resizeEventCount, 1); // initial resize event before paint
        wParent.hide();
        wChild.resize(QSize(640,480));
        QCOMPARE (wChild.m_resizeEventCount, 1);
        wParent.show();
        QCOMPARE (wChild.m_resizeEventCount, 2);
    }

    {
        ResizeWidget wTopLevel;
        wTopLevel.show();
        QCOMPARE (wTopLevel.m_resizeEventCount, 1); // initial resize event before paint for toplevels
        wTopLevel.hide();
        wTopLevel.resize(QSize(640,480));
        QCOMPARE (wTopLevel.m_resizeEventCount, 1);
        wTopLevel.show();
        QCOMPARE (wTopLevel.m_resizeEventCount, 2);
    }
}

void tst_QWidget::showMinimized()
{
    QWidget plain;
    plain.move(100, 100);
    plain.resize(200, 200);
    QPoint pos = plain.pos();

    plain.showMinimized();
    QVERIFY(plain.isMinimized());
    QVERIFY(plain.isVisible());
    QCOMPARE(plain.pos(), pos);

    plain.showNormal();
    QVERIFY(!plain.isMinimized());
    QVERIFY(plain.isVisible());
    QCOMPARE(plain.pos(), pos);

    plain.showMinimized();
    QVERIFY(plain.isMinimized());
    QVERIFY(plain.isVisible());
    QCOMPARE(plain.pos(), pos);

    plain.hide();
    QVERIFY(plain.isMinimized());
    QVERIFY(!plain.isVisible());

    plain.showMinimized();
    QVERIFY(plain.isMinimized());
    QVERIFY(plain.isVisible());

    plain.setGeometry(200, 200, 300, 300);
    plain.showNormal();
    QCOMPARE(plain.geometry(), QRect(200, 200, 300, 300));

    {
        QWidget frame;
        QWidget widget(&frame);
        widget.showMinimized();
        QVERIFY(widget.isMinimized());
    }
}

void tst_QWidget::showMinimizedKeepsFocus()
{
    //here we test that minimizing a widget and restoring it doesn't change the focus inside of it
    {
        QWidget window;
        QWidget child1(&window), child2(&window);
        child1.setFocusPolicy(Qt::StrongFocus);
        child2.setFocusPolicy(Qt::StrongFocus);
        window.show();
        qApp->setActiveWindow(&window);
        QTest::qWaitForWindowShown(&window);
        child2.setFocus();
        QTest::qWait(50);

        QTRY_COMPARE(window.focusWidget(), &child2);
        QTRY_COMPARE(qApp->focusWidget(), &child2);

        window.showMinimized();
        QTest::qWait(10);
        QTRY_VERIFY(window.isMinimized());
        QTRY_COMPARE(window.focusWidget(), &child2);

        window.showNormal();
        QTest::qWait(10);
        QTRY_COMPARE(window.focusWidget(), &child2);
    }

    //testing deletion of the focusWidget
    {
        QWidget window;
        QWidget *child = new QWidget(&window);
        child->setFocusPolicy(Qt::StrongFocus);
        window.show();
        qApp->setActiveWindow(&window);
        QTest::qWaitForWindowShown(&window);
        child->setFocus();
        QTest::qWait(50);
        QTRY_COMPARE(window.focusWidget(), child);
        QTRY_COMPARE(qApp->focusWidget(), child);

        delete child;
        QCOMPARE(window.focusWidget(), static_cast<QWidget*>(0));
        QCOMPARE(qApp->focusWidget(), static_cast<QWidget*>(0));
    }

    //testing reparenting the focus widget
    {
        QWidget window;
        QWidget *child = new QWidget(&window);
        child->setFocusPolicy(Qt::StrongFocus);
        window.show();
        qApp->setActiveWindow(&window);
        QTest::qWaitForWindowShown(&window);
        child->setFocus();
        QTest::qWait(50);
        QTRY_COMPARE(window.focusWidget(), child);
        QTRY_COMPARE(qApp->focusWidget(), child);

        child->setParent(0);
        QCOMPARE(window.focusWidget(), static_cast<QWidget*>(0));
        QCOMPARE(qApp->focusWidget(), static_cast<QWidget*>(0));
    }

    //testing setEnabled(false)
    {
        QWidget window;
        QWidget *child = new QWidget(&window);
        child->setFocusPolicy(Qt::StrongFocus);
        window.show();
        qApp->setActiveWindow(&window);
        QTest::qWaitForWindowShown(&window);
        child->setFocus();
        QTest::qWait(10);
        QTRY_COMPARE(window.focusWidget(), child);
        QTRY_COMPARE(qApp->focusWidget(), child);

        child->setEnabled(false);
        QCOMPARE(window.focusWidget(), static_cast<QWidget*>(0));
        QCOMPARE(qApp->focusWidget(), static_cast<QWidget*>(0));
    }

    //testing clearFocus
    {
        QWidget window;
        QWidget *firstchild = new QWidget(&window);
        firstchild->setFocusPolicy(Qt::StrongFocus);
        QWidget *child = new QWidget(&window);
        child->setFocusPolicy(Qt::StrongFocus);
        window.show();
        qApp->setActiveWindow(&window);
        QTest::qWaitForWindowShown(&window);
        child->setFocus();
        QTest::qWait(10);
        QTRY_COMPARE(window.focusWidget(), child);
        QTRY_COMPARE(qApp->focusWidget(), child);

        child->clearFocus();
        QCOMPARE(window.focusWidget(), static_cast<QWidget*>(0));
        QCOMPARE(qApp->focusWidget(), static_cast<QWidget*>(0));

        window.showMinimized();
        QTest::qWait(30);
        QTRY_VERIFY(window.isMinimized());
#ifdef Q_WS_QWS
        QEXPECT_FAIL("", "QWS does not implement showMinimized()", Continue);
#endif
        QCOMPARE(window.focusWidget(), static_cast<QWidget*>(0));
#ifdef Q_WS_QWS
        QEXPECT_FAIL("", "QWS does not implement showMinimized()", Continue);
#endif
        QTRY_COMPARE(qApp->focusWidget(), static_cast<QWidget*>(0));

        window.showNormal();
        qApp->setActiveWindow(&window);
        QTest::qWaitForWindowShown(&window);
        QTest::qWait(30);
#ifdef Q_WS_MAC
        if (!macHasAccessToWindowsServer())
            QEXPECT_FAIL("", "When not having WindowServer access, we lose focus.", Continue);
#endif
        QTRY_COMPARE(window.focusWidget(), firstchild);
#ifdef Q_WS_MAC
        if (!macHasAccessToWindowsServer())
            QEXPECT_FAIL("", "When not having WindowServer access, we lose focus.", Continue);
#endif
        QTRY_COMPARE(qApp->focusWidget(), firstchild);
    }
}


void tst_QWidget::reparent()
{
    QWidget parent;
    parent.setWindowTitle("Toplevel");
    parent.setGeometry(300, 300, 200, 150);

    QWidget child(0);
    child.setObjectName("child");
    child.setGeometry(10, 10, 180, 130);
    QPalette pal1;
    pal1.setColor(child.backgroundRole(), Qt::white);
    child.setPalette(pal1);

    QWidget childTLW(&child, Qt::Window);
    childTLW.setObjectName("childTLW");
    childTLW.setGeometry(100, 100, 50, 50);
    QPalette pal2;
    pal2.setColor(childTLW.backgroundRole(), Qt::yellow);
    childTLW.setPalette(pal2);

    parent.show();
    childTLW.show();
    QTest::qWaitForWindowShown(&parent);

#ifdef Q_OS_WINCE
    parent.move(50, 50);
#else
    parent.move(300, 300);
#endif

    QPoint childPos = parent.mapToGlobal(child.pos());
    QPoint tlwPos = childTLW.pos();

    child.setParent(0, child.windowFlags() & ~Qt::WindowType_Mask);
    child.setGeometry(childPos.x(), childPos.y(), child.width(), child.height());
    child.show();

#ifdef Q_WS_X11
    // On X11, the window manager will apply NorthWestGravity rules to 'child', which
    // means the top-left corner of the window frame will be placed at 'childPos',
    // causing this test to fail
#else
    QCOMPARE(child.geometry().topLeft(), childPos);
#endif
    QTRY_COMPARE(childTLW.pos(), tlwPos);

#ifdef Q_WS_WIN
    QWidget childTLWChild(&childTLW);
    childTLWChild.setObjectName("childTLWChild");

    QWidget grandChild(&child);
    grandChild.setObjectName("grandChild");
    grandChild.setGeometry(10, 10, 160, 110);
    QPalette pal3;
    pal3.setColor(grandChild.backgroundRole(), Qt::red);
    grandChild.setPalette(pal3);
    //grandChild.setPaletteBackgroundColor(Qt::red);

    QWidget grandChildTLW(&grandChild, Qt::Window);
    grandChildTLW.setObjectName("grandChildTLW");
    grandChildTLW.setGeometry(200, 200, 50, 50);
    QPalette pal4;
    pal4.setColor(grandChildTLW.backgroundRole(), Qt::yellow);
    grandChildTLW.setPalette(pal4);
    //grandChildTLW.setPaletteBackgroundColor(Qt::yellow);

    QWidget grandChildTLWChild(&grandChildTLW);
    grandChildTLWChild.setObjectName("grandChildTLWChild");

    QVERIFY(IsWindow(childTLW.winId()));
    QVERIFY(IsWindow(childTLWChild.winId()));
    QVERIFY(IsWindow(grandChildTLW.winId()));
    QVERIFY(IsWindow(grandChildTLWChild.winId()));

    parent.show();

    QVERIFY(IsWindow(childTLW.winId()));
    QVERIFY(IsWindow(childTLWChild.winId()));
    QVERIFY(IsWindow(grandChildTLW.winId()));
    QVERIFY(IsWindow(grandChildTLWChild.winId()));

    child.setParent(&parent);
    child.move(10,10);
    child.show();

    // this appears to stabelize results
    qApp->processEvents();

    QVERIFY(IsWindow(childTLW.winId()));
    QVERIFY(IsWindow(childTLWChild.winId()));

    QVERIFY(IsWindow(grandChildTLW.winId()));
    QVERIFY(IsWindow(grandChildTLWChild.winId()));
#else
    QSKIP("This test makes only sense on Windows", SkipAll);
#endif
}

void tst_QWidget::icon()
{
#if defined(Q_WS_QWS)
    QSKIP("Qt/Embedded does it differently", SkipAll);
#else
    QPixmap p(20,20);
    p.fill(Qt::red);
    testWidget->setWindowIcon(p);

    QVERIFY(!testWidget->windowIcon().isNull());
    testWidget->show();
    QVERIFY(!testWidget->windowIcon().isNull());
    testWidget->showFullScreen();
    QVERIFY(!testWidget->windowIcon().isNull());
    testWidget->showNormal();
    QVERIFY(!testWidget->windowIcon().isNull());
#endif
}

void tst_QWidget::hideWhenFocusWidgetIsChild()
{
    testWidget->activateWindow();
    QWidget *parentWidget = new QWidget(testWidget);
    parentWidget->setObjectName("parentWidget");
    parentWidget->setGeometry(0, 0, 100, 100);
    QLineEdit *edit = new QLineEdit(parentWidget);
    edit->setObjectName("edit1");
    QLineEdit *edit3 = new QLineEdit(parentWidget);
    edit3->setObjectName("edit3");
    edit3->move(0,50);
    parentWidget->show();
    QLineEdit *edit2 = new QLineEdit(testWidget);
    edit2->setObjectName("edit2");
    edit2->show();
    edit2->move(110, 100);
    edit->setFocus();
    qApp->processEvents();
    QString actualFocusWidget, expectedFocusWidget;
#ifdef Q_WS_X11
    if (!qApp->focusWidget())
        QSKIP("Your window manager is too broken for this test", SkipAll);
#endif
    QVERIFY(qApp->focusWidget());
    actualFocusWidget.sprintf("%p %s %s", qApp->focusWidget(), qApp->focusWidget()->objectName().toLatin1().constData(), qApp->focusWidget()->metaObject()->className());
    expectedFocusWidget.sprintf("%p %s %s", edit, edit->objectName().toLatin1().constData(), edit->metaObject()->className());
    QCOMPARE(actualFocusWidget, expectedFocusWidget);

    parentWidget->hide();
    qApp->processEvents();
    actualFocusWidget.sprintf("%p %s %s", qApp->focusWidget(), qApp->focusWidget()->objectName().toLatin1().constData(), qApp->focusWidget()->metaObject()->className());
    expectedFocusWidget.sprintf("%p %s %s", edit2, edit2->objectName().toLatin1().constData(), edit2->metaObject()->className());
    QCOMPARE(actualFocusWidget, expectedFocusWidget);

    delete edit2;
    delete parentWidget;
}

void tst_QWidget::normalGeometry()
{
#ifdef Q_OS_IRIX
    QSKIP("4DWM issues on IRIX makes this test fail", SkipAll);
#endif
    QWidget parent;
    parent.setWindowTitle("NormalGeometry parent");
    QWidget *child = new QWidget(&parent);

    QCOMPARE(parent.normalGeometry(), parent.geometry());
    QCOMPARE(child->normalGeometry(), QRect());

    parent.setGeometry(100, 100, 200, 200);
    parent.show();
    QTest::qWaitForWindowShown(&parent);
    QApplication::processEvents();

    QRect geom = parent.geometry();
    // ### the window manager places the top-left corner at
    // ### 100,100... making geom something like 102,124 (offset by
    // ### the frame/frame)... this indicates a rather large different
    // ### between how X11 and Windows works
    // QCOMPARE(geom, QRect(100, 100, 200, 200));
    QCOMPARE(parent.normalGeometry(), geom);

    parent.setWindowState(parent.windowState() ^ Qt::WindowMaximized);
    QTest::qWait(10);
    QTRY_VERIFY(parent.windowState() & Qt::WindowMaximized);
    QTRY_VERIFY(parent.geometry() != geom);
    QTRY_COMPARE(parent.normalGeometry(), geom);

    parent.setWindowState(parent.windowState() ^ Qt::WindowMaximized);
    QTest::qWait(10);
    QTRY_VERIFY(!(parent.windowState() & Qt::WindowMaximized));
    QTRY_COMPARE(parent.geometry(), geom);
    QTRY_COMPARE(parent.normalGeometry(), geom);

    parent.showMaximized();
    QTest::qWait(10);
    QTRY_VERIFY(parent.windowState() & Qt::WindowMaximized);
    QTRY_VERIFY(parent.geometry() != geom);
    QCOMPARE(parent.normalGeometry(), geom);

    parent.showNormal();
    QTest::qWait(10);
    QTRY_VERIFY(!(parent.windowState() & Qt::WindowMaximized));
    QTRY_COMPARE(parent.geometry(), geom);
    QCOMPARE(parent.normalGeometry(), geom);

    parent.setWindowState(parent.windowState() ^ Qt::WindowMinimized);
    QTest::qWait(10);
    parent.setWindowState(parent.windowState() ^ Qt::WindowMaximized);
    QTest::qWait(10);
    QTRY_VERIFY(parent.windowState() & (Qt::WindowMinimized|Qt::WindowMaximized));
    // ### when minimized and maximized at the same time, the geometry
    // ### does *NOT* have to be the normal geometry, it could be the
    // ### maximized geometry.
    // QCOMPARE(parent.geometry(), geom);
    QTRY_COMPARE(parent.normalGeometry(), geom);

    parent.setWindowState(parent.windowState() ^ Qt::WindowMinimized);
    QTest::qWait(10);
    QTRY_VERIFY(!(parent.windowState() & Qt::WindowMinimized));
    QTRY_VERIFY(parent.windowState() & Qt::WindowMaximized);
    QTRY_VERIFY(parent.geometry() != geom);
    QTRY_COMPARE(parent.normalGeometry(), geom);

    parent.setWindowState(parent.windowState() ^ Qt::WindowMaximized);
    QTest::qWait(10);
    QTRY_VERIFY(!(parent.windowState() & Qt::WindowMaximized));
    QTRY_COMPARE(parent.geometry(), geom);
    QTRY_COMPARE(parent.normalGeometry(), geom);

    parent.setWindowState(parent.windowState() ^ Qt::WindowFullScreen);
    QTest::qWait(10);
    QTRY_VERIFY(parent.windowState() & Qt::WindowFullScreen);
    QTRY_VERIFY(parent.geometry() != geom);
    QTRY_COMPARE(parent.normalGeometry(), geom);

    parent.setWindowState(parent.windowState() ^ Qt::WindowFullScreen);
    QTest::qWait(10);
    QVERIFY(!(parent.windowState() & Qt::WindowFullScreen));
    QTRY_COMPARE(parent.geometry(), geom);
    QTRY_COMPARE(parent.normalGeometry(), geom);

    parent.showFullScreen();
    QTest::qWait(10);
    QTRY_VERIFY(parent.windowState() & Qt::WindowFullScreen);
    QTRY_VERIFY(parent.geometry() != geom);
    QTRY_COMPARE(parent.normalGeometry(), geom);

    parent.showNormal();
    QTest::qWait(10);
    QTRY_VERIFY(!(parent.windowState() & Qt::WindowFullScreen));
    QTRY_COMPARE(parent.geometry(), geom);
    QTRY_COMPARE(parent.normalGeometry(), geom);

    parent.showNormal();
    parent.setWindowState(Qt:: WindowFullScreen | Qt::WindowMaximized);
    parent.setWindowState(Qt::WindowMinimized | Qt:: WindowFullScreen | Qt::WindowMaximized);
    parent.setWindowState(Qt:: WindowFullScreen | Qt::WindowMaximized);
    QTest::qWait(10);
    QTRY_COMPARE(parent.normalGeometry(), geom);
}


void tst_QWidget::setGeometry()
{
    QWidget tlw;
    QWidget child(&tlw);

    QRect tr(100,100,200,200);
    QRect cr(50,50,50,50);
    tlw.setGeometry(tr);
    child.setGeometry(cr);
    tlw.show();
    QTest::qWait(50);
    QCOMPARE(tlw.geometry().size(), tr.size());
    QCOMPARE(child.geometry(), cr);

    tlw.setParent(0, Qt::Window|Qt::FramelessWindowHint);
    tr = QRect(0,0,100,100);
    tr.moveTopLeft(QApplication::desktop()->availableGeometry().topLeft());
    tlw.setGeometry(tr);
    QCOMPARE(tlw.geometry(), tr);
    tlw.show();
    QTest::qWait(50);
    if (tlw.frameGeometry() != tlw.geometry())
        QSKIP("Your window manager is too broken for this test", SkipAll);
    QCOMPARE(tlw.geometry(), tr);

}

void tst_QWidget::windowOpacity()
{
#ifdef Q_OS_WINCE
    QSKIP( "Windows CE does not support windowOpacity", SkipAll);
#endif
    QWidget widget;
    QWidget child(&widget);

    // Initial value should be 1.0
    QCOMPARE(widget.windowOpacity(), 1.0);
    // children should always return 1.0
    QCOMPARE(child.windowOpacity(), 1.0);

    widget.setWindowOpacity(0.0);
    QCOMPARE(widget.windowOpacity(), 0.0);
    child.setWindowOpacity(0.0);
    QCOMPARE(child.windowOpacity(), 1.0);

    widget.setWindowOpacity(1.0);
    QCOMPARE(widget.windowOpacity(), 1.0);
    child.setWindowOpacity(1.0);
    QCOMPARE(child.windowOpacity(), 1.0);

    widget.setWindowOpacity(2.0);
    QCOMPARE(widget.windowOpacity(), 1.0);
    child.setWindowOpacity(2.0);
    QCOMPARE(child.windowOpacity(), 1.0);

    widget.setWindowOpacity(-1.0);
    QCOMPARE(widget.windowOpacity(), 0.0);
    child.setWindowOpacity(-1.0);
    QCOMPARE(child.windowOpacity(), 1.0);
}

class UpdateWidget : public QWidget
{
public:
    UpdateWidget(QWidget *parent = 0) : QWidget(parent) {
        reset();
    }

    void paintEvent(QPaintEvent *e) {
        paintedRegion += e->region();
        ++numPaintEvents;
        if (resizeInPaintEvent) {
            resizeInPaintEvent = false;
            resize(size() + QSize(2, 2));
        }
    }

    bool event(QEvent *event)
    {
        switch (event->type()) {
        case QEvent::ZOrderChange:
            ++numZOrderChangeEvents;
            break;
        case QEvent::UpdateRequest:
            ++numUpdateRequestEvents;
            break;
        case QEvent::ActivationChange:
        case QEvent::FocusIn:
        case QEvent::FocusOut:
        case QEvent::WindowActivate:
        case QEvent::WindowDeactivate:
            if (!updateOnActivationChangeAndFocusIn)
                return true; // Filter out to avoid update() calls in QWidget.
            break;
        default:
            break;
        }
        return QWidget::event(event);
    }

    void reset() {
        numPaintEvents = 0;
        numZOrderChangeEvents = 0;
        numUpdateRequestEvents = 0;
        updateOnActivationChangeAndFocusIn = false;
        resizeInPaintEvent = false;
        paintedRegion = QRegion();
    }

    int numPaintEvents;
    int numZOrderChangeEvents;
    int numUpdateRequestEvents;
    bool updateOnActivationChangeAndFocusIn;
    bool resizeInPaintEvent;
    QRegion paintedRegion;
};

void tst_QWidget::lostUpdatesOnHide()
{
#ifndef Q_WS_MAC
    UpdateWidget widget;
    widget.setAttribute(Qt::WA_DontShowOnScreen);
    widget.show();
    widget.hide();
    QTest::qWait(50);
    widget.show();
    QTest::qWait(50);

    QCOMPARE(widget.numPaintEvents, 1);
#endif
}

void tst_QWidget::raise()
{
#ifdef QT_MAC_USE_COCOA
    QSKIP("Cocoa has no Z-Order for views, we hack it, but it results in paint events.", SkipAll);
#endif
    QTest::qWait(10);
    QWidget *parent = new QWidget(0);
    QList<UpdateWidget *> allChildren;

    UpdateWidget *child1 = new UpdateWidget(parent);
    child1->setAutoFillBackground(true);
    allChildren.append(child1);

    UpdateWidget *child2 = new UpdateWidget(parent);
    child2->setAutoFillBackground(true);
    allChildren.append(child2);

    UpdateWidget *child3 = new UpdateWidget(parent);
    child3->setAutoFillBackground(true);
    allChildren.append(child3);

    UpdateWidget *child4 = new UpdateWidget(parent);
    child4->setAutoFillBackground(true);
    allChildren.append(child4);

    parent->show();
    QTest::qWaitForWindowShown(parent);
    QTest::qWait(10);

    QList<QObject *> list1;
    list1 << child1 << child2 << child3 << child4;
    QVERIFY(parent->children() == list1);
    QCOMPARE(allChildren.count(), list1.count());

    foreach (UpdateWidget *child, allChildren) {
        int expectedPaintEvents = child == child4 ? 1 : 0;
        if (expectedPaintEvents == 0) {
            QVERIFY(child->numPaintEvents == 0);
        } else {
            // show() issues multiple paint events on some window managers
            QTRY_VERIFY(child->numPaintEvents >= expectedPaintEvents);
        }
        QCOMPARE(child->numZOrderChangeEvents, 0);
        child->reset();
    }

    for (int i = 0; i < 5; ++i)
        child2->raise();
    QTest::qWait(50);

    foreach (UpdateWidget *child, allChildren) {
        int expectedPaintEvents = child == child2 ? 1 : 0;
        int expectedZOrderChangeEvents = child == child2 ? 1 : 0;
        QTRY_COMPARE(child->numPaintEvents, expectedPaintEvents);
        QCOMPARE(child->numZOrderChangeEvents, expectedZOrderChangeEvents);
        child->reset();
    }

    QList<QObject *> list2;
    list2 << child1 << child3 << child4 << child2;
    QVERIFY(parent->children() == list2);

    // Creates a widget on top of all the children and checks that raising one of
    // the children underneath doesn't trigger a repaint on the covering widget.
    QWidget topLevel;
    parent->setParent(&topLevel);
    topLevel.show();
    QTest::qWaitForWindowShown(&topLevel);
    QTest::qWait(50);

    UpdateWidget *onTop = new UpdateWidget(&topLevel);
    onTop->reset();
    onTop->resize(topLevel.size());
    onTop->setAutoFillBackground(true);
    onTop->show();
    QTest::qWait(50);
    QTRY_VERIFY(onTop->numPaintEvents > 0);
    onTop->reset();

    // Reset all the children.
    foreach (UpdateWidget *child, allChildren)
        child->reset();

    for (int i = 0; i < 5; ++i)
        child3->raise();
    QTest::qWait(50);

    QCOMPARE(onTop->numPaintEvents, 0);
    QCOMPARE(onTop->numZOrderChangeEvents, 0);

    QList<QObject *> list3;
    list3 << child1 << child4 << child2 << child3;
    QVERIFY(parent->children() == list3);

    foreach (UpdateWidget *child, allChildren) {
        int expectedPaintEvents = 0;
        int expectedZOrderChangeEvents = child == child3 ? 1 : 0;
        QTRY_COMPARE(child->numPaintEvents, expectedPaintEvents);
        QCOMPARE(child->numZOrderChangeEvents, expectedZOrderChangeEvents);
        child->reset();
    }
}

void tst_QWidget::lower()
{
#ifdef QT_MAC_USE_COCOA
    QSKIP("Cocoa has no Z-Order for views, we hack it, but it results in paint events.", SkipAll);
#endif
    QWidget *parent = new QWidget(0);
    QList<UpdateWidget *> allChildren;

    UpdateWidget *child1 = new UpdateWidget(parent);
    child1->setAutoFillBackground(true);
    allChildren.append(child1);

    UpdateWidget *child2 = new UpdateWidget(parent);
    child2->setAutoFillBackground(true);
    allChildren.append(child2);

    UpdateWidget *child3 = new UpdateWidget(parent);
    child3->setAutoFillBackground(true);
    allChildren.append(child3);

    UpdateWidget *child4 = new UpdateWidget(parent);
    child4->setAutoFillBackground(true);
    allChildren.append(child4);

    parent->show();
    QTest::qWaitForWindowShown(parent);
    QTest::qWait(100);

    QList<QObject *> list1;
    list1 << child1 << child2 << child3 << child4;
    QVERIFY(parent->children() == list1);
    QCOMPARE(allChildren.count(), list1.count());

    foreach (UpdateWidget *child, allChildren) {
        int expectedPaintEvents = child == child4 ? 1 : 0;
        if (expectedPaintEvents == 0) {
            QVERIFY(child->numPaintEvents == 0);
        } else {
            // show() issues multiple paint events on some window managers
            QTRY_VERIFY(child->numPaintEvents >= expectedPaintEvents);
        }
        QCOMPARE(child->numZOrderChangeEvents, 0);
        child->reset();
    }

    for (int i = 0; i < 5; ++i)
        child4->lower();

    QTest::qWait(100);

    foreach (UpdateWidget *child, allChildren) {
        int expectedPaintEvents = child == child3 ? 1 : 0;
        int expectedZOrderChangeEvents = child == child4 ? 1 : 0;
        QTRY_COMPARE(child->numZOrderChangeEvents, expectedZOrderChangeEvents);
        QTRY_COMPARE(child->numPaintEvents, expectedPaintEvents);
        child->reset();
    }

    QList<QObject *> list2;
    list2 << child4 << child1 << child2 << child3;
    QVERIFY(parent->children() == list2);

    delete parent;
}

void tst_QWidget::stackUnder()
{
#ifdef QT_MAC_USE_COCOA
    QSKIP("Cocoa has no Z-Order for views, we hack it, but it results in paint events.", SkipAll);
#endif
    QTest::qWait(10);
    QWidget *parent = new QWidget(0);
    QList<UpdateWidget *> allChildren;

    UpdateWidget *child1 = new UpdateWidget(parent);
    child1->setAutoFillBackground(true);
    allChildren.append(child1);

    UpdateWidget *child2 = new UpdateWidget(parent);
    child2->setAutoFillBackground(true);
    allChildren.append(child2);

    UpdateWidget *child3 = new UpdateWidget(parent);
    child3->setAutoFillBackground(true);
    allChildren.append(child3);

    UpdateWidget *child4 = new UpdateWidget(parent);
    child4->setAutoFillBackground(true);
    allChildren.append(child4);

    parent->show();
    QTest::qWaitForWindowShown(parent);
    QTest::qWait(10);
#ifdef Q_WS_QWS
    QApplication::sendPostedEvents(); //glib workaround
#endif

    QList<QObject *> list1;
    list1 << child1 << child2 << child3 << child4;
    QVERIFY(parent->children() == list1);

    foreach (UpdateWidget *child, allChildren) {
        int expectedPaintEvents = child == child4 ? 1 : 0;
#if defined(Q_WS_WIN) || defined(Q_WS_MAC)
        if (expectedPaintEvents == 1 && child->numPaintEvents == 2)
            QEXPECT_FAIL(0, "Mac and Windows issues double repaints for Z-Order change", Continue);
#endif
        QTRY_COMPARE(child->numPaintEvents, expectedPaintEvents);
        QCOMPARE(child->numZOrderChangeEvents, 0);
        child->reset();
    }

    for (int i = 0; i < 5; ++i)
        child4->stackUnder(child2);
    QTest::qWait(10);

    QList<QObject *> list2;
    list2 << child1 << child4 << child2 << child3;
    QVERIFY(parent->children() == list2);

    foreach (UpdateWidget *child, allChildren) {
        int expectedPaintEvents = child == child3 ? 1 : 0;
        int expectedZOrderChangeEvents = child == child4 ? 1 : 0;
        QTRY_COMPARE(child->numPaintEvents, expectedPaintEvents);
        QTRY_COMPARE(child->numZOrderChangeEvents, expectedZOrderChangeEvents);
        child->reset();
    }

    for (int i = 0; i < 5; ++i)
        child1->stackUnder(child3);
    QTest::qWait(10);

    QList<QObject *> list3;
    list3 << child4 << child2 << child1 << child3;
    QVERIFY(parent->children() == list3);

    foreach (UpdateWidget *child, allChildren) {
        int expectedZOrderChangeEvents = child == child1 ? 1 : 0;
        if (child == child3) {
#ifdef Q_OS_WINCE
            qApp->processEvents();
#endif
#ifndef Q_WS_MAC
            QEXPECT_FAIL(0, "See QTBUG-493", Continue);
#endif
            QCOMPARE(child->numPaintEvents, 0);
        } else {
            QCOMPARE(child->numPaintEvents, 0);
        }
        QTRY_COMPARE(child->numZOrderChangeEvents, expectedZOrderChangeEvents);
        child->reset();
    }

    delete parent;
}

void drawPolygon(QPaintDevice *dev, int w, int h)
{
    QPainter p(dev);
    p.fillRect(0, 0, w, h, Qt::white);

    QPolygon a;
    a << QPoint(0, 0) << QPoint(w/2, h/2) << QPoint(w, 0)
      << QPoint(w/2, h) << QPoint(0, 0);

    p.setPen(QPen(Qt::black, 1));
    p.setBrush(Qt::DiagCrossPattern);
    p.drawPolygon(a);
}

class ContentsPropagationWidget : public QWidget
{
    Q_OBJECT
public:
    ContentsPropagationWidget(QWidget *parent = 0) : QWidget(parent)
    {
        QWidget *child = this;
        for (int i=0; i<32; ++i) {
            child = new QWidget(child);
            child->setGeometry(i, i, 400 - i*2, 400 - i*2);
        }
    }

    void setContentsPropagation(bool enable) {
        foreach (QObject *child, children())
            qobject_cast<QWidget *>(child)->setAutoFillBackground(!enable);
    }

protected:
    void paintEvent(QPaintEvent *)
    {
        int w = width(), h = height();
        drawPolygon(this, w, h);
    }

    QSize sizeHint() const { return QSize(500, 500); }
};

void tst_QWidget::testContentsPropagation()
{
#ifdef Q_WS_MAC
    QSKIP("Pixmap is not antialiased whereas widget is.", SkipAll);
#endif
    ContentsPropagationWidget widget;
#ifdef Q_WS_QWS
    widget.resize(500,500);
#else
    widget.setFixedSize(500, 500);
#endif
    widget.setContentsPropagation(false);
    QPixmap widgetSnapshot = QPixmap::grabWidget(&widget);

    QPixmap correct(500, 500);
    drawPolygon(&correct, 500, 500);
    //correct.save("correct.png", "PNG");

    //widgetSnapshot.save("snap1.png", "PNG");
    QVERIFY(widgetSnapshot.toImage() != correct.toImage());

    widget.setContentsPropagation(true);
    widgetSnapshot = QPixmap::grabWidget(&widget);
    //widgetSnapshot.save("snap2.png", "PNG");

    QCOMPARE(widgetSnapshot, correct);
}

/*
    Test that saving and restoring window geometry with
    saveGeometry() and restoreGeometry() works.
*/
void tst_QWidget::saveRestoreGeometry()
{
#ifdef Q_OS_IRIX
    QSKIP("4DWM issues on IRIX makes this test fail", SkipAll);
#endif
    const QPoint position(100, 100);
    const QSize size(200, 200);

    QByteArray savedGeometry;

    {
        QWidget widget;
        widget.move(position);
        widget.resize(size);
        widget.show();
        QTest::qWaitForWindowShown(&widget);
        QApplication::processEvents();

        QTRY_COMPARE(widget.pos(), position);
        QCOMPARE(widget.size(), size);
        savedGeometry = widget.saveGeometry();
    }

    {
        QWidget widget;

        const QByteArray empty;
        const QByteArray one("a");
        const QByteArray two("ab");
        const QByteArray three("abc");
        const QByteArray four("abca");
        const QByteArray garbage("abcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabc");

        QVERIFY(widget.restoreGeometry(empty) == false);
        QVERIFY(widget.restoreGeometry(one) == false);
        QVERIFY(widget.restoreGeometry(two) == false);
        QVERIFY(widget.restoreGeometry(three) == false);
        QVERIFY(widget.restoreGeometry(four) == false);
        QVERIFY(widget.restoreGeometry(garbage) == false);

        QVERIFY(widget.restoreGeometry(savedGeometry));
        widget.show();
        QTest::qWaitForWindowShown(&widget);
        QApplication::processEvents();

        QTRY_COMPARE(widget.pos(), position);
        QCOMPARE(widget.size(), size);
        widget.show();
        QCOMPARE(widget.pos(), position);
        QCOMPARE(widget.size(), size);
    }

    {
        QWidget widget;
        widget.move(position);
        widget.resize(size);
        widget.show();
        QTest::qWaitForWindowShown(&widget);
        QTest::qWait(500);
        QTRY_COMPARE(widget.geometry().size(), size);

        QRect geom;

        //Restore from Full screen
        savedGeometry = widget.saveGeometry();
        geom = widget.geometry();
        widget.setWindowState(widget.windowState() | Qt::WindowFullScreen);
        QTRY_VERIFY((widget.windowState() & Qt::WindowFullScreen));
        QTest::qWait(500);
        QVERIFY(widget.restoreGeometry(savedGeometry));
        QTest::qWait(120);
        QTRY_VERIFY(!(widget.windowState() & Qt::WindowFullScreen));
        QTRY_COMPARE(widget.geometry(), geom);

        //Restore to full screen
        widget.setWindowState(widget.windowState() | Qt::WindowFullScreen);
        QTest::qWait(120);
        QTRY_VERIFY((widget.windowState() & Qt::WindowFullScreen));
        QTest::qWait(500);
        savedGeometry = widget.saveGeometry();
        geom = widget.geometry();
        widget.setWindowState(widget.windowState() ^ Qt::WindowFullScreen);
        QTest::qWait(120);
        QTRY_VERIFY(!(widget.windowState() & Qt::WindowFullScreen));
        QTest::qWait(400);
        QVERIFY(widget.restoreGeometry(savedGeometry));
        QTest::qWait(120);
        QTRY_VERIFY((widget.windowState() & Qt::WindowFullScreen));
        QTRY_COMPARE(widget.geometry(), geom);
        QVERIFY((widget.windowState() & Qt::WindowFullScreen));
        widget.setWindowState(widget.windowState() ^ Qt::WindowFullScreen);
        QTest::qWait(120);
        QTRY_VERIFY(!(widget.windowState() & Qt::WindowFullScreen));
        QTest::qWait(120);

        //Restore from Maximised
        widget.move(position);
        widget.resize(size);
        QTest::qWait(10);
        QTRY_COMPARE(widget.size(), size);
        QTest::qWait(500);
        savedGeometry = widget.saveGeometry();
        geom = widget.geometry();
        widget.setWindowState(widget.windowState() | Qt::WindowMaximized);
        QTest::qWait(120);
        QTRY_VERIFY((widget.windowState() & Qt::WindowMaximized));
        QTRY_VERIFY(widget.geometry() != geom);
        QTest::qWait(500);
        QVERIFY(widget.restoreGeometry(savedGeometry));
        QTest::qWait(120);
        QTRY_COMPARE(widget.geometry(), geom);

        QVERIFY(!(widget.windowState() & Qt::WindowMaximized));

        //Restore to maximised
        widget.setWindowState(widget.windowState() | Qt::WindowMaximized);
        QTest::qWait(120);
        QTRY_VERIFY((widget.windowState() & Qt::WindowMaximized));
        QTest::qWait(500);
        geom = widget.geometry();
        savedGeometry = widget.saveGeometry();
        widget.setWindowState(widget.windowState() ^ Qt::WindowMaximized);
        QTest::qWait(120);
        QTRY_VERIFY(!(widget.windowState() & Qt::WindowMaximized));
        QTest::qWait(500);
        QVERIFY(widget.restoreGeometry(savedGeometry));
        QTest::qWait(120);
        QTRY_VERIFY((widget.windowState() & Qt::WindowMaximized));
        QTRY_COMPARE(widget.geometry(), geom);
    }
}

void tst_QWidget::restoreVersion1Geometry_data()
{
    QTest::addColumn<QString>("fileName");
    QTest::addColumn<uint>("expectedWindowState");
    QTest::addColumn<QPoint>("expectedPosition");
    QTest::addColumn<QSize>("expectedSize");
    QTest::addColumn<QRect>("expectedNormalGeometry");
    const QPoint position(100, 100);
    const QSize size(200, 200);
    const QRect normalGeometry(102, 124, 200, 200);

    QTest::newRow("geometry.dat") << ":geometry.dat" << uint(Qt::WindowNoState) << position << size << normalGeometry;
    QTest::newRow("geometry-maximized.dat") << ":geometry-maximized.dat" << uint(Qt::WindowMaximized) << position << size << normalGeometry;
    QTest::newRow("geometry-fullscreen.dat") << ":geometry-fullscreen.dat" << uint(Qt::WindowFullScreen) << position << size << normalGeometry;
}

/*
    Test that the current version of restoreGeometry() can restore geometry
    saved width saveGeometry() version 1.0.
*/
void tst_QWidget::restoreVersion1Geometry()
{
#ifdef Q_OS_IRIX
    QSKIP("4DWM issues on IRIX makes this test fail", SkipAll);
#endif

    QFETCH(QString, fileName);
    QFETCH(uint, expectedWindowState);
    QFETCH(QPoint, expectedPosition);
    QFETCH(QSize, expectedSize);
    QFETCH(QRect, expectedNormalGeometry);

    // WindowActive is uninteresting for this test
    const uint WindowStateMask = Qt::WindowFullScreen | Qt::WindowMaximized | Qt::WindowMinimized;

    QFile f(fileName);
    QVERIFY(f.exists());
    f.open(QIODevice::ReadOnly);
    const QByteArray savedGeometry = f.readAll();
    QCOMPARE(savedGeometry.count(), 46);
    f.close();

    QWidget widget;

    QVERIFY(widget.restoreGeometry(savedGeometry));

    QCOMPARE(uint(widget.windowState() & WindowStateMask), expectedWindowState);
    if (expectedWindowState == Qt::WindowNoState) {
        QCOMPARE(widget.pos(), expectedPosition);
        QCOMPARE(widget.size(), expectedSize);
    }
    widget.show();
    QTest::qWaitForWindowShown(&widget);
    QTest::qWait(100);

    if (expectedWindowState == Qt::WindowNoState) {
        QTRY_COMPARE(widget.pos(), expectedPosition);
        QTRY_COMPARE(widget.size(), expectedSize);
    }

    widget.showNormal();
    QTest::qWait(10);

    if (expectedWindowState != Qt::WindowNoState) {
        // restoring from maximized or fullscreen, we can only restore to the normal geometry
        QTRY_COMPARE(widget.geometry(), expectedNormalGeometry);
    } else {
        QTRY_COMPARE(widget.pos(), expectedPosition);
        QTRY_COMPARE(widget.size(), expectedSize);
    }

#if 0
    // Code for saving a new geometry*.dat files
    {
        QWidget widgetToSave;
        widgetToSave.move(expectedPosition);
        widgetToSave.resize(expectedSize);
        widgetToSave.show();
#ifdef Q_WS_X11
        qt_x11_wait_for_window_manager(&widget);
#endif
        QTest::qWait(500); // stabilize
        widgetToSave.setWindowState(Qt::WindowStates(expectedWindowState));
        QTest::qWait(500); // stabilize

        QByteArray geometryToSave = widgetToSave.saveGeometry();

        // Code for saving a new geometry.dat file.
        f.setFileName(fileName.mid(1));
        QVERIFY(f.open(QIODevice::WriteOnly)); // did you forget to 'p4 edit *.dat'? :)
        f.write(geometryToSave);
        f.close();
    }
#endif

}

void tst_QWidget::widgetAt()
{
    Q_CHECK_PAINTEVENTS

    QWidget *w1 = new QWidget(0, Qt::X11BypassWindowManagerHint);
    w1->setGeometry(0,0,150,150);
    w1->setObjectName("w1");

    QWidget *w2 = new QWidget(0, Qt::X11BypassWindowManagerHint  | Qt::FramelessWindowHint);
    w2->setGeometry(50,50,100,100);
    w2->setObjectName("w2");
    w1->show();
    QTest::qWaitForWindowShown(w1);
    qApp->processEvents();
    QWidget *wr;
    QTRY_VERIFY((wr = QApplication::widgetAt(100, 100)));
    QCOMPARE(wr->objectName(), QString("w1"));

    w2->show();
    QTest::qWaitForWindowShown(w2);
    qApp->processEvents();
    qApp->processEvents();
    qApp->processEvents();
    QTRY_VERIFY((wr = QApplication::widgetAt(100, 100)));
    QCOMPARE(wr->objectName(), QString("w2"));

    w2->lower();
    qApp->processEvents();
    QTRY_VERIFY((wr = QApplication::widgetAt(100, 100)));
    const bool match = (wr->objectName() == QString("w1"));
    w2->raise();
    QVERIFY(match);

    qApp->processEvents();
    QTRY_VERIFY((wr = QApplication::widgetAt(100, 100)));
    QCOMPARE(wr->objectName(), QString("w2"));


    QWidget *w3 = new QWidget(w2);
    w3->setGeometry(10,10,50,50);
    w3->setObjectName("w3");
    w3->show();
    qApp->processEvents();
    QTRY_VERIFY((wr = QApplication::widgetAt(100,100)));
    QCOMPARE(wr->objectName(), QString("w3"));

    w3->setAttribute(Qt::WA_TransparentForMouseEvents);
    qApp->processEvents();
    QTRY_VERIFY((wr = QApplication::widgetAt(100, 100)));
    QCOMPARE(wr->objectName(), QString("w2"));

    QRegion rgn = QRect(QPoint(0,0), w2->size());
    QPoint point = w2->mapFromGlobal(QPoint(100,100));
    rgn -= QRect(point, QSize(1,1));
    w2->setMask(rgn);
    qApp->processEvents();
    QTest::qWait(10);
#if defined(Q_OS_WINCE)
    QEXPECT_FAIL("", "Windows CE does only support rectangular regions", Continue); //See also task 147191
#endif
#if defined(Q_OS_SYMBIAN)
    QEXPECT_FAIL("", "Symbian/S60 does only support rectangular regions", Continue); //See also task 147191
#endif
    QTRY_COMPARE(QApplication::widgetAt(100,100)->objectName(), w1->objectName());
    QTRY_COMPARE(QApplication::widgetAt(101,101)->objectName(), w2->objectName());

    QBitmap bitmap(w2->size());
    QPainter p(&bitmap);
    p.fillRect(bitmap.rect(), Qt::color1);
    p.setPen(Qt::color0);
    p.drawPoint(w2->mapFromGlobal(QPoint(100,100)));
    p.end();
    w2->setMask(bitmap);
    qApp->processEvents();
    QTest::qWait(10);
#if defined(Q_OS_WINCE)
    QEXPECT_FAIL("", "Windows CE does only support rectangular regions", Continue); //See also task 147191
#endif
#if defined(Q_OS_SYMBIAN)
    QEXPECT_FAIL("", "Symbian/S60 does only support rectangular regions", Continue); //See also task 147191
#endif
    QTRY_VERIFY(QApplication::widgetAt(100,100) == w1);
    QTRY_VERIFY(QApplication::widgetAt(101,101) == w2);

    delete w2;
    delete w1;
}

#if defined(Q_WS_X11)
bool getProperty(Display *display, Window target, Atom type, Atom property,
                 unsigned char** data, unsigned long* count)
{
    Atom atom_return;
    int size;
    unsigned long nitems, bytes_left;

    int ret = XGetWindowProperty(display, target, property,
                                 0l, 1l, false,
                                 type, &atom_return, &size,
                                 &nitems, &bytes_left, data);
    if (ret != Success || nitems < 1)
        return false;

    if (bytes_left != 0) {
        XFree(*data);
        unsigned long remain = ((size / 8) * nitems) + bytes_left;
        ret = XGetWindowProperty(display, target,
                                 property, 0l, remain, false,
                                 type, &atom_return, &size,
                                 &nitems, &bytes_left, data);
        if (ret != Success)
            return false;
    }

    *count = nitems;
    return true;
}

QString textPropertyToString(Display *display, XTextProperty& text_prop)
{
    QString ret;
    if (text_prop.value && text_prop.nitems > 0) {
        if (text_prop.encoding == XA_STRING) {
            ret = reinterpret_cast<char *>(text_prop.value);
        } else {
            text_prop.nitems = strlen(reinterpret_cast<char *>(text_prop.value));
            char **list;
            int num;
            if (XmbTextPropertyToTextList(display, &text_prop, &list, &num) == Success
                && num > 0 && *list) {
                ret = QString::fromLocal8Bit(*list);
                XFreeStringList(list);
            }
        }
    }
    return ret;
}

#endif

#if defined(Q_WS_S60)
// Returns the application's status pane control, if not present returns NULL.
static CCoeControl* GetStatusPaneControl( TInt aPaneId )
{
    const TUid paneUid = { aPaneId };

    CEikStatusPane* statusPane = CEikonEnv::Static()->AppUiFactory()->StatusPane();
    if (statusPane && statusPane->PaneCapabilities(paneUid).IsPresent()){
		CCoeControl* control = NULL;
        // ControlL shouldn't leave because the pane is present
		TRAPD(err, control = statusPane->ControlL(paneUid));
		return err != KErrNone ? NULL : control;
    }
    return NULL;
}
// Returns the application's title pane, if not present returns NULL.
static CAknTitlePane* TitlePane()
{
    return static_cast<CAknTitlePane*>(GetStatusPaneControl(EEikStatusPaneUidTitle));
}

// Returns the application's title pane, if not present returns NULL.
static CAknContextPane* ContextPane()
{
    return static_cast<CAknContextPane*>(GetStatusPaneControl(EEikStatusPaneUidContext));
}
#endif

static QString visibleWindowTitle(QWidget *window, Qt::WindowState state = Qt::WindowNoState)
{
    QString vTitle;

#ifdef Q_WS_WIN
    Q_UNUSED(state);
    const size_t maxTitleLength = 256;
    wchar_t title[maxTitleLength];
    GetWindowText(window->winId(), title, maxTitleLength);
    vTitle = QString::fromWCharArray(title);
#elif defined(Q_WS_X11)
    /*
      We can't check what the window manager displays, but we can
      check what we tell the window manager to display.  This will
      have to do.
    */
    Atom UTF8_STRING = XInternAtom(window->x11Info().display(), "UTF8_STRING", false);
    Atom _NET_WM_NAME = XInternAtom(window->x11Info().display(), "_NET_WM_NAME", false);
    Atom _NET_WM_ICON_NAME = XInternAtom(window->x11Info().display(), "_NET_WM_ICON_NAME", false);
    uchar *data = 0;
    ulong length = 0;
    if (state == Qt::WindowMinimized) {
        if (getProperty(window->x11Info().display(), window->winId(),
                    UTF8_STRING, _NET_WM_ICON_NAME, &data, &length)) {
            vTitle = QString::fromUtf8((char *) data, length);
            XFree(data);
        } else {
            XTextProperty text_prop;
            if (XGetWMIconName(window->x11Info().display(), window->winId(), &text_prop)) {
                vTitle = textPropertyToString(window->x11Info().display(), text_prop);
                XFree((char *) text_prop.value);
            }
        }
    } else {
        if (getProperty(window->x11Info().display(), window->winId(),
                    UTF8_STRING, _NET_WM_NAME, &data, &length)) {
            vTitle = QString::fromUtf8((char *) data, length);
            XFree(data);
        } else {
            XTextProperty text_prop;
            if (XGetWMName(window->x11Info().display(), window->winId(), &text_prop)) {
                vTitle = textPropertyToString(window->x11Info().display(), text_prop);
                XFree((char *) text_prop.value);
            }
        }
    }
#elif defined(Q_WS_MAC)
    vTitle = nativeWindowTitle(window, state);
#elif defined(Q_WS_QWS)
    if (qwsServer) {
    const QWSWindow *win = 0;
    const QList<QWSWindow*> windows = qwsServer->clientWindows();
    for (int i = 0; i < windows.count(); ++i) {
        const QWSWindow* w = windows.at(i);
        if (w->winId() == window->winId()) {
            win = w;
            break;
        }
    }
    if (win)
        vTitle = win->caption();
    }
#elif defined (Q_WS_S60)
    CAknTitlePane* titlePane = TitlePane();
    if(titlePane)
        {
        const TDesC* nTitle = titlePane->Text();
        vTitle = QString::fromUtf16(nTitle->Ptr(), nTitle->Length());
        }
#endif

    return vTitle;
}

void tst_QWidget::windowTitle()
{
    QWidget widget(0);
    widget.setWindowTitle("Application Name");
    widget.winId(); // Make sure the window is created...
    QCOMPARE(visibleWindowTitle(&widget), QString("Application Name"));

    widget.setWindowTitle("Application Name *");
    QCOMPARE(visibleWindowTitle(&widget), QString("Application Name *"));

    widget.setWindowTitle("Application Name[*]");
    QCOMPARE(visibleWindowTitle(&widget), QString("Application Name"));

    widget.setWindowTitle("Application Name[*][*]");
    QCOMPARE(visibleWindowTitle(&widget), QString("Application Name[*]"));

    widget.setWindowTitle("Application Name[*][*][*]");
    QCOMPARE(visibleWindowTitle(&widget), QString("Application Name[*]"));

    widget.setWindowTitle("Application Name[*][*][*][*]");
    QCOMPARE(visibleWindowTitle(&widget), QString("Application Name[*][*]"));
}

void tst_QWidget::windowIconText()
{
#ifdef Q_OS_SYMBIAN
    QSKIP("Symbian/S60 windows don't have window icon text", SkipAll);
#endif
    QWidget widget(0);

    widget.setWindowTitle("Application Name");
    widget.setWindowIconText("Application Minimized");
    widget.showNormal();
    QCOMPARE(visibleWindowTitle(&widget), QString("Application Name"));
    widget.showMinimized();
#if defined(Q_WS_QWS) || defined(Q_OS_WINCE)
    QEXPECT_FAIL(0, "Qt/Embedded/WinCE does not implement showMinimized()", Continue);
    //See task 147193 for WinCE
#endif
    QApplication::instance()->processEvents();
    QCOMPARE(visibleWindowTitle(&widget, Qt::WindowMinimized),
            QString("Application Minimized"));

    widget.setWindowTitle("Application Name[*]");
    widget.setWindowIconText("Application Minimized[*]");
    widget.showNormal();
    QApplication::instance()->processEvents();
    QCOMPARE(visibleWindowTitle(&widget), QString("Application Name"));
    widget.showMinimized();
#if defined (Q_WS_QWS) || defined(Q_OS_WINCE)
    QEXPECT_FAIL(0, "Qt/Embedded/WinCE does not implement showMinimized()", Continue);
    //See task 147193 for WinCE
#endif
    QApplication::instance()->processEvents();
    QCOMPARE(visibleWindowTitle(&widget, Qt::WindowMinimized),
            QString("Application Minimized"));

    widget.setWindowModified(true);
    widget.showNormal();
    QApplication::instance()->processEvents();
    if (widget.style()->styleHint(QStyle::SH_TitleBar_ModifyNotification, 0, &widget))
        QCOMPARE(visibleWindowTitle(&widget), QString("Application Name*"));
    else
        QCOMPARE(visibleWindowTitle(&widget), QString("Application Name"));
    widget.showMinimized();
#if defined (Q_WS_QWS) || defined(Q_OS_WINCE)
    QEXPECT_FAIL(0, "Qt/Embedded/WinCE does not implement showMinimized()", Continue);
    //See task 147193 for WinCE
#endif
    QApplication::instance()->processEvents();
#ifdef Q_WS_MAC
    QCOMPARE(visibleWindowTitle(&widget, Qt::WindowMinimized),
            QString("Application Minimized"));
    QVERIFY(nativeWindowModified(&widget));
#else
    QCOMPARE(visibleWindowTitle(&widget, Qt::WindowMinimized),
            QString("Application Minimized*"));
#endif
}

void tst_QWidget::windowModified()
{
    QWidget widget(0);
    widget.show();
    QTest::qWaitForWindowShown(&widget);
#ifndef Q_WS_MAC
    QTest::ignoreMessage(QtWarningMsg, "QWidget::setWindowModified: The window title does not contain a '[*]' placeholder");
#endif
    widget.setWindowTitle("Application Name");
    QTest::qWait(10);
    QTRY_COMPARE(visibleWindowTitle(&widget), QString("Application Name"));

#ifdef Q_WS_MAC
    widget.setWindowModified(true);
    QVERIFY(nativeWindowModified(&widget));
#else
    widget.setWindowModified(true);
    QApplication::instance()->processEvents();
    QCOMPARE(visibleWindowTitle(&widget), QString("Application Name"));

    widget.setWindowModified(false);
    QApplication::instance()->processEvents();
    QCOMPARE(visibleWindowTitle(&widget), QString("Application Name"));

    widget.setWindowTitle("Application Name[*]");

    widget.setWindowModified(true);
    QApplication::instance()->processEvents();
    if (widget.style()->styleHint(QStyle::SH_TitleBar_ModifyNotification, 0, &widget))
        QCOMPARE(visibleWindowTitle(&widget), QString("Application Name*"));
    else
        QCOMPARE(visibleWindowTitle(&widget), QString("Application Name"));

    widget.setWindowModified(false);
    QApplication::instance()->processEvents();
    QCOMPARE(visibleWindowTitle(&widget), QString("Application Name"));

    widget.setWindowTitle("Application[*] Name[*]");

    widget.setWindowModified(true);
    QApplication::instance()->processEvents();
    QCOMPARE(visibleWindowTitle(&widget), QString("Application* Name*"));

    widget.setWindowModified(false);
    QApplication::instance()->processEvents();
    QCOMPARE(visibleWindowTitle(&widget), QString("Application Name"));

    widget.setWindowTitle("Application Name[*][*]");

    widget.setWindowModified(true);
    QApplication::instance()->processEvents();
    QCOMPARE(visibleWindowTitle(&widget), QString("Application Name[*]"));

    widget.setWindowModified(false);
    QApplication::instance()->processEvents();
    QCOMPARE(visibleWindowTitle(&widget), QString("Application Name[*]"));

    widget.setWindowTitle("Application[*][*] Name[*][*]");

    widget.setWindowModified(true);
    QApplication::instance()->processEvents();
    QCOMPARE(visibleWindowTitle(&widget), QString("Application[*] Name[*]"));

    widget.setWindowModified(false);
    QApplication::instance()->processEvents();
    QCOMPARE(visibleWindowTitle(&widget), QString("Application[*] Name[*]"));

    widget.setWindowTitle("Application[*] Name[*][*][*]");

    widget.setWindowModified(true);
    QApplication::instance()->processEvents();
    QCOMPARE(visibleWindowTitle(&widget), QString("Application* Name[*]*"));

    widget.setWindowModified(false);
    QApplication::instance()->processEvents();
    QCOMPARE(visibleWindowTitle(&widget), QString("Application Name[*]"));

    widget.setWindowTitle("Application[*][*][*] Name[*][*][*]");

    widget.setWindowModified(true);
    QApplication::instance()->processEvents();
    QCOMPARE(visibleWindowTitle(&widget), QString("Application[*]* Name[*]*"));

    widget.setWindowModified(false);
    QApplication::instance()->processEvents();
    QCOMPARE(visibleWindowTitle(&widget), QString("Application[*] Name[*]"));
#endif
}

void tst_QWidget::task110173()
{
    QWidget w;

    QPushButton *pb1 = new QPushButton("click", &w);
    pb1->setFocusPolicy(Qt::ClickFocus);
    pb1->move(100, 100);

    QPushButton *pb2 = new QPushButton("push", &w);
    pb2->setFocusPolicy(Qt::ClickFocus);
    pb2->move(300, 300);

    QTest::keyClick( &w, Qt::Key_Tab );
    w.show();
    QTest::qWaitForWindowShown(&w);
    QTest::qWait(200);
}

class Widget : public QWidget
{
public:
    Widget() : deleteThis(false) { setFocusPolicy(Qt::StrongFocus); }
    void actionEvent(QActionEvent *) { if (deleteThis) delete this; }
    void changeEvent(QEvent *) { if (deleteThis) delete this; }
    void closeEvent(QCloseEvent *) { if (deleteThis) delete this; }
    void hideEvent(QHideEvent *) { if (deleteThis) delete this; }
    void focusOutEvent(QFocusEvent *) { if (deleteThis) delete this; }
    void keyPressEvent(QKeyEvent *) { if (deleteThis) delete this; }
    void keyReleaseEvent(QKeyEvent *) { if (deleteThis) delete this; }
    void mouseDoubleClickEvent(QMouseEvent *) { if (deleteThis) delete this; }
    void mousePressEvent(QMouseEvent *) { if (deleteThis) delete this; }
    void mouseReleaseEvent(QMouseEvent *) { if (deleteThis) delete this; }
    void mouseMoveEvent(QMouseEvent *) { if (deleteThis) delete this; }

    bool deleteThis;
};

void tst_QWidget::testDeletionInEventHandlers()
{
    // closeEvent
    QPointer<Widget> w = new Widget;
    w->deleteThis = true;
    w->close();
    QVERIFY(w == 0);
    delete w;

    // focusOut (crashes)
    //w = new Widget;
    //w->show();
    //w->setFocus();
    //QVERIFY(qApp->focusWidget() == w);
    //w->deleteThis = true;
    //w->clearFocus();
    //QVERIFY(w == 0);

    // key press
    w = new Widget;
    w->show();
    w->deleteThis = true;
    QTest::keyPress(w, Qt::Key_A);
    QVERIFY(w == 0);
    delete w;

    // key release
    w = new Widget;
    w->show();
    w->deleteThis = true;
    QTest::keyRelease(w, Qt::Key_A);
    QVERIFY(w == 0);
    delete w;

    // mouse press
    w = new Widget;
    w->show();
    w->deleteThis = true;
    QTest::mousePress(w, Qt::LeftButton);
    QVERIFY(w == 0);
    delete w;

    // mouse release
    w = new Widget;
    w->show();
    w->deleteThis = true;
    QTest::mouseRelease(w, Qt::LeftButton);
    QVERIFY(w == 0);
    delete w;

    // mouse double click
    w = new Widget;
    w->show();
    w->deleteThis = true;
    QTest::mouseDClick(w, Qt::LeftButton);
    QVERIFY(w == 0);
    delete w;

    // hide event (crashes)
    //w = new Widget;
    //w->show();
    //w->deleteThis = true;
    //w->hide();
    //QVERIFY(w == 0);

    // action event
    w = new Widget;
    w->deleteThis = true;
    w->addAction(new QAction(w));
    QVERIFY(w == 0);
    delete w;

    // change event
    w = new Widget;
    w->show();
    w->deleteThis = true;
    w->setMouseTracking(true);
    QVERIFY(w == 0);
    delete w;

    w = new Widget;
    w->setMouseTracking(true);
    w->show();
    w->deleteThis = true;
    QMouseEvent me(QEvent::MouseMove, QPoint(0, 0), Qt::NoButton, Qt::NoButton, Qt::NoModifier);
    QApplication::sendEvent(w, &me);
    QVERIFY(w == 0);
    delete w;
}

#ifdef Q_WS_MAC

bool testAndRelease(const HIViewRef view)
{
//    qDebug() << CFGetRetainCount(view);
    if (CFGetRetainCount(view) != 2)
        return false;
    CFRelease(view);
    CFRelease(view);
    return true;
}

typedef QPair<QWidget *, HIViewRef> WidgetViewPair;

WidgetViewPair createAndRetain(QWidget * const parent = 0)
{
    QWidget * const widget = new QWidget(parent);
    const HIViewRef view = (HIViewRef)widget->winId();
    // Retain twice so we can safely call CFGetRetaintCount even if the retain count
    // is off by one because of a double release.
    CFRetain(view);
    CFRetain(view);
    return qMakePair(widget, view);
}

/*
    Test that retaining and releasing the HIView returned by QWidget::winId()
    works even if the widget itself is deleted.
*/
void tst_QWidget::retainHIView()
{
    // Single window
    {
        const WidgetViewPair window  = createAndRetain();
        delete window.first;
        QVERIFY(testAndRelease(window.second));
    }

    // Child widget
    {
        const WidgetViewPair parent = createAndRetain();
        const WidgetViewPair child = createAndRetain(parent.first);

        delete parent.first;
        QVERIFY(testAndRelease(parent.second));
        QVERIFY(testAndRelease(child.second));
    }

    // Multiple children
    {
        const WidgetViewPair parent = createAndRetain();
        const WidgetViewPair child1 = createAndRetain(parent.first);
        const WidgetViewPair child2 = createAndRetain(parent.first);

        delete parent.first;
        QVERIFY(testAndRelease(parent.second));
        QVERIFY(testAndRelease(child1.second));
        QVERIFY(testAndRelease(child2.second));
    }

    // Grandchild widget
    {
        const WidgetViewPair parent = createAndRetain();
        const WidgetViewPair child = createAndRetain(parent.first);
        const WidgetViewPair grandchild = createAndRetain(child.first);

        delete parent.first;
        QVERIFY(testAndRelease(parent.second));
        QVERIFY(testAndRelease(child.second));
        QVERIFY(testAndRelease(grandchild.second));
    }

    // Reparent child widget
    {
        const WidgetViewPair parent1 = createAndRetain();
        const WidgetViewPair parent2 = createAndRetain();
        const WidgetViewPair child = createAndRetain(parent1.first);

        child.first->setParent(parent2.first);

        delete parent1.first;
        QVERIFY(testAndRelease(parent1.second));
        delete parent2.first;
        QVERIFY(testAndRelease(parent2.second));
        QVERIFY(testAndRelease(child.second));
    }

    // Reparent window
    {
        const WidgetViewPair window1 = createAndRetain();
        const WidgetViewPair window2 = createAndRetain();
        const WidgetViewPair child1 = createAndRetain(window1.first);
        const WidgetViewPair child2 = createAndRetain(window2.first);

        window2.first->setParent(window1.first);

        delete window2.first;
        QVERIFY(testAndRelease(window2.second));
        QVERIFY(testAndRelease(child2.second));
        delete window1.first;
        QVERIFY(testAndRelease(window1.second));
        QVERIFY(testAndRelease(child1.second));
    }

    // Delete child widget
    {
        const WidgetViewPair parent = createAndRetain();
        const WidgetViewPair child = createAndRetain(parent.first);

        delete child.first;
        QVERIFY(testAndRelease(child.second));
        delete parent.first;
        QVERIFY(testAndRelease(parent.second));
    }
}

void tst_QWidget::sheetOpacity()
{
    QWidget tmpWindow;
    QWidget sheet(&tmpWindow, Qt::Sheet);
    tmpWindow.show();
    sheet.show();
    QCOMPARE(int(sheet.windowOpacity() * 255), 242);  // 95%
    sheet.setParent(0, Qt::Dialog);
    QCOMPARE(int(sheet.windowOpacity() * 255), 255);
}

class MaskedPainter : public QWidget
{
public:
    QRect mask;

    MaskedPainter()
    : mask(20, 20, 50, 50)
    {
        setMask(mask);
    }

    void paintEvent(QPaintEvent *)
    {
        QPainter p(this);
        p.fillRect(mask, QColor(Qt::red));
    }
};

/*
    Verifies that the entire area inside the mask is painted red.
*/
bool verifyWidgetMask(QWidget *widget, QRect mask)
{
    const QImage image = QPixmap::grabWindow(widget->winId()).toImage();

    const QImage masked = image.copy(mask);
    QImage red(masked);
    red.fill(QColor(Qt::red).rgb());

    return (masked == red);
}

void tst_QWidget::setMask()
{
    testWidget->hide(); // get this out of the way.

    {
        MaskedPainter w;
        w.resize(200, 200);
        w.show();
        QTest::qWait(100);
        QVERIFY(verifyWidgetMask(&w, w.mask));
    }
    {
        MaskedPainter w;
        w.resize(200, 200);
        w.setWindowFlags(w.windowFlags() | Qt::FramelessWindowHint);
        w.show();
        QTest::qWait(100);
        QRect mask = w.mask;

        QVERIFY(verifyWidgetMask(&w, mask));
    }
}
#endif

class StaticWidget : public QWidget
{
Q_OBJECT
public:
    bool partial;
    bool gotPaintEvent;
    QRegion paintedRegion;

    StaticWidget(QWidget *parent = 0)
    :QWidget(parent)
    {
        setAttribute(Qt::WA_StaticContents);
        setAttribute(Qt::WA_OpaquePaintEvent);
        setPalette(Qt::red); // Make sure we have an opaque palette.
        setAutoFillBackground(true);
        gotPaintEvent = false;
    }

    void paintEvent(QPaintEvent *e)
    {
        paintedRegion += e->region();
        gotPaintEvent = true;
//        qDebug() << "paint" << e->region();
        // Look for a full update, set partial to false if found.
        foreach(QRect r, e->region().rects()) {
            partial = (r != rect());
            if (partial == false)
                break;
        }
    }
};

/*
    Test that widget resizes and moves can be done with minimal repaints when WA_StaticContents
    and WA_OpaquePaintEvent is set. Test is mac-only for now.
*/
void tst_QWidget::optimizedResizeMove()
{
    QWidget parent;
    parent.resize(400, 400);

    StaticWidget staticWidget(&parent);
    staticWidget.gotPaintEvent = false;
    staticWidget.move(150, 150);
    staticWidget.resize(150, 150);
    parent.show();
    QTest::qWaitForWindowShown(&parent);
    QTest::qWait(20);
    QTRY_COMPARE(staticWidget.gotPaintEvent, true);

    staticWidget.gotPaintEvent = false;
    staticWidget.move(staticWidget.pos() + QPoint(10, 10));
    QTest::qWait(20);
    QCOMPARE(staticWidget.gotPaintEvent, false);

    staticWidget.gotPaintEvent = false;
    staticWidget.move(staticWidget.pos() + QPoint(-10, -10));
    QTest::qWait(20);
    QCOMPARE(staticWidget.gotPaintEvent, false);

    staticWidget.gotPaintEvent = false;
    staticWidget.move(staticWidget.pos() + QPoint(-10, 10));
    QTest::qWait(20);
    QCOMPARE(staticWidget.gotPaintEvent, false);

    staticWidget.gotPaintEvent = false;
    staticWidget.resize(staticWidget.size() + QSize(10, 10));
    QTest::qWait(20);
    QCOMPARE(staticWidget.gotPaintEvent, true);
    QCOMPARE(staticWidget.partial, true);

    staticWidget.gotPaintEvent = false;
    staticWidget.resize(staticWidget.size() + QSize(-10, -10));
    QTest::qWait(20);
    QCOMPARE(staticWidget.gotPaintEvent, false);

    staticWidget.gotPaintEvent = false;
    staticWidget.resize(staticWidget.size() + QSize(10, -10));
    QTest::qWait(20);
    QCOMPARE(staticWidget.gotPaintEvent, true);
    QCOMPARE(staticWidget.partial, true);

    staticWidget.gotPaintEvent = false;
    staticWidget.move(staticWidget.pos() + QPoint(10, 10));
    staticWidget.resize(staticWidget.size() + QSize(-10, -10));
    QTest::qWait(20);
    QCOMPARE(staticWidget.gotPaintEvent, false);

    staticWidget.gotPaintEvent = false;
    staticWidget.move(staticWidget.pos() + QPoint(10, 10));
    staticWidget.resize(staticWidget.size() + QSize(10, 10));
    QTest::qWait(20);
    QCOMPARE(staticWidget.gotPaintEvent, true);
    QCOMPARE(staticWidget.partial, true);

    staticWidget.gotPaintEvent = false;
    staticWidget.move(staticWidget.pos() + QPoint(-10, -10));
    staticWidget.resize(staticWidget.size() + QSize(-10, -10));
    QTest::qWait(20);
    QCOMPARE(staticWidget.gotPaintEvent, false);

    staticWidget.setAttribute(Qt::WA_StaticContents, false);
    staticWidget.gotPaintEvent = false;
    staticWidget.move(staticWidget.pos() + QPoint(-10, -10));
    staticWidget.resize(staticWidget.size() + QSize(-10, -10));
    QTest::qWait(20);
    QCOMPARE(staticWidget.gotPaintEvent, true);
    QCOMPARE(staticWidget.partial, false);
    staticWidget.setAttribute(Qt::WA_StaticContents, true);

    staticWidget.setAttribute(Qt::WA_StaticContents, false);
    staticWidget.gotPaintEvent = false;
    staticWidget.move(staticWidget.pos() + QPoint(10, 10));
    QTest::qWait(20);
    QCOMPARE(staticWidget.gotPaintEvent, false);
    staticWidget.setAttribute(Qt::WA_StaticContents, true);
}

void tst_QWidget::optimizedResize_topLevel()
{
#if defined(Q_WS_MAC) || defined(Q_WS_QWS)
    QSKIP("We do not yet have static contents support for *top-levels* on this platform", SkipAll);
#endif

    StaticWidget topLevel;
    topLevel.gotPaintEvent = false;
    topLevel.show();
    QTest::qWaitForWindowShown(&topLevel);
    QTest::qWait(10);
    QTRY_COMPARE(topLevel.gotPaintEvent, true);

    topLevel.gotPaintEvent = false;
    topLevel.partial = false;
    topLevel.paintedRegion = QRegion();

#ifndef Q_WS_WIN
    topLevel.resize(topLevel.size() + QSize(10, 10));
#else
    // Static contents does not work when programmatically resizing
    // top-levels with QWidget::resize. We do some funky stuff in
    // setGeometry_sys. However, resizing it with the mouse or with
    // a native function call works (it basically has to go through
    // WM_RESIZE in QApplication). This is a corner case, though.
    // See task 243708
    const QRect frame = topLevel.frameGeometry();
    MoveWindow(topLevel.winId(), frame.x(), frame.y(),
               frame.width() + 10, frame.height() + 10,
               true);
#endif

    QTest::qWait(100);

    // Expected update region: New rect - old rect.
    QRegion expectedUpdateRegion(topLevel.rect());
    expectedUpdateRegion -= QRect(QPoint(), topLevel.size() - QSize(10, 10));

    QTRY_COMPARE(topLevel.gotPaintEvent, true);
    QCOMPARE(topLevel.partial, true);
    QCOMPARE(topLevel.paintedRegion, expectedUpdateRegion);
}

class SiblingDeleter : public QWidget
{
public:
    inline SiblingDeleter(QWidget *sibling, QWidget *parent)
        : QWidget(parent), sibling(sibling) {}
    inline virtual ~SiblingDeleter() { delete sibling; }

private:
    QPointer<QWidget> sibling;
};


void tst_QWidget::childDeletesItsSibling()
{
    QWidget *commonParent = new QWidget(0);
    QPointer<QWidget> child = new QWidget(0);
    QPointer<QWidget> siblingDeleter = new SiblingDeleter(child, commonParent);
    child->setParent(commonParent);
    delete commonParent; // don't crash
    QVERIFY(!child);
    QVERIFY(!siblingDeleter);

}

#ifdef Q_WS_QWS
# define SET_SAFE_SIZE(w) \
    do { \
        QSize safeSize(qt_screen->width() - 250, qt_screen->height() - 250);      \
         if (!safeSize.isValid()) \
             QSKIP("Screen size too small", SkipAll); \
         if (defaultSize.width() > safeSize.width() || defaultSize.height() > safeSize.height()) { \
             defaultSize = safeSize; \
             w.resize(defaultSize); \
             w.setAttribute(Qt::WA_Resized, false); \
         } \
    } while (false)
#else
# define SET_SAFE_SIZE(w)
#endif


void tst_QWidget::setMinimumSize()
{
    QWidget w;
    QSize defaultSize = w.size();
    SET_SAFE_SIZE(w);

    w.setMinimumSize(defaultSize + QSize(100, 100));
    QCOMPARE(w.size(), defaultSize + QSize(100, 100));
    QVERIFY(!w.testAttribute(Qt::WA_Resized));

    w.setMinimumSize(defaultSize + QSize(50, 50));
    QCOMPARE(w.size(), defaultSize + QSize(100, 100));
    QVERIFY(!w.testAttribute(Qt::WA_Resized));

    w.setMinimumSize(defaultSize + QSize(200, 200));
    QCOMPARE(w.size(), defaultSize + QSize(200, 200));
    QVERIFY(!w.testAttribute(Qt::WA_Resized));

#ifdef Q_OS_WINCE
    QSKIP("Setting a minimum size larger than the desktop does not work", SkipAll);
#endif
    QSize nonDefaultSize = defaultSize + QSize(5,5);
    w.setMinimumSize(nonDefaultSize);
    w.show();
    QTest::qWait(50);
    QVERIFY(w.height() >= nonDefaultSize.height());
    QVERIFY(w.width() >= nonDefaultSize.width());
}

void tst_QWidget::setMaximumSize()
{
    QWidget w;
    QSize defaultSize = w.size();
    SET_SAFE_SIZE(w);

    w.setMinimumSize(defaultSize + QSize(100, 100));
    QCOMPARE(w.size(), defaultSize + QSize(100, 100));
    QVERIFY(!w.testAttribute(Qt::WA_Resized));
    w.setMinimumSize(defaultSize);

    w.setMaximumSize(defaultSize + QSize(200, 200));
    QCOMPARE(w.size(), defaultSize + QSize(100, 100));
    QVERIFY(!w.testAttribute(Qt::WA_Resized));

    w.setMaximumSize(defaultSize + QSize(50, 50));
    QCOMPARE(w.size(), defaultSize + QSize(50, 50));
    QVERIFY(!w.testAttribute(Qt::WA_Resized));

#if 0
    //we don't enforce maximum size on show, apparently
    QSize nonDefaultSize = defaultSize - QSize(5,5);
    w.setMaximumSize(nonDefaultSize);
    w.show();
    QTest::qWait(50);
    qDebug() << nonDefaultSize << w.size();
    QVERIFY(w.height() <= nonDefaultSize.height());
    QVERIFY(w.width() <= nonDefaultSize.width());
#endif
}

void tst_QWidget::setFixedSize()
{
    QWidget w;
    QSize defaultSize = w.size();
    SET_SAFE_SIZE(w);

    w.setFixedSize(defaultSize + QSize(100, 100));
    QCOMPARE(w.size(), defaultSize + QSize(100, 100));
    QVERIFY(w.testAttribute(Qt::WA_Resized));

    w.setFixedSize(defaultSize + QSize(200, 200));

    QCOMPARE(w.minimumSize(), defaultSize + QSize(200,200));
    QCOMPARE(w.maximumSize(), defaultSize + QSize(200,200));
    QCOMPARE(w.size(), defaultSize + QSize(200, 200));
    QVERIFY(w.testAttribute(Qt::WA_Resized));

    w.setFixedSize(defaultSize + QSize(50, 50));
    QCOMPARE(w.size(), defaultSize + QSize(50, 50));
    QVERIFY(w.testAttribute(Qt::WA_Resized));

    w.setAttribute(Qt::WA_Resized, false);
    w.setFixedSize(defaultSize + QSize(50, 50));
    QVERIFY(!w.testAttribute(Qt::WA_Resized));

    w.setFixedSize(defaultSize + QSize(150, 150));
    w.show();
    QTest::qWait(50);
    QVERIFY(w.size() == defaultSize + QSize(150,150));
}

void tst_QWidget::ensureCreated()
{
    {
        QWidget widget;
        WId widgetWinId = widget.winId();
        Q_UNUSED(widgetWinId);
        QVERIFY(widget.testAttribute(Qt::WA_WState_Created));
    }

    {
        QWidget window;

        QDialog dialog(&window);
        dialog.setWindowModality(Qt::NonModal);

        WId dialogWinId = dialog.winId();
        Q_UNUSED(dialogWinId);
        QVERIFY(dialog.testAttribute(Qt::WA_WState_Created));
        QVERIFY(window.testAttribute(Qt::WA_WState_Created));
    }

    {
        QWidget window;

        QDialog dialog(&window);
        dialog.setWindowModality(Qt::WindowModal);

        WId dialogWinId = dialog.winId();
        Q_UNUSED(dialogWinId);
        QVERIFY(dialog.testAttribute(Qt::WA_WState_Created));
        QVERIFY(window.testAttribute(Qt::WA_WState_Created));
    }

    {
        QWidget window;

        QDialog dialog(&window);
        dialog.setWindowModality(Qt::ApplicationModal);

        WId dialogWinId = dialog.winId();
        Q_UNUSED(dialogWinId);
        QVERIFY(dialog.testAttribute(Qt::WA_WState_Created));
        QVERIFY(window.testAttribute(Qt::WA_WState_Created));
    }
}

class WinIdChangeWidget : public QWidget {
public:
    WinIdChangeWidget(QWidget *p = 0)
        : QWidget(p)
        , m_winIdChangeEventCount(0)
    {

    }
protected:
    bool event(QEvent *e)
    {
        if (e->type() == QEvent::WinIdChange) {
            ++m_winIdChangeEventCount;
            return true;
        }
        return QWidget::event(e);
    }
public:
    int m_winIdChangeEventCount;
};

void tst_QWidget::winIdChangeEvent()
{
    {
        // Transforming an alien widget into a native widget
        WinIdChangeWidget widget;
        const WId winIdBefore = widget.internalWinId();
        const WId winIdAfter = widget.winId();
        QVERIFY(winIdBefore != winIdAfter);
        QCOMPARE(widget.m_winIdChangeEventCount, 1);
    }

    {
        // Changing parent of a native widget
        // Should cause winId of child to change, on all platforms
        QWidget parent1, parent2;
        WinIdChangeWidget child(&parent1);
        const WId winIdBefore = child.winId();
        QCOMPARE(child.m_winIdChangeEventCount, 1);
        child.setParent(&parent2);
        const WId winIdAfter = child.internalWinId();
        QVERIFY(winIdBefore != winIdAfter);
        QCOMPARE(child.m_winIdChangeEventCount, 2);
    }

    {
        // Changing grandparent of a native widget
        // Should cause winId of grandchild to change only on Symbian
        QWidget grandparent1, grandparent2;
        QWidget parent(&grandparent1);
        WinIdChangeWidget child(&parent);
        const WId winIdBefore = child.winId();
        QCOMPARE(child.m_winIdChangeEventCount, 1);
        parent.setParent(&grandparent2);
        const WId winIdAfter = child.internalWinId();
#ifdef Q_OS_SYMBIAN
        QVERIFY(winIdBefore != winIdAfter);
        QCOMPARE(child.m_winIdChangeEventCount, 2);
#else
        QCOMPARE(winIdBefore, winIdAfter);
        QCOMPARE(child.m_winIdChangeEventCount, 1);
#endif
    }

    {
        // Changing parent of an alien widget
        QWidget parent1, parent2;
        WinIdChangeWidget child(&parent1);
        const WId winIdBefore = child.internalWinId();
        child.setParent(&parent2);
        const WId winIdAfter = child.internalWinId();
        QCOMPARE(winIdBefore, winIdAfter);
        QCOMPARE(child.m_winIdChangeEventCount, 0);
    }

    {
        // Making native child widget into a top-level window
        QWidget parent;
        WinIdChangeWidget child(&parent);
        child.winId();
        const WId winIdBefore = child.internalWinId();
        QCOMPARE(child.m_winIdChangeEventCount, 1);
        const Qt::WindowFlags flags = child.windowFlags();
        child.setWindowFlags(flags | Qt::Window);
        const WId winIdAfter = child.internalWinId();
        QVERIFY(winIdBefore != winIdAfter);
        QCOMPARE(child.m_winIdChangeEventCount, 2);
    }
}

#ifdef Q_OS_SYMBIAN
void tst_QWidget::reparentCausesChildWinIdChange()
{
    QWidget *parent = new QWidget;
    QWidget *w1 = new QWidget;
    QWidget *w2 = new QWidget;
    QWidget *w3 = new QWidget;
    w1->setParent(parent);
    w2->setParent(w1);
    w3->setParent(w2);

    WId winId1 = w1->winId();
    WId winId2 = w2->winId();
    WId winId3 = w3->winId();

    // reparenting causes winIds of the widget being reparented, and all of its children, to change
    w1->setParent(0);
    QVERIFY(w1->winId() != winId1);
    winId1 = w1->winId();
    QVERIFY(w2->winId() != winId2);
    winId2 = w2->winId();
    QVERIFY(w3->winId() != winId3);
    winId3 = w3->winId();

    w1->setParent(parent);
    QVERIFY(w1->winId() != winId1);
    winId1 = w1->winId();
    QVERIFY(w2->winId() != winId2);
    winId2 = w2->winId();
    QVERIFY(w3->winId() != winId3);
    winId3 = w3->winId();

    w2->setParent(0);
    QVERIFY(w2->winId() != winId2);
    winId2 = w2->winId();
    QVERIFY(w3->winId() != winId3);
    winId3 = w3->winId();

    w2->setParent(parent);
    QVERIFY(w2->winId() != winId2);
    winId2 = w2->winId();
    QVERIFY(w3->winId() != winId3);
    winId3 = w3->winId();

    w2->setParent(w1);
    QVERIFY(w2->winId() != winId2);
    winId2 = w2->winId();
    QVERIFY(w3->winId() != winId3);
    winId3 = w3->winId();

    w3->setParent(0);
    QVERIFY(w3->winId() != winId3);
    winId3 = w3->winId();

    w3->setParent(w1);
    QVERIFY(w3->winId() != winId3);
    winId3 = w3->winId();

    w3->setParent(w2);
    QVERIFY(w3->winId() != winId3);
    winId3 = w3->winId();

    delete parent;
}
#else
void tst_QWidget::persistentWinId()
{
    QWidget *parent = new QWidget;
    QWidget *w1 = new QWidget;
    QWidget *w2 = new QWidget;
    QWidget *w3 = new QWidget;
    w1->setParent(parent);
    w2->setParent(w1);
    w3->setParent(w2);

    WId winId1 = w1->winId();
    WId winId2 = w2->winId();
    WId winId3 = w3->winId();

    // reparenting should change the winId of the widget being reparented, but not of its children
    w1->setParent(0);
    QVERIFY(w1->winId() != winId1);
    winId1 = w1->winId();
    QCOMPARE(w2->winId(), winId2);
    QCOMPARE(w3->winId(), winId3);

    w1->setParent(parent);
    QVERIFY(w1->winId() != winId1);
    winId1 = w1->winId();
    QCOMPARE(w2->winId(), winId2);
    QCOMPARE(w3->winId(), winId3);

    w2->setParent(0);
    QVERIFY(w2->winId() != winId2);
    winId2 = w2->winId();
    QCOMPARE(w3->winId(), winId3);

    w2->setParent(parent);
    QVERIFY(w2->winId() != winId2);
    winId2 = w2->winId();
    QCOMPARE(w3->winId(), winId3);

    w2->setParent(w1);
    QVERIFY(w2->winId() != winId2);
    winId2 = w2->winId();
    QCOMPARE(w3->winId(), winId3);

    w3->setParent(0);
    QVERIFY(w3->winId() != winId3);
    winId3 = w3->winId();

    w3->setParent(w1);
    QVERIFY(w3->winId() != winId3);
    winId3 = w3->winId();

    w3->setParent(w2);
    QVERIFY(w3->winId() != winId3);
    winId3 = w3->winId();

    delete parent;
}
#endif // Q_OS_SYMBIAN

void tst_QWidget::showNativeChild()
{
    QWidget topLevel;
    topLevel.setGeometry(0, 0, 100, 100);
    QWidget child(&topLevel);
    child.winId();
    topLevel.show();
    QTest::qWaitForWindowShown(&topLevel);
}

class ShowHideEventWidget : public QWidget
{
public:
    int numberOfShowEvents, numberOfHideEvents;

    ShowHideEventWidget(QWidget *parent = 0)
        : QWidget(parent), numberOfShowEvents(0), numberOfHideEvents(0)
    { }

    void create()
    { QWidget::create(); }

    void showEvent(QShowEvent *)
    { ++numberOfShowEvents; }

    void hideEvent(QHideEvent *)
    { ++numberOfHideEvents; }
};

void tst_QWidget::showHideEvent_data()
{
    QTest::addColumn<bool>("show");
    QTest::addColumn<bool>("hide");
    QTest::addColumn<bool>("create");
    QTest::addColumn<int>("expectedShowEvents");
    QTest::addColumn<int>("expectedHideEvents");

    QTest::newRow("window: only show")
            << true
            << false
            << false
            << 1
            << 0;
    QTest::newRow("window: show/hide")
            << true
            << true
            << false
            << 1
            << 1;
    QTest::newRow("window: show/hide/create")
            << true
            << true
            << true
            << 1
            << 1;
    QTest::newRow("window: hide/create")
            << false
            << true
            << true
            << 0
            << 0;
    QTest::newRow("window: only hide")
            << false
            << true
            << false
            << 0
            << 0;
    QTest::newRow("window: nothing")
            << false
            << false
            << false
            << 0
            << 0;
}

void tst_QWidget::showHideEvent()
{
    QFETCH(bool, show);
    QFETCH(bool, hide);
    QFETCH(bool, create);
    QFETCH(int, expectedShowEvents);
    QFETCH(int, expectedHideEvents);

    ShowHideEventWidget widget;
    if (show)
        widget.show();
    if (hide)
        widget.hide();
    if (create && !widget.testAttribute(Qt::WA_WState_Created))
        widget.create();

    QCOMPARE(widget.numberOfShowEvents, expectedShowEvents);
    QCOMPARE(widget.numberOfHideEvents, expectedHideEvents);
}

void tst_QWidget::update()
{
    QTest::qWait(10);  // Wait for the initStuff to do it's stuff.
    Q_CHECK_PAINTEVENTS

    UpdateWidget w;
    w.setGeometry(50, 50, 100, 100);
    w.show();
    QTest::qWaitForWindowShown(&w);

    QApplication::processEvents();
    QApplication::processEvents();

    QTRY_COMPARE(w.numPaintEvents, 1);

    QCOMPARE(w.visibleRegion(), QRegion(w.rect()));
    QCOMPARE(w.paintedRegion, w.visibleRegion());
    w.reset();

    UpdateWidget child(&w);
    child.setGeometry(10, 10, 80, 80);
    child.show();

    QPoint childOffset = child.mapToParent(QPoint());

    // widgets are transparent by default, so both should get repaints
    {
        QApplication::processEvents();
        QApplication::processEvents();
        QCOMPARE(child.numPaintEvents, 1);
        QCOMPARE(child.visibleRegion(), QRegion(child.rect()));
        QCOMPARE(child.paintedRegion, child.visibleRegion());
        QCOMPARE(w.numPaintEvents, 1);
        QCOMPARE(w.visibleRegion(), QRegion(w.rect()));
        QCOMPARE(w.paintedRegion, child.visibleRegion().translated(childOffset));

        w.reset();
        child.reset();

        w.update();
        QApplication::processEvents();
        QApplication::processEvents();
        QCOMPARE(child.numPaintEvents, 1);
        QCOMPARE(child.visibleRegion(), QRegion(child.rect()));
        QCOMPARE(child.paintedRegion, child.visibleRegion());
        QCOMPARE(w.numPaintEvents, 1);
        QCOMPARE(w.visibleRegion(), QRegion(w.rect()));
        QCOMPARE(w.paintedRegion, w.visibleRegion());
    }

    QPalette opaquePalette = child.palette();
    opaquePalette.setColor(child.backgroundRole(), QColor(Qt::red));

    // setting an opaque background on the child should prevent paint-events
    // for the parent in the child area
    {
        child.setPalette(opaquePalette);
        child.setAutoFillBackground(true);
        QApplication::processEvents();

        w.reset();
        child.reset();

        w.update();
        QApplication::processEvents();
        QApplication::processEvents();

        QCOMPARE(w.numPaintEvents, 1);
        QRegion expectedVisible = QRegion(w.rect())
                                  - child.visibleRegion().translated(childOffset);
        QCOMPARE(w.visibleRegion(), expectedVisible);
#ifdef QT_MAC_USE_COCOA
        QEXPECT_FAIL(0, "Cocoa compositor paints the content view", Continue);
#endif
        QCOMPARE(w.paintedRegion, expectedVisible);
#ifdef QT_MAC_USE_COCOA
        QEXPECT_FAIL(0, "Cocoa compositor says to paint this.", Continue);
#endif
        QCOMPARE(child.numPaintEvents, 0);

        w.reset();
        child.reset();

        child.update();
        QApplication::processEvents();
        QApplication::processEvents();

        QCOMPARE(w.numPaintEvents, 0);
        QCOMPARE(child.numPaintEvents, 1);
        QCOMPARE(child.paintedRegion, child.visibleRegion());

        w.reset();
        child.reset();
    }

    // overlapping sibling
    UpdateWidget sibling(&w);
    child.setGeometry(10, 10, 20, 20);
    sibling.setGeometry(15, 15, 20, 20);
    sibling.show();

    QApplication::processEvents();
    w.reset();
    child.reset();
    sibling.reset();

    const QPoint siblingOffset = sibling.mapToParent(QPoint());

    sibling.update();
    QApplication::processEvents();
    QApplication::processEvents();

    // child is opaque, sibling transparent
    {
        QCOMPARE(sibling.numPaintEvents, 1);
        QCOMPARE(sibling.paintedRegion, sibling.visibleRegion());

        QCOMPARE(child.numPaintEvents, 1);
        QCOMPARE(child.paintedRegion.translated(childOffset),
                 child.visibleRegion().translated(childOffset)
                 & sibling.visibleRegion().translated(siblingOffset));

        QCOMPARE(w.numPaintEvents, 1);
#ifdef QT_MAC_USE_COCOA
        QEXPECT_FAIL(0, "Cocoa compositor paints the content view", Continue);
#endif
        QCOMPARE(w.paintedRegion,
                 w.visibleRegion() & sibling.visibleRegion().translated(siblingOffset));
#ifdef QT_MAC_USE_COCOA
        QEXPECT_FAIL(0, "Cocoa compositor paints the content view", Continue);
#endif
        QCOMPARE(w.paintedRegion,
                 (w.visibleRegion() - child.visibleRegion().translated(childOffset))
                 & sibling.visibleRegion().translated(siblingOffset));

    }
    w.reset();
    child.reset();
    sibling.reset();

    sibling.setPalette(opaquePalette);
    sibling.setAutoFillBackground(true);

    sibling.update();
    QApplication::processEvents();
    QApplication::processEvents();

    // child opaque, sibling opaque
    {
        QCOMPARE(sibling.numPaintEvents, 1);
        QCOMPARE(sibling.paintedRegion, sibling.visibleRegion());

#ifdef QT_MAC_USE_COCOA
        QEXPECT_FAIL(0, "Cocoa compositor paints child and sibling", Continue);
#endif
        QCOMPARE(child.numPaintEvents, 0);
        QCOMPARE(child.visibleRegion(),
                 QRegion(child.rect())
                 - sibling.visibleRegion().translated(siblingOffset - childOffset));

        QCOMPARE(w.numPaintEvents, 0);
        QCOMPARE(w.visibleRegion(),
                 QRegion(w.rect())
                 - child.visibleRegion().translated(childOffset)
                 - sibling.visibleRegion().translated(siblingOffset));
    }
}

static inline bool isOpaque(QWidget *widget)
{
    if (!widget)
        return false;
    return qt_widget_private(widget)->isOpaque;
}

void tst_QWidget::isOpaque()
{
#ifndef Q_WS_MAC
    QWidget w;
    QVERIFY(::isOpaque(&w));

    QWidget child(&w);
    QVERIFY(!::isOpaque(&child));

    child.setAutoFillBackground(true);
    QVERIFY(::isOpaque(&child));

    QPalette palette;

    // background color

    palette = child.palette();
    palette.setColor(child.backgroundRole(), QColor(255, 0, 0, 127));
    child.setPalette(palette);
    QVERIFY(!::isOpaque(&child));

    palette.setColor(child.backgroundRole(), QColor(255, 0, 0, 255));
    child.setPalette(palette);
    QVERIFY(::isOpaque(&child));

    palette.setColor(QPalette::Window, QColor(0, 0, 255, 127));
    w.setPalette(palette);

    QVERIFY(!::isOpaque(&w));

    child.setAutoFillBackground(false);
    QVERIFY(!::isOpaque(&child));

    // Qt::WA_OpaquePaintEvent

    child.setAttribute(Qt::WA_OpaquePaintEvent);
    QVERIFY(::isOpaque(&child));

    child.setAttribute(Qt::WA_OpaquePaintEvent, false);
    QVERIFY(!::isOpaque(&child));

    // Qt::WA_NoSystemBackground

    child.setAttribute(Qt::WA_NoSystemBackground);
    QVERIFY(!::isOpaque(&child));

    child.setAttribute(Qt::WA_NoSystemBackground, false);
    QVERIFY(!::isOpaque(&child));

    palette.setColor(QPalette::Window, QColor(0, 0, 255, 255));
    w.setPalette(palette);
    QVERIFY(::isOpaque(&w));

    w.setAttribute(Qt::WA_NoSystemBackground);
    QVERIFY(!::isOpaque(&w));

    w.setAttribute(Qt::WA_NoSystemBackground, false);
    QVERIFY(::isOpaque(&w));

    {
        QPalette palette = QApplication::palette();
        QPalette old = palette;
        palette.setColor(QPalette::Window, Qt::transparent);
        QApplication::setPalette(palette);

        QWidget widget;
        QVERIFY(!::isOpaque(&widget));

        QApplication::setPalette(old);
        QCOMPARE(::isOpaque(&widget), old.color(QPalette::Window).alpha() == 255);
    }
#endif
}

#ifndef Q_WS_MAC
/*
    Test that scrolling of a widget invalidates the correct regions
*/
void tst_QWidget::scroll()
{
    UpdateWidget updateWidget;
    updateWidget.resize(500, 500);
    updateWidget.reset();
    updateWidget.show();
    QTest::qWaitForWindowShown(&updateWidget);
    QTest::qWait(50);
    qApp->processEvents();
    QTRY_VERIFY(updateWidget.numPaintEvents > 0);

    {
        updateWidget.reset();
        updateWidget.scroll(10, 10);
        qApp->processEvents();
        QRegion dirty(QRect(0, 0, 500, 10));
        dirty += QRegion(QRect(0, 10, 10, 490));
        QCOMPARE(updateWidget.paintedRegion, dirty);
    }

    {
        updateWidget.reset();
        updateWidget.update(0, 0, 10, 10);
        updateWidget.scroll(0, 10);
        qApp->processEvents();
        QRegion dirty(QRect(0, 0, 500, 10));
        dirty += QRegion(QRect(0, 10, 10, 10));
        QCOMPARE(updateWidget.paintedRegion, dirty);
    }

    {
        updateWidget.reset();
        updateWidget.update(0, 0, 100, 100);
        updateWidget.scroll(10, 10, QRect(50, 50, 100, 100));
        qApp->processEvents();
        QRegion dirty(QRect(0, 0, 100, 50));
        dirty += QRegion(QRect(0, 50, 150, 10));
        dirty += QRegion(QRect(0, 60, 110, 40));
        dirty += QRegion(QRect(50, 100, 60, 10));
        dirty += QRegion(QRect(50, 110, 10, 40));
        QCOMPARE(updateWidget.paintedRegion, dirty);
    }

    {
        updateWidget.reset();
        updateWidget.update(0, 0, 100, 100);
        updateWidget.scroll(10, 10, QRect(100, 100, 100, 100));
        qApp->processEvents();
        QRegion dirty(QRect(0, 0, 100, 100));
        dirty += QRegion(QRect(100, 100, 100, 10));
        dirty += QRegion(QRect(100, 110, 10, 90));
        QCOMPARE(updateWidget.paintedRegion, dirty);
    }
}
#endif

class DestroyedSlotChecker : public QObject
{
    Q_OBJECT

public:
    bool wasQWidget;

    DestroyedSlotChecker()
        : wasQWidget(false)
    {
    }

public slots:
    void destroyedSlot(QObject *object)
    {
        wasQWidget = (qobject_cast<QWidget *>(object) != 0 || object->isWidgetType());
    }
};

/*
    Test that qobject_cast<QWidget*> returns 0 in a slot
    connected to QObject::destroyed.
*/
void tst_QWidget::qobject_castInDestroyedSlot()
{
    DestroyedSlotChecker checker;
    QWidget *widget = new QWidget();

    QObject::connect(widget, SIGNAL(destroyed(QObject *)), &checker, SLOT(destroyedSlot(QObject *)));
    delete widget;

    QVERIFY(checker.wasQWidget == true);
}

Q_DECLARE_METATYPE(QList<QRect>)

void tst_QWidget::setWindowGeometry_data()
{
    QTest::addColumn<QList<QRect> >("rects");
    QTest::addColumn<int>("windowFlags");

    QList<QList<QRect> > rects;
    rects << (QList<QRect>()
              << QRect(100, 100, 200, 200)
              << QApplication::desktop()->availableGeometry().adjusted(100, 100, -100, -100)
              << QRect(130, 100, 0, 200)
              << QRect(100, 50, 200, 0)
              << QRect(130, 50, 0, 0))
          << (QList<QRect>()
              << QApplication::desktop()->availableGeometry().adjusted(100, 100, -100, -100)
              << QRect(130, 100, 0, 200)
              << QRect(100, 50, 200, 0)
              << QRect(130, 50, 0, 0)
              << QRect(100, 100, 200, 200))
          << (QList<QRect>()
              << QRect(130, 100, 0, 200)
              << QRect(100, 50, 200, 0)
              << QRect(130, 50, 0, 0)
              << QRect(100, 100, 200, 200)
              << QApplication::desktop()->availableGeometry().adjusted(100, 100, -100, -100))
          << (QList<QRect>()
              << QRect(100, 50, 200, 0)
              << QRect(130, 50, 0, 0)
              << QRect(100, 100, 200, 200)
              << QApplication::desktop()->availableGeometry().adjusted(100, 100, -100, -100)
              << QRect(130, 100, 0, 200))
          << (QList<QRect>()
              << QRect(130, 50, 0, 0)
              << QRect(100, 100, 200, 200)
              << QApplication::desktop()->availableGeometry().adjusted(100, 100, -100, -100)
              << QRect(130, 100, 0, 200)
              << QRect(100, 50, 200, 0));

    QList<int> windowFlags;
    windowFlags << 0
                << Qt::FramelessWindowHint
#ifdef Q_WS_X11
                << Qt::X11BypassWindowManagerHint
#endif
                ;

    foreach (QList<QRect> l, rects) {
        QRect rect = l.first();
        foreach (int windowFlag, windowFlags) {
            QTest::newRow(QString("%1,%2 %3x%4, flags %5")
                          .arg(rect.x())
                          .arg(rect.y())
                          .arg(rect.width())
                          .arg(rect.height())
                          .arg(windowFlag, 0, 16).toAscii())
                << l
                << windowFlag;
        }
    }
}

void tst_QWidget::setWindowGeometry()
{
#ifdef Q_WS_X11
    //Since WindowManager operation are all assync, and we have no way to know if the window
    // manager has finished playing with the window geometry, this test can't be reliable.
    QSKIP("Window Manager behaviour are too random for this test", SkipAll);
#endif
    QFETCH(QList<QRect>, rects);
    QFETCH(int, windowFlags);
    QRect rect = rects.takeFirst();

    {
        // test setGeometry() without actually showing the window
        QWidget widget;
        if (windowFlags != 0)
            widget.setWindowFlags(Qt::WindowFlags(windowFlags));

        widget.setGeometry(rect);
        QTest::qWait(100);
        QCOMPARE(widget.geometry(), rect);

        // setGeometry() without showing
        foreach (QRect r, rects) {
            widget.setGeometry(r);
            QTest::qWait(100);
            QCOMPARE(widget.geometry(), r);
        }
    }

    {
        // setGeometry() first, then show()
        QWidget widget;
        if (windowFlags != 0)
            widget.setWindowFlags(Qt::WindowFlags(windowFlags));

        widget.setGeometry(rect);
        widget.show();
        QTest::qWaitForWindowShown(&widget);
        QTest::qWait(20);
        QTRY_COMPARE(widget.geometry(), rect);

        // setGeometry() while shown
        foreach (QRect r, rects) {
            widget.setGeometry(r);
            QTest::qWait(10);
            QTRY_COMPARE(widget.geometry(), r);
        }
        widget.setGeometry(rect);
        QTest::qWait(20);
        QTRY_COMPARE(widget.geometry(), rect);

        // now hide
        widget.hide();
        QTest::qWait(20);
        QTRY_COMPARE(widget.geometry(), rect);

        // setGeometry() after hide()
        foreach (QRect r, rects) {
            widget.setGeometry(r);
            QTest::qWait(10);
            QTRY_COMPARE(widget.geometry(), r);
        }
        widget.setGeometry(rect);
        QTest::qWait(10);
        QTRY_COMPARE(widget.geometry(), rect);

        // show() again, geometry() should still be the same
        widget.show();
        QTest::qWaitForWindowShown(&widget);
        QTest::qWait(10);
        QTRY_COMPARE(widget.geometry(), rect);

        // final hide(), again geometry() should be unchanged
        widget.hide();
        QTest::qWait(10);
        QTRY_COMPARE(widget.geometry(), rect);
    }

    {
        // show() first, then setGeometry()
        QWidget widget;
        if (windowFlags != 0)
            widget.setWindowFlags(Qt::WindowFlags(windowFlags));

        widget.show();
        QTest::qWaitForWindowShown(&widget);
        widget.setGeometry(rect);
        QTest::qWait(10);
        QTRY_COMPARE(widget.geometry(), rect);

        // setGeometry() while shown
        foreach (QRect r, rects) {
            widget.setGeometry(r);
            QTest::qWait(10);
            QTRY_COMPARE(widget.geometry(), r);
        }
        widget.setGeometry(rect);
        QTest::qWait(10);
        QTRY_COMPARE(widget.geometry(), rect);

        // now hide
        widget.hide();
        QTest::qWait(10);
        QTRY_COMPARE(widget.geometry(), rect);

        // setGeometry() after hide()
        foreach (QRect r, rects) {
            widget.setGeometry(r);
            QTest::qWait(10);
            QTRY_COMPARE(widget.geometry(), r);
        }
        widget.setGeometry(rect);
        QTest::qWait(10);
        QTRY_COMPARE(widget.geometry(), rect);

        // show() again, geometry() should still be the same
        widget.show();
        QTest::qWaitForWindowShown(&widget);
        QTest::qWait(10);
        QTRY_COMPARE(widget.geometry(), rect);

        // final hide(), again geometry() should be unchanged
        widget.hide();
        QTest::qWait(10);
        QTRY_COMPARE(widget.geometry(), rect);
    }
}

#if defined (Q_WS_WIN) && !defined(Q_OS_WINCE)
void tst_QWidget::setGeometry_win()
{
    QWidget widget;
    widget.setGeometry(0, 600, 100,100);
    widget.show();
    widget.setWindowState(widget.windowState() | Qt::WindowMaximized);
    QRect geom = widget.normalGeometry();
    widget.close();
    widget.setGeometry(geom);
    widget.setWindowState(widget.windowState() | Qt::WindowMaximized);
    widget.show();
    RECT rt;
    ::GetWindowRect(widget.internalWinId(), &rt);
    QVERIFY(rt.left <= 0);
    QVERIFY(rt.top <= 0);
}
#endif

void tst_QWidget::windowMoveResize_data()
{
    setWindowGeometry_data();
}

void tst_QWidget::windowMoveResize()
{
#ifdef Q_WS_X11
    //Since WindowManager operation are all assync, and we have no way to know if the window
    // manager has finished playing with the window geometry, this test can't be reliable.
    QSKIP("Window Manager behaviour are too random for this test", SkipAll);
#endif
#ifdef Q_OS_IRIX
    QSKIP("4DWM issues on IRIX makes this test fail", SkipAll);
#endif
    QFETCH(QList<QRect>, rects);
    QFETCH(int, windowFlags);

    QRect rect = rects.takeFirst();

    {
        // test setGeometry() without actually showing the window
        QWidget widget;
        if (windowFlags != 0)
            widget.setWindowFlags(Qt::WindowFlags(windowFlags));

        widget.move(rect.topLeft());
        widget.resize(rect.size());
        QTest::qWait(10);
        QTRY_COMPARE(widget.pos(), rect.topLeft());
        QTRY_COMPARE(widget.size(), rect.size());

        // move() without showing
        foreach (QRect r, rects) {
            widget.move(r.topLeft());
            widget.resize(r.size());
            QApplication::processEvents();
            QTRY_COMPARE(widget.pos(), r.topLeft());
            QTRY_COMPARE(widget.size(), r.size());
        }
    }

    {
        // move() first, then show()
        QWidget widget;
        if (windowFlags != 0)
            widget.setWindowFlags(Qt::WindowFlags(windowFlags));

        widget.move(rect.topLeft());
        widget.resize(rect.size());
        widget.show();

        QTest::qWait(10);
#if defined(Q_WS_MAC) && !defined(QT_MAC_USE_COCOA)
        QEXPECT_FAIL("130,50 0x0, flags 0",
                     "Showing a window with 0x0 size shifts it up.",
                     Continue);
#endif
        QTRY_COMPARE(widget.pos(), rect.topLeft());
        QTRY_COMPARE(widget.size(), rect.size());

        // move() while shown
        foreach (QRect r, rects) {
#ifdef Q_WS_X11
            if ((widget.width() == 0 || widget.height() == 0) && r.width() != 0 && r.height() != 0) {
                QEXPECT_FAIL("130,100 0x200, flags 0",
                             "First resize after show of zero-sized gets wrong win_gravity.",
                             Continue);
                QEXPECT_FAIL("100,50 200x0, flags 0",
                             "First resize after show of zero-sized gets wrong win_gravity.",
                             Continue);
                QEXPECT_FAIL("130,50 0x0, flags 0",
                             "First resize after show of zero-sized gets wrong win_gravity.",
                             Continue);
            }
#endif
            widget.move(r.topLeft());
            widget.resize(r.size());
            QApplication::processEvents();
            QTRY_COMPARE(widget.pos(), r.topLeft());
            QTRY_COMPARE(widget.size(), r.size());
        }
        widget.move(rect.topLeft());
        widget.resize(rect.size());
        QApplication::processEvents();
        QTRY_COMPARE(widget.pos(), rect.topLeft());
        QTRY_COMPARE(widget.size(), rect.size());

        // now hide
        widget.hide();
        QTest::qWait(10);
        QTRY_COMPARE(widget.pos(), rect.topLeft());
        QTRY_COMPARE(widget.size(), rect.size());

        // move() after hide()
        foreach (QRect r, rects) {
            widget.move(r.topLeft());
            widget.resize(r.size());
            QApplication::processEvents();
#if defined(Q_WS_MAC) && defined(QT_MAC_USE_COCOA)
            if (r.width() == 0 && r.height() > 0) {
                widget.move(r.topLeft());
                widget.resize(r.size());
             }
#endif
            QTRY_COMPARE(widget.pos(), r.topLeft());
            QTRY_COMPARE(widget.size(), r.size());
        }
        widget.move(rect.topLeft());
        widget.resize(rect.size());
        QTest::qWait(10);
        QTRY_COMPARE(widget.pos(), rect.topLeft());
        QTRY_COMPARE(widget.size(), rect.size());

        // show() again, pos() should be the same
        widget.show();
        QTest::qWaitForWindowShown(&widget);
        QApplication::processEvents();
        QTRY_COMPARE(widget.pos(), rect.topLeft());
        QTRY_COMPARE(widget.size(), rect.size());

        // final hide(), again pos() should be unchanged
        widget.hide();
        QApplication::processEvents();
        QTRY_COMPARE(widget.pos(), rect.topLeft());
        QTRY_COMPARE(widget.size(), rect.size());
    }

    {
        // show() first, then move()
        QWidget widget;
        if (windowFlags != 0)
            widget.setWindowFlags(Qt::WindowFlags(windowFlags));

        widget.show();
        QTest::qWaitForWindowShown(&widget);
        QApplication::processEvents();
        widget.move(rect.topLeft());
        widget.resize(rect.size());
        QApplication::processEvents();
        QTRY_COMPARE(widget.pos(), rect.topLeft());
        QTRY_COMPARE(widget.size(), rect.size());

        // move() while shown
        foreach (QRect r, rects) {
            widget.move(r.topLeft());
            widget.resize(r.size());
            QApplication::processEvents();
            QTRY_COMPARE(widget.pos(), r.topLeft());
            QTRY_COMPARE(widget.size(), r.size());
        }
        widget.move(rect.topLeft());
        widget.resize(rect.size());
        QApplication::processEvents();
        QTRY_COMPARE(widget.pos(), rect.topLeft());
        QTRY_COMPARE(widget.size(), rect.size());

        // now hide
        widget.hide();
        QApplication::processEvents();
        QTRY_COMPARE(widget.pos(), rect.topLeft());
        QTRY_COMPARE(widget.size(), rect.size());

        // move() after hide()
        foreach (QRect r, rects) {
            widget.move(r.topLeft());
            widget.resize(r.size());
            QApplication::processEvents();
#if defined(Q_WS_MAC) && defined(QT_MAC_USE_COCOA)
            if (r.width() == 0 && r.height() > 0) {
                widget.move(r.topLeft());
                widget.resize(r.size());
             }
#endif
            QTRY_COMPARE(widget.pos(), r.topLeft());
            QTRY_COMPARE(widget.size(), r.size());
        }
        widget.move(rect.topLeft());
        widget.resize(rect.size());
        QApplication::processEvents();
        QTRY_COMPARE(widget.pos(), rect.topLeft());
        QTRY_COMPARE(widget.size(), rect.size());

        // show() again, pos() should be the same
        widget.show();
        QTest::qWaitForWindowShown(&widget);
        QTest::qWait(10);
        QTRY_COMPARE(widget.pos(), rect.topLeft());
        QTRY_COMPARE(widget.size(), rect.size());

        // final hide(), again pos() should be unchanged
        widget.hide();
        QTest::qWait(10);
        QTRY_COMPARE(widget.pos(), rect.topLeft());
        QTRY_COMPARE(widget.size(), rect.size());
    }
}

class ColorWidget : public QWidget
{
public:
    ColorWidget(QWidget *parent = 0, const QColor &c = QColor(Qt::red))
        : QWidget(parent, Qt::FramelessWindowHint), color(c)
    {
        QPalette opaquePalette = palette();
        opaquePalette.setColor(backgroundRole(), color);
        setPalette(opaquePalette);
        setAutoFillBackground(true);
    }

    void paintEvent(QPaintEvent *e) {
        r += e->region();
    }

    void reset() {
        r = QRegion();
    }

    QColor color;
    QRegion r;
};

#define VERIFY_COLOR(region, color) {                                   \
    const QRegion r = QRegion(region);                                  \
    for (int i = 0; i < r.rects().size(); ++i) {                        \
        const QRect rect = r.rects().at(i);                             \
        for (int t = 0; t < 5; t++) {                                   \
            const QPixmap pixmap = QPixmap::grabWindow(QDesktopWidget().winId(), \
                                                   rect.left(), rect.top(), \
                                                   rect.width(), rect.height()); \
            QCOMPARE(pixmap.size(), rect.size());                       \
            QPixmap expectedPixmap(pixmap); /* ensure equal formats */  \
            expectedPixmap.fill(color);                                 \
            if (pixmap.toImage().pixel(0,0) != QColor(color).rgb() && t < 4 ) \
            { QTest::qWait(200); continue; }                            \
            QCOMPARE(pixmap.toImage().pixel(0,0), QColor(color).rgb()); \
            QCOMPARE(pixmap, expectedPixmap);                           \
            break;                                                      \
        }                                                               \
    }                                                                   \
}

void tst_QWidget::moveChild_data()
{
    QTest::addColumn<QPoint>("offset");

    QTest::newRow("right") << QPoint(20, 0);
    QTest::newRow("down") << QPoint(0, 20);
    QTest::newRow("left") << QPoint(-20, 0);
    QTest::newRow("up") << QPoint(0, -20);
}

void tst_QWidget::moveChild()
{
    QFETCH(QPoint, offset);

    ColorWidget parent;
    // prevent custom styles
    parent.setStyle(new QWindowsStyle);
    ColorWidget child(&parent, Qt::blue);

#ifndef Q_OS_WINCE
    parent.setGeometry(QRect(QPoint(QApplication::desktop()->availableGeometry(&parent).topLeft()),
                             QSize(100, 100)));
#else
    parent.setGeometry(60, 60, 150, 150);
#endif
    child.setGeometry(25, 25, 50, 50);
    QPoint childOffset = child.mapToGlobal(QPoint());

    parent.show();
    QTest::qWaitForWindowShown(&parent);
    QTest::qWait(30);
    const QPoint tlwOffset = parent.geometry().topLeft();

#ifdef QT_MAC_USE_COCOA
    QEXPECT_FAIL(0, "Cocoa compositor paints the entire content view, even when opaque", Continue);
#endif
    QTRY_COMPARE(parent.r, QRegion(parent.rect()) - child.geometry());
    QTRY_COMPARE(child.r, QRegion(child.rect()));
    VERIFY_COLOR(child.geometry().translated(tlwOffset),
                 child.color);
    VERIFY_COLOR(QRegion(parent.geometry()) - child.geometry().translated(tlwOffset),
                 parent.color);
    parent.reset();
    child.reset();

    // move

    const QRect oldGeometry = child.geometry();

    QPoint pos = child.pos() + offset;
    child.move(pos);
    QTest::qWait(100);
    QTRY_COMPARE(pos, child.pos());

    QCOMPARE(parent.r, QRegion(oldGeometry) - child.geometry());
#if !defined(Q_WS_MAC)
    // should be scrolled in backingstore
    QCOMPARE(child.r, QRegion());
#endif
    VERIFY_COLOR(child.geometry().translated(tlwOffset),
                child.color);
    VERIFY_COLOR(QRegion(parent.geometry()) - child.geometry().translated(tlwOffset),
                parent.color);
}

void tst_QWidget::showAndMoveChild()
{
    QWidget parent(0, Qt::FramelessWindowHint);
    // prevent custom styles
    parent.setStyle(new QWindowsStyle);

    QDesktopWidget desktop;
    QRect desktopDimensions = desktop.availableGeometry(&parent);

    parent.setGeometry(desktopDimensions);
    parent.setPalette(Qt::red);
    parent.show();
    QTest::qWaitForWindowShown(&parent);
    QTest::qWait(10);

    const QPoint tlwOffset = parent.geometry().topLeft();
    QWidget child(&parent);
    child.resize(desktopDimensions.width()/2, desktopDimensions.height()/2);
    child.setPalette(Qt::blue);
    child.setAutoFillBackground(true);

    // Ensure that the child is repainted correctly when moved right after show.
    // NB! Do NOT processEvents() (or qWait()) in between show() and move().
    child.show();
    child.move(desktopDimensions.width()/2, desktopDimensions.height()/2);
    qApp->processEvents();

    VERIFY_COLOR(child.geometry().translated(tlwOffset), Qt::blue);
    VERIFY_COLOR(QRegion(parent.geometry()) - child.geometry().translated(tlwOffset), Qt::red);
}

void tst_QWidget::subtractOpaqueSiblings()
{
#ifdef QT_MAC_USE_COCOA
    QSKIP("Cocoa only has rect granularity.", SkipAll);
#else
    QWidget w;
    w.setGeometry(50, 50, 300, 300);

    ColorWidget *large = new ColorWidget(&w, Qt::red);
    large->setGeometry(50, 50, 200, 200);

    ColorWidget *medium = new ColorWidget(large, Qt::gray);
    medium->setGeometry(50, 50, 100, 100);

    ColorWidget *tall = new ColorWidget(&w, Qt::blue);
    tall->setGeometry(100, 30, 50, 100);

    w.show();
    QTest::qWaitForWindowShown(&w);
    QTest::qWait(10);

    large->reset();
    medium->reset();
    tall->reset();

    medium->update();
    QTest::qWait(10);

    // QWidgetPrivate::subtractOpaqueSiblings() should prevent parts of medium
    // to be repainted and tall from be repainted at all.

    QTRY_COMPARE(large->r, QRegion());
    QTRY_COMPARE(tall->r, QRegion());
    QTRY_COMPARE(medium->r.translated(medium->mapTo(&w, QPoint())),
             QRegion(medium->geometry().translated(large->pos()))
             - tall->geometry());
#endif
}

void tst_QWidget::deleteStyle()
{
    QWidget widget;
    widget.setStyle(new QWindowsStyle);
    widget.show();
    delete widget.style();
    qApp->processEvents();
}

#ifdef Q_WS_WIN
void tst_QWidget::getDC()
{
    QWidget widget;
    widget.setGeometry(0, 0, 2, 4);

    HDC dc = widget.getDC();
    QVERIFY(dc != 0);

    widget.releaseDC(dc);
}
#endif

class TopLevelFocusCheck: public QWidget
{
    Q_OBJECT
public:
    QLineEdit* edit;
    TopLevelFocusCheck(QWidget* parent = 0) : QWidget(parent)
    {
        edit = new QLineEdit(this);
        edit->hide();
        edit->installEventFilter(this);
    }

public slots:
    void mouseDoubleClickEvent ( QMouseEvent * /*event*/ )
    {
        edit->show();
        edit->setFocus(Qt::OtherFocusReason);
        qApp->processEvents();
    }
    bool eventFilter(QObject *obj, QEvent *event)
    {
        if (obj == edit && event->type()== QEvent::FocusOut) {
            edit->hide();
            return true;
        }
        return false;
    }
};

void tst_QWidget::multipleToplevelFocusCheck()
{
    TopLevelFocusCheck w1;
    TopLevelFocusCheck w2;

    w1.resize(200, 200);
    w1.show();
    QTest::qWaitForWindowShown(&w1);
    w2.resize(200,200);
    w2.show();
    QTest::qWaitForWindowShown(&w2);

    QTest::qWait(100);

    QApplication::setActiveWindow(&w1);
    w1.activateWindow();
    QApplication::processEvents();
    QTRY_COMPARE(QApplication::activeWindow(), static_cast<QWidget *>(&w1));
    QTest::qWait(50);
    QTest::mouseDClick(&w1, Qt::LeftButton);
    QTRY_COMPARE(QApplication::focusWidget(), static_cast<QWidget *>(w1.edit));

    w2.activateWindow();
    QApplication::setActiveWindow(&w2);
    QApplication::processEvents();
    QTRY_COMPARE(QApplication::activeWindow(), static_cast<QWidget *>(&w2));
    QTest::mouseClick(&w2, Qt::LeftButton);
#ifdef Q_WS_QWS
    QEXPECT_FAIL("", "embedded toplevels take focus anyway", Continue);
#endif
    QTRY_COMPARE(QApplication::focusWidget(), (QWidget *)0);

    QTest::mouseDClick(&w2, Qt::LeftButton);
    QTRY_COMPARE(QApplication::focusWidget(), static_cast<QWidget *>(w2.edit));

    w1.activateWindow();
    QApplication::setActiveWindow(&w1);
    QApplication::processEvents();
    QTRY_COMPARE(QApplication::activeWindow(), static_cast<QWidget *>(&w1));
    QTest::mouseDClick(&w1, Qt::LeftButton);
    QTRY_COMPARE(QApplication::focusWidget(), static_cast<QWidget *>(w1.edit));

    w2.activateWindow();
    QApplication::setActiveWindow(&w2);
    QApplication::processEvents();
    QTRY_COMPARE(QApplication::activeWindow(), static_cast<QWidget *>(&w2));
    QTest::mouseClick(&w2, Qt::LeftButton);
    QTRY_COMPARE(QApplication::focusWidget(), (QWidget *)0);
}

void tst_QWidget::setFocus()
{
    {
        // move focus to another window
        testWidget->activateWindow();
        QApplication::setActiveWindow(testWidget);
        if (testWidget->focusWidget())
            testWidget->focusWidget()->clearFocus();
        else
            testWidget->clearFocus();

        // window and children never shown, nobody gets focus
        QWidget window;

        QWidget child1(&window);
        child1.setFocusPolicy(Qt::StrongFocus);

        QWidget child2(&window);
        child2.setFocusPolicy(Qt::StrongFocus);

        child1.setFocus();
        QVERIFY(!child1.hasFocus());
        QCOMPARE(window.focusWidget(), &child1);
        QCOMPARE(QApplication::focusWidget(), static_cast<QWidget *>(0));

        child2.setFocus();
        QVERIFY(!child2.hasFocus());
        QCOMPARE(window.focusWidget(), &child2);
        QCOMPARE(QApplication::focusWidget(), static_cast<QWidget *>(0));
    }

    {
        // window and children show, but window not active, nobody gets focus
        QWidget window;

        QWidget child1(&window);
        child1.setFocusPolicy(Qt::StrongFocus);

        QWidget child2(&window);
        child2.setFocusPolicy(Qt::StrongFocus);

        window.show();

        // note: window may be active, but we don't want it to be
        testWidget->activateWindow();
        QApplication::setActiveWindow(testWidget);
        if (testWidget->focusWidget())
            testWidget->focusWidget()->clearFocus();
        else
            testWidget->clearFocus();

        child1.setFocus();
        QVERIFY(!child1.hasFocus());
        QCOMPARE(window.focusWidget(), &child1);
        QCOMPARE(QApplication::focusWidget(), static_cast<QWidget *>(0));

        child2.setFocus();
        QVERIFY(!child2.hasFocus());
        QCOMPARE(window.focusWidget(), &child2);
        QCOMPARE(QApplication::focusWidget(), static_cast<QWidget *>(0));
    }

    {
        // window and children show, but window *is* active, children get focus
        QWidget window;

        QWidget child1(&window);
        child1.setFocusPolicy(Qt::StrongFocus);

        QWidget child2(&window);
        child2.setFocusPolicy(Qt::StrongFocus);

        window.show();
#ifdef Q_WS_X11
        QApplication::setActiveWindow(&window);
        QTest::qWaitForWindowShown(&window);
#else
        window.activateWindow();
        QApplication::processEvents();
#endif

        child1.setFocus();
        QTRY_VERIFY(child1.hasFocus());
        QCOMPARE(window.focusWidget(), &child1);
        QCOMPARE(QApplication::focusWidget(), &child1);

        child2.setFocus();
        QVERIFY(child2.hasFocus());
        QCOMPARE(window.focusWidget(), &child2);
        QCOMPARE(QApplication::focusWidget(), &child2);
    }

    {
        // window shown and active, children created, don't get focus, but get focus when shown
        QWidget window;

        window.show();
#ifdef Q_WS_X11
        QApplication::setActiveWindow(&window);
        QTest::qWaitForWindowShown(&window);
#else
        window.activateWindow();
#endif

        QWidget child1(&window);
        child1.setFocusPolicy(Qt::StrongFocus);

        QWidget child2(&window);
        child2.setFocusPolicy(Qt::StrongFocus);

        child1.setFocus();
        QVERIFY(!child1.hasFocus());
        QCOMPARE(window.focusWidget(), static_cast<QWidget *>(0));
        QCOMPARE(QApplication::focusWidget(), static_cast<QWidget *>(0));

        child1.show();
#ifdef Q_WS_X11
        QApplication::setActiveWindow(&child1);
        child1.activateWindow();
#endif
        QApplication::processEvents();
        QTRY_VERIFY(child1.hasFocus());
        QCOMPARE(window.focusWidget(), &child1);
        QCOMPARE(QApplication::focusWidget(), &child1);

        child2.setFocus();
        QVERIFY(!child2.hasFocus());
        QCOMPARE(window.focusWidget(), &child1);
        QCOMPARE(QApplication::focusWidget(), &child1);

        child2.show();
        QVERIFY(child2.hasFocus());
        QCOMPARE(window.focusWidget(), &child2);
        QCOMPARE(QApplication::focusWidget(), &child2);
    }

    {
        // window shown and active, children created, don't get focus,
        // even after setFocus(), hide(), then show()
        QWidget window;

        window.show();
#ifdef Q_WS_X11
        QApplication::setActiveWindow(&window);
        QTest::qWaitForWindowShown(&window);
#else
        window.activateWindow();
#endif

        QWidget child1(&window);
        child1.setFocusPolicy(Qt::StrongFocus);

        QWidget child2(&window);
        child2.setFocusPolicy(Qt::StrongFocus);

        child1.setFocus();
        QVERIFY(!child1.hasFocus());
        QCOMPARE(window.focusWidget(), static_cast<QWidget *>(0));
        QCOMPARE(QApplication::focusWidget(), static_cast<QWidget *>(0));

        child1.hide();
        QVERIFY(!child1.hasFocus());
        QCOMPARE(window.focusWidget(), static_cast<QWidget *>(0));
        QCOMPARE(QApplication::focusWidget(), static_cast<QWidget *>(0));

        child1.show();
        QVERIFY(!child1.hasFocus());
        QCOMPARE(window.focusWidget(), static_cast<QWidget *>(0));
        QCOMPARE(QApplication::focusWidget(), static_cast<QWidget *>(0));

        child2.setFocus();
        QVERIFY(!child2.hasFocus());
        QCOMPARE(window.focusWidget(), static_cast<QWidget *>(0));
        QCOMPARE(QApplication::focusWidget(), static_cast<QWidget *>(0));

        child2.hide();
        QVERIFY(!child2.hasFocus());
        QCOMPARE(window.focusWidget(), static_cast<QWidget *>(0));
        QCOMPARE(QApplication::focusWidget(), static_cast<QWidget *>(0));

        child2.show();
        QVERIFY(!child2.hasFocus());
        QCOMPARE(window.focusWidget(), static_cast<QWidget *>(0));
        QCOMPARE(QApplication::focusWidget(), static_cast<QWidget *>(0));
    }
}

class EventSpy : public QObject
{
public:
    EventSpy(QWidget *widget, QEvent::Type event)
        : m_widget(widget), eventToSpy(event), m_count(0)
    {
        if (m_widget)
            m_widget->installEventFilter(this);
    }

    QWidget *widget() const { return m_widget; }
    int count() const { return m_count; }
    void clear() { m_count = 0; }

protected:
    bool eventFilter(QObject *object, QEvent *event)
    {
        if (event->type() == eventToSpy)
            ++m_count;
        return  QObject::eventFilter(object, event);
    }

private:
    QWidget *m_widget;
    QEvent::Type eventToSpy;
    int m_count;
};

void tst_QWidget::setCursor()
{
#ifndef QT_NO_CURSOR
    {
        QWidget window;
        QWidget child(&window);

        QVERIFY(!window.testAttribute(Qt::WA_SetCursor));
        QVERIFY(!child.testAttribute(Qt::WA_SetCursor));

        window.setCursor(window.cursor());
        QVERIFY(window.testAttribute(Qt::WA_SetCursor));
        QVERIFY(!child.testAttribute(Qt::WA_SetCursor));
        QCOMPARE(child.cursor().shape(), window.cursor().shape());
    }

    // do it again, but with window show()n
    {
        QWidget window;
        QWidget child(&window);
        window.show();

        QVERIFY(!window.testAttribute(Qt::WA_SetCursor));
        QVERIFY(!child.testAttribute(Qt::WA_SetCursor));

        window.setCursor(window.cursor());
        QVERIFY(window.testAttribute(Qt::WA_SetCursor));
        QVERIFY(!child.testAttribute(Qt::WA_SetCursor));
        QCOMPARE(child.cursor().shape(), window.cursor().shape());
    }


    {
        QWidget window;
        QWidget child(&window);

        window.setCursor(Qt::WaitCursor);
        QVERIFY(window.testAttribute(Qt::WA_SetCursor));
        QVERIFY(!child.testAttribute(Qt::WA_SetCursor));
        QCOMPARE(child.cursor().shape(), window.cursor().shape());
    }

    // same thing again, just with window show()n
    {
        QWidget window;
        QWidget child(&window);

        window.show();
        window.setCursor(Qt::WaitCursor);
        QVERIFY(window.testAttribute(Qt::WA_SetCursor));
        QVERIFY(!child.testAttribute(Qt::WA_SetCursor));
        QCOMPARE(child.cursor().shape(), window.cursor().shape());
    }

    // reparenting child should not cause the WA_SetCursor to become set
    {
        QWidget window;
        QWidget window2;
        QWidget child(&window);

        window.setCursor(Qt::WaitCursor);

        child.setParent(0);
        QVERIFY(!child.testAttribute(Qt::WA_SetCursor));
        QCOMPARE(child.cursor().shape(), QCursor().shape());

        child.setParent(&window2);
        QVERIFY(!child.testAttribute(Qt::WA_SetCursor));
        QCOMPARE(child.cursor().shape(), window2.cursor().shape());

            window2.setCursor(Qt::WaitCursor);
        QVERIFY(!child.testAttribute(Qt::WA_SetCursor));
        QCOMPARE(child.cursor().shape(), window2.cursor().shape());
    }

    // again, with windows show()n
    {
        QWidget window;
        QWidget window2;
        QWidget child(&window);

        window.setCursor(Qt::WaitCursor);
        window.show();

        child.setParent(0);
        QVERIFY(!child.testAttribute(Qt::WA_SetCursor));
        QCOMPARE(child.cursor().shape(), QCursor().shape());

        child.setParent(&window2);
        QVERIFY(!child.testAttribute(Qt::WA_SetCursor));
        QCOMPARE(child.cursor().shape(), window2.cursor().shape());

        window2.show();
        window2.setCursor(Qt::WaitCursor);
        QVERIFY(!child.testAttribute(Qt::WA_SetCursor));
        QCOMPARE(child.cursor().shape(), window2.cursor().shape());
    }

    // test if CursorChange is sent
    {
        QWidget widget;
        EventSpy spy(&widget, QEvent::CursorChange);
        QCOMPARE(spy.count(), 0);
        widget.setCursor(QCursor(Qt::WaitCursor));
        QCOMPARE(spy.count(), 1);
        widget.unsetCursor();
        QCOMPARE(spy.count(), 2);
    }
#endif
}

void tst_QWidget::setToolTip()
{
    QWidget widget;
    EventSpy spy(&widget, QEvent::ToolTipChange);
    QCOMPARE(spy.count(), 0);

    QCOMPARE(widget.toolTip(), QString());
    widget.setToolTip(QString("Hello"));
    QCOMPARE(widget.toolTip(), QString("Hello"));
    QCOMPARE(spy.count(), 1);
    widget.setToolTip(QString());
    QCOMPARE(widget.toolTip(), QString());
    QCOMPARE(spy.count(), 2);

#ifdef Q_OS_WINCE_WM
    QSKIP("Mouse over doesn't work on Windows mobile.", SkipAll);
#endif

    for (int pass = 0; pass < 2; ++pass) {
        QWidget *popup = new QWidget(0, Qt::Popup);
        popup->resize(150, 50);
        QFrame *frame = new QFrame(popup);
        frame->setGeometry(0, 0, 50, 50);
        frame->setFrameStyle(QFrame::Box | QFrame::Plain);
        EventSpy spy1(frame, QEvent::ToolTip);
        EventSpy spy2(popup, QEvent::ToolTip);
        frame->setMouseTracking(pass == 0 ? false : true);
        frame->setToolTip(QLatin1String("TOOLTIP FRAME"));
        popup->setToolTip(QLatin1String("TOOLTIP POPUP"));
        popup->show();
        QTest::qWaitForWindowShown(popup);
        QTest::qWait(10);
        QTest::mouseMove(frame);
        QTest::qWait(900);          // delay is 700

        QCOMPARE(spy1.count(), 1);
        QCOMPARE(spy2.count(), 0);
        if (pass == 0)
            QTest::qWait(2200);     // delay is 2000
        QTest::mouseMove(popup);
        delete popup;
    }
}

void tst_QWidget::testWindowIconChangeEventPropagation()
{
    // Create widget hierarchy.
    QWidget topLevelWidget;
    QWidget topLevelChild(&topLevelWidget);

    QDialog dialog(&topLevelWidget);
    QWidget dialogChild(&dialog);

    QWidgetList widgets;
    widgets << &topLevelWidget << &topLevelChild
            << &dialog << &dialogChild;
    QCOMPARE(widgets.count(), 4);

    // Create spy lists.
    QList <EventSpy *> applicationEventSpies;
    QList <EventSpy *> widgetEventSpies;
    foreach (QWidget *widget, widgets) {
        applicationEventSpies.append(new EventSpy(widget, QEvent::ApplicationWindowIconChange));
        widgetEventSpies.append(new EventSpy(widget, QEvent::WindowIconChange));
    }

    // QApplication::setWindowIcon
    const QIcon windowIcon = qApp->style()->standardIcon(QStyle::SP_TitleBarMenuButton);
    qApp->setWindowIcon(windowIcon);

    for (int i = 0; i < widgets.count(); ++i) {
        // Check QEvent::ApplicationWindowIconChange
        EventSpy *spy = applicationEventSpies.at(i);
        QWidget *widget = spy->widget();
        if (widget->isWindow()) {
            QCOMPARE(spy->count(), 1);
            QCOMPARE(widget->windowIcon(), windowIcon);
        } else {
            QCOMPARE(spy->count(), 0);
        }
        spy->clear();

        // Check QEvent::WindowIconChange
        spy = widgetEventSpies.at(i);
        QCOMPARE(spy->count(), 1);
        spy->clear();
    }

    // Set icon on a top-level widget.
    topLevelWidget.setWindowIcon(*new QIcon);

    for (int i = 0; i < widgets.count(); ++i) {
        // Check QEvent::ApplicationWindowIconChange
        EventSpy *spy = applicationEventSpies.at(i);
        QCOMPARE(spy->count(), 0);
        spy->clear();

        // Check QEvent::WindowIconChange
        spy = widgetEventSpies.at(i);
        QWidget *widget = spy->widget();
        if (widget == &topLevelWidget) {
            QCOMPARE(widget->windowIcon(), QIcon());
            QCOMPARE(spy->count(), 1);
        } else if (topLevelWidget.isAncestorOf(widget)) {
            QCOMPARE(spy->count(), 1);
        } else {
            QCOMPARE(spy->count(), 0);
        }
        spy->clear();
    }

    // Cleanup.
    for (int i = 0; i < widgets.count(); ++i) {
        delete applicationEventSpies.at(i);
        delete widgetEventSpies.at(i);
    }
    qApp->setWindowIcon(QIcon());
}

#ifdef Q_WS_X11
void tst_QWidget::minAndMaxSizeWithX11BypassWindowManagerHint()
{
    // Same size as in QWidget::create_sys().
    const QSize desktopSize = QApplication::desktop()->size();
    const QSize originalSize(desktopSize.width() / 2, desktopSize.height() * 4 / 10);

    { // Maximum size.
    QWidget widget(0, Qt::X11BypassWindowManagerHint);

    const QSize newMaximumSize = widget.size().boundedTo(originalSize) - QSize(10, 10);
    widget.setMaximumSize(newMaximumSize);
    QCOMPARE(widget.size(), newMaximumSize);

    widget.show();
    qt_x11_wait_for_window_manager(&widget);
    QCOMPARE(widget.size(), newMaximumSize);
    }

    { // Minimum size.
    QWidget widget(0, Qt::X11BypassWindowManagerHint);

    const QSize newMinimumSize = widget.size().expandedTo(originalSize) + QSize(10, 10);
    widget.setMinimumSize(newMinimumSize);
    QCOMPARE(widget.size(), newMinimumSize);

    widget.show();
    qt_x11_wait_for_window_manager(&widget);
    QCOMPARE(widget.size(), newMinimumSize);
    }
}

class ShowHideShowWidget : public QWidget
{
    Q_OBJECT

    int state;
public:
    bool gotExpectedMapNotify;

    ShowHideShowWidget()
        : state(0), gotExpectedMapNotify(false)
    {
        startTimer(1000);
    }

    void timerEvent(QTimerEvent *)
    {
        switch (state++) {
        case 0:
            show();
            break;
        case 1:
            emit done();
            break;
        }
    }

    bool x11Event(XEvent *event)
    {
        if (state == 1 && event->type == MapNotify)
            gotExpectedMapNotify = true;
        return false;
    }

signals:
    void done();
};

void tst_QWidget::showHideShow()
{
    ShowHideShowWidget w;
    w.show();
    w.hide();

    QEventLoop eventLoop;
    connect(&w, SIGNAL(done()), &eventLoop, SLOT(quit()));
    eventLoop.exec();

    QVERIFY(w.gotExpectedMapNotify);
}

void tst_QWidget::clean_qt_x11_enforce_cursor()
{
    {
        QWidget window;
        QWidget *w = new QWidget(&window);
        QWidget *child = new QWidget(w);
        child->setAttribute(Qt::WA_SetCursor, true);

        window.show();
        QApplication::setActiveWindow(&window);
        QTest::qWaitForWindowShown(&window);
        QTest::qWait(100);
        QCursor::setPos(window.geometry().center());
        QTest::qWait(100);

        child->setFocus();
        QApplication::processEvents();
        QTest::qWait(100);

        delete w;
    }

    QGraphicsScene scene;
    QLineEdit *edit = new QLineEdit;
    scene.addWidget(edit);

    // If the test didn't crash, then it passed.
}
#endif

class EventRecorder : public QObject
{
    Q_OBJECT

public:
    typedef QList<QPair<QWidget *, QEvent::Type> > EventList;

    EventRecorder(QObject *parent = 0)
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
        QWidget *widget = qobject_cast<QWidget *>(object);
        if (widget && !event->spontaneous())
            events.append(qMakePair(widget, event->type()));
        return false;
    }

private:
    EventList events;
};

void tst_QWidget::compatibilityChildInsertedEvents()
{
    EventRecorder::EventList expected;
    bool accessibilityEnabled = false;
#if defined(Q_WS_MAC) && !defined(QT_MAC_USE_COCOA)
    accessibilityEnabled = AXAPIEnabled();
#endif

    // Move away the cursor; otherwise it might result in an enter event if it's
    // inside the widget when the widget is shown.
    QCursor::setPos(qApp->desktop()->availableGeometry().bottomRight());
    QTest::qWait(100);

    {
        // no children created, not shown
        QWidget widget;
        EventRecorder spy;
        widget.installEventFilter(&spy);

        QCoreApplication::postEvent(&widget, new QEvent(QEvent::Type(QEvent::User + 1)));

        QCoreApplication::sendPostedEvents();

        expected =
            EventRecorder::EventList()
            << qMakePair(&widget, QEvent::PolishRequest)
            << qMakePair(&widget, QEvent::Polish)
            << qMakePair(&widget, QEvent::Type(QEvent::User + 1));
        QCOMPARE(spy.eventList(), expected);
    }

    {
        // no children, shown
        QWidget widget;
        EventRecorder spy;
        widget.installEventFilter(&spy);

        QCoreApplication::postEvent(&widget, new QEvent(QEvent::Type(QEvent::User + 1)));

        widget.show();
        expected =
            EventRecorder::EventList()
            << qMakePair(&widget, QEvent::WinIdChange)
            << qMakePair(&widget, QEvent::Polish)
            << qMakePair(&widget, QEvent::Move)
            << qMakePair(&widget, QEvent::Resize)
            << qMakePair(&widget, QEvent::Show);

        if (accessibilityEnabled)
            expected << qMakePair(&widget, QEvent::AccessibilityPrepare);
        expected << qMakePair(&widget, QEvent::ShowToParent);
        QCOMPARE(spy.eventList(), expected);
        spy.clear();

        QCoreApplication::sendPostedEvents();
        expected =
            EventRecorder::EventList()
            << qMakePair(&widget, QEvent::PolishRequest)
            << qMakePair(&widget, QEvent::Type(QEvent::User + 1))
#if defined(Q_WS_X11) || defined(Q_WS_WIN) || defined(Q_WS_QWS) || defined(Q_WS_S60)
            << qMakePair(&widget, QEvent::UpdateRequest)
#endif
            ;
        QCOMPARE(spy.eventList(), expected);
    }

    {
        // 2 children, not shown
        QWidget widget;
        EventRecorder spy;
        widget.installEventFilter(&spy);

        QCoreApplication::postEvent(&widget, new QEvent(QEvent::Type(QEvent::User + 1)));

        QWidget child1(&widget);
        QWidget child2;
        child2.setParent(&widget);

        QCoreApplication::postEvent(&widget, new QEvent(QEvent::Type(QEvent::User + 2)));

        expected =
            EventRecorder::EventList()
            << qMakePair(&widget, QEvent::ChildAdded)
            << qMakePair(&widget, QEvent::ChildAdded);
        QCOMPARE(spy.eventList(), expected);
        spy.clear();

        QCoreApplication::sendPostedEvents();
        expected =
            EventRecorder::EventList()
#ifdef QT_HAS_QT3SUPPORT
            << qMakePair(&widget, QEvent::ChildInsertedRequest)
            << qMakePair(&widget, QEvent::ChildInserted)
            << qMakePair(&widget, QEvent::ChildInserted)
#endif
            << qMakePair(&widget, QEvent::PolishRequest)
            << qMakePair(&widget, QEvent::Polish)
            << qMakePair(&widget, QEvent::ChildPolished)
            << qMakePair(&widget, QEvent::ChildPolished)
            << qMakePair(&widget, QEvent::Type(QEvent::User + 1))
            << qMakePair(&widget, QEvent::Type(QEvent::User + 2));
        QCOMPARE(spy.eventList(), expected);
    }

    {
        // 2 children, widget shown
        QWidget widget;
        EventRecorder spy;
        widget.installEventFilter(&spy);

        QCoreApplication::postEvent(&widget, new QEvent(QEvent::Type(QEvent::User + 1)));

        QWidget child1(&widget);
        QWidget child2;
        child2.setParent(&widget);

        QCoreApplication::postEvent(&widget, new QEvent(QEvent::Type(QEvent::User + 2)));

        expected =
            EventRecorder::EventList()
            << qMakePair(&widget, QEvent::ChildAdded)
            << qMakePair(&widget, QEvent::ChildAdded);
        QCOMPARE(spy.eventList(), expected);
        spy.clear();

        widget.show();
        expected =
            EventRecorder::EventList()
            << qMakePair(&widget, QEvent::WinIdChange)
            << qMakePair(&widget, QEvent::Polish)
#ifdef QT_HAS_QT3SUPPORT
            << qMakePair(&widget, QEvent::ChildInserted)
            << qMakePair(&widget, QEvent::ChildInserted)
#endif
            << qMakePair(&widget, QEvent::ChildPolished)
            << qMakePair(&widget, QEvent::ChildPolished)
            << qMakePair(&widget, QEvent::Move)
            << qMakePair(&widget, QEvent::Resize)
            << qMakePair(&widget, QEvent::Show);

        if (accessibilityEnabled)
            expected << qMakePair(&widget, QEvent::AccessibilityPrepare);
        expected << qMakePair(&widget, QEvent::ShowToParent);
        QCOMPARE(spy.eventList(), expected);
        spy.clear();

        QCoreApplication::sendPostedEvents();
        expected =
            EventRecorder::EventList()
#ifdef QT_HAS_QT3SUPPORT
            << qMakePair(&widget, QEvent::ChildInsertedRequest)
#endif
            << qMakePair(&widget, QEvent::PolishRequest)
            << qMakePair(&widget, QEvent::Type(QEvent::User + 1))
            << qMakePair(&widget, QEvent::Type(QEvent::User + 2))
#if defined(Q_WS_X11) || defined(Q_WS_WIN) || defined(Q_WS_QWS) || defined(Q_WS_S60)
            << qMakePair(&widget, QEvent::UpdateRequest)
#endif
            ;
        QCOMPARE(spy.eventList(), expected);
    }

    {
        // 2 children, but one is reparented away, not shown
        QWidget widget;
        EventRecorder spy;
        widget.installEventFilter(&spy);

        QCoreApplication::postEvent(&widget, new QEvent(QEvent::Type(QEvent::User + 1)));

        QWidget child1(&widget);
        QWidget child2;
        child2.setParent(&widget);
        child2.setParent(0);

        QCoreApplication::postEvent(&widget, new QEvent(QEvent::Type(QEvent::User + 2)));

        expected =
            EventRecorder::EventList()
            << qMakePair(&widget, QEvent::ChildAdded)
            << qMakePair(&widget, QEvent::ChildAdded)
            << qMakePair(&widget, QEvent::ChildRemoved);
        QCOMPARE(spy.eventList(), expected);
        spy.clear();

        QCoreApplication::sendPostedEvents();
        expected =
            EventRecorder::EventList()
#ifdef QT_HAS_QT3SUPPORT
            << qMakePair(&widget, QEvent::ChildInsertedRequest)
            << qMakePair(&widget, QEvent::ChildInserted)
#endif
            << qMakePair(&widget, QEvent::PolishRequest)
            << qMakePair(&widget, QEvent::Polish)
            << qMakePair(&widget, QEvent::ChildPolished)
            << qMakePair(&widget, QEvent::Type(QEvent::User + 1))
            << qMakePair(&widget, QEvent::Type(QEvent::User + 2));
        QCOMPARE(spy.eventList(), expected);
    }

    {
        // 2 children, but one is reparented away, then widget is shown
        QWidget widget;
        EventRecorder spy;
        widget.installEventFilter(&spy);

        QCoreApplication::postEvent(&widget, new QEvent(QEvent::Type(QEvent::User + 1)));

        QWidget child1(&widget);
        QWidget child2;
        child2.setParent(&widget);
        child2.setParent(0);

        QCoreApplication::postEvent(&widget, new QEvent(QEvent::Type(QEvent::User + 2)));

        expected =
            EventRecorder::EventList()
            << qMakePair(&widget, QEvent::ChildAdded)
            << qMakePair(&widget, QEvent::ChildAdded)
            << qMakePair(&widget, QEvent::ChildRemoved);
        QCOMPARE(spy.eventList(), expected);
        spy.clear();

        widget.show();
        expected =
            EventRecorder::EventList()
            << qMakePair(&widget, QEvent::WinIdChange)
            << qMakePair(&widget, QEvent::Polish)
#ifdef QT_HAS_QT3SUPPORT
            << qMakePair(&widget, QEvent::ChildInserted)
#endif
            << qMakePair(&widget, QEvent::ChildPolished)
            << qMakePair(&widget, QEvent::Move)
            << qMakePair(&widget, QEvent::Resize)
            << qMakePair(&widget, QEvent::Show);

        if (accessibilityEnabled)
            expected << qMakePair(&widget, QEvent::AccessibilityPrepare);
        expected << qMakePair(&widget, QEvent::ShowToParent);
        QCOMPARE(spy.eventList(), expected);
        spy.clear();

        QCoreApplication::sendPostedEvents();
        expected =
            EventRecorder::EventList()
#ifdef QT_HAS_QT3SUPPORT
            << qMakePair(&widget, QEvent::ChildInsertedRequest)
#endif
            << qMakePair(&widget, QEvent::PolishRequest)
            << qMakePair(&widget, QEvent::Type(QEvent::User + 1))
            << qMakePair(&widget, QEvent::Type(QEvent::User + 2))
#if defined(Q_WS_X11) || defined(Q_WS_WIN) || defined(Q_WS_QWS) || defined(Q_WS_S60)
            << qMakePair(&widget, QEvent::UpdateRequest)
#endif
            ;
        QCOMPARE(spy.eventList(), expected);
    }
}

class RenderWidget : public QWidget
{
public:
    RenderWidget(QWidget *source)
        : source(source), ellipse(false) {}

    void setEllipseEnabled(bool enable = true)
    {
        ellipse = enable;
        update();
    }

protected:
    void paintEvent(QPaintEvent *)
    {
        if (ellipse) {
            QPainter painter(this);
            painter.fillRect(rect(), Qt::red);
            painter.end();
            QRegion regionToRender = QRegion(0, 0, source->width(), source->height() / 2,
                                             QRegion::Ellipse);
            source->render(this, QPoint(0, 30), regionToRender);
        } else {
            source->render(this);
        }
    }

private:
    QWidget *source;
    bool ellipse;
};

void tst_QWidget::render()
{
    return;
    QCalendarWidget source;
    // disable anti-aliasing to eliminate potential differences when subpixel antialiasing
    // is enabled on the screen
    QFont f;
    f.setStyleStrategy(QFont::NoAntialias);
    source.setFont(f);
    source.show();
    QTest::qWaitForWindowShown(&source);

    // Render the entire source into target.
    RenderWidget target(&source);
    target.resize(source.size());
    target.show();

    qApp->processEvents();
    qApp->sendPostedEvents();
    QTest::qWait(250);

    QImage sourceImage = QPixmap::grabWidget(&source).toImage();
    qApp->processEvents();
    QImage targetImage = QPixmap::grabWidget(&target).toImage();
    qApp->processEvents();
    QCOMPARE(sourceImage, targetImage);

    // Fill target.rect() will Qt::red and render
    // QRegion(0, 0, source->width(), source->height() / 2, QRegion::Ellipse)
    // of source into target with offset (0, 30).
    target.setEllipseEnabled();
    qApp->processEvents();
    qApp->sendPostedEvents();

    targetImage = QPixmap::grabWidget(&target).toImage();
    QVERIFY(sourceImage != targetImage);

    QCOMPARE(targetImage.pixel(target.width() / 2, 29), QColor(Qt::red).rgb());
    QCOMPARE(targetImage.pixel(target.width() / 2, 30), sourceImage.pixel(source.width() / 2, 0));

    // Test that a child widget properly fills its background
    {
        QWidget window;
        window.resize(100, 100);
        // prevent custom styles
        window.setStyle(new QWindowsStyle);
        window.show();
        QTest::qWaitForWindowShown(&window);
        QWidget child(&window);
        child.resize(window.size());
        child.show();

        qApp->processEvents();
        QCOMPARE(QPixmap::grabWidget(&child), QPixmap::grabWidget(&window));
    }

    { // Check that the target offset is correct.
        QWidget widget;
        widget.resize(200, 200);
        widget.setAutoFillBackground(true);
        widget.setPalette(Qt::red);
        // prevent custom styles
        widget.setStyle(new QWindowsStyle);
        widget.show();
        QTest::qWaitForWindowShown(&widget);
        QImage image(widget.size(), QImage::Format_RGB32);
        image.fill(QColor(Qt::blue).rgb());

        // Target offset (0, 0)
        widget.render(&image, QPoint(), QRect(20, 20, 100, 100));
        QCOMPARE(image.pixel(0, 0), QColor(Qt::red).rgb());
        QCOMPARE(image.pixel(99, 99), QColor(Qt::red).rgb());
        QCOMPARE(image.pixel(100, 100), QColor(Qt::blue).rgb());

        // Target offset (20, 20).
        image.fill(QColor(Qt::blue).rgb());
        widget.render(&image, QPoint(20, 20), QRect(20, 20, 100, 100));
        QCOMPARE(image.pixel(0, 0), QColor(Qt::blue).rgb());
        QCOMPARE(image.pixel(19, 19), QColor(Qt::blue).rgb());
        QCOMPARE(image.pixel(20, 20), QColor(Qt::red).rgb());
        QCOMPARE(image.pixel(119, 119), QColor(Qt::red).rgb());
        QCOMPARE(image.pixel(120, 120), QColor(Qt::blue).rgb());
    }
}

// On Windows the active palette is used instead of the inactive palette even
// though the widget is invisible. This is probably related to task 178507/168682,
// but for the renderInvisible test it doesn't matter, we're mostly interested
// in testing the geometry so just workaround the palette issue for now.
static void workaroundPaletteIssue(QWidget *widget)
{
#ifndef Q_WS_WIN
    return;
#endif
    if (!widget)
        return;

    QWidget *navigationBar = qFindChild<QWidget *>(widget, QLatin1String("qt_calendar_navigationbar"));
    QVERIFY(navigationBar);

    QPalette palette = navigationBar->palette();
    const QColor background = palette.color(QPalette::Inactive, navigationBar->backgroundRole());
    const QColor highlightedText = palette.color(QPalette::Inactive, QPalette::HighlightedText);
    palette.setColor(QPalette::Active, navigationBar->backgroundRole(), background);
    palette.setColor(QPalette::Active, QPalette::HighlightedText, highlightedText);
    navigationBar->setPalette(palette);
}

//#define RENDER_DEBUG
void tst_QWidget::renderInvisible()
{
    QCalendarWidget *calendar = new QCalendarWidget;
    // disable anti-aliasing to eliminate potential differences when subpixel antialiasing
    // is enabled on the screen
    QFont f;
    f.setStyleStrategy(QFont::NoAntialias);
    calendar->setFont(f);
    calendar->show();
    QTest::qWaitForWindowShown(calendar);

    // Create a dummy focus widget to get rid of focus rect in reference image.
    QLineEdit dummyFocusWidget;
    dummyFocusWidget.show();
    QTest::qWaitForWindowShown(&dummyFocusWidget);
    qApp->processEvents();
    QTest::qWait(120);

    // Create normal reference image.
    const QSize calendarSize = calendar->size();
    QImage referenceImage(calendarSize, QImage::Format_ARGB32);
    calendar->render(&referenceImage);
#ifdef RENDER_DEBUG
    referenceImage.save("referenceImage.png");
#endif
    QVERIFY(!referenceImage.isNull());

    // Create resized reference image.
    const QSize calendarSizeResized = calendar->size() + QSize(50, 50);
    calendar->resize(calendarSizeResized);
    qApp->processEvents();
    QTest::qWait(30);
    QImage referenceImageResized(calendarSizeResized, QImage::Format_ARGB32);
    calendar->render(&referenceImageResized);
#ifdef RENDER_DEBUG
    referenceImageResized.save("referenceImageResized.png");
#endif
    QVERIFY(!referenceImageResized.isNull());

    // Explicitly hide the calendar.
    calendar->hide();
    qApp->processEvents();
    QTest::qWait(30);
    workaroundPaletteIssue(calendar);

    { // Make sure we get the same image when the calendar is explicitly hidden.
    QImage testImage(calendarSizeResized, QImage::Format_ARGB32);
    calendar->render(&testImage);
#ifdef RENDER_DEBUG
    testImage.save("explicitlyHiddenCalendarResized.png");
#endif
    QCOMPARE(testImage, referenceImageResized);
    }

    // Now that we have reference images we can delete the source and re-create
    // the calendar and check that we get the same images from a calendar which has never
    // been visible, laid out or created (Qt::WA_WState_Created).
    delete calendar;
    calendar = new QCalendarWidget;
    calendar->setFont(f);
    workaroundPaletteIssue(calendar);

    { // Never been visible, created or laid out.
    QImage testImage(calendarSize, QImage::Format_ARGB32);
    calendar->render(&testImage);
#ifdef RENDER_DEBUG
    testImage.save("neverBeenVisibleCreatedOrLaidOut.png");
#endif
    QCOMPARE(testImage, referenceImage);
    }

    calendar->hide();
    qApp->processEvents();
    QTest::qWait(30);

    { // Calendar explicitly hidden.
    QImage testImage(calendarSize, QImage::Format_ARGB32);
    calendar->render(&testImage);
#ifdef RENDER_DEBUG
    testImage.save("explicitlyHiddenCalendar.png");
#endif
    QCOMPARE(testImage, referenceImage);
    }

    // Get navigation bar and explicitly hide it.
    QWidget *navigationBar = qFindChild<QWidget *>(calendar, QLatin1String("qt_calendar_navigationbar"));
    QVERIFY(navigationBar);
    navigationBar->hide();

    { // Check that the navigation bar isn't drawn when rendering the entire calendar.
    QImage testImage(calendarSize, QImage::Format_ARGB32);
    calendar->render(&testImage);
#ifdef RENDER_DEBUG
    testImage.save("calendarWithoutNavigationBar.png");
#endif
    QVERIFY(testImage != referenceImage);
    }

    { // Make sure the navigation bar renders correctly even though it's hidden.
    QImage testImage(navigationBar->size(), QImage::Format_ARGB32);
    navigationBar->render(&testImage);
#ifdef RENDER_DEBUG
    testImage.save("explicitlyHiddenNavigationBar.png");
#endif
    QCOMPARE(testImage, referenceImage.copy(navigationBar->rect()));
    }

    // Get next month button.
    QWidget *nextMonthButton = qFindChild<QWidget *>(navigationBar, QLatin1String("qt_calendar_nextmonth"));
    QVERIFY(nextMonthButton);

    { // Render next month button.
    // Fill test image with correct background color.
    QImage testImage(nextMonthButton->size(), QImage::Format_ARGB32);
    navigationBar->render(&testImage, QPoint(), QRegion(), QWidget::RenderFlags());
#ifdef RENDER_DEBUG
    testImage.save("nextMonthButtonBackground.png");
#endif

    // Set the button's background color to Qt::transparent; otherwise it will fill the
    // background with QPalette::Window.
    const QPalette originalPalette = nextMonthButton->palette();
    QPalette palette = originalPalette;
    palette.setColor(QPalette::Window, Qt::transparent);
    nextMonthButton->setPalette(palette);

    // Render the button on top of the background.
    nextMonthButton->render(&testImage);
#ifdef RENDER_DEBUG
    testImage.save("nextMonthButton.png");
#endif
    const QRect buttonRect(nextMonthButton->mapTo(calendar, QPoint()), nextMonthButton->size());
    QCOMPARE(testImage, referenceImage.copy(buttonRect));

    // Restore palette.
    nextMonthButton->setPalette(originalPalette);
    }

    // Navigation bar isn't explicitly hidden anymore.
    navigationBar->show();
    qApp->processEvents();
    QTest::qWait(30);
    QVERIFY(!calendar->isVisible());

    // Now, completely mess up the layout. This will trigger an update on the layout
    // when the calendar is visible or shown, but it's not. QWidget::render must therefore
    // make sure the layout is activated before rendering.
    QVERIFY(!calendar->isVisible());
    calendar->resize(calendarSizeResized);
    qApp->processEvents();

    { // Make sure we get an image equal to the resized reference image.
    QImage testImage(calendarSizeResized, QImage::Format_ARGB32);
    calendar->render(&testImage);
#ifdef RENDER_DEBUG
    testImage.save("calendarResized.png");
#endif
    QCOMPARE(testImage, referenceImageResized);
    }

    { // Make sure we lay out the widget correctly the first time it's rendered.
    QCalendarWidget calendar;
    const QSize calendarSize = calendar.sizeHint();

    QImage image(2 * calendarSize, QImage::Format_ARGB32);
    image.fill(QColor(Qt::red).rgb());
    calendar.render(&image);

    for (int i = calendarSize.height(); i < 2 * calendarSize.height(); ++i)
        for (int j = calendarSize.width(); j < 2 * calendarSize.width(); ++j)
            QCOMPARE(image.pixel(j, i), QColor(Qt::red).rgb());
    }

    { // Ensure that we don't call adjustSize() on invisible top-levels if render() is called
      // right after widgets have been added/removed to/from its layout.
    QWidget topLevel;
    topLevel.setLayout(new QVBoxLayout);

    QWidget *widget = new QLineEdit;
    topLevel.layout()->addWidget(widget);

    const QSize initialSize = topLevel.size();
    QPixmap pixmap(topLevel.sizeHint());
    topLevel.render(&pixmap); // triggers adjustSize()
    const QSize finalSize = topLevel.size();
    QVERIFY(finalSize != initialSize);

    topLevel.layout()->removeWidget(widget);
    QCOMPARE(topLevel.size(), finalSize);
    topLevel.render(&pixmap);
    QCOMPARE(topLevel.size(), finalSize);

    topLevel.layout()->addWidget(widget);
    QCOMPARE(topLevel.size(), finalSize);
    topLevel.render(&pixmap);
    QCOMPARE(topLevel.size(), finalSize);
    }
}

void tst_QWidget::renderWithPainter()
{
    QWidget widget;
    // prevent custom styles
    widget.setStyle(new QWindowsStyle);
    widget.show();
    widget.resize(70, 50);
    widget.setAutoFillBackground(true);
    widget.setPalette(Qt::black);

    // Render the entire widget onto the image.
    QImage image(QSize(70, 50), QImage::Format_ARGB32);
    image.fill(QColor(Qt::red).rgb());
    QPainter painter(&image);
    widget.render(&painter);

    for (int i = 0; i < image.height(); ++i) {
        for (int j = 0; j < image.width(); ++j)
            QCOMPARE(image.pixel(j, i), QColor(Qt::black).rgb());
    }

    // Translate painter (10, 10).
    painter.save();
    image.fill(QColor(Qt::red).rgb());
    painter.translate(10, 10);
    widget.render(&painter);
    painter.restore();

    for (int i = 0; i < image.height(); ++i) {
        for (int j = 0; j < image.width(); ++j) {
            if (i < 10 || j < 10)
                QCOMPARE(image.pixel(j, i), QColor(Qt::red).rgb());
            else
                QCOMPARE(image.pixel(j, i), QColor(Qt::black).rgb());
        }
    }

    // Pass target offset (10, 10) (the same as QPainter::translate).
    image.fill(QColor(Qt::red).rgb());
    widget.render(&painter, QPoint(10, 10));

    for (int i = 0; i < image.height(); ++i) {
        for (int j = 0; j < image.width(); ++j) {
            if (i < 10 || j < 10)
                QCOMPARE(image.pixel(j, i), QColor(Qt::red).rgb());
            else
                QCOMPARE(image.pixel(j, i), QColor(Qt::black).rgb());
        }
    }

    // Translate (10, 10) and pass target offset (10, 10).
    painter.save();
    image.fill(QColor(Qt::red).rgb());
    painter.translate(10, 10);
    widget.render(&painter, QPoint(10, 10));
    painter.restore();

    for (int i = 0; i < image.height(); ++i) {
        for (int j = 0; j < image.width(); ++j) {
            if (i < 20 || j < 20)
                QCOMPARE(image.pixel(j, i), QColor(Qt::red).rgb());
            else
                QCOMPARE(image.pixel(j, i), QColor(Qt::black).rgb());
        }
    }

    // Rotate painter 90 degrees.
    painter.save();
    image.fill(QColor(Qt::red).rgb());
    painter.rotate(90);
    widget.render(&painter);
    painter.restore();

    for (int i = 0; i < image.height(); ++i) {
        for (int j = 0; j < image.width(); ++j)
            QCOMPARE(image.pixel(j, i), QColor(Qt::red).rgb());
    }

    // Translate and rotate.
    image.fill(QColor(Qt::red).rgb());
    widget.resize(40, 10);
    painter.translate(10, 10);
    painter.rotate(90);
    widget.render(&painter);

    for (int i = 0; i < image.height(); ++i) {
        for (int j = 0; j < image.width(); ++j) {
            if (i >= 10 && j >= 0 && j < 10)
                QCOMPARE(image.pixel(j, i), QColor(Qt::black).rgb());
            else
                QCOMPARE(image.pixel(j, i), QColor(Qt::red).rgb());
        }
    }

    // Make sure QWidget::render does not modify the render hints set on the painter.
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform
                           | QPainter::NonCosmeticDefaultPen | QPainter::TextAntialiasing);
    QPainter::RenderHints oldRenderHints = painter.renderHints();
    widget.render(&painter);
    QCOMPARE(painter.renderHints(), oldRenderHints);
}

void tst_QWidget::render_task188133()
{
    QMainWindow mainWindow;
#if defined(QT3_SUPPORT)
    mainWindow.setCentralWidget(new Q3TextEdit);
#endif
    // Make sure QWidget::render does not trigger QWidget::repaint/update
    // and asserts for Qt::WA_WState_Created.
    QPixmap pixmap = QPixmap::grabWidget(&mainWindow);
}

void tst_QWidget::render_task211796()
{
    class MyWidget : public QWidget
    {
        void resizeEvent(QResizeEvent *)
        {
            QPixmap pixmap(size());
            render(&pixmap);
        }
    };

    { // Please don't die in a resize recursion.
        MyWidget widget;
        widget.resize(200, 200);
        widget.show();
    }

    { // Same check with a deeper hierarchy.
        QWidget widget;
        widget.show();
        QWidget child(&widget);
        MyWidget grandChild;
        grandChild.setParent(&child);
        grandChild.resize(100, 100);
        child.show();
    }
}

void tst_QWidget::render_task217815()
{
    // Make sure we don't change the size of the widget when calling
    // render() and the widget has an explicit size set.
    // This was a problem on Windows because we called createWinId(),
    // which in turn enforced the size to be bigger than the smallest
    // possible native window size (which is (115,something) on WinXP).
    QWidget widget;
    const QSize explicitSize(80, 20);
    widget.resize(explicitSize);
    QCOMPARE(widget.size(), explicitSize);

    QPixmap pixmap(explicitSize);
    widget.render(&pixmap);

    QCOMPARE(widget.size(), explicitSize);
}

void tst_QWidget::render_windowOpacity()
{
#ifdef Q_OS_WINCE
    QSKIP("Window Opacity is not supported on Windows CE", SkipAll);
#endif

    const qreal opacity = 0.5;

    { // Check that the painter opacity effects the widget drawing.
    QWidget topLevel;
    QWidget child(&topLevel);
    child.resize(50, 50);
    child.setPalette(Qt::red);
    child.setAutoFillBackground(true);

    QPixmap expected(child.size());
#ifdef Q_WS_X11
    if (expected.depth() < 24) {
        QSKIP("This test won't give correct results with dithered pixmaps", SkipAll);
    }
#endif
    expected.fill(Qt::green);
    QPainter painter(&expected);
    painter.setOpacity(opacity);
    painter.fillRect(QRect(QPoint(0, 0), child.size()), Qt::red);
    painter.end();

    QPixmap result(child.size());
    result.fill(Qt::green);
    painter.begin(&result);
    painter.setOpacity(opacity);
    child.render(&painter);
    painter.end();
    QCOMPARE(result, expected);
    }

    { // Combine the opacity set on the painter with the widget opacity.
    class MyWidget : public QWidget
    {
    public:
        void paintEvent(QPaintEvent *)
        {
            QPainter painter(this);
            painter.setOpacity(opacity);
            QCOMPARE(painter.opacity(), opacity);
            painter.fillRect(rect(), Qt::red);
        }
        qreal opacity;
    };

    MyWidget widget;
    widget.resize(50, 50);
    widget.opacity = opacity;
    widget.setPalette(Qt::blue);
    widget.setAutoFillBackground(true);

    QPixmap expected(widget.size());
    expected.fill(Qt::green);
    QPainter painter(&expected);
    painter.setOpacity(opacity);
    QPixmap pixmap(widget.size());
    pixmap.fill(Qt::blue);
    QPainter pixmapPainter(&pixmap);
    pixmapPainter.setOpacity(opacity);
    pixmapPainter.fillRect(QRect(QPoint(), widget.size()), Qt::red);
    painter.drawPixmap(QPoint(), pixmap);
    painter.end();

    QPixmap result(widget.size());
    result.fill(Qt::green);
    painter.begin(&result);
    painter.setOpacity(opacity);
    widget.render(&painter);
    painter.end();
    QCOMPARE(result, expected);
    }
}

void tst_QWidget::render_systemClip()
{
    QWidget widget;
    widget.setPalette(Qt::blue);
    widget.resize(100, 100);

    QImage image(widget.size(), QImage::Format_RGB32);
    image.fill(QColor(Qt::red).rgb());

    QPaintEngine *paintEngine = image.paintEngine();
    QVERIFY(paintEngine);
    paintEngine->setSystemClip(QRegion(0, 0, 50, 50));

    QPainter painter(&image);
    // Make sure we're using the same paint engine and has the right clip set.
    QCOMPARE(painter.paintEngine(), paintEngine);
    QCOMPARE(paintEngine->systemClip(), QRegion(0, 0, 50, 50));

    // Translate painter outside system clip.
    painter.translate(50, 0);
    widget.render(&painter);

#ifdef RENDER_DEBUG
    image.save("outside_systemclip.png");
#endif

    // All pixels should be red.
    for (int i = 0; i < image.height(); ++i) {
        for (int j = 0; j < image.width(); ++j)
            QCOMPARE(image.pixel(j, i), QColor(Qt::red).rgb());
    }

    // Restore painter and refill image with red.
    image.fill(QColor(Qt::red).rgb());
    painter.translate(-50, 0);

    // Set transform on the painter.
    QTransform transform;
    transform.shear(0, 1);
    painter.setTransform(transform);
    widget.render(&painter);

#ifdef RENDER_DEBUG
    image.save("blue_triangle.png");
#endif

    // We should now have a blue triangle starting at scan line 1, and the rest should be red.
    // rrrrrrrrrr
    // brrrrrrrrr
    // bbrrrrrrrr
    // bbbrrrrrrr
    // bbbbrrrrrr
    // rrrrrrrrrr
    // ...

#ifndef Q_WS_MAC
    for (int i = 0; i < image.height(); ++i) {
        for (int j = 0; j < image.width(); ++j) {
            if (i < 50 && j < i)
                QCOMPARE(image.pixel(j, i), QColor(Qt::blue).rgb());
            else
                QCOMPARE(image.pixel(j, i), QColor(Qt::red).rgb());
        }
    }
#else
    // We don't paint directly on the image on the Mac, so we cannot do the pixel comparison
    // as above due to QPainter::SmoothPixmapTransform. We therefore need to generate an
    // expected image by first painting on a pixmap, and then draw the pixmap onto
    // the image using QPainter::SmoothPixmapTransform. Then we can compare pixels :)
    // The check is basically the same, except that it takes the smoothening into account.
    QPixmap pixmap(50, 50);
    const QRegion sysClip(0, 0, 50, 50);
    widget.render(&pixmap, QPoint(), sysClip);

    QImage expectedImage(widget.size(), QImage::Format_RGB32);
    expectedImage.fill(QColor(Qt::red).rgb());
    expectedImage.paintEngine()->setSystemClip(sysClip);

    QPainter expectedImagePainter(&expectedImage);
    expectedImagePainter.setTransform(QTransform().shear(0, 1));
    // NB! This is the important part (SmoothPixmapTransform).
    expectedImagePainter.setRenderHints(QPainter::SmoothPixmapTransform);
    expectedImagePainter.drawPixmap(QPoint(0, 0), pixmap);
    expectedImagePainter.end();

    QCOMPARE(image, expectedImage);
#endif
}

void tst_QWidget::render_systemClip2_data()
{
    QTest::addColumn<bool>("autoFillBackground");
    QTest::addColumn<bool>("usePaintEvent");
    QTest::addColumn<QColor>("expectedColor");

    QTest::newRow("Only auto-fill background") << true << false << QColor(Qt::blue);
    QTest::newRow("Only draw in paintEvent") << false << true << QColor(Qt::green);
    QTest::newRow("Auto-fill background and draw in paintEvent") << true << true << QColor(Qt::green);
}

void tst_QWidget::render_systemClip2()
{
    QFETCH(bool, autoFillBackground);
    QFETCH(bool, usePaintEvent);
    QFETCH(QColor, expectedColor);

    Q_ASSERT_X(expectedColor != QColor(Qt::red), Q_FUNC_INFO,
               "Qt::red is the reference color for the image, pick another color");

    class MyWidget : public QWidget
    {
    public:
        bool usePaintEvent;
        void paintEvent(QPaintEvent *)
        {
            if (usePaintEvent)
                QPainter(this).fillRect(rect(), Qt::green);
        }
    };

    MyWidget widget;
    widget.usePaintEvent = usePaintEvent;
    widget.setPalette(Qt::blue);
    // NB! widget.setAutoFillBackground(autoFillBackground) won't do the
    // trick here since the widget is a top-level. The background is filled
    // regardless, unless Qt::WA_OpaquePaintEvent or Qt::WA_NoSystemBackground
    // is set. We therefore use the opaque attribute to turn off auto-fill.
    if (!autoFillBackground)
        widget.setAttribute(Qt::WA_OpaquePaintEvent);
    widget.resize(100, 100);

    QImage image(widget.size(), QImage::Format_RGB32);
    image.fill(QColor(Qt::red).rgb());

    QPaintEngine *paintEngine = image.paintEngine();
    QVERIFY(paintEngine);

    QRegion systemClip(QRegion(50, 0, 50, 10));
    systemClip += QRegion(90, 10, 10, 40);
    paintEngine->setSystemClip(systemClip);

    // Render entire widget directly onto device.
    widget.render(&image);

#ifdef RENDER_DEBUG
    image.save("systemclip_with_device.png");
#endif
    // All pixels within the system clip should now be
    // the expectedColor, and the rest should be red.
    for (int i = 0; i < image.height(); ++i) {
        for (int j = 0; j < image.width(); ++j) {
            if (systemClip.contains(QPoint(j, i)))
                QCOMPARE(image.pixel(j, i), expectedColor.rgb());
            else
                QCOMPARE(image.pixel(j, i), QColor(Qt::red).rgb());
        }
    }

    // Refill image with red.
    image.fill(QColor(Qt::red).rgb());
    paintEngine->setSystemClip(systemClip);

    // Do the same with an untransformed painter.
    QPainter painter(&image);
    //Make sure we're using the same paint engine and has the right clip set.
    QCOMPARE(painter.paintEngine(), paintEngine);
    QCOMPARE(paintEngine->systemClip(), systemClip);

    widget.render(&painter);

#ifdef RENDER_DEBUG
    image.save("systemclip_with_untransformed_painter.png");
#endif
    // All pixels within the system clip should now be
    // the expectedColor, and the rest should be red.
    for (int i = 0; i < image.height(); ++i) {
        for (int j = 0; j < image.width(); ++j) {
            if (systemClip.contains(QPoint(j, i)))
                QCOMPARE(image.pixel(j, i), expectedColor.rgb());
            else
                QCOMPARE(image.pixel(j, i), QColor(Qt::red).rgb());
        }
    }
}

void tst_QWidget::render_systemClip3_data()
{
    QTest::addColumn<QSize>("size");
    QTest::addColumn<bool>("useSystemClip");

    // Reference: http://en.wikipedia.org/wiki/Flag_of_Norway
    QTest::newRow("Norwegian Civil Flag") << QSize(220, 160) << false;
    QTest::newRow("Norwegian War Flag") << QSize(270, 160) << true;
}

// This test ensures that the current engine clip (systemClip + painter clip)
// is preserved after QPainter::setClipRegion(..., Qt::ReplaceClip);
void tst_QWidget::render_systemClip3()
{
    QFETCH(QSize, size);
    QFETCH(bool, useSystemClip);

    // Calculate the inner/outer cross of the flag.
    QRegion outerCross(0, 0, size.width(), size.height());
    outerCross -= QRect(0, 0, 60, 60);
    outerCross -= QRect(100, 0, size.width() - 100, 60);
    outerCross -= QRect(0, 100, 60, 60);
    outerCross -= QRect(100, 100, size.width() - 100, 60);

    QRegion innerCross(0, 0, size.width(), size.height());
    innerCross -= QRect(0, 0, 70, 70);
    innerCross -= QRect(90, 0, size.width() - 90, 70);
    innerCross -= QRect(0, 90, 70, 70);
    innerCross -= QRect(90, 90, size.width() - 90, 70);

    const QRegion redArea(QRegion(0, 0, size.width(), size.height()) - outerCross);
    const QRegion whiteArea(outerCross - innerCross);
    const QRegion blueArea(innerCross);
    QRegion systemClip;

    // Okay, here's the image that should look like a Norwegian civil/war flag in the end.
    QImage flag(size, QImage::Format_ARGB32);
    flag.fill(QColor(Qt::transparent).rgba());

    if (useSystemClip) {
        QPainterPath warClip(QPoint(size.width(), 0));
        warClip.lineTo(size.width() - 110, 60);
        warClip.lineTo(size.width(), 80);
        warClip.lineTo(size.width() - 110, 100);
        warClip.lineTo(size.width(), 160);
        warClip.closeSubpath();
        systemClip = QRegion(0, 0, size.width(), size.height()) - QRegion(warClip.toFillPolygon().toPolygon());
        flag.paintEngine()->setSystemClip(systemClip);
    }

    QPainter painter(&flag);
    painter.fillRect(QRect(QPoint(), size), Qt::red); // Fill image background with red.
    painter.setClipRegion(outerCross); // Limit widget painting to inside the outer cross.

    // Here's the widget that's supposed to draw the inner/outer cross of the flag.
    // The outer cross (white) should be drawn when the background is auto-filled, and
    // the inner cross (blue) should be drawn in the paintEvent.
    class MyWidget : public QWidget
    { public:
        void paintEvent(QPaintEvent *)
        {
            QPainter painter(this);
            // Be evil and try to paint outside the outer cross. This should not be
            // possible since the shared painter is clipped to the outer cross.
            painter.setClipRect(0, 0, 60, 60, Qt::ReplaceClip);
            painter.fillRect(rect(), Qt::green);
            painter.setClipRegion(clip, Qt::ReplaceClip);
            painter.fillRect(rect(), Qt::blue);
        }
        QRegion clip;
    };

    MyWidget widget;
    widget.clip = innerCross;
    widget.setFixedSize(size);
    widget.setPalette(Qt::white);
    widget.setAutoFillBackground(true);
    widget.render(&painter);

#ifdef RENDER_DEBUG
    flag.save("flag.png");
#endif

    // Let's make sure we got a Norwegian flag.
    for (int i = 0; i < flag.height(); ++i) {
        for (int j = 0; j < flag.width(); ++j) {
            const QPoint pixel(j, i);
            const QRgb pixelValue = flag.pixel(pixel);
            if (useSystemClip && !systemClip.contains(pixel))
                QCOMPARE(pixelValue, QColor(Qt::transparent).rgba());
            else if (redArea.contains(pixel))
                QCOMPARE(pixelValue, QColor(Qt::red).rgba());
            else if (whiteArea.contains(pixel))
                QCOMPARE(pixelValue, QColor(Qt::white).rgba());
            else
                QCOMPARE(pixelValue, QColor(Qt::blue).rgba());
        }
    }
}

void tst_QWidget::render_task252837()
{
    QWidget widget;
    widget.resize(200, 200);

    QPixmap pixmap(widget.size());
    QPainter painter(&pixmap);
    // Please do not crash.
    widget.render(&painter);
}

void tst_QWidget::render_worldTransform()
{
    class MyWidget : public QWidget
    { public:
        void paintEvent(QPaintEvent *)
        {
            QPainter painter(this);
            // Make sure world transform is identity.
            QCOMPARE(painter.worldTransform(), QTransform());

            // Make sure device transform is correct.
            const QPoint widgetOffset = geometry().topLeft();
            QTransform expectedDeviceTransform = QTransform::fromTranslate(105, 5);
            expectedDeviceTransform.rotate(90);
            expectedDeviceTransform.translate(widgetOffset.x(), widgetOffset.y());
            QCOMPARE(painter.deviceTransform(), expectedDeviceTransform);

            // Set new world transform.
            QTransform newWorldTransform = QTransform::fromTranslate(10, 10);
            newWorldTransform.rotate(90);
            painter.setWorldTransform(newWorldTransform);
            QCOMPARE(painter.worldTransform(), newWorldTransform);

            // Again, check device transform.
            expectedDeviceTransform.translate(10, 10);
            expectedDeviceTransform.rotate(90);
            QCOMPARE(painter.deviceTransform(), expectedDeviceTransform);

            painter.fillRect(QRect(0, 0, 20, 10), Qt::green);
        }
    };

    MyWidget widget;
    widget.setFixedSize(100, 100);
    widget.setPalette(Qt::red);
    widget.setAutoFillBackground(true);

    MyWidget child;
    child.setParent(&widget);
    child.move(50, 50);
    child.setFixedSize(50, 50);
    child.setPalette(Qt::blue);
    child.setAutoFillBackground(true);

    QImage image(QSize(110, 110), QImage::Format_RGB32);
    image.fill(QColor(Qt::black).rgb());

    QPainter painter(&image);
    painter.translate(105, 5);
    painter.rotate(90);

    const QTransform worldTransform = painter.worldTransform();
    const QTransform deviceTransform = painter.deviceTransform();

    // Render widgets onto image.
    widget.render(&painter);
#ifdef RENDER_DEBUG
    image.save("render_worldTransform_image.png");
#endif

    // Ensure the transforms are unchanged after render.
    QCOMPARE(painter.worldTransform(), painter.worldTransform());
    QCOMPARE(painter.deviceTransform(), painter.deviceTransform());
    painter.end();

    // Paint expected image.
    QImage expected(QSize(110, 110), QImage::Format_RGB32);
    expected.fill(QColor(Qt::black).rgb());

    QPainter expectedPainter(&expected);
    expectedPainter.translate(105, 5);
    expectedPainter.rotate(90);
    expectedPainter.save();
    expectedPainter.fillRect(widget.rect(),Qt::red);
    expectedPainter.translate(10, 10);
    expectedPainter.rotate(90);
    expectedPainter.fillRect(QRect(0, 0, 20, 10), Qt::green);
    expectedPainter.restore();
    expectedPainter.translate(50, 50);
    expectedPainter.fillRect(child.rect(),Qt::blue);
    expectedPainter.translate(10, 10);
    expectedPainter.rotate(90);
    expectedPainter.fillRect(QRect(0, 0, 20, 10), Qt::green);
    expectedPainter.end();

#ifdef RENDER_DEBUG
    expected.save("render_worldTransform_expected.png");
#endif

    QCOMPARE(image, expected);
}

void tst_QWidget::setContentsMargins()
{
    QLabel label("why does it always rain on me?");
    QSize oldSize = label.sizeHint();
    label.setFrameStyle(QFrame::Sunken | QFrame::Box);
    QSize newSize = label.sizeHint();
    QVERIFY(oldSize != newSize);

    QLabel label2("why does it always rain on me?");
    label2.show();
    label2.setFrameStyle(QFrame::Sunken | QFrame::Box);
    QCOMPARE(newSize, label2.sizeHint());

    QLabel label3("why does it always rain on me?");
    label3.setFrameStyle(QFrame::Sunken | QFrame::Box);
    QCOMPARE(newSize, label3.sizeHint());
}

void tst_QWidget::moveWindowInShowEvent_data()
{
    QTest::addColumn<QPoint>("initial");
    QTest::addColumn<QPoint>("position");

    QPoint p = QApplication::desktop()->availableGeometry().topLeft();

    QTest::newRow("1") << p << (p + QPoint(10, 10));
    QTest::newRow("2") << (p + QPoint(10,10)) << p;
}

void tst_QWidget::moveWindowInShowEvent()
{
#ifdef Q_OS_IRIX
    QSKIP("4DWM issues on IRIX makes this test fail", SkipAll);
#endif
    QFETCH(QPoint, initial);
    QFETCH(QPoint, position);

    class MoveWindowInShowEventWidget : public QWidget
    {
    public:
        QPoint position;
        void showEvent(QShowEvent *)
        {
            move(position);
        }
    };

    MoveWindowInShowEventWidget widget;
    widget.resize(QSize(qApp->desktop()->availableGeometry().size() / 3).expandedTo(QSize(1, 1)));
    // move to this position in showEvent()
    widget.position = position;

    // put the widget in it's starting position
    widget.move(initial);
    QCOMPARE(widget.pos(), initial);

    // show it
    widget.show();
    #ifdef Q_WS_X11
    qt_x11_wait_for_window_manager(&widget);
    #endif
    QTest::qWait(100);
    // it should have moved
    QCOMPARE(widget.pos(), position);
}

void tst_QWidget::repaintWhenChildDeleted()
{
#ifdef Q_WS_WIN
    if (QSysInfo::WindowsVersion & QSysInfo::WV_VISTA) {
        QTest::qWait(1000);
    }
#endif
    ColorWidget w(0, Qt::red);
#if !defined(Q_OS_WINCE) && !defined(Q_WS_S60)
    QPoint startPoint = QApplication::desktop()->availableGeometry(&w).topLeft();
    startPoint.rx() += 50;
    startPoint.ry() += 50;
    w.setGeometry(QRect(startPoint, QSize(100, 100)));
#else
    w.setGeometry(60, 60, 110, 110);
#endif
    w.show();
    QTest::qWaitForWindowShown(&w);
    QTest::qWait(10);
    QTRY_COMPARE(w.r, QRegion(w.rect()));
    w.r = QRegion();

    {
        const QPoint tlwOffset = w.geometry().topLeft();
        ColorWidget child(&w, Qt::blue);
        child.setGeometry(10, 10, 10, 10);
        child.show();
        QTest::qWait(10);
        QTRY_COMPARE(child.r, QRegion(child.rect()));
        w.r = QRegion();
    }

    QTest::qWait(10);
    QTRY_COMPARE(w.r, QRegion(10, 10, 10, 10));
}

// task 175114
void tst_QWidget::hideOpaqueChildWhileHidden()
{
    ColorWidget w(0, Qt::red);
#if !defined(Q_OS_WINCE) && !defined(Q_WS_S60)
    QPoint startPoint = QApplication::desktop()->availableGeometry(&w).topLeft();
    startPoint.rx() += 50;
    startPoint.ry() += 50;
    w.setGeometry(QRect(startPoint, QSize(100, 100)));
#else
    w.setGeometry(60, 60, 110, 110);
#endif

    ColorWidget child(&w, Qt::blue);
    child.setGeometry(10, 10, 80, 80);

    ColorWidget child2(&child, Qt::white);
    child2.setGeometry(10, 10, 60, 60);

    w.show();
    QTest::qWaitForWindowShown(&w);
    QTest::qWait(10);
    QTRY_COMPARE(child2.r, QRegion(child2.rect()));
    child.r = QRegion();
    child2.r = QRegion();
    w.r = QRegion();

    child.hide();
    child2.hide();
    QTest::qWait(100);

    QCOMPARE(w.r, QRegion(child.geometry()));

    child.show();
    QTest::qWait(100);
    QCOMPARE(child.r, QRegion(child.rect()));
    QCOMPARE(child2.r, QRegion());
}

void tst_QWidget::updateWhileMinimized()
{
#if defined(Q_OS_WINCE) || defined(Q_WS_QWS)
   QSKIP("This test doesn't make sense without support for showMinimized()", SkipAll);
#endif

    UpdateWidget widget;
   // Filter out activation change and focus events to avoid update() calls in QWidget.
    widget.updateOnActivationChangeAndFocusIn = false;
    widget.reset();
    widget.show();
    QTest::qWaitForWindowShown(&widget);
    QApplication::processEvents();
    QTRY_VERIFY(widget.numPaintEvents > 0);
    QTest::qWait(150);

    // Minimize window.
    widget.showMinimized();
    QTest::qWait(110);

    widget.reset();

    // The widget is not visible on the screen (but isVisible() still returns true).
    // Make sure update requests are discarded until the widget is shown again.
    widget.update(0, 0, 50, 50);
    QTest::qWait(10);
    QCOMPARE(widget.numPaintEvents, 0);

    // Restore window.
    widget.showNormal();
    QTest::qWait(30);
    QTRY_COMPARE(widget.numPaintEvents, 1);
    QCOMPARE(widget.paintedRegion, QRegion(0, 0, 50, 50));
}

#if defined(Q_WS_WIN) || defined(Q_WS_X11)
class PaintOnScreenWidget: public QWidget
{
public:
    PaintOnScreenWidget(QWidget *parent = 0, Qt::WindowFlags f = 0)
        :QWidget(parent, f)
    {
    }
#if defined(Q_WS_WIN)
    // This is the only way to enable PaintOnScreen on Windows.
    QPaintEngine * paintEngine () const {return 0;}
#endif
};

void tst_QWidget::alienWidgets()
{
    qApp->setAttribute(Qt::AA_DontCreateNativeWidgetSiblings);
    QWidget parent;
    QWidget child(&parent);
    QWidget grandChild(&child);
    QWidget greatGrandChild(&grandChild);
    parent.show();

#ifdef Q_WS_X11
    qt_x11_wait_for_window_manager(&parent);
#endif

    // Verify that the WA_WState_Created attribute is set
    // and the top-level is the only native window.
    QVERIFY(parent.testAttribute(Qt::WA_WState_Created));
    QVERIFY(parent.internalWinId());

    QVERIFY(child.testAttribute(Qt::WA_WState_Created));
    QVERIFY(!child.internalWinId());

    QVERIFY(grandChild.testAttribute(Qt::WA_WState_Created));
    QVERIFY(!grandChild.internalWinId());

    QVERIFY(greatGrandChild.testAttribute(Qt::WA_WState_Created));
    QVERIFY(!greatGrandChild.internalWinId());

    // Enforce native windows all the way up in the parent hierarchy
    // if not WA_DontCreateNativeAncestors is set.
    grandChild.setAttribute(Qt::WA_DontCreateNativeAncestors);
    greatGrandChild.setAttribute(Qt::WA_NativeWindow);
    QVERIFY(greatGrandChild.internalWinId());
    QVERIFY(grandChild.internalWinId());
    QVERIFY(!child.internalWinId());

    {
        // Ensure that hide() on an ancestor of a widget with
        // Qt::WA_DontCreateNativeAncestors still gets unmapped
        QWidget window;
        QWidget widget(&window);
        QWidget child(&widget);
        child.setAttribute(Qt::WA_NativeWindow);
        child.setAttribute(Qt::WA_DontCreateNativeAncestors);
        window.show();
        QVERIFY(child.testAttribute(Qt::WA_Mapped));
        widget.hide();
        QVERIFY(!child.testAttribute(Qt::WA_Mapped));
    }

    // Enforce a native window when calling QWidget::winId.
    QVERIFY(child.winId());
    QVERIFY(child.internalWinId());

    // Check that paint on screen widgets (incl. children) are native.
    PaintOnScreenWidget paintOnScreen(&parent);
    QWidget paintOnScreenChild(&paintOnScreen);
    paintOnScreen.show();
    QVERIFY(paintOnScreen.testAttribute(Qt::WA_WState_Created));
    QVERIFY(!paintOnScreen.testAttribute(Qt::WA_NativeWindow));
    QVERIFY(!paintOnScreen.internalWinId());
    QVERIFY(!paintOnScreenChild.testAttribute(Qt::WA_NativeWindow));
    QVERIFY(!paintOnScreenChild.internalWinId());

    paintOnScreen.setAttribute(Qt::WA_PaintOnScreen);
    QVERIFY(paintOnScreen.testAttribute(Qt::WA_NativeWindow));
    QVERIFY(paintOnScreen.internalWinId());
    QVERIFY(paintOnScreenChild.testAttribute(Qt::WA_NativeWindow));
    QVERIFY(paintOnScreenChild.internalWinId());

    // Check that widgets with the Qt::MSWindowsOwnDC attribute set
    // are native.
    QWidget msWindowsOwnDC(&parent, Qt::MSWindowsOwnDC);
    msWindowsOwnDC.show();
    QVERIFY(msWindowsOwnDC.testAttribute(Qt::WA_WState_Created));
    QVERIFY(msWindowsOwnDC.testAttribute(Qt::WA_NativeWindow));
    QVERIFY(msWindowsOwnDC.internalWinId());

    { // Enforce a native window when calling QWidget::handle() (on X11) or QWidget::getDC() (on Windows).
        QWidget widget(&parent);
        widget.show();
        QVERIFY(widget.testAttribute(Qt::WA_WState_Created));
        QVERIFY(!widget.internalWinId());
#ifdef Q_WS_X11
        widget.handle();
#else
        widget.getDC();
#endif
        QVERIFY(widget.internalWinId());
    }

#ifdef Q_WS_X11
#ifndef QT_NO_XRENDER
    { // Enforce a native window when calling QWidget::x11PictureHandle().
        QWidget widget(&parent);
        widget.show();
        QVERIFY(widget.testAttribute(Qt::WA_WState_Created));
        QVERIFY(!widget.internalWinId());
        widget.x11PictureHandle();
        QVERIFY(widget.internalWinId());
    }
#endif

    { // Make sure we don't create native windows when setting Qt::WA_X11NetWmWindowType attributes
      // on alien widgets (see task 194231).
        QWidget dummy;
        QVERIFY(dummy.winId());
        QWidget widget(&dummy);
        widget.setAttribute(Qt::WA_X11NetWmWindowTypeToolBar);
        QVERIFY(!widget.internalWinId());
    }
#endif


    { // Make sure we create native ancestors when setting Qt::WA_PaintOnScreen before show().
        QWidget topLevel;
        QWidget child(&topLevel);
        QWidget grandChild(&child);
        PaintOnScreenWidget greatGrandChild(&grandChild);

        greatGrandChild.setAttribute(Qt::WA_PaintOnScreen);
        QVERIFY(!child.internalWinId());
        QVERIFY(!grandChild.internalWinId());
        QVERIFY(!greatGrandChild.internalWinId());

        topLevel.show();
        QVERIFY(child.internalWinId());
        QVERIFY(grandChild.internalWinId());
        QVERIFY(greatGrandChild.internalWinId());
    }

    { // Ensure that widgets reparented into Qt::WA_PaintOnScreen widgets become native.
        QWidget topLevel;
        QWidget *widget = new PaintOnScreenWidget(&topLevel);
        widget->setAttribute(Qt::WA_PaintOnScreen);
        QWidget *child = new QWidget;
        QWidget *dummy = new QWidget(child);
        QWidget *grandChild = new QWidget(child);
        QWidget *dummy2 = new QWidget(grandChild);

        child->setParent(widget);

        QVERIFY(!topLevel.internalWinId());
        QVERIFY(!child->internalWinId());
        QVERIFY(!dummy->internalWinId());
        QVERIFY(!grandChild->internalWinId());
        QVERIFY(!dummy2->internalWinId());

        topLevel.show();
        QVERIFY(topLevel.internalWinId());
        QVERIFY(widget->testAttribute(Qt::WA_NativeWindow));
        QVERIFY(child->internalWinId());
        QVERIFY(child->testAttribute(Qt::WA_NativeWindow));
        QVERIFY(!child->testAttribute(Qt::WA_PaintOnScreen));
        QVERIFY(!dummy->internalWinId());
        QVERIFY(!dummy->testAttribute(Qt::WA_NativeWindow));
        QVERIFY(!grandChild->internalWinId());
        QVERIFY(!grandChild->testAttribute(Qt::WA_NativeWindow));
        QVERIFY(!dummy2->internalWinId());
        QVERIFY(!dummy2->testAttribute(Qt::WA_NativeWindow));
    }

    { // Ensure that ancestors of a Qt::WA_PaintOnScreen widget stay native
      // if they are re-created (typically in QWidgetPrivate::setParent_sys) (task 210822).
        QWidget window;
        QWidget child(&window);

        QWidget grandChild;
        grandChild.setWindowTitle("This causes the widget to be created");

        PaintOnScreenWidget paintOnScreenWidget;
        paintOnScreenWidget.setAttribute(Qt::WA_PaintOnScreen);
        paintOnScreenWidget.setParent(&grandChild);

        grandChild.setParent(&child);

        window.show();

        QVERIFY(window.internalWinId());
        QVERIFY(child.internalWinId());
        QVERIFY(child.testAttribute(Qt::WA_NativeWindow));
        QVERIFY(grandChild.internalWinId());
        QVERIFY(grandChild.testAttribute(Qt::WA_NativeWindow));
        QVERIFY(paintOnScreenWidget.internalWinId());
        QVERIFY(paintOnScreenWidget.testAttribute(Qt::WA_NativeWindow));
    }

    { // Ensure that all siblings are native unless Qt::AA_DontCreateNativeWidgetSiblings is set.
        qApp->setAttribute(Qt::AA_DontCreateNativeWidgetSiblings, false);
        QWidget mainWindow;
        QWidget *toolBar = new QWidget(&mainWindow);
        QWidget *dockWidget = new QWidget(&mainWindow);
        QWidget *centralWidget = new QWidget(&mainWindow);

        QWidget *button = new QWidget(centralWidget);
        QWidget *mdiArea = new QWidget(centralWidget);

        QWidget *horizontalScroll = new QWidget(mdiArea);
        QWidget *verticalScroll = new QWidget(mdiArea);
        QWidget *viewport = new QWidget(mdiArea);

        viewport->setAttribute(Qt::WA_NativeWindow);
        mainWindow.show();

        // Ensure that the viewport and its siblings are native:
        QVERIFY(verticalScroll->testAttribute(Qt::WA_NativeWindow));
        QVERIFY(verticalScroll->testAttribute(Qt::WA_NativeWindow));
        QVERIFY(horizontalScroll->testAttribute(Qt::WA_NativeWindow));

        // Ensure that the mdi area and its siblings are native:
        QVERIFY(mdiArea->testAttribute(Qt::WA_NativeWindow));
        QVERIFY(button->testAttribute(Qt::WA_NativeWindow));

        // Ensure that the central widget and its siblings are native:
        QVERIFY(centralWidget->testAttribute(Qt::WA_NativeWindow));
        QVERIFY(dockWidget->testAttribute(Qt::WA_NativeWindow));
        QVERIFY(toolBar->testAttribute(Qt::WA_NativeWindow));
    }
}
#endif // Q_WS_WIN / Q_WS_X11

class ASWidget : public QWidget
{
public:
    ASWidget(QSize sizeHint, QSizePolicy sizePolicy, bool layout, bool hfwLayout, QWidget *parent = 0)
        : QWidget(parent), mySizeHint(sizeHint)
    {
        setSizePolicy(sizePolicy);
        if (layout) {
            QSizePolicy sp = QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
            sp.setHeightForWidth(hfwLayout);

            QVBoxLayout *vbox = new QVBoxLayout;
            vbox->setMargin(0);
            vbox->addWidget(new ASWidget(sizeHint + QSize(30, 20), sp, false, false));
            setLayout(vbox);
        }
    }

    QSize sizeHint() const {
        if (layout())
            return layout()->totalSizeHint();
        return mySizeHint;
    }
    int heightForWidth(int width) const {
        if (sizePolicy().hasHeightForWidth()) {
            return width * 2;
        } else {
            return -1;
        }
    }

    QSize mySizeHint;
};

void tst_QWidget::adjustSize_data()
{
    const int MagicW = 200;
    const int MagicH = 100;

    QTest::addColumn<QSize>("sizeHint");
    QTest::addColumn<int>("hPolicy");
    QTest::addColumn<int>("vPolicy");
    QTest::addColumn<bool>("hfwSP");
    QTest::addColumn<bool>("layout");
    QTest::addColumn<bool>("hfwLayout");
    QTest::addColumn<bool>("haveParent");
    QTest::addColumn<QSize>("expectedSize");

    QTest::newRow("1") << QSize(5, 6) << int(QSizePolicy::Minimum) << int(QSizePolicy::Expanding)
        << false << false << false << false << QSize(5, qMax(6, MagicH));
    QTest::newRow("2") << QSize(5, 6) << int(QSizePolicy::Minimum) << int(QSizePolicy::Expanding)
        << true << false << false << false << QSize(5, qMax(10, MagicH));
    QTest::newRow("3") << QSize(5, 6) << int(QSizePolicy::Minimum) << int(QSizePolicy::Expanding)
        << false << true << false << false << QSize(35, 26);
    QTest::newRow("4") << QSize(5, 6) << int(QSizePolicy::Minimum) << int(QSizePolicy::Expanding)
        << false << true << true << false << QSize(35, 70);
    QTest::newRow("5") << QSize(40001, 30001) << int(QSizePolicy::Minimum) << int(QSizePolicy::Expanding)
        << false << false << false << false << QSize(100000, 100000);
    QTest::newRow("6") << QSize(40001, 30001) << int(QSizePolicy::Minimum) << int(QSizePolicy::Expanding)
        << true << false << false << false << QSize(100000, 100000);
    QTest::newRow("7") << QSize(40001, 30001) << int(QSizePolicy::Minimum) << int(QSizePolicy::Expanding)
        << false << true << false << false << QSize(100000, 100000);
    QTest::newRow("8") << QSize(40001, 30001) << int(QSizePolicy::Minimum) << int(QSizePolicy::Expanding)
        << false << true << true << false << QSize(100000, 100000);
    QTest::newRow("9") << QSize(5, 6) << int(QSizePolicy::Expanding) << int(QSizePolicy::Minimum)
        << true << false << false << false << QSize(qMax(5, MagicW), 10);

    QTest::newRow("1c") << QSize(5, 6) << int(QSizePolicy::Minimum) << int(QSizePolicy::Expanding)
        << false << false << false << true << QSize(5, 6);
    QTest::newRow("2c") << QSize(5, 6) << int(QSizePolicy::Minimum) << int(QSizePolicy::Expanding)
        << true << false << false << true << QSize(5, 6 /* or 10 would be OK too, since hfw contradicts sizeHint() */);
    QTest::newRow("3c") << QSize(5, 6) << int(QSizePolicy::Minimum) << int(QSizePolicy::Expanding)
        << false << true << false << true << QSize(35, 26);
    QTest::newRow("4c") << QSize(5, 6) << int(QSizePolicy::Minimum) << int(QSizePolicy::Expanding)
        << false << true << true << true << QSize(35, 70);
    QTest::newRow("5c") << QSize(40001, 30001) << int(QSizePolicy::Minimum) << int(QSizePolicy::Expanding)
        << false << false << false << true << QSize(40001, 30001);
    QTest::newRow("6c") << QSize(40001, 30001) << int(QSizePolicy::Minimum) << int(QSizePolicy::Expanding)
        << true << false << false << true << QSize(40001, 30001 /* or 80002 would be OK too, since hfw contradicts sizeHint() */);
    QTest::newRow("7c") << QSize(40001, 30001) << int(QSizePolicy::Minimum) << int(QSizePolicy::Expanding)
        << false << true << false << true << QSize(40001 + 30, 30001 + 20);
    QTest::newRow("8c") << QSize(40001, 30001) << int(QSizePolicy::Minimum) << int(QSizePolicy::Expanding)
        << false << true << true << true << QSize(40001 + 30, 80002 + 60);
    QTest::newRow("9c") << QSize(5, 6) << int(QSizePolicy::Expanding) << int(QSizePolicy::Minimum)
        << true << false << false << true << QSize(5, 6);
}

void tst_QWidget::adjustSize()
{
    QFETCH(QSize, sizeHint);
    QFETCH(int, hPolicy);
    QFETCH(int, vPolicy);
    QFETCH(bool, hfwSP);
    QFETCH(bool, layout);
    QFETCH(bool, hfwLayout);
    QFETCH(bool, haveParent);
    QFETCH(QSize, expectedSize);

    QWidget *parent = new QWidget;

    QSizePolicy sp = QSizePolicy(QSizePolicy::Policy(hPolicy), QSizePolicy::Policy(vPolicy));
    sp.setHeightForWidth(hfwSP);

    QWidget *child = new ASWidget(sizeHint, sp, layout, hfwLayout, haveParent ? parent : 0);
    child->resize(123, 456);
    child->adjustSize();
    if (expectedSize == QSize(100000, 100000)) {
        QVERIFY(child->size().width() < sizeHint.width());
        QVERIFY(child->size().height() < sizeHint.height());
    } else {
#if defined (Q_OS_WINCE)
        if (!haveParent) {
            const QRect& desktopRect = qApp->desktop()->availableGeometry();
            expectedSize.setWidth(qMin(expectedSize.width(), desktopRect.width()));
            expectedSize.setHeight(qMin(expectedSize.height(), desktopRect.height()));
        }
#endif
        QCOMPARE(child->size(), expectedSize);
    }

    delete parent;
}

class TestLayout : public QVBoxLayout
{
    Q_OBJECT
public:
    TestLayout(QWidget *w = 0) : QVBoxLayout(w)
    {
        invalidated = false;
    }

    void invalidate()
    {
        invalidated = true;
    }

    bool invalidated;
};

void tst_QWidget::updateGeometry_data()
{
    QTest::addColumn<QSize>("minSize");
    QTest::addColumn<bool>("shouldInvalidate");
    QTest::addColumn<QSize>("maxSize");
    QTest::addColumn<bool>("shouldInvalidate2");
    QTest::addColumn<int>("verticalSizePolicy");
    QTest::addColumn<bool>("shouldInvalidate3");
    QTest::addColumn<bool>("setVisible");
    QTest::addColumn<bool>("shouldInvalidate4");

    QTest::newRow("setMinimumSize")
        << QSize(100, 100) << true
        << QSize() << false
        << int(QSizePolicy::Preferred) << false
        << true << false;
    QTest::newRow("setMaximumSize")
        << QSize() << false
        << QSize(100, 100) << true
        << int(QSizePolicy::Preferred) << false
        << true << false;
    QTest::newRow("setMinimumSize, then maximumSize to a different size")
        << QSize(100, 100) << true
        << QSize(300, 300) << true
        << int(QSizePolicy::Preferred) << false
        << true << false;
    QTest::newRow("setMinimumSize, then maximumSize to the same size")
        << QSize(100, 100) << true
        << QSize(100, 100) << true
        << int(QSizePolicy::Preferred) << false
        << true << false;
    QTest::newRow("setMinimumSize, then maximumSize to the same size and then hide it")
        << QSize(100, 100) << true
        << QSize(100, 100) << true
        << int(QSizePolicy::Preferred) << false
        << false << true;
    QTest::newRow("Change sizePolicy")
        << QSize() << false
        << QSize() << false
        << int(QSizePolicy::Minimum) << true
        << true << false;

}

void tst_QWidget::updateGeometry()
{
    QFETCH(QSize, minSize);
    QFETCH(bool, shouldInvalidate);
    QFETCH(QSize, maxSize);
    QFETCH(bool, shouldInvalidate2);
    QFETCH(int, verticalSizePolicy);
    QFETCH(bool, shouldInvalidate3);
    QFETCH(bool, setVisible);
    QFETCH(bool, shouldInvalidate4);
    QWidget parent;
    parent.resize(200, 200);
    TestLayout *lout = new TestLayout();
    parent.setLayout(lout);
    QWidget *child = new QWidget(&parent);
    lout->addWidget(child);
    parent.show();
    QApplication::processEvents();

    lout->invalidated = false;
    if (minSize.isValid())
        child->setMinimumSize(minSize);
    QCOMPARE(lout->invalidated, shouldInvalidate);

    lout->invalidated = false;
    if (maxSize.isValid())
        child->setMaximumSize(maxSize);
    QCOMPARE(lout->invalidated, shouldInvalidate2);

    lout->invalidated = false;
    child->setSizePolicy(QSizePolicy(QSizePolicy::Preferred, (QSizePolicy::Policy)verticalSizePolicy));
    if (shouldInvalidate3)
        QCOMPARE(lout->invalidated, true);

    lout->invalidated = false;
    if (!setVisible)
        child->setVisible(false);
    QCOMPARE(lout->invalidated, shouldInvalidate4);
}

void tst_QWidget::sendUpdateRequestImmediately()
{
#ifdef Q_WS_MAC
    if (!QApplicationPrivate::graphicsSystem())
        QSKIP("We only send update requests on the Mac when passing -graphicssystem", SkipAll);
#endif

    UpdateWidget updateWidget;
    updateWidget.show();
#ifdef Q_WS_X11
    qt_x11_wait_for_window_manager(&updateWidget);
#endif

    qApp->processEvents();
#ifdef Q_WS_QWS
    QApplication::sendPostedEvents(); //glib workaround
#endif
    updateWidget.reset();

    QCOMPARE(updateWidget.numUpdateRequestEvents, 0);
    updateWidget.repaint();
    QCOMPARE(updateWidget.numUpdateRequestEvents, 1);
}

class RedirectedWidget : public QWidget
{
protected:
    void paintEvent(QPaintEvent *)
    {
        // Verify that the widget has a redirection set. The widget is redirected to
        // the backing store on all platforms using it; otherwise to itself if the wrect
        // does not start in (0, 0) or it has a mask set.
        QPaintDevice *oldRedirection = QPainter::redirected(this);
#ifndef Q_WS_MAC
        QVERIFY(oldRedirection);
#endif

        QImage image(size(), QImage::Format_RGB32);
        image.fill(Qt::blue);

        {
        QPainter painter(this);
        QCOMPARE(painter.device(), static_cast<QPaintDevice *>(this));
        }

        QPainter::setRedirected(this, &image);
        QCOMPARE(QPainter::redirected(this), static_cast<QPaintDevice *>(&image));

        QPainter painter(this);
        painter.fillRect(rect(), Qt::red);

        QPainter::restoreRedirected(this);
        QCOMPARE(QPainter::redirected(this), oldRedirection);

        for (int i = 0; i < image.height(); ++i)
            for (int j = 0; j < image.width(); ++j)
                QCOMPARE(image.pixel(j, i), QColor(Qt::red).rgb());
    }

};

// Test to make sure we're compatible in the particular case where QPainter::setRedirected
// actually works. It has been broken for all other cases since Qt 4.1.4 (backing store).
// QWidget::render is the modern and more powerful way of doing the same.
void tst_QWidget::painterRedirection()
{
    RedirectedWidget widget;
    // Set FramelessWindowHint and mask to trigger internal painter redirection on the Mac.
    widget.setWindowFlags(widget.windowFlags() | Qt::FramelessWindowHint);
    widget.setMask(QRect(10, 10, 50, 50));
    widget.setFixedSize(100, 200);
    widget.show();
#ifdef Q_WS_X11
    qt_x11_wait_for_window_manager(&widget);
#endif
    QPixmap pixmap(widget.size());
    QPainter::setRedirected(&widget, &pixmap, QPoint());
    widget.repaint();
    QCOMPARE(QPainter::redirected(&widget), static_cast<QPaintDevice *>(&pixmap));
}


void tst_QWidget::doubleRepaint()
{
#ifdef Q_OS_IRIX
   QSKIP("4DWM issues on IRIX makes this test fail", SkipAll);
#elif defined(Q_WS_MAC)
    if (!macHasAccessToWindowsServer())
        QSKIP("Not having window server access causes the wrong number of repaints to be issues", SkipAll);
#endif
   UpdateWidget widget;
   widget.setFocusPolicy(Qt::StrongFocus);
   // Filter out activation change and focus events to avoid update() calls in QWidget.
   widget.updateOnActivationChangeAndFocusIn = false;

   // Show: 1 repaint
   int expectedRepaints = 1;
   widget.show();
   QTest::qWaitForWindowShown(&widget);
   QTest::qWait(10);
   QTRY_COMPARE(widget.numPaintEvents, expectedRepaints);
   widget.numPaintEvents = 0;

   // Minmize: Should not trigger a repaint.
   widget.showMinimized();
   QTest::qWait(10);
   QCOMPARE(widget.numPaintEvents, 0);
   widget.numPaintEvents = 0;

   // Restore: Should not trigger a repaint.
   widget.showNormal();
   QTest::qWaitForWindowShown(&widget);
   QTest::qWait(10);
   QCOMPARE(widget.numPaintEvents, 0);
}

#ifndef Q_WS_MAC
// This test only makes sense on the Mac when passing -graphicssystem.
void tst_QWidget::resizeInPaintEvent()
{
    QWidget window;
    UpdateWidget widget(&window);
    window.show();
    QTest::qWaitForWindowShown(&window);
    QTRY_VERIFY(widget.numPaintEvents > 0);

    widget.reset();
    QCOMPARE(widget.numPaintEvents, 0);

    widget.resizeInPaintEvent = true;
    // This will call resize in the paintEvent, which in turn will call
    // invalidateBuffer() and a new update request should be posted.
    widget.repaint();
    QCOMPARE(widget.numPaintEvents, 1);
    widget.numPaintEvents = 0;

    QTest::qWait(10);
    // Make sure the resize triggers another update.
    QTRY_COMPARE(widget.numPaintEvents, 1);
}

void tst_QWidget::opaqueChildren()
{
    QWidget widget;
    widget.resize(200, 200);

    QWidget child(&widget);
    child.setGeometry(-700, -700, 200, 200);

    QWidget grandChild(&child);
    grandChild.resize(200, 200);

    QWidget greatGrandChild(&grandChild);
    greatGrandChild.setGeometry(50, 50, 200, 200);
    greatGrandChild.setPalette(Qt::red);
    greatGrandChild.setAutoFillBackground(true); // Opaque child widget.

    widget.show();
#ifdef Q_WS_X11
    qt_x11_wait_for_window_manager(&widget);
#endif
    QTest::qWait(100);

    // Child, grandChild and greatGrandChild are outside the ancestor clip.
    QRegion expectedOpaqueRegion(50, 50, 150, 150);
    QCOMPARE(qt_widget_private(&grandChild)->getOpaqueChildren(), expectedOpaqueRegion);

    // Now they are all inside the ancestor clip.
    child.setGeometry(50, 50, 150, 150);
    QCOMPARE(qt_widget_private(&grandChild)->getOpaqueChildren(), expectedOpaqueRegion);

    // Set mask on greatGrandChild.
    const QRegion mask(10, 10, 50, 50);
    greatGrandChild.setMask(mask);
    expectedOpaqueRegion &= mask.translated(50, 50);
    QCOMPARE(qt_widget_private(&grandChild)->getOpaqueChildren(), expectedOpaqueRegion);

    // Make greatGrandChild "transparent".
    greatGrandChild.setAutoFillBackground(false);
    QCOMPARE(qt_widget_private(&grandChild)->getOpaqueChildren(), QRegion());
}
#endif


class MaskSetWidget : public QWidget
{
    Q_OBJECT
public:
    MaskSetWidget(QWidget* p =0)
            : QWidget(p) {}

    void paintEvent(QPaintEvent* event) {
        QPainter p(this);

        paintedRegion += event->region();
        foreach(QRect r, event->region().rects())
            p.fillRect(r, Qt::red);
    }

    void resizeEvent(QResizeEvent*) {
        setMask(QRegion(QRect(0, 0, width(), 10).normalized()));
    }

    QRegion paintedRegion;

public slots:
    void resizeDown() {
        setGeometry(QRect(0, 50, 50, 50));
    }

    void resizeUp() {
        setGeometry(QRect(0, 50, 150, 50));
    }

};

void tst_QWidget::setMaskInResizeEvent()
{
    UpdateWidget w;
    w.reset();
    w.resize(200, 200);
    w.setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    w.raise();

    MaskSetWidget testWidget(&w);
    testWidget.setGeometry(0, 0, 100, 100);
    testWidget.setMask(QRegion(QRect(0,0,100,10)));
    testWidget.show();
    w.show();
    QTest::qWaitForWindowShown(&w);
    QTest::qWait(30);
    QTRY_VERIFY(w.numPaintEvents > 0);

    w.reset();
    testWidget.paintedRegion = QRegion();
    QTimer::singleShot(0, &testWidget, SLOT(resizeDown()));
    QTest::qWait(100);

    QRegion expectedParentUpdate(0, 0, 100, 10); // Old testWidget area.
    expectedParentUpdate += testWidget.geometry(); // New testWidget area.
    QCOMPARE(w.paintedRegion, expectedParentUpdate);
    QCOMPARE(testWidget.paintedRegion, testWidget.mask());

    testWidget.paintedRegion = QRegion();
    // Now resize the widget again, but in the oposite direction
    QTimer::singleShot(0, &testWidget, SLOT(resizeUp()));
    QTest::qWait(100);

    QTRY_COMPARE(testWidget.paintedRegion, testWidget.mask());
}

class MoveInResizeWidget : public QWidget
{
    Q_OBJECT
public:
    MoveInResizeWidget(QWidget* p = 0)
        : QWidget(p)
    {
        setWindowFlags(Qt::FramelessWindowHint);
    }

    void resizeEvent(QResizeEvent*) {

        move(QPoint(100,100));

        static bool firstTime = true;
        if (firstTime)
            QTimer::singleShot(250, this, SLOT(resizeMe()));

        firstTime = false;
    }

public slots:
    void resizeMe() {
        resize(100, 100);
    }
};

void tst_QWidget::moveInResizeEvent()
{
    MoveInResizeWidget testWidget;
    testWidget.setGeometry(50, 50, 200, 200);
    testWidget.show();
    QTest::qWaitForWindowShown(&testWidget);
    QTest::qWait(300);

    QRect expectedGeometry(100,100, 100, 100);
    QTRY_COMPARE(testWidget.geometry(), expectedGeometry);
}


#if defined(Q_WS_WIN) || defined(Q_WS_X11)
void tst_QWidget::immediateRepaintAfterShow()
{
    UpdateWidget widget;
    widget.show();
    qApp->processEvents();
    // On X11 in particular, we are now waiting for a MapNotify event before
    // syncing the backing store. However, if someone request a repaint()
    // we must repaint immediately regardless of the current state.
    widget.numPaintEvents = 0;
    widget.repaint();
    QCOMPARE(widget.numPaintEvents, 1);
}

void tst_QWidget::immediateRepaintAfterInvalidateBuffer()
{
    QWidget *widget = new UpdateWidget;
    widget->show();
#ifdef Q_WS_X11
    qt_x11_wait_for_window_manager(widget);
#endif
    QTest::qWait(200);

    static_cast<UpdateWidget *>(widget)->numPaintEvents = 0;

    // Marks the area covered by the widget as dirty in the backing store and
    // posts an UpdateRequest event.
    qt_widget_private(widget)->invalidateBuffer(widget->rect());
    QCOMPARE(static_cast<UpdateWidget *>(widget)->numPaintEvents, 0);

    // The entire widget is already dirty, but this time we want to update immediately
    // by calling repaint(), and thus we have to repaint the widget and not wait for
    // the UpdateRequest to be sent when we get back to the event loop.
    widget->repaint();
    QCOMPARE(static_cast<UpdateWidget *>(widget)->numPaintEvents, 1);

    delete widget;
}
#endif

void tst_QWidget::effectiveWinId()
{
    QWidget parent;
    QWidget child(&parent);

    // Shouldn't crash.
    QVERIFY(!parent.effectiveWinId());
    QVERIFY(!child.effectiveWinId());

    parent.show();

    QVERIFY(parent.effectiveWinId());
    QVERIFY(child.effectiveWinId());
}

class CustomWidget : public QWidget
{
public:
    mutable int metricCallCount;

    CustomWidget(QWidget *parent = 0) : QWidget(parent), metricCallCount(0) {}

    virtual int metric(PaintDeviceMetric metric) const {
        ++metricCallCount;
        return QWidget::metric(metric);
    }
};

void tst_QWidget::customDpi()
{
    QWidget *topLevel = new QWidget;
    CustomWidget *custom = new CustomWidget(topLevel);
    QWidget *child = new QWidget(custom);

    custom->metricCallCount = 0;
    topLevel->logicalDpiX();
    QCOMPARE(custom->metricCallCount, 0);
    custom->logicalDpiX();
    QCOMPARE(custom->metricCallCount, 1);
    child->logicalDpiX();
#ifdef Q_WS_S60
    // QWidget::metric is not recursive on Symbian
    QCOMPARE(custom->metricCallCount, 1);
#else
    QCOMPARE(custom->metricCallCount, 2);
#endif

    delete topLevel;
}

void tst_QWidget::customDpiProperty()
{
    QWidget *topLevel = new QWidget;
    QWidget *middle = new CustomWidget(topLevel);
    QWidget *child = new QWidget(middle);

    const int initialDpiX = topLevel->logicalDpiX();
    const int initialDpiY = topLevel->logicalDpiY();

    middle->setProperty("_q_customDpiX", 300);
    middle->setProperty("_q_customDpiY", 400);

    QCOMPARE(topLevel->logicalDpiX(), initialDpiX);
    QCOMPARE(topLevel->logicalDpiY(), initialDpiY);

    QCOMPARE(middle->logicalDpiX(), 300);
    QCOMPARE(middle->logicalDpiY(), 400);

    QCOMPARE(child->logicalDpiX(), 300);
    QCOMPARE(child->logicalDpiY(), 400);

    middle->setProperty("_q_customDpiX", QVariant());
    middle->setProperty("_q_customDpiY", QVariant());

    QCOMPARE(topLevel->logicalDpiX(), initialDpiX);
    QCOMPARE(topLevel->logicalDpiY(), initialDpiY);

    QCOMPARE(middle->logicalDpiX(), initialDpiX);
    QCOMPARE(middle->logicalDpiY(), initialDpiY);

    QCOMPARE(child->logicalDpiX(), initialDpiX);
    QCOMPARE(child->logicalDpiY(), initialDpiY);

    delete topLevel;
}

void tst_QWidget::quitOnCloseAttribute()
{
    QWidget w;
    QCOMPARE(w.testAttribute(Qt::WA_QuitOnClose), true);
    w.setAttribute(Qt::WA_QuitOnClose, false);
    QCOMPARE(w.testAttribute(Qt::WA_QuitOnClose), false);

    w.setAttribute(Qt::WA_QuitOnClose);
    w.setWindowFlags(Qt::Tool);
    QCOMPARE(w.testAttribute(Qt::WA_QuitOnClose), false);

    w.setAttribute(Qt::WA_QuitOnClose);
    w.setWindowFlags(Qt::Popup);
    QCOMPARE(w.testAttribute(Qt::WA_QuitOnClose), false);

    w.setAttribute(Qt::WA_QuitOnClose);
    w.setWindowFlags(Qt::ToolTip);
    QCOMPARE(w.testAttribute(Qt::WA_QuitOnClose), false);

    w.setAttribute(Qt::WA_QuitOnClose);
    w.setWindowFlags(Qt::SplashScreen);
    QCOMPARE(w.testAttribute(Qt::WA_QuitOnClose), false);

    w.setAttribute(Qt::WA_QuitOnClose);
    w.setWindowFlags(Qt::SubWindow);
    QCOMPARE(w.testAttribute(Qt::WA_QuitOnClose), false);

    w.setAttribute(Qt::WA_QuitOnClose);
    w.setWindowFlags(Qt::Dialog);
    QCOMPARE(w.testAttribute(Qt::WA_QuitOnClose), true);
    w.show();
    QCOMPARE(w.testAttribute(Qt::WA_QuitOnClose), true);
    w.setWindowFlags(Qt::Tool);
    QCOMPARE(w.testAttribute(Qt::WA_QuitOnClose), false);
}

void tst_QWidget::moveRect()
{
    QWidget widget;
    widget.setUpdatesEnabled(false);
    QWidget child(&widget);
    child.setUpdatesEnabled(false);
    child.setAttribute(Qt::WA_OpaquePaintEvent);
    widget.show();
    QTest::qWait(200);
    child.move(10, 10); // Don't crash.
}

#ifdef Q_WS_WIN
class GDIWidget : public QDialog
{
public:
    GDIWidget() { setAttribute(Qt::WA_PaintOnScreen); }
    QPaintEngine *paintEngine() const { return 0; }


    void paintEvent(QPaintEvent *) {
        HDC hdc = getDC();
        SelectObject(hdc, CreateSolidBrush(RGB(255, 0, 0)));
        Rectangle(hdc, 0, 0, 10, 10);

        releaseDC(hdc);

        QImage im = QPixmap::grabWindow(winId(), 0, 0, width(), height()).toImage();
        color = im.pixel(1, 1);

        accept();
    }

    QSize sizeHint() const {
        return QSize(400, 300);
    }

    QColor color;
};

void tst_QWidget::gdiPainting()
{
    GDIWidget w;
    w.exec();

    QCOMPARE(w.color, QColor(255, 0, 0));

}

void tst_QWidget::paintOnScreenPossible()
{
    QWidget w1;
    w1.setAttribute(Qt::WA_PaintOnScreen);
    QVERIFY(!w1.testAttribute(Qt::WA_PaintOnScreen));

    GDIWidget w2;
    w2.setAttribute(Qt::WA_PaintOnScreen);
    QVERIFY(w2.testAttribute(Qt::WA_PaintOnScreen));
}
#endif

void tst_QWidget::reparentStaticWidget()
{
    QWidget window1;

    QWidget *child = new QWidget(&window1);
    child->setPalette(Qt::red);
    child->setAutoFillBackground(true);
    child->setAttribute(Qt::WA_StaticContents);
    child->resize(100, 100);

    QWidget *grandChild = new QWidget(child);
    grandChild->setPalette(Qt::blue);
    grandChild->setAutoFillBackground(true);
    grandChild->resize(50, 50);
    grandChild->setAttribute(Qt::WA_StaticContents);
    window1.show();
    QTest::qWaitForWindowShown(&window1);

    QWidget window2;
    window2.show();
    QTest::qWaitForWindowShown(&window2);
    QTest::qWait(20);

    // Reparent into another top-level.
    child->setParent(&window2);
    child->show();

    // Please don't crash.
    window1.resize(window1.size() + QSize(2, 2));
    QTest::qWait(20);

    // Make sure we move all static children even though
    // the reparented widget itself is non-static.
    child->setAttribute(Qt::WA_StaticContents, false);
    child->setParent(&window1);
    child->show();

    // Please don't crash.
    window2.resize(window2.size() + QSize(2, 2));
    QTest::qWait(20);

    child->setParent(0);
    child->show();
    QTest::qWait(20);

    // Please don't crash.
    child->resize(child->size() + QSize(2, 2));
    window2.resize(window2.size() + QSize(2, 2));
    QTest::qWait(20);

    QWidget *siblingOfGrandChild = new QWidget(child);
    siblingOfGrandChild->show();
    QTest::qWait(20);

    // Nothing should happen when reparenting within the same top-level.
    grandChild->setParent(siblingOfGrandChild);
    grandChild->show();
    QTest::qWait(20);

    QWidget paintOnScreen;
    paintOnScreen.setAttribute(Qt::WA_PaintOnScreen);
    paintOnScreen.show();
    QTest::qWaitForWindowShown(&paintOnScreen);
    QTest::qWait(20);

    child->setParent(&paintOnScreen);
    child->show();
    QTest::qWait(20);

    // Please don't crash.
    paintOnScreen.resize(paintOnScreen.size() + QSize(2, 2));
    QTest::qWait(20);

}

void tst_QWidget::QTBUG6883_reparentStaticWidget2()
{
    QMainWindow mw;
    QDockWidget *one = new QDockWidget("one", &mw);
    mw.addDockWidget(Qt::LeftDockWidgetArea, one , Qt::Vertical);

    QWidget *child = new QWidget();
    child->setPalette(Qt::red);
    child->setAutoFillBackground(true);
    child->setAttribute(Qt::WA_StaticContents);
    child->resize(100, 100);
    one->setWidget(child);

    QToolBar *mainTools = mw.addToolBar("Main Tools");
    mainTools->addWidget(new QLineEdit);

    mw.show();
    QTest::qWaitForWindowShown(&mw);

    one->setFloating(true);
    QTest::qWait(20);
    //do not crash
}

#ifdef Q_WS_QWS
void tst_QWidget::updateOutsideSurfaceClip()
{
    UpdateWidget widget;
    widget.setWindowFlags(Qt::FramelessWindowHint);
    widget.resize(100, 100);
    widget.raise();
    widget.show();
    QTest::qWait(200);
    widget.reset();

    // Move widget partially outside buffer and change the surface clip.
    widget.move(-50, 0);
    QTest::qWait(100);

    // Update region is outside the surface clip and should not trigger a repaint.
    widget.update(0, 0, 20, 20);
    QTest::qWait(100);
    QCOMPARE(widget.numPaintEvents, 0);

    // Now, move the widget back so that the update region is inside the clip
    // and make sure we get a repaint of the dirty area.
    widget.move(0, 0);
    QTest::qWait(100);
    QCOMPARE(widget.numPaintEvents, 1);
    QCOMPARE(widget.paintedRegion, QRegion(0, 0, 20, 20));
}
#endif
class ColorRedWidget : public QWidget
{
public:
    ColorRedWidget(QWidget *parent = 0)
        : QWidget(parent, Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::ToolTip)
    {
    }

    void paintEvent(QPaintEvent *) {
        QPainter p(this);
        p.fillRect(rect(),Qt::red);
    }
};

void tst_QWidget::translucentWidget()
{
    QPixmap pm(16,16);
    pm.fill(Qt::red);
    ColorRedWidget label;
    label.setFixedSize(16,16);
    label.setAttribute(Qt::WA_TranslucentBackground);
    label.move(qApp->desktop()->availableGeometry().topLeft());
    label.show();
#ifdef Q_WS_X11
    qt_x11_wait_for_window_manager(&label);
#endif
    QTest::qWait(200);

    QPixmap widgetSnapshot;

#ifdef Q_WS_WIN
    QWidget *desktopWidget = QApplication::desktop()->screen(0);
    if (QSysInfo::windowsVersion() >= QSysInfo::WV_VISTA)
        widgetSnapshot = QPixmap::grabWindow(desktopWidget->winId(), 0,0, label.width(), label.height());
    else
#endif
        widgetSnapshot = QPixmap::grabWindow(label.winId());
    QImage actual = widgetSnapshot.toImage().convertToFormat(QImage::Format_RGB32);
    QImage expected = pm.toImage().convertToFormat(QImage::Format_RGB32);
    QCOMPARE(actual.size(),expected.size());
    QCOMPARE(actual,expected);
}

class MaskResizeTestWidget : public QWidget
{
    Q_OBJECT
public:
    MaskResizeTestWidget(QWidget* p =0)
            : QWidget(p) {
        setMask(QRegion(QRect(0, 0, 100, 100).normalized()));
    }

    void paintEvent(QPaintEvent* event) {
        QPainter p(this);

        paintedRegion += event->region();
        foreach(QRect r, event->region().rects())
            p.fillRect(r, Qt::red);
    }

    QRegion paintedRegion;

public slots:
    void enlargeMask() {
        QRegion newMask(QRect(0, 0, 150, 150).normalized());
        setMask(newMask);
    }

    void shrinkMask() {
        QRegion newMask(QRect(0, 0, 50, 50).normalized());
        setMask(newMask);
    }

};

void tst_QWidget::setClearAndResizeMask()
{
    UpdateWidget topLevel;
    topLevel.resize(150, 150);
    topLevel.show();
    QTest::qWaitForWindowShown(&topLevel);
    QTRY_VERIFY(topLevel.numPaintEvents > 0);
    topLevel.reset();

    // Mask top-level widget
    const QRegion topLevelMask(0, 0, 100, 100, QRegion::Ellipse);
    topLevel.setMask(topLevelMask);
    QCOMPARE(topLevel.mask(), topLevelMask);
#if defined(Q_WS_WIN) || defined(Q_WS_X11) // We don't control what's happening on other platforms.
    // and ensure that the top-level doesn't get any update.
    QCOMPARE(topLevel.numPaintEvents, 0);
#endif

    topLevel.reset();

    // Clear top-level mask
    topLevel.clearMask();
    QCOMPARE(topLevel.mask(), QRegion());
    QTest::qWait(10);
    QRegion outsideOldMask(topLevel.rect());
    outsideOldMask -= topLevelMask;
#if defined(Q_WS_WIN) || defined(Q_WS_X11) // We don't control what's happening on other platforms.
    // and ensure that the top-level gets an update for the area outside the old mask.
    QTRY_VERIFY(topLevel.numPaintEvents > 0);
    QTRY_COMPARE(topLevel.paintedRegion, outsideOldMask);
#endif

    UpdateWidget child(&topLevel);
    child.setAutoFillBackground(true); // NB! Opaque child.
    child.resize(100, 100);
    child.show();
    QTest::qWait(10);

    child.reset();
    topLevel.reset();

    // Mask child widget with a mask that is smaller than the rect
    const QRegion childMask(0, 0, 50, 50);
    child.setMask(childMask);
    QTRY_COMPARE(child.mask(), childMask);
    QTest::qWait(50);
    // and ensure that the child widget doesn't get any update.
#ifdef Q_WS_MAC
    // Mac always issues a full update when calling setMask, and we cannot force it to not do so.
    QCOMPARE(child.numPaintEvents, 1);
#else
    QCOMPARE(child.numPaintEvents, 0);
#endif
    // and the parent widget gets an update for the newly exposed area.
    QTRY_COMPARE(topLevel.numPaintEvents, 1);
    QRegion expectedParentExpose(child.rect());
    expectedParentExpose -= childMask;
    QCOMPARE(topLevel.paintedRegion, expectedParentExpose);

    child.reset();
    topLevel.reset();

    // Clear child widget mask
    child.clearMask();
    QTRY_COMPARE(child.mask(), QRegion());
    QTest::qWait(10);
    // and ensure that that the child widget gets an update for the area outside the old mask.
    QTRY_COMPARE(child.numPaintEvents, 1);
    outsideOldMask = child.rect();
#ifndef Q_WS_MAC
    // Mac always issues a full update when calling setMask, and we cannot force it to not do so.
    outsideOldMask -= childMask;
#endif
    QCOMPARE(child.paintedRegion, outsideOldMask);
    // and the parent widget doesn't get any update.
    QCOMPARE(topLevel.numPaintEvents, 0);

    child.reset();
    topLevel.reset();

    // Mask child widget with a mask that is bigger than the rect
    child.setMask(QRegion(0, 0, 1000, 1000));
    QTest::qWait(100);
#ifdef Q_WS_MAC
    // Mac always issues a full update when calling setMask, and we cannot force it to not do so.
    QTRY_COMPARE(child.numPaintEvents, 1);
#else
    // and ensure that we don't get any updates at all.
    QTRY_COMPARE(child.numPaintEvents, 0);
#endif
    QCOMPARE(topLevel.numPaintEvents, 0);

    // ...and the same applies when clearing the mask.
    child.clearMask();
    QTest::qWait(100);
#ifdef Q_WS_MAC
    // Mac always issues a full update when calling setMask, and we cannot force it to not do so.
    QTRY_VERIFY(child.numPaintEvents > 0);
#else
    QCOMPARE(child.numPaintEvents, 0);
#endif
    QCOMPARE(topLevel.numPaintEvents, 0);

    QWidget resizeParent;
    MaskResizeTestWidget resizeChild(&resizeParent);

    resizeParent.resize(300,300);
    resizeParent.raise();
    resizeParent.setWindowFlags(Qt::WindowStaysOnTopHint);
    resizeChild.setGeometry(50,50,200,200);
    QPalette pal = resizeParent.palette();
    pal.setColor(QPalette::Window, QColor(Qt::white));
    resizeParent.setPalette(pal);

    resizeParent.show();
    QTest::qWaitForWindowShown(&resizeParent);
    // Disable the size grip on the Mac; otherwise it'll be included when grabbing the window.
    resizeParent.setFixedSize(resizeParent.size());
    resizeChild.show();
    QTest::qWait(100);
    resizeChild.paintedRegion = QRegion();

    QTimer::singleShot(100, &resizeChild, SLOT(shrinkMask()));
    QTest::qWait(200);
#ifdef Q_WS_MAC
    // Mac always issues a full update when calling setMask, and we cannot force it to not do so.
    QTRY_COMPARE(resizeChild.paintedRegion, resizeChild.mask());
#else
    QTRY_COMPARE(resizeChild.paintedRegion, QRegion());
#endif

    resizeChild.paintedRegion = QRegion();
    const QRegion oldMask = resizeChild.mask();
    QTimer::singleShot(0, &resizeChild, SLOT(enlargeMask()));
    QTest::qWait(100);
#ifdef Q_WS_MAC
    // Mac always issues a full update when calling setMask, and we cannot force it to not do so.
    QTRY_COMPARE(resizeChild.paintedRegion, resizeChild.mask());
#else
    QTRY_COMPARE(resizeChild.paintedRegion, resizeChild.mask() - oldMask);
#endif
}

void tst_QWidget::maskedUpdate()
{
    UpdateWidget topLevel;
    topLevel.resize(200, 200);
    const QRegion topLevelMask(50, 50, 70, 70);
    topLevel.setMask(topLevelMask);

    UpdateWidget child(&topLevel);
    child.setGeometry(20, 20, 180, 180);
    const QRegion childMask(60, 60, 30, 30);
    child.setMask(childMask);

    UpdateWidget grandChild(&child);
    grandChild.setGeometry(50, 50, 100, 100);
    const QRegion grandChildMask(20, 20, 10, 10);
    grandChild.setMask(grandChildMask);

    topLevel.show();
    QTest::qWaitForWindowShown(&topLevel);
    QTRY_VERIFY(topLevel.numPaintEvents > 0);


#define RESET_WIDGETS \
    topLevel.reset(); \
    child.reset(); \
    grandChild.reset();

#define CLEAR_MASK(widget) \
    widget.clearMask(); \
    QTest::qWait(100); \
    RESET_WIDGETS;

    // All widgets are transparent at this point, so any call to update() will result
    // in composition, i.e. the update propagates to ancestors and children.

    // TopLevel update.
    RESET_WIDGETS;
    topLevel.update();
    QTest::qWait(10);

    QTRY_COMPARE(topLevel.paintedRegion, topLevelMask);
    QTRY_COMPARE(child.paintedRegion, childMask);
    QTRY_COMPARE(grandChild.paintedRegion, grandChildMask);

    // Child update.
    RESET_WIDGETS;
    child.update();
    QTest::qWait(10);

    QTRY_COMPARE(topLevel.paintedRegion, childMask.translated(child.pos()));
    QTRY_COMPARE(child.paintedRegion, childMask);
    QTRY_COMPARE(grandChild.paintedRegion, grandChildMask);

    // GrandChild update.
    RESET_WIDGETS;
    grandChild.update();
    QTest::qWait(10);

    QTRY_COMPARE(topLevel.paintedRegion, grandChildMask.translated(grandChild.mapTo(&topLevel, QPoint())));
    QTRY_COMPARE(child.paintedRegion, grandChildMask.translated(grandChild.pos()));
    QTRY_COMPARE(grandChild.paintedRegion, grandChildMask);

    topLevel.setAttribute(Qt::WA_OpaquePaintEvent);
    child.setAttribute(Qt::WA_OpaquePaintEvent);
    grandChild.setAttribute(Qt::WA_OpaquePaintEvent);

    // All widgets are now opaque, which means no composition, i.e.
    // the update does not propate to ancestors and children.

    // TopLevel update.
    RESET_WIDGETS;
    topLevel.update();
    QTest::qWait(10);

    QRegion expectedTopLevelUpdate = topLevelMask;
    expectedTopLevelUpdate -= childMask.translated(child.pos()); // Subtract opaque children.
    QTRY_COMPARE(topLevel.paintedRegion, expectedTopLevelUpdate);
    QTRY_COMPARE(child.paintedRegion, QRegion());
    QTRY_COMPARE(grandChild.paintedRegion, QRegion());

    // Child update.
    RESET_WIDGETS;
    child.update();
    QTest::qWait(10);

    QTRY_COMPARE(topLevel.paintedRegion, QRegion());
    QRegion expectedChildUpdate = childMask;
    expectedChildUpdate -= grandChildMask.translated(grandChild.pos()); // Subtract oapque children.
    QTRY_COMPARE(child.paintedRegion, expectedChildUpdate);
    QTRY_COMPARE(grandChild.paintedRegion, QRegion());

    // GrandChild update.
    RESET_WIDGETS;
    grandChild.update();
    QTest::qWait(10);

    QTRY_COMPARE(topLevel.paintedRegion, QRegion());
    QTRY_COMPARE(child.paintedRegion, QRegion());
    QTRY_COMPARE(grandChild.paintedRegion, grandChildMask);

    // GrandChild update.
    CLEAR_MASK(grandChild);
    grandChild.update();
    QTest::qWait(10);

    QTRY_COMPARE(topLevel.paintedRegion, QRegion());
    QTRY_COMPARE(child.paintedRegion, QRegion());
    QRegion expectedGrandChildUpdate = grandChild.rect();
    // Clip with parent's mask.
    expectedGrandChildUpdate &= childMask.translated(-grandChild.pos());
    QCOMPARE(grandChild.paintedRegion, expectedGrandChildUpdate);

    // GrandChild update.
    CLEAR_MASK(child);
    grandChild.update();
    QTest::qWait(10);

    QTRY_COMPARE(topLevel.paintedRegion, QRegion());
    QTRY_COMPARE(child.paintedRegion, QRegion());
    expectedGrandChildUpdate = grandChild.rect();
    // Clip with parent's mask.
    expectedGrandChildUpdate &= topLevelMask.translated(-grandChild.mapTo(&topLevel, QPoint()));
    QTRY_COMPARE(grandChild.paintedRegion, expectedGrandChildUpdate);

    // Child update.
    RESET_WIDGETS;
    child.update();
    QTest::qWait(10);

    QTRY_COMPARE(topLevel.paintedRegion, QRegion());
    expectedChildUpdate = child.rect();
    // Clip with parent's mask.
    expectedChildUpdate &= topLevelMask.translated(-child.pos());
    expectedChildUpdate -= grandChild.geometry(); // Subtract opaque children.
    QTRY_COMPARE(child.paintedRegion, expectedChildUpdate);
    QTRY_COMPARE(grandChild.paintedRegion, QRegion());

    // GrandChild update.
    CLEAR_MASK(topLevel);
    grandChild.update();
    QTest::qWait(10);

    QTRY_COMPARE(topLevel.paintedRegion, QRegion());
    QTRY_COMPARE(child.paintedRegion, QRegion());
    QTRY_COMPARE(grandChild.paintedRegion, QRegion(grandChild.rect())); // Full update.
}

#if defined(Q_WS_X11) || defined(Q_WS_WIN) || defined(Q_WS_QWS)
void tst_QWidget::syntheticEnterLeave()
{
    class MyWidget : public QWidget
    {
    public:
        MyWidget(QWidget *parent = 0) : QWidget(parent), numEnterEvents(0), numLeaveEvents(0) {}
        void enterEvent(QEvent *) { ++numEnterEvents; }
        void leaveEvent(QEvent *) { ++numLeaveEvents; }
        int numEnterEvents;
        int numLeaveEvents;
    };

    QCursor::setPos(QPoint(0,0));

    MyWidget window;
    window.setWindowFlags(Qt::WindowStaysOnTopHint);
    window.resize(200, 200);

    MyWidget *child1 = new MyWidget(&window);
    child1->setPalette(Qt::blue);
    child1->setAutoFillBackground(true);
    child1->resize(200, 200);
    child1->setCursor(Qt::OpenHandCursor);

    MyWidget *child2 = new MyWidget(&window);
    child2->resize(200, 200);

    MyWidget *grandChild = new MyWidget(child2);
    grandChild->setPalette(Qt::red);
    grandChild->setAutoFillBackground(true);
    grandChild->resize(200, 200);
    grandChild->setCursor(Qt::WaitCursor);

    window.show();
    window.raise();
#ifdef Q_WS_X11
    qt_x11_wait_for_window_manager(&window);
#endif
    QTest::qWait(300);

#define RESET_EVENT_COUNTS \
    window.numEnterEvents = 0; \
    window.numLeaveEvents = 0; \
    child1->numEnterEvents = 0; \
    child1->numLeaveEvents = 0; \
    child2->numEnterEvents = 0; \
    child2->numLeaveEvents = 0; \
    grandChild->numEnterEvents = 0; \
    grandChild->numLeaveEvents = 0;

    // Position the cursor in the middle of the window.
    const QPoint globalPos = window.mapToGlobal(QPoint(100, 100));
    QCursor::setPos(globalPos); // Enter child2 and grandChild.
    QTest::qWait(300);

#ifdef Q_OS_WINCE_WM
    QSKIP("Windows Mobile has no proper cursor support", SkipAll);
#endif

    QCOMPARE(window.numLeaveEvents, 0);
    QCOMPARE(child2->numLeaveEvents, 0);
    QCOMPARE(grandChild->numLeaveEvents, 0);
    QCOMPARE(child1->numLeaveEvents, 0);

    // This event arrives asynchronously
    QTRY_COMPARE(window.numEnterEvents, 1);
    QCOMPARE(child2->numEnterEvents, 1);
    QCOMPARE(grandChild->numEnterEvents, 1);
    QCOMPARE(child1->numEnterEvents, 0);

    RESET_EVENT_COUNTS;
    child2->hide(); // Leave child2 and grandChild, enter child1.

    QCOMPARE(window.numLeaveEvents, 0);
    QCOMPARE(child2->numLeaveEvents, 1);
    QCOMPARE(grandChild->numLeaveEvents, 1);
    QCOMPARE(child1->numLeaveEvents, 0);

    QCOMPARE(window.numEnterEvents, 0);
    QCOMPARE(child2->numEnterEvents, 0);
    QCOMPARE(grandChild->numEnterEvents, 0);
    QCOMPARE(child1->numEnterEvents, 1);

    RESET_EVENT_COUNTS;
    child2->show(); // Leave child1, enter child2 and grandChild.

    QCOMPARE(window.numLeaveEvents, 0);
    QCOMPARE(child2->numLeaveEvents, 0);
    QCOMPARE(grandChild->numLeaveEvents, 0);
    QCOMPARE(child1->numLeaveEvents, 1);

    QCOMPARE(window.numEnterEvents, 0);
    QCOMPARE(child2->numEnterEvents, 1);
    QCOMPARE(grandChild->numEnterEvents, 1);
    QCOMPARE(child1->numEnterEvents, 0);

    RESET_EVENT_COUNTS;
    delete child2; // Enter child1 (and do not send leave events to child2 and grandChild).

    QCOMPARE(window.numLeaveEvents, 0);
    QCOMPARE(child1->numLeaveEvents, 0);

    QCOMPARE(window.numEnterEvents, 0);
    QCOMPARE(child1->numEnterEvents, 1);
}

void tst_QWidget::taskQTBUG_4055_sendSyntheticEnterLeave()
{
#ifdef Q_OS_WINCE_WM
    QSKIP("Windows Mobile has no proper cursor support", SkipAll);
#endif
    class SELParent : public QWidget
    {
    public:
        SELParent(QWidget *parent = 0): QWidget(parent) { }

        void mousePressEvent(QMouseEvent *) { child->show(); }
        QWidget *child;
    };

    class SELChild : public QWidget
     {
     public:
         SELChild(QWidget *parent = 0) : QWidget(parent), numEnterEvents(0), numMouseMoveEvents(0) {}
         void enterEvent(QEvent *) { ++numEnterEvents; }
         void mouseMoveEvent(QMouseEvent *event)
         {
             QCOMPARE(event->button(), Qt::NoButton);
             QCOMPARE(event->buttons(), Qt::MouseButtons(Qt::NoButton));
             ++numMouseMoveEvents;
         }
         void reset() { numEnterEvents = numMouseMoveEvents = 0; }
         int numEnterEvents, numMouseMoveEvents;
     };

    QCursor::setPos(QPoint(0,0));

     SELParent parent;
     parent.resize(200, 200);
     SELChild child(&parent);
     child.resize(200, 200);
     parent.show();
 #ifdef Q_WS_X11
     qt_x11_wait_for_window_manager(&parent);
 #endif
     QTest::qWait(150);

     QCursor::setPos(child.mapToGlobal(QPoint(100, 100)));
     // Make sure the cursor has entered the child.
     QTRY_VERIFY(child.numEnterEvents > 0);

     child.hide();
     child.reset();
     child.show();

     // Make sure the child gets enter event and no mouse move event.
     QTRY_COMPARE(child.numEnterEvents, 1);
     QCOMPARE(child.numMouseMoveEvents, 0);

     child.hide();
     child.reset();
     child.setMouseTracking(true);
     child.show();

     // Make sure the child gets enter event and mouse move event.
     // Note that we verify event->button() and event->buttons()
     // in SELChild::mouseMoveEvent().
     QTRY_COMPARE(child.numEnterEvents, 1);
     QCOMPARE(child.numMouseMoveEvents, 1);

     // Sending synthetic enter/leave trough the parent's mousePressEvent handler.
     parent.child = &child;

     child.hide();
     child.reset();
     QTest::mouseClick(&parent, Qt::LeftButton);

     // Make sure the child gets enter event and one mouse move event.
     QTRY_COMPARE(child.numEnterEvents, 1);
     QCOMPARE(child.numMouseMoveEvents, 1);

     child.hide();
     child.reset();
     child.setMouseTracking(false);
     QTest::mouseClick(&parent, Qt::LeftButton);

     // Make sure the child gets enter event and no mouse move event.
     QTRY_COMPARE(child.numEnterEvents, 1);
     QCOMPARE(child.numMouseMoveEvents, 0);
 }
#endif

void tst_QWidget::windowFlags()
{
    QWidget w;
    w.setWindowFlags(w.windowFlags() | Qt::FramelessWindowHint);
    QVERIFY(w.windowFlags() & Qt::FramelessWindowHint);
}

void tst_QWidget::initialPosForDontShowOnScreenWidgets()
{
    { // Check default position.
        const QPoint expectedPos(0, 0);
        QWidget widget;
        widget.setAttribute(Qt::WA_DontShowOnScreen);
        widget.winId(); // Make sure create_sys is called.
        QCOMPARE(widget.pos(), expectedPos);
        QCOMPARE(widget.geometry().topLeft(), expectedPos);
    }

    { // Explicitly move to a position.
        const QPoint expectedPos(100, 100);
        QWidget widget;
        widget.setAttribute(Qt::WA_DontShowOnScreen);
        widget.move(expectedPos);
        widget.winId(); // Make sure create_sys is called.
        QCOMPARE(widget.pos(), expectedPos);
        QCOMPARE(widget.geometry().topLeft(), expectedPos);
    }
}

#ifdef Q_WS_X11
void tst_QWidget::paintOutsidePaintEvent()
{
    QWidget widget;
    widget.resize(200, 200);

    QWidget child1(&widget);
    child1.resize(100, 100);
    child1.setPalette(Qt::red);
    child1.setAutoFillBackground(true);

    QWidget child2(&widget);
    child2.setGeometry(50, 50, 100, 100);
    child2.setPalette(Qt::blue);
    child2.setAutoFillBackground(true);

    widget.show();
    QTest::qWaitForWindowShown(&widget);
    QTest::qWait(60);

    const QPixmap before = QPixmap::grabWindow(widget.winId());

    // Child 1 should be clipped by child 2, so nothing should change.
    child1.setAttribute(Qt::WA_PaintOutsidePaintEvent);
    QPainter painter(&child1);
    painter.fillRect(child1.rect(), Qt::red);
    painter.end();
    XSync(QX11Info::display(), false); // Flush output buffer.
    QTest::qWait(60);

    const QPixmap after = QPixmap::grabWindow(widget.winId());

    QCOMPARE(before, after);
}
#endif

class MyEvilObject : public QObject
{
    Q_OBJECT
public:
    MyEvilObject(QWidget *widgetToCrash) : QObject(), widget(widgetToCrash)
    {
        connect(widget, SIGNAL(destroyed(QObject *)), this, SLOT(beEvil(QObject *)));
        delete widget;
    }
    QWidget *widget;

private slots:
    void beEvil(QObject *) { widget->update(0, 0, 150, 150); }
};

void tst_QWidget::updateOnDestroyedSignal()
{
    QWidget widget;

    QWidget *child = new QWidget(&widget);
    child->resize(100, 100);
    child->setAutoFillBackground(true);
    child->setPalette(Qt::red);

    widget.show();
#ifdef Q_WS_X11
    qt_x11_wait_for_window_manager(&widget);
#endif
    QTest::qWait(200);

    // Please do not crash.
    MyEvilObject evil(child);
    QTest::qWait(200);
}

void tst_QWidget::toplevelLineEditFocus()
{
    testWidget->hide();

    QLineEdit w;
    w.show();
    QTest::qWaitForWindowShown(&w);
    QTest::qWait(20);

    QTRY_COMPARE(QApplication::activeWindow(), (QWidget*)&w);
    QTRY_COMPARE(QApplication::focusWidget(), (QWidget*)&w);
}

void tst_QWidget::focusWidget_task254563()
{
    //having different visibility for widget is important
    QWidget top;
    top.show();
    QWidget container(&top);
    QWidget *widget = new QWidget(&container);
    widget->show();

    widget->setFocus(); //set focus (will set the focus widget up to the toplevel to be 'widget')
    container.setFocus();
    delete widget; // will call clearFocus but that doesn't help
    QVERIFY(top.focusWidget() != widget); //dangling pointer
}

void tst_QWidget::destroyBackingStore()
{
#ifdef QT_BUILD_INTERNAL
    UpdateWidget w;
    w.reset();
    w.show();

    QTest::qWaitForWindowShown(&w);
    QApplication::processEvents();
    QTRY_VERIFY(w.numPaintEvents > 0);
    w.reset();
    w.update();
    qt_widget_private(&w)->topData()->backingStore.create(&w);

    w.update();
    QApplication::processEvents();
#ifdef Q_WS_QWS
    QApplication::processEvents();
#endif
    QCOMPARE(w.numPaintEvents, 1);

    // Check one more time, because the second time around does more caching.
    w.update();
    QApplication::processEvents();
    QCOMPARE(w.numPaintEvents, 2);
#else
    QSKIP("Test case relies on developer build (AUTOTEST_EXPORT)", SkipAll);
#endif
}

// Helper function
QWidgetBackingStore* backingStore(QWidget &widget)
{
    QWidgetBackingStore *backingStore = 0;
#ifdef QT_BUILD_INTERNAL
    if (QTLWExtra *topExtra = qt_widget_private(&widget)->maybeTopData())
        backingStore = topExtra->backingStore.data();
#endif
    return backingStore;
}

// Wait for a condition to be true, timing out after 1 second
// This is used following calls to QWidget::show() and QWidget::hide(), which are
// expected to asynchronously trigger native window visibility events.
#define WAIT_AND_VERIFY(condition)                                              \
    do {                                                                        \
        QTime start = QTime::currentTime();                                     \
        while (!(condition) && (start.elapsed() < 1000)) {                      \
            qApp->processEvents();                                              \
            QTest::qWait(50);                                                   \
        }                                                                       \
        if (!QTest::qVerify((condition), #condition, "", __FILE__, __LINE__))   \
            return;                                                             \
    } while (0)

void tst_QWidget::destroyBackingStoreWhenHidden()
{
#ifndef QT_BUILD_INTERNAL
    QSKIP("Test step requires access to Q_AUTOTEST_EXPORT", SkipAll);
#endif

#ifndef Q_OS_SYMBIAN
    QSKIP("Only Symbian destroys backing store when native window becomes invisible", SkipAll);
#endif

    testWidget->hide();
    QTest::qWait(1000);

    // 1. Single top-level QWidget
    {
    QWidget w;
    w.setAutoFillBackground(true);
    w.setPalette(Qt::yellow);
    w.setGeometry(0, 0, 100, 100);
    w.show();
    QTest::qWaitForWindowShown(&w);
    QVERIFY(0 != backingStore(w));

    w.hide();
    WAIT_AND_VERIFY(0 == backingStore(w));

    w.show();
    QTest::qWaitForWindowShown(&w);
    QVERIFY(0 != backingStore(w));
    }

    // 2. Two top-level widgets
    {
    QWidget w1;
    w1.setGeometry(0, 0, 100, 100);
    w1.setAutoFillBackground(true);
    w1.setPalette(Qt::red);
    w1.show();
    QTest::qWaitForWindowShown(&w1);
    QVERIFY(0 != backingStore(w1));

    QWidget w2;
    w2.setGeometry(w1.geometry());
    w1.setAutoFillBackground(true);
    w1.setPalette(Qt::blue);
    w2.show();
    QTest::qWaitForWindowShown(&w2);
    QVERIFY(0 != backingStore(w2));

    // Check that w1 deleted its backing store when obscured by w2
    QVERIFY(0 == backingStore(w1));

    w2.move(w2.pos() + QPoint(10, 10));

    // Check that w1 recreates its backing store when partially revealed
    WAIT_AND_VERIFY(0 != backingStore(w1));
    }

    // 3. Native child widget
    {
    QWidget parent;
    parent.setGeometry(0, 0, 100, 100);
    parent.setAutoFillBackground(true);
    parent.setPalette(Qt::yellow);

    QWidget child(&parent);
    child.setAutoFillBackground(true);
    child.setPalette(Qt::green);

    QVBoxLayout layout(&parent);
    layout.setContentsMargins(10, 10, 10, 10);
    layout.addWidget(&child);
    parent.setLayout(&layout);

    child.winId();

    parent.show();
    QTest::qWaitForWindowShown(&parent);

    // Check that child window does not obscure parent window
    QVERIFY(!parent.visibleRegion().subtracted(child.visibleRegion()).isEmpty());

    // Native child widget should share parent's backing store
    QWidgetBackingStore *const parentBs = backingStore(parent);
    QVERIFY(0 != parentBs);
    QVERIFY(0 == backingStore(child));

    // Set margins to zero so that child widget totally obscures parent
    layout.setContentsMargins(0, 0, 0, 0);

    WAIT_AND_VERIFY(parent.visibleRegion().subtracted(child.visibleRegion()).isEmpty());

    // Backing store should remain unchanged despite child window obscuring
    // parent window
    QVERIFY(parentBs == backingStore(parent));
    QVERIFY(0 == backingStore(child));
    }

    // 4. Alien child widget which is made full-screen
    {
    QWidget parent;
    parent.setGeometry(0, 0, 100, 100);
    parent.setAutoFillBackground(true);
    parent.setPalette(Qt::red);

    QWidget child(&parent);
    child.setAutoFillBackground(true);
    child.setPalette(Qt::blue);

    QVBoxLayout layout(&parent);
    layout.setContentsMargins(10, 10, 10, 10);
    layout.addWidget(&child);
    parent.setLayout(&layout);

    parent.show();
    QTest::qWaitForWindowShown(&parent);

    // Check that child window does not obscure parent window
    QVERIFY(!parent.visibleRegion().subtracted(child.visibleRegion()).isEmpty());

    // Native child widget should share parent's backing store
    QVERIFY(0 != backingStore(parent));
    QVERIFY(0 == backingStore(child));

    // Make child widget full screen
    child.setWindowFlags((child.windowFlags() | Qt::Window) ^ Qt::SubWindow);
    child.setWindowState(child.windowState() | Qt::WindowFullScreen);
    child.show();
    QTest::qWaitForWindowShown(&child);

    // Check that child window obscures parent window
    QVERIFY(parent.visibleRegion().subtracted(child.visibleRegion()).isEmpty());

    // Now that extent of child widget goes beyond parent's extent,
    // a new backing store should be created for the child widget.
    QVERIFY(0 != backingStore(child));

    // Parent is obscured, therefore its backing store should be destroyed
    QVERIFY(0 == backingStore(parent));

    // Disable full screen
    child.setWindowFlags(child.windowFlags() ^ (Qt::Window | Qt::SubWindow));
    child.setWindowState(child.windowState() ^ Qt::WindowFullScreen);
    child.show();
    QTest::qWaitForWindowShown(&child);

    // Check that parent is now visible again
    QVERIFY(!parent.visibleRegion().subtracted(child.visibleRegion()).isEmpty());

    // Native child widget should once again share parent's backing store
    QVERIFY(0 != backingStore(parent));
    QVERIFY(0 == backingStore(child));
    }

    // 5. Native child widget which is made full-screen
    {
    QWidget parent;
    parent.setGeometry(0, 0, 100, 100);
    parent.setAutoFillBackground(true);
    parent.setPalette(Qt::red);

    QWidget child(&parent);
    child.setAutoFillBackground(true);
    child.setPalette(Qt::blue);

    QVBoxLayout layout(&parent);
    layout.setContentsMargins(10, 10, 10, 10);
    layout.addWidget(&child);
    parent.setLayout(&layout);

    child.winId();

    parent.show();
    QTest::qWaitForWindowShown(&parent);

    // Check that child window does not obscure parent window
    QVERIFY(!parent.visibleRegion().subtracted(child.visibleRegion()).isEmpty());

    // Native child widget should share parent's backing store
    QVERIFY(0 != backingStore(parent));
    QVERIFY(0 == backingStore(child));

    // Make child widget full screen
    child.setWindowFlags((child.windowFlags() | Qt::Window) ^ Qt::SubWindow);
    child.setWindowState(child.windowState() | Qt::WindowFullScreen);
    child.show();
    QTest::qWaitForWindowShown(&child);

    // Ensure that 'window hidden' event is received by parent
    qApp->processEvents();

    // Check that child window obscures parent window
    QVERIFY(parent.visibleRegion().subtracted(child.visibleRegion()).isEmpty());

    // Now that extent of child widget goes beyond parent's extent,
    // a new backing store should be created for the child widget.
    QVERIFY(0 != backingStore(child));

    // Parent is obscured, therefore its backing store should be destroyed
    QEXPECT_FAIL("", "QTBUG-12406", Continue);
    QVERIFY(0 == backingStore(parent));

    // Disable full screen
    child.setWindowFlags(child.windowFlags() ^ (Qt::Window | Qt::SubWindow));
    child.setWindowState(child.windowState() ^ Qt::WindowFullScreen);
    child.show();
    QTest::qWaitForWindowShown(&child);

    // Check that parent is now visible again
    QVERIFY(!parent.visibleRegion().subtracted(child.visibleRegion()).isEmpty());

    // Native child widget should once again share parent's backing store
    QVERIFY(0 != backingStore(parent));
    QVERIFY(0 == backingStore(child));
    }

    // 6. Partial reveal followed by full reveal
    {
    QWidget upper;
    upper.setAutoFillBackground(true);
    upper.setPalette(Qt::red);
    upper.setGeometry(50, 50, 100, 100);

    QWidget lower;
    lower.setAutoFillBackground(true);
    lower.setPalette(Qt::green);
    lower.setGeometry(50, 50, 100, 100);

    lower.show();
    QTest::qWaitForWindowShown(&lower);
    upper.show();
    QTest::qWaitForWindowShown(&upper);
    upper.raise();

    QVERIFY(0 != backingStore(upper));
    QVERIFY(0 == backingStore(lower));

    // Check that upper obscures lower
    QVERIFY(lower.visibleRegion().subtracted(upper.visibleRegion()).isEmpty());

    // Partially reveal lower
    upper.move(100, 100);

    // Completely reveal lower
    upper.hide();

    // Hide lower widget - this should cause its backing store to be deleted
    lower.hide();

    // Check that backing store was deleted
    WAIT_AND_VERIFY(0 == backingStore(lower));
    }

    // 7. Reparenting of visible native child widget
    {
    QWidget parent1;
    parent1.setAutoFillBackground(true);
    parent1.setPalette(Qt::green);
    parent1.setGeometry(50, 50, 100, 100);

    QWidget *child = new QWidget(&parent1);
    child->winId();
    child->setAutoFillBackground(true);
    child->setPalette(Qt::red);
    child->setGeometry(10, 10, 30, 30);

    QWidget parent2;
    parent2.setAutoFillBackground(true);
    parent2.setPalette(Qt::blue);
    parent2.setGeometry(150, 150, 100, 100);

    parent1.show();
    QTest::qWaitForWindowShown(&parent1);
    QVERIFY(0 != backingStore(parent1));

    parent2.show();
    QTest::qWaitForWindowShown(&parent2);
    QVERIFY(0 != backingStore(parent2));

    child->setParent(&parent2);
    child->setGeometry(10, 10, 30, 30);
    child->show();

    parent1.hide();
    WAIT_AND_VERIFY(0 == backingStore(parent1));

    parent2.hide();
    WAIT_AND_VERIFY(0 == backingStore(parent2));
    }
}

#undef WAIT_AND_VERIFY

void tst_QWidget::rectOutsideCoordinatesLimit_task144779()
{
#ifdef Q_OS_WINCE_WM
    QSKIP( "Tables of 5000 elements do not make sense on Windows Mobile.", SkipAll);
#endif
    QApplication::setOverrideCursor(Qt::BlankCursor); //keep the cursor out of screen grabs
    QWidget main(0,Qt::FramelessWindowHint); //don't get confused by the size of the window frame
    QPalette palette;
    palette.setColor(QPalette::Window, Qt::red);
    main.setPalette(palette);

    QDesktopWidget desktop;
    QRect desktopDimensions = desktop.availableGeometry(&main);
    QSize mainSize(400, 400);
    mainSize = mainSize.boundedTo(desktopDimensions.size());
    main.resize(mainSize);

    QWidget *offsetWidget = new QWidget(&main);
    offsetWidget->setGeometry(0, -(15000 - mainSize.height()), mainSize.width(), 15000);

    // big widget is too big for the coordinates, it must be limited by wrect
    // if wrect is not at the right position because of offsetWidget, bigwidget
    // is not painted correctly
    QWidget *bigWidget = new QWidget(offsetWidget);
    bigWidget->setGeometry(0, 0, mainSize.width(), 50000);
    palette.setColor(QPalette::Window, Qt::green);
    bigWidget->setPalette(palette);
    bigWidget->setAutoFillBackground(true);

    main.show();
    QTest::qWaitForWindowShown(&main);

    QPixmap correct(main.size());
    correct.fill(Qt::green);

    QTRY_COMPARE(QPixmap::grabWindow(main.winId()).toImage().convertToFormat(QImage::Format_RGB32),
                 correct.toImage().convertToFormat(QImage::Format_RGB32));
    QApplication::restoreOverrideCursor();
}

void tst_QWidget::inputFocus_task257832()
{
      QLineEdit *widget = new QLineEdit;
      QInputContext *context = widget->inputContext();
      if (!context)
            QSKIP("No input context", SkipSingle);
      widget->setFocus();
      widget->winId();    // make sure, widget has been created
      context->setFocusWidget(widget);
      QCOMPARE(context->focusWidget(), static_cast<QWidget*>(widget));
      widget->setReadOnly(true);
      QVERIFY(!context->focusWidget());
      delete widget;
}

void tst_QWidget::setGraphicsEffect()
{
    // Check that we don't have any effect by default.
    QWidget *widget = new QWidget;
    QVERIFY(!widget->graphicsEffect());

    // SetGet check.
    QPointer<QGraphicsEffect> blurEffect = new QGraphicsBlurEffect;
    widget->setGraphicsEffect(blurEffect);
    QCOMPARE(widget->graphicsEffect(), static_cast<QGraphicsEffect *>(blurEffect));

    // Ensure the existing effect is deleted when setting a new one.
    QPointer<QGraphicsEffect> shadowEffect = new QGraphicsDropShadowEffect;
    widget->setGraphicsEffect(shadowEffect);
    QVERIFY(!blurEffect);
    QCOMPARE(widget->graphicsEffect(), static_cast<QGraphicsEffect *>(shadowEffect));
    blurEffect = new QGraphicsBlurEffect;

    // Ensure the effect is uninstalled when setting it on a new target.
    QWidget *anotherWidget = new QWidget;
    anotherWidget->setGraphicsEffect(blurEffect);
    widget->setGraphicsEffect(blurEffect);
    QVERIFY(!anotherWidget->graphicsEffect());
    QVERIFY(!shadowEffect);

    // Ensure the existing effect is deleted when deleting the widget.
    delete widget;
    QVERIFY(!blurEffect);
    delete anotherWidget;

    // Ensure the effect is uninstalled when deleting it
    widget = new QWidget;
    blurEffect = new QGraphicsBlurEffect;
    widget->setGraphicsEffect(blurEffect);
    delete blurEffect;
    QVERIFY(!widget->graphicsEffect());

    // Ensure the existing effect is uninstalled and deleted when setting a null effect
    blurEffect = new QGraphicsBlurEffect;
    widget->setGraphicsEffect(blurEffect);
    widget->setGraphicsEffect(0);
    QVERIFY(!widget->graphicsEffect());
    QVERIFY(!blurEffect);

    delete widget;
}

void tst_QWidget::activateWindow()
{
    // Test case for task 260685

    // Create first mainwindow and set it active
    QMainWindow* mainwindow = new QMainWindow();
    QLabel* label = new QLabel(mainwindow);
    mainwindow->setCentralWidget(label);
    mainwindow->setVisible(true);
    mainwindow->activateWindow();
    QTest::qWaitForWindowShown(mainwindow);
    qApp->processEvents();

    QTRY_VERIFY(mainwindow->isActiveWindow());

    // Create second mainwindow and set it active
    QMainWindow* mainwindow2 = new QMainWindow();
    QLabel* label2 = new QLabel(mainwindow2);
    mainwindow2->setCentralWidget(label2);
    mainwindow2->setVisible(true);
    mainwindow2->activateWindow();
    qApp->processEvents();

    QTRY_VERIFY(!mainwindow->isActiveWindow());
    QTRY_VERIFY(mainwindow2->isActiveWindow());

    // Revert first mainwindow back to visible active
    mainwindow->setVisible(true);
    mainwindow->activateWindow();
    qApp->processEvents();

    QTRY_VERIFY(mainwindow->isActiveWindow());
    QTRY_VERIFY(!mainwindow2->isActiveWindow());
}

void tst_QWidget::openModal_taskQTBUG_5804()
{
    class Widget : public QWidget
    {
    public:
        Widget(QWidget *parent) : QWidget(parent)
        {
        }
        ~Widget()
        {
            QMessageBox msgbox;
            QTimer::singleShot(10, &msgbox, SLOT(accept()));
            msgbox.exec(); //open a modal dialog
        }
    };

    QWidget *win = new QWidget;
    new Widget(win);
    win->show();
    QTest::qWaitForWindowShown(win);
    delete win;
}

#ifdef Q_OS_SYMBIAN
void tst_QWidget::cbaVisibility()
{
    // Test case for task 261048

    // Create first mainwindow in fullsreen and activate it
    QMainWindow* mainwindow = new QMainWindow();
    QLabel* label = new QLabel(mainwindow);
    mainwindow->setCentralWidget(label);
    mainwindow->setWindowState(Qt::WindowFullScreen);
    mainwindow->setVisible(true);
    mainwindow->activateWindow();
    qApp->processEvents();

    QVERIFY(mainwindow->isActiveWindow());
    QVERIFY(QDesktopWidget().availableGeometry().size() == mainwindow->size());

    // Create second mainwindow in maximized and activate it
    QMainWindow* mainwindow2 = new QMainWindow();
    QLabel* label2 = new QLabel(mainwindow2);
    mainwindow2->setCentralWidget(label2);
    mainwindow2->setWindowState(Qt::WindowMaximized);
    mainwindow2->setVisible(true);
    mainwindow2->activateWindow();
    qApp->processEvents();

    QVERIFY(!mainwindow->isActiveWindow());
    QVERIFY(mainwindow2->isActiveWindow());
    QVERIFY(QDesktopWidget().availableGeometry().size() == mainwindow2->size());

    // Verify window decorations i.e. status pane and CBA are visible.
    CEikStatusPane* statusPane = CEikonEnv::Static()->AppUiFactory()->StatusPane();
    QVERIFY(statusPane->IsVisible());
    CEikButtonGroupContainer* buttonGroup = CEikonEnv::Static()->AppUiFactory()->Cba();
    QVERIFY(buttonGroup->IsVisible());
}

void tst_QWidget::fullScreenWindowModeTransitions()
{
    QWidget widget;
    QVBoxLayout *layout = new QVBoxLayout;
    QPushButton *button = new QPushButton("test Button");
    layout->addWidget(button);
    widget.setLayout(layout);
    widget.show();

    const QRect normalGeometry = widget.normalGeometry();
    const QRect fullScreenGeometry = qApp->desktop()->screenGeometry(&widget);
    const QRect maximumScreenGeometry = qApp->desktop()->availableGeometry(&widget);
    CEikStatusPane *statusPane = CEikonEnv::Static()->AppUiFactory()->StatusPane();
    CEikButtonGroupContainer *buttonGroup = CEikonEnv::Static()->AppUiFactory()->Cba();

    //Enter
    widget.showNormal();
    widget.showFullScreen();
    QCOMPARE(widget.geometry(), fullScreenGeometry);
    QVERIFY(!buttonGroup->IsVisible());
    QVERIFY(!statusPane->IsVisible());

    widget.showMaximized();
    widget.showFullScreen();
    QCOMPARE(widget.geometry(), fullScreenGeometry);
    QVERIFY(!buttonGroup->IsVisible());
    QVERIFY(!statusPane->IsVisible());

    widget.showMinimized();
    widget.showFullScreen();
    QCOMPARE(widget.geometry(), fullScreenGeometry);
    QVERIFY(!buttonGroup->IsVisible());
    QVERIFY(!statusPane->IsVisible());

    //Exit
    widget.showFullScreen();
    widget.showNormal();
    QCOMPARE(widget.geometry(), normalGeometry);
    QVERIFY(buttonGroup->IsVisible());
    QVERIFY(statusPane->IsVisible());

    widget.showFullScreen();
    widget.showMaximized();
    QCOMPARE(widget.geometry(), maximumScreenGeometry);
    QVERIFY(buttonGroup->IsVisible());
    QVERIFY(statusPane->IsVisible());

    widget.showFullScreen();
    widget.showMinimized();
    QCOMPARE(widget.geometry(), fullScreenGeometry);
    QVERIFY(!buttonGroup->IsVisible());
    QVERIFY(!statusPane->IsVisible());
}

void tst_QWidget::maximizedWindowModeTransitions()
{
    QWidget widget;
    QVBoxLayout *layout = new QVBoxLayout;
    QPushButton *button = new QPushButton("test Button");
    layout->addWidget(button);
    widget.setLayout(layout);
    widget.show();

    const QRect normalGeometry = widget.normalGeometry();
    const QRect fullScreenGeometry = qApp->desktop()->screenGeometry(&widget);
    const QRect maximumScreenGeometry = qApp->desktop()->availableGeometry(&widget);
    CEikStatusPane *statusPane = CEikonEnv::Static()->AppUiFactory()->StatusPane();
    CEikButtonGroupContainer *buttonGroup = CEikonEnv::Static()->AppUiFactory()->Cba();

    //Enter
    widget.showNormal();
    widget.showMaximized();
    QCOMPARE(widget.geometry(), maximumScreenGeometry);
    QVERIFY(buttonGroup->IsVisible());
    QVERIFY(statusPane->IsVisible());

    widget.showFullScreen();
    widget.showMaximized();
    QCOMPARE(widget.geometry(), maximumScreenGeometry);
    QVERIFY(buttonGroup->IsVisible());
    QVERIFY(statusPane->IsVisible());

    widget.showMinimized();
    widget.showMaximized();
    QCOMPARE(widget.geometry(), maximumScreenGeometry);
    QVERIFY(buttonGroup->IsVisible());
    QVERIFY(statusPane->IsVisible());

    //Exit
    widget.showMaximized();
    widget.showNormal();
    QCOMPARE(widget.geometry(), normalGeometry);
    QVERIFY(buttonGroup->IsVisible());
    QVERIFY(statusPane->IsVisible());

    widget.showMaximized();
    widget.showFullScreen();
    QCOMPARE(widget.geometry(), fullScreenGeometry);
    QVERIFY(!buttonGroup->IsVisible());
    QVERIFY(!statusPane->IsVisible());

    widget.showMaximized();
    widget.showMinimized();
    // Since showMinimized hides window decoration availableGeometry gives different value
    // than with decoration visible. Altual size does not really matter since widget is invisible.
    QCOMPARE(widget.geometry(), qApp->desktop()->availableGeometry(&widget));
    QVERIFY(!buttonGroup->IsVisible());
    QVERIFY(!statusPane->IsVisible());
}

void tst_QWidget::minimizedWindowModeTransitions()
{
    QWidget widget;
    QVBoxLayout *layout = new QVBoxLayout;
    QPushButton *button = new QPushButton("test Button");
    layout->addWidget(button);
    widget.setLayout(layout);
    widget.show();

    const QRect normalGeometry = widget.normalGeometry();
    const QRect fullScreenGeometry = qApp->desktop()->screenGeometry(&widget);
    const QRect maximumScreenGeometry = qApp->desktop()->availableGeometry(&widget);
    CEikStatusPane *statusPane = CEikonEnv::Static()->AppUiFactory()->StatusPane();
    CEikButtonGroupContainer *buttonGroup = CEikonEnv::Static()->AppUiFactory()->Cba();

    //Enter
    widget.showNormal();
    widget.showMinimized();
    QCOMPARE(widget.geometry(), normalGeometry);
    QVERIFY(!buttonGroup->IsVisible());
    QVERIFY(!statusPane->IsVisible());

    widget.showFullScreen();
    widget.showMinimized();
    QCOMPARE(widget.geometry(), fullScreenGeometry);
    QVERIFY(!buttonGroup->IsVisible());
    QVERIFY(!statusPane->IsVisible());

    widget.showMaximized();
    widget.showMinimized();
    // Since showMinimized hides window decoration availableGeometry gives different value
    // than with decoration visible. Altual size does not really matter since widget is invisible.
    QCOMPARE(widget.geometry(), qApp->desktop()->availableGeometry(&widget));
    QVERIFY(!buttonGroup->IsVisible());
    QVERIFY(!statusPane->IsVisible());

    //Exit
    widget.showMinimized();
    widget.showNormal();
    QCOMPARE(widget.geometry(), normalGeometry);
    QVERIFY(buttonGroup->IsVisible());
    QVERIFY(statusPane->IsVisible());

    widget.showMinimized();
    widget.showFullScreen();
    QCOMPARE(widget.geometry(), fullScreenGeometry);
    QVERIFY(!buttonGroup->IsVisible());
    QVERIFY(!statusPane->IsVisible());

    widget.showMinimized();
    widget.showMaximized();
    QCOMPARE(widget.geometry(), maximumScreenGeometry);
    QVERIFY(buttonGroup->IsVisible());
    QVERIFY(statusPane->IsVisible());
}

void tst_QWidget::normalWindowModeTransitions()
{
    QWidget widget;
    QVBoxLayout *layout = new QVBoxLayout;
    QPushButton *button = new QPushButton("test Button");
    layout->addWidget(button);
    widget.setLayout(layout);
    widget.show();

    const QRect normalGeometry = widget.normalGeometry();
    const QRect fullScreenGeometry = qApp->desktop()->screenGeometry(&widget);
    const QRect maximumScreenGeometry = qApp->desktop()->availableGeometry(&widget);
    CEikStatusPane *statusPane = CEikonEnv::Static()->AppUiFactory()->StatusPane();
    CEikButtonGroupContainer *buttonGroup = CEikonEnv::Static()->AppUiFactory()->Cba();

    //Enter
    widget.showMaximized();
    widget.showNormal();
    QCOMPARE(widget.geometry(), normalGeometry);
    QVERIFY(buttonGroup->IsVisible());
    QVERIFY(statusPane->IsVisible());

    widget.showFullScreen();
    widget.showNormal();
    QCOMPARE(widget.geometry(), normalGeometry);
    QVERIFY(buttonGroup->IsVisible());
    QVERIFY(statusPane->IsVisible());

    widget.showMinimized();
    widget.showNormal();
    QCOMPARE(widget.geometry(), normalGeometry);
    QVERIFY(buttonGroup->IsVisible());
    QVERIFY(statusPane->IsVisible());

    //Exit
    widget.showNormal();
    widget.showMaximized();
    QCOMPARE(widget.geometry(), maximumScreenGeometry);
    QVERIFY(buttonGroup->IsVisible());
    QVERIFY(statusPane->IsVisible());

    widget.showNormal();
    widget.showFullScreen();
    QCOMPARE(widget.geometry(), fullScreenGeometry);
    QVERIFY(!buttonGroup->IsVisible());
    QVERIFY(!statusPane->IsVisible());

    widget.showNormal();
    widget.showMinimized();
    QCOMPARE(widget.geometry(), normalGeometry);
    QVERIFY(!buttonGroup->IsVisible());
    QVERIFY(!statusPane->IsVisible());
}
#endif

class InputContextTester : public QInputContext
{
    Q_OBJECT
public:
    QString identifierName() { return QString(); }
    bool isComposing() const { return false; }
    QString language() { return QString(); }
    void reset() { ++resets; }
    int resets;
};

void tst_QWidget::focusProxyAndInputMethods()
{
    InputContextTester *inputContext = new InputContextTester;
    QWidget *toplevel = new QWidget(0, Qt::X11BypassWindowManagerHint);
    toplevel->setAttribute(Qt::WA_InputMethodEnabled, true);
    toplevel->setInputContext(inputContext); // ownership is transferred

    QWidget *child = new QWidget(toplevel);
    child->setFocusProxy(toplevel);
    child->setAttribute(Qt::WA_InputMethodEnabled, true);

    toplevel->setFocusPolicy(Qt::WheelFocus);
    child->setFocusPolicy(Qt::WheelFocus);

    QVERIFY(!child->hasFocus());
    QVERIFY(!toplevel->hasFocus());

    toplevel->show();
    QTest::qWaitForWindowShown(toplevel);
    QApplication::setActiveWindow(toplevel);
    QVERIFY(toplevel->hasFocus());
    QVERIFY(child->hasFocus());

    // verify that toggling input methods on the child widget
    // correctly propagate to the focus proxy's input method
    // and that the input method gets the focus proxy passed
    // as the focus widget instead of the child widget.
    // otherwise input method queries go to the wrong widget

    QCOMPARE(inputContext->focusWidget(), toplevel);

    child->setAttribute(Qt::WA_InputMethodEnabled, false);
    QVERIFY(!inputContext->focusWidget());

    child->setAttribute(Qt::WA_InputMethodEnabled, true);
    QCOMPARE(inputContext->focusWidget(), toplevel);

    child->setEnabled(false);
    QVERIFY(!inputContext->focusWidget());

    child->setEnabled(true);
    QCOMPARE(inputContext->focusWidget(), toplevel);

    delete toplevel;
}

#ifdef QT_BUILD_INTERNAL
class scrollWidgetWBS : public QWidget
{
public:
    void deleteBackingStore()
    {
        static_cast<QWidgetPrivate*>(d_ptr.data())->topData()->backingStore.destroy();
    }
    void enableBackingStore()
    {
        if (!static_cast<QWidgetPrivate*>(d_ptr.data())->maybeBackingStore()) {
            static_cast<QWidgetPrivate*>(d_ptr.data())->topData()->backingStore.create(this);
            static_cast<QWidgetPrivate*>(d_ptr.data())->invalidateBuffer(this->rect());
            repaint();
        }
    }
};
#endif

void tst_QWidget::scrollWithoutBackingStore()
{
#ifdef QT_BUILD_INTERNAL
    scrollWidgetWBS scrollable;
    scrollable.resize(100,100);
    QLabel child(QString("@"),&scrollable);
    child.resize(50,50);
    scrollable.show();
    QTest::qWaitForWindowShown(&scrollable);
    scrollable.scroll(50,50);
    QCOMPARE(child.pos(),QPoint(50,50));
    scrollable.deleteBackingStore();
    scrollable.scroll(-25,-25);
    QCOMPARE(child.pos(),QPoint(25,25));
    scrollable.enableBackingStore();
    QCOMPARE(child.pos(),QPoint(25,25));
#else
    QSKIP("Test case relies on developer build (AUTOTEST_EXPORT)", SkipAll);
#endif
}

void tst_QWidget::taskQTBUG_7532_tabOrderWithFocusProxy()
{
    QWidget w;
    w.setFocusPolicy(Qt::TabFocus);
    QWidget *fp = new QWidget(&w);
    fp->setFocusPolicy(Qt::TabFocus);
    w.setFocusProxy(fp);
    QWidget::setTabOrder(&w, fp);

    // No Q_ASSERT, then it's allright.
}

void tst_QWidget::movedAndResizedAttributes()
{
#if defined (Q_OS_MAC) || defined(Q_WS_QWS) || defined(Q_OS_SYMBIAN)
    QEXPECT_FAIL("", "FixMe, QTBUG-8941 and QTBUG-8977", Abort);
    QVERIFY(false);
#else
    QWidget w;
    w.show();

    QVERIFY(!w.testAttribute(Qt::WA_Moved));
    QVERIFY(!w.testAttribute(Qt::WA_Resized));

    w.setWindowState(Qt::WindowFullScreen);

    QVERIFY(!w.testAttribute(Qt::WA_Moved));
    QVERIFY(!w.testAttribute(Qt::WA_Resized));

    w.setWindowState(Qt::WindowMaximized);

    QVERIFY(!w.testAttribute(Qt::WA_Moved));
    QVERIFY(!w.testAttribute(Qt::WA_Resized));

    w.setWindowState(Qt::WindowMinimized);

    QVERIFY(!w.testAttribute(Qt::WA_Moved));
    QVERIFY(!w.testAttribute(Qt::WA_Resized));

    w.showNormal();

    QVERIFY(!w.testAttribute(Qt::WA_Moved));
    QVERIFY(!w.testAttribute(Qt::WA_Resized));

    w.showMaximized();

    QVERIFY(!w.testAttribute(Qt::WA_Moved));
    QVERIFY(!w.testAttribute(Qt::WA_Resized));

    w.showFullScreen();

    QVERIFY(!w.testAttribute(Qt::WA_Moved));
    QVERIFY(!w.testAttribute(Qt::WA_Resized));

    w.showNormal();
    w.move(10,10);
    QVERIFY(w.testAttribute(Qt::WA_Moved));
#if defined(Q_OS_WIN)
    QEXPECT_FAIL("", "FixMe, QTBUG-8911", Abort);
#endif
    QVERIFY(!w.testAttribute(Qt::WA_Resized));

    w.resize(100, 100);
    QVERIFY(w.testAttribute(Qt::WA_Moved));
    QVERIFY(w.testAttribute(Qt::WA_Resized));
#endif
}

void tst_QWidget::childAt()
{
    QWidget parent(0, Qt::FramelessWindowHint);
    parent.resize(200, 200);

    QWidget *child = new QWidget(&parent);
    child->setPalette(Qt::red);
    child->setAutoFillBackground(true);
    child->setGeometry(20, 20, 160, 160);

    QWidget *grandChild = new QWidget(child);
    grandChild->setPalette(Qt::blue);
    grandChild->setAutoFillBackground(true);
    grandChild->setGeometry(-20, -20, 220, 220);

    QVERIFY(!parent.childAt(19, 19));
    QVERIFY(!parent.childAt(180, 180));
    QCOMPARE(parent.childAt(20, 20), grandChild);
    QCOMPARE(parent.childAt(179, 179), grandChild);

    grandChild->setAttribute(Qt::WA_TransparentForMouseEvents);
    QCOMPARE(parent.childAt(20, 20), child);
    QCOMPARE(parent.childAt(179, 179), child);
    grandChild->setAttribute(Qt::WA_TransparentForMouseEvents, false);

    child->setMask(QRect(50, 50, 60, 60));

    QVERIFY(!parent.childAt(69, 69));
    QVERIFY(!parent.childAt(130, 130));
    QCOMPARE(parent.childAt(70, 70), grandChild);
    QCOMPARE(parent.childAt(129, 129), grandChild);

    child->setAttribute(Qt::WA_MouseNoMask);
    QCOMPARE(parent.childAt(69, 69), grandChild);
    QCOMPARE(parent.childAt(130, 130), grandChild);
    child->setAttribute(Qt::WA_MouseNoMask, false);

    grandChild->setAttribute(Qt::WA_TransparentForMouseEvents);
    QCOMPARE(parent.childAt(70, 70), child);
    QCOMPARE(parent.childAt(129, 129), child);
    grandChild->setAttribute(Qt::WA_TransparentForMouseEvents, false);

    grandChild->setMask(QRect(80, 80, 40, 40));

    QCOMPARE(parent.childAt(79, 79), child);
    QCOMPARE(parent.childAt(120, 120), child);
    QCOMPARE(parent.childAt(80, 80), grandChild);
    QCOMPARE(parent.childAt(119, 119), grandChild);

    grandChild->setAttribute(Qt::WA_MouseNoMask);

    QCOMPARE(parent.childAt(79, 79), grandChild);
    QCOMPARE(parent.childAt(120, 120), grandChild);
}

#ifdef Q_WS_MAC
void tst_QWidget::childAt_unifiedToolBar()
{
    QLabel *label = new QLabel(QLatin1String("foo"));
    QToolBar *toolBar = new QToolBar;
    toolBar->addWidget(new QLabel("dummy"));
    toolBar->addWidget(label);

    QMainWindow mainWindow;
    mainWindow.addToolBar(toolBar);
    mainWindow.show();

    // Calculate the top-left corner of the tool bar and the label (in mainWindow's coordinates).
    QPoint labelTopLeft = label->mapTo(&mainWindow, QPoint());
    QPoint toolBarTopLeft = toolBar->mapTo(&mainWindow, QPoint());

    QCOMPARE(mainWindow.childAt(toolBarTopLeft), static_cast<QWidget *>(toolBar));
    QCOMPARE(mainWindow.childAt(labelTopLeft), static_cast<QWidget *>(label));

    // Enable unified tool bars.
    mainWindow.setUnifiedTitleAndToolBarOnMac(true);
    QTest::qWait(50);

    // The tool bar is now in the "non-client" area of QMainWindow, i.e.
    // outside the mainWindow's rect(), and since mapTo et al. doesn't work
    // in that case (see commit 35667fd45ada49269a5987c235fdedfc43e92bb8),
    // we use mapToGlobal/mapFromGlobal to re-calculate the corners.
    QPoint oldToolBarTopLeft = toolBarTopLeft;
    toolBarTopLeft = mainWindow.mapFromGlobal(toolBar->mapToGlobal(QPoint()));
    QVERIFY(toolBarTopLeft != oldToolBarTopLeft);
    QVERIFY(toolBarTopLeft.y() < 0);
    labelTopLeft = mainWindow.mapFromGlobal(label->mapToGlobal(QPoint()));

    QCOMPARE(mainWindow.childAt(toolBarTopLeft), static_cast<QWidget *>(toolBar));
    QCOMPARE(mainWindow.childAt(labelTopLeft), static_cast<QWidget *>(label));
}

#ifdef QT_MAC_USE_COCOA
void tst_QWidget::taskQTBUG_11373()
{
    QMainWindow * myWindow = new QMainWindow();
    QWidget * center = new QWidget();
    myWindow -> setCentralWidget(center);
    QWidget * drawer = new QWidget(myWindow, Qt::Drawer);
    drawer -> hide();
    QCOMPARE(drawer->isVisible(), false);
    myWindow -> show();
    myWindow -> raise();
    // The drawer shouldn't be visible now.
    QCOMPARE(drawer->isVisible(), false);
    myWindow -> setWindowState(Qt::WindowFullScreen);
    myWindow -> setWindowState(Qt::WindowNoState);
    // The drawer should still not be visible, since we haven't shown it.
    QCOMPARE(drawer->isVisible(), false);
}
#endif // QT_MAC_USE_COCOA
#endif

QTEST_MAIN(tst_QWidget)
#include "tst_qwidget.moc"
