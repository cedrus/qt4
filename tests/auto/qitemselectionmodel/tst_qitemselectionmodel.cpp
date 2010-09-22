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

#include <QtGui/QtGui>

//TESTED_CLASS=
//TESTED_FILES=

class tst_QItemSelectionModel : public QObject
{
    Q_OBJECT

public:
    tst_QItemSelectionModel();
    virtual ~tst_QItemSelectionModel();


public slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
private slots:
    void clear_data();
    void clear();
    void clearAndSelect();
    void toggleSelection();
    void select_data();
    void select();
    void persistentselections_data();
    void persistentselections();
    void resetModel();
    void removeRows_data();
    void removeRows();
    void removeColumns_data();
    void removeColumns();
    void modelLayoutChanged_data();
    void modelLayoutChanged();
    void selectedRows_data();
    void selectedRows();
    void selectedColumns_data();
    void selectedColumns();
    void setCurrentIndex();
    void splitOnInsert();
    void task196285_rowIntersectsSelection();
    void unselectable();
    void task220420_selectedIndexes();
    void task240734_layoutChanged();
    void merge_data();
    void merge();
    void task119433_isRowSelected();
    void task252069_rowIntersectsSelection();
    void task232634_childrenDeselectionSignal();
    void task260134_layoutChangedWithAllSelected();
    void QTBUG5671_layoutChangedWithAllSelected();
    void QTBUG2804_layoutChangedTreeSelection();

private:
    QAbstractItemModel *model;
    QItemSelectionModel *selection;
};

QDataStream &operator<<(QDataStream &, const QModelIndex &);
QDataStream &operator>>(QDataStream &, QModelIndex &);
QDataStream &operator<<(QDataStream &, const QModelIndexList &);
QDataStream &operator>>(QDataStream &, QModelIndexList &);

typedef QList<int> IntList;
typedef QPair<int, int> IntPair;
typedef QList<IntPair> PairList;


Q_DECLARE_METATYPE(PairList)
Q_DECLARE_METATYPE(QModelIndex)
Q_DECLARE_METATYPE(QModelIndexList)
Q_DECLARE_METATYPE(IntList)
Q_DECLARE_METATYPE(QItemSelection)

class QStreamHelper: public QAbstractItemModel
{
public:
    QStreamHelper() {}
    static QModelIndex create(int row = -1, int column = -1, void *data = 0)
    {
        QStreamHelper helper;
        return helper.QAbstractItemModel::createIndex(row, column, data);
    }

    QModelIndex index(int, int, const QModelIndex&) const
        { return QModelIndex(); }
    QModelIndex parent(const QModelIndex&) const
        { return QModelIndex(); }
    int rowCount(const QModelIndex & = QModelIndex()) const
        { return 0; }
    int columnCount(const QModelIndex & = QModelIndex()) const
        { return 0; }
    QVariant data(const QModelIndex &, int = Qt::DisplayRole) const
        { return QVariant(); }
    bool hasChildren(const QModelIndex &) const
        { return false; }
};

QDataStream &operator<<(QDataStream &s, const QModelIndex &input)
{
    s << input.row()
      << input.column()
      << reinterpret_cast<qlonglong>(input.internalPointer());
    return s;
}

QDataStream &operator>>(QDataStream &s, QModelIndex &output)
{
    int r, c;
    qlonglong ptr;
    s >> r;
    s >> c;
    s >> ptr;
    output = QStreamHelper::create(r, c, reinterpret_cast<void *>(ptr));
    return s;
}

QDataStream &operator<<(QDataStream &s, const QModelIndexList &input)
{
    s << input.count();
    for (int i=0; i<input.count(); ++i)
        s << input.at(i);
    return s;
}

QDataStream &operator>>(QDataStream &s, QModelIndexList &output)
{
    QModelIndex tmpIndex;
    int count;
    s >> count;
    for (int i=0; i<count; ++i) {
        s >> tmpIndex;
        output << tmpIndex;
    }
    return s;
}

tst_QItemSelectionModel::tst_QItemSelectionModel() : model(0), selection(0)
{
}

tst_QItemSelectionModel::~tst_QItemSelectionModel()
{
}

/*
  This test usually uses a model with a 5x5 table
  -------------------------------------------
  |  0,0  |  0,1    |  0,2  |  0,3    |  0,4  |
  -------------------------------------------
  |  1,0  |  1,1    |  1,2  |  1,3    |  1,4  |
  -------------------------------------------
  |  2,0  |  2,1    |  2,2  |  2,3    |  2,4  |
  -------------------------------------------
  |  3,0  |  3,1    |  3,2  |  3,3    |  3,4  |
  -------------------------------------------
  |  4,0  |  4,1    |  4,2  |  4,3    |  4,4  |
  -------------------------------------------

  ...that for each row has a children in a new 5x5 table ad infinitum.

*/
void tst_QItemSelectionModel::initTestCase()
{
    qRegisterMetaType<QItemSelection>("QItemSelection");

    model = new QStandardItemModel(5, 5);
    QModelIndex parent = model->index(0, 0, QModelIndex());
    model->insertRows(0, 5, parent);
    model->insertColumns(0, 5, parent);
    selection  = new QItemSelectionModel(model);
}

void tst_QItemSelectionModel::cleanupTestCase()
{
    delete selection;
    delete model;
}

void tst_QItemSelectionModel::init()
{
    selection->clear();
    while (model->rowCount(QModelIndex()) > 5)
        model->removeRow(0, QModelIndex());
    while (model->rowCount(QModelIndex()) < 5)
        model->insertRow(0, QModelIndex());
}

void tst_QItemSelectionModel::clear_data()
{
    QTest::addColumn<QModelIndexList>("indexList");
    QTest::addColumn<IntList>("commandList");
    {
        QModelIndexList index;
        IntList command;
        index << model->index(0, 0, QModelIndex());
        command << (QItemSelectionModel::Select | QItemSelectionModel::Rows);
        index << model->index(1, 0, QModelIndex());
        command << (QItemSelectionModel::Select | QItemSelectionModel::Rows);
        QTest::newRow("(0, 0) and (1, 0): Select|Rows")
            << index
            << command;
    }
    {
        QModelIndexList index;
        IntList command;
        index << model->index(0, 0, QModelIndex());
        command << (QItemSelectionModel::Select | QItemSelectionModel::Columns);
        index << model->index(0, 1, QModelIndex());
        command << (QItemSelectionModel::Select | QItemSelectionModel::Columns);
        QTest::newRow("(0, 0) and (1, 0): Select|Columns")
            << index
            << command;
    }
    {
        QModelIndexList index;
        IntList command;
        index << model->index(0, 0, QModelIndex());
        command << QItemSelectionModel::Select;
        index << model->index(1, 1, QModelIndex());
        command << QItemSelectionModel::Select;
        index << model->index(2, 2, QModelIndex());
        command << QItemSelectionModel::SelectCurrent;
        QTest::newRow("(0, 0), (1, 1) and (2, 2): Select, Select, SelectCurrent")
            << index
            << command;
    }
    {
        QModelIndexList index;
        IntList command;
        index << model->index(0, 0, QModelIndex());
        command << QItemSelectionModel::Select;
        index << model->index(1, 1, QModelIndex());
        command << QItemSelectionModel::Select;
        index << model->index(1, 1, QModelIndex());
        command << QItemSelectionModel::Toggle;
        QTest::newRow("(0, 0), (1, 1) and (1, 1): Select, Select, Toggle")
            << index
            << command;
    }
    {
        QModelIndexList index;
        IntList command;
        index << model->index(0, 0, model->index(0, 0, QModelIndex()));
        command << (QItemSelectionModel::Select | QItemSelectionModel::Rows);
        QTest::newRow("child (0, 0) of (0, 0): Select|Rows")
            << index
            << command;
    }
}

void tst_QItemSelectionModel::clear()
{
    QFETCH(QModelIndexList, indexList);
    QFETCH(IntList, commandList);

    // do selections
    for (int i=0; i<indexList.count(); ++i) {
        selection->select(indexList.at(i), (QItemSelectionModel::SelectionFlags)commandList.at(i));
    }
    // test that we have selected items
    QVERIFY(!selection->selectedIndexes().isEmpty());
    selection->clear();
    // test that they were all cleared
    QVERIFY(selection->selectedIndexes().isEmpty());
}

void tst_QItemSelectionModel::clearAndSelect()
{
    // populate selectionmodel
    selection->select(model->index(1, 1, QModelIndex()), QItemSelectionModel::Select);
    QCOMPARE(selection->selectedIndexes().count(), 1);
	QVERIFY(selection->hasSelection());

    // ClearAndSelect with empty selection
    QItemSelection emptySelection;
    selection->select(emptySelection, QItemSelectionModel::ClearAndSelect);

    // verify the selectionmodel is empty
    QVERIFY(selection->selectedIndexes().isEmpty());
	QVERIFY(selection->hasSelection()==false);
}

void tst_QItemSelectionModel::toggleSelection()
{
	//test the toggle selection and checks whether selectedIndex
	//and hasSelection returns the correct value

	selection->clearSelection();
    QCOMPARE(selection->selectedIndexes().count(), 0);
	QVERIFY(selection->hasSelection()==false);

	QModelIndex index=model->index(1, 1, QModelIndex());
    // populate selectionmodel
    selection->select(index, QItemSelectionModel::Toggle);
    QCOMPARE(selection->selectedIndexes().count(), 1);
	QVERIFY(selection->hasSelection()==true);

    selection->select(index, QItemSelectionModel::Toggle);
    QCOMPARE(selection->selectedIndexes().count(), 0);
	QVERIFY(selection->hasSelection()==false);

    // populate selectionmodel with rows
    selection->select(index, QItemSelectionModel::Toggle | QItemSelectionModel::Rows);
    QCOMPARE(selection->selectedIndexes().count(), model->columnCount());
	QVERIFY(selection->hasSelection()==true);

    selection->select(index, QItemSelectionModel::Toggle | QItemSelectionModel::Rows);
    QCOMPARE(selection->selectedIndexes().count(), 0);
	QVERIFY(selection->hasSelection()==false);

}


