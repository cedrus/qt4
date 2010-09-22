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
#include <qapplication.h>
#include <QPushButton>
#include <QMainWindow>
#include <QMenuBar>
#include <QToolBar>
#include <QToolButton>
#include <QStatusBar>
#include <QListWidget>
#include <QWidgetAction>
#include <QDesktopWidget>
#include <qdialog.h>

#include <qmenu.h>
#include <qstyle.h>
#include <qdebug.h>

#include "../../shared/util.h"

//TESTED_CLASS=
//TESTED_FILES=

class tst_QMenu : public QObject
{
    Q_OBJECT

public:
    tst_QMenu();
    virtual ~tst_QMenu();


public slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();
private slots:
    void getSetCheck();
    void addActionsAndClear();

    void keyboardNavigation_data();
    void keyboardNavigation();
    void focus();
    void overrideMenuAction();
    void statusTip();
    void widgetActionFocus();
    void mouseActivation();
    void tearOff();
    void layoutDirection();

#if defined(QT3_SUPPORT)
    void indexBasedInsertion_data();
    void indexBasedInsertion();
#endif
    void task208001_stylesheet();
    void activeSubMenuPosition();
    void task242454_sizeHint();
    void task176201_clear();
    void task250673_activeMultiColumnSubMenuPosition();
    void task256918_setFont();
    void menuSizeHint();
    void task258920_mouseBorder();
    void setFixedWidth();
    void deleteActionInTriggered();
    void pushButtonPopulateOnAboutToShow();
    void QTBUG7907_submenus_autoselect();
    void QTBUG7411_submenus_activate();
    void QTBUG_10735_crashWithDialog();
protected slots:
    void onActivated(QAction*);
    void onHighlighted(QAction*);
    void onStatusMessageChanged(const QString &);
    void onStatusTipTimer();
    void deleteAction(QAction *a) { delete a; }
    void populateMenu();
private:
    void createActions();
    QMenu *menus[2], *lastMenu;
    enum { num_builtins = 10 };
    QAction *activated, *highlighted, *builtins[num_builtins];
    QString statustip;
};

// Testing get/set functions
void tst_QMenu::getSetCheck()
{
    QMenu obj1;
    // QAction * QMenu::defaultAction()
    // void QMenu::setDefaultAction(QAction *)
    QAction *var1 = new QAction(0);
    obj1.setDefaultAction(var1);
    QCOMPARE(var1, obj1.defaultAction());
    obj1.setDefaultAction((QAction *)0);
    QCOMPARE((QAction *)0, obj1.defaultAction());
    delete var1;

    // QAction * QMenu::activeAction()
    // void QMenu::setActiveAction(QAction *)
    QAction *var2 = new QAction(0);
    obj1.setActiveAction(var2);
    QCOMPARE(var2, obj1.activeAction());
    obj1.setActiveAction((QAction *)0);
    QCOMPARE((QAction *)0, obj1.activeAction());
    delete var2;
}

tst_QMenu::tst_QMenu()
{
    QApplication::setEffectEnabled(Qt::UI_AnimateMenu, false);
}

tst_QMenu::~tst_QMenu()
{

}

void
tst_QMenu::initTestCase()
{
    for(int i = 0; i < num_builtins; i++)
        builtins[i] = 0;
    for(int i = 0; i < 2; i++) {
        menus[i] = new QMenu;
        QObject::connect(menus[i], SIGNAL(triggered(QAction*)), this, SLOT(onActivated(QAction*)));
        QObject::connect(menus[i], SIGNAL(hovered(QAction*)), this, SLOT(onHighlighted(QAction*)));
    }
}

void
tst_QMenu::cleanupTestCase()
{
    for(int i = 0; i < 2; i++)
        menus[i]->clear();
    for(int i = 0; i < num_builtins; i++) {
        bool menuAction = false;
        for (int j = 0; j < 2; ++j)
            if (menus[j]->menuAction() == builtins[i])
                menuAction = true;
        if (!menuAction)
            delete builtins[i];
    }
    delete menus[0];
    delete menus[1];
}

void
tst_QMenu::init()
{
    activated = highlighted = 0;
    lastMenu = 0;
}

void
tst_QMenu::cleanup()
{
}

