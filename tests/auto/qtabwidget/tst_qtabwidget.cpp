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
#include <qtabwidget.h>
#include <qdebug.h>
#include <qapplication.h>
#include <qlabel.h>

//TESTED_CLASS=
//TESTED_FILES=

#if defined(Q_OS_WIN) && !defined(Q_OS_WINCE)
#  include <qt_windows.h>
#define Q_CHECK_PAINTEVENTS \
    if (::SwitchDesktop(::GetThreadDesktop(::GetCurrentThreadId())) == 0) \
        QSKIP("desktop is not visible, this test would fail", SkipSingle);
#else
#define Q_CHECK_PAINTEVENTS
#endif
#if defined(Q_WS_X11)
#  include <private/qt_x11_p.h>
#  include <qx11info_x11.h>
#elif defined(Q_WS_QWS)
# include <qwindowsystem_qws.h>
#endif

#ifndef Q_CHECK_PAINTEVENTS
#define Q_CHECK_PAINTEVENTS
#endif //Q_CHECK_PAINTEVENTS


class QTabWidgetChild:public QTabWidget {
  public:
    QTabWidgetChild():tabCount(0) {
        QVERIFY(tabBar() != NULL);
        QWidget *w = new QWidget;
        int index = addTab(w, "test");
          QCOMPARE(tabCount, 1);
          removeTab(index);
          QCOMPARE(tabCount, 0);

          // Test bad arguments
          // This will assert, so don't do it :)
          //setTabBar(NULL);
    };

  protected:
    virtual void tabInserted(int /*index */ ) {
        tabCount++;
    };
    virtual void tabRemoved(int /*index */ ) {
        tabCount--;
    };
    int tabCount;
};

class tst_QTabWidget:public QObject {
  Q_OBJECT
  public:
    tst_QTabWidget();

  public slots:
    void init();
    void cleanup();
  private slots:
    void getSetCheck();
    void testChild();
    void addRemoveTab();
    void tabPosition();
    void tabEnabled();
    void tabText();
    void tabShape();
    void tabTooltip();
    void tabIcon();
    void indexOf();
    void currentWidget();
    void currentIndex();
    void cornerWidget();
    void removeTab();
    void clear();
    void keyboardNavigation();
    void paintEventCount();

  private:
    int addPage();
    void removePage(int index);
    QTabWidget *tw;
};

// Testing get/set functions
void tst_QTabWidget::getSetCheck()
{
    QTabWidget obj1;
    QWidget *w1 = new QWidget;
    QWidget *w2 = new QWidget;
    QWidget *w3 = new QWidget;
    QWidget *w4 = new QWidget;
    QWidget *w5 = new QWidget;

    obj1.addTab(w1, "Page 1");
    obj1.addTab(w2, "Page 2");
    obj1.addTab(w3, "Page 3");
    obj1.addTab(w4, "Page 4");
    obj1.addTab(w5, "Page 5");

    // TabShape QTabWidget::tabShape()
    // void QTabWidget::setTabShape(TabShape)
    obj1.setTabShape(QTabWidget::TabShape(QTabWidget::Rounded));
    QCOMPARE(QTabWidget::TabShape(QTabWidget::Rounded), obj1.tabShape());
    obj1.setTabShape(QTabWidget::TabShape(QTabWidget::Triangular));
    QCOMPARE(QTabWidget::TabShape(QTabWidget::Triangular), obj1.tabShape());

    // int QTabWidget::currentIndex()
    // void QTabWidget::setCurrentIndex(int)
    obj1.setCurrentIndex(0);
    QCOMPARE(0, obj1.currentIndex());
    obj1.setCurrentIndex(INT_MIN);
    QCOMPARE(0, obj1.currentIndex());
    obj1.setCurrentIndex(INT_MAX);
    QCOMPARE(0, obj1.currentIndex());
    obj1.setCurrentIndex(4);
    QCOMPARE(4, obj1.currentIndex());

    // QWidget * QTabWidget::currentWidget()
    // void QTabWidget::setCurrentWidget(QWidget *)
    obj1.setCurrentWidget(w1);
    QCOMPARE(w1, obj1.currentWidget());
    obj1.setCurrentWidget(w5);
    QCOMPARE(w5, obj1.currentWidget());
    obj1.setCurrentWidget((QWidget *)0);
    QCOMPARE(w5, obj1.currentWidget()); // current not changed
}

