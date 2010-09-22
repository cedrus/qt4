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
#include <QStandardItemModel>
#include <QStringListModel>
#include <QSortFilterProxyModel>
#include <QTableView>

#include <qabstractitemmodel.h>
#include <qapplication.h>
#include <qheaderview.h>
#include <private/qheaderview_p.h>
#include <qitemdelegate.h>
#include <qtreewidget.h>
#include <qdebug.h>

typedef QList<int> IntList;
Q_DECLARE_METATYPE(IntList)

typedef QList<bool> BoolList;
Q_DECLARE_METATYPE(BoolList)

//TESTED_CLASS=
//TESTED_FILES=

// Will try to wait for the condition while allowing event processing
// for a maximum of 2 seconds.
#define WAIT_FOR_CONDITION(expr, expected) \
    do { \
        const int step = 100; \
        for (int i = 0; i < 2000 && expr != expected; i+=step) { \
            QTest::qWait(step); \
        } \
    } while(0)

class protected_QHeaderView : public QHeaderView
{
    Q_OBJECT
public:
    protected_QHeaderView(Qt::Orientation orientation) : QHeaderView(orientation) {
        resizeSections();
    };

    void testEvent();
    void testhorizontalOffset();
    void testverticalOffset();
    friend class tst_QHeaderView;
};

class tst_QHeaderView : public QObject
{
    Q_OBJECT

public:
    tst_QHeaderView();
    virtual ~tst_QHeaderView();

public slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

private slots:
    void getSetCheck();
    void visualIndex();

    void visualIndexAt_data();
    void visualIndexAt();

    void noModel();
    void emptyModel();
    void removeRows();
    void removeCols();

    void clickable();
    void movable();
    void hidden();
    void stretch();

    void sectionSize_data();
    void sectionSize();

    void length();
    void offset();
    void sectionSizeHint();
    void logicalIndex();
    void logicalIndexAt();
    void swapSections();

    void moveSection_data();
    void moveSection();

    void resizeMode();

    void resizeSection_data();
    void resizeSection();

    void resizeAndMoveSection_data();
    void resizeAndMoveSection();
    void resizeHiddenSection_data();
    void resizeHiddenSection();
    void resizeAndInsertSection_data();
    void resizeAndInsertSection();
    void resizeWithResizeModes_data();
    void resizeWithResizeModes();
    void moveAndInsertSection_data();
    void moveAndInsertSection();
    void highlightSections();
    void showSortIndicator();
    void sortIndicatorTracking();
    void removeAndInsertRow();
    void unhideSection();
    void event();
    void headerDataChanged();
    void currentChanged();
    void horizontalOffset();
    void verticalOffset();
    void stretchSectionCount();
    void hiddenSectionCount();
    void focusPolicy();
    void moveSectionAndReset();
    void moveSectionAndRemove();
    void saveRestore();

    void defaultAlignment_data();
    void defaultAlignment();

    void globalResizeMode_data();
    void globalResizeMode();

    void sectionPressedSignal_data();
    void sectionPressedSignal();
    void sectionClickedSignal_data() { sectionPressedSignal_data(); }
    void sectionClickedSignal();

    void defaultSectionSize_data();
    void defaultSectionSize();

    void oneSectionSize();

    void hideAndInsert_data();
    void hideAndInsert();

    void removeSection();
    void preserveHiddenSectionWidth();
    void invisibleStretchLastSection();

    void emptySectionSpan();
    void task236450_hidden_data();
    void task236450_hidden();
    void task248050_hideRow();
    void QTBUG6058_reset();
    void QTBUG7833_sectionClicked();
    void QTBUG8650_crashOnInsertSections();
    void QTBUG12268_hiddenMovedSectionSorting();

protected:
    QHeaderView *view;
    QStandardItemModel *model;
};

class QtTestModel: public QAbstractTableModel
{

Q_OBJECT

public:
    QtTestModel(QObject *parent = 0): QAbstractTableModel(parent),
       cols(0), rows(0), wrongIndex(false) {}
    int rowCount(const QModelIndex&) const { return rows; }
    int columnCount(const QModelIndex&) const { return cols; }
    bool isEditable(const QModelIndex &) const { return true; }

    QVariant data(const QModelIndex &idx, int) const
    {
        if (idx.row() < 0 || idx.column() < 0 || idx.column() >= cols || idx.row() >= rows) {
            wrongIndex = true;
            qWarning("Invalid modelIndex [%d,%d,%p]", idx.row(), idx.column(), idx.internalPointer());
        }
        return QString("[%1,%2,%3]").arg(idx.row()).arg(idx.column()).arg(0);//idx.data());
    }

    void insertOneColumn(int col)
    {
        beginInsertColumns(QModelIndex(), col, col);
        --cols;
        endInsertColumns();
    }

    void removeLastRow()
    {
        beginRemoveRows(QModelIndex(), rows - 1, rows - 1);
        --rows;
        endRemoveRows();
    }

    void removeAllRows()
    {
        beginRemoveRows(QModelIndex(), 0, rows - 1);
        rows = 0;
        endRemoveRows();
    }

    void removeOneColumn(int col)
    {
        beginRemoveColumns(QModelIndex(), col, col);
        --cols;
        endRemoveColumns();
    }

    void removeLastColumn()
    {
        beginRemoveColumns(QModelIndex(), cols - 1, cols - 1);
        --cols;
        endRemoveColumns();
    }

    void removeAllColumns()
    {
        beginRemoveColumns(QModelIndex(), 0, cols - 1);
        cols = 0;
        endRemoveColumns();
    }

    void cleanup()
    {
        cols = 3;
        rows = 3;
        emit layoutChanged();
    }

    int cols, rows;
    mutable bool wrongIndex;
};

// Testing get/set functions
void tst_QHeaderView::getSetCheck()
{
    protected_QHeaderView obj1(Qt::Horizontal);
    // bool QHeaderView::highlightSections()
    // void QHeaderView::setHighlightSections(bool)
    obj1.setHighlightSections(false);
    QCOMPARE(false, obj1.highlightSections());
    obj1.setHighlightSections(true);
    QCOMPARE(true, obj1.highlightSections());

    // bool QHeaderView::stretchLastSection()
    // void QHeaderView::setStretchLastSection(bool)
    obj1.setStretchLastSection(false);
    QCOMPARE(false, obj1.stretchLastSection());
    obj1.setStretchLastSection(true);
    QCOMPARE(true, obj1.stretchLastSection());

    // int QHeaderView::defaultSectionSize()
    // void QHeaderView::setDefaultSectionSize(int)
    obj1.setDefaultSectionSize(0);
    QCOMPARE(0, obj1.defaultSectionSize());
    obj1.setDefaultSectionSize(INT_MIN);
    QCOMPARE(INT_MIN, obj1.defaultSectionSize());
    obj1.setDefaultSectionSize(INT_MAX);
    QCOMPARE(INT_MAX, obj1.defaultSectionSize());
    // ### the test above does not make sense for values below 0

    // int QHeaderView::minimumSectionSize()
    // void QHeaderView::setMinimumSectionSize(int)
    obj1.setMinimumSectionSize(0);
    QCOMPARE(0, obj1.minimumSectionSize());
    obj1.setMinimumSectionSize(INT_MIN);
    QCOMPARE(INT_MIN, obj1.minimumSectionSize());
    obj1.setMinimumSectionSize(INT_MAX);
    QCOMPARE(INT_MAX, obj1.minimumSectionSize());
    // ### the test above does not make sense for values below 0

    // int QHeaderView::offset()
    // void QHeaderView::setOffset(int)
    obj1.setOffset(0);
    QCOMPARE(0, obj1.offset());
    obj1.setOffset(INT_MIN);
    QCOMPARE(INT_MIN, obj1.offset());
    obj1.setOffset(INT_MAX);
    QCOMPARE(INT_MAX, obj1.offset());

}

tst_QHeaderView::tst_QHeaderView()
{
    qRegisterMetaType<int>("Qt::SortOrder");
}

tst_QHeaderView::~tst_QHeaderView()
{
}

void tst_QHeaderView::initTestCase()
{
#ifdef Q_OS_WINCE //disable magic for WindowsCE
    qApp->setAutoMaximizeThreshold(-1);
#endif
}

void tst_QHeaderView::cleanupTestCase()
{
}