void
tst_QMenu::createActions()
{
    if(!builtins[0])
        builtins[0] = new QAction("New", 0);
    menus[0]->addAction(builtins[0]);

    if(!builtins[1]) {
        builtins[1] = new QAction(0);
        builtins[1]->setSeparator(true);
    }
    menus[0]->addAction(builtins[1]);

    if(!builtins[2]) {
        builtins[2] = menus[1]->menuAction();
        builtins[2]->setText("&Open..");
        builtins[8] = new QAction("Close", 0);
        menus[1]->addAction(builtins[8]);
        builtins[9] = new QAction("Quit", 0);
        menus[1]->addAction(builtins[9]);
    }
    menus[0]->addAction(builtins[2]);

    if(!builtins[3])
        builtins[3] = new QAction("Open &as..", 0);
    menus[0]->addAction(builtins[3]);

    if(!builtins[4]) {
        builtins[4] = new QAction("Save", 0);
        builtins[4]->setEnabled(false);
    }
    menus[0]->addAction(builtins[4]);

    if(!builtins[5])
        builtins[5] = new QAction("Sa&ve as..", 0);
    menus[0]->addAction(builtins[5]);

    if(!builtins[6]) {
        builtins[6] = new QAction(0);
        builtins[6]->setSeparator(true);
    }
    menus[0]->addAction(builtins[6]);

    if(!builtins[7])
        builtins[7] = new QAction("Prin&t", 0);
    menus[0]->addAction(builtins[7]);
}

void
tst_QMenu::onHighlighted(QAction *action)
{
    highlighted = action;
    lastMenu = qobject_cast<QMenu*>(sender());
}

void
tst_QMenu::onActivated(QAction *action)
{
    activated = action;
    lastMenu = qobject_cast<QMenu*>(sender());
}

void tst_QMenu::onStatusMessageChanged(const QString &s)
{
    statustip=s;
}

void tst_QMenu::populateMenu(){
    //just adds 3 dummy actions and a separator.
        lastMenu->addAction("Foo");
        lastMenu->addAction("Bar");
        lastMenu->addAction("FooBar");
        lastMenu->addSeparator();

}


//actual tests
void
tst_QMenu::addActionsAndClear()
{
#ifdef QT_SOFTKEYS_ENABLED
    // Softkeys add extra "Select" and "Back" actions to menu by default.
    // Two first actions will be Select and Back when softkeys are enabled
    int numSoftkeyActions = 2;
#else
    int numSoftkeyActions = 0;
#endif

    QCOMPARE(menus[0]->actions().count(), 0 + numSoftkeyActions);
    createActions();
    QCOMPARE(menus[0]->actions().count(), 8 + numSoftkeyActions);
    menus[0]->clear();
    QCOMPARE(menus[0]->actions().count(), 0);
}

void tst_QMenu::mouseActivation()
{
#ifdef Q_OS_WINCE_WM
    QSKIP("We have a separate mouseActivation test for Windows mobile.", SkipAll);
#endif
    QMenu menu;
    menu.addAction("Menu Action");
    menu.show();
    QTest::mouseClick(&menu, Qt::LeftButton, 0, QPoint(5, 5), 300);
    QVERIFY(!menu.isVisible());

    //context menus can allways be accessed with right click except on windows
    menu.show();
    QTest::mouseClick(&menu, Qt::RightButton, 0, QPoint(5, 5), 300);
    QVERIFY(!menu.isVisible());

#ifdef Q_OS_WIN
    //on windows normal mainwindow menus Can only be accessed with left mouse button
    QMenuBar menubar;
    QMenu submenu("Menu");
    submenu.addAction("action");
    QAction *action = menubar.addMenu(&submenu);
    menubar.show();

    QTest::mouseClick(&menubar, Qt::LeftButton, 0, menubar.actionGeometry(action).center(), 300);
    QVERIFY(submenu.isVisible());
    QTest::mouseClick(&submenu, Qt::LeftButton, 0, QPoint(5, 5), 300);
    QVERIFY(!submenu.isVisible());

    QTest::mouseClick(&menubar, Qt::LeftButton, 0, menubar.actionGeometry(action).center(), 300);
    QVERIFY(submenu.isVisible());
    QTest::mouseClick(&submenu, Qt::RightButton, 0, QPoint(5, 5), 300);
    QVERIFY(submenu.isVisible());
#endif
}