tst_QTabWidget::tst_QTabWidget()
{
}

void tst_QTabWidget::init()
{
    tw = new QTabWidget(0);
    QCOMPARE(tw->count(), 0);
    QCOMPARE(tw->currentIndex(), -1);
    QVERIFY(tw->currentWidget() == NULL);
}

void tst_QTabWidget::cleanup()
{
    delete tw;
    tw = 0;
}

void tst_QTabWidget::testChild()
{
    QTabWidgetChild t;
}

#define LABEL "TEST"
#define TIP "TIP"
int tst_QTabWidget::addPage()
{
    QWidget *w = new QWidget();
    return tw->addTab(w, LABEL);
}

void tst_QTabWidget::removePage(int index)
{
    QWidget *w = tw->widget(index);
    tw->removeTab(index);
    delete w;
}

/**
 * Tests:
 * addTab(...) which really calls -> insertTab(...)
 * widget(...)
 * removeTab(...);
 * If this fails then many others probably will too.
 */
void tst_QTabWidget::addRemoveTab()
{
    // Test bad arguments
    tw->addTab(NULL, LABEL);
    QCOMPARE(tw->count(), 0);
    tw->removeTab(-1);
    QCOMPARE(tw->count(), 0);
    QVERIFY(tw->widget(-1) == 0);

    QWidget *w = new QWidget();
    int index = tw->addTab(w, LABEL);
    // return value
    QCOMPARE(tw->indexOf(w), index);

    QCOMPARE(tw->count(), 1);
    QVERIFY(tw->widget(index) == w);
    QCOMPARE(tw->tabText(index), QString(LABEL));

    removePage(index);
    QCOMPARE(tw->count(), 0);
}

void tst_QTabWidget::tabPosition()
{
    tw->setTabPosition(QTabWidget::North);
    QCOMPARE(tw->tabPosition(), QTabWidget::North);
    tw->setTabPosition(QTabWidget::South);
    QCOMPARE(tw->tabPosition(), QTabWidget::South);
    tw->setTabPosition(QTabWidget::East);
    QCOMPARE(tw->tabPosition(), QTabWidget::East);
    tw->setTabPosition(QTabWidget::West);
    QCOMPARE(tw->tabPosition(), QTabWidget::West);
}

void tst_QTabWidget::tabEnabled()
{
    // Test bad arguments
    QVERIFY(tw->isTabEnabled(-1) == false);
    tw->setTabEnabled(-1, false);

    int index = addPage();

    tw->setTabEnabled(index, true);
    QVERIFY(tw->isTabEnabled(index));
    QVERIFY(tw->widget(index)->isEnabled());
    tw->setTabEnabled(index, false);
    QVERIFY(!tw->isTabEnabled(index));
    QVERIFY(!tw->widget(index)->isEnabled());
    tw->setTabEnabled(index, true);
    QVERIFY(tw->isTabEnabled(index));
    QVERIFY(tw->widget(index)->isEnabled());

    removePage(index);
}

void tst_QTabWidget::tabText()
{
    // Test bad arguments
    QCOMPARE(tw->tabText(-1), QString(""));
    tw->setTabText(-1, LABEL);

    int index = addPage();

    tw->setTabText(index, "new");
    QCOMPARE(tw->tabText(index), QString("new"));
    tw->setTabText(index, LABEL);
    QCOMPARE(tw->tabText(index), QString(LABEL));

    removePage(index);
}

void tst_QTabWidget::tabShape()
{
    int index = addPage();

    tw->setTabShape(QTabWidget::Rounded);
    QCOMPARE(tw->tabShape(), QTabWidget::Rounded);
    tw->setTabShape(QTabWidget::Triangular);
    QCOMPARE(tw->tabShape(), QTabWidget::Triangular);
    tw->setTabShape(QTabWidget::Rounded);
    QCOMPARE(tw->tabShape(), QTabWidget::Rounded);

    removePage(index);
}