void tst_QHeaderView::init()
{
    view = new QHeaderView(Qt::Vertical);
    // Some initial value tests before a model is added
    QCOMPARE(view->length(), 0);
    QVERIFY(view->sizeHint() == QSize(0,0));
    QCOMPARE(view->sectionSizeHint(0), -1);

    /*
    model = new QStandardItemModel(1, 1);
    view->setModel(model);
    //qDebug() << view->count();
    view->sizeHint();
    */

    int rows = 4;
    int columns = 4;
    model = new QStandardItemModel(rows, columns);
    /*
    for (int row = 0; row < rows; ++row) {
        for (int column = 0; column < columns; ++column) {
            QModelIndex index = model->index(row, column, QModelIndex());
            model->setData(index, QVariant((row+1) * (column+1)));
        }
    }
    */

    QSignalSpy spy(view, SIGNAL(sectionCountChanged(int, int)));
    view->setModel(model);
    QCOMPARE(spy.count(), 1);
    view->show();
}

void tst_QHeaderView::cleanup()
{
    delete view;
    view = 0;
    delete model;
    model = 0;
}

void tst_QHeaderView::noModel()
{
    QHeaderView emptyView(Qt::Vertical);
    QCOMPARE(emptyView.count(), 0);
}

void tst_QHeaderView::emptyModel()
{
    QtTestModel testmodel;
    view->setModel(&testmodel);
    QVERIFY(!testmodel.wrongIndex);
    QCOMPARE(view->count(), testmodel.rows);
    view->setModel(model);
}

void tst_QHeaderView::removeRows()
{
    QtTestModel model;
    model.rows = model.cols = 10;

    QHeaderView vertical(Qt::Vertical);
    QHeaderView horizontal(Qt::Horizontal);

    vertical.setModel(&model);
    horizontal.setModel(&model);
    vertical.show();
    horizontal.show();
    QCOMPARE(vertical.count(), model.rows);
    QCOMPARE(horizontal.count(), model.cols);

    model.removeLastRow();
    QVERIFY(!model.wrongIndex);
    QCOMPARE(vertical.count(), model.rows);
    QCOMPARE(horizontal.count(), model.cols);

    model.removeAllRows();
    QVERIFY(!model.wrongIndex);
    QCOMPARE(vertical.count(), model.rows);
    QCOMPARE(horizontal.count(), model.cols);
}


void tst_QHeaderView::removeCols()
{
    QtTestModel model;
    model.rows = model.cols = 10;

    QHeaderView vertical(Qt::Vertical);
    QHeaderView horizontal(Qt::Horizontal);
    vertical.setModel(&model);
    horizontal.setModel(&model);
    vertical.show();
    horizontal.show();
    QCOMPARE(vertical.count(), model.rows);
    QCOMPARE(horizontal.count(), model.cols);

    model.removeLastColumn();
    QVERIFY(!model.wrongIndex);
    QCOMPARE(vertical.count(), model.rows);
    QCOMPARE(horizontal.count(), model.cols);

    model.removeAllColumns();
    QVERIFY(!model.wrongIndex);
    QCOMPARE(vertical.count(), model.rows);
    QCOMPARE(horizontal.count(), model.cols);
}

void tst_QHeaderView::movable()
{
    QCOMPARE(view->isMovable(), false);
    view->setMovable(false);
    QCOMPARE(view->isMovable(), false);
    view->setMovable(true);
    QCOMPARE(view->isMovable(), true);
}

void tst_QHeaderView::clickable()
{
    QCOMPARE(view->isClickable(), false);
    view->setClickable(false);
    QCOMPARE(view->isClickable(), false);
    view->setClickable(true);
    QCOMPARE(view->isClickable(), true);
}

void tst_QHeaderView::hidden()
{
    //hideSection() & showSection call setSectionHidden
    // Test bad arguments
    QCOMPARE(view->isSectionHidden(-1), false);
    QCOMPARE(view->isSectionHidden(view->count()), false);
    QCOMPARE(view->isSectionHidden(999999), false);

    view->setSectionHidden(-1, true);
    view->setSectionHidden(view->count(), true);
    view->setSectionHidden(999999, true);
    view->setSectionHidden(-1, false);
    view->setSectionHidden(view->count(), false);
    view->setSectionHidden(999999, false);

    // Hidden sections shouldn't have visual properties (except position)
    int pos = view->defaultSectionSize();
    view->setSectionHidden(1, true);
    QCOMPARE(view->sectionSize(1), 0);
    QCOMPARE(view->sectionPosition(1), pos);
    view->resizeSection(1, 100);
    QCOMPARE(view->sectionViewportPosition(1), pos);
    QCOMPARE(view->sectionSize(1), 0);
    view->setSectionHidden(1, false);
    QCOMPARE(view->isSectionHidden(0), false);
    QCOMPARE(view->sectionSize(0), view->defaultSectionSize());
}

void tst_QHeaderView::stretch()
{
    // Show before resize and setStrechLastSection
#if defined(Q_OS_WINCE)
    QSize viewSize(200,300);
#else
    QSize viewSize(500, 500);
#endif
    view->resize(viewSize);
    view->setStretchLastSection(true);
    QCOMPARE(view->stretchLastSection(), true);
    view->show();
    QCOMPARE(view->width(), viewSize.width());
    QCOMPARE(view->visualIndexAt(view->viewport()->height() - 5), 3);

    view->setSectionHidden(3, true);
    QCOMPARE(view->visualIndexAt(view->viewport()->height() - 5), 2);

    view->setStretchLastSection(false);
    QCOMPARE(view->stretchLastSection(), false);
}

void tst_QHeaderView::oneSectionSize()
{
    //this ensures that if there is only one section, it gets a correct width (more than 0)
    QHeaderView view (Qt::Vertical);
    QtTestModel model;
    model.cols = 1;
    model.rows = 1;

    view.setResizeMode(QHeaderView::Interactive);
    view.setModel(&model);

    view.show();

    QVERIFY(view.sectionSize(0) > 0);
}


void tst_QHeaderView::sectionSize_data()
{
    QTest::addColumn<QList<int> >("boundsCheck");
    QTest::addColumn<QList<int> >("defaultSizes");
    QTest::addColumn<int>("initialDefaultSize");
    QTest::addColumn<int>("lastVisibleSectionSize");
    QTest::addColumn<int>("persistentSectionSize");

    QTest::newRow("data set one")
        << (QList<int>() << -1 << 0 << 4 << 9999)
        << (QList<int>() << 10 << 30 << 30)
        << 30
        << 300
        << 20;
}

void tst_QHeaderView::sectionSize()
{
    QFETCH(QList<int>, boundsCheck);
    QFETCH(QList<int>, defaultSizes);
    QFETCH(int, initialDefaultSize);
    QFETCH(int, lastVisibleSectionSize);
    QFETCH(int, persistentSectionSize);

#ifdef Q_OS_WINCE
    // We test on a device with doubled pixels. Therefore we need to specify
    // different boundaries.
    initialDefaultSize = qMax(view->minimumSectionSize(), 30);
#endif

    // bounds check
    foreach (int val, boundsCheck)
        view->sectionSize(val);

    // default size
    QCOMPARE(view->defaultSectionSize(), initialDefaultSize);
    foreach (int def, defaultSizes) {
        view->setDefaultSectionSize(def);
        QCOMPARE(view->defaultSectionSize(), def);
    }

    view->setDefaultSectionSize(initialDefaultSize);
    for (int s = 0; s < view->count(); ++s)
        QCOMPARE(view->sectionSize(s), initialDefaultSize);
    view->doItemsLayout();

    // stretch last section
    view->setStretchLastSection(true);
    int lastSection = view->count() - 1;

    //test that when hiding the last column,
    //resizing the new last visible columns still works
    view->hideSection(lastSection);
    view->resizeSection(lastSection - 1, lastVisibleSectionSize);
    QCOMPARE(view->sectionSize(lastSection - 1), lastVisibleSectionSize);
    view->showSection(lastSection);

    // turn off stretching
    view->setStretchLastSection(false);
    QCOMPARE(view->sectionSize(lastSection), initialDefaultSize);

    // test persistence
    int sectionCount = view->count();
    for (int i = 0; i < sectionCount; ++i)
        view->resizeSection(i, persistentSectionSize);
    QtTestModel model;
    model.cols = sectionCount * 2;
    model.rows = sectionCount * 2;
    view->setModel(&model);
    for (int j = 0; j < sectionCount; ++j)
        QCOMPARE(view->sectionSize(j), persistentSectionSize);
    for (int k = sectionCount; k < view->count(); ++k)
        QCOMPARE(view->sectionSize(k), initialDefaultSize);
}