void tst_QItemSelectionModel::select_data()
{
    QTest::addColumn<QModelIndexList>("indexList");
    QTest::addColumn<bool>("useRanges");
    QTest::addColumn<IntList>("commandList");
    QTest::addColumn<QModelIndexList>("expectedList");

    {
        QModelIndexList index;
        QModelIndexList expected;
        IntList command;
        index << model->index(0, 0, QModelIndex());
        command << QItemSelectionModel::Select;
        expected << model->index(0, 0, QModelIndex());
        QTest::newRow("(0, 0): Select")
            << index
            << false
            << command
            << expected;
    }
    {
        QModelIndexList index;
        QModelIndexList expected;
        IntList command;
        index << model->index(0, 0, model->index(0, 0, QModelIndex()));
        command << QItemSelectionModel::Select;
        expected << model->index(0, 0, model->index(0, 0, QModelIndex()));
        QTest::newRow("child (0, 0) of (0, 0): Select")
            << index
            << false
            << command
            << expected;
    }
    {
        QModelIndexList index;
        QModelIndexList expected;
        IntList command;
        index << model->index(0, 0, QModelIndex());
        command << QItemSelectionModel::Deselect;
        QTest::newRow("(0, 0): Deselect")
            << index
            << false
            << command
            << expected;
    }
    {
        QModelIndexList index;
        QModelIndexList expected;
        IntList command;
        index << model->index(0, 0, QModelIndex());
        command << QItemSelectionModel::Toggle;
        expected << model->index(0, 0, QModelIndex());
        QTest::newRow("(0, 0): Toggle")
            << index
            << false
            << command
            << expected;
    }
    {
        QModelIndexList index;
        QModelIndexList expected;
        IntList command;
        index << model->index(0, 0, QModelIndex());
        command << QItemSelectionModel::Select;
        index << model->index(0, 0, QModelIndex());
        command << QItemSelectionModel::Toggle;
        QTest::newRow("(0, 0) and (0, 0): Select and Toggle")
            << index
            << false
            << command
            << expected;
    }
    {
        QModelIndexList index;
        QModelIndexList expected;
        IntList command;
        index << model->index(0, 0, QModelIndex());
        command << QItemSelectionModel::Select;
        index << model->index(0, 0, QModelIndex());
        command << QItemSelectionModel::Deselect;
        QTest::newRow("(0, 0) and (0, 0): Select and Deselect")
            << index
            << false
            << command
            << expected;
    }
    {
        QModelIndexList index;
        QModelIndexList expected;
        IntList command;
        index << model->index(0, 0, QModelIndex());
        command << QItemSelectionModel::Select;
        index << model->index(0, 0, model->index(0, 0, QModelIndex()));
        command << QItemSelectionModel::ClearAndSelect;
        expected << model->index(0, 0, model->index(0, 0, QModelIndex()));
        QTest::newRow("(0, 0) and child (0, 0) of (0, 0): Select and ClearAndSelect")
            << index
            << false
            << command
            << expected;
    }
    {
        QModelIndexList index;
        QModelIndexList expected;
        IntList command;
        index << model->index(0, 0, QModelIndex());
        index << model->index(4, 0, QModelIndex());
        command << QItemSelectionModel::Select;
        index << model->index(0, 1, QModelIndex());
        index << model->index(4, 1, QModelIndex());
        command << QItemSelectionModel::Select;
        index << model->index(0, 0, QModelIndex());
        index << model->index(4, 1, QModelIndex());
        command << QItemSelectionModel::Deselect;
        QTest::newRow("(0, 0 to 4, 0) and (0, 1 to 4, 1) and (0, 0 to 4, 1): Select and Select and Deselect")
            << index
            << true
            << command
            << expected;
    }
    {
        QModelIndexList index;
        QModelIndexList expected;
        IntList command;
        index << model->index(0, 0, QModelIndex());
        command << QItemSelectionModel::Select;
        index << model->index(4, 4, QModelIndex());
        command << QItemSelectionModel::Select;
        expected << model->index(0, 0, QModelIndex()) << model->index(4, 4, QModelIndex());
        QTest::newRow("(0, 0) and (4, 4): Select")
            << index
            << false
            << command
            << expected;
    }
    {
        QModelIndexList index;
        QModelIndexList expected;
        IntList command;
        index << model->index(0, 0, QModelIndex());
        command << QItemSelectionModel::Select;
        index << model->index(4, 4, QModelIndex());
        command << QItemSelectionModel::ClearAndSelect;
        expected << model->index(4, 4, QModelIndex());
        QTest::newRow("(0, 0) and (4, 4): Select and ClearAndSelect")
            << index
            << false
            << command
            << expected;
    }
    {
        QModelIndexList index;
        QModelIndexList expected;
        IntList command;
        index << model->index(0, 0, QModelIndex());
        command << (QItemSelectionModel::Select | QItemSelectionModel::Rows);
        index << model->index(4, 4, QModelIndex());
        command << (QItemSelectionModel::Select | QItemSelectionModel::Rows);
        expected << model->index(0, 0, QModelIndex())
                 << model->index(0, 1, QModelIndex())
                 << model->index(0, 2, QModelIndex())
                 << model->index(0, 3, QModelIndex())
                 << model->index(0, 4, QModelIndex())
                 << model->index(4, 0, QModelIndex())
                 << model->index(4, 1, QModelIndex())
                 << model->index(4, 2, QModelIndex())
                 << model->index(4, 3, QModelIndex())
                 << model->index(4, 4, QModelIndex());
        QTest::newRow("(0, 0) and (4, 4): Select|Rows")
            << index
            << false
            << command
            << expected;
    }
    {
        QModelIndexList index;
        QModelIndexList expected;
        IntList command;
        index << model->index(0, 0, model->index(0, 0, QModelIndex()));
        command << (QItemSelectionModel::Select | QItemSelectionModel::Rows);
        index << model->index(4, 4, model->index(0, 0, QModelIndex()));
        command << (QItemSelectionModel::Select | QItemSelectionModel::Rows);
        QModelIndex parent = model->index(0, 0, QModelIndex());
        expected << model->index(0, 0, parent)
                 << model->index(0, 1, parent)
                 << model->index(0, 2, parent)
                 << model->index(0, 3, parent)
                 << model->index(0, 4, parent)
                 << model->index(4, 0, parent)
                 << model->index(4, 1, parent)
                 << model->index(4, 2, parent)
                 << model->index(4, 3, parent)
                 << model->index(4, 4, parent);
        QTest::newRow("child (0, 0) and (4, 4) of (0, 0): Select|Rows")
            << index
            << false
            << command
            << expected;
    }
    {
        QModelIndexList index;
        QModelIndexList expected;
        IntList command;
        index << model->index(0, 0, QModelIndex());
        command << (QItemSelectionModel::Select | QItemSelectionModel::Columns);
        index << model->index(4, 4, QModelIndex());
        command << (QItemSelectionModel::Select | QItemSelectionModel::Columns);
        expected << model->index(0, 0, QModelIndex())
                 << model->index(1, 0, QModelIndex())
                 << model->index(2, 0, QModelIndex())
                 << model->index(3, 0, QModelIndex())
                 << model->index(4, 0, QModelIndex())
                 << model->index(0, 4, QModelIndex())
                 << model->index(1, 4, QModelIndex())
                 << model->index(2, 4, QModelIndex())
                 << model->index(3, 4, QModelIndex())
                 << model->index(4, 4, QModelIndex());
        QTest::newRow("(0, 0) and (4, 4): Select|Columns")
            << index
            << false
            << command
            << expected;
    }
    {
        QModelIndexList index;
        QModelIndexList expected;
        IntList command;
        index << model->index(0, 0, model->index(0, 0, QModelIndex()));
        command << (QItemSelectionModel::Select | QItemSelectionModel::Columns);
        index << model->index(4, 4, model->index(0, 0, QModelIndex()));
        command << (QItemSelectionModel::Select | QItemSelectionModel::Columns);
        expected << model->index(0, 0, model->index(0, 0, QModelIndex()))
                 << model->index(1, 0, model->index(0, 0, QModelIndex()))
                 << model->index(2, 0, model->index(0, 0, QModelIndex()))
                 << model->index(3, 0, model->index(0, 0, QModelIndex()))
                 << model->index(4, 0, model->index(0, 0, QModelIndex()))
                 << model->index(0, 4, model->index(0, 0, QModelIndex()))
                 << model->index(1, 4, model->index(0, 0, QModelIndex()))
                 << model->index(2, 4, model->index(0, 0, QModelIndex()))
                 << model->index(3, 4, model->index(0, 0, QModelIndex()))
                 << model->index(4, 4, model->index(0, 0, QModelIndex()));
        QTest::newRow("child (0, 0) and (4, 4) of (0, 0): Select|Columns")
            << index
            << false
            << command
            << expected;
    }
    {
        QModelIndexList index;
        QModelIndexList expected;
        IntList command;
        index << model->index(0, 0, QModelIndex());
        index << model->index(4, 0, QModelIndex());
        command << QItemSelectionModel::Select;
        expected << model->index(0, 0, QModelIndex())
                 << model->index(1, 0, QModelIndex())
                 << model->index(2, 0, QModelIndex())
                 << model->index(3, 0, QModelIndex())
                 << model->index(4, 0, QModelIndex());
        QTest::newRow("(0, 0 to 4, 0): Select")
            << index
            << true
            << command
            << expected;
    }
    /* ### FAILS
    {
        QModelIndexList index;
        QModelIndexList expected;
        IntList command;
        index << model->index(0, 0, QModelIndex());
        index << model->index(0, 0, model->index(0, 0, QModelIndex()));
        command << QItemSelectionModel::Select;
        QTest::newRow("(0, 0 to child 0, 0): Select")
            << index
            << true
            << command
            << expected;
    }
    */
    {
        QModelIndexList index;
        QModelIndexList expected;
        IntList command;
        index << model->index(0, 0, model->index(0, 0, QModelIndex()));
        index << model->index(0, 0, model->index(1, 0, QModelIndex()));
        command << QItemSelectionModel::Select;
        QTest::newRow("child (0, 0) of (0, 0) to child (0, 0) of (1, 0): Select")
            << index
            << true
            << command
            << expected;
    }
    {
        QModelIndexList index;
        QModelIndexList expected;
        IntList command;
        index << model->index(0, 0, QModelIndex());
        index << model->index(4, 4, QModelIndex());
        command << QItemSelectionModel::Select;
        expected << model->index(0, 0, QModelIndex())
                 << model->index(0, 1, QModelIndex())
                 << model->index(0, 2, QModelIndex())
                 << model->index(0, 3, QModelIndex())
                 << model->index(0, 4, QModelIndex())
                 << model->index(1, 0, QModelIndex())
                 << model->index(1, 1, QModelIndex())
                 << model->index(1, 2, QModelIndex())
                 << model->index(1, 3, QModelIndex())
                 << model->index(1, 4, QModelIndex())
                 << model->index(2, 0, QModelIndex())
                 << model->index(2, 1, QModelIndex())
                 << model->index(2, 2, QModelIndex())
                 << model->index(2, 3, QModelIndex())
                 << model->index(2, 4, QModelIndex())
                 << model->index(3, 0, QModelIndex())
                 << model->index(3, 1, QModelIndex())
                 << model->index(3, 2, QModelIndex())
                 << model->index(3, 3, QModelIndex())
                 << model->index(3, 4, QModelIndex())
                 << model->index(4, 0, QModelIndex())
                 << model->index(4, 1, QModelIndex())
                 << model->index(4, 2, QModelIndex())
                 << model->index(4, 3, QModelIndex())
                 << model->index(4, 4, QModelIndex());
        QTest::newRow("(0, 0 to 4, 4): Select")
            << index
            << true
            << command
            << expected;
    }
    {
        QModelIndexList index;
        QModelIndexList expected;
        IntList command;
        index << model->index(0, 0, QModelIndex());
        index << model->index(4, 0, QModelIndex());
        command << (QItemSelectionModel::Select | QItemSelectionModel::Rows);
        expected << model->index(0, 0, QModelIndex())
                 << model->index(0, 1, QModelIndex())
                 << model->index(0, 2, QModelIndex())
                 << model->index(0, 3, QModelIndex())
                 << model->index(0, 4, QModelIndex())
                 << model->index(1, 0, QModelIndex())
                 << model->index(1, 1, QModelIndex())
                 << model->index(1, 2, QModelIndex())
                 << model->index(1, 3, QModelIndex())
                 << model->index(1, 4, QModelIndex())
                 << model->index(2, 0, QModelIndex())
                 << model->index(2, 1, QModelIndex())
                 << model->index(2, 2, QModelIndex())
                 << model->index(2, 3, QModelIndex())
                 << model->index(2, 4, QModelIndex())
                 << model->index(3, 0, QModelIndex())
                 << model->index(3, 1, QModelIndex())
                 << model->index(3, 2, QModelIndex())
                 << model->index(3, 3, QModelIndex())
                 << model->index(3, 4, QModelIndex())
                 << model->index(4, 0, QModelIndex())
                 << model->index(4, 1, QModelIndex())
                 << model->index(4, 2, QModelIndex())
                 << model->index(4, 3, QModelIndex())
                 << model->index(4, 4, QModelIndex());
        QTest::newRow("(0, 0 to 4, 0): Select|Rows")
            << index
            << true
            << command
            << expected;
    }
    {
        QModelIndexList index;
        QModelIndexList expected;
        IntList command;
        index << model->index(0, 0, QModelIndex());
        index << model->index(0, 4, QModelIndex());
        command << (QItemSelectionModel::Select | QItemSelectionModel::Columns);
        expected << model->index(0, 0, QModelIndex())
                 << model->index(0, 1, QModelIndex())
                 << model->index(0, 2, QModelIndex())
                 << model->index(0, 3, QModelIndex())
                 << model->index(0, 4, QModelIndex())
                 << model->index(1, 0, QModelIndex())
                 << model->index(1, 1, QModelIndex())
                 << model->index(1, 2, QModelIndex())
                 << model->index(1, 3, QModelIndex())
                 << model->index(1, 4, QModelIndex())
                 << model->index(2, 0, QModelIndex())
                 << model->index(2, 1, QModelIndex())
                 << model->index(2, 2, QModelIndex())
                 << model->index(2, 3, QModelIndex())
                 << model->index(2, 4, QModelIndex())
                 << model->index(3, 0, QModelIndex())
                 << model->index(3, 1, QModelIndex())
                 << model->index(3, 2, QModelIndex())
                 << model->index(3, 3, QModelIndex())
                 << model->index(3, 4, QModelIndex())
                 << model->index(4, 0, QModelIndex())
                 << model->index(4, 1, QModelIndex())
                 << model->index(4, 2, QModelIndex())
                 << model->index(4, 3, QModelIndex())
                 << model->index(4, 4, QModelIndex());
        QTest::newRow("(0, 0 to 0, 4): Select|Columns")
            << index
            << true
            << command
            << expected;
    }
    {
        QModelIndexList index;
        QModelIndexList expected;
        IntList command;
        index << model->index(0, 0, QModelIndex());
        index << model->index(4, 4, QModelIndex());
        command << (QItemSelectionModel::Select | QItemSelectionModel::Rows);
        expected << model->index(0, 0, QModelIndex())
                 << model->index(0, 1, QModelIndex())
                 << model->index(0, 2, QModelIndex())
                 << model->index(0, 3, QModelIndex())
                 << model->index(0, 4, QModelIndex())
                 << model->index(1, 0, QModelIndex())
                 << model->index(1, 1, QModelIndex())
                 << model->index(1, 2, QModelIndex())
                 << model->index(1, 3, QModelIndex())
                 << model->index(1, 4, QModelIndex())
                 << model->index(2, 0, QModelIndex())
                 << model->index(2, 1, QModelIndex())
                 << model->index(2, 2, QModelIndex())
                 << model->index(2, 3, QModelIndex())
                 << model->index(2, 4, QModelIndex())
                 << model->index(3, 0, QModelIndex())
                 << model->index(3, 1, QModelIndex())
                 << model->index(3, 2, QModelIndex())
                 << model->index(3, 3, QModelIndex())
                 << model->index(3, 4, QModelIndex())
                 << model->index(4, 0, QModelIndex())
                 << model->index(4, 1, QModelIndex())
                 << model->index(4, 2, QModelIndex())
                 << model->index(4, 3, QModelIndex())
                 << model->index(4, 4, QModelIndex());
        QTest::newRow("(0, 0 to 4, 4): Select|Rows")
            << index
            << true
            << command
            << expected;
    }
    {
        QModelIndexList index;
        QModelIndexList expected;
        IntList command;
        index << model->index(0, 0, QModelIndex());
        index << model->index(4, 4, QModelIndex());
        command << (QItemSelectionModel::Select | QItemSelectionModel::Columns);
        expected << model->index(0, 0, QModelIndex())
                 << model->index(0, 1, QModelIndex())
                 << model->index(0, 2, QModelIndex())
                 << model->index(0, 3, QModelIndex())
                 << model->index(0, 4, QModelIndex())
                 << model->index(1, 0, QModelIndex())
                 << model->index(1, 1, QModelIndex())
                 << model->index(1, 2, QModelIndex())
                 << model->index(1, 3, QModelIndex())
                 << model->index(1, 4, QModelIndex())
                 << model->index(2, 0, QModelIndex())
                 << model->index(2, 1, QModelIndex())
                 << model->index(2, 2, QModelIndex())
                 << model->index(2, 3, QModelIndex())
                 << model->index(2, 4, QModelIndex())
                 << model->index(3, 0, QModelIndex())
                 << model->index(3, 1, QModelIndex())
                 << model->index(3, 2, QModelIndex())
                 << model->index(3, 3, QModelIndex())
                 << model->index(3, 4, QModelIndex())
                 << model->index(4, 0, QModelIndex())
                 << model->index(4, 1, QModelIndex())
                 << model->index(4, 2, QModelIndex())
                 << model->index(4, 3, QModelIndex())
                 << model->index(4, 4, QModelIndex());
        QTest::newRow("(0, 0 to 4, 4): Select|Columns")
            << index
            << true
            << command
            << expected;
    }
    {
        QModelIndexList index;
        QModelIndexList expected;
        IntList command;
        index << model->index(0, 2, QModelIndex());
        index << model->index(4, 2, QModelIndex());
        command << QItemSelectionModel::Select;
        index << model->index(2, 0, QModelIndex());
        index << model->index(2, 4, QModelIndex());
        command << QItemSelectionModel::Select;
        expected << model->index(0, 2, QModelIndex())
                 << model->index(1, 2, QModelIndex())
                 << model->index(2, 2, QModelIndex())
                 << model->index(3, 2, QModelIndex())
                 << model->index(4, 2, QModelIndex())
                 << model->index(2, 0, QModelIndex())
                 << model->index(2, 1, QModelIndex())
                 << model->index(2, 3, QModelIndex())
                 << model->index(2, 4, QModelIndex());
        QTest::newRow("(0, 2 to 4, 2) and (2, 0 to 2, 4): Select")
            << index
            << true
            << command
            << expected;
    }
    {
        QModelIndexList index;
        QModelIndexList expected;
        IntList command;
        index << model->index(0, 2, QModelIndex());
        index << model->index(4, 2, QModelIndex());
        command << QItemSelectionModel::Select;
        index << model->index(2, 0, QModelIndex());
        index << model->index(2, 4, QModelIndex());
        command << QItemSelectionModel::SelectCurrent;
        expected << model->index(2, 0, QModelIndex())
                 << model->index(2, 1, QModelIndex())
                 << model->index(2, 2, QModelIndex())
                 << model->index(2, 3, QModelIndex())
                 << model->index(2, 4, QModelIndex());
        QTest::newRow("(0, 2 to 4, 2) and (2, 0 to 2, 4): Select and SelectCurrent")
            << index
            << true
            << command
            << expected;
    }
    {
        QModelIndexList index;
        QModelIndexList expected;
        IntList command;
        index << model->index(0, 2, QModelIndex());
        index << model->index(4, 2, QModelIndex());
        command << QItemSelectionModel::Select;
        index << model->index(2, 0, QModelIndex());
        index << model->index(2, 4, QModelIndex());
        command << QItemSelectionModel::Toggle;
        expected << model->index(0, 2, QModelIndex())
                 << model->index(1, 2, QModelIndex())
                 << model->index(3, 2, QModelIndex())
                 << model->index(4, 2, QModelIndex())
                 << model->index(2, 0, QModelIndex())
                 << model->index(2, 1, QModelIndex())
                 << model->index(2, 3, QModelIndex())
                 << model->index(2, 4, QModelIndex());
        QTest::newRow("(0, 2 to 4, 2) and (2, 0 to 2, 4): Select and Toggle")
            << index
            << true
            << command
            << expected;
    }
    {
        QModelIndexList index;
        QModelIndexList expected;
        IntList command;
        index << model->index(0, 2, QModelIndex());
        index << model->index(4, 2, QModelIndex());
        command << QItemSelectionModel::Select;
        index << model->index(2, 0, QModelIndex());
        index << model->index(2, 4, QModelIndex());
        command << QItemSelectionModel::Deselect;
        expected << model->index(0, 2, QModelIndex())
                 << model->index(1, 2, QModelIndex())
                 << model->index(3, 2, QModelIndex())
                 << model->index(4, 2, QModelIndex());
        QTest::newRow("(0, 2 to 4, 2) and (2, 0 to 2, 4): Select and Deselect")
            << index
            << true
            << command
            << expected;
    }

    {
        QModelIndexList index;
        QModelIndexList expected;
        IntList command;
        index << model->index(0, 0, QModelIndex());
        index << model->index(2, 2, QModelIndex());
        command << QItemSelectionModel::Select;

        index << model->index(0, 0, QModelIndex());
        index << model->index(0, 0, QModelIndex());
        command << QItemSelectionModel::Toggle;

        expected << model->index(0, 1, QModelIndex())
                 << model->index(0, 2, QModelIndex())
                 << model->index(1, 0, QModelIndex())
                 << model->index(1, 1, QModelIndex())
                 << model->index(1, 2, QModelIndex())
                 << model->index(2, 0, QModelIndex())
                 << model->index(2, 1, QModelIndex())
                 << model->index(2, 2, QModelIndex());

        QTest::newRow("(0, 0 to 2, 2) and (0, 0 to 0, 0): Select and Toggle at selection boundary")
            << index
            << true
            << command
            << expected;
    }

    {
        QModelIndexList index;
        QModelIndexList expected;
        IntList command;
        index << model->index(0, 0, QModelIndex());
        index << model->index(2, 2, QModelIndex());
        command << QItemSelectionModel::Select;

        index << model->index(0, 1, QModelIndex());
        index << model->index(0, 1, QModelIndex());
        command << QItemSelectionModel::Toggle;

        expected << model->index(0, 0, QModelIndex())
                 << model->index(0, 2, QModelIndex())
                 << model->index(1, 0, QModelIndex())
                 << model->index(1, 1, QModelIndex())
                 << model->index(1, 2, QModelIndex())
                 << model->index(2, 0, QModelIndex())
                 << model->index(2, 1, QModelIndex())
                 << model->index(2, 2, QModelIndex());

        QTest::newRow("(0, 0 to 2, 2) and (0, 1 to 0, 1): Select and Toggle at selection boundary")
            << index
            << true
            << command
            << expected;
    }

    {
        QModelIndexList index;
        QModelIndexList expected;
        IntList command;
        index << model->index(0, 0, QModelIndex());
        index << model->index(2, 2, QModelIndex());
        command << QItemSelectionModel::Select;

        index << model->index(0, 2, QModelIndex());
        index << model->index(0, 2, QModelIndex());
        command << QItemSelectionModel::Toggle;

        expected << model->index(0, 0, QModelIndex())
                 << model->index(0, 1, QModelIndex())
                 << model->index(1, 0, QModelIndex())
                 << model->index(1, 1, QModelIndex())
                 << model->index(1, 2, QModelIndex())
                 << model->index(2, 0, QModelIndex())
                 << model->index(2, 1, QModelIndex())
                 << model->index(2, 2, QModelIndex());

        QTest::newRow("(0, 0 to 2, 2) and (0, 2 to 0, 2): Select and Toggle at selection boundary")
            << index
            << true
            << command
            << expected;
    }

    {
        QModelIndexList index;
        QModelIndexList expected;
        IntList command;
        index << model->index(0, 0, QModelIndex());
        index << model->index(2, 2, QModelIndex());
        command << QItemSelectionModel::Select;

        index << model->index(1, 0, QModelIndex());
        index << model->index(1, 0, QModelIndex());
        command << QItemSelectionModel::Toggle;

        expected << model->index(0, 0, QModelIndex())
                 << model->index(0, 1, QModelIndex())
                 << model->index(0, 2, QModelIndex())
                 << model->index(1, 1, QModelIndex())
                 << model->index(1, 2, QModelIndex())
                 << model->index(2, 0, QModelIndex())
                 << model->index(2, 1, QModelIndex())
                 << model->index(2, 2, QModelIndex());

        QTest::newRow("(0, 0 to 2, 2) and (1, 0 to 1, 0): Select and Toggle at selection boundary")
            << index
            << true
            << command
            << expected;
    }

    {
        QModelIndexList index;
        QModelIndexList expected;
        IntList command;
        index << model->index(0, 0, QModelIndex());
        index << model->index(2, 2, QModelIndex());
        command << QItemSelectionModel::Select;

        index << model->index(1, 1, QModelIndex());
        index << model->index(1, 1, QModelIndex());
        command << QItemSelectionModel::Toggle;

        expected << model->index(0, 0, QModelIndex())
                 << model->index(0, 1, QModelIndex())
                 << model->index(0, 2, QModelIndex())
                 << model->index(1, 0, QModelIndex())
                 << model->index(1, 2, QModelIndex())
                 << model->index(2, 0, QModelIndex())
                 << model->index(2, 1, QModelIndex())
                 << model->index(2, 2, QModelIndex());

        QTest::newRow("(0, 0 to 2, 2) and (1, 1 to 1, 1): Select and Toggle at selection boundary")
            << index
            << true
            << command
            << expected;
    }
    {
        QModelIndexList index;
        QModelIndexList expected;
        IntList command;
        index << model->index(0, 0, QModelIndex());
        index << model->index(2, 2, QModelIndex());
        command << QItemSelectionModel::Select;

        index << model->index(1, 2, QModelIndex());
        index << model->index(1, 2, QModelIndex());
        command << QItemSelectionModel::Toggle;

        expected << model->index(0, 0, QModelIndex())
                 << model->index(0, 1, QModelIndex())
                 << model->index(0, 2, QModelIndex())
                 << model->index(1, 0, QModelIndex())
                 << model->index(1, 1, QModelIndex())
                 << model->index(2, 0, QModelIndex())
                 << model->index(2, 1, QModelIndex())
                 << model->index(2, 2, QModelIndex());

        QTest::newRow("(0, 0 to 2, 2) and (1, 2 to 1, 2): Select and Toggle at selection boundary")
            << index
            << true
            << command
            << expected;
    }
    {
        QModelIndexList index;
        QModelIndexList expected;
        IntList command;
        index << model->index(0, 0, QModelIndex());
        index << model->index(2, 2, QModelIndex());
        command << QItemSelectionModel::Select;

        index << model->index(2, 0, QModelIndex());
        index << model->index(2, 0, QModelIndex());
        command << QItemSelectionModel::Toggle;

        expected << model->index(0, 0, QModelIndex())
                 << model->index(0, 1, QModelIndex())
                 << model->index(0, 2, QModelIndex())
                 << model->index(1, 0, QModelIndex())
                 << model->index(1, 1, QModelIndex())
                 << model->index(1, 2, QModelIndex())
                 << model->index(2, 1, QModelIndex())
                 << model->index(2, 2, QModelIndex());

        QTest::newRow("(0, 0 to 2, 2) and (2, 0 to 2, 0): Select and Toggle at selection boundary")
            << index
            << true
            << command
            << expected;
    }
    {
        QModelIndexList index;
        QModelIndexList expected;
        IntList command;
        index << model->index(0, 0, QModelIndex());
        index << model->index(2, 2, QModelIndex());
        command << QItemSelectionModel::Select;

        index << model->index(2, 1, QModelIndex());
        index << model->index(2, 1, QModelIndex());
        command << QItemSelectionModel::Toggle;

        expected << model->index(0, 0, QModelIndex())
                 << model->index(0, 1, QModelIndex())
                 << model->index(0, 2, QModelIndex())
                 << model->index(1, 0, QModelIndex())
                 << model->index(1, 1, QModelIndex())
                 << model->index(1, 2, QModelIndex())
                 << model->index(2, 0, QModelIndex())
                 << model->index(2, 2, QModelIndex());

        QTest::newRow("(0, 0 to 2, 2) and (2, 1 to 2, 1): Select and Toggle at selection boundary")
            << index
            << true
            << command
            << expected;
    }
    {
        QModelIndexList index;
        QModelIndexList expected;
        IntList command;

        index << model->index(0, 0, QModelIndex());
        index << model->index(2, 2, QModelIndex());
        command << QItemSelectionModel::Select;

        index << model->index(2, 2, QModelIndex());
        index << model->index(2, 2, QModelIndex());
        command << QItemSelectionModel::Toggle;

        expected << model->index(0, 0, QModelIndex())
                 << model->index(0, 1, QModelIndex())
                 << model->index(0, 2, QModelIndex())
                 << model->index(1, 0, QModelIndex())
                 << model->index(1, 1, QModelIndex())
                 << model->index(1, 2, QModelIndex())
                 << model->index(2, 0, QModelIndex())
                 << model->index(2, 1, QModelIndex());

        QTest::newRow("(0, 0 to 2, 2) and (2, 2 to 2, 2): Select and Toggle at selection boundary")
            << index
            << true
            << command
            << expected;
    }
    {
        QModelIndexList indexes;
	IntList commands;
	QModelIndexList expected;

	indexes  << model->index(0, 0, QModelIndex()) << model->index(0, 0, QModelIndex()) // press 0
		 << model->index(0, 0, QModelIndex()) << model->index(0, 0, QModelIndex()) // release 0
		 << model->index(1, 0, QModelIndex()) << model->index(1, 0, QModelIndex()) // press 1
		 << model->index(1, 0, QModelIndex()) << model->index(1, 0, QModelIndex()) // release 1
		 << model->index(2, 0, QModelIndex()) << model->index(2, 0, QModelIndex()) // press 2
		 << model->index(2, 0, QModelIndex()) << model->index(2, 0, QModelIndex()) // release 2
		 << model->index(3, 0, QModelIndex()) << model->index(3, 0, QModelIndex()) // press 3
		 << model->index(3, 0, QModelIndex()) << model->index(3, 0, QModelIndex()) // release 3
		 << model->index(2, 0, QModelIndex()) << model->index(2, 0, QModelIndex()) // press 2 again
		 << model->index(2, 0, QModelIndex()) << model->index(2, 0, QModelIndex());// move 2

	commands << (QItemSelectionModel::NoUpdate)                                // press 0
		 << (QItemSelectionModel::Toggle|QItemSelectionModel::Rows)        // release 0
		 << (QItemSelectionModel::NoUpdate)                                // press 1
		 << (QItemSelectionModel::Toggle|QItemSelectionModel::Rows)        // release 1
		 << (QItemSelectionModel::NoUpdate)                                // press 2
		 << (QItemSelectionModel::Toggle|QItemSelectionModel::Rows)        // release 2
		 << (QItemSelectionModel::NoUpdate)                                // press 3
		 << (QItemSelectionModel::Toggle|QItemSelectionModel::Rows)        // release 3
	         << (QItemSelectionModel::NoUpdate)                                // press 2 again
		 << (QItemSelectionModel::Toggle/*Current*/|QItemSelectionModel::Rows);// move 2

	expected << model->index(0, 0, QModelIndex())
		 << model->index(0, 1, QModelIndex())
		 << model->index(0, 2, QModelIndex())
		 << model->index(0, 3, QModelIndex())
		 << model->index(0, 4, QModelIndex())

		 << model->index(1, 0, QModelIndex())
		 << model->index(1, 1, QModelIndex())
		 << model->index(1, 2, QModelIndex())
		 << model->index(1, 3, QModelIndex())
		 << model->index(1, 4, QModelIndex())
	  /*
		 << model->index(2, 0, QModelIndex())
		 << model->index(2, 1, QModelIndex())
		 << model->index(2, 2, QModelIndex())
		 << model->index(2, 3, QModelIndex())
		 << model->index(2, 4, QModelIndex())
	  */
		 << model->index(3, 0, QModelIndex())
		 << model->index(3, 1, QModelIndex())
		 << model->index(3, 2, QModelIndex())
		 << model->index(3, 3, QModelIndex())
		 << model->index(3, 4, QModelIndex());

        QTest::newRow("simulated treeview multiselection behavior")
            << indexes
            << true
            << commands
            << expected;
    }
}