void
tst_QMenu::keyboardNavigation_data()
{
    QTest::addColumn<int>("key");
    QTest::addColumn<int>("expected_action");
    QTest::addColumn<int>("expected_menu");
    QTest::addColumn<bool>("init");
    QTest::addColumn<bool>("expected_activated");
    QTest::addColumn<bool>("expected_highlighted");

    //test up and down (order is important here)
    QTest::newRow("data0") << int(Qt::Key_Down) << 0 << 0 << true << false << true;
    QTest::newRow("data1") << int(Qt::Key_Down) << 2 << 0 << false << false << true; //skips the separator
    QTest::newRow("data2") << int(Qt::Key_Down) << 3 << 0 << false << false << true;

    if (QApplication::style()->styleHint(QStyle::SH_Menu_AllowActiveAndDisabled))
        QTest::newRow("data3_noMac") << int(Qt::Key_Down) << 4 << 0 << false << false << true;
    else
        QTest::newRow("data3_Mac") << int(Qt::Key_Down) << 5 << 0 << false << false << true;
    QTest::newRow("data4") << int(Qt::Key_Up) << 3 << 0 << false << false << true;
    QTest::newRow("data5") << int(Qt::Key_Up) << 2 << 0 << false << false << true;
    QTest::newRow("data6") << int(Qt::Key_Right) << 8 << 1 << false << false << true;
    QTest::newRow("data7") << int(Qt::Key_Down) << 9 << 1 << false << false << true;
    QTest::newRow("data8") << int(Qt::Key_Escape) << 2 << 0 << false << false << false;
    QTest::newRow("data9") << int(Qt::Key_Down) << 3 << 0 << false << false<< true;
    QTest::newRow("data10") << int(Qt::Key_Return) << 3 << 0 << false << true << false;

    //test shortcuts
#if 0
    QTest::newRow("shortcut0") << (Qt::ALT | Qt::Key_A) << 2 << 0 << true << true << false;
#endif
}

void
tst_QMenu::keyboardNavigation()
{
    DEPENDS_ON( "addActionsAndClear" ); //if add/clear fails...
    QFETCH(int, key);
    QFETCH(int, expected_action);
    QFETCH(int, expected_menu);
    QFETCH(bool, init);
    QFETCH(bool, expected_activated);
    QFETCH(bool, expected_highlighted);

    if(init) {
        lastMenu = menus[0];
        lastMenu->clear();
        createActions();
        lastMenu->popup(QPoint(0, 0));
    }

    QTest::keyClick(lastMenu, (Qt::Key)key);
    if(expected_activated) {
        QCOMPARE(activated, builtins[expected_action]);
        QCOMPARE(menus[expected_menu]->activeAction(), (QAction *)0);
    } else {
        QCOMPARE(menus[expected_menu]->activeAction(), builtins[expected_action]);
        if(expected_highlighted)
            QCOMPARE(menus[expected_menu]->activeAction(), highlighted);
    }
}


#ifdef Q_WS_MAC
QT_BEGIN_NAMESPACE
    extern bool qt_tab_all_widgets; // qapplication_mac.cpp
QT_END_NAMESPACE
#endif

void tst_QMenu::focus()
{
    QMenu menu;
    menu.addAction("One");
    menu.addAction("Two");
    menu.addAction("Three");
    bool fullKeyboardControl = true;

#ifdef Q_WS_MAC
    fullKeyboardControl = qt_tab_all_widgets;
#endif

    if (!fullKeyboardControl)
        QSKIP("Computer is currently set up to NOT tab to all widgets,"
             " this test assumes you can tab to all widgets", SkipAll);

    QWidget window;
    QPushButton button("Push me", &window);
    window.show();
    qApp->setActiveWindow(&window);

    QVERIFY(button.hasFocus());
    QCOMPARE(QApplication::focusWidget(), (QWidget *)&button);
    QCOMPARE(QApplication::activeWindow(), &window);
    menu.show();
#if 0
    QVERIFY(!button.hasFocus());
    QCOMPARE(QApplication::focusWidget(), &menu);
    QCOMPARE(QApplication::activeWindow(), &window);
#else
    QVERIFY(button.hasFocus());
    QCOMPARE(QApplication::focusWidget(), (QWidget *)&button);
    QCOMPARE(QApplication::activeWindow(), &window);
#endif
    menu.hide();
    QVERIFY(button.hasFocus());
    QCOMPARE(QApplication::focusWidget(), (QWidget *)&button);
    QCOMPARE(QApplication::activeWindow(), &window);
}

