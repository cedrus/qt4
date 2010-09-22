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

#include "qcombobox.h"
#include <private/qcombobox_p.h>

#include <qfontcombobox.h>
#include <qdesktopwidget.h>
#include <qapplication.h>
#include <qpushbutton.h>
#include <qdialog.h>
#include <qevent.h>
#include <qlineedit.h>
#include <qlistview.h>
#include <qheaderview.h>
#include <qlistwidget.h>
#include <qtreewidget.h>
#include <qtablewidget.h>
#include <qscrollbar.h>
#include <qboxlayout.h>
#ifdef Q_WS_MAC
#include <qmacstyle_mac.h>
#elif defined Q_WS_X11
#include <private/qt_x11_p.h>
#endif

#include <qstandarditemmodel.h>
#include <qstringlistmodel.h>
#include <qcombobox.h>
#include <qpushbutton.h>
#include <qdialog.h>
#include <qstringlist.h>
#include <qvalidator.h>
#include <qcompleter.h>
#ifndef QT_NO_STYLE_CLEANLOOKS
#include <qcleanlooksstyle.h>
#endif
#include <qabstractitemview.h>
#include "../../shared/util.h"
#include <qstyleditemdelegate.h>
#ifndef QT_NO_STYLE_WINDOWS
#include <qwindowsstyle.h>
#endif

//TESTED_CLASS=
//TESTED_FILES=

class tst_QComboBox : public QObject
{
    Q_OBJECT

public:
    tst_QComboBox();
    ~tst_QComboBox();

public slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

private slots:
    void getSetCheck();
    void ensureReturnIsIgnored();
    void setEditable();
    void setPalette();
    void sizeAdjustPolicy();
    void clear();
    void insertPolicy_data();
    void insertPolicy();
    void virtualAutocompletion();
    void autoCompletionCaseSensitivity();
    void hide();
    void currentIndex_data();
    void currentIndex();
    void insertItems_data();
    void insertItems();
    void insertItem_data();
    void insertItem();
    void insertOnCurrentIndex();
    void textpixmapdata_data();
    void textpixmapdata();
    void editTextChanged();
    void setModel();
    void modelDeleted();
    void setMaxCount();
    void setCurrentIndex();
    void convenienceViews();
    void findText_data();
    void findText();
    void flaggedItems_data();
    void flaggedItems();
    void pixmapIcon();
    void mouseWheel_data();
    void mouseWheel();
    void layoutDirection();
    void itemListPosition();
    void separatorItem_data();
    void separatorItem();
    void task190351_layout();
    void task191329_size();
    void task166349_setEditableOnReturn();
    void task190205_setModelAdjustToContents();
    void task248169_popupWithMinimalSize();
    void task247863_keyBoardSelection();
    void task220195_keyBoardSelection2();
    void setModelColumn();
    void noScrollbar_data();
    void noScrollbar();
    void setItemDelegate();
    void task253944_itemDelegateIsReset();
    void subControlRectsWithOffset_data();
    void subControlRectsWithOffset();
    void task260974_menuItemRectangleForComboBoxPopup();
    void removeItem();
    void resetModel();
    void keyBoardNavigationWithMouse();
    void task_QTBUG_1071_changingFocusEmitsActivated();
    void maxVisibleItems();

protected slots:
    void onEditTextChanged( const QString &newString );

private:
    QComboBox *testWidget;
    QDialog *parent;
    QPushButton* ok;
    int editTextCount;
    QString editText;
};

class MyAbstractItemDelegate : public QAbstractItemDelegate
{
public:
    MyAbstractItemDelegate() : QAbstractItemDelegate() {};
    void paint(QPainter *, const QStyleOptionViewItem &, const QModelIndex &) const {}
    QSize sizeHint(const QStyleOptionViewItem &, const QModelIndex &) const { return QSize(); }
};

class MyAbstractItemModel: public QAbstractItemModel
{
public:
    MyAbstractItemModel() : QAbstractItemModel() {};
    QModelIndex index(int, int, const QModelIndex &) const { return QModelIndex(); }
    QModelIndex parent(const QModelIndex &) const  { return QModelIndex(); }
    int rowCount(const QModelIndex &) const { return 0; }
    int columnCount(const QModelIndex &) const { return 0; }
    bool hasChildren(const QModelIndex &) const { return false; }
    QVariant data(const QModelIndex &, int) const { return QVariant(); }
    bool setData(const QModelIndex &, const QVariant &, int) { return false; }
    bool insertRows(int, int, const QModelIndex &) { return false; }
    bool insertColumns(int, int, const QModelIndex &) { return false; }
    void setPersistent(const QModelIndex &, const QModelIndex &) {}
    bool removeRows (int, int, const QModelIndex &) { return false; }
    bool removeColumns(int, int, const QModelIndex &) { return false; }
    void reset() {}
};

class MyAbstractItemView : public QAbstractItemView
{
public:
    MyAbstractItemView() : QAbstractItemView() {}
    QRect visualRect(const QModelIndex &) const { return QRect(); }
    void scrollTo(const QModelIndex &, ScrollHint) {}
    QModelIndex indexAt(const QPoint &) const { return QModelIndex(); }
protected:
    QModelIndex moveCursor(CursorAction, Qt::KeyboardModifiers) { return QModelIndex(); }
    int horizontalOffset() const { return 0; }
    int verticalOffset() const { return 0; }
    bool isIndexHidden(const QModelIndex &) const { return false; }
    void setSelection(const QRect &, QItemSelectionModel::SelectionFlags) {}
    QRegion visualRegionForSelection(const QItemSelection &) const { return QRegion(); }
};

// Testing get/set functions
void tst_QComboBox::getSetCheck()
{
    QComboBox obj1;
    // int QComboBox::maxVisibleItems()
    // void QComboBox::setMaxVisibleItems(int)
    obj1.setMaxVisibleItems(100);
    QCOMPARE(100, obj1.maxVisibleItems());
    obj1.setMaxVisibleItems(0);
    QCOMPARE(obj1.maxVisibleItems(), 0);
    QTest::ignoreMessage(QtWarningMsg, "QComboBox::setMaxVisibleItems: "
                         "Invalid max visible items (-2147483648) must be >= 0");
    obj1.setMaxVisibleItems(INT_MIN);
    QCOMPARE(obj1.maxVisibleItems(), 0); // Cannot be set to something negative => old value
    obj1.setMaxVisibleItems(INT_MAX);
    QCOMPARE(INT_MAX, obj1.maxVisibleItems());

    // int QComboBox::maxCount()
    // void QComboBox::setMaxCount(int)
    obj1.setMaxCount(0);
    QCOMPARE(0, obj1.maxCount());
#ifndef QT_DEBUG
    QTest::ignoreMessage(QtWarningMsg, "QComboBox::setMaxCount: Invalid count (-2147483648) must be >= 0");
    obj1.setMaxCount(INT_MIN);
    QCOMPARE(0, obj1.maxCount()); // Setting a value below 0 makes no sense, and shouldn't be allowed
#endif
    obj1.setMaxCount(INT_MAX);
    QCOMPARE(INT_MAX, obj1.maxCount());

    // bool QComboBox::autoCompletion()
    // void QComboBox::setAutoCompletion(bool)
    obj1.setAutoCompletion(false);
    QCOMPARE(false, obj1.autoCompletion());
    obj1.setAutoCompletion(true);
    QCOMPARE(true, obj1.autoCompletion());

    // bool QComboBox::duplicatesEnabled()
    // void QComboBox::setDuplicatesEnabled(bool)
    obj1.setDuplicatesEnabled(false);
    QCOMPARE(false, obj1.duplicatesEnabled());
    obj1.setDuplicatesEnabled(true);
    QCOMPARE(true, obj1.duplicatesEnabled());

    // InsertPolicy QComboBox::insertPolicy()
    // void QComboBox::setInsertPolicy(InsertPolicy)
    obj1.setInsertPolicy(QComboBox::InsertPolicy(QComboBox::NoInsert));
    QCOMPARE(QComboBox::InsertPolicy(QComboBox::NoInsert), obj1.insertPolicy());
    obj1.setInsertPolicy(QComboBox::InsertPolicy(QComboBox::InsertAtTop));
    QCOMPARE(QComboBox::InsertPolicy(QComboBox::InsertAtTop), obj1.insertPolicy());
    obj1.setInsertPolicy(QComboBox::InsertPolicy(QComboBox::InsertAtCurrent));
    QCOMPARE(QComboBox::InsertPolicy(QComboBox::InsertAtCurrent), obj1.insertPolicy());
    obj1.setInsertPolicy(QComboBox::InsertPolicy(QComboBox::InsertAtBottom));
    QCOMPARE(QComboBox::InsertPolicy(QComboBox::InsertAtBottom), obj1.insertPolicy());
    obj1.setInsertPolicy(QComboBox::InsertPolicy(QComboBox::InsertAfterCurrent));
    QCOMPARE(QComboBox::InsertPolicy(QComboBox::InsertAfterCurrent), obj1.insertPolicy());
    obj1.setInsertPolicy(QComboBox::InsertPolicy(QComboBox::InsertBeforeCurrent));
    QCOMPARE(QComboBox::InsertPolicy(QComboBox::InsertBeforeCurrent), obj1.insertPolicy());
    obj1.setInsertPolicy(QComboBox::InsertPolicy(QComboBox::InsertAlphabetically));
    QCOMPARE(QComboBox::InsertPolicy(QComboBox::InsertAlphabetically), obj1.insertPolicy());

    // SizeAdjustPolicy QComboBox::sizeAdjustPolicy()
    // void QComboBox::setSizeAdjustPolicy(SizeAdjustPolicy)
    obj1.setSizeAdjustPolicy(QComboBox::SizeAdjustPolicy(QComboBox::AdjustToContents));
    QCOMPARE(QComboBox::SizeAdjustPolicy(QComboBox::AdjustToContents), obj1.sizeAdjustPolicy());
    obj1.setSizeAdjustPolicy(QComboBox::SizeAdjustPolicy(QComboBox::AdjustToContentsOnFirstShow));
    QCOMPARE(QComboBox::SizeAdjustPolicy(QComboBox::AdjustToContentsOnFirstShow), obj1.sizeAdjustPolicy());
    obj1.setSizeAdjustPolicy(QComboBox::SizeAdjustPolicy(QComboBox::AdjustToMinimumContentsLength));
    QCOMPARE(QComboBox::SizeAdjustPolicy(QComboBox::AdjustToMinimumContentsLength), obj1.sizeAdjustPolicy());
    obj1.setSizeAdjustPolicy(QComboBox::SizeAdjustPolicy(QComboBox::AdjustToMinimumContentsLengthWithIcon));
    QCOMPARE(QComboBox::SizeAdjustPolicy(QComboBox::AdjustToMinimumContentsLengthWithIcon), obj1.sizeAdjustPolicy());

    // int QComboBox::minimumContentsLength()
    // void QComboBox::setMinimumContentsLength(int)
    obj1.setMinimumContentsLength(0);
    QCOMPARE(0, obj1.minimumContentsLength());
    obj1.setMinimumContentsLength(100);
    QCOMPARE(100, obj1.minimumContentsLength());
    obj1.setMinimumContentsLength(INT_MIN);
    QCOMPARE(100, obj1.minimumContentsLength()); // Cannot be set to something negative => old value
    obj1.setMinimumContentsLength(INT_MAX);
    QCOMPARE(INT_MAX, obj1.minimumContentsLength());

    // QLineEdit * QComboBox::lineEdit()
    // void QComboBox::setLineEdit(QLineEdit *)
    QLineEdit *var8 = new QLineEdit(0);
    obj1.setLineEdit(var8);
    QCOMPARE(var8, obj1.lineEdit());
    QTest::ignoreMessage(QtWarningMsg, "QComboBox::setLineEdit: cannot set a 0 line edit");
    obj1.setLineEdit((QLineEdit *)0);
    QCOMPARE(var8, obj1.lineEdit());
    // delete var8; // No delete, since QComboBox takes ownership

    // const QValidator * QComboBox::validator()
    // void QComboBox::setValidator(const QValidator *)
    QIntValidator *var9 = new QIntValidator(0);
    obj1.setValidator(var9);
    QCOMPARE(obj1.validator(), (const QValidator *)var9);
    obj1.setValidator((QValidator *)0);
    QCOMPARE(obj1.validator(), (const QValidator *)0);
    delete var9;

    // QAbstractItemDelegate * QComboBox::itemDelegate()
    // void QComboBox::setItemDelegate(QAbstractItemDelegate *)
    MyAbstractItemDelegate *var10 = new MyAbstractItemDelegate;
    obj1.setItemDelegate(var10);
    QCOMPARE(obj1.itemDelegate(), (QAbstractItemDelegate *)var10);
    QTest::ignoreMessage(QtWarningMsg, "QComboBox::setItemDelegate: cannot set a 0 delegate");
    obj1.setItemDelegate((QAbstractItemDelegate *)0);
    QCOMPARE(obj1.itemDelegate(), (QAbstractItemDelegate *)var10);
    // delete var10; // No delete, since QComboBox takes ownership

    // QAbstractItemModel * QComboBox::model()
    // void QComboBox::setModel(QAbstractItemModel *)
    MyAbstractItemModel *var11 = new MyAbstractItemModel;
    obj1.setModel(var11);
    QCOMPARE(obj1.model(), (QAbstractItemModel *)var11);
    QTest::ignoreMessage(QtWarningMsg, "QComboBox::setModel: cannot set a 0 model");
    obj1.setModel((QAbstractItemModel *)0);
    QCOMPARE(obj1.model(), (QAbstractItemModel *)var11);
    delete var11;
    obj1.model();

    // int QComboBox::modelColumn()
    // void QComboBox::setModelColumn(int)
    obj1.setModelColumn(0);
    QCOMPARE(0, obj1.modelColumn());
    obj1.setModelColumn(INT_MIN);
//    QCOMPARE(0, obj1.modelColumn()); // Cannot be set to something negative => column 0
    obj1.setModelColumn(INT_MAX);
    QCOMPARE(INT_MAX, obj1.modelColumn());
    obj1.setModelColumn(0); // back to normal

    // QAbstractItemView * QComboBox::view()
    // void QComboBox::setView(QAbstractItemView *)
    MyAbstractItemView *var13 = new MyAbstractItemView;
    obj1.setView(var13);
    QCOMPARE(obj1.view(), (QAbstractItemView *)var13);
    QTest::ignoreMessage(QtWarningMsg, "QComboBox::setView: cannot set a 0 view");
    obj1.setView((QAbstractItemView *)0);
    QCOMPARE(obj1.view(), (QAbstractItemView *)var13);
    delete var13;

    // int QComboBox::currentIndex()
    // void QComboBox::setCurrentIndex(int)
    obj1.setEditable(false);
    obj1.setCurrentIndex(0);
    QCOMPARE(-1, obj1.currentIndex());
    obj1.setCurrentIndex(INT_MIN);
    QCOMPARE(-1, obj1.currentIndex());
    obj1.setCurrentIndex(INT_MAX);
    QCOMPARE(-1, obj1.currentIndex());
    obj1.addItems(QStringList() << "1" << "2" << "3" << "4" << "5");
    obj1.setCurrentIndex(0);
    QCOMPARE(0, obj1.currentIndex()); // Valid
    obj1.setCurrentIndex(INT_MIN);
    QCOMPARE(-1, obj1.currentIndex()); // Invalid => -1
    obj1.setCurrentIndex(4);
    QCOMPARE(4, obj1.currentIndex()); // Valid
    obj1.setCurrentIndex(INT_MAX);
    QCOMPARE(-1, obj1.currentIndex()); // Invalid => -1
}