void tst_QItemSelectionModel::select()
{
    QFETCH(QModelIndexList, indexList);
    QFETCH(bool, useRanges);
    QFETCH(IntList, commandList);
    QFETCH(QModelIndexList, expectedList);

    int lastCommand = 0;
    // do selections
    for (int i = 0; i<commandList.count(); ++i) {
        if (useRanges) {
            selection->select(QItemSelection(indexList.at(2*i), indexList.at(2*i+1)),
                              (QItemSelectionModel::SelectionFlags)commandList.at(i));
        } else {
            selection->select(indexList.at(i),
                              (QItemSelectionModel::SelectionFlags)commandList.at(i));
        }
        lastCommand = commandList.at(i);
    }


    QModelIndexList selectedList = selection->selectedIndexes();

    QVERIFY(selection->hasSelection()!=selectedList.isEmpty());

    // debug output
//     for (int i=0; i<selectedList.count(); ++i)
//         qDebug(QString("selected (%1, %2)")
//                .arg(selectedList.at(i).row())
//                .arg(selectedList.at(i).column()));

    // test that the number of indices are as expected
    QVERIFY2(selectedList.count() == expectedList.count(),
            QString("expected indices: %1 actual indices: %2")
            .arg(expectedList.count())
            .arg(selectedList.count()).toLatin1());

    // test existence of each index
    for (int i=0; i<expectedList.count(); ++i) {
        QVERIFY2(selectedList.contains(expectedList.at(i)),
                QString("expected index(%1, %2) not found in selectedIndexes()")
                .arg(expectedList.at(i).row())
                .arg(expectedList.at(i).column()).toLatin1());
    }

    // test that isSelected agrees
    for (int i=0; i<indexList.count(); ++i) {
        QModelIndex idx = indexList.at(i);
        QVERIFY2(selection->isSelected(idx) == selectedList.contains(idx),
                 QString("isSelected(index: %1, %2) does not match selectedIndexes()")
                 .arg(idx.row())
                 .arg(idx.column()).toLatin1());
    }

    //for now we assume Rows/Columns flag is the same for all commands, therefore we just check lastCommand
    // test that isRowSelected agrees
    if (lastCommand & QItemSelectionModel::Rows) {
        for (int i=0; i<selectedList.count(); ++i)
            QVERIFY2(selection->isRowSelected(selectedList.at(i).row(),
                                             model->parent(selectedList.at(i))),
                    QString("isRowSelected(row: %1) does not match selectedIndexes()")
                    .arg(selectedList.at(i).row()).toLatin1());
    }

    // test that isColumnSelected agrees
    if (lastCommand & QItemSelectionModel::Columns) {
        for (int i=0; i<selectedList.count(); ++i)
            QVERIFY2(selection->isColumnSelected(selectedList.at(i).column(),
                                                model->parent(selectedList.at(i))),
                    QString("isColumnSelected(column: %1) does not match selectedIndexes()")
                    .arg(selectedList.at(i).column()).toLatin1());
    }
}