void tst_QMenu::overrideMenuAction()
{
	//test the override menu action by first creating an action to which we set its menu
	QMainWindow w;

	QAction *aFileMenu = new QAction("&File", &w);
	w.menuBar()->addAction(aFileMenu);

	QMenu *m = new QMenu(&w);
	QAction *menuaction = m->menuAction();
	connect(m, SIGNAL(triggered(QAction*)), SLOT(onActivated(QAction*)));
	aFileMenu->setMenu(m); //this sets the override menu action for the QMenu
    QCOMPARE(m->menuAction(), aFileMenu);

#ifdef Q_WS_MAC
    QSKIP("On Mac, we need to create native key events to test menu action activation", SkipAll);
#elif defined(Q_OS_WINCE)
    QSKIP("On Windows CE, we need to create native key events to test menu action activation", SkipAll);
#elif defined(Q_OS_SYMBIAN)
    QSKIP("On Symbian OS, we need to create native key events to test menu action activation", SkipAll);
#endif

    QAction *aQuit = new QAction("Quit", &w);
	aQuit->setShortcut(QKeySequence("Ctrl+X"));
	m->addAction(aQuit);

	w.show();
    QApplication::setActiveWindow(&w);
    w.setFocus();
    QTest::qWaitForWindowShown(&w);
    QTRY_VERIFY(w.hasFocus());

	//test of the action inside the menu
	QTest::keyClick(&w, Qt::Key_X, Qt::ControlModifier);
    QTRY_COMPARE(activated, aQuit);

	//test if the menu still pops out
	QTest::keyClick(&w, Qt::Key_F, Qt::AltModifier);
    QTRY_VERIFY(m->isVisible());

	delete aFileMenu;

	//after the deletion of the override menu action,
	//the menu should have its default menu action back
	QCOMPARE(m->menuAction(), menuaction);

}

void tst_QMenu::statusTip()
{
    //check that the statustip of actions inserted into the menu are displayed
    QMainWindow w;
    connect(w.statusBar(), SIGNAL(messageChanged(const QString &)), SLOT(onStatusMessageChanged(const QString &)));; //creates the status bar
    QToolBar tb;
    QAction a("main action", &tb);
    a.setStatusTip("main action");
    QMenu m(&tb);
    QAction subact("sub action", &m);
    subact.setStatusTip("sub action");
    m.addAction(&subact);
    a.setMenu(&m);
    tb.addAction(&a);

    w.addToolBar(&tb);
    w.show();

    QRect rect1 = tb.actionGeometry(&a);
    QToolButton *btn = qobject_cast<QToolButton*>(tb.childAt(rect1.center()));

    QVERIFY(btn != NULL);

    //because showMenu calls QMenu::exec, we need to use a singleshot
    //to continue the test
    QTimer::singleShot(200,this, SLOT(onStatusTipTimer()));
    btn->showMenu();
    QVERIFY(statustip.isEmpty());
}

//2nd part of the test
void tst_QMenu::onStatusTipTimer()
{
    QMenu *menu = qobject_cast<QMenu*>(QApplication::activePopupWidget());
    QVERIFY(menu != 0);
    QVERIFY(menu->isVisible());
    QTest::keyClick(menu, Qt::Key_Down);

    //we store the statustip to press escape in any case
    //otherwise, if the test fails it blocks (never gets out of QMenu::exec
    const QString st=statustip;

    menu->close(); //goes out of the menu

    QCOMPARE(st, QString("sub action"));
    QVERIFY(menu->isVisible() == false);
}