typedef QList<QVariant> VariantList;
typedef QList<QIcon> IconList;

Q_DECLARE_METATYPE(VariantList)
Q_DECLARE_METATYPE(IconList)
Q_DECLARE_METATYPE(QComboBox::InsertPolicy)

tst_QComboBox::tst_QComboBox()
{
    qRegisterMetaType<QModelIndex>("QModelIndex");
    parent = 0;
}

tst_QComboBox::~tst_QComboBox()
{
}


void tst_QComboBox::initTestCase()
{
    // Create the test class
    parent = new QDialog(0);
    parent->setObjectName("parent");
    parent->resize(400, 400);
    testWidget = new QComboBox(parent);
    testWidget->setObjectName("testObject");
    testWidget->setGeometry(0, 0, 100, 100);
    editTextCount = 0;
    editText.clear();
    connect(testWidget, SIGNAL(editTextChanged(const QString&)),
            this, SLOT(onEditTextChanged(const QString&)));
    parent->show();
}

void tst_QComboBox::cleanupTestCase()
{
    delete parent;
    parent = 0;
}


void tst_QComboBox::init()
{
    // all tests starts with a clean non-editable combobox
    testWidget->setEditable(false);
    testWidget->clear();
#ifdef Q_OS_WINCE //disable magic for WindowsCE
    qApp->setAutoMaximizeThreshold(-1);
#endif
}

void tst_QComboBox::cleanup()
{
    //nothing
}


void tst_QComboBox::setEditable()
{
    // make sure we have no lineedit
    QVERIFY(!testWidget->lineEdit());
    // test setEditable(true)
    testWidget->setEditable(true);
    QVERIFY(testWidget->lineEdit());
    testWidget->addItem("foo");
    QCOMPARE(testWidget->lineEdit()->text(), QString("foo"));
    // test setEditable(false)

    QLineEdit *lineEdit = testWidget->lineEdit();
    // line edit is visible when combobox is editable
    QVERIFY(lineEdit->isVisible());
    testWidget->setEditable(false);
    QVERIFY(!testWidget->lineEdit());
    // line edit should have been explicitly hidden when editable was turned off
    QVERIFY(!lineEdit->isVisible());
}


void tst_QComboBox::setPalette()
{
#ifdef Q_WS_MAC
    if (qobject_cast<QMacStyle *>(testWidget->style())) {
        QSKIP("This test doesn't make sense for pixmap-based styles", SkipAll);
    }
#endif
    QPalette pal = testWidget->palette();
    pal.setColor(QPalette::Base, Qt::red);
    testWidget->setPalette(pal);
    testWidget->setEditable(!testWidget->isEditable());

    pal.setColor(QPalette::Base, Qt::blue);
    testWidget->setPalette(pal);

    const QObjectList comboChildren = testWidget->children();
    for (int i = 0; i < comboChildren.size(); ++i) {
        QObject *o = comboChildren.at(i);
        if (o->isWidgetType()) {
            QVERIFY(((QWidget*)o)->palette() == pal);
        }
    }
}

void tst_QComboBox::sizeAdjustPolicy()
{
    // test that adding new items will not change the sizehint for AdjustToContentsOnFirstShow
    QVERIFY(!testWidget->count());
    QVERIFY(testWidget->sizeAdjustPolicy() == QComboBox::AdjustToContentsOnFirstShow);
    QVERIFY(testWidget->isVisible());
    QSize firstShow = testWidget->sizeHint();
    testWidget->addItem("normal item");
    QCOMPARE(testWidget->sizeHint(), firstShow);

    // check that with minimumContentsLength/AdjustToMinimumContentsLength sizehint changes
    testWidget->setMinimumContentsLength(30);
    QCOMPARE(testWidget->sizeHint(), firstShow);
    testWidget->setSizeAdjustPolicy(QComboBox::AdjustToMinimumContentsLength);
    QSize minimumContentsLength = testWidget->sizeHint();
    QVERIFY(minimumContentsLength.width() > firstShow.width());
    testWidget->setMinimumContentsLength(60);
    QVERIFY(minimumContentsLength.width() < testWidget->sizeHint().width());

    // check that with minimumContentsLength/AdjustToMinimumContentsLengthWithIcon sizehint changes
    testWidget->setSizeAdjustPolicy(QComboBox::AdjustToMinimumContentsLengthWithIcon);
    testWidget->setMinimumContentsLength(30);
    minimumContentsLength = testWidget->sizeHint();
    QVERIFY(minimumContentsLength.width() > firstShow.width());
    testWidget->setMinimumContentsLength(60);
    QVERIFY(minimumContentsLength.width() < testWidget->sizeHint().width());
    minimumContentsLength = testWidget->sizeHint();
    testWidget->setIconSize(QSize(128,128));
    QVERIFY(minimumContentsLength.width() < testWidget->sizeHint().width());

    // check AdjustToContents changes with content
    testWidget->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    QSize content = testWidget->sizeHint();
    testWidget->addItem("small");
    QCOMPARE(testWidget->sizeHint(), content);
    testWidget->addItem("looooooooooooooooooooooong item");
    // minimumContentsLength() > sizeof("looooooooooooooooooooooong item"), so the sizeHint()
    // stays the same
    QCOMPARE(testWidget->sizeHint(), content);
    // over 60 characters (cf. setMinimumContentsLength() call above)
    testWidget->addItem("loooooooooooooooooooooooooooooooooooooooooooooo"
                        "ooooooooooooooooooooooooooooooooooooooooooooooo"
                        "ooooooooooooooooooooooooooooong item");
    QVERIFY(testWidget->sizeHint().width() > content.width());

    // check AdjustToContents also shrinks when item changes
    content = testWidget->sizeHint();
    for (int i=0; i<testWidget->count(); ++i)
        testWidget->setItemText(i, "XXXXXXXXXX");
    QVERIFY(testWidget->sizeHint().width() < content.width());

    // check AdjustToContents shrinks when items are removed
    content = testWidget->sizeHint();
    while (testWidget->count())
        testWidget->removeItem(0);
    QCOMPARE(testWidget->sizeHint(), content);
    testWidget->setMinimumContentsLength(0);
    QVERIFY(testWidget->sizeHint().width() < content.width());
}

void tst_QComboBox::clear()
{
    // first non editable combobox
    testWidget->addItem("foo");
    testWidget->addItem("bar");
    QVERIFY(testWidget->count() > 0);
    QCOMPARE(testWidget->currentIndex(), 0);

    testWidget->clear();
    QCOMPARE(testWidget->count(), 0);
    QCOMPARE(testWidget->currentIndex(), -1);
    QVERIFY(testWidget->currentText().isEmpty());

    // then editable combobox
    testWidget->clear();
    testWidget->setEditable(true);
    testWidget->addItem("foo");
    testWidget->addItem("bar");
    QVERIFY(testWidget->count() > 0);
    QCOMPARE(testWidget->currentIndex(), 0);
    QVERIFY(testWidget->lineEdit());
    QVERIFY(!testWidget->lineEdit()->text().isEmpty());
    testWidget->clear();
    QCOMPARE(testWidget->count(), 0);
    QCOMPARE(testWidget->currentIndex(), -1);
    QVERIFY(testWidget->currentText().isEmpty());
    QVERIFY(testWidget->lineEdit()->text().isEmpty());
}