void tst_QHeaderView::visualIndex()
{
    // Test bad arguments
    QCOMPARE(view->visualIndex(999999), -1);
    QCOMPARE(view->visualIndex(-1), -1);
    QCOMPARE(view->visualIndex(1), 1);
    view->setSectionHidden(1, true);
    QCOMPARE(view->visualIndex(1), 1);
    QCOMPARE(view->visualIndex(2), 2);

    view->setSectionHidden(1, false);
    QCOMPARE(view->visualIndex(1), 1);
    QCOMPARE(view->visualIndex(2), 2);
}

void tst_QHeaderView::visualIndexAt_data()
{
    QTest::addColumn<QList<int> >("hidden");
    QTest::addColumn<QList<int> >("from");
    QTest::addColumn<QList<int> >("to");
    QTest::addColumn<QList<int> >("coordinate");
    QTest::addColumn<QList<int> >("visual");

    QList<int> coordinateList;
#ifndef Q_OS_WINCE
    coordinateList << -1 << 0 << 31 << 91 << 99999;
#else
    // We test on a device with doubled pixels. Therefore we need to specify
    // different boundaries.
    coordinateList << -1 << 0 << 33 << 97 << 99999;
#endif

    QTest::newRow("no hidden, no moved sections")
        << QList<int>()
        << QList<int>()
        << QList<int>()
        << coordinateList
        << (QList<int>() << -1 << 0 << 1 << 3 << -1);

    QTest::newRow("no hidden, moved sections")
        << QList<int>()
        << (QList<int>() << 0)
        << (QList<int>() << 1)
        << coordinateList
        << (QList<int>() << -1 << 0 << 1 << 3 << -1);

    QTest::newRow("hidden, no moved sections")
        << (QList<int>() << 0)
        << QList<int>()
        << QList<int>()
        << coordinateList
        << (QList<int>() << -1 << 1 << 2 << 3 << -1);
}

void tst_QHeaderView::visualIndexAt()
{
    QFETCH(QList<int>, hidden);
    QFETCH(QList<int>, from);
    QFETCH(QList<int>, to);
    QFETCH(QList<int>, coordinate);
    QFETCH(QList<int>, visual);

    view->setStretchLastSection(true);
    view->show();

    for (int i = 0; i < hidden.count(); ++i)
        view->setSectionHidden(hidden.at(i), true);

    for (int j = 0; j < from.count(); ++j)
        view->moveSection(from.at(j), to.at(j));

    for (int k = 0; k < coordinate.count(); ++k)
        QCOMPARE(view->visualIndexAt(coordinate.at(k)), visual.at(k));
}

void tst_QHeaderView::length()
{
#if defined(Q_OS_WINCE)
    QFont font(QLatin1String("Tahoma"), 7);
    view->setFont(font);
#elif defined(Q_OS_SYMBIAN)
    QFont font(QLatin1String("Series 60 Sans"), 6);
    view->setFont(font);
#endif
    view->setStretchLastSection(true);
    view->show();

    //minimumSectionSize should be the size of the last section of the widget is not tall enough
    int length = view->minimumSectionSize();
    for (int i=0; i < view->count()-1; i++) {
        length += view->sectionSize(i);
    }

    length = qMax(length, view->viewport()->height());
    QCOMPARE(length, view->length());

    view->setStretchLastSection(false);
    view->show();

    QVERIFY(length != view->length());

    // layoutChanged might mean rows have been removed
    QtTestModel model;
    model.cols = 10;
    model.rows = 10;
    view->setModel(&model);
    int oldLength = view->length();
    model.cleanup();
    QCOMPARE(model.rows, view->count());
    QVERIFY(oldLength != view->length());
}

void tst_QHeaderView::offset()
{
    QCOMPARE(view->offset(), 0);
    view->setOffset(10);
    QCOMPARE(view->offset(), 10);
    view->setOffset(0);
    QCOMPARE(view->offset(), 0);

    // Test odd arguments
    view->setOffset(-1);
}

void tst_QHeaderView::sectionSizeHint()
{
    // Test bad arguments
    view->sectionSizeHint(-1);
    view->sectionSizeHint(99999);

    // TODO how to test the return value?
}

void tst_QHeaderView::logicalIndex()
{
    // Test bad arguments
    QCOMPARE(view->logicalIndex(-1), -1);
    QCOMPARE(view->logicalIndex(99999), -1);
}

void tst_QHeaderView::logicalIndexAt()
{
    // Test bad arguments
    view->logicalIndexAt(-1);
    view->logicalIndexAt(99999);
    QCOMPARE(view->logicalIndexAt(0), 0);
    QCOMPARE(view->logicalIndexAt(1), 0);

    view->show();
    view->setStretchLastSection(true);
    // First item
    QCOMPARE(view->logicalIndexAt(0), 0);
    QCOMPARE(view->logicalIndexAt(view->sectionSize(0)-1), 0);
    QCOMPARE(view->logicalIndexAt(view->sectionSize(0)+1), 1);
    // Last item
    int last = view->length() - 1;//view->viewport()->height() - 10;
    QCOMPARE(view->logicalIndexAt(last), 3);
    // Not in widget
    int outofbounds = view->length() + 1;//view->viewport()->height() + 1;
    QCOMPARE(view->logicalIndexAt(outofbounds), -1);

    view->moveSection(0,1);
    // First item
    QCOMPARE(view->logicalIndexAt(0), 1);
    QCOMPARE(view->logicalIndexAt(view->sectionSize(0)-1), 1);
    QCOMPARE(view->logicalIndexAt(view->sectionSize(0)+1), 0);
    // Last item
    QCOMPARE(view->logicalIndexAt(last), 3);
    view->moveSection(1,0);

}

void tst_QHeaderView::swapSections()
{
    view->swapSections(-1, 1);
    view->swapSections(99999, 1);
    view->swapSections(1, -1);
    view->swapSections(1, 99999);

    QVector<int> logical = (QVector<int>() << 0 << 1 << 2 << 3);

    QSignalSpy spy1(view, SIGNAL(sectionMoved(int, int, int)));

    QCOMPARE(view->sectionsMoved(), false);
    view->swapSections(1, 1);
    QCOMPARE(view->sectionsMoved(), false);
    view->swapSections(1, 2);
    QCOMPARE(view->sectionsMoved(), true);
    view->swapSections(2, 1);
    QCOMPARE(view->sectionsMoved(), true);
    for (int i = 0; i < view->count(); ++i)
        QCOMPARE(view->logicalIndex(i), logical.at(i));
    QCOMPARE(spy1.count(), 4);

    logical = (QVector<int>()  << 3 << 1 << 2 << 0);
    view->swapSections(3, 0);
    QCOMPARE(view->sectionsMoved(), true);
    for (int j = 0; j < view->count(); ++j)
        QCOMPARE(view->logicalIndex(j), logical.at(j));
    QCOMPARE(spy1.count(), 6);
}

void tst_QHeaderView::moveSection_data()
{
    QTest::addColumn<QList<int> >("hidden");
    QTest::addColumn<QList<int> >("from");
    QTest::addColumn<QList<int> >("to");
    QTest::addColumn<QList<bool> >("moved");
    QTest::addColumn<QList<int> >("logical");
    QTest::addColumn<int>("count");

    QTest::newRow("bad args, no hidden")
        << QList<int>()
        << (QList<int>() << -1 << 1 << 99999 << 1)
        << (QList<int>() << 1 << -1 << 1 << 99999)
        << (QList<bool>() << false << false << false << false)
        << (QList<int>() << 0 << 1 << 2 << 3)
        << 0;

    QTest::newRow("good args, no hidden")
        << QList<int>()
        << (QList<int>() << 1 << 1 << 2 << 1)
        << (QList<int>() << 1 << 2 << 1 << 2)
        << (QList<bool>() << false << true << true << true)
        << (QList<int>() << 0 << 2 << 1 << 3)
        << 3;

    QTest::newRow("hidden sections")
        << (QList<int>() << 0 << 3)
        << (QList<int>() << 1 << 1 << 2 << 1)
        << (QList<int>() << 1 << 2 << 1 << 2)
        << (QList<bool>() << false << true << true << true)
        << (QList<int>() << 0 << 2 << 1 << 3)
        << 3;
}