void tst_QMenu::widgetActionFocus()
{
    //test if the focus is correctly handled with a QWidgetAction
    QMenu m;
    QListWidget *l = new QListWidget(&m);
    for(int i = 1; i<3 ; i++)
        l->addItem(QString("item%1").arg(i));
    QWidgetAction *wa = new QWidgetAction(&m);
    wa->setDefaultWidget(l);
    m.addAction(wa);
    m.setActiveAction(wa);
    l->setFocus(); //to ensure it has primarily the focus
    QAction *menuitem1=m.addAction("menuitem1");
    QAction *menuitem2=m.addAction("menuitem2");

    m.popup(QPoint());

    QVERIFY(m.isVisible());
    QVERIFY(l->hasFocus());
    QVERIFY(l->currentItem());
    QCOMPARE(l->currentItem()->text(), QString("item1"));

    QTest::keyClick(QApplication::focusWidget(), Qt::Key_Down);
    QVERIFY(l->currentItem());
    QCOMPARE(l->currentItem()->text(), QString("item2"));

    QTest::keyClick(QApplication::focusWidget(), Qt::Key_Down);
    QVERIFY(m.hasFocus());
    QCOMPARE(m.activeAction(), menuitem1);

    QTest::keyClick(QApplication::focusWidget(), Qt::Key_Down);
    QVERIFY(m.hasFocus());
    QCOMPARE(m.activeAction(), menuitem2);

    QTest::keyClick(QApplication::focusWidget(), Qt::Key_Up);
    QVERIFY(m.hasFocus());
    QCOMPARE(m.activeAction(), menuitem1);

    QTest::keyClick(QApplication::focusWidget(), Qt::Key_Up);
    QVERIFY(l->hasFocus());
    QCOMPARE(m.activeAction(), (QAction *)wa);
}

void tst_QMenu::tearOff()
{
    QWidget widget;
    QMenu *menu = new QMenu(&widget);
    QVERIFY(!menu->isTearOffEnabled()); //default value
    menu->setTearOffEnabled(true);
    menu->addAction("aaa");
    menu->addAction("bbb");
    QVERIFY(menu->isTearOffEnabled());

    widget.show();
    menu->popup(QPoint(0,0));
    QTest::qWait(50);
    QVERIFY(!menu->isTearOffMenuVisible());

    QTest::mouseClick(menu, Qt::LeftButton, 0, QPoint(3, 3), 10);
    QTest::qWait(100);

    QVERIFY(menu->isTearOffMenuVisible());
    QPointer<QMenu> torn = 0;
    foreach (QWidget *w, QApplication::allWidgets()) {
        if (w->inherits("QTornOffMenu")) {
            torn = static_cast<QMenu *>(w);
            break;
        }
    }
    QVERIFY(torn);
    QVERIFY(torn->isVisible());

    menu->hideTearOffMenu();
    QVERIFY(!menu->isTearOffMenuVisible());
    QVERIFY(!torn->isVisible());
}

void tst_QMenu::layoutDirection()
{
    QMainWindow win;
    win.setLayoutDirection(Qt::RightToLeft);

    QMenu menu(&win);
    menu.show();
    QTest::qWaitForWindowShown(&menu);
    QCOMPARE(menu.layoutDirection(), Qt::RightToLeft);
    menu.close();

    menu.setParent(0);
    menu.show();
    QTest::qWaitForWindowShown(&menu);
    QCOMPARE(menu.layoutDirection(), QApplication::layoutDirection());
    menu.close();

    //now the menubar
    QAction *action = win.menuBar()->addMenu(&menu);
    win.menuBar()->setActiveAction(action);
    QTest::qWaitForWindowShown(&menu);
    QCOMPARE(menu.layoutDirection(), Qt::RightToLeft);
}



#if defined(QT3_SUPPORT)
void tst_QMenu::indexBasedInsertion_data()
{
    QTest::addColumn<int>("indexForInsertion");
    QTest::addColumn<int>("expectedIndex");

    QTest::newRow("negative-index-appends") << -1 << 1;
    QTest::newRow("prepend") << 0 << 0;
    QTest::newRow("append") << 1 << 1;
}