void tst_QComboBox::insertPolicy_data()
{
    QTest::addColumn<QStringList>("initialEntries");
    QTest::addColumn<QComboBox::InsertPolicy>("insertPolicy");
    QTest::addColumn<int>("currentIndex");
    QTest::addColumn<QString>("userInput");
    QTest::addColumn<QStringList>("result");

    /* Each insertPolicy should test at least:
       no initial entries
       one initial entry
       five initial entries, current is first item
       five initial entries, current is third item
       five initial entries, current is last item
    */

    /* QComboBox::NoInsert - the string will not be inserted into the combobox.
       QComboBox::InsertAtTop - insert the string as the first item in the combobox.
       QComboBox::InsertAtCurrent - replace the previously selected item with the string the user has entered.
       QComboBox::InsertAtBottom - insert the string as the last item in the combobox.
       QComboBox::InsertAfterCurrent - insert the string after the previously selected item.
       QComboBox::InsertBeforeCurrent - insert the string before the previously selected item.
       QComboBox::InsertAlphabetically - insert the string at the alphabetic position.
    */
    QStringList initial;
    QStringList oneEntry("One");
    QStringList fiveEntries;
    fiveEntries << "One" << "Two" << "Three" << "Four" << "Five";
    QString input("insert");

    {
        QTest::newRow("NoInsert-NoInitial") << initial << QComboBox::NoInsert << 0 << input << initial;
        QTest::newRow("NoInsert-OneInitial") << oneEntry << QComboBox::NoInsert << 0 << input << oneEntry;
        QTest::newRow("NoInsert-FiveInitial-FirstCurrent") << fiveEntries << QComboBox::NoInsert << 0 << input << fiveEntries;
        QTest::newRow("NoInsert-FiveInitial-ThirdCurrent") << fiveEntries << QComboBox::NoInsert << 2 << input << fiveEntries;
        QTest::newRow("NoInsert-FiveInitial-LastCurrent") << fiveEntries << QComboBox::NoInsert << 4 << input << fiveEntries;
    }

    {
        QStringList initialAtTop("insert");
        QStringList oneAtTop;
        oneAtTop << "insert" << "One";
        QStringList fiveAtTop;
        fiveAtTop << "insert" << "One" << "Two" << "Three" << "Four" << "Five";

        QTest::newRow("AtTop-NoInitial") << initial << QComboBox::InsertAtTop << 0 << input << initialAtTop;
        QTest::newRow("AtTop-OneInitial") << oneEntry << QComboBox::InsertAtTop << 0 << input << oneAtTop;
        QTest::newRow("AtTop-FiveInitial-FirstCurrent") << fiveEntries << QComboBox::InsertAtTop << 0 << input << fiveAtTop;
        QTest::newRow("AtTop-FiveInitial-ThirdCurrent") << fiveEntries << QComboBox::InsertAtTop << 2 << input << fiveAtTop;
        QTest::newRow("AtTop-FiveInitial-LastCurrent") << fiveEntries << QComboBox::InsertAtTop << 4 << input << fiveAtTop;
    }

    {
        QStringList initialAtCurrent("insert");
        QStringList oneAtCurrent("insert");
        QStringList fiveAtCurrentFirst;
        fiveAtCurrentFirst << "insert" << "Two" << "Three" << "Four" << "Five";
        QStringList fiveAtCurrentThird;
        fiveAtCurrentThird << "One" << "Two" << "insert" << "Four" << "Five";
        QStringList fiveAtCurrentLast;
        fiveAtCurrentLast << "One" << "Two" << "Three" << "Four" << "insert";

        QTest::newRow("AtCurrent-NoInitial") << initial << QComboBox::InsertAtCurrent << 0 << input << initialAtCurrent;
        QTest::newRow("AtCurrent-OneInitial") << oneEntry << QComboBox::InsertAtCurrent << 0 << input << oneAtCurrent;
        QTest::newRow("AtCurrent-FiveInitial-FirstCurrent") << fiveEntries << QComboBox::InsertAtCurrent << 0 << input << fiveAtCurrentFirst;
        QTest::newRow("AtCurrent-FiveInitial-ThirdCurrent") << fiveEntries << QComboBox::InsertAtCurrent << 2 << input << fiveAtCurrentThird;
        QTest::newRow("AtCurrent-FiveInitial-LastCurrent") << fiveEntries << QComboBox::InsertAtCurrent << 4 << input << fiveAtCurrentLast;
    }

    {
        QStringList initialAtBottom("insert");
        QStringList oneAtBottom;
        oneAtBottom << "One" << "insert";
        QStringList fiveAtBottom;
        fiveAtBottom << "One" << "Two" << "Three" << "Four" << "Five" << "insert";

        QTest::newRow("AtBottom-NoInitial") << initial << QComboBox::InsertAtBottom << 0 << input << initialAtBottom;
        QTest::newRow("AtBottom-OneInitial") << oneEntry << QComboBox::InsertAtBottom << 0 << input << oneAtBottom;
        QTest::newRow("AtBottom-FiveInitial-FirstCurrent") << fiveEntries << QComboBox::InsertAtBottom << 0 << input << fiveAtBottom;
        QTest::newRow("AtBottom-FiveInitial-ThirdCurrent") << fiveEntries << QComboBox::InsertAtBottom << 2 << input << fiveAtBottom;
        QTest::newRow("AtBottom-FiveInitial-LastCurrent") << fiveEntries << QComboBox::InsertAtBottom << 4 << input << fiveAtBottom;
    }

    {
        QStringList initialAfterCurrent("insert");
        QStringList oneAfterCurrent;
        oneAfterCurrent << "One" << "insert";
        QStringList fiveAfterCurrentFirst;
        fiveAfterCurrentFirst << "One" << "insert" << "Two" << "Three" << "Four" << "Five";
        QStringList fiveAfterCurrentThird;
        fiveAfterCurrentThird << "One" << "Two" << "Three" << "insert" << "Four" << "Five";
        QStringList fiveAfterCurrentLast;
        fiveAfterCurrentLast << "One" << "Two" << "Three" << "Four" << "Five" << "insert";

        QTest::newRow("AfterCurrent-NoInitial") << initial << QComboBox::InsertAfterCurrent << 0 << input << initialAfterCurrent;
        QTest::newRow("AfterCurrent-OneInitial") << oneEntry << QComboBox::InsertAfterCurrent << 0 << input << oneAfterCurrent;
        QTest::newRow("AfterCurrent-FiveInitial-FirstCurrent") << fiveEntries << QComboBox::InsertAfterCurrent << 0 << input << fiveAfterCurrentFirst;
        QTest::newRow("AfterCurrent-FiveInitial-ThirdCurrent") << fiveEntries << QComboBox::InsertAfterCurrent << 2 << input << fiveAfterCurrentThird;
        QTest::newRow("AfterCurrent-FiveInitial-LastCurrent") << fiveEntries << QComboBox::InsertAfterCurrent << 4 << input << fiveAfterCurrentLast;
    }

    {
        QStringList initialBeforeCurrent("insert");
        QStringList oneBeforeCurrent;
        oneBeforeCurrent << "insert" << "One";
        QStringList fiveBeforeCurrentFirst;
        fiveBeforeCurrentFirst << "insert" << "One" << "Two" << "Three" << "Four" << "Five";
        QStringList fiveBeforeCurrentThird;
        fiveBeforeCurrentThird << "One" << "Two" << "insert" << "Three" << "Four" << "Five";
        QStringList fiveBeforeCurrentLast;
        fiveBeforeCurrentLast << "One" << "Two" << "Three" << "Four" << "insert" << "Five";

        QTest::newRow("BeforeCurrent-NoInitial") << initial << QComboBox::InsertBeforeCurrent << 0 << input << initialBeforeCurrent;
        QTest::newRow("BeforeCurrent-OneInitial") << oneEntry << QComboBox::InsertBeforeCurrent << 0 << input << oneBeforeCurrent;
        QTest::newRow("BeforeCurrent-FiveInitial-FirstCurrent") << fiveEntries << QComboBox::InsertBeforeCurrent << 0 << input << fiveBeforeCurrentFirst;
        QTest::newRow("BeforeCurrent-FiveInitial-ThirdCurrent") << fiveEntries << QComboBox::InsertBeforeCurrent << 2 << input << fiveBeforeCurrentThird;
        QTest::newRow("BeforeCurrent-FiveInitial-LastCurrent") << fiveEntries << QComboBox::InsertBeforeCurrent << 4 << input << fiveBeforeCurrentLast;
    }

    {
        oneEntry.clear();
        oneEntry << "foobar";
        fiveEntries.clear();
        fiveEntries << "bar" << "foo" << "initial" << "Item" << "stamp";

        QStringList initialAlphabetically("insert");
        QStringList oneAlphabetically;
        oneAlphabetically << "foobar" << "insert";
        QStringList fiveAlphabetically;
        fiveAlphabetically << "bar" << "foo" << "initial" << "insert" << "Item" << "stamp";

        QTest::newRow("Alphabetically-NoInitial") << initial << QComboBox::InsertAlphabetically << 0 << input << initialAlphabetically;
        QTest::newRow("Alphabetically-OneInitial") << oneEntry << QComboBox::InsertAlphabetically << 0 << input << oneAlphabetically;
        QTest::newRow("Alphabetically-FiveInitial-FirstCurrent") << fiveEntries << QComboBox::InsertAlphabetically << 0 << input << fiveAlphabetically;
        QTest::newRow("Alphabetically-FiveInitial-ThirdCurrent") << fiveEntries << QComboBox::InsertAlphabetically << 2 << input << fiveAlphabetically;
        QTest::newRow("Alphabetically-FiveInitial-LastCurrent") << fiveEntries << QComboBox::InsertAlphabetically << 4 << input << fiveAlphabetically;
    }
}

void tst_QComboBox::insertPolicy()
{
    QFETCH(QStringList, initialEntries);
    QFETCH(QComboBox::InsertPolicy, insertPolicy);
    QFETCH(int, currentIndex);
    QFETCH(QString, userInput);
    QFETCH(QStringList, result);

    testWidget->clear();
    testWidget->setInsertPolicy(insertPolicy);
    testWidget->addItems(initialEntries);
    testWidget->setEditable(true);
    if (initialEntries.count() > 0)
        testWidget->setCurrentIndex(currentIndex);

    // clear
    QTest::mouseDClick(testWidget->lineEdit(), Qt::LeftButton);
    QTest::keyClick(testWidget->lineEdit(), Qt::Key_Delete);

    QTest::keyClicks(testWidget->lineEdit(), userInput);
    QTest::keyClick(testWidget->lineEdit(), Qt::Key_Return);

    // First check that there is the right number of entries, or
    // we may unwittingly pass
    QVERIFY((int)result.count() == testWidget->count());

    // No need to compare if there are no strings to compare
    if (result.count() > 0) {
        for (int i=0; i<testWidget->count(); ++i) {
            QCOMPARE(testWidget->itemText(i), result.at(i));
        }
    }
}


void tst_QComboBox::virtualAutocompletion()
{
    testWidget->clear();
    testWidget->setAutoCompletion(true);
    testWidget->addItem("Foo");
    testWidget->addItem("Bar");
    testWidget->addItem("Boat");
    testWidget->addItem("Boost");
    testWidget->clearEditText();

    // We need to set the keyboard input interval to a higher value
    // as the processEvent() call takes too much time, so it restarts
    // the keyboard search then
#if defined(QT_ARCH_ARM) || defined(QT_ARCH_MIPS) || defined(QT_ARCH_SYMBIAN)
    int oldInterval = QApplication::keyboardInputInterval();
    QApplication::setKeyboardInputInterval(1500);
#endif

    // NOTE:
    // Cannot use keyClick for this test, as it simulates keyclicks too well
    // The virtual keyboards we're trying to catch here, do not perform that
    // well, and send a keypress & keyrelease right after each other.
    // This provokes the actual error, as there's no events in between to do
    // the text completion.
    QKeyEvent kp1(QEvent::KeyPress, Qt::Key_B, 0, "b");
    QKeyEvent kr1(QEvent::KeyRelease, Qt::Key_B, 0, "b");
    QApplication::sendEvent(testWidget, &kp1);
    QApplication::sendEvent(testWidget, &kr1);

    qApp->processEvents(); // Process events to trigger autocompletion
    QTRY_VERIFY(testWidget->currentIndex() == 1);

#ifdef QTEST_RUNNING_USING_VALGRIND
    QSKIP("App running with valgrind are not fast enough", SkipAll);
#endif

    QKeyEvent kp2(QEvent::KeyPress, Qt::Key_O, 0, "o");
    QKeyEvent kr2(QEvent::KeyRelease, Qt::Key_O, 0, "o");

    QApplication::sendEvent(testWidget, &kp2);
    QApplication::sendEvent(testWidget, &kr2);

    qApp->processEvents(); // Process events to trigger autocompletion
    QTRY_COMPARE(testWidget->currentIndex(), 2);

    QApplication::sendEvent(testWidget, &kp2);
    QApplication::sendEvent(testWidget, &kr2);
    qApp->processEvents(); // Process events to trigger autocompletion
    QTRY_COMPARE(testWidget->currentIndex(), 3);
#if defined(QT_ARCH_ARM) || defined(QT_ARCH_MIPS) || defined(QT_ARCH_SYMBIAN)
    QApplication::setKeyboardInputInterval(oldInterval);
#endif
}

void tst_QComboBox::autoCompletionCaseSensitivity()
{
    //we have put the focus because the completer
    //is only used when the widget actually has the focus
    testWidget->setFocus();
    qApp->setActiveWindow(testWidget);
    QTRY_COMPARE(qApp->focusWidget(), (QWidget *)testWidget);

    testWidget->clear();
    testWidget->setAutoCompletion(true);
    testWidget->addItem("Cow");
    testWidget->addItem("irrelevant1");
    testWidget->addItem("aww");
    testWidget->addItem("A*");
    testWidget->addItem("irrelevant2");
    testWidget->addItem("aBCDEF");
    testWidget->addItem("irrelevant3");
    testWidget->addItem("abcdef");
    testWidget->addItem("abCdef");
    testWidget->setEditable(true);

    // case insensitive
    testWidget->clearEditText();
    QSignalSpy spyReturn(testWidget, SIGNAL(activated(int)));
    testWidget->setAutoCompletionCaseSensitivity(Qt::CaseInsensitive);
    QVERIFY(testWidget->autoCompletionCaseSensitivity() == Qt::CaseInsensitive);

    QTest::keyClick(testWidget->lineEdit(), Qt::Key_A);
    qApp->processEvents();
    QCOMPARE(testWidget->currentText(), QString("aww"));
    QCOMPARE(spyReturn.count(), 0);

    QTest::keyClick(testWidget->lineEdit(), Qt::Key_B);
    qApp->processEvents();
    // autocompletions preserve userkey-case from 4.2
    QCOMPARE(testWidget->currentText(), QString("abCDEF"));
    QCOMPARE(spyReturn.count(), 0);

    QTest::keyClick(testWidget->lineEdit(), Qt::Key_Enter);
    qApp->processEvents();
    QCOMPARE(testWidget->currentText(), QString("aBCDEF")); // case restored to item's case
    QCOMPARE(spyReturn.count(), 1);

    testWidget->clearEditText();
    QTest::keyClick(testWidget->lineEdit(), 'c');
    QCOMPARE(testWidget->currentText(), QString("cow"));
    QTest::keyClick(testWidget->lineEdit(), Qt::Key_Enter);
    QCOMPARE(testWidget->currentText(), QString("Cow")); // case restored to item's case

    testWidget->clearEditText();
    QTest::keyClick(testWidget->lineEdit(), 'a');
    QTest::keyClick(testWidget->lineEdit(), '*');
    QCOMPARE(testWidget->currentText(), QString("a*"));
    QTest::keyClick(testWidget->lineEdit(), Qt::Key_Enter);
    QCOMPARE(testWidget->currentText(), QString("A*"));

    // case sensitive
    testWidget->clearEditText();
    testWidget->setAutoCompletionCaseSensitivity(Qt::CaseSensitive);
    QVERIFY(testWidget->autoCompletionCaseSensitivity() == Qt::CaseSensitive);
    QTest::keyClick(testWidget->lineEdit(), Qt::Key_A);
    qApp->processEvents();
    QCOMPARE(testWidget->currentText(), QString("aww"));
    QTest::keyClick(testWidget->lineEdit(), Qt::Key_B);
    qApp->processEvents();
    QCOMPARE(testWidget->currentText(), QString("abcdef"));

    testWidget->setCurrentIndex(0); // to reset the completion's "start"
    testWidget->clearEditText();
    QTest::keyClick(testWidget->lineEdit(), 'a');
    QTest::keyClick(testWidget->lineEdit(), 'b');
    QCOMPARE(testWidget->currentText(), QString("abcdef"));
    QTest::keyClick(testWidget->lineEdit(), 'C');
    qApp->processEvents();
    QCOMPARE(testWidget->currentText(), QString("abCdef"));
    QTest::keyClick(testWidget->lineEdit(), Qt::Key_Enter);
    qApp->processEvents();
    QCOMPARE(testWidget->currentText(), QString("abCdef")); // case restored to item's case

    testWidget->clearEditText();
    QTest::keyClick(testWidget->lineEdit(), 'c');
    QCOMPARE(testWidget->currentText(), QString("c"));
    QTest::keyClick(testWidget->lineEdit(), Qt::Key_Backspace);
    QTest::keyClick(testWidget->lineEdit(), 'C');
    QCOMPARE(testWidget->currentText(), QString("Cow"));
    QTest::keyClick(testWidget->lineEdit(), Qt::Key_Enter);
    QCOMPARE(testWidget->currentText(), QString("Cow"));

    testWidget->clearEditText();
    QTest::keyClick(testWidget->lineEdit(), 'a');
    QTest::keyClick(testWidget->lineEdit(), '*');
    QCOMPARE(testWidget->currentText(), QString("a*"));
    QTest::keyClick(testWidget->lineEdit(), Qt::Key_Enter);
    QCOMPARE(testWidget->currentText(), QString("a*")); // A* not matched
}