void tst_QHeaderView::moveSection()
{
    QFETCH(QList<int>, hidden);
    QFETCH(QList<int>, from);
    QFETCH(QList<int>, to);
    QFETCH(QList<bool>, moved);
    QFETCH(QList<int>, logical);
    QFETCH(int, count);

    QVERIFY(from.count() == to.count());
    QVERIFY(from.count() == moved.count());
    QVERIFY(view->count() == logical.count());

    QSignalSpy spy1(view, SIGNAL(sectionMoved(int, int, int)));
    QCOMPARE(view->sectionsMoved(), false);

    for (int h = 0; h < hidden.count(); ++h)
        view->setSectionHidden(hidden.at(h), true);

    for (int i = 0; i < from.count(); ++i) {
        view->moveSection(from.at(i), to.at(i));
        QCOMPARE(view->sectionsMoved(), moved.at(i));
    }

    for (int j = 0; j < view->count(); ++j)
        QCOMPARE(view->logicalIndex(j), logical.at(j));

    QCOMPARE(spy1.count(), count);
}

void tst_QHeaderView::resizeAndMoveSection_data()
{
    QTest::addColumn<IntList>("logicalIndexes");
    QTest::addColumn<IntList>("sizes");
    QTest::addColumn<int>("logicalFrom");
    QTest::addColumn<int>("logicalTo");

    QTest::newRow("resizeAndMove-1")
        << (IntList() << 0 << 1)
        << (IntList() << 20 << 40)
        << 0 << 1;

    QTest::newRow("resizeAndMove-2")
        << (IntList() << 0 << 1 << 2 << 3)
        << (IntList() << 20 << 60 << 10 << 80)
        << 0 << 2;

    QTest::newRow("resizeAndMove-3")
        << (IntList() << 0 << 1 << 2 << 3)
        << (IntList() << 100 << 60 << 40 << 10)
        << 0 << 3;

    QTest::newRow("resizeAndMove-4")
        << (IntList() << 0 << 1 << 2 << 3)
        << (IntList() << 10 << 40 << 80 << 30)
        << 1 << 2;

    QTest::newRow("resizeAndMove-5")
        << (IntList() << 2 << 3)
        << (IntList() << 100 << 200)
        << 3 << 2;
}

void tst_QHeaderView::resizeAndMoveSection()
{
    QFETCH(IntList, logicalIndexes);
    QFETCH(IntList, sizes);
    QFETCH(int, logicalFrom);
    QFETCH(int, logicalTo);

    // Save old visual indexes and sizes
    IntList oldVisualIndexes;
    IntList oldSizes;
    foreach (int logical, logicalIndexes) {
        oldVisualIndexes.append(view->visualIndex(logical));
        oldSizes.append(view->sectionSize(logical));
    }

    // Resize sections
    for (int i = 0; i < logicalIndexes.size(); ++i) {
        int logical = logicalIndexes.at(i);
        view->resizeSection(logical, sizes.at(i));
    }

    // Move sections
    int visualFrom = view->visualIndex(logicalFrom);
    int visualTo = view->visualIndex(logicalTo);
    view->moveSection(visualFrom, visualTo);
    QCOMPARE(view->visualIndex(logicalFrom), visualTo);

    // Check that sizes are still correct
    for (int i = 0; i < logicalIndexes.size(); ++i) {
        int logical = logicalIndexes.at(i);
        QCOMPARE(view->sectionSize(logical), sizes.at(i));
    }

    // Move sections back
    view->moveSection(visualTo, visualFrom);

    // Check that sizes are still correct
    for (int i = 0; i < logicalIndexes.size(); ++i) {
        int logical = logicalIndexes.at(i);
        QCOMPARE(view->sectionSize(logical), sizes.at(i));
    }

    // Put everything back as it was
    for (int i = 0; i < logicalIndexes.size(); ++i) {
        int logical = logicalIndexes.at(i);
        view->resizeSection(logical, oldSizes.at(i));
        QCOMPARE(view->visualIndex(logical), oldVisualIndexes.at(i));
    }
}

void tst_QHeaderView::resizeHiddenSection_data()
{
    QTest::addColumn<int>("section");
    QTest::addColumn<int>("initialSize");
    QTest::addColumn<int>("finalSize");

    QTest::newRow("section 0 resize 50 to 20")
        << 0 << 50 << 20;

    QTest::newRow("section 1 resize 50 to 20")
        << 1 << 50 << 20;

    QTest::newRow("section 2 resize 50 to 20")
        << 2 << 50 << 20;

    QTest::newRow("section 3 resize 50 to 20")
        << 3 << 50 << 20;
}

void tst_QHeaderView::resizeHiddenSection()
{
    QFETCH(int, section);
    QFETCH(int, initialSize);
    QFETCH(int, finalSize);

    view->resizeSection(section, initialSize);
    view->setSectionHidden(section, true);
    QCOMPARE(view->sectionSize(section), 0);

    view->resizeSection(section, finalSize);
    QCOMPARE(view->sectionSize(section), 0);

    view->setSectionHidden(section, false);
    QCOMPARE(view->sectionSize(section), finalSize);
}

void tst_QHeaderView::resizeAndInsertSection_data()
{
    QTest::addColumn<int>("section");
    QTest::addColumn<int>("size");
    QTest::addColumn<int>("insert");
    QTest::addColumn<int>("compare");
    QTest::addColumn<int>("expected");

    QTest::newRow("section 0 size 50 insert 0")
        << 0 << 50 << 0 << 1 << 50;

    QTest::newRow("section 1 size 50 insert 1")
        << 0 << 50 << 1 << 0 << 50;

    QTest::newRow("section 1 size 50 insert 0")
        << 1 << 50 << 0 << 2 << 50;

}

void tst_QHeaderView::resizeAndInsertSection()
{
    QFETCH(int, section);
    QFETCH(int, size);
    QFETCH(int, insert);
    QFETCH(int, compare);
    QFETCH(int, expected);

    view->setStretchLastSection(false);

    view->resizeSection(section, size);
    QCOMPARE(view->sectionSize(section), size);

    model->insertRow(insert);

    QCOMPARE(view->sectionSize(compare), expected);
}

void tst_QHeaderView::resizeWithResizeModes_data()
{
    QTest::addColumn<int>("size");
    QTest::addColumn<QList<int> >("sections");
    QTest::addColumn<QList<int> >("modes");
    QTest::addColumn<QList<int> >("expected");

    QTest::newRow("stretch first section")
        << 600
        << (QList<int>() << 100 << 100 << 100 << 100)
        << (QList<int>() << ((int)QHeaderView::Stretch)
                         << ((int)QHeaderView::Interactive)
                         << ((int)QHeaderView::Interactive)
                         << ((int)QHeaderView::Interactive))
        << (QList<int>() << 300 << 100 << 100 << 100);
}

void  tst_QHeaderView::resizeWithResizeModes()
{
    QFETCH(int, size);
    QFETCH(QList<int>, sections);
    QFETCH(QList<int>, modes);
    QFETCH(QList<int>, expected);

    view->setStretchLastSection(false);
    for (int i = 0; i < sections.count(); ++i) {
        view->resizeSection(i, sections.at(i));
        view->setResizeMode(i, (QHeaderView::ResizeMode)modes.at(i));
    }
    view->show();
    view->resize(size, size);
    for (int j = 0; j < expected.count(); ++j)
        QCOMPARE(view->sectionSize(j), expected.at(j));
}

void tst_QHeaderView::moveAndInsertSection_data()
{
    QTest::addColumn<int>("from");
    QTest::addColumn<int>("to");
    QTest::addColumn<int>("insert");
    QTest::addColumn<QList<int> >("mapping");

    QTest::newRow("move from 1 to 3, insert 0")
        << 1 << 3 << 0 <<(QList<int>() << 0 << 1 << 3 << 4 << 2);

}

void tst_QHeaderView::moveAndInsertSection()
{
    QFETCH(int, from);
    QFETCH(int, to);
    QFETCH(int, insert);
    QFETCH(QList<int>, mapping);

    view->setStretchLastSection(false);

    view->moveSection(from, to);

    model->insertRow(insert);

    for (int i = 0; i < mapping.count(); ++i)
        QCOMPARE(view->logicalIndex(i), mapping.at(i));
}