void tst_QMenu::indexBasedInsertion()
{
    // test the compat'ed index based insertion

    QFETCH(int, indexForInsertion);
    QFETCH(int, expectedIndex);

    {
        QMenu menu;
        menu.addAction("Regular Item");

        menu.insertItem("New Item", -1 /*id*/, indexForInsertion);

        QAction *act = menu.actions().value(expectedIndex);
        QVERIFY(act);
        QCOMPARE(act->text(), QString("New Item"));
    }
    {
        QMenu menu;
        menu.addAction("Regular Item");

        menu.insertSeparator(indexForInsertion);

        QAction *act = menu.actions().value(expectedIndex);
        QVERIFY(act);
        QVERIFY(act->isSeparator());
    }
}
#endif

void tst_QMenu::task208001_stylesheet()
{
    //test if it crash
    QMainWindow main;
    main.setStyleSheet("QMenu [title =\"File\"] { color: red;}");
    main.menuBar()->addMenu("File");
}

void tst_QMenu::activeSubMenuPosition()
{
    QPushButton lab("subMenuPosition test");

    QMenu *sub = new QMenu("Submenu", &lab);
    sub->addAction("Sub-Item1");
    QAction *subAction = sub->addAction("Sub-Item2");

    QMenu *main = new QMenu("Menu-Title", &lab);
    (void)main->addAction("Item 1");
    QAction *menuAction = main->addMenu(sub);
    (void)main->addAction("Item 3");
    (void)main->addAction("Item 4");

    main->setActiveAction(menuAction);
    sub->setActiveAction(subAction);
#ifdef Q_OS_SYMBIAN
    main->popup(QPoint(50,200));
#else
    main->popup(QPoint(200,200));
#endif

    QVERIFY(main->isVisible());
    QCOMPARE(main->activeAction(), menuAction);
    QVERIFY(sub->isVisible());
    QVERIFY(sub->pos() != QPoint(0,0));
    // well, it's enough to check the pos is not (0,0) but it's more safe
    // to check that submenu is to the right of the main menu too.
#ifdef Q_OS_WINCE_WM
    QSKIP("Not true for Windows Mobile Soft Keys", SkipSingle);
#endif

#ifdef Q_OS_SYMBIAN
    // On Symbian, QS60Style::pixelMetric(QStyle::PM_SubMenuOverlap) is different with other styles.
    QVERIFY(sub->pos().x() < main->pos().x());
#else
    QVERIFY(sub->pos().x() > main->pos().x());
#endif
    QCOMPARE(sub->activeAction(), subAction);
}

void tst_QMenu::task242454_sizeHint()
{
    QMenu menu;
    QString s = QLatin1String("foo\nfoo\nfoo\nfoo");
    menu.addAction(s);
    QVERIFY(menu.sizeHint().width() > menu.fontMetrics().boundingRect(QRect(), Qt::TextSingleLine, s).width());
}


class Menu : public QMenu
{
    Q_OBJECT
public slots:
    void clear()
    {
        QMenu::clear();
    }
};

void tst_QMenu::task176201_clear()
{
    //this test used to crash
    Menu menu;
    QAction *action = menu.addAction("test");
    menu.connect(action, SIGNAL(triggered()), SLOT(clear()));
    menu.popup(QPoint());
    QTest::mouseClick(&menu, Qt::LeftButton, 0, menu.rect().center());
}

void tst_QMenu::task250673_activeMultiColumnSubMenuPosition()
{
    class MyMenu : public QMenu
    {
    public:
        int columnCount() const { return QMenu::columnCount(); }
    };

    QMenu sub;

    if (sub.style()->styleHint(QStyle::SH_Menu_Scrollable, 0, &sub)) {
        //the style prevents the menus from getting columns
        QSKIP("the style doesn't support multiple columns, it makes the menu scrollable", SkipSingle);
    }

    sub.addAction("Sub-Item1");
    QAction *subAction = sub.addAction("Sub-Item2");

    MyMenu main;
    main.addAction("Item 1");
    QAction *menuAction = main.addMenu(&sub);
    main.popup(QPoint(200,200));

    uint i = 2;
    while (main.columnCount() < 2) {
        main.addAction(QString("Item %1").arg(i));
        ++i;
        Q_ASSERT(i<1000);
    }
    main.setActiveAction(menuAction);
    sub.setActiveAction(subAction);

    QVERIFY(main.isVisible());
    QCOMPARE(main.activeAction(), menuAction);
    QVERIFY(sub.isVisible());
    QVERIFY(sub.pos().x() > main.pos().x());

    const int subMenuOffset = main.style()->pixelMetric(QStyle::PM_SubMenuOverlap, 0, &main);
    QVERIFY((sub.geometry().left() - subMenuOffset + 5) < main.geometry().right());
}