void tst_QTabWidget::tabTooltip()
{
    // Test bad arguments
    QCOMPARE(tw->tabToolTip(-1), QString(""));
    tw->setTabText(-1, TIP);

    int index = addPage();

    tw->setTabToolTip(index, "tip");
    QCOMPARE(tw->tabToolTip(index), QString("tip"));
    tw->setTabToolTip(index, TIP);
    QCOMPARE(tw->tabToolTip(index), QString(TIP));

    removePage(index);
}

void tst_QTabWidget::tabIcon()
{
    // Test bad arguments
    QVERIFY(tw->tabToolTip(-1).isNull());
    tw->setTabIcon(-1, QIcon());

    int index = addPage();

    QIcon icon;
    tw->setTabIcon(index, icon);
    QVERIFY(tw->tabIcon(index).isNull());

    removePage(index);
}

void tst_QTabWidget::indexOf()
{
    // Test bad arguments
    QCOMPARE(tw->indexOf(NULL), -1);

    int index = addPage();
    QWidget *w = tw->widget(index);
    QCOMPARE(tw->indexOf(w), index);

    removePage(index);
}

void tst_QTabWidget::currentWidget()
{
    // Test bad arguments
    tw->setCurrentWidget(NULL);
    QVERIFY(tw->currentWidget() == NULL);

    int index = addPage();
    QWidget *w = tw->widget(index);
    QVERIFY(tw->currentWidget() == w);
    QCOMPARE(tw->currentIndex(), index);

    tw->setCurrentWidget(NULL);
    QVERIFY(tw->currentWidget() == w);
    QCOMPARE(tw->currentIndex(), index);

    int index2 = addPage();
    QWidget *w2 = tw->widget(index2);
    Q_UNUSED(w2);
    QVERIFY(tw->currentWidget() == w);
    QCOMPARE(tw->currentIndex(), index);

    removePage(index2);
    removePage(index);
}

/**
 * setCurrentWidget(..) calls setCurrentIndex(..)
 * currentChanged(..) SIGNAL
 */
void tst_QTabWidget::currentIndex()
{
    // Test bad arguments
    QSignalSpy spy(tw, SIGNAL(currentChanged(int)));
#ifdef QT3_SUPPORT
    QSignalSpy spySupport(tw, SIGNAL(currentChanged(QWidget *)));
#endif
    QCOMPARE(tw->currentIndex(), -1);
    tw->setCurrentIndex(-1);
    QCOMPARE(tw->currentIndex(), -1);
    QCOMPARE(spy.count(), 0);

    int firstIndex = addPage();
    tw->setCurrentIndex(firstIndex);
    QCOMPARE(tw->currentIndex(), firstIndex);
    QCOMPARE(spy.count(), 1);
    QList<QVariant> arguments = spy.takeFirst();
    QVERIFY(arguments.at(0).toInt() == firstIndex);
#ifdef QT3_SUPPORT
    QCOMPARE(spySupport.count(), 1);
#endif
    
    int index = addPage();
    QCOMPARE(tw->currentIndex(), firstIndex);
    tw->setCurrentIndex(index);
    QCOMPARE(tw->currentIndex(), index);
    QCOMPARE(spy.count(), 1);
    arguments = spy.takeFirst();
    QVERIFY(arguments.at(0).toInt() == index);
#ifdef QT3_SUPPORT
    QCOMPARE(spySupport.count(), 2);
#endif
    
    removePage(index);
    QCOMPARE(tw->currentIndex(), firstIndex);
    QCOMPARE(spy.count(), 1);
    arguments = spy.takeFirst();
    QVERIFY(arguments.at(0).toInt() == firstIndex);
#ifdef QT3_SUPPORT
    QCOMPARE(spySupport.count(), 3);
#endif

    removePage(firstIndex);
    QCOMPARE(tw->currentIndex(), -1);
    QCOMPARE(spy.count(), 1);
    arguments = spy.takeFirst();
    QVERIFY(arguments.at(0).toInt() == -1);
#ifdef QT3_SUPPORT
    QCOMPARE(spySupport.count(), 4);
#endif

}