void tst_QHeaderView::resizeMode()
{
    // Q_ASSERT's when resizeMode is called with an invalid index
    int last = view->count() - 1;
    view->setResizeMode(QHeaderView::Interactive);
    QCOMPARE(view->resizeMode(last), QHeaderView::Interactive);
    QCOMPARE(view->resizeMode(1), QHeaderView::Interactive);
    view->setResizeMode(QHeaderView::Stretch);
    QCOMPARE(view->resizeMode(last), QHeaderView::Stretch);
    QCOMPARE(view->resizeMode(1), QHeaderView::Stretch);
    view->setResizeMode(QHeaderView::Custom);
    QCOMPARE(view->resizeMode(last), QHeaderView::Custom);
    QCOMPARE(view->resizeMode(1), QHeaderView::Custom);

    // test when sections have been moved
    view->setStretchLastSection(false);
    for (int i=0; i < (view->count() - 1); ++i)
        view->setResizeMode(i, QHeaderView::Interactive);
    int logicalIndex = view->count() / 2;
    view->setResizeMode(logicalIndex, QHeaderView::Stretch);
    view->moveSection(view->visualIndex(logicalIndex), 0);
    for (int i=0; i < (view->count() - 1); ++i) {
        if (i == logicalIndex)
            QCOMPARE(view->resizeMode(i), QHeaderView::Stretch);
        else
            QCOMPARE(view->resizeMode(i), QHeaderView::Interactive);
    }
}

void tst_QHeaderView::resizeSection_data()
{
    QTest::addColumn<int>("initial");
    QTest::addColumn<QList<int> >("logical");
    QTest::addColumn<QList<int> >("size");
    QTest::addColumn<QList<int> >("mode");
    QTest::addColumn<int>("resized");
    QTest::addColumn<QList<int> >("expected");

    QTest::newRow("bad args")
        << 100
        << (QList<int>() << -1 << -1 << 99999 << 99999 << 4)
        << (QList<int>() << -1 << 0 << 99999 << -1 << -1)
        << (QList<int>()
            << int(QHeaderView::Interactive)
            << int(QHeaderView::Interactive)
            << int(QHeaderView::Interactive)
            << int(QHeaderView::Interactive))
        << 0
        << (QList<int>() << 0 << 0 << 0 << 0 << 0);
}

void tst_QHeaderView::resizeSection()
{

    QFETCH(int, initial);
    QFETCH(QList<int>, logical);
    QFETCH(QList<int>, size);
    QFETCH(QList<int>, mode);
    QFETCH(int, resized);
    QFETCH(QList<int>, expected);

    view->resize(400, 400);

    view->show();
    view->setMovable(true);
    view->setStretchLastSection(false);

    for (int i = 0; i < logical.count(); ++i)
        if (logical.at(i) > -1 && logical.at(i) < view->count()) // for now
            view->setResizeMode(logical.at(i), (QHeaderView::ResizeMode)mode.at(i));

    for (int j = 0; j < logical.count(); ++j)
        view->resizeSection(logical.at(j), initial);

    QSignalSpy spy(view, SIGNAL(sectionResized(int, int, int)));

    for (int k = 0; k < logical.count(); ++k)
        view->resizeSection(logical.at(k), size.at(k));

    QCOMPARE(spy.count(), resized);

    for (int l = 0; l < logical.count(); ++l)
        QCOMPARE(view->sectionSize(logical.at(l)), expected.at(l));
}

void tst_QHeaderView::highlightSections()
{
    view->setHighlightSections(true);
    QCOMPARE(view->highlightSections(), true);
    view->setHighlightSections(false);
    QCOMPARE(view->highlightSections(), false);
}

void tst_QHeaderView::showSortIndicator()
{
    view->setSortIndicatorShown(true);
    QCOMPARE(view->isSortIndicatorShown(), true);
    QCOMPARE(view->sortIndicatorOrder(), Qt::DescendingOrder);
    view->setSortIndicator(1, Qt::AscendingOrder);
    QCOMPARE(view->sortIndicatorOrder(), Qt::AscendingOrder);
    view->setSortIndicator(1, Qt::DescendingOrder);
    QCOMPARE(view->sortIndicatorOrder(), Qt::DescendingOrder);
    view->setSortIndicatorShown(false);
    QCOMPARE(view->isSortIndicatorShown(), false);

    view->setSortIndicator(999999, Qt::DescendingOrder);
    // Don't segfault baby :)
    view->setSortIndicatorShown(true);

    view->setSortIndicator(0, Qt::DescendingOrder);
    // Don't assert baby :)
}

void tst_QHeaderView::sortIndicatorTracking()
{
    QtTestModel model;
    model.rows = model.cols = 10;

    QHeaderView hv(Qt::Horizontal);

    hv.setModel(&model);
    hv.show();
    hv.setSortIndicatorShown(true);
    hv.setSortIndicator(1, Qt::DescendingOrder);

    model.removeOneColumn(8);
    QCOMPARE(hv.sortIndicatorSection(), 1);

    model.removeOneColumn(2);
    QCOMPARE(hv.sortIndicatorSection(), 1);

    model.insertOneColumn(2);
    QCOMPARE(hv.sortIndicatorSection(), 1);

    model.insertOneColumn(1);
    QCOMPARE(hv.sortIndicatorSection(), 2);

    model.removeOneColumn(0);
    QCOMPARE(hv.sortIndicatorSection(), 1);

    model.removeOneColumn(1);
    QCOMPARE(hv.sortIndicatorSection(), -1);
}

void tst_QHeaderView::removeAndInsertRow()
{
    // Check if logicalIndex returns the correct value after we have removed a row
    // we might as well te
    for (int i = 0; i < model->rowCount(); ++i) {
        QCOMPARE(i, view->logicalIndex(i));
    }

    while (model->removeRow(0)) {
        for (int i = 0; i < model->rowCount(); ++i) {
            QCOMPARE(i, view->logicalIndex(i));
        }
    }

    int pass = 0;
    for (pass = 0; pass < 5; pass++) {
        for (int i = 0; i < model->rowCount(); ++i) {
            QCOMPARE(i, view->logicalIndex(i));
        }
        model->insertRow(0);
    }

    while (model->removeRows(0, 2)) {
        for (int i = 0; i < model->rowCount(); ++i) {
            QCOMPARE(i, view->logicalIndex(i));
        }
    }

    for (pass = 0; pass < 3; pass++) {
        model->insertRows(0, 2);
        for (int i = 0; i < model->rowCount(); ++i) {
            QCOMPARE(i, view->logicalIndex(i));
        }
    }

    for (pass = 0; pass < 3; pass++) {
        model->insertRows(3, 2);
        for (int i = 0; i < model->rowCount(); ++i) {
            QCOMPARE(i, view->logicalIndex(i));
        }
    }

    // Insert at end
    for (pass = 0; pass < 3; pass++) {
        int rowCount = model->rowCount();
        model->insertRows(rowCount, 1);
        for (int i = 0; i < rowCount; ++i) {
            QCOMPARE(i, view->logicalIndex(i));
        }
    }

}
void tst_QHeaderView::unhideSection()
{
    // You should not necessarily expect the same size back again, so the best test we can do is to test if it is larger than 0 after a unhide.
    QCOMPARE(view->sectionsHidden(), false);
    view->setSectionHidden(0, true);
    QCOMPARE(view->sectionsHidden(), true);
    QVERIFY(view->sectionSize(0) == 0);
    view->setResizeMode(QHeaderView::Interactive);
    view->setSectionHidden(0, false);
    QVERIFY(view->sectionSize(0) > 0);

    view->setSectionHidden(0, true);
    QVERIFY(view->sectionSize(0) == 0);
    view->setSectionHidden(0, true);
    QVERIFY(view->sectionSize(0) == 0);
    view->setResizeMode(QHeaderView::Stretch);
    view->setSectionHidden(0, false);
    QVERIFY(view->sectionSize(0) > 0);

}

void tst_QHeaderView::event()
{
    protected_QHeaderView x(Qt::Vertical);
    x.testEvent();
    protected_QHeaderView y(Qt::Horizontal);
    y.testEvent();
}


void protected_QHeaderView::testEvent()
{
    // No crashy please
    QHoverEvent enterEvent(QEvent::HoverEnter, QPoint(), QPoint());
    event(&enterEvent);
    QHoverEvent eventLeave(QEvent::HoverLeave, QPoint(), QPoint());
    event(&eventLeave);
    QHoverEvent eventMove(QEvent::HoverMove, QPoint(), QPoint());
    event(&eventMove);
}

void tst_QHeaderView::headerDataChanged()
{
    // This shouldn't asserver because view is Vertical
    view->headerDataChanged(Qt::Horizontal, -1, -1);
#if 0
    // This will assert
    view->headerDataChanged(Qt::Vertical, -1, -1);
#endif

    // No crashing please
    view->headerDataChanged(Qt::Horizontal, 0, 1);
    view->headerDataChanged(Qt::Vertical, 0, 1);
}