void tst_QMenu::task256918_setFont()
{
    QMenu menu;
    QAction *action = menu.addAction("foo");
    QFont f;
    f.setPointSize(30);
    action->setFont(f);
    menu.show(); //ensures that the actiongeometry are calculated
    QVERIFY(menu.actionGeometry(action).height() > f.pointSize());
}

void tst_QMenu::menuSizeHint()
{
    QMenu menu;
    //this is a list of arbitrary strings so that we check the geometry
    QStringList list = QStringList() << "trer" << "ezrfgtgvqd" << "sdgzgzerzerzer" << "eerzertz"  << "er";
    foreach(QString str, list)
        menu.addAction(str);

    int left, top, right, bottom;
    menu.getContentsMargins(&left, &top, &right, &bottom);
    const int panelWidth = menu.style()->pixelMetric(QStyle::PM_MenuPanelWidth, 0, &menu);
    const int hmargin = menu.style()->pixelMetric(QStyle::PM_MenuHMargin, 0, &menu),
        vmargin = menu.style()->pixelMetric(QStyle::PM_MenuVMargin, 0, &menu);

    int maxWidth =0;
    QRect result;
    foreach(QAction *action, menu.actions()) {
#ifdef QT_SOFTKEYS_ENABLED
        // Softkey actions are not widgets and have no geometry.
        if (menu.actionGeometry(action).topLeft() == QPoint(0,0))
            continue;
#endif
        maxWidth = qMax(maxWidth, menu.actionGeometry(action).width());
        result |= menu.actionGeometry(action);
        QCOMPARE(result.x(), left + hmargin + panelWidth);
        QCOMPARE(result.y(), top + vmargin + panelWidth);
    }

    QStyleOption opt(0);
    opt.rect = menu.rect();
    opt.state = QStyle::State_None;

    QSize resSize = QSize(result.x(), result.y()) + result.size() + QSize(hmargin + right + panelWidth, vmargin + top + panelWidth);

    resSize = menu.style()->sizeFromContents(QStyle::CT_Menu, &opt,
                                    resSize.expandedTo(QApplication::globalStrut()), &menu);

    QCOMPARE(resSize, menu.sizeHint());
}

class Menu258920 : public QMenu
{
    Q_OBJECT
public slots:
    void paintEvent(QPaintEvent *e)
    {
        QMenu::paintEvent(e);
        painted = true;
    }

public:
    bool painted;
};

void tst_QMenu::task258920_mouseBorder()
{
#ifdef Q_OS_WINCE_WM
    QSKIP("Mouse move related signals for Windows Mobile unavailable", SkipAll);
#endif
    Menu258920 menu;
    // On Symbian, styleHint(QStyle::SH_Menu_MouseTracking) in QS60Style is false.
    // For other styles which inherit from QWindowsStyle, the value is true.
    menu.setMouseTracking(true);
    QAction *action = menu.addAction("test");

    menu.popup(QApplication::desktop()->availableGeometry().center());
    QTest::qWaitForWindowShown(&menu);
    QTest::qWait(100);
    QRect actionRect = menu.actionGeometry(action);
    QTest::mouseMove(&menu, actionRect.center());
    QTest::qWait(30);
    QTest::mouseMove(&menu, actionRect.center() + QPoint(10, 0));
    QTest::qWait(30);
    QCOMPARE(action, menu.activeAction());
    menu.painted = false;
    QTest::mouseMove(&menu, QPoint(actionRect.center().x(), actionRect.bottom() + 1));
    QTest::qWait(30);
    QCOMPARE(static_cast<QAction*>(0), menu.activeAction());
    QVERIFY(menu.painted);
}

void tst_QMenu::setFixedWidth()
{
    QMenu menu;
    menu.addAction("action");
    menu.setFixedWidth(300);
    //the sizehint should reflect the minimumwidth because the action will try to
    //get as much space as possible
    QCOMPARE(menu.sizeHint().width(), menu.minimumWidth());
}