void tst_QTabWidget::cornerWidget()
{
    // Test bad arguments
    tw->setCornerWidget(NULL, Qt::TopRightCorner);

    QVERIFY(tw->cornerWidget(Qt::TopLeftCorner) == 0);
    QVERIFY(tw->cornerWidget(Qt::TopRightCorner) == 0);
    QVERIFY(tw->cornerWidget(Qt::BottomLeftCorner) == 0);
    QVERIFY(tw->cornerWidget(Qt::BottomRightCorner) == 0);

    QWidget *w = new QWidget(0);
    tw->setCornerWidget(w, Qt::TopLeftCorner);
    QCOMPARE(w->parent(), (QObject *)tw);
    QVERIFY(tw->cornerWidget(Qt::TopLeftCorner) == w);
    tw->setCornerWidget(w, Qt::TopRightCorner);
    QVERIFY(tw->cornerWidget(Qt::TopRightCorner) == w);
    tw->setCornerWidget(w, Qt::BottomLeftCorner);
    QVERIFY(tw->cornerWidget(Qt::BottomLeftCorner) == w);
    tw->setCornerWidget(w, Qt::BottomRightCorner);
    QVERIFY(tw->cornerWidget(Qt::BottomRightCorner) == w);

    tw->setCornerWidget(0, Qt::TopRightCorner);
    QVERIFY(tw->cornerWidget(Qt::TopRightCorner) == 0);
    QCOMPARE(w->isHidden(), true);
}

//test that the QTabWidget::count() is correct at the moment the currentChanged signal is emit
class RemoveTabObject : public QObject
{
    Q_OBJECT
    public:
        RemoveTabObject(QTabWidget *_tw) : tw(_tw), count(-1) {
            connect(tw, SIGNAL(currentChanged(int)), this, SLOT(currentChanged()));
        }

        QTabWidget *tw;
        int count;
    public slots:
        void currentChanged() { count = tw->count(); }
};

void tst_QTabWidget::removeTab()
{
    tw->show();
    QCOMPARE(tw->count(), 0);
    RemoveTabObject ob(tw);
    tw->addTab(new QLabel("1"), "1");
    QCOMPARE(ob.count, 1);
    tw->addTab(new QLabel("2"), "2");
    tw->addTab(new QLabel("3"), "3");
    tw->addTab(new QLabel("4"), "4");
    tw->addTab(new QLabel("5"), "5");
    QCOMPARE(ob.count, 1);
    QCOMPARE(tw->count(), 5);
    
    tw->setCurrentIndex(4);
    QCOMPARE(ob.count,5);
    tw->removeTab(4);
    QCOMPARE(ob.count, 4);
    QCOMPARE(tw->count(), 4);
    QCOMPARE(tw->currentIndex(), 3);

    tw->setCurrentIndex(1);
    tw->removeTab(1);
    QCOMPARE(ob.count, 3);
    QCOMPARE(tw->count(), 3);
    QCOMPARE(tw->currentIndex(), 1);

    delete tw->widget(1);
    QCOMPARE(tw->count(), 2);
    QCOMPARE(ob.count, 2);
    QCOMPARE(tw->currentIndex(), 1);
    delete tw->widget(1);
    QCOMPARE(tw->count(), 1);
    QCOMPARE(ob.count, 1);
    tw->removeTab(0);
    QCOMPARE(tw->count(), 0);
    QCOMPARE(ob.count, 0);
}

void tst_QTabWidget::clear()
{
    tw->addTab(new QWidget, "1");
    tw->addTab(new QWidget, "2");
    tw->addTab(new QWidget, "3");
    tw->addTab(new QWidget, "4");
    tw->addTab(new QWidget, "5");
    tw->setCurrentIndex(4);
    tw->clear();
    QCOMPARE(tw->count(), 0);
    QCOMPARE(tw->currentIndex(), -1);
}