void tst_QHeaderView::currentChanged()
{
    view->setCurrentIndex(QModelIndex());
}

void tst_QHeaderView::horizontalOffset()
{
    protected_QHeaderView x(Qt::Vertical);
    x.testhorizontalOffset();
    protected_QHeaderView y(Qt::Horizontal);
    y.testhorizontalOffset();
}

void tst_QHeaderView::verticalOffset()
{
    protected_QHeaderView x(Qt::Vertical);
    x.testverticalOffset();
    protected_QHeaderView y(Qt::Horizontal);
    y.testverticalOffset();
}

void  protected_QHeaderView::testhorizontalOffset()
{
    if(orientation() == Qt::Horizontal){
        QCOMPARE(horizontalOffset(), 0);
        setOffset(10);
        QCOMPARE(horizontalOffset(), 10);
    }
    else
        QCOMPARE(horizontalOffset(), 0);

}

void  protected_QHeaderView::testverticalOffset()
{
    if(orientation() == Qt::Vertical){
        QCOMPARE(verticalOffset(), 0);
        setOffset(10);
        QCOMPARE(verticalOffset(), 10);
    }
    else
        QCOMPARE(verticalOffset(), 0);
}

void tst_QHeaderView::stretchSectionCount()
{
    view->setStretchLastSection(false);
    QCOMPARE(view->stretchSectionCount(), 0);
    view->setStretchLastSection(true);
    QCOMPARE(view->stretchSectionCount(), 0);

    view->setResizeMode(0, QHeaderView::Stretch);
    QCOMPARE(view->stretchSectionCount(), 1);
}

void tst_QHeaderView::hiddenSectionCount()
{
    model->clear();
    model->insertRows(0, 10);
    // Hide every other one
    for (int i=0; i<10; i++)
        view->setSectionHidden(i, (i & 1) == 0);

    QCOMPARE(view->hiddenSectionCount(), 5);

    view->setResizeMode(QHeaderView::Stretch);
    QCOMPARE(view->hiddenSectionCount(), 5);

    // Remove some rows and make sure they are now still counted
    model->removeRow(9);
    model->removeRow(8);
    model->removeRow(7);
    model->removeRow(6);
    QCOMPARE(view->count(), 6);
    QCOMPARE(view->hiddenSectionCount(), 3);
    model->removeRows(0,5);
    QCOMPARE(view->count(), 1);
    QCOMPARE(view->hiddenSectionCount(), 0);
    QVERIFY(view->count() >=  view->hiddenSectionCount());
}

void tst_QHeaderView::focusPolicy()
{
    QHeaderView view(Qt::Horizontal);
    QCOMPARE(view.focusPolicy(), Qt::NoFocus);

    QTreeWidget widget;
    QCOMPARE(widget.header()->focusPolicy(), Qt::NoFocus);
    QVERIFY(!widget.focusProxy());
    QVERIFY(!widget.hasFocus());
    QVERIFY(!widget.header()->focusProxy());
    QVERIFY(!widget.header()->hasFocus());

    widget.show();
    widget.setFocus(Qt::OtherFocusReason);
    QApplication::setActiveWindow(&widget);
    QTest::qWaitForWindowShown(&widget);
    widget.activateWindow();
    QTest::qWait(100);

    qApp->processEvents();

    WAIT_FOR_CONDITION(widget.hasFocus(), true);

    QVERIFY(widget.hasFocus());
    QVERIFY(!widget.header()->hasFocus());

    widget.setFocusPolicy(Qt::NoFocus);
    widget.clearFocus();

    qApp->processEvents();
    qApp->processEvents();

    WAIT_FOR_CONDITION(widget.hasFocus(), false);
    QVERIFY(!widget.hasFocus());
    QVERIFY(!widget.header()->hasFocus());

    QTest::keyPress(&widget, Qt::Key_Tab);

    qApp->processEvents();
    qApp->processEvents();

    QVERIFY(!widget.hasFocus());
    QVERIFY(!widget.header()->hasFocus());
}

class SimpleModel : public QAbstractItemModel
{
    Q_OBJECT
public:

    SimpleModel( QObject* parent=0)
        : QAbstractItemModel(parent),
        m_col_count(3) {}

    QModelIndex parent(const QModelIndex &/*child*/) const
    {
        return QModelIndex();
    }
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const
    {
        return hasIndex(row, column, parent) ? createIndex(row, column, 0) : QModelIndex();
    }
    int rowCount(const QModelIndex & /* parent */) const
    {
        return 8;
    }
    int columnCount(const QModelIndex &/*parent= QModelIndex()*/) const
    {
        return m_col_count;
    }

    QVariant data(const QModelIndex &index, int role) const
    {
        if (!index.isValid())
        {
            return QVariant();
        }
        if (role == Qt::DisplayRole) {
            return QString::fromAscii("%1,%2").arg(index.row()).arg(index.column());
        }
        return QVariant();
    }

    void setColumnCount( int c )
    {
        m_col_count = c;
    }

private:
    int m_col_count;
};

void tst_QHeaderView::moveSectionAndReset()
{
    SimpleModel m;
    QHeaderView v(Qt::Horizontal);
    v.setModel(&m);
    int cc = 2;
    for (cc = 2; cc < 4; ++cc) {
        m.setColumnCount(cc);
        int movefrom = 0;
        int moveto;
        for (moveto = 1; moveto < cc; ++moveto) {
            v.moveSection(movefrom, moveto);
            m.setColumnCount(cc - 1);
            v.reset();
            for (int i = 0; i < cc - 1; ++i) {
                QCOMPARE(v.logicalIndex(v.visualIndex(i)), i);
            }
        }
    }
}

void tst_QHeaderView::moveSectionAndRemove()
{
    QStandardItemModel m;
    QHeaderView v(Qt::Horizontal);

    v.setModel(&m);
    v.model()->insertColumns(0, 3);
    v.moveSection(0, 1);

    QCOMPARE(v.count(), 3);
    v.model()->removeColumns(0, v.model()->columnCount());
    QCOMPARE(v.count(), 0);
}

void tst_QHeaderView::saveRestore()
{
    SimpleModel m;
    QHeaderView h1(Qt::Horizontal);
    h1.setModel(&m);
    h1.swapSections(0, 2);
    h1.resizeSection(1, 10);
    h1.setSortIndicatorShown(true);
    h1.setSortIndicator(1,Qt::DescendingOrder);
    QByteArray s1 = h1.saveState();

    QHeaderView h2(Qt::Vertical);
    QSignalSpy spy(&h2, SIGNAL(sortIndicatorChanged(int,Qt::SortOrder)));

    h2.setModel(&m);
    h2.restoreState(s1);

    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.at(0).at(0).toInt(), 1);

    QCOMPARE(h2.logicalIndex(0), 2);
    QCOMPARE(h2.logicalIndex(2), 0);
    QCOMPARE(h2.sectionSize(1), 10);
    QCOMPARE(h2.sortIndicatorSection(), 1);
    QCOMPARE(h2.sortIndicatorOrder(), Qt::DescendingOrder);
    QCOMPARE(h2.isSortIndicatorShown(), true);

    QByteArray s2 = h2.saveState();

    QVERIFY(s1 == s2);
}

void tst_QHeaderView::defaultAlignment_data()
{
    QTest::addColumn<int>("direction");
    QTest::addColumn<int>("initial");
    QTest::addColumn<int>("alignment");

    QTest::newRow("horizontal right aligned")
        << int(Qt::Horizontal)
        << int(Qt::AlignCenter)
        << int(Qt::AlignRight);

    QTest::newRow("horizontal left aligned")
        << int(Qt::Horizontal)
        << int(Qt::AlignCenter)
        << int(Qt::AlignLeft);

    QTest::newRow("vertical right aligned")
        << int(Qt::Vertical)
        << int(Qt::AlignLeft|Qt::AlignVCenter)
        << int(Qt::AlignRight);

    QTest::newRow("vertical left aligned")
        << int(Qt::Vertical)
        << int(Qt::AlignLeft|Qt::AlignVCenter)
        << int(Qt::AlignLeft);
}

void tst_QHeaderView::defaultAlignment()
{
    QFETCH(int, direction);
    QFETCH(int, initial);
    QFETCH(int, alignment);

    SimpleModel m;

    QHeaderView header((Qt::Orientation)direction);
    header.setModel(&m);

    QCOMPARE(header.defaultAlignment(), (Qt::Alignment)initial);
    header.setDefaultAlignment((Qt::Alignment)alignment);
    QCOMPARE(header.defaultAlignment(), (Qt::Alignment)alignment);
}