void tst_QMenu::deleteActionInTriggered()
{
    // should not crash
    QMenu m;
    QObject::connect(&m, SIGNAL(triggered(QAction*)), this, SLOT(deleteAction(QAction*)));
    QWeakPointer<QAction> a = m.addAction("action");
    a.data()->trigger();
    QVERIFY(!a);
}

void tst_QMenu::pushButtonPopulateOnAboutToShow()
{
    QPushButton b("Test PushButton");
    b.setWindowFlags(Qt::FramelessWindowHint | Qt::X11BypassWindowManagerHint);
    lastMenu = new QMenu;
    b.setMenu(lastMenu);
    const int scrNumber = QApplication::desktop()->screenNumber(&b);
    connect(lastMenu, SIGNAL(aboutToShow()), this, SLOT(populateMenu()));
    b.show();
    const QRect screen = QApplication::desktop()->screenGeometry(scrNumber);

    b.move(10, screen.bottom()-b.height()-5);
    QTest::qWaitForWindowShown(&b);
    QTimer::singleShot(300,lastMenu, SLOT(hide()));
    QTest::mouseClick(&b, Qt::LeftButton, Qt::NoModifier, b.rect().center());
    QVERIFY(!lastMenu->geometry().intersects(b.geometry()));

    b.move(10, screen.bottom()-lastMenu->height()-5);
    QTimer::singleShot(300,lastMenu, SLOT(hide()));
    QTest::mouseClick(&b, Qt::LeftButton, Qt::NoModifier, b.rect().center());
    QVERIFY(!lastMenu->geometry().intersects(b.geometry()));

}
void tst_QMenu::QTBUG7907_submenus_autoselect()
{
    QMenu menu("Test Menu");
    QMenu set1("Setting1");
    QMenu set2("Setting2");
    QMenu subset("Subsetting");
    subset.addAction("Values");
    set1.addMenu(&subset);
    menu.addMenu(&set1);
    menu.addMenu(&set2);
    menu.show();
    QTest::qWaitForWindowShown(&menu);
    QTest::mouseClick(&menu, Qt::LeftButton, Qt::NoModifier, QPoint(5,5) );
    QTest::qWait(500);
    QVERIFY(!subset.isVisible());
}

void tst_QMenu::QTBUG7411_submenus_activate()
{
    QMenu menu("Test Menu");
    QAction *act = menu.addAction("foo");
    QMenu sub1("&sub1");
    sub1.addAction("foo");
    sub1.setTitle("&sub1");
    QAction *act1 = menu.addMenu(&sub1);
    menu.show();
    QTest::qWaitForWindowShown(&menu);
    menu.setActiveAction(act);
    QTest::keyPress(&menu, Qt::Key_Down);
    QCOMPARE(menu.activeAction(), act1);
    QVERIFY(!sub1.isVisible());
    QTest::keyPress(&menu, Qt::Key_S);
    QTRY_VERIFY(sub1.isVisible());
}

class MyMenu : public QMenu
{
    Q_OBJECT
public:
    MyMenu() : m_currentIndex(0)
    {
        for (int i = 0; i < 2; ++i)
            dialogActions[i] = addAction( QString("dialog %1").arg(i), dialogs + i, SLOT(exec()));
    }


    void activateAction(int index)
    {
        m_currentIndex = index;
        popup(QPoint());
        QTest::qWaitForWindowShown(this);
        setActiveAction(dialogActions[index]);
        QTimer::singleShot(500, this, SLOT(checkVisibility()));
        QTest::keyClick(this, Qt::Key_Enter); //activation
    }

public slots:
    void activateLastAction()
    {
        activateAction(1); 
    }

    void checkVisibility()
    {
        QTRY_VERIFY(dialogs[m_currentIndex].isVisible());
        if (m_currentIndex == 1) {
            QApplication::closeAllWindows(); //this is the end of the test
        }
    }


private:
    QAction *dialogActions[2];
    QDialog dialogs[2];
    int m_currentIndex;
};

void tst_QMenu::QTBUG_10735_crashWithDialog()
{
    MyMenu menu;

    QTimer::singleShot(1000, &menu, SLOT(activateLastAction()));
    menu.activateAction(0);
 
}


QTEST_MAIN(tst_QMenu)
#include "tst_qmenu.moc"