void tst_QItemSelectionModel::persistentselections_data()
{
    QTest::addColumn<PairList>("indexList");
    QTest::addColumn<IntList>("commandList");
    QTest::addColumn<IntList>("insertRows"); // start, count
    QTest::addColumn<IntList>("insertColumns"); // start, count
    QTest::addColumn<IntList>("deleteRows"); // start, count
    QTest::addColumn<IntList>("deleteColumns"); // start, count
    QTest::addColumn<PairList>("expectedList");

    PairList index, expected;
    IntList command, insertRows, insertColumns, deleteRows, deleteColumns;


    index.clear(); expected.clear(); command.clear();
    insertRows.clear(); insertColumns.clear(); deleteRows.clear(); deleteColumns.clear();
    index << IntPair(0, 0);
    command << QItemSelectionModel::ClearAndSelect;
    deleteRows << 4 << 1;
    expected << IntPair(0, 0);
    QTest::newRow("ClearAndSelect (0, 0). Delete last row.")
        << index << command
        << insertRows << insertColumns << deleteRows << deleteColumns
        << expected;

     index.clear(); expected.clear(); command.clear();
     insertRows.clear(); insertColumns.clear(); deleteRows.clear(); deleteColumns.clear();
     index << IntPair(0, 0);
     command << QItemSelectionModel::ClearAndSelect;
     deleteRows << 0 << 1;
     QTest::newRow("ClearAndSelect (0, 0). Delete first row.")
         << index << command
         << insertRows << insertColumns << deleteRows << deleteColumns
         << expected;

     index.clear(); expected.clear(); command.clear();
     insertRows.clear(); insertColumns.clear(); deleteRows.clear(); deleteColumns.clear();
     index << IntPair(1, 0);
     command << QItemSelectionModel::ClearAndSelect;
     deleteRows << 0 << 1;
     expected << IntPair(0, 0);
     QTest::newRow("ClearAndSelect (1, 0). Delete first row.")
         << index << command
         << insertRows << insertColumns << deleteRows << deleteColumns
         << expected;

     index.clear(); expected.clear(); command.clear();
     insertRows.clear(); insertColumns.clear(); deleteRows.clear(); deleteColumns.clear();
     index << IntPair(0, 0);
     command << QItemSelectionModel::ClearAndSelect;
     insertRows << 5 << 1;
     expected << IntPair(0, 0);
     QTest::newRow("ClearAndSelect (0, 0). Append row.")
         << index << command
         << insertRows << insertColumns << deleteRows << deleteColumns
         << expected;

     index.clear(); expected.clear(); command.clear();
     insertRows.clear(); insertColumns.clear(); deleteRows.clear(); deleteColumns.clear();
     index << IntPair(0, 0);
     command << QItemSelectionModel::ClearAndSelect;
     insertRows << 0 << 1;
     expected << IntPair(1, 0);
     QTest::newRow("ClearAndSelect (0, 0). Insert before first row.")
         << index << command
         << insertRows << insertColumns << deleteRows << deleteColumns
         << expected;

     index.clear(); expected.clear(); command.clear();
     insertRows.clear(); insertColumns.clear(); deleteRows.clear(); deleteColumns.clear();
     index << IntPair(0, 0)
           << IntPair(4, 0);
     command << QItemSelectionModel::ClearAndSelect;
     insertRows << 5 << 1;
     expected << IntPair(0, 0)
              << IntPair(1, 0)
              << IntPair(2, 0)
              << IntPair(3, 0)
              << IntPair(4, 0);
     QTest::newRow("ClearAndSelect (0, 0) to (4, 0). Append row.")
         << index << command
         << insertRows << insertColumns << deleteRows << deleteColumns
         << expected;

     index.clear(); expected.clear(); command.clear();
     insertRows.clear(); insertColumns.clear(); deleteRows.clear(); deleteColumns.clear();
     index << IntPair(0, 0)
           << IntPair(4, 0);
     command << QItemSelectionModel::ClearAndSelect;
     insertRows << 0  << 1;
     expected << IntPair(1, 0)
              << IntPair(2, 0)
              << IntPair(3, 0)
              << IntPair(4, 0)
              << IntPair(5, 0);
     QTest::newRow("ClearAndSelect (0, 0) to (4, 0). Insert before first row.")
         << index << command
         << insertRows << insertColumns << deleteRows << deleteColumns
         << expected;

     index.clear(); expected.clear(); command.clear();
     insertRows.clear(); insertColumns.clear(); deleteRows.clear(); deleteColumns.clear();
     index << IntPair(0, 0)
           << IntPair(4, 0);
     command << QItemSelectionModel::ClearAndSelect;
     deleteRows << 0  << 1;
     expected << IntPair(0, 0)
              << IntPair(1, 0)
              << IntPair(2, 0)
              << IntPair(3, 0);
     QTest::newRow("ClearAndSelect (0, 0) to (4, 0). Delete first row.")
         << index << command
         << insertRows << insertColumns << deleteRows << deleteColumns
         << expected;

     index.clear(); expected.clear(); command.clear();
     insertRows.clear(); insertColumns.clear(); deleteRows.clear(); deleteColumns.clear();
     index << IntPair(0, 0)
           << IntPair(4, 0);
     command << QItemSelectionModel::ClearAndSelect;
     deleteRows << 4  << 1;
     expected << IntPair(0, 0)
              << IntPair(1, 0)
              << IntPair(2, 0)
              << IntPair(3, 0);
     QTest::newRow("ClearAndSelect (0, 0) to (4, 0). Delete last row.")
         << index << command
         << insertRows << insertColumns << deleteRows << deleteColumns
         << expected;

     index.clear(); expected.clear(); command.clear();
     insertRows.clear(); insertColumns.clear(); deleteRows.clear(); deleteColumns.clear();
     index << IntPair(0, 0)
           << IntPair(4, 0);
     command << QItemSelectionModel::ClearAndSelect;
     deleteRows << 1  << 3;
     expected << IntPair(0, 0)
              << IntPair(1, 0);
     QTest::newRow("ClearAndSelect (0, 0) to (4, 0). Deleting all but first and last row.")
         << index << command
         << insertRows << insertColumns << deleteRows << deleteColumns
         << expected;

     index.clear(); expected.clear(); command.clear();
     insertRows.clear(); insertColumns.clear(); deleteRows.clear(); deleteColumns.clear();
     index << IntPair(0, 0)
           << IntPair(4, 0);
     command << QItemSelectionModel::ClearAndSelect;
     insertRows << 1 << 1;
     expected << IntPair(0, 0)
         // the inserted row should not be selected
              << IntPair(2, 0)
              << IntPair(3, 0)
              << IntPair(4, 0)
              << IntPair(5, 0);
     QTest::newRow("ClearAndSelect (0, 0) to (4, 0). Insert after first row.")
         << index << command
         << insertRows << insertColumns << deleteRows << deleteColumns
         << expected;
}