void tst_QHeaderView::globalResizeMode_data()
{
    QTest::addColumn<int>("direction");
    QTest::addColumn<int>("mode");
    QTest::addColumn<int>("insert");

    QTest::newRow("horizontal ResizeToContents 0")
        << int(Qt::Horizontal)
        << int(QHeaderView::ResizeToContents)
        << 0;
}

void tst_QHeaderView::globalResizeMode()
{
    QFETCH(int, direction);
    QFETCH(int, mode);
    QFETCH(int, insert);

    QStandardItemModel m(4, 4);
    QHeaderView h((Qt::Orientation)direction);
    h.setModel(&m);

    h.setResizeMode((QHeaderView::ResizeMode)mode);
    m.insertRow(insert);
    for (int i = 0; i < h.count(); ++i)
        QCOMPARE(h.resizeMode(i), (QHeaderView::ResizeMode)mode);
}


void tst_QHeaderView::sectionPressedSignal_data()
{
    QTest::addColumn<int>("direction");
    QTest::addColumn<bool>("clickable");
    QTest::addColumn<int>("count");

    QTest::newRow("horizontal unclickable 0")
        << int(Qt::Horizontal)
        << false
        << 0;

    QTest::newRow("horizontal clickable 1")
        << int(Qt::Horizontal)
        << true
        << 1;
}

void tst_QHeaderView::sectionPressedSignal()
{
    QFETCH(int, direction);
    QFETCH(bool, clickable);
    QFETCH(int, count);

    QStandardItemModel m(4, 4);
    QHeaderView h((Qt::Orientation)direction);

    h.setModel(&m);
    h.show();
    h.setClickable(clickable);

    QSignalSpy spy(&h, SIGNAL(sectionPressed(int)));

    QCOMPARE(spy.count(), 0);
    QTest::mousePress(h.viewport(), Qt::LeftButton, Qt::NoModifier, QPoint(5, 5));
    QCOMPARE(spy.count(), count);
}

void tst_QHeaderView::sectionClickedSignal()
{
    QFETCH(int, direction);
    QFETCH(bool, clickable);
    QFETCH(int, count);

    QStandardItemModel m(4, 4);
    QHeaderView h((Qt::Orientation)direction);

    h.setModel(&m);
    h.show();
    h.setClickable(clickable);
    h.setSortIndicatorShown(true);

    QSignalSpy spy(&h, SIGNAL(sectionClicked(int)));
    QSignalSpy spy2(&h, SIGNAL(sortIndicatorChanged(int,Qt::SortOrder)));

    QCOMPARE(spy.count(), 0);
    QCOMPARE(spy2.count(), 0);
    QTest::mouseClick(h.viewport(), Qt::LeftButton, Qt::NoModifier, QPoint(5, 5));
    QCOMPARE(spy.count(), count);
    QCOMPARE(spy2.count(), count);

    //now let's try with the sort indicator hidden (the result should be the same
    spy.clear();
    spy2.clear();
    h.setSortIndicatorShown(false);
    QTest::mouseClick(h.viewport(), Qt::LeftButton, Qt::NoModifier, QPoint(5, 5));
    QCOMPARE(spy.count(), count);
    QCOMPARE(spy2.count(), count);
}

void tst_QHeaderView::defaultSectionSize_data()
{
    QTest::addColumn<int>("direction");
    QTest::addColumn<int>("oldDefaultSize");
    QTest::addColumn<int>("newDefaultSize");

    //QTest::newRow("horizontal,-5") << int(Qt::Horizontal) << 100 << -5;
    QTest::newRow("horizontal, 0") << int(Qt::Horizontal) << 100 << 0;
    QTest::newRow("horizontal, 5") << int(Qt::Horizontal) << 100 << 5;
    QTest::newRow("horizontal,25") << int(Qt::Horizontal) << 100 << 5;
}

void tst_QHeaderView::defaultSectionSize()
{
    QFETCH(int, direction);
    QFETCH(int, oldDefaultSize);
    QFETCH(int, newDefaultSize);

    QStandardItemModel m(4, 4);
    QHeaderView h((Qt::Orientation)direction);

    h.setModel(&m);

    QCOMPARE(h.defaultSectionSize(), oldDefaultSize);
    h.setDefaultSectionSize(newDefaultSize);
    QCOMPARE(h.defaultSectionSize(), newDefaultSize);
    h.reset();
    for (int i = 0; i < h.count(); ++i)
        QCOMPARE(h.sectionSize(i), newDefaultSize);
}

void tst_QHeaderView::hideAndInsert_data()
{
    QTest::addColumn<int>("direction");
    QTest::addColumn<int>("hide");
    QTest::addColumn<int>("insert");
    QTest::addColumn<int>("hidden");

    QTest::newRow("horizontal, 0, 0") << int(Qt::Horizontal) << 0 << 0 << 1;
}

void tst_QHeaderView::hideAndInsert()
{
    QFETCH(int, direction);
    QFETCH(int, hide);
    QFETCH(int, insert);
    QFETCH(int, hidden);

    QStandardItemModel m(4, 4);
    QHeaderView h((Qt::Orientation)direction);

    h.setModel(&m);

    h.setSectionHidden(hide, true);

    if (direction == Qt::Vertical)
        m.insertRow(insert);
    else
        m.insertColumn(insert);

    for (int i = 0; i < h.count(); ++i)
        if (i != hidden)
            QCOMPARE(h.isSectionHidden(i), false);
        else
            QCOMPARE(h.isSectionHidden(i), true);
}

void tst_QHeaderView::removeSection()
{
//test that removing a hidden section gives the expected result: the next row should be hidden
//(see task
    const int hidden = 3; //section that will be hidden
    const QStringList list = QStringList() << "0" << "1" << "2" << "3" << "4" << "5" << "6";

    QStringListModel model( list );
    QHeaderView view(Qt::Vertical);
    view.setModel(&model);
    view.hideSection(hidden);
    view.hideSection(1);
    model.removeRow(1);
    view.show();

    for(int i = 0; i < view.count(); i++) {
        if (i == (hidden-1)) { //-1 because we removed a row in the meantime
            QCOMPARE(view.sectionSize(i), 0);
            QVERIFY(view.isSectionHidden(i));
        } else {
            QCOMPARE(view.sectionSize(i), view.defaultSectionSize() );
            QVERIFY(!view.isSectionHidden(i));
        }
    }
}

void tst_QHeaderView::preserveHiddenSectionWidth()
{
    const QStringList list = QStringList() << "0" << "1" << "2" << "3";

    QStringListModel model( list );
    QHeaderView view(Qt::Vertical);
    view.setModel(&model);
    view.resizeSection(0, 100);
    view.resizeSection(1, 10);
    view.resizeSection(2, 50);
    view.setResizeMode(3, QHeaderView::Stretch);
    view.show();

    view.hideSection(2);
    model.removeRow(1);
    view.showSection(1);
    QCOMPARE(view.sectionSize(0), 100);
    QCOMPARE(view.sectionSize(1), 50);

    view.hideSection(1);
    model.insertRow(1);
    view.showSection(2);
    QCOMPARE(view.sectionSize(0), 100);
    QCOMPARE(view.sectionSize(1), view.defaultSectionSize());
    QCOMPARE(view.sectionSize(2), 50);
}

void tst_QHeaderView::invisibleStretchLastSection()
{
    int count = 6;
    QStandardItemModel model(1, count);
    QHeaderView view(Qt::Horizontal);
    view.setModel(&model);
    int height = view.height();

    view.resize(view.defaultSectionSize() * (count / 2), height); // don't show all sections
    view.show();
    view.setStretchLastSection(true);
    // stretch section is not visible; it should not be stretched
    for (int i = 0; i < count; ++i)
        QCOMPARE(view.sectionSize(i), view.defaultSectionSize());

    view.resize(view.defaultSectionSize() * (count + 1), height); // give room to stretch

    // stretch section is visible; it should be stretched
    for (int i = 0; i < count - 1; ++i)
        QCOMPARE(view.sectionSize(i), view.defaultSectionSize());
    QCOMPARE(view.sectionSize(count - 1), view.defaultSectionSize() * 2);
}

void tst_QHeaderView::emptySectionSpan()
{
    QHeaderViewPrivate::SectionSpan span;
    QCOMPARE(span.sectionSize(), 0);
}