void tst_QTabWidget::keyboardNavigation()
{
    int firstIndex = addPage();
    addPage();
    addPage();
    tw->setCurrentIndex(firstIndex);
    QCOMPARE(tw->currentIndex(), firstIndex);

    QTest::keyClick(tw, Qt::Key_Tab, Qt::ControlModifier);
    QCOMPARE(tw->currentIndex(), 1);
    QTest::keyClick(tw, Qt::Key_Tab, Qt::ControlModifier);
    QCOMPARE(tw->currentIndex(), 2);
    QTest::keyClick(tw, Qt::Key_Tab, Qt::ControlModifier);
    QCOMPARE(tw->currentIndex(), 0);
    tw->setTabEnabled(1, false);
    QTest::keyClick(tw, Qt::Key_Tab, Qt::ControlModifier);
    QCOMPARE(tw->currentIndex(), 2);

    QTest::keyClick(tw, Qt::Key_Tab, Qt::ControlModifier | Qt::ShiftModifier);
    QCOMPARE(tw->currentIndex(), 0);
    QTest::keyClick(tw, Qt::Key_Tab, Qt::ControlModifier | Qt::ShiftModifier);
    QCOMPARE(tw->currentIndex(), 2);
    tw->setTabEnabled(1, true);
    QTest::keyClick(tw, Qt::Key_Tab, Qt::ControlModifier | Qt::ShiftModifier);
    QCOMPARE(tw->currentIndex(), 1);
    QTest::keyClick(tw, Qt::Key_Tab, Qt::ControlModifier | Qt::ShiftModifier);
    QCOMPARE(tw->currentIndex(), 0);
    QTest::keyClick(tw, Qt::Key_Tab, Qt::ControlModifier | Qt::ShiftModifier);
    QCOMPARE(tw->currentIndex(), 2);
    QTest::keyClick(tw, Qt::Key_Tab, Qt::ControlModifier);
    QCOMPARE(tw->currentIndex(), 0);

    // Disable all and try to go to the next. It should not move anywhere, and more importantly
    // it should not loop forever. (a naive "search for the first enabled tabbar") implementation
    // might do that)
    tw->setTabEnabled(0, false);
    tw->setTabEnabled(1, false);
    tw->setTabEnabled(2, false);
    QTest::keyClick(tw, Qt::Key_Tab, Qt::ControlModifier);
    // TODO: Disabling the current tab will move current tab to the next,
    // but what if next tab is also disabled. We should look into this. 
    QVERIFY(tw->currentIndex() < 3 && tw->currentIndex() >= 0);
}

class PaintCounter : public QWidget
{
public:
    PaintCounter() :count(0) { setAttribute(Qt::WA_OpaquePaintEvent); }
    int count;
protected:
    void paintEvent(QPaintEvent*) {
        ++count;
    }
};


void tst_QTabWidget::paintEventCount()
{
    Q_CHECK_PAINTEVENTS

    PaintCounter *tab1 = new PaintCounter;
    PaintCounter *tab2 = new PaintCounter;

    tw->addTab(tab1, "one");
    tw->addTab(tab2, "two");

    QCOMPARE(tab1->count, 0);
    QCOMPARE(tab2->count, 0);
    QCOMPARE(tw->currentIndex(), 0);

    tw->show();

    QTest::qWait(1000);

    // Mac, Windows and Windows CE get multiple repaints on the first show, so use those as a starting point.
    static const int MaxInitialPaintCount =
#if defined(Q_OS_WINCE)
        4;
#elif defined(Q_WS_WIN)
        2;
#elif defined(Q_WS_MAC)
        5;
#else
        2;
#endif
    QVERIFY(tab1->count <= MaxInitialPaintCount);
    QCOMPARE(tab2->count, 0);

    const int initalPaintCount = tab1->count;

    tw->setCurrentIndex(1);

    QTest::qWait(100);

    QCOMPARE(tab1->count, initalPaintCount);
    QCOMPARE(tab2->count, 1);

    tw->setCurrentIndex(0);

    QTest::qWait(100);

    QCOMPARE(tab1->count, initalPaintCount + 1);
    QCOMPARE(tab2->count, 1);
}


QTEST_MAIN(tst_QTabWidget)
#include "tst_qtabwidget.moc"