void tst_QComboBox::hide()
{
    testWidget->addItem("foo");
    testWidget->showPopup();
    //allow combobox effect to complete
    QTRY_VERIFY(testWidget->view());
    QTRY_VERIFY(testWidget->view()->isVisible());
    testWidget->hidePopup();
    //allow combobox effect to complete
    QTRY_VERIFY(!testWidget->view()->isVisible());
    testWidget->hide();
    QVERIFY(!testWidget->isVisible());
}



void tst_QComboBox::currentIndex_data()
{
    QTest::addColumn<QStringList>("initialItems");
    QTest::addColumn<int>("setCurrentIndex");
    QTest::addColumn<int>("removeIndex");
    QTest::addColumn<int>("insertIndex");
    QTest::addColumn<QString>("insertText");
    QTest::addColumn<int>("expectedCurrentIndex");
    QTest::addColumn<QString>("expectedCurrentText");
    QTest::addColumn<int>("expectedSignalCount");

    QStringList initialItems;
    int setCurrentIndex;
    int removeIndex;
    int insertIndex;
    QString insertText;
    int expectedCurrentIndex;
    QString expectedCurrentText;
    int expectedSignalCount;

    {
        initialItems.clear();
        initialItems << "foo" << "bar";
        setCurrentIndex = -2;
        removeIndex = -1;
        insertIndex = -1;
        insertText = "";
        expectedCurrentIndex = 0;
        expectedCurrentText = "foo";
        expectedSignalCount = 1;
        QTest::newRow("first added item is set to current if there is no current")
            << initialItems << setCurrentIndex << removeIndex
            << insertIndex << insertText << expectedCurrentIndex << expectedCurrentText
            << expectedSignalCount;
    }
    {
        initialItems.clear();
        initialItems << "foo" << "bar";
        setCurrentIndex = 1;
        removeIndex = -1;
        insertIndex = -1;
        insertText = "";
        expectedCurrentIndex = 1;
        expectedCurrentText = "bar";
        expectedSignalCount = 2;
        QTest::newRow("check that setting the index works")
            << initialItems << setCurrentIndex << removeIndex
            << insertIndex << insertText << expectedCurrentIndex << expectedCurrentText
            << expectedSignalCount;

    }
    {
        initialItems.clear();
        initialItems << "foo" << "bar";
        setCurrentIndex = -1; // will invalidate the currentIndex
        removeIndex = -1;
        insertIndex = -1;
        insertText = "";
        expectedCurrentIndex = -1;
        expectedCurrentText = "";
        expectedSignalCount = 2;
        QTest::newRow("check that isetting the index to -1 works")
            << initialItems << setCurrentIndex << removeIndex
            << insertIndex << insertText << expectedCurrentIndex << expectedCurrentText
            << expectedSignalCount;

    }
    {
        initialItems.clear();
        initialItems << "foo";
        setCurrentIndex = 0;
        removeIndex = 0;
        insertIndex = -1;
        insertText = "";
        expectedCurrentIndex = -1;
        expectedCurrentText = "";
        expectedSignalCount = 2;
        QTest::newRow("check that current index is invalid when removing the only item")
            << initialItems << setCurrentIndex << removeIndex
            << insertIndex << insertText << expectedCurrentIndex << expectedCurrentText
            << expectedSignalCount;
    }
    {
        initialItems.clear();
        initialItems << "foo" << "bar";
        setCurrentIndex = 1;
        removeIndex = 0;
        insertIndex = -1;
        insertText = "";
        expectedCurrentIndex = 0;
        expectedCurrentText = "bar";
        expectedSignalCount = 3;
        QTest::newRow("check that the current index follows the item when removing an item above")
            << initialItems << setCurrentIndex << removeIndex
            << insertIndex << insertText << expectedCurrentIndex << expectedCurrentText
            << expectedSignalCount;

    }
    {
        initialItems.clear();
        initialItems << "foo" << "bar" << "baz";
        setCurrentIndex = 1;
        removeIndex = 1;
        insertIndex = -1;
        insertText = "";
        expectedCurrentIndex = 1;
        expectedCurrentText = "baz";
        expectedSignalCount = 3;
        QTest::newRow("check that the current index uses the next item if current is removed")
            << initialItems << setCurrentIndex << removeIndex
            << insertIndex << insertText << expectedCurrentIndex << expectedCurrentText
            << expectedSignalCount;
    }
    {
        initialItems.clear();
        initialItems << "foo" << "bar" << "baz";
        setCurrentIndex = 2;
        removeIndex = 2;
        insertIndex = -1;
        insertText = "";
        expectedCurrentIndex = 1;
        expectedCurrentText = "bar";
        expectedSignalCount = 3;
        QTest::newRow("check that the current index is moved to the one before if current is removed")
            << initialItems << setCurrentIndex << removeIndex
            << insertIndex << insertText << expectedCurrentIndex << expectedCurrentText
            << expectedSignalCount;
    }
    {
        initialItems.clear();
        initialItems << "foo" << "bar" << "baz";
        setCurrentIndex = 1;
        removeIndex = 2;
        insertIndex = -1;
        insertText = "";
        expectedCurrentIndex = 1;
        expectedCurrentText = "bar";
        expectedSignalCount = 2;
        QTest::newRow("check that the current index is unchanged if you remove an item after")
            << initialItems << setCurrentIndex << removeIndex
            << insertIndex << insertText << expectedCurrentIndex << expectedCurrentText
            << expectedSignalCount;
    }
    {
        initialItems.clear();
        initialItems << "foo" << "bar";
        setCurrentIndex = 1;
        removeIndex = -1;
        insertIndex = 0;
        insertText = "baz";
        expectedCurrentIndex = 2;
        expectedCurrentText = "bar";
        expectedSignalCount = 3;
        QTest::newRow("check that the current index follows the item if you insert before current")
            << initialItems << setCurrentIndex << removeIndex
            << insertIndex << insertText << expectedCurrentIndex << expectedCurrentText
            << expectedSignalCount;
    }
    {
        initialItems.clear();
        initialItems << "foo";
        setCurrentIndex = 0;
        removeIndex = -1;
        insertIndex = 0;
        insertText = "bar";
        expectedCurrentIndex = 1;
        expectedCurrentText = "foo";
        expectedSignalCount = 2;
        QTest::newRow("check that the current index follows the item if you insert on the current")
            << initialItems << setCurrentIndex << removeIndex
            << insertIndex << insertText << expectedCurrentIndex << expectedCurrentText
            << expectedSignalCount;
    }
    {
        initialItems.clear();
        initialItems << "foo";
        setCurrentIndex = 0;
        removeIndex = -1;
        insertIndex = 1;
        insertText = "bar";
        expectedCurrentIndex = 0;
        expectedCurrentText = "foo";
        expectedSignalCount = 1;
        QTest::newRow("check that the current index stays the same if you insert after the current")
            << initialItems << setCurrentIndex << removeIndex
            << insertIndex << insertText << expectedCurrentIndex << expectedCurrentText
            << expectedSignalCount;
    }
}

void tst_QComboBox::currentIndex()
{
    QFETCH(QStringList, initialItems);
    QFETCH(int, setCurrentIndex);
    QFETCH(int, removeIndex);
    QFETCH(int, insertIndex);
    QFETCH(QString, insertText);
    QFETCH(int, expectedCurrentIndex);
    QFETCH(QString, expectedCurrentText);
    QFETCH(int, expectedSignalCount);

    // test both editable/non-editable combobox
    for (int edit = 0; edit < 2; ++edit) {
        testWidget->clear();
        testWidget->setEditable(edit ? true : false);
        if (edit)
            QVERIFY(testWidget->lineEdit());

        // verify it is empty, has no current index and no current text
        QCOMPARE(testWidget->count(), 0);
        QCOMPARE(testWidget->currentIndex(), -1);
        QVERIFY(testWidget->currentText().isEmpty());

        // spy on currentIndexChanged
        QSignalSpy indexChangedInt(testWidget, SIGNAL(currentIndexChanged(int)));
        QSignalSpy indexChangedString(testWidget, SIGNAL(currentIndexChanged(const QString&)));

        // stuff items into it
        foreach(QString text, initialItems) {
            testWidget->addItem(text);
        }
        QCOMPARE(testWidget->count(), initialItems.count());

        // set current index, remove and/or insert
        if (setCurrentIndex >= -1) {
            testWidget->setCurrentIndex(setCurrentIndex);
            QCOMPARE(testWidget->currentIndex(), setCurrentIndex);
        }

        if (removeIndex >= 0)
            testWidget->removeItem(removeIndex);
        if (insertIndex >= 0)
            testWidget->insertItem(insertIndex, insertText);

        // compare with expected index and text
        QCOMPARE(testWidget->currentIndex(), expectedCurrentIndex);
        QCOMPARE(testWidget->currentText(), expectedCurrentText);

        // check that signal count is correct
        QCOMPARE(indexChangedInt.count(), expectedSignalCount);
        QCOMPARE(indexChangedString.count(), expectedSignalCount);

        // compare with last sent signal values
        if (indexChangedInt.count())
            QCOMPARE(indexChangedInt.at(indexChangedInt.count() - 1).at(0).toInt(),
                    testWidget->currentIndex());
        if (indexChangedString.count())
            QCOMPARE(indexChangedString.at(indexChangedString.count() - 1).at(0).toString(),
                     testWidget->currentText());

        if (edit) {
            testWidget->setCurrentIndex(-1);
            testWidget->setInsertPolicy(QComboBox::InsertAtBottom);
            QTest::keyPress(testWidget, 'a');
            QTest::keyPress(testWidget, 'b');
            QCOMPARE(testWidget->currentText(), QString("ab"));
            QCOMPARE(testWidget->currentIndex(), -1);
            int numItems = testWidget->count();
            QTest::keyPress(testWidget, Qt::Key_Return);
            QCOMPARE(testWidget->count(), numItems + 1);
            QCOMPARE(testWidget->currentIndex(), numItems);
            testWidget->setCurrentIndex(-1);
            QTest::keyPress(testWidget, 'a');
            QTest::keyPress(testWidget, 'b');
            QCOMPARE(testWidget->currentIndex(), -1);
        }
    }
}

void tst_QComboBox::insertItems_data()
{
    QTest::addColumn<QStringList>("initialItems");
    QTest::addColumn<QStringList>("insertedItems");
    QTest::addColumn<int>("insertIndex");
    QTest::addColumn<int>("expectedIndex");

    QStringList initialItems;
    QStringList insertedItems;

    initialItems << "foo" << "bar";
    insertedItems << "mongo";

    QTest::newRow("prepend") << initialItems << insertedItems << 0 << 0;
    QTest::newRow("prepend with negative value") << initialItems << insertedItems << -1 << 0;
    QTest::newRow("append") << initialItems << insertedItems << initialItems.count() << initialItems.count();
    QTest::newRow("append with too high value") << initialItems << insertedItems << 999 << initialItems.count();
    QTest::newRow("insert") << initialItems << insertedItems << 1 << 1;
}