void tst_QHeaderView::task236450_hidden_data()
{
    QTest::addColumn<QList<int> >("hide1");
    QTest::addColumn<QList<int> >("hide2");

    QTest::newRow("set 1") << (QList<int>() << 1 << 3)
                           << (QList<int>() << 1 << 5);

    QTest::newRow("set 2") << (QList<int>() << 2 << 3)
                           << (QList<int>() << 1 << 5);

    QTest::newRow("set 3") << (QList<int>() << 0 << 2 << 4)
                           << (QList<int>() << 2 << 3 << 5);

}

void tst_QHeaderView::task236450_hidden()
{
    QFETCH(QList<int>, hide1);
    QFETCH(QList<int>, hide2);
    const QStringList list = QStringList() << "0" << "1" << "2" << "3" << "4" << "5";

    QStringListModel model( list );
    protected_QHeaderView view(Qt::Vertical);
    view.setModel(&model);
    view.show();

    foreach (int i, hide1)
        view.hideSection(i);

    QCOMPARE(view.hiddenSectionCount(), hide1.count());
    for (int i = 0; i < 6; i++) {
        QCOMPARE(!view.isSectionHidden(i), !hide1.contains(i));
    }

    view.setDefaultSectionSize(2);
    view.scheduleDelayedItemsLayout();
    view.executeDelayedItemsLayout(); //force to do a relayout

    QCOMPARE(view.hiddenSectionCount(), hide1.count());
    for (int i = 0; i < 6; i++) {
        QCOMPARE(!view.isSectionHidden(i), !hide1.contains(i));
        view.setSectionHidden(i, hide2.contains(i));
    }

    QCOMPARE(view.hiddenSectionCount(), hide2.count());
    for (int i = 0; i < 6; i++) {
        QCOMPARE(!view.isSectionHidden(i), !hide2.contains(i));
    }

}

void tst_QHeaderView::task248050_hideRow()
{
    //this is the sequence of events that make the task fail
    protected_QHeaderView header(Qt::Vertical);
    QStandardItemModel model(0, 1);
    header.setStretchLastSection(false);
    header.setDefaultSectionSize(17);
    header.setModel(&model);
    header.doItemsLayout();

    model.setRowCount(3);

    QCOMPARE(header.sectionPosition(2), 17*2);

    header.hideSection(1);
    QCOMPARE(header.sectionPosition(2), 17);

    QTest::qWait(100);
    //the size of the section shouldn't have changed
    QCOMPARE(header.sectionPosition(2), 17);
}


//returns 0 if everything is fine.
static int checkHeaderViewOrder(QHeaderView *view, const QVector<int> &expected)
{
    if (view->count() != expected.count())
        return 1;

    for (int i = 0; i < expected.count(); i++) {
        if (view->logicalIndex(i) != expected.at(i))
            return i + 10;
        if (view->visualIndex(expected.at(i)) != i)
            return i + 100;
    }
    return 0;
}


void tst_QHeaderView::QTBUG6058_reset()
{
    QStringListModel model1( QStringList() << "0" << "1" << "2" << "3" << "4" << "5" );
    QStringListModel model2( QStringList() << "a" << "b" << "c" );
    QSortFilterProxyModel proxy;

    QHeaderView view(Qt::Vertical);
    view.setModel(&proxy);
    view.show();
    QTest::qWait(20);

    proxy.setSourceModel(&model1);
    QApplication::processEvents();
    view.swapSections(0,2);
    view.swapSections(1,4);
    QApplication::processEvents();
    QCOMPARE(checkHeaderViewOrder(&view, QVector<int>() << 2 << 4 << 0 << 3 << 1 << 5) , 0);

    proxy.setSourceModel(&model2);
    QApplication::processEvents();
    QCOMPARE(checkHeaderViewOrder(&view, QVector<int>() << 2 << 0 << 1 ) , 0);

    proxy.setSourceModel(&model1);
    QApplication::processEvents();
    QCOMPARE(checkHeaderViewOrder(&view, QVector<int>() << 2 << 0 << 1 << 3 << 4 << 5 ) , 0);
}

void tst_QHeaderView::QTBUG7833_sectionClicked()
{




    QTableView tv;
    QStandardItemModel *sim = new QStandardItemModel(&tv);
    QSortFilterProxyModel *proxyModel = new QSortFilterProxyModel(&tv);
    proxyModel->setSourceModel(sim);
    proxyModel->setDynamicSortFilter(true);
    proxyModel->setSortCaseSensitivity(Qt::CaseInsensitive);

    QList<QStandardItem *> row;
    for (int i = 0; i < 12; i++)
        row.append(new QStandardItem(QString(QLatin1Char('A' + i))));
    sim->appendRow(row);
    row.clear();
    for (int i = 12; i > 0; i--)
        row.append(new QStandardItem(QString(QLatin1Char('A' + i))));
    sim->appendRow(row);

    tv.setSortingEnabled(true);
    tv.horizontalHeader()->setSortIndicatorShown(true);
    tv.horizontalHeader()->setClickable(true);
    tv.horizontalHeader()->setStretchLastSection(true);
    tv.horizontalHeader()->setResizeMode(QHeaderView::Interactive);

    tv.setModel(proxyModel);
    tv.setColumnHidden(5, true);
    tv.setColumnHidden(6, true);
    tv.horizontalHeader()->swapSections(8, 10);
    tv.sortByColumn(1, Qt::AscendingOrder);

    QSignalSpy clickedSpy(tv.horizontalHeader(), SIGNAL(sectionClicked(int)));
    QSignalSpy pressedSpy(tv.horizontalHeader(), SIGNAL(sectionPressed(int)));


    QTest::mouseClick(tv.horizontalHeader()->viewport(), Qt::LeftButton, Qt::NoModifier,
                      QPoint(tv.horizontalHeader()->sectionViewportPosition(11) + 5, 5));
    QCOMPARE(clickedSpy.count(), 1);
    QCOMPARE(pressedSpy.count(), 1);
    QCOMPARE(clickedSpy.at(0).at(0).toInt(), 11);
    QCOMPARE(pressedSpy.at(0).at(0).toInt(), 11);

    QTest::mouseClick(tv.horizontalHeader()->viewport(), Qt::LeftButton, Qt::NoModifier,
                      QPoint(tv.horizontalHeader()->sectionViewportPosition(8) + 5, 5));

    QCOMPARE(clickedSpy.count(), 2);
    QCOMPARE(pressedSpy.count(), 2);
    QCOMPARE(clickedSpy.at(1).at(0).toInt(), 8);
    QCOMPARE(pressedSpy.at(1).at(0).toInt(), 8);

    QTest::mouseClick(tv.horizontalHeader()->viewport(), Qt::LeftButton, Qt::NoModifier,
                      QPoint(tv.horizontalHeader()->sectionViewportPosition(0) + 5, 5));

    QCOMPARE(clickedSpy.count(), 3);
    QCOMPARE(pressedSpy.count(), 3);
    QCOMPARE(clickedSpy.at(2).at(0).toInt(), 0);
    QCOMPARE(pressedSpy.at(2).at(0).toInt(), 0);
}

void tst_QHeaderView::QTBUG8650_crashOnInsertSections()
{
    QStringList headerLabels;
    QHeaderView view(Qt::Horizontal);
    QStandardItemModel model(2,2);
    view.setModel(&model);
    view.moveSection(1, 0);
    view.hideSection(0);

    QList<QStandardItem *> items;
    items << new QStandardItem("c");
    model.insertColumn(0, items);
}

void tst_QHeaderView::QTBUG12268_hiddenMovedSectionSorting()
{
    QTableView view;
    QStandardItemModel *model = new QStandardItemModel(4,3, &view);
    for (int i = 0; i< model->rowCount(); ++i)
        for (int j = 0; j< model->columnCount(); ++j)
            model->setData(model->index(i,j), QString("item [%1,%2]").arg(i).arg(j));
    view.setModel(model);
    view.horizontalHeader()->setMovable(true);
    view.setSortingEnabled(true);
    view.sortByColumn(1, Qt::AscendingOrder);
    view.horizontalHeader()->moveSection(0,2);
    view.setColumnHidden(1, true);
    view.show();
    QTest::qWaitForWindowShown(&view);
    QCOMPARE(view.horizontalHeader()->hiddenSectionCount(), 1);
    QTest::mouseClick(view.horizontalHeader()->viewport(), Qt::LeftButton);
    QCOMPARE(view.horizontalHeader()->hiddenSectionCount(), 1);
}

QTEST_MAIN(tst_QHeaderView)
#include "tst_qheaderview.moc"