void tst_QItemSelectionModel::persistentselections()
{
    QFETCH(PairList, indexList);
    QFETCH(IntList, commandList);
    QFETCH(IntList, insertRows);
    QFETCH(IntList, insertColumns);
    QFETCH(IntList, deleteRows);
    QFETCH(IntList, deleteColumns);
    QFETCH(PairList, expectedList);

    // make sure the model is sane (5x5)
    QCOMPARE(model->rowCount(QModelIndex()), 5);
    QCOMPARE(model->columnCount(QModelIndex()), 5);

    // do selections
    for (int i=0; i<commandList.count(); ++i) {
        if (indexList.count() == commandList.count()) {
            QModelIndex index = model->index(indexList.at(i).first,
                                             indexList.at(i).second,
                                             QModelIndex());
            selection->select(index, (QItemSelectionModel::SelectionFlags)commandList.at(i));
        } else {
            QModelIndex tl = model->index(indexList.at(2*i).first,
                                          indexList.at(2*i).second,
                                          QModelIndex());
            QModelIndex br = model->index(indexList.at(2*i+1).first,
                                          indexList.at(2*i+1).second,
                                          QModelIndex());
            selection->select(QItemSelection(tl, br),
                              (QItemSelectionModel::SelectionFlags)commandList.at(i));
        }
    }
    // test that we have selected items
    QVERIFY(!selection->selectedIndexes().isEmpty());
    QVERIFY(selection->hasSelection());

    // insert/delete row and/or columns
    if (insertRows.count() > 1)
        model->insertRows(insertRows.at(0), insertRows.at(1), QModelIndex());
    if (insertColumns.count() > 1)
        model->insertColumns(insertColumns.at(0), insertColumns.at(1), QModelIndex());
    if (deleteRows.count() > 1)
        model->removeRows(deleteRows.at(0), deleteRows.at(1), QModelIndex());
    if (deleteColumns.count() > 1)
        model->removeColumns(deleteColumns.at(0), deleteColumns.at(1), QModelIndex());

    // check that the selected items are the correct number and indexes
    QModelIndexList selectedList = selection->selectedIndexes();
    QCOMPARE(selectedList.count(), expectedList.count());
    foreach(IntPair pair, expectedList) {
        QModelIndex index = model->index(pair.first, pair.second, QModelIndex());
        QVERIFY(selectedList.contains(index));
    }
}

// "make reset public"-model
class MyStandardItemModel: public QStandardItemModel
{
    Q_OBJECT
public:
    inline MyStandardItemModel(int i1, int i2): QStandardItemModel(i1, i2) {}
    inline void reset() { QStandardItemModel::reset(); }
};

void tst_QItemSelectionModel::resetModel()
{
    MyStandardItemModel model(20, 20);
    QTreeView view;
    view.setModel(&model);

    QSignalSpy spy(view.selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)));

    view.selectionModel()->select(QItemSelection(model.index(0, 0), model.index(5, 5)), QItemSelectionModel::Select);

    QCOMPARE(spy.count(), 1);

    model.reset();

    QVERIFY(view.selectionModel()->selection().isEmpty());
    QVERIFY(view.selectionModel()->hasSelection() == false);

    view.selectionModel()->select(QItemSelection(model.index(0, 0), model.index(5, 5)), QItemSelectionModel::Select);

    QCOMPARE(spy.count(), 2);
    QCOMPARE(spy.at(1).count(), 2);
    // make sure we don't get an "old selection"
    QCOMPARE(spy.at(1).at(1).userType(), qMetaTypeId<QItemSelection>());
    QVERIFY(qvariant_cast<QItemSelection>(spy.at(1).at(1)).isEmpty());
}