void tst_QComboBox::insertItems()
{
    QFETCH(QStringList, initialItems);
    QFETCH(QStringList, insertedItems);
    QFETCH(int, insertIndex);
    QFETCH(int, expectedIndex);

    testWidget->insertItems(0, initialItems);
    QCOMPARE(testWidget->count(), initialItems.count());

    testWidget->insertItems(insertIndex, insertedItems);

    QCOMPARE(testWidget->count(), initialItems.count() + insertedItems.count());
    for (int i=0; i<insertedItems.count(); ++i)
        QCOMPARE(testWidget->itemText(expectedIndex + i), insertedItems.at(i));
}

void tst_QComboBox::insertItem_data()
{
    QTest::addColumn<QStringList>("initialItems");
    QTest::addColumn<int>("insertIndex");
    QTest::addColumn<QString>("itemLabel");
    QTest::addColumn<int>("expectedIndex");
    QTest::addColumn<bool>("testQt3Support");
    QTest::addColumn<bool>("editable");

    QStringList initialItems;
    initialItems << "foo" << "bar";
    for(int e = 0 ; e<2 ; e++) {
        bool editable = (e==0);
        QTest::newRow("Insert less then 0") << initialItems << -1 << "inserted" << 0 << false << editable;
        QTest::newRow("Insert at 0") << initialItems << 0 << "inserted" << 0 << false << editable;
        QTest::newRow("Insert beyond count") << initialItems << 3 << "inserted" << 2 << false << editable;
        QTest::newRow("Insert at count") << initialItems << 2 << "inserted" << 2 << false << editable;
        QTest::newRow("Insert in the middle") << initialItems << 1 << "inserted" << 1 << false << editable;
#if defined(QT3_SUPPORT)
        QTest::newRow("Qt3Support: Insert less then 0") << initialItems << -1 << "inserted" << 2 << true << editable;
#endif
    }
}

void tst_QComboBox::insertItem()
{
    QFETCH(QStringList, initialItems);
    QFETCH(int, insertIndex);
    QFETCH(QString, itemLabel);
    QFETCH(int, expectedIndex);
    QFETCH(bool, testQt3Support);
    QFETCH(bool, editable);

    testWidget->insertItems(0, initialItems);
    QCOMPARE(testWidget->count(), initialItems.count());

    testWidget->setEditable(true);
    if (editable)
        testWidget->setEditText("FOO");
#if defined (QT3_SUPPORT)
    if (testQt3Support)
        testWidget->insertItem(itemLabel, insertIndex);
    else
        testWidget->insertItem(insertIndex, itemLabel);
#else
    Q_UNUSED(testQt3Support);
    testWidget->insertItem(insertIndex, itemLabel);
#endif


    QCOMPARE(testWidget->count(), initialItems.count() + 1);
    QCOMPARE(testWidget->itemText(expectedIndex), itemLabel);

    if (editable)
        QCOMPARE(testWidget->currentText(), QString("FOO"));
}

void tst_QComboBox::insertOnCurrentIndex()
{
    testWidget->setEditable(true);
    testWidget->addItem("first item");
    testWidget->setCurrentIndex(0);
    testWidget->insertItem(0, "second item");
    QCOMPARE(testWidget->lineEdit()->text(), QString::fromAscii("first item"));
}

void tst_QComboBox::textpixmapdata_data()
{
    QTest::addColumn<QStringList>("text");
    QTest::addColumn<IconList>("icons");
    QTest::addColumn<VariantList>("variant");

    QStringList text;
    IconList icon;
    VariantList variant;

    {
        text.clear(); icon.clear(); variant.clear();
        text << "foo" << "bar";
        icon << QIcon() << QIcon();
        variant << QVariant() << QVariant();
        QTest::newRow("just text") << text << icon << variant;
    }
    {
        text.clear(); icon.clear(); variant.clear();
        text << QString() << QString();
        icon << QIcon(QPixmap("qtlogo.png")) << QIcon(QPixmap("qtlogoinverted.png"));
        variant << QVariant() << QVariant();
        QTest::newRow("just icons") << text << icon << variant;
    }
    {
        text.clear(); icon.clear(); variant.clear();
        text << QString() << QString();
        icon << QIcon() << QIcon();
        variant << 12 << "bingo";
        QTest::newRow("just user data") << text << icon << variant;
    }
    {
        text.clear(); icon.clear(); variant.clear();
        text << "foo" << "bar";
        icon << QIcon(QPixmap("qtlogo.png")) << QIcon(QPixmap("qtlogoinverted.png"));
        variant << 12 << "bingo";
        QTest::newRow("text, icons and user data") << text << icon << variant;
    }
}

void tst_QComboBox::textpixmapdata()
{
    QFETCH(QStringList, text);
    QFETCH(IconList, icons);
    QFETCH(VariantList, variant);

    QVERIFY(text.count() == icons.count() && text.count() == variant.count());

    for (int i = 0; i<text.count(); ++i) {
        testWidget->insertItem(i, text.at(i));
        testWidget->setItemIcon(i, icons.at(i));
        testWidget->setItemData(i, variant.at(i), Qt::UserRole);
    }

    QCOMPARE(testWidget->count(), text.count());

    for (int i = 0; i<text.count(); ++i) {
        QIcon icon = testWidget->itemIcon(i);
        QVERIFY(icon.serialNumber() == icons.at(i).serialNumber());
        QPixmap original = icons.at(i).pixmap(1024);
        QPixmap pixmap = icon.pixmap(1024);
        QVERIFY(pixmap.toImage() == original.toImage());
    }

    for (int i = 0; i<text.count(); ++i) {
        QCOMPARE(testWidget->itemText(i), text.at(i));
        // ### we should test icons/pixmap as well, but I need to fix the api mismatch first
        QCOMPARE(testWidget->itemData(i, Qt::UserRole), variant.at(i));
    }
}

void tst_QComboBox::setCurrentIndex()
{
    QCOMPARE(testWidget->count(), 0);
    testWidget->addItem("foo");
    testWidget->addItem("bar");
    QCOMPARE(testWidget->count(), 2);

    QCOMPARE(testWidget->currentIndex(), 0);
    testWidget->setCurrentIndex(0);
    QCOMPARE(testWidget->currentText(), QString("foo"));

    testWidget->setCurrentIndex(1);
    QCOMPARE(testWidget->currentText(), QString("bar"));

    testWidget->setCurrentIndex(0);
    QCOMPARE(testWidget->currentText(), QString("foo"));
}

void tst_QComboBox::editTextChanged()
{
    QCOMPARE(testWidget->count(), 0);
    testWidget->addItem("foo");
    testWidget->addItem("bar");
    QCOMPARE(testWidget->count(), 2);

    // first we test non editable
    testWidget->setEditable(false);
    QCOMPARE(testWidget->isEditable(), false);

    // no signal should be sent when current is set to the same
    QCOMPARE(testWidget->currentIndex(), 0);
    editTextCount = 0;
    editText.clear();
    testWidget->setCurrentIndex(0);
    QCOMPARE(testWidget->currentIndex(), 0);
    QCOMPARE(editTextCount, 0);
    QCOMPARE(editText.isEmpty(), true);

    // no signal should be sent when changing to other index because we are not editable
    QCOMPARE(testWidget->currentIndex(), 0);
    editTextCount = 0;
    editText.clear();
    testWidget->setCurrentIndex(1);
    QCOMPARE(testWidget->currentIndex(), 1);
    QCOMPARE(editTextCount, 0);
    QCOMPARE(editText.isEmpty(), true);

    // now set to editable and reset current index
    testWidget->setEditable(true);
    QCOMPARE(testWidget->isEditable(), true);
    testWidget->setCurrentIndex(0);

    // no signal should be sent when current is set to the same
    QCOMPARE(testWidget->currentIndex(), 0);
    editTextCount = 0;
    editText.clear();
    testWidget->setCurrentIndex(0);
    QCOMPARE(testWidget->currentIndex(), 0);
    QCOMPARE(editTextCount, 0);
    QCOMPARE(editText.isEmpty(), true);

    // signal should be sent when changing to other index
    QCOMPARE(testWidget->currentIndex(), 0);
    editTextCount = 0;
    editText.clear();
    testWidget->setCurrentIndex(1);
    QCOMPARE(testWidget->currentIndex(), 1);
    QCOMPARE(editTextCount, 1);
    QCOMPARE(editText, QString("bar"));

    // insert some keys and notice they are all signaled
    editTextCount = 0;
    editText.clear();
    QTest::keyClicks(testWidget, "bingo");
    QCOMPARE(editTextCount, 5);
    QCOMPARE(editText, QString("barbingo"));
}

void tst_QComboBox::onEditTextChanged(const QString &text)
{
    editTextCount++;
    editText = text;
}

void tst_QComboBox::setModel()
{
    QComboBox box;
    QCOMPARE(box.currentIndex(), -1);
    box.addItems((QStringList() << "foo" << "bar"));
    QCOMPARE(box.currentIndex(), 0);
    box.setCurrentIndex(1);
    QCOMPARE(box.currentIndex(), 1);

    // check that currentIndex is set to invalid
    QAbstractItemModel *oldModel = box.model();
    box.setModel(new QStandardItemModel(&box));
    QCOMPARE(box.currentIndex(), -1);
    QVERIFY(box.model() != oldModel);

    // check that currentIndex is set to first item
    oldModel = box.model();
    box.setModel(new QStandardItemModel(2,1, &box));
    QCOMPARE(box.currentIndex(), 0);
    QVERIFY(box.model() != oldModel);
}

void tst_QComboBox::modelDeleted()
{
    QComboBox box;
    QStandardItemModel *model = new QStandardItemModel;
    box.setModel(model);
    QCOMPARE(box.model(), static_cast<QAbstractItemModel *>(model));
    delete model;
    QVERIFY(box.model());
    QCOMPARE(box.findText("bubu"), -1);

    delete box.model();
    QVERIFY(box.model());
    delete box.model();
    QVERIFY(box.model());
}

void tst_QComboBox::setMaxCount()
{
    QStringList items;
    items << "1" << "2" << "3" << "4" << "5";

    QComboBox box;
    box.addItems(items);
    QCOMPARE(box.count(), 5);

    box.setMaxCount(4);
    QCOMPARE(box.count(), 4);
    QCOMPARE(box.itemText(0), QString("1"));
    QCOMPARE(box.itemText(1), QString("2"));
    QCOMPARE(box.itemText(2), QString("3"));
    QCOMPARE(box.itemText(3), QString("4"));

    // appending should do nothing
    box.addItem("foo");
    QCOMPARE(box.count(), 4);
    QCOMPARE(box.findText("foo"), -1);

    // inserting one item at top should remove the last
    box.insertItem(0, "0");
    QCOMPARE(box.count(), 4);
    QCOMPARE(box.itemText(0), QString("0"));
    QCOMPARE(box.itemText(1), QString("1"));
    QCOMPARE(box.itemText(2), QString("2"));
    QCOMPARE(box.itemText(3), QString("3"));

    // insert 5 items in a box with maxCount 4
    box.insertItems(0, items);
    QCOMPARE(box.count(), 4);
    QCOMPARE(box.itemText(0), QString("1"));
    QCOMPARE(box.itemText(1), QString("2"));
    QCOMPARE(box.itemText(2), QString("3"));
    QCOMPARE(box.itemText(3), QString("4"));

    // insert 5 items at pos 2. Make sure only two get inserted
    QSignalSpy spy(box.model(), SIGNAL(rowsInserted(QModelIndex,int,int)));
    box.insertItems(2, items);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.at(0).at(1).toInt(), 2);
    QCOMPARE(spy.at(0).at(2).toInt(), 3);

    QCOMPARE(box.count(), 4);
    QCOMPARE(box.itemText(0), QString("1"));
    QCOMPARE(box.itemText(1), QString("2"));
    QCOMPARE(box.itemText(2), QString("1"));
    QCOMPARE(box.itemText(3), QString("2"));

    box.insertItems(0, QStringList());
    QCOMPARE(box.count(), 4);

    box.setMaxCount(0);
    QCOMPARE(box.count(), 0);
    box.addItem("foo");
    QCOMPARE(box.count(), 0);
    box.addItems(items);
    QCOMPARE(box.count(), 0);
}

void tst_QComboBox::convenienceViews()
{
    // QListWidget
    QComboBox listCombo;
    QListWidget *list = new QListWidget();
    listCombo.setModel(list->model());
    listCombo.setView(list);
    // add items
    list->addItem("list0");
    listCombo.addItem("list1");
    QCOMPARE(listCombo.count(), 2);
    QCOMPARE(listCombo.itemText(0), QString("list0"));
    QCOMPARE(listCombo.itemText(1), QString("list1"));

    // QTreeWidget
    QComboBox treeCombo;
    QTreeWidget *tree = new QTreeWidget();
    tree->setColumnCount(1);
    tree->header()->hide();
    treeCombo.setModel(tree->model());
    treeCombo.setView(tree);
    // add items
    tree->addTopLevelItem(new QTreeWidgetItem(QStringList("tree0")));
    treeCombo.addItem("tree1");
    QCOMPARE(treeCombo.count(), 2);
    QCOMPARE(treeCombo.itemText(0), QString("tree0"));
    QCOMPARE(treeCombo.itemText(1), QString("tree1"));

    // QTableWidget
    QComboBox tableCombo;
    QTableWidget *table = new QTableWidget(0,1);
    table->verticalHeader()->hide();
    table->horizontalHeader()->hide();
    tableCombo.setModel(table->model());
    tableCombo.setView(table);
    // add items
    table->setRowCount(table->rowCount() + 1);
    table->setItem(0, table->rowCount() - 1, new QTableWidgetItem("table0"));
    tableCombo.addItem("table1");
    QCOMPARE(tableCombo.count(), 2);
    QCOMPARE(tableCombo.itemText(0), QString("table0"));
    QCOMPARE(tableCombo.itemText(1), QString("table1"));
}

class ReturnClass : public QWidget
{
    Q_OBJECT
public:
    ReturnClass(QWidget *parent = 0)
        : QWidget(parent), received(false)
    {
        QComboBox *box = new QComboBox(this);
        box->setEditable(true);
        edit = box->lineEdit();
        box->setGeometry(rect());
    }

    void keyPressEvent(QKeyEvent *e)
    {
        received = (e->key() == Qt::Key_Return || e->key() == Qt::Key_Enter);
    }

    QLineEdit *edit;

    bool received;

};



void tst_QComboBox::ensureReturnIsIgnored()
{
    ReturnClass r;
    r.show();

    QTest::keyClick(r.edit, Qt::Key_Return);
    QVERIFY(r.received);
    r.received = false;
    QTest::keyClick(r.edit, Qt::Key_Enter);
    QVERIFY(r.received);
}


void tst_QComboBox::findText_data()
{
    QTest::addColumn<QStringList>("items");
    QTest::addColumn<int>("matchflags");
    QTest::addColumn<QString>("search");
    QTest::addColumn<int>("result");

    QStringList list;
    list << "One" << "Two" << "Three" << "Four" << "Five" << "Six" << "one";
    QTest::newRow("CaseSensitive_1") << list << (int)(Qt::MatchExactly|Qt::MatchCaseSensitive)
                                     << QString("Two") << 1;
    QTest::newRow("CaseSensitive_2") << list << (int)(Qt::MatchExactly|Qt::MatchCaseSensitive)
                                     << QString("two") << -1;
    QTest::newRow("CaseSensitive_3") << list << (int)(Qt::MatchExactly|Qt::MatchCaseSensitive)
                                     << QString("One") << 0;
    QTest::newRow("CaseSensitive_4") << list << (int)(Qt::MatchExactly|Qt::MatchCaseSensitive)
                                     << QString("one") << 6;
    QTest::newRow("CaseInsensitive_1") << list << (int)(Qt::MatchExactly) << QString("Two") << 1;
    QTest::newRow("CaseInsensitive_2") << list << (int)(Qt::MatchExactly) << QString("two") << -1;
    QTest::newRow("CaseInsensitive_3") << list << (int)(Qt::MatchExactly) << QString("One") << 0;
    QTest::newRow("CaseInsensitive_4") << list << (int)(Qt::MatchExactly) << QString("one") << 6;
}
void tst_QComboBox::findText()
{
    QFETCH(QStringList, items);
    QFETCH(int, matchflags);
    QFETCH(QString, search);
    QFETCH(int, result);

    testWidget->clear();
    testWidget->addItems(items);

    QCOMPARE(testWidget->findText(search, (Qt::MatchFlags)matchflags), result);
}

typedef QList<int> IntList;
typedef QList<Qt::Key> KeyList;
Q_DECLARE_METATYPE(IntList)
Q_DECLARE_METATYPE(KeyList)

void tst_QComboBox::flaggedItems_data()
{
    QTest::addColumn<QStringList>("itemList");
    QTest::addColumn<IntList>("deselectFlagList");
    QTest::addColumn<IntList>("disableFlagList");
    QTest::addColumn<KeyList>("keyMovementList");
    QTest::addColumn<bool>("editable");
    QTest::addColumn<int>("expectedIndex");

    for (int editable=0;editable<2;editable++) {
        QString testCase = editable ? "editable:" : "non-editable:";
        QStringList itemList;
        itemList << "One" << "Two" << "Three" << "Four" << "Five" << "Six" << "Seven" << "Eight";
        IntList deselectFlagList;
        IntList disableFlagList;
        KeyList keyMovementList;

        keyMovementList << Qt::Key_Down << Qt::Key_Down << Qt::Key_Down << Qt::Key_Down;
        QTest::newRow(testCase.toAscii() + "normal") << itemList << deselectFlagList << disableFlagList << keyMovementList << bool(editable) << 4;

        deselectFlagList.clear();
        disableFlagList.clear();
        deselectFlagList << 1 << 3;
        QTest::newRow(testCase.toAscii() + "non-selectable") << itemList << deselectFlagList << disableFlagList << keyMovementList << bool(editable) << 4;

        deselectFlagList.clear();
        disableFlagList.clear();
        disableFlagList << 2;
        QTest::newRow(testCase.toAscii() + "disabled") << itemList << deselectFlagList << disableFlagList << keyMovementList << bool(editable) << 5;

        deselectFlagList.clear();
        disableFlagList.clear();
        deselectFlagList << 1 << 3;
        disableFlagList << 2 << 3;
        QTest::newRow(testCase.toAscii() + "mixed") << itemList << deselectFlagList << disableFlagList << keyMovementList << bool(editable) << 6;
        deselectFlagList.clear();
        disableFlagList.clear();
        disableFlagList << 0 << 1 << 2 << 3 << 4 << 5 << 6;
        QTest::newRow(testCase.toAscii() + "nearly-empty") << itemList << deselectFlagList << disableFlagList << keyMovementList << bool(editable) << 7;

        deselectFlagList.clear();
        disableFlagList.clear();
        disableFlagList << 0 << 1 << 2 << 3 << 5 << 6 << 7;
        keyMovementList.clear();
        QTest::newRow(testCase.toAscii() + "only one enabled") << itemList << deselectFlagList << disableFlagList << keyMovementList << bool(editable) << 4;

        if (!editable) {
            deselectFlagList.clear();
            disableFlagList.clear();
            keyMovementList.clear();
            disableFlagList << 0 << 2 << 3;
            keyMovementList << Qt::Key_Down << Qt::Key_Home;
            QTest::newRow(testCase.toAscii() + "home-disabled") << itemList << deselectFlagList << disableFlagList << keyMovementList << bool(editable) << 1;

            keyMovementList.clear();
            keyMovementList << Qt::Key_End;
            QTest::newRow(testCase.toAscii() + "end-key") << itemList << deselectFlagList << disableFlagList << keyMovementList << bool(editable) << 7;

            disableFlagList.clear();
            disableFlagList << 1 ;
            keyMovementList << Qt::Key_T;
            QTest::newRow(testCase.toAscii() + "keyboard-search") << itemList << deselectFlagList << disableFlagList << keyMovementList << bool(editable) << 2;

            itemList << "nine" << "ten";
            keyMovementList << Qt::Key_T;
            QTest::newRow(testCase.toAscii() + "search same start letter") << itemList << deselectFlagList << disableFlagList << keyMovementList << bool(editable) << 9;

            keyMovementList.clear();
            keyMovementList << Qt::Key_T << Qt::Key_H;
            QTest::newRow(testCase.toAscii() + "keyboard search item") << itemList << deselectFlagList << disableFlagList << keyMovementList << bool(editable) << 2;

            disableFlagList.clear();
            disableFlagList << 1 << 3 << 5 << 7 << 9;
            keyMovementList.clear();
            keyMovementList << Qt::Key_End << Qt::Key_Up << Qt::Key_Up << Qt::Key_PageDown << Qt::Key_PageUp << Qt::Key_PageUp << Qt::Key_Down;
            QTest::newRow(testCase.toAscii() + "all key combinations") << itemList << deselectFlagList << disableFlagList << keyMovementList << bool(editable) << 4;
        } else {
            deselectFlagList.clear();
            disableFlagList.clear();
            disableFlagList << 1;
            keyMovementList.clear();
            keyMovementList << Qt::Key_T << Qt::Key_Enter;
            QTest::newRow(testCase.toAscii() + "disabled") << itemList << deselectFlagList << disableFlagList << keyMovementList << bool(editable) << 2;
            QTest::newRow(testCase.toAscii() + "broken autocompletion") << itemList << deselectFlagList << disableFlagList << keyMovementList << bool(editable) << 2;
        }
    }
}

void tst_QComboBox::flaggedItems()
{
    QFETCH(QStringList, itemList);
    QFETCH(IntList, deselectFlagList);
    QFETCH(IntList, disableFlagList);
    QFETCH(KeyList, keyMovementList);
    QFETCH(bool, editable);
    QFETCH(int, expectedIndex);

    QComboBox comboBox;
    QListWidget listWidget;
    listWidget.addItems(itemList);

    comboBox.setEditable(editable);
    foreach (int index, deselectFlagList)
        listWidget.item(index)->setFlags(listWidget.item(index)->flags() & ~Qt::ItemIsSelectable);

    foreach (int index, disableFlagList)
        listWidget.item(index)->setFlags(listWidget.item(index)->flags() & ~Qt::ItemIsEnabled);

    comboBox.setModel(listWidget.model());
    comboBox.setView(&listWidget);
    comboBox.show();
    QApplication::setActiveWindow(&comboBox);
    comboBox.activateWindow();
    comboBox.setFocus();
    QTRY_VERIFY(comboBox.isVisible());
    QTRY_VERIFY(comboBox.hasFocus());

    if (editable)
        comboBox.lineEdit()->selectAll();

    QSignalSpy indexChangedInt(&comboBox, SIGNAL(currentIndexChanged(int)));
    for (int i = 0; i < keyMovementList.count(); ++i) {
        Qt::Key key = keyMovementList[i];
        QTest::keyClick(&comboBox, key);
        if (indexChangedInt.count() != i + 1) {
            QTest::qWait(400);
        }
    }

    QCOMPARE(comboBox.currentIndex() , expectedIndex);
}

void tst_QComboBox::pixmapIcon()
{
    QComboBox box;
    QStandardItemModel *model = new QStandardItemModel(2, 1, &box);

    QPixmap pix(10, 10);
    pix.fill(Qt::red);
    model->setData(model->index(0, 0), "Element 1");
    model->setData(model->index(0, 0), pix, Qt::DecorationRole);

    QIcon icon(pix);
    model->setData(model->index(1, 0), "Element 2");
    model->setData(model->index(1, 0), icon, Qt::DecorationRole);

    box.setModel(model);

    QCOMPARE( box.itemIcon(0).isNull(), false );
    QCOMPARE( box.itemIcon(1).isNull(), false );
}

// defined to be 120 by the wheel mouse vendors according to the docs
#define WHEEL_DELTA 120

void tst_QComboBox::mouseWheel_data()
{
    QTest::addColumn<IntList>("disabledItems");
    QTest::addColumn<int>("startIndex");
    QTest::addColumn<int>("wheelDirection");
    QTest::addColumn<int>("expectedIndex");

    IntList disabled;
    disabled << 0 << 1 << 2 << 4;
    int start = 3;
    int wheel = 1;
    int expected = 3;
    QTest::newRow("upper locked") << disabled << start << wheel << expected;

    wheel = -1;
    expected = 5;
    QTest::newRow("jump over") << disabled << start << wheel << expected;

    disabled.clear();
    disabled << 1 << 2 << 3 << 4 << 5 << 6 << 7 << 8 << 9;
    start = 0;
    wheel = -1;
    expected = 0;
    QTest::newRow("single Item enabled") << disabled << start << wheel << expected;
}

void tst_QComboBox::mouseWheel()
{
    QFETCH(IntList, disabledItems);
    QFETCH(int, startIndex);
    QFETCH(int, wheelDirection);
    QFETCH(int, expectedIndex);

    QCoreApplication *applicationInstance = QCoreApplication::instance();
    QVERIFY(applicationInstance != 0);

    QComboBox box;
    QStringList list;
    list << "one" << "two" << "three" << "four" << "five" << "six" << "seven" << "eight" << "nine" << "ten";

    QListWidget listWidget;
    listWidget.addItems(list);

    foreach (int index, disabledItems)
        listWidget.item(index)->setFlags(listWidget.item(index)->flags() & ~Qt::ItemIsEnabled);

    box.setModel(listWidget.model());
    box.setView(&listWidget);
    for (int i=0; i < 2; ++i) {
        box.setEditable(i==0?false:true);
        box.setCurrentIndex(startIndex);

        QWheelEvent event = QWheelEvent(box.rect().bottomRight() , WHEEL_DELTA * wheelDirection, Qt::NoButton, Qt::NoModifier);
        QVERIFY(applicationInstance->sendEvent(&box,&event));

        QCOMPARE(box.currentIndex(), expectedIndex);
    }
}