void tst_QItemSelectionModel::removeRows_data()
{
    QTest::addColumn<int>("rowCount");
    QTest::addColumn<int>("columnCount");

    QTest::addColumn<int>("selectTop");
    QTest::addColumn<int>("selectLeft");
    QTest::addColumn<int>("selectBottom");
    QTest::addColumn<int>("selectRight");

    QTest::addColumn<int>("removeTop");
    QTest::addColumn<int>("removeBottom");

    QTest::addColumn<int>("expectedTop");
    QTest::addColumn<int>("expectedLeft");
    QTest::addColumn<int>("expectedBottom");
    QTest::addColumn<int>("expectedRight");

    QTest::newRow("4x4 <0,1><1,1>")
        << 4 << 4
        << 0 << 1 << 1 << 1
        << 0 << 0
        << 0 << 1 << 0 << 1;
}

void tst_QItemSelectionModel::removeRows()
{
    QFETCH(int, rowCount);
    QFETCH(int, columnCount);
    QFETCH(int, selectTop);
    QFETCH(int, selectLeft);
    QFETCH(int, selectBottom);
    QFETCH(int, selectRight);
    QFETCH(int, removeTop);
    QFETCH(int, removeBottom);
    QFETCH(int, expectedTop);
    QFETCH(int, expectedLeft);
    QFETCH(int, expectedBottom);
    QFETCH(int, expectedRight);

    MyStandardItemModel model(rowCount, columnCount);
    QItemSelectionModel selections(&model);
    QSignalSpy spy(&selections, SIGNAL(selectionChanged(QItemSelection,QItemSelection)));

    QModelIndex tl = model.index(selectTop, selectLeft);
    QModelIndex br = model.index(selectBottom, selectRight);
    selections.select(QItemSelection(tl, br), QItemSelectionModel::ClearAndSelect);

    QCOMPARE(spy.count(), 1);
    QVERIFY(selections.isSelected(tl));
    QVERIFY(selections.isSelected(br));
    QVERIFY(selections.hasSelection());

    model.removeRows(removeTop, removeBottom - removeTop + 1);

    QCOMPARE(spy.count(), 2);
    tl = model.index(expectedTop, expectedLeft);
    br = model.index(expectedBottom, expectedRight);
    QVERIFY(selections.isSelected(tl));
    QVERIFY(selections.isSelected(br));
}

void tst_QItemSelectionModel::removeColumns_data()
{
    QTest::addColumn<int>("rowCount");
    QTest::addColumn<int>("columnCount");

    QTest::addColumn<int>("selectTop");
    QTest::addColumn<int>("selectLeft");
    QTest::addColumn<int>("selectBottom");
    QTest::addColumn<int>("selectRight");

    QTest::addColumn<int>("removeLeft");
    QTest::addColumn<int>("removeRight");

    QTest::addColumn<int>("expectedTop");
    QTest::addColumn<int>("expectedLeft");
    QTest::addColumn<int>("expectedBottom");
    QTest::addColumn<int>("expectedRight");

    QTest::newRow("4x4 <0,1><1,1>")
        << 4 << 4
        << 1 << 0 << 1 << 1
        << 0 << 0
        << 1 << 0 << 1 << 0;
}

void tst_QItemSelectionModel::removeColumns()
{
    QFETCH(int, rowCount);
    QFETCH(int, columnCount);
    QFETCH(int, selectTop);
    QFETCH(int, selectLeft);
    QFETCH(int, selectBottom);
    QFETCH(int, selectRight);
    QFETCH(int, removeLeft);
    QFETCH(int, removeRight);
    QFETCH(int, expectedTop);
    QFETCH(int, expectedLeft);
    QFETCH(int, expectedBottom);
    QFETCH(int, expectedRight);

    MyStandardItemModel model(rowCount, columnCount);
    QItemSelectionModel selections(&model);
    QSignalSpy spy(&selections, SIGNAL(selectionChanged(QItemSelection,QItemSelection)));

    QModelIndex tl = model.index(selectTop, selectLeft);
    QModelIndex br = model.index(selectBottom, selectRight);
    selections.select(QItemSelection(tl, br), QItemSelectionModel::ClearAndSelect);

    QCOMPARE(spy.count(), 1);
    QVERIFY(selections.isSelected(tl));
    QVERIFY(selections.isSelected(br));
    QVERIFY(selections.hasSelection());

    model.removeColumns(removeLeft, removeRight - removeLeft + 1);

    QCOMPARE(spy.count(), 2);
    tl = model.index(expectedTop, expectedLeft);
    br = model.index(expectedBottom, expectedRight);
    QVERIFY(selections.isSelected(tl));
    QVERIFY(selections.isSelected(br));
}

typedef QList<IntList> IntListList;
typedef QPair<IntPair, IntPair> IntPairPair;
typedef QList<IntPairPair> IntPairPairList;
Q_DECLARE_METATYPE(IntListList)
Q_DECLARE_METATYPE(IntPairPair)
Q_DECLARE_METATYPE(IntPairPairList)

void tst_QItemSelectionModel::modelLayoutChanged_data()
{
    QTest::addColumn<IntListList>("items");
    QTest::addColumn<IntPairPairList>("initialSelectedRanges");
    QTest::addColumn<int>("sortOrder");
    QTest::addColumn<int>("sortColumn");
    QTest::addColumn<IntPairPairList>("expectedSelectedRanges");

    QTest::newRow("everything selected, then row order reversed")
        << (IntListList()
            << (IntList() << 0 << 1 << 2 << 3)
            << (IntList() << 3 << 2 << 1 << 0))
        << (IntPairPairList()
            << IntPairPair(IntPair(0, 0), IntPair(3, 1)))
        << int(Qt::DescendingOrder)
        << 0
        << (IntPairPairList()
            << IntPairPair(IntPair(0, 0), IntPair(3, 1)));
    QTest::newRow("first two rows selected, then row order reversed")
        << (IntListList()
            << (IntList() << 0 << 1 << 2 << 3)
            << (IntList() << 3 << 2 << 1 << 0))
        << (IntPairPairList()
            << IntPairPair(IntPair(0, 0), IntPair(1, 1)))
        << int(Qt::DescendingOrder)
        << 0
        << (IntPairPairList()
            << IntPairPair(IntPair(2, 0), IntPair(3, 1)));
    QTest::newRow("middle two rows selected, then row order reversed")
        << (IntListList()
            << (IntList() << 0 << 1 << 2 << 3)
            << (IntList() << 3 << 2 << 1 << 0))
        << (IntPairPairList()
            << IntPairPair(IntPair(1, 0), IntPair(2, 1)))
        << int(Qt::DescendingOrder)
        << 0
        << (IntPairPairList()
            << IntPairPair(IntPair(1, 0), IntPair(2, 1)));
    QTest::newRow("two ranges")
        << (IntListList()
            << (IntList() << 2 << 0 << 3 << 1)
            << (IntList() << 2 << 0 << 3 << 1))
        << (IntPairPairList()
            << IntPairPair(IntPair(1, 0), IntPair(1, 1))
            << IntPairPair(IntPair(3, 0), IntPair(3, 1)))
        << int(Qt::AscendingOrder)
        << 0
        << (IntPairPairList()
            << IntPairPair(IntPair(0, 0), IntPair(0, 1))
            << IntPairPair(IntPair(1, 0), IntPair(1, 1)));
}

void tst_QItemSelectionModel::modelLayoutChanged()
{
    QFETCH(IntListList, items);
    QFETCH(IntPairPairList, initialSelectedRanges);
    QFETCH(int, sortOrder);
    QFETCH(int, sortColumn);
    QFETCH(IntPairPairList, expectedSelectedRanges);

    MyStandardItemModel model(items.at(0).count(), items.count());
    // initialize model data
    for (int i = 0; i < model.rowCount(); ++i) {
        for (int j = 0; j < model.columnCount(); ++j) {
            QModelIndex index = model.index(i, j);
            model.setData(index, items.at(j).at(i), Qt::DisplayRole);
        }
    }

    // select initial ranges
    QItemSelectionModel selectionModel(&model);
    foreach (IntPairPair range, initialSelectedRanges) {
        IntPair tl = range.first;
        IntPair br = range.second;
        QItemSelection selection(
            model.index(tl.first, tl.second),
            model.index(br.first, br.second));
        selectionModel.select(selection, QItemSelectionModel::Select);
    }

    // sort the model
    model.sort(sortColumn, Qt::SortOrder(sortOrder));

    // verify that selection is as expected
    QItemSelection selection = selectionModel.selection();
    QCOMPARE(selection.count(), expectedSelectedRanges.count());
	QVERIFY(selectionModel.hasSelection() == !expectedSelectedRanges.isEmpty());

    for (int i = 0; i < expectedSelectedRanges.count(); ++i) {
        IntPairPair expectedRange = expectedSelectedRanges.at(i);
        IntPair expectedTl = expectedRange.first;
        IntPair expectedBr = expectedRange.second;
        QItemSelectionRange actualRange = selection.at(i);
        QModelIndex actualTl = actualRange.topLeft();
        QModelIndex actualBr = actualRange.bottomRight();
        QCOMPARE(actualTl.row(), expectedTl.first);
        QCOMPARE(actualTl.column(), expectedTl.second);
        QCOMPARE(actualBr.row(), expectedBr.first);
        QCOMPARE(actualBr.column(), expectedBr.second);
    }
}

void tst_QItemSelectionModel::selectedRows_data()
{
    QTest::addColumn<int>("rowCount");
    QTest::addColumn<int>("columnCount");
    QTest::addColumn<int>("column");
    QTest::addColumn<IntList>("selectRows");
    QTest::addColumn<IntList>("expectedRows");
    QTest::addColumn<IntList>("unexpectedRows");

    QTest::newRow("10x10, first row")
        << 10 << 10 << 0
        << (IntList() << 0)
        << (IntList() << 0)
        << (IntList() << 1 << 2 << 3 << 4 << 5 << 6 << 7 << 8 << 9);

    QTest::newRow("10x10, first 4 rows")
        << 10 << 10 << 0
        << (IntList() << 0 << 1 << 2 << 3)
        << (IntList() << 0 << 1 << 2 << 3)
        << (IntList() << 4 << 5 << 6 << 7 << 8 << 9);

    QTest::newRow("10x10, last 4 rows")
        << 10 << 10 << 0
        << (IntList() << 6 << 7 << 8 << 9)
        << (IntList() << 6 << 7 << 8 << 9)
        << (IntList() << 0 << 1 << 2 << 3 << 4 << 6);
}