void tst_QComboBox::layoutDirection()
{
    QComboBox box;
    Qt::LayoutDirection dir;
    QLineEdit *lineEdit;

    // RTL
    box.setLayoutDirection(Qt::RightToLeft);
    QStyleOptionComboBox opt;
    opt.direction = Qt::RightToLeft;
    dir = (Qt::LayoutDirection)box.style()->styleHint(QStyle::SH_ComboBox_LayoutDirection, &opt, &box);

    QCOMPARE(box.view()->layoutDirection(), dir);
    box.setEditable(true);
    QCOMPARE(box.lineEdit()->layoutDirection(), dir);
    lineEdit = new QLineEdit;
    QCOMPARE(lineEdit->layoutDirection(), qApp->layoutDirection());
    box.setLineEdit(lineEdit);
    QCOMPARE(lineEdit->layoutDirection(), dir);

    // LTR
    box.setLayoutDirection(Qt::LeftToRight);
    qApp->setLayoutDirection(Qt::RightToLeft);

    opt.direction = Qt::LeftToRight;
    dir = (Qt::LayoutDirection)box.style()->styleHint(QStyle::SH_ComboBox_LayoutDirection, &opt, &box);

    QCOMPARE(box.view()->layoutDirection(), dir);
    box.setEditable(true);
    QCOMPARE(box.lineEdit()->layoutDirection(), dir);
    lineEdit = new QLineEdit;
    QCOMPARE(lineEdit->layoutDirection(), qApp->layoutDirection());
    box.setLineEdit(lineEdit);
    QCOMPARE(lineEdit->layoutDirection(), dir);

}

void tst_QComboBox::itemListPosition()
{
    //tests that the list is not out of the screen boundaries

    //put the QApplication layout back
    QApplication::setLayoutDirection(Qt::LeftToRight);

    //we test QFontComboBox because it has the specific behaviour to set a fixed size
    //to the list view
    QFontComboBox combo;

    //the code to get the avaialbe screen space is copied from QComboBox code
    const int scrNumber = QApplication::desktop()->screenNumber(&combo);
    QRect screen;
#ifdef Q_WS_WIN
    screen = QApplication::desktop()->screenGeometry(scrNumber);
#elif defined Q_WS_X11
    if (X11->desktopEnvironment == DE_KDE)
        screen = QApplication::desktop()->screenGeometry(scrNumber);
    else
        screen = QApplication::desktop()->availableGeometry(scrNumber);
#else
    screen = QApplication::desktop()->availableGeometry(scrNumber);
#endif

    combo.move(screen.width()-combo.sizeHint().width(), 0); //puts the combo to the top-right corner

    combo.show();
    //wait because the window manager can move the window if there is a right panel
    QTRY_VERIFY(combo.isVisible());
    combo.showPopup();
    QTRY_VERIFY(combo.view());
    QTRY_VERIFY(combo.view()->isVisible());

#if defined(Q_WS_S60)
    // Assuming that QtS60 style is used, here. But other ones would certainly also fail
    QEXPECT_FAIL("", "QtS60Style does not yet position the combobox popup correctly", Continue);
#endif
    QVERIFY( combo.view()->window()->x() + combo.view()->window()->width() <= screen.x() + screen.width() );

}

void tst_QComboBox::separatorItem_data()
{
    QTest::addColumn<QStringList>("items");
    QTest::addColumn<IntList>("separators");

    QTest::newRow("test") << (QStringList() << "one" << "two" << "three" << "other...")
                          << (IntList() << 4);
}

void tst_QComboBox::separatorItem()
{
    QFETCH(QStringList, items);
    QFETCH(IntList, separators);

    QComboBox box;
    box.addItems(items);
    foreach(int index, separators)
        box.insertSeparator(index);
    QCOMPARE(box.count(), (items.count() + separators.count()));
    for (int i = 0, s = 0; i < box.count(); ++i) {
        if (i == separators.at(s)) {
            QCOMPARE(box.itemText(i), QString());
            ++s;
        } else {
            QCOMPARE(box.itemText(i), items.at(i - s));
        }
    }
}

void tst_QComboBox::task190351_layout()
{
#ifndef QT_NO_STYLE_CLEANLOOKS
    const QString oldStyle = QApplication::style()->objectName();
    QApplication::setStyle(new QCleanlooksStyle);

    QComboBox listCombo;
    QListWidget *list = new QListWidget();
    listCombo.setModel(list->model());
    listCombo.setView(list);
    for(int i = 1; i < 150; i++)
        list->addItem(QLatin1String("list") + QString::number(i));

    listCombo.show();
    QTest::qWaitForWindowShown(&listCombo);
    QTRY_VERIFY(listCombo.isVisible());
    listCombo.setCurrentIndex(70);
    listCombo.showPopup();
    QTRY_VERIFY(listCombo.view());
    QTest::qWaitForWindowShown(listCombo.view());
    QTRY_VERIFY(listCombo.view()->isVisible());
    QApplication::processEvents();

#ifdef QT_BUILD_INTERNAL
    QFrame *container = qFindChild<QComboBoxPrivateContainer *>(&listCombo);
    QVERIFY(container);
    QCOMPARE(static_cast<QAbstractItemView *>(list), qFindChild<QAbstractItemView *>(container));
    QWidget *top = qFindChild<QComboBoxPrivateScroller *>(container);
    QVERIFY(top);
    QVERIFY(top->isVisible());
    QCOMPARE(top->mapToGlobal(QPoint(0, top->height())).y(), list->mapToGlobal(QPoint()).y());
#endif

    QApplication::setStyle(oldStyle);
#else
    QSKIP("Qt configured without cleanlooks style", SkipAll);
#endif
}

class task166349_ComboBox : public QComboBox
{
    Q_OBJECT
public:
    task166349_ComboBox(QWidget *parent = 0) : QComboBox(parent)
    {
        QStringList list;
        list << "one" << "two";
        connect(this, SIGNAL(currentIndexChanged(int)), this, SLOT(onCurrentIndexChanged(int)));
        addItems(list);
    }
public slots:
    void onCurrentIndexChanged(int index)
    {
        setEditable(index % 2 == 1);
    }
};

void tst_QComboBox::task166349_setEditableOnReturn()
{
    task166349_ComboBox comboBox;
    QTest::keyClick(&comboBox, Qt::Key_Down);
    QTest::keyClick(&comboBox, Qt::Key_1);
    QTest::keyClick(&comboBox, Qt::Key_Enter);
    QCOMPARE(QLatin1String("two1"), comboBox.itemText(comboBox.count() - 1));
}

void tst_QComboBox::task191329_size()
{
#ifndef QT_NO_STYLE_CLEANLOOKS
    const QString oldStyle = QApplication::style()->objectName();
    QApplication::setStyle(new QCleanlooksStyle);

    QComboBox tableCombo;
    int rows;
    if (QApplication::desktop()->screenGeometry().height() < 480)
        rows = 8;
    else
        rows = 15;

    QStandardItemModel model(rows, 2);
    for (int row = 0; row < model.rowCount(); ++row) {
        for (int column = 0; column < model.columnCount(); ++column) {
            QStandardItem *item = new QStandardItem(QString("row %0, column %1").arg(row).arg(column));
            model.setItem(row, column, item);
        }
    }
    QTableView *table = new QTableView();
    table->verticalHeader()->hide();
    table->horizontalHeader()->hide();
    tableCombo.setView(table);
    tableCombo.setModel(&model);

    tableCombo.show();
    QTRY_VERIFY(tableCombo.isVisible());
    tableCombo.showPopup();
    QTRY_VERIFY(tableCombo.view());
    QTRY_VERIFY(tableCombo.view()->isVisible());

#ifdef QT_BUILD_INTERNAL
    QFrame *container = qFindChild<QComboBoxPrivateContainer *>(&tableCombo);
    QVERIFY(container);
    QCOMPARE(static_cast<QAbstractItemView *>(table), qFindChild<QAbstractItemView *>(container));
    foreach (QWidget *button, qFindChildren<QComboBoxPrivateScroller *>(container)) {
        //the popup should be large enough to contains everithing so the top and left button are hidden
        QVERIFY(!button->isVisible());
    }
#endif

    QApplication::setStyle(oldStyle);
#else
    QSKIP("Qt configured without cleanlooks style", SkipAll);
#endif
}

void tst_QComboBox::task190205_setModelAdjustToContents()
{
    QStringList initialContent;
    QStringList finalContent;
    initialContent << "foo" << "bar";
    finalContent << "bar" << "foooooooobar";

    QComboBox box;
    box.setSizeAdjustPolicy(QComboBox::AdjustToContents);
    box.addItems(initialContent);
    box.show();

    //wait needed in order to get the combo initial size
    QTRY_VERIFY(box.isVisible());

    box.setModel(new QStringListModel(finalContent));

    QComboBox correctBox;
    correctBox.addItems(finalContent);
    correctBox.show();

    QCoreApplication::processEvents();
#ifdef Q_WS_X11
    qt_x11_wait_for_window_manager(&box);
    qt_x11_wait_for_window_manager(&correctBox);
#endif

    // box should be resized to the same size as correctBox
    QTRY_COMPARE(box.size(), correctBox.size());
}

void tst_QComboBox::task248169_popupWithMinimalSize()
{
    QStringList initialContent;
    initialContent << "foo" << "bar" << "foobar";

    QComboBox comboBox;
    comboBox.addItems(initialContent);
    QDesktopWidget desktop;
    QRect desktopSize = desktop.availableGeometry();
    comboBox.view()->setMinimumWidth(desktopSize.width() / 2);

    comboBox.setGeometry(desktopSize.width() - (desktopSize.width() / 4), (desktopSize.width() / 4), (desktopSize.width() / 2), (desktopSize.width() / 4));

    comboBox.show();
    QTest::qWaitForWindowShown(&comboBox);
    QTRY_VERIFY(comboBox.isVisible());
    comboBox.showPopup();
    QTRY_VERIFY(comboBox.view());
    QTest::qWaitForWindowShown(comboBox.view());
    QTRY_VERIFY(comboBox.view()->isVisible());

#ifdef QT_BUILD_INTERNAL
    QFrame *container = qFindChild<QComboBoxPrivateContainer *>(&comboBox);
    QVERIFY(container);
    QTRY_VERIFY(desktop.screenGeometry(container).contains(container->geometry()));
#endif
}

void tst_QComboBox::task247863_keyBoardSelection()
{
  QComboBox combo;
  combo.setEditable(false);
  combo.addItem( QLatin1String("111"));
  combo.addItem( QLatin1String("222"));
  combo.show();
  QApplication::setActiveWindow(&combo);
  QTRY_COMPARE(QApplication::activeWindow(), static_cast<QWidget *>(&combo));

  QSignalSpy spy(&combo, SIGNAL(activated(const QString &)));
  qApp->setEffectEnabled(Qt::UI_AnimateCombo, false);
  QTest::keyClick(&combo, Qt::Key_Space);
  qApp->setEffectEnabled(Qt::UI_AnimateCombo, true);
  QTest::keyClick(0, Qt::Key_Down);
  QTest::keyClick(0, Qt::Key_Enter);
  QCOMPARE(combo.currentText(), QLatin1String("222"));
  QCOMPARE(spy.count(), 1);
}

void tst_QComboBox::task220195_keyBoardSelection2()
{
    QComboBox combo;
    combo.setEditable(false);
    combo.addItem( QLatin1String("foo1"));
    combo.addItem( QLatin1String("foo2"));
    combo.addItem( QLatin1String("foo3"));
    combo.show();
    QApplication::setActiveWindow(&combo);
    QTRY_COMPARE(QApplication::activeWindow(), static_cast<QWidget *>(&combo));

    combo.setCurrentIndex(-1);
    QVERIFY(combo.currentText().isNull());

    QTest::keyClick(&combo, 'f');
    QCOMPARE(combo.currentText(), QLatin1String("foo1"));
    QTest::qWait(QApplication::keyboardInputInterval() + 30);
    QTest::keyClick(&combo, 'f');
    QCOMPARE(combo.currentText(), QLatin1String("foo2"));
    QTest::qWait(QApplication::keyboardInputInterval() + 30);
    QTest::keyClick(&combo, 'f');
    QCOMPARE(combo.currentText(), QLatin1String("foo3"));
    QTest::qWait(QApplication::keyboardInputInterval() + 30);
    QTest::keyClick(&combo, 'f');
    QCOMPARE(combo.currentText(), QLatin1String("foo1"));
    QTest::qWait(QApplication::keyboardInputInterval() + 30);

    combo.setCurrentIndex(1);
    QCOMPARE(combo.currentText(), QLatin1String("foo2"));
    QTest::keyClick(&combo, 'f');
    QCOMPARE(combo.currentText(), QLatin1String("foo3"));
}