void tst_QItemSelectionModel::selectedRows()
{
    QFETCH(int, rowCount);
    QFETCH(int, columnCount);
    QFETCH(int, column);
    QFETCH(IntList, selectRows);
    QFETCH(IntList, expectedRows);
    QFETCH(IntList, unexpectedRows);

    MyStandardItemModel model(rowCount, columnCount);
    QItemSelectionModel selectionModel(&model);

    for (int i = 0; i < selectRows.count(); ++i)
        selectionModel.select(model.index(selectRows.at(i), 0),
                              QItemSelectionModel::Select
                              |QItemSelectionModel::Rows);

    for (int j = 0; j < selectRows.count(); ++j)
        QVERIFY(selectionModel.isRowSelected(expectedRows.at(j), QModelIndex()));

    for (int k = 0; k < selectRows.count(); ++k)
        QVERIFY(!selectionModel.isRowSelected(unexpectedRows.at(k), QModelIndex()));

    QModelIndexList selectedRowIndexes = selectionModel.selectedRows(column);
    QCOMPARE(selectedRowIndexes.count(), expectedRows.count());
    qSort(selectedRowIndexes);
    for (int l = 0; l < selectedRowIndexes.count(); ++l) {
        QCOMPARE(selectedRowIndexes.at(l).row(), expectedRows.at(l));
        QCOMPARE(selectedRowIndexes.at(l).column(), column);
    }
}

void tst_QItemSelectionModel::selectedColumns_data()
{
    QTest::addColumn<int>("rowCount");
    QTest::addColumn<int>("columnCount");
    QTest::addColumn<int>("row");
    QTest::addColumn<IntList>("selectColumns");
    QTest::addColumn<IntList>("expectedColumns");
    QTest::addColumn<IntList>("unexpectedColumns");

    QTest::newRow("10x10, first columns")
        << 10 << 10 << 0
        << (IntList() << 0)
        << (IntList() << 0)
        << (IntList() << 1 << 2 << 3 << 4 << 5 << 6 << 7 << 8 << 9);

    QTest::newRow("10x10, first 4 columns")
        << 10 << 10 << 0
        << (IntList() << 0 << 1 << 2 << 3)
        << (IntList() << 0 << 1 << 2 << 3)
        << (IntList() << 4 << 5 << 6 << 7 << 8 << 9);

    QTest::newRow("10x10, last 4 columns")
        << 10 << 10 << 0
        << (IntList() << 6 << 7 << 8 << 9)
        << (IntList() << 6 << 7 << 8 << 9)
        << (IntList() << 0 << 1 << 2 << 3 << 4 << 6);
}

void tst_QItemSelectionModel::selectedColumns()
{
    QFETCH(int, rowCount);
    QFETCH(int, columnCount);
    QFETCH(int, row);
    QFETCH(IntList, selectColumns);
    QFETCH(IntList, expectedColumns);
    QFETCH(IntList, unexpectedColumns);

    MyStandardItemModel model(rowCount, columnCount);
    QItemSelectionModel selectionModel(&model);

    for (int i = 0; i < selectColumns.count(); ++i)
        selectionModel.select(model.index(0, selectColumns.at(i)),
                              QItemSelectionModel::Select
                              |QItemSelectionModel::Columns);

    for (int j = 0; j < selectColumns.count(); ++j)
        QVERIFY(selectionModel.isColumnSelected(expectedColumns.at(j), QModelIndex()));

    for (int k = 0; k < selectColumns.count(); ++k)
        QVERIFY(!selectionModel.isColumnSelected(unexpectedColumns.at(k), QModelIndex()));

    QModelIndexList selectedColumnIndexes = selectionModel.selectedColumns(row);
    QCOMPARE(selectedColumnIndexes.count(), expectedColumns.count());
    qSort(selectedColumnIndexes);
    for (int l = 0; l < selectedColumnIndexes.count(); ++l) {
        QCOMPARE(selectedColumnIndexes.at(l).column(), expectedColumns.at(l));
        QCOMPARE(selectedColumnIndexes.at(l).row(), row);
    }
}

void tst_QItemSelectionModel::setCurrentIndex()
{
    // Build up a simple tree
    QStandardItemModel *treemodel = new QStandardItemModel(0, 1);
    treemodel->insertRow(0, new QStandardItem(1));
    treemodel->insertRow(1, new QStandardItem(2));

    QTreeView treeView;
    treeView.setModel(treemodel);
    QItemSelectionModel *selectionModel = treeView.selectionModel();
    selectionModel->setCurrentIndex(
            treemodel->index(0, 0, treemodel->index(0, 0)),
            QItemSelectionModel::SelectCurrent);

    QSignalSpy currentSpy(selectionModel,
            SIGNAL(currentChanged(QModelIndex,QModelIndex)));
    QSignalSpy rowSpy(selectionModel,
            SIGNAL(currentRowChanged(QModelIndex,QModelIndex)));
    QSignalSpy columnSpy(selectionModel,
            SIGNAL(currentColumnChanged(QModelIndex,QModelIndex)));

    // Select the same row and column indexes, but with a different parent
    selectionModel->setCurrentIndex(
            treemodel->index(0, 0, treemodel->index(1, 0)),
            QItemSelectionModel::SelectCurrent);

    QCOMPARE(currentSpy.count(), 1);
    QCOMPARE(rowSpy.count(), 1);
    QCOMPARE(columnSpy.count(), 1);

    // Select another row in the same parent
    selectionModel->setCurrentIndex(
            treemodel->index(1, 0, treemodel->index(1, 0)),
            QItemSelectionModel::SelectCurrent);

    QCOMPARE(currentSpy.count(), 2);
    QCOMPARE(rowSpy.count(), 2);
    QCOMPARE(columnSpy.count(), 1);

    delete treemodel;
}

void tst_QItemSelectionModel::splitOnInsert()
{
    QStandardItemModel model(4, 1);
    QItemSelectionModel selectionModel(&model);
    selectionModel.select(model.index(2, 0), QItemSelectionModel::Select);
    model.insertRow(2);
    model.removeRow(3);
    QVERIFY(!selectionModel.isSelected(model.index(1, 0)));
}

void tst_QItemSelectionModel::task196285_rowIntersectsSelection()
{
    QTableWidget table;
    table.setColumnCount(1);
    table.setRowCount(1);
    table.setItem(0, 0, new QTableWidgetItem("foo"));
    QAbstractItemModel *model = table.model();
    QItemSelectionModel *selectionModel = table.selectionModel();
    QModelIndex index = model->index(0, 0, QModelIndex());

    selectionModel->select(index, QItemSelectionModel::Select);
    QVERIFY(selectionModel->rowIntersectsSelection(0, QModelIndex()));
    QVERIFY(selectionModel->columnIntersectsSelection(0, QModelIndex()));

    selectionModel->select(index, QItemSelectionModel::Deselect);
    QVERIFY(!selectionModel->rowIntersectsSelection(0, QModelIndex()));
    QVERIFY(!selectionModel->columnIntersectsSelection(0, QModelIndex()));

    selectionModel->select(index, QItemSelectionModel::Toggle);
    QVERIFY(selectionModel->rowIntersectsSelection(0, QModelIndex()));
    QVERIFY(selectionModel->columnIntersectsSelection(0, QModelIndex()));

    selectionModel->select(index, QItemSelectionModel::Toggle);
    QVERIFY(!selectionModel->rowIntersectsSelection(0, QModelIndex()));
    QVERIFY(!selectionModel->columnIntersectsSelection(0, QModelIndex()));
}

void tst_QItemSelectionModel::unselectable()
{
    QTreeWidget w;
    for (int i = 0; i < 10; ++i)
        w.setItemSelected(new QTreeWidgetItem(&w), true);
    QCOMPARE(w.topLevelItemCount(), 10);
    QCOMPARE(w.selectionModel()->selectedIndexes().count(), 10);
    QCOMPARE(w.selectionModel()->selectedRows().count(), 10);
    for (int j = 0; j < 10; ++j)
        w.topLevelItem(j)->setFlags(0);
    QCOMPARE(w.selectionModel()->selectedIndexes().count(), 0);
    QCOMPARE(w.selectionModel()->selectedRows().count(), 0);
}

void tst_QItemSelectionModel::task220420_selectedIndexes()
{
    QStandardItemModel model(2, 2);
    QItemSelectionModel selectionModel(&model);
    QItemSelection selection;
    selection.append(QItemSelectionRange(model.index(0,0)));
    selection.append(QItemSelectionRange(model.index(0,1)));

    //we select the 1st row
    selectionModel.select(selection, QItemSelectionModel::Rows | QItemSelectionModel::Select);

    QCOMPARE(selectionModel.selectedRows().count(), 1);
    QCOMPARE(selectionModel.selectedIndexes().count(), model.columnCount());
}


class QtTestTableModel: public QAbstractTableModel
{
    Q_OBJECT

    public:
        QtTestTableModel(int rows = 0, int columns = 0, QObject *parent = 0)
        : QAbstractTableModel(parent),
        row_count(rows),
        column_count(columns) {}

        int rowCount(const QModelIndex& = QModelIndex()) const { return row_count; }
        int columnCount(const QModelIndex& = QModelIndex()) const { return column_count; }
        bool isEditable(const QModelIndex &) const { return true; }

        QVariant data(const QModelIndex &idx, int role) const
        {
            if (role == Qt::DisplayRole || role == Qt::EditRole)
                return QString("[%1,%2]").arg(idx.row()).arg(idx.column());
            return QVariant();
        }

        int row_count;
        int column_count;
        friend class tst_QItemSelectionModel;
};


void tst_QItemSelectionModel::task240734_layoutChanged()
{
    QtTestTableModel model(1,1);
    QItemSelectionModel selectionModel(&model);
    selectionModel.select(model.index(0,0), QItemSelectionModel::Select);
    QCOMPARE(selectionModel.selectedIndexes().count() , 1);

    emit model.layoutAboutToBeChanged();
    model.row_count = 5;
    emit model.layoutChanged();

    //The selection should not change.
    QCOMPARE(selectionModel.selectedIndexes().count() , 1);
    QCOMPARE(selectionModel.selectedIndexes().first() , model.index(0,0));
}

void tst_QItemSelectionModel::merge_data()
{
    QTest::addColumn<QItemSelection>("init");
    QTest::addColumn<QItemSelection>("other");
    QTest::addColumn<int>("command");
    QTest::addColumn<QItemSelection>("result");

    QTest::newRow("Simple select")
        << QItemSelection()
        << QItemSelection(model->index(2, 1) , model->index(3, 4))
        << int(QItemSelectionModel::Select)
        << QItemSelection(model->index(2, 1) , model->index(3, 4));

    QTest::newRow("Simple deselect")
        << QItemSelection(model->index(2, 1) , model->index(3, 4))
        << QItemSelection(model->index(2, 1) , model->index(3, 4))
        << int(QItemSelectionModel::Deselect)
        << QItemSelection();

    QTest::newRow("Simple Toggle deselect")
        << QItemSelection(model->index(2, 1) , model->index(3, 4))
        << QItemSelection(model->index(2, 1) , model->index(3, 4))
        << int(QItemSelectionModel::Toggle)
        << QItemSelection();

    QTest::newRow("Simple Toggle select")
        << QItemSelection()
        << QItemSelection(model->index(2, 1) , model->index(3, 4))
        << int(QItemSelectionModel::Toggle)
        << QItemSelection(model->index(2, 1) , model->index(3, 4));

    QTest::newRow("Add select")
        << QItemSelection(model->index(2, 1) , model->index(3, 3))
        << QItemSelection(model->index(2, 2) , model->index(3, 4))
        << int(QItemSelectionModel::Select)
        << QItemSelection(model->index(2, 1) , model->index(3, 4));

    QTest::newRow("Deselect")
        << QItemSelection(model->index(2, 1) , model->index(3, 4))
        << QItemSelection(model->index(2, 2) , model->index(3, 4))
        << int(QItemSelectionModel::Deselect)
        << QItemSelection(model->index(2, 1) , model->index(3, 1));

    QItemSelection r1(model->index(2, 1) , model->index(3, 1));
    r1.select(model->index(2, 4) , model->index(3, 4));
    QTest::newRow("Toggle")
        << QItemSelection(model->index(2, 1) , model->index(3, 3))
        << QItemSelection(model->index(2, 2) , model->index(3, 4))
        << int(QItemSelectionModel::Toggle)
        << r1;
}