void tst_QComboBox::setModelColumn()
{
    QStandardItemModel model(5,3);
    model.setItem(0,0, new QStandardItem("0"));
    model.setItem(1,0, new QStandardItem("1"));
    model.setItem(2,0, new QStandardItem("2"));
    model.setItem(3,0, new QStandardItem("3"));
    model.setItem(4,0, new QStandardItem("4"));
    model.setItem(0,1, new QStandardItem("zero"));
    model.setItem(1,1, new QStandardItem("un"));
    model.setItem(2,1, new QStandardItem("deux"));
    model.setItem(3,1, new QStandardItem("trois"));
    model.setItem(4,1, new QStandardItem("quatre"));
    model.setItem(0,2, new QStandardItem("a"));
    model.setItem(1,2, new QStandardItem("b"));
    model.setItem(2,2, new QStandardItem("c"));
    model.setItem(3,2, new QStandardItem("d"));
    model.setItem(4,2, new QStandardItem("e"));

    QComboBox box;
    box.setModel(&model);
    QCOMPARE(box.currentText(), QString("0"));
    box.setModelColumn(1);
    QCOMPARE(box.currentText(), QString("zero"));
}

void tst_QComboBox::noScrollbar_data()
{
    QTest::addColumn<QString>("stylesheet");

    QTest::newRow("normal") << QString();
    QTest::newRow("border") << QString::fromLatin1("QAbstractItemView { border: 12px solid blue;}");
    QTest::newRow("margin") << QString::fromLatin1("QAbstractItemView { margin: 12px 15px 13px 10px; }");
    QTest::newRow("padding") << QString::fromLatin1("QAbstractItemView { padding: 12px 15px 13px 10px;}");
    QTest::newRow("everything") << QString::fromLatin1("QAbstractItemView {  border: 12px  solid blue; "
                                                       " padding: 12px 15px 13px 10px; margin: 12px 15px 13px 10px;  }");
    QTest::newRow("everything and more") << QString::fromLatin1("QAbstractItemView {  border: 1px 3px 5px 1px solid blue; "
                                                       " padding: 2px 5px 3px 1px; margin: 2px 5px 3px 1px;  } "
                                                       " QAbstractItemView::item {  border: 2px solid green; "
                                                       "                      padding: 1px 1px 2px 2px; margin: 1px; } " );
}

void tst_QComboBox::noScrollbar()
{
    QStringList initialContent;
    initialContent << "foo" << "bar" << "foobar" << "moo";
    QFETCH(QString, stylesheet);
    QString oldCss = qApp->styleSheet();
    qApp->setStyleSheet(stylesheet);

    {
        QComboBox comboBox;
        comboBox.addItems(initialContent);
        comboBox.show();
        comboBox.resize(200, comboBox.height());
        QTRY_VERIFY(comboBox.isVisible());
        comboBox.showPopup();
        QTRY_VERIFY(comboBox.view());
        QTRY_VERIFY(comboBox.view()->isVisible());

        QVERIFY(!comboBox.view()->horizontalScrollBar()->isVisible());
        QVERIFY(!comboBox.view()->verticalScrollBar()->isVisible());
    }

    {
        QTableWidget *table = new QTableWidget(2,2);
        QComboBox comboBox;
        comboBox.setModel(table->model());
        comboBox.setView(table);
        comboBox.show();
        QTRY_VERIFY(comboBox.isVisible());
        comboBox.resize(200, comboBox.height());
        comboBox.showPopup();
        QTRY_VERIFY(comboBox.view());
        QTRY_VERIFY(comboBox.view()->isVisible());

        QVERIFY(!comboBox.view()->horizontalScrollBar()->isVisible());
        QVERIFY(!comboBox.view()->verticalScrollBar()->isVisible());
    }

    qApp->setStyleSheet(oldCss);
}

void tst_QComboBox::setItemDelegate()
{
    QComboBox comboBox;
    QStyledItemDelegate *itemDelegate = new QStyledItemDelegate;
    comboBox.setItemDelegate(itemDelegate);
    // the cast is a workaround for the XLC and Metrowerks compilers
    QCOMPARE(static_cast<QStyledItemDelegate *>(comboBox.itemDelegate()), itemDelegate);
}

void tst_QComboBox::task253944_itemDelegateIsReset()
{
    QComboBox comboBox;
    QStyledItemDelegate *itemDelegate = new QStyledItemDelegate;
    comboBox.setItemDelegate(itemDelegate);

    // the casts are workarounds for the XLC and Metrowerks compilers

    comboBox.setEditable(true);
    QCOMPARE(static_cast<QStyledItemDelegate *>(comboBox.itemDelegate()), itemDelegate);

    comboBox.setStyleSheet("QComboBox { border: 1px solid gray; }");
    QCOMPARE(static_cast<QStyledItemDelegate *>(comboBox.itemDelegate()), itemDelegate);
}


void tst_QComboBox::subControlRectsWithOffset_data()
{
    QTest::addColumn<bool>("editable");

    QTest::newRow("editable = true") << true;
    QTest::newRow("editable = false") << false;
}

void tst_QComboBox::subControlRectsWithOffset()
{
    // The sub control rect relative position should not depends
    // on the position of the combobox

    class FriendlyCombo : public QComboBox {
    public:
        void styleOption(QStyleOptionComboBox *optCombo) {
            initStyleOption(optCombo);
        }
    } combo;
    QStyleOptionComboBox optCombo;
    combo.styleOption(&optCombo);


    const QRect rectAtOrigin(0, 0, 80, 30);
    const QPoint offset(25, 50);
    const QRect rectWithOffset = rectAtOrigin.translated(offset);

    QStyle *style = combo.style();

    QFETCH(bool, editable);
    optCombo.editable = editable;

    optCombo.rect = rectAtOrigin;
    QRect editFieldRect = style->subControlRect(QStyle::CC_ComboBox, &optCombo, QStyle::SC_ComboBoxEditField, 0);
    QRect arrowRect = style->subControlRect(QStyle::CC_ComboBox, &optCombo, QStyle::SC_ComboBoxArrow, 0);
    QRect listboxRect = style->subControlRect(QStyle::CC_ComboBox, &optCombo, QStyle::SC_ComboBoxListBoxPopup, 0);

    optCombo.rect = rectWithOffset;
    QRect editFieldRectWithOffset = style->subControlRect(QStyle::CC_ComboBox, &optCombo, QStyle::SC_ComboBoxEditField, 0);
    QRect arrowRectWithOffset = style->subControlRect(QStyle::CC_ComboBox, &optCombo, QStyle::SC_ComboBoxArrow, 0);
    QRect listboxRectWithOffset = style->subControlRect(QStyle::CC_ComboBox, &optCombo, QStyle::SC_ComboBoxListBoxPopup, 0);

    QCOMPARE(editFieldRect, editFieldRectWithOffset.translated(-offset));
    QCOMPARE(arrowRect, arrowRectWithOffset.translated(-offset));
    QCOMPARE(listboxRect, listboxRectWithOffset.translated(-offset));

}

void tst_QComboBox::task260974_menuItemRectangleForComboBoxPopup()
{
#ifdef QT_NO_STYLE_WINDOWS
    QSKIP("test depends on windows style", QTest::SkipAll);
#else
    class TestStyle: public QWindowsStyle
    {
    public:
        int styleHint(StyleHint hint, const QStyleOption *option, const QWidget *widget, QStyleHintReturn *ret) const
        {
            if (hint == SH_ComboBox_Popup) return 1;
            else return QCommonStyle::styleHint(hint, option, widget, ret);
        }

        void drawControl(ControlElement element, const QStyleOption *option, QPainter *, const QWidget *) const
        {
            if (element == CE_MenuItem)
                discoveredRect = option->rect;
        }

        mutable QRect discoveredRect;
    } style;


    {
        QComboBox comboBox;
        comboBox.setStyle(&style);
        comboBox.addItem("Item 1");

        comboBox.show();
        QTRY_VERIFY(comboBox.isVisible());
        comboBox.showPopup();
        QTRY_VERIFY(comboBox.view());
        QTRY_VERIFY(comboBox.view()->isVisible());

        QTRY_VERIFY(style.discoveredRect.width() <= comboBox.width());
    }
#endif
}

void tst_QComboBox::removeItem()
{
    QComboBox cb;
    cb.removeItem(-1);
    cb.removeItem(1);
    cb.removeItem(0);
    QCOMPARE(cb.count(), 0);

    cb.addItem("foo");
    cb.removeItem(-1);
    QCOMPARE(cb.count(), 1);
    cb.removeItem(1);
    QCOMPARE(cb.count(), 1);
    cb.removeItem(0);
    QCOMPARE(cb.count(), 0);
}

void tst_QComboBox::resetModel()
{
    class StringListModel : public QStringListModel
    {
    public:
        StringListModel(const QStringList &list) : QStringListModel(list)
        {
        }

        void reset()
        {
            QStringListModel::reset();
        }
    };
    QComboBox cb;
    StringListModel model( QStringList() << "1" << "2");
    QSignalSpy spy(&cb, SIGNAL(currentIndexChanged(int)));
    QCOMPARE(spy.count(), 0);
    QCOMPARE(cb.currentIndex(), -1); //no selection

    cb.setModel(&model);

    QCOMPARE(spy.count(), 1);
    QCOMPARE(cb.currentIndex(), 0); //first item selected

    model.reset();
    QCOMPARE(spy.count(), 2);
    QCOMPARE(cb.currentIndex(), -1); //no selection

}

void tst_QComboBox::keyBoardNavigationWithMouse()
{
    QComboBox combo;
    combo.setEditable(false);
    for (int i = 0; i < 80; i++)
        combo.addItem( QString::number(i));
    combo.show();
    QApplication::setActiveWindow(&combo);
    QTest::qWaitForWindowShown(&combo);
    QTRY_COMPARE(QApplication::activeWindow(), static_cast<QWidget *>(&combo));

    QCOMPARE(combo.currentText(), QLatin1String("0"));

    combo.setFocus();
    QTRY_VERIFY(combo.hasFocus());

    QTest::keyClick(testWidget->lineEdit(), Qt::Key_Space);
    QTest::qWait(30);
    QTRY_VERIFY(combo.view());
    QTRY_VERIFY(combo.view()->isVisible());
    QTest::qWait(130);

    QCOMPARE(combo.currentText(), QLatin1String("0"));

#ifdef Q_OS_WINCE
    QSKIP("When calling cursor function, Windows CE responds with: This function is not supported on this system.", SkipAll);
#endif

    QCursor::setPos(combo.view()->mapToGlobal(combo.view()->rect().center()));
    QTest::qWait(200);

#define GET_SELECTION(SEL) \
    QCOMPARE(combo.view()->selectionModel()->selection().count(), 1); \
    QCOMPARE(combo.view()->selectionModel()->selection().indexes().count(), 1); \
    SEL = combo.view()->selectionModel()->selection().indexes().first().row()

    int selection;
    GET_SELECTION(selection);

    //since we moved the mouse is in the middle it should even be around 5;
    QVERIFY(selection > 3);

    static const int final = 40;
    for (int i = selection + 1;  i <= final; i++)
    {
        QTest::keyClick(combo.view(), Qt::Key_Down);
        QTest::qWait(20);
        GET_SELECTION(selection);
        QCOMPARE(selection, i);
    }

    QTest::keyClick(combo.view(), Qt::Key_Enter);
    QTRY_COMPARE(combo.currentText(), QString::number(final));
}

void tst_QComboBox::task_QTBUG_1071_changingFocusEmitsActivated()
{
    QWidget w;
    QVBoxLayout layout(&w);
    QComboBox cb;
    cb.setEditable(true);
    QSignalSpy spy(&cb, SIGNAL(activated(int)));
    cb.addItem("0");
    cb.addItem("1");
    cb.addItem("2");
    QLineEdit edit;
    layout.addWidget(&cb);
    layout.addWidget(&edit);

    w.show();
    QApplication::setActiveWindow(&w);
    QTest::qWaitForWindowShown(&w);
    cb.clearEditText();
    cb.setFocus();
    QApplication::processEvents();
    QTRY_VERIFY(cb.hasFocus());
    QTest::keyClick(0, '1');
    QCOMPARE(spy.count(), 0);
    edit.setFocus();
    QTRY_VERIFY(edit.hasFocus());
    QTRY_COMPARE(spy.count(), 1);
}

void tst_QComboBox::maxVisibleItems()
{
    QComboBox comboBox;
    QCOMPARE(comboBox.maxVisibleItems(), 10); //default value.

    QStringList content;
    for(int i = 1; i < 50; i++)
        content += QString::number(i);

    comboBox.addItems(content);
    comboBox.show();
    comboBox.resize(200, comboBox.height());
    QTRY_VERIFY(comboBox.isVisible());

    comboBox.setMaxVisibleItems(5);
    QCOMPARE(comboBox.maxVisibleItems(), 5);

    comboBox.showPopup();
    QTRY_VERIFY(comboBox.view());
    QTRY_VERIFY(comboBox.view()->isVisible());

    QAbstractItemView *v = comboBox.view();
    int itemHeight = v->visualRect(v->model()->index(0,0)).height();
    if (v->style()->styleHint(QStyle::SH_ComboBox_Popup))
        QCOMPARE(v->viewport()->height(), itemHeight * comboBox.maxVisibleItems());
    // QCombobox without a popup does not work, see QTBUG-760
}


QTEST_MAIN(tst_QComboBox)
#include "tst_qcombobox.moc"