void tst_QItemSelectionModel::merge()
{
    QFETCH(QItemSelection, init);
    QFETCH(QItemSelection, other);
    QFETCH(int, command);
    QFETCH(QItemSelection, result);

    init.merge(other, QItemSelectionModel::SelectionFlags(command));

    foreach(const QModelIndex &idx, init.indexes())
        QVERIFY(result.contains(idx));
    foreach(const QModelIndex &idx, result.indexes())
        QVERIFY(init.contains(idx));
}

void tst_QItemSelectionModel::task119433_isRowSelected()
{
    QStandardItemModel model(2,2);
    model.setData(model.index(0,0), 0, Qt::UserRole - 1);
    QItemSelectionModel sel(&model);
    sel.select( QItemSelection(model.index(0,0), model.index(0, 1)), QItemSelectionModel::Select);
    QCOMPARE(sel.selectedIndexes().count(), 1);
    QVERIFY(sel.isRowSelected(0, QModelIndex()));
}

void tst_QItemSelectionModel::task252069_rowIntersectsSelection()
{
    QStandardItemModel m;
    for (int i=0; i<8; ++i) {
        for (int j=0; j<8; ++j) {
            QStandardItem *item = new QStandardItem(QString("Item number %1").arg(i));
            if ((i % 2 == 0 && j == 0)  ||
                (j % 2 == 0 && i == 0)  ||
                 j == 5 || i == 5 ) {
                item->setEnabled(false);
                //item->setSelectable(false);
            }
            m.setItem(i, j, item);
        }
    }

    QItemSelectionModel selected(&m);
    //nothing is selected
    QVERIFY(!selected.rowIntersectsSelection(0, QModelIndex()));
    QVERIFY(!selected.rowIntersectsSelection(2, QModelIndex()));
    QVERIFY(!selected.rowIntersectsSelection(3, QModelIndex()));
    QVERIFY(!selected.rowIntersectsSelection(5, QModelIndex()));
    QVERIFY(!selected.columnIntersectsSelection(0, QModelIndex()));
    QVERIFY(!selected.columnIntersectsSelection(2, QModelIndex()));
    QVERIFY(!selected.columnIntersectsSelection(3, QModelIndex()));
    QVERIFY(!selected.columnIntersectsSelection(5, QModelIndex()));
    selected.select(m.index(2, 0), QItemSelectionModel::Select | QItemSelectionModel::Rows);
    QVERIFY(!selected.rowIntersectsSelection(0, QModelIndex()));
    QVERIFY( selected.rowIntersectsSelection(2, QModelIndex()));
    QVERIFY(!selected.rowIntersectsSelection(3, QModelIndex()));
    QVERIFY(!selected.rowIntersectsSelection(5, QModelIndex()));
    QVERIFY(!selected.columnIntersectsSelection(0, QModelIndex()));
    QVERIFY( selected.columnIntersectsSelection(2, QModelIndex()));
    QVERIFY( selected.columnIntersectsSelection(3, QModelIndex()));
    QVERIFY(!selected.columnIntersectsSelection(5, QModelIndex()));
    selected.select(m.index(0, 5), QItemSelectionModel::Select | QItemSelectionModel::Columns);
    QVERIFY(!selected.rowIntersectsSelection(0, QModelIndex()));
    QVERIFY( selected.rowIntersectsSelection(2, QModelIndex()));
    QVERIFY(!selected.rowIntersectsSelection(3, QModelIndex()));
    QVERIFY(!selected.rowIntersectsSelection(5, QModelIndex()));
    QVERIFY(!selected.columnIntersectsSelection(0, QModelIndex()));
    QVERIFY( selected.columnIntersectsSelection(2, QModelIndex()));
    QVERIFY( selected.columnIntersectsSelection(3, QModelIndex()));
    QVERIFY(!selected.columnIntersectsSelection(5, QModelIndex()));
}

void tst_QItemSelectionModel::task232634_childrenDeselectionSignal()
{
    QStandardItemModel model;

    QStandardItem *parentItem = model.invisibleRootItem();
    for (int i = 0; i < 4; ++i) {
        QStandardItem *item = new QStandardItem(QString("item %0").arg(i));
        parentItem->appendRow(item);
        parentItem = item;
    }

    QModelIndex root = model.index(0,0);
    QModelIndex par = root.child(0,0);
    QModelIndex sel = par.child(0,0);

    QItemSelectionModel selectionModel(&model);
    selectionModel.select(sel, QItemSelectionModel::SelectCurrent);

    QSignalSpy deselectSpy(&selectionModel, SIGNAL(selectionChanged(const QItemSelection& , const QItemSelection&)));
    model.removeRows(0, 1, root);
    QVERIFY(deselectSpy.count() == 1);

    // More testing stress for the patch.
    model.clear();
    selectionModel.clear();

    parentItem = model.invisibleRootItem();
    for (int i = 0; i < 2; ++i) {
        QStandardItem *item = new QStandardItem(QString("item %0").arg(i));
        parentItem->appendRow(item);
    }
    for (int i = 0; i < 2; ++i) {
        parentItem = model.invisibleRootItem()->child(i, 0);
        for (int j = 0; j < 2; ++j) {
            QStandardItem *item = new QStandardItem(QString("item %0.%1").arg(i).arg(j));
            parentItem->appendRow(item);
        }
    }

    sel = model.index(0, 0).child(0, 0);
    selectionModel.select(sel, QItemSelectionModel::Select);
    QModelIndex sel2 = model.index(1, 0).child(0, 0);
    selectionModel.select(sel2, QItemSelectionModel::Select);

    QVERIFY(selectionModel.selection().contains(sel));
    QVERIFY(selectionModel.selection().contains(sel2));
    deselectSpy.clear();
    model.removeRow(0, model.index(0, 0));
    QVERIFY(deselectSpy.count() == 1);
    QVERIFY(!selectionModel.selection().contains(sel));
    QVERIFY(selectionModel.selection().contains(sel2));
}

void tst_QItemSelectionModel::task260134_layoutChangedWithAllSelected()
{
    QStringListModel model( QStringList() << "foo" << "bar" << "foo2");
    QSortFilterProxyModel proxy;
    proxy.setSourceModel(&model);
    QItemSelectionModel selection(&proxy);


    QCOMPARE(model.rowCount(), 3);
    QCOMPARE(proxy.rowCount(), 3);
    proxy.setFilterRegExp( QRegExp("f"));
    QCOMPARE(proxy.rowCount(), 2);

    QList<QPersistentModelIndex> indexList;
    indexList << proxy.index(0,0) << proxy.index(1,0);
    selection.select( QItemSelection(indexList.first(), indexList.last()), QItemSelectionModel::Select);

    //let's check the selection hasn't changed
    QCOMPARE(selection.selectedIndexes().count(), indexList.count());
    foreach(QPersistentModelIndex index, indexList)
        QVERIFY(selection.isSelected(index));

    proxy.setFilterRegExp(QRegExp());
    QCOMPARE(proxy.rowCount(), 3);

    //let's check the selection hasn't changed
    QCOMPARE(selection.selectedIndexes().count(), indexList.count());
    foreach(QPersistentModelIndex index, indexList)
        QVERIFY(selection.isSelected(index));
}


void tst_QItemSelectionModel::QTBUG5671_layoutChangedWithAllSelected()
{
    struct MyFilterModel : public QSortFilterProxyModel
    {     // Override sort filter proxy to remove even numbered rows.
        bool filtering;
        virtual bool filterAcceptsRow( int source_row, const QModelIndex& source_parent ) const
        {
            return !filtering || !( source_row & 1 );
        }
    };

    //same as task260134_layoutChangedWithAllSelected but with a sightly bigger model

    enum { cNumRows=30, cNumCols=20 };

    QStandardItemModel model(cNumRows, cNumCols);
    MyFilterModel proxy;
    proxy.filtering = true;
    proxy.setSourceModel(&model);
    QItemSelectionModel selection(&proxy);

    // Populate the tree view.
    for (unsigned int i = 0; i < cNumCols; i++)
        model.setHeaderData( i, Qt::Horizontal, QString::fromLatin1("Column %1").arg(i));

    for (unsigned int r = 0; r < cNumRows; r++) {
        for (unsigned int c = 0; c < cNumCols; c++) {
            model.setData(model.index(r, c, QModelIndex()),
                          QString::fromLatin1("r:%1/c:%2").arg(r, c));
        }
    }


    QCOMPARE(model.rowCount(), int(cNumRows));
    QCOMPARE(proxy.rowCount(), int(cNumRows/2));

    selection.select( QItemSelection(proxy.index(0,0), proxy.index(proxy.rowCount() - 1, proxy.columnCount() - 1)), QItemSelectionModel::Select);

    QList<QPersistentModelIndex> indexList;
    foreach(const QModelIndex &id, selection.selectedIndexes())
        indexList << id;

    proxy.filtering = false;
    proxy.invalidate();
    QCOMPARE(proxy.rowCount(), int(cNumRows));

    //let's check the selection hasn't changed
    QCOMPARE(selection.selectedIndexes().count(), indexList.count());
    foreach(QPersistentModelIndex index, indexList)
        QVERIFY(selection.isSelected(index));
}

void tst_QItemSelectionModel::QTBUG2804_layoutChangedTreeSelection()
{
    QStandardItemModel model;
    QStandardItem top1("Child1"), top2("Child2"), top3("Child3");
    QStandardItem sub11("Alpha"), sub12("Beta"), sub13("Gamma"), sub14("Delta"),
        sub21("Alpha"), sub22("Beta"), sub23("Gamma"), sub24("Delta");
    top1.appendColumn(QList<QStandardItem*>() << &sub11 << &sub12 << &sub13 << &sub14);
    top2.appendColumn(QList<QStandardItem*>() << &sub21 << &sub22 << &sub23 << &sub24);
    model.appendColumn(QList<QStandardItem*>() << &top1 << &top2 << &top3);

    QItemSelectionModel selModel(&model);

    selModel.select(sub11.index(), QItemSelectionModel::Select);
    selModel.select(sub12.index(), QItemSelectionModel::Select);
    selModel.select(sub21.index(), QItemSelectionModel::Select);
    selModel.select(sub23.index(), QItemSelectionModel::Select);

    QModelIndexList list = selModel.selectedIndexes();
    QCOMPARE(list.count(), 4);

    model.sort(0); //this will provoke a relayout

    QCOMPARE(selModel.selectedIndexes().count(), 4);
}


QTEST_MAIN(tst_QItemSelectionModel)
#include "tst_qitemselectionmodel.moc"
