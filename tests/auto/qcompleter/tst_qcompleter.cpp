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
#include <QtGui>
#include "qcompleter.h"

#include <QtTest/QtTest>
#include <QtGui>
#include <QtDebug>
#include <QPair>
#include <QList>
#include <QPointer>

#include "../../shared/util.h"

//TESTED_CLASS=
//TESTED_FILES=

class CsvCompleter : public QCompleter
{
    Q_OBJECT
public:
    CsvCompleter(QObject *parent = 0) : QCompleter(parent), csv(true) { }

    QString pathFromIndex(const QModelIndex& sourceIndex) const;

    void setCsvCompletion(bool set) { csv = set; }

protected:
    QStringList splitPath(const QString &path) const {
        return csv ? path.split(",") : QCompleter::splitPath(path);
    }

private:
    bool csv;
};

QString CsvCompleter::pathFromIndex(const QModelIndex& si) const
{
    if (!csv)
        return QCompleter::pathFromIndex(si);

    if (!si.isValid())
        return QString();

    QModelIndex idx = si;
    QStringList list;
    do {
        QString t = model()->data(idx, completionRole()).toString();
        list.prepend(t);
        QModelIndex parent = idx.parent();
        idx = parent.sibling(parent.row(), si.column());
    } while (idx.isValid());

    if (list.count() == 1)
        return list[0];
    return list.join(",");
}

class tst_QCompleter : public QObject
{
    Q_OBJECT
public:
    tst_QCompleter();
    ~tst_QCompleter();

private slots:
    void getSetCheck();

    void multipleWidgets();
    void focusIn();

    void csMatchingOnCsSortedModel_data();
    void csMatchingOnCsSortedModel();
    void ciMatchingOnCiSortedModel_data();
    void ciMatchingOnCiSortedModel();

    void ciMatchingOnCsSortedModel_data();
    void ciMatchingOnCsSortedModel();
    void csMatchingOnCiSortedModel_data();
    void csMatchingOnCiSortedModel();

    void directoryModel_data();
    void directoryModel();

    void changingModel_data();
    void changingModel();

    void sortedEngineRowCount_data();
    void sortedEngineRowCount();
    void unsortedEngineRowCount_data();
    void unsortedEngineRowCount();

    void currentRow();
    void sortedEngineMapFromSource();
    void unsortedEngineMapFromSource();

    void historySearch();

    void modelDeletion();
    void setters();

    void dynamicSortOrder();
    void disabledItems();

    // task-specific tests below me
    void task178797_activatedOnReturn();
    void task189564_omitNonSelectableItems();
    void task246056_setCompletionPrefix();
    void task250064_lostFocus();

    void task253125_lineEditCompletion_data();
    void task253125_lineEditCompletion();
    void task247560_keyboardNavigation();

private:
    void filter();
    void testRowCount();
    enum ModelType {
        CASE_SENSITIVELY_SORTED_MODEL,
        CASE_INSENSITIVELY_SORTED_MODEL,
        DIRECTORY_MODEL,
        HISTORY_MODEL
    };
    void setSourceModel(ModelType);

    CsvCompleter *completer;
    QTreeWidget *treeWidget;
    const int completionColumn;
    const int columnCount;
};

tst_QCompleter::tst_QCompleter() : completer(0), completionColumn(0), columnCount(3)
{
    treeWidget = new QTreeWidget;
    treeWidget->setColumnCount(columnCount);
}

tst_QCompleter::~tst_QCompleter()
{
    delete treeWidget;
    delete completer;
}

void tst_QCompleter::setSourceModel(ModelType type)
{
    QString text;
    QTreeWidgetItem *parent, *child;
    treeWidget->clear();
    switch(type) {
    case CASE_SENSITIVELY_SORTED_MODEL:
        // Creates a tree model with top level items P0, P1, .., p0, p1,..
        // Each of these items parents have children (for P0 - c0P0, c1P0,...)
        for (int i = 0; i < 2; i++) {
            for (int j = 0; j < 5; j++) {
                parent = new QTreeWidgetItem(treeWidget);
                text.sprintf("%c%i", i == 0 ? 'P' : 'p', j);
                parent->setText(completionColumn, text);
                for (int k = 0; k < 5; k++) {
                    child = new QTreeWidgetItem(parent);
                    QString t = QString().sprintf("c%i", k) + text;
                    child->setText(completionColumn, t);
                }
            }
        }
        completer->setModel(treeWidget->model());
        completer->setCompletionColumn(completionColumn);
        break;
    case CASE_INSENSITIVELY_SORTED_MODEL:
    case HISTORY_MODEL:
        // Creates a tree model with top level items P0, p0, P1, p1,...
        // Each of these items have children c0p0, c1p0,..
        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 2; j++) {
                parent = new QTreeWidgetItem(treeWidget);
                text.sprintf("%c%i", j == 0 ? 'P' : 'p', i);
                parent->setText(completionColumn, text);
                for (int k = 0; k < 5; k++) {
                    child = new QTreeWidgetItem(parent);
                    QString t = QString().sprintf("c%i", k) + text;
                    child->setText(completionColumn, t);
                }
            }
        }
        completer->setModel(treeWidget->model());
        completer->setCompletionColumn(completionColumn);
        if (type == CASE_INSENSITIVELY_SORTED_MODEL)
            break;
        parent = new QTreeWidgetItem(treeWidget);
        parent->setText(completionColumn, QLatin1String("p3,c3p3"));
        parent = new QTreeWidgetItem(treeWidget);
        parent->setText(completionColumn, QLatin1String("p2,c4p2"));
        break;
    case DIRECTORY_MODEL:
        completer->setCsvCompletion(false);
        completer->setModel(new QDirModel(completer));
        completer->setCompletionColumn(0);
        break;
    default:
        qDebug() << "Invalid type";
    }
}

void tst_QCompleter::filter()
{
    QFETCH(QString, filterText);
    QFETCH(QString, step);
    QFETCH(QString, completion);
    QFETCH(QString, completionText);

    if (filterText.compare("FILTERING_OFF", Qt::CaseInsensitive) == 0) {
        completer->setCompletionMode(QCompleter::UnfilteredPopupCompletion);
        return;
    }

    completer->setCompletionPrefix(filterText);

    for (int i = 0; i < step.length(); i++) {
        int row = completer->currentRow();
        switch (step[i].toUpper().toAscii()) {
        case 'P': --row; break;
        case 'N': ++row; break;
        case 'L': row = completer->completionCount() - 1; break;
        case 'F': row = 0; break;
        default:
            Q_ASSERT(false);
        }
        completer->setCurrentRow(row);
    }

    //QModelIndex si = completer->currentIndex();
    //QCOMPARE(completer->model()->data(si).toString(), completion);
    QVERIFY(0 == QString::compare(completer->currentCompletion(), completionText, completer->caseSensitivity()));
}

// Testing get/set functions
void tst_QCompleter::getSetCheck()
{
    QStandardItemModel model(3,3);
    QCompleter completer(&model);

    // QString QCompleter::completionPrefix()
    // void QCompleter::setCompletionPrefix(QString)
    completer.setCompletionPrefix(QString("te"));
    QCOMPARE(completer.completionPrefix(), QString("te"));
    completer.setCompletionPrefix(QString());
    QCOMPARE(completer.completionPrefix(), QString());

    // ModelSorting QCompleter::modelSorting()
    // void QCompleter::setModelSorting(ModelSorting)
    completer.setModelSorting(QCompleter::CaseSensitivelySortedModel);
    QCOMPARE(completer.modelSorting(), QCompleter::CaseSensitivelySortedModel);
    completer.setModelSorting(QCompleter::CaseInsensitivelySortedModel);
    QCOMPARE(completer.modelSorting(), QCompleter::CaseInsensitivelySortedModel);
    completer.setModelSorting(QCompleter::UnsortedModel);
    QCOMPARE(completer.modelSorting(), QCompleter::UnsortedModel);

    // CompletionMode QCompleter::completionMode()
    // void QCompleter::setCompletionMode(CompletionMode)
    QCOMPARE(completer.completionMode(), QCompleter::PopupCompletion); // default value
    completer.setCompletionMode(QCompleter::UnfilteredPopupCompletion);
    QCOMPARE(completer.completionMode(), QCompleter::UnfilteredPopupCompletion);
    completer.setCompletionMode(QCompleter::InlineCompletion);
    QCOMPARE(completer.completionMode(), QCompleter::InlineCompletion);

    // int QCompleter::completionColumn()
    // void QCompleter::setCompletionColumn(int)
    completer.setCompletionColumn(2);
    QCOMPARE(completer.completionColumn(), 2);
    completer.setCompletionColumn(1);
    QCOMPARE(completer.completionColumn(), 1);

    // int QCompleter::completionRole()
    // void QCompleter::setCompletionRole(int)
    QCOMPARE(completer.completionRole(), static_cast<int>(Qt::EditRole)); // default value
    completer.setCompletionRole(Qt::DisplayRole);
    QCOMPARE(completer.completionRole(), static_cast<int>(Qt::DisplayRole));

    // int QCompleter::maxVisibleItems()
    // void QCompleter::setMaxVisibleItems(int)
    QCOMPARE(completer.maxVisibleItems(), 7); // default value
    completer.setMaxVisibleItems(10);
    QCOMPARE(completer.maxVisibleItems(), 10);
    QTest::ignoreMessage(QtWarningMsg, "QCompleter::setMaxVisibleItems: "
                         "Invalid max visible items (-2147483648) must be >= 0");
    completer.setMaxVisibleItems(INT_MIN);
    QCOMPARE(completer.maxVisibleItems(), 10); // Cannot be set to something negative => old value

    // Qt::CaseSensitivity QCompleter::caseSensitivity()
    // void QCompleter::setCaseSensitivity(Qt::CaseSensitivity)
    QCOMPARE(completer.caseSensitivity(), Qt::CaseSensitive); // default value
    completer.setCaseSensitivity(Qt::CaseInsensitive);
    QCOMPARE(completer.caseSensitivity(), Qt::CaseInsensitive);

    // bool QCompleter::wrapAround()
    // void QCompleter::setWrapAround(bool)
    QCOMPARE(completer.wrapAround(), true); // default value
    completer.setWrapAround(false);
    QCOMPARE(completer.wrapAround(), false);
}

void tst_QCompleter::csMatchingOnCsSortedModel_data()
{
    delete completer;
    completer = new CsvCompleter;
    completer->setModelSorting(QCompleter::CaseSensitivelySortedModel);
    completer->setCaseSensitivity(Qt::CaseSensitive);
    setSourceModel(CASE_SENSITIVELY_SORTED_MODEL);

    QTest::addColumn<QString>("filterText");
    QTest::addColumn<QString>("step");
    QTest::addColumn<QString>("completion");
    QTest::addColumn<QString>("completionText");

    for (int i = 0; i < 2; i++) {
         if (i == 1)
             QTest::newRow("FILTERING_OFF") << "FILTERING_OFF" << "" << "" << "";

         // Plain text filter
         QTest::newRow("()") << "" << "" << "P0" << "P0";
         QTest::newRow("()F") << "" << "F" << "P0" << "P0";
         QTest::newRow("()L") << "" << "L" << "p4" << "p4";
         QTest::newRow("()L") << "" << "L" << "p4" << "p4";
         QTest::newRow("()N") << "" << "N" << "P1" << "P1";
         QTest::newRow("(P)") << "P" << "" << "P0" << "P0";
         QTest::newRow("(P)F") << "P" << "" << "P0" << "P0";
         QTest::newRow("(P)L") << "P" << "L" << "P4" << "P4";
         QTest::newRow("(p)") << "p" << "" << "p0" << "p0";
         QTest::newRow("(p)N") << "p" << "N" << "p1" << "p1";
         QTest::newRow("(p)NN") << "p" << "NN" << "p2" << "p2";
         QTest::newRow("(p)NNN") << "p" << "NNN" << "p3" << "p3";
         QTest::newRow("(p)NNNN") << "p" << "NNNN" << "p4" << "p4";
         QTest::newRow("(p1)") << "p1" << "" << "p1" << "p1";
         QTest::newRow("(p11)") << "p11" << "" << "" << "";

         // Tree filter
         QTest::newRow("(P0,)") << "P0," << "" << "c0P0" << "P0,c0P0";
         QTest::newRow("(P0,c)") << "P0,c" << "" << "c0P0" << "P0,c0P0";
         QTest::newRow("(P0,c1)") << "P0,c1" << "" << "c1P0" << "P0,c1P0";
         QTest::newRow("(P0,c3P0)") << "P0,c3P0" << "" << "c3P0" << "P0,c3P0";
         QTest::newRow("(P3,c)F") << "P3,c" << "F" << "c0P3" << "P3,c0P3";
         QTest::newRow("(P3,c)L") << "P3,c" << "L" << "c4P3" << "P3,c4P3";
         QTest::newRow("(P3,c)N") << "P3,c" << "N" << "c1P3" << "P3,c1P3";
         QTest::newRow("(P3,c)NN") << "P3,c" << "NN" << "c2P3" << "P3,c2P3";
         QTest::newRow("(P3,,c)") << "P3,,c" << "" << "" << "";
         QTest::newRow("(P3,c0P3,)") << "P3,c0P3," << "" << "" << "";
         QTest::newRow("(P,)") << "P," << "" << "" << "";
     }
}

void tst_QCompleter::csMatchingOnCsSortedModel()
{
    filter();
}

void tst_QCompleter::ciMatchingOnCiSortedModel_data()
{
    delete completer;
    completer = new CsvCompleter;
    completer->setModelSorting(QCompleter::CaseInsensitivelySortedModel);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    setSourceModel(CASE_INSENSITIVELY_SORTED_MODEL);

    QTest::addColumn<QString>("filterText");
    QTest::addColumn<QString>("step");
    QTest::addColumn<QString>("completion");
    QTest::addColumn<QString>("completionText");

    for (int i = 0; i < 2; i++) {
        if (i == 1)
            QTest::newRow("FILTERING_OFF") << "FILTERING_OFF" << "" << "" << "";

        // Plain text filter
        QTest::newRow("()") << "" << "" << "P0" << "P0";
        QTest::newRow("()F") << "" << "F" << "P0" << "P0";
        QTest::newRow("()L") << "" << "L" << "p4" << "p4";
        QTest::newRow("()N") << "" << "N" << "p0" << "p0";
        QTest::newRow("(P)") << "P" << "" << "P0" << "P0";
        QTest::newRow("(P)F") << "P" << "" << "P0" << "P0";
        QTest::newRow("(P)L") << "P" << "L" << "p4" << "p4";
        QTest::newRow("(p)") << "p" << "" << "P0" << "P0";
        QTest::newRow("(p)N") << "p" << "N" << "p0" << "p0";
        QTest::newRow("(p)NN") << "p" << "NN" << "P1" << "P1";
        QTest::newRow("(p)NNN") << "p" << "NNN" << "p1" << "p1";
        QTest::newRow("(p1)") << "p1" << "" << "P1" << "P1";
        QTest::newRow("(p1)N") << "p1" << "N" << "p1" << "p1";
        QTest::newRow("(p11)") << "p11" << "" << "" << "";

        //// Tree filter
        QTest::newRow("(p0,)") << "p0," << "" << "c0P0" << "P0,c0P0";
        QTest::newRow("(p0,c)") << "p0,c" << "" << "c0P0" << "P0,c0P0";
        QTest::newRow("(p0,c1)") << "p0,c1" << "" << "c1P0" << "P0,c1P0";
        QTest::newRow("(p0,c3P0)") << "p0,c3P0" << "" << "c3P0" << "P0,c3P0";
        QTest::newRow("(p3,c)F") << "p3,c" << "F" << "c0P3" << "P3,c0P3";
        QTest::newRow("(p3,c)L") << "p3,c" << "L" << "c4P3" << "P3,c4P3";
        QTest::newRow("(p3,c)N") << "p3,c" << "N" << "c1P3" << "P3,c1P3";
        QTest::newRow("(p3,c)NN") << "p3,c" << "NN" << "c2P3" << "P3,c2P3";
        QTest::newRow("(p3,,c)") << "p3,,c" << "" << "" << "";
        QTest::newRow("(p3,c0P3,)") << "p3,c0P3," << "" << "" << "";
        QTest::newRow("(p,)") << "p," << "" << "" << "";
    }
}

void tst_QCompleter::ciMatchingOnCiSortedModel()
{
    filter();
}

void tst_QCompleter::ciMatchingOnCsSortedModel_data()
{
    delete completer;
    completer = new CsvCompleter;
    completer->setModelSorting(QCompleter::CaseSensitivelySortedModel);
    setSourceModel(CASE_SENSITIVELY_SORTED_MODEL);
    completer->setCaseSensitivity(Qt::CaseInsensitive);

    QTest::addColumn<QString>("filterText");
    QTest::addColumn<QString>("step");
    QTest::addColumn<QString>("completion");
    QTest::addColumn<QString>("completionText");

    for (int i = 0; i < 2; i++) {
        if (i == 1)
            QTest::newRow("FILTERING_OFF") << "FILTERING_OFF" << "" << "" << "";

        // Plain text filter
        QTest::newRow("()") << "" << "" << "P0" << "P0";
        QTest::newRow("()F") << "" << "F" << "P0" << "P0";
        QTest::newRow("()L") << "" << "L" << "p4" << "p4";
        QTest::newRow("(P)") << "P" << "" << "P0" << "P0";
        QTest::newRow("(P)F") << "P" << "" << "P0" << "P0";
        QTest::newRow("(P)L") << "P" << "L" << "p4" << "p4";
        QTest::newRow("(p)") << "p" << "" << "P0" << "P0";
        QTest::newRow("(p)N") << "p" << "N" << "P1" << "P1";
        QTest::newRow("(p)NN") << "p" << "NN" << "P2" << "P2";
        QTest::newRow("(p)NNN") << "p" << "NNN" << "P3" << "P3";
        QTest::newRow("(p1)") << "p1" << "" << "P1" << "P1";
        QTest::newRow("(p1)N") << "p1" << "N" << "p1" << "p1";
        QTest::newRow("(p11)") << "p11" << "" << "" << "";

        // Tree filter
        QTest::newRow("(p0,)") << "p0," << "" << "c0P0" << "P0,c0P0";
        QTest::newRow("(p0,c)") << "p0,c" << "" << "c0P0" << "P0,c0P0";
        QTest::newRow("(p0,c1)") << "p0,c1" << "" << "c1P0" << "P0,c1P0";
        QTest::newRow("(p0,c3P0)") << "p0,c3P0" << "" << "c3P0" << "P0,c3P0";
        QTest::newRow("(p3,c)F") << "p3,c" << "F" << "c0P3" << "P3,c0P3";
        QTest::newRow("(p3,c)L") << "p3,c" << "L" << "c4P3" << "P3,c4P3";
        QTest::newRow("(p3,c)N") << "p3,c" << "N" << "c1P3" << "P3,c1P3";
        QTest::newRow("(p3,c)NN") << "p3,c" << "NN" << "c2P3" << "P3,c2P3";
        QTest::newRow("(p3,,c)") << "p3,,c" << "" << "" << "";
        QTest::newRow("(p3,c0P3,)") << "p3,c0P3," << "" << "" << "";
        QTest::newRow("(p,)") << "p," << "" << "" << "";
    }
}

void tst_QCompleter::ciMatchingOnCsSortedModel()
{
    filter();
}

void tst_QCompleter::csMatchingOnCiSortedModel_data()
{
    delete completer;
    completer = new CsvCompleter;
    completer->setModelSorting(QCompleter::CaseInsensitivelySortedModel);
    setSourceModel(CASE_INSENSITIVELY_SORTED_MODEL);
    completer->setCaseSensitivity(Qt::CaseSensitive);

    QTest::addColumn<QString>("filterText");
    QTest::addColumn<QString>("step");
    QTest::addColumn<QString>("completion");
    QTest::addColumn<QString>("completionText");

    for (int i = 0; i < 2; i++) {
        if (i == 1)
            QTest::newRow("FILTERING_OFF") << "FILTERING_OFF" << "" << "" << "";

        // Plain text filter
        QTest::newRow("()") << "" << "" << "P0" << "P0";
        QTest::newRow("()F") << "" << "F" << "P0" << "P0";
        QTest::newRow("()L") << "" << "L" << "p4" << "p4";
        QTest::newRow("()N") << "" << "N" << "p0" << "p0";
        QTest::newRow("(P)") << "P" << "" << "P0" << "P0";
        QTest::newRow("(P)F") << "P" << "" << "P0" << "P0";
        QTest::newRow("(P)L") << "P" << "L" << "P4" << "P4";
        QTest::newRow("(p)") << "p" << "" << "p0" << "p0";
        QTest::newRow("(p)N") << "p" << "N" << "p1" << "p1";
        QTest::newRow("(p)NN") << "p" << "NN" << "p2" << "p2";
        QTest::newRow("(p)NNN") << "p" << "NNN" << "p3" << "p3";
        QTest::newRow("(p1)") << "p1" << "" << "p1" << "p1";
        QTest::newRow("(p11)") << "p11" << "" << "" << "";

        //// Tree filter
        QTest::newRow("(p0,)") << "p0," << "" << "c0p0" << "p0,c0p0";
        QTest::newRow("(p0,c)") << "p0,c" << "" << "c0p0" << "p0,c0p0";
        QTest::newRow("(p0,c1)") << "p0,c1" << "" << "c1p0" << "p0,c1p0";
        QTest::newRow("(p0,c3P0)") << "p0,c3p0" << "" << "c3p0" << "p0,c3p0";
        QTest::newRow("(p3,c)F") << "p3,c" << "F" << "c0p3" << "p3,c0p3";
        QTest::newRow("(p3,c)L") << "p3,c" << "L" << "c4p3" << "p3,c4p3";
        QTest::newRow("(p3,c)N") << "p3,c" << "N" << "c1p3" << "p3,c1p3";
        QTest::newRow("(p3,c)NN") << "p3,c" << "NN" << "c2p3" << "p3,c2p3";
        QTest::newRow("(p3,,c)") << "p3,,c" << "" << "" << "";
        QTest::newRow("(p3,c0P3,)") << "p3,c0P3," << "" << "" << "";
        QTest::newRow("(p,)") << "p," << "" << "" << "";

        QTest::newRow("FILTERING_OFF") << "FILTERING_OFF" << "" << "" << "";
    }
}

void tst_QCompleter::csMatchingOnCiSortedModel()
{
    filter();
}

void tst_QCompleter::directoryModel_data()
{
    delete completer;
    completer = new CsvCompleter;
    completer->setModelSorting(QCompleter::CaseSensitivelySortedModel);
    setSourceModel(DIRECTORY_MODEL);
    completer->setCaseSensitivity(Qt::CaseInsensitive);

    QTest::addColumn<QString>("filterText");
    QTest::addColumn<QString>("step");
    QTest::addColumn<QString>("completion");
    QTest::addColumn<QString>("completionText");

    // NOTE: Add tests carefully, ensurely the paths exist on all systems
    // Output is the sourceText; currentCompletionText()

    for (int i = 0; i < 2; i++) {
        if (i == 1)
            QTest::newRow("FILTERING_OFF") << "FILTERING_OFF" << "" << "" << "";

#if defined(Q_OS_WINCE)
        QTest::newRow("()") << "" << "" << "/" << "/";
        QTest::newRow("()") << "\\Program" << "" << "Program Files" << "\\Program Files";
#elif defined(Q_OS_WIN)
        QTest::newRow("()") << "C" << "" << "C:" << "C:";
        QTest::newRow("()") << "C:\\Program" << "" << "Program Files" << "C:\\Program Files";
#elif defined(Q_OS_SYMBIAN)
        QTest::newRow("()") << "C" << "" << "C:" << "C:";
        QTest::newRow("()") << "C:\\re" << "" << "resource" << "C:\\resource";
#elif defined (Q_OS_MAC)
        QTest::newRow("()") << "" << "" << "/" << "/";
        QTest::newRow("(/a)") << "/a" << "" << "Applications" << "/Applications";
        QTest::newRow("(/d)") << "/d" << "" << "Developer" << "/Developer";
#else
        QTest::newRow("()") << "" << "" << "/" << "/";
#if !defined(Q_OS_IRIX) && !defined(Q_OS_AIX) && !defined(Q_OS_HPUX)
        QTest::newRow("(/h)") << "/h" << "" << "home" << "/home";
#endif
        QTest::newRow("(/et)") << "/et" << "" << "etc" << "/etc";
        QTest::newRow("(/etc/passw)") << "/etc/passw" << "" << "passwd" << "/etc/passwd";
#endif
    }
}

void tst_QCompleter::directoryModel()
{
    filter();
}

void tst_QCompleter::changingModel_data()
{
}

void tst_QCompleter::changingModel()
{
    for (int i = 0; i < 2; i++) {
        delete completer;
        completer = new CsvCompleter;
        completer->setModelSorting(QCompleter::CaseSensitivelySortedModel);
        completer->setCaseSensitivity(Qt::CaseSensitive);
        setSourceModel(CASE_SENSITIVELY_SORTED_MODEL);

        if (i == 1) {
            completer->setCompletionMode(QCompleter::UnfilteredPopupCompletion);
        }

        completer->setCompletionPrefix("p");
        completer->setCurrentRow(completer->completionCount() - 1);
        QCOMPARE(completer->currentCompletion(), QString("p4"));

        // Test addition of data
        QTreeWidgetItem p5item;
        p5item.setText(completionColumn, "p5");
        treeWidget->addTopLevelItem(&p5item);
        completer->setCompletionPrefix("p5");
        QCOMPARE(completer->currentCompletion(), QString("p5"));

        // Test removal of data
        int p5index = treeWidget->indexOfTopLevelItem(&p5item);
        treeWidget->takeTopLevelItem(p5index);
        QCOMPARE(completer->currentCompletion(), QString(""));

        // Test clear
        treeWidget->clear();
        QCOMPARE(completer->currentIndex(), QModelIndex());
    }
}

void tst_QCompleter::testRowCount()
{
    QFETCH(QString, filterText);
    QFETCH(bool, hasChildren);
    QFETCH(int, rowCount);
    QFETCH(int, completionCount);

    if (filterText.compare("FILTERING_OFF", Qt::CaseInsensitive) == 0) {
        completer->setCompletionMode(QCompleter::UnfilteredPopupCompletion);
        return;
    }

    completer->setCompletionPrefix(filterText);
    const QAbstractItemModel *completionModel = completer->completionModel();
    QCOMPARE(completionModel->rowCount(), rowCount);
    QCOMPARE(completionCount, completionCount);
    QCOMPARE(completionModel->hasChildren(), hasChildren);
    QCOMPARE(completionModel->columnCount(), columnCount);
}

void tst_QCompleter::sortedEngineRowCount_data()
{
    delete completer;
    completer = new CsvCompleter;
    completer->setModelSorting(QCompleter::CaseInsensitivelySortedModel);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    setSourceModel(CASE_INSENSITIVELY_SORTED_MODEL);

    QTest::addColumn<QString>("filterText");
    QTest::addColumn<bool>("hasChildren");
    QTest::addColumn<int>("rowCount");
    QTest::addColumn<int>("completionCount");

    QTest::newRow("whatever") << "whatever" << false << 0 << 0;
    QTest::newRow("p") << "p" << true << 10 << 10;
    QTest::newRow("p1") << "p1" << true << 2 << 2;
    QTest::newRow("P1,") << "P1," << true << 5 << 5;
    QTest::newRow("P1,c") << "P1,c" << true << 5 << 5;
    QTest::newRow("P1,cc") << "P1,cc" << false << 0 << 0;

    QTest::newRow("FILTERING_OFF") << "FILTERING_OFF" << false << 0 << 0;

    QTest::newRow("whatever(filter off)") << "whatever" << true << 10 << 0;
    QTest::newRow("p1(filter off)") << "p1" << true << 10 << 2;
    QTest::newRow("p1,(filter off)") << "p1," << true << 5 << 5;
    QTest::newRow("p1,c(filter off)") << "p1,c" << true << 5 << 5;
    QTest::newRow("P1,cc(filter off)") << "P1,cc" << true << 5 << 0;
}

void tst_QCompleter::sortedEngineRowCount()
{
    testRowCount();
}

void tst_QCompleter::unsortedEngineRowCount_data()
{
    delete completer;
    completer = new CsvCompleter;
    completer->setModelSorting(QCompleter::CaseInsensitivelySortedModel);
    completer->setCaseSensitivity(Qt::CaseSensitive);
    setSourceModel(CASE_INSENSITIVELY_SORTED_MODEL);

    QTest::addColumn<QString>("filterText");
    QTest::addColumn<bool>("hasChildren");
    QTest::addColumn<int>("rowCount");
    QTest::addColumn<int>("completionCount");

    QTest::newRow("whatever") << "whatever" << false << 0 << 0;
    QTest::newRow("p") << "p" << true << 5 << 5;
    QTest::newRow("p1") << "p1" << true << 1 << 1;
    QTest::newRow("P1,") << "P1," << true << 5 << 5;
    QTest::newRow("P1,c") << "P1,c" << true << 5 << 5;
    QTest::newRow("P1,cc") << "P1,cc" << false << 0 << 0;

    QTest::newRow("FILTERING_OFF") << "FILTERING_OFF" << false << 0 << 0;

    QTest::newRow("whatever(filter off)") << "whatever" << true << 10 << 0;
    QTest::newRow("p1(filter off)") << "p1" << true << 10 << 1;
    QTest::newRow("p1,(filter off)") << "p1," << true << 5 << 5;
    QTest::newRow("p1,c(filter off)") << "p1,c" << true << 5 << 5;
    QTest::newRow("P1,cc(filter off)") << "P1,cc" << true << 5 << 0;
}

void tst_QCompleter::unsortedEngineRowCount()
{
    testRowCount();
}

void tst_QCompleter::currentRow()
{
    delete completer;
    completer = new CsvCompleter;
    completer->setModelSorting(QCompleter::CaseInsensitivelySortedModel);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    setSourceModel(CASE_INSENSITIVELY_SORTED_MODEL);

    // blank text
    completer->setCompletionPrefix("");
    QCOMPARE(completer->currentRow(), 0);
    QVERIFY(completer->setCurrentRow(4));
    QCOMPARE(completer->currentRow(), 4);
    QVERIFY(!completer->setCurrentRow(13));
    QVERIFY(completer->setCurrentRow(4));

    // some text
     completer->setCompletionPrefix("p1");
    QCOMPARE(completer->currentRow(), 0);
    QVERIFY(completer->setCurrentRow(1));
    QCOMPARE(completer->currentRow(), 1);
    QVERIFY(!completer->setCurrentRow(2));
    QCOMPARE(completer->currentRow(), 1);

    // invalid text
    completer->setCompletionPrefix("well");
    QCOMPARE(completer->currentRow(), -1);
}

void tst_QCompleter::sortedEngineMapFromSource()
{
    delete completer;
    completer = new CsvCompleter;
    completer->setModelSorting(QCompleter::CaseInsensitivelySortedModel);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    setSourceModel(CASE_INSENSITIVELY_SORTED_MODEL);

    QModelIndex si1, si2, pi;
    QAbstractItemModel *sourceModel = completer->model();
    const QAbstractProxyModel *completionModel =
        qobject_cast<const QAbstractProxyModel *>(completer->completionModel());

    // Fitering ON
    // empty
    si1 = sourceModel->index(4, completionColumn); // "P2"
    si2 = sourceModel->index(2, 0, si1); // "P2,c0P2"
    pi = completionModel->mapFromSource(si1);
    QCOMPARE(completionModel->data(pi).toString(), QLatin1String("P2"));
    pi = completionModel->mapFromSource(si2);
    QCOMPARE(pi.isValid(), false);

    // some text
    completer->setCompletionPrefix("p");
    pi = completionModel->mapFromSource(si1);
    QCOMPARE(completionModel->data(pi).toString(), QLatin1String("P2"));
    pi = completionModel->mapFromSource(si2);
    QCOMPARE(pi.isValid(), false);

    // more text
    completer->setCompletionPrefix("p2");
    pi = completionModel->mapFromSource(si1);
    QCOMPARE(completionModel->data(pi).toString(), QLatin1String("P2"));
    pi = completionModel->mapFromSource(si2);
    QCOMPARE(pi.isValid(), false);

    // invalid text
    completer->setCompletionPrefix("whatever");
    pi = completionModel->mapFromSource(si1);
    QVERIFY(!pi.isValid());

    // Fitering OFF
    completer->setCompletionMode(QCompleter::UnfilteredPopupCompletion);
    // empty
    si1 = sourceModel->index(4, completionColumn); // "P2"
    si2 = sourceModel->index(2, 0, si1); // "P2,c0P2"
    pi = completionModel->mapFromSource(si1);
    QCOMPARE(completionModel->data(pi).toString(), QLatin1String("P2"));
    pi = completionModel->mapFromSource(si2);
    QCOMPARE(pi.isValid(), false);

    // some text
    completer->setCompletionPrefix("p");
    pi = completionModel->mapFromSource(si1);
    QCOMPARE(completionModel->data(pi).toString(), QLatin1String("P2"));
    pi = completionModel->mapFromSource(si2);
    QCOMPARE(pi.isValid(), false);

    // more text
    completer->setCompletionPrefix("p2");
    pi = completionModel->mapFromSource(si1);
    QCOMPARE(completionModel->data(pi).toString(), QLatin1String("P2"));
    pi = completionModel->mapFromSource(si2);
    QCOMPARE(pi.isValid(), false);

    // invalid text
    completer->setCompletionPrefix("whatever");
    pi = completionModel->mapFromSource(si1);
    QCOMPARE(completionModel->data(pi).toString(), QLatin1String("P2"));
}

void tst_QCompleter::unsortedEngineMapFromSource()
{
    delete completer;
    completer = new CsvCompleter;
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    setSourceModel(HISTORY_MODEL); // case insensitively sorted model
    completer->setModelSorting(QCompleter::UnsortedModel);

    QModelIndex si, si2, si3, pi;
    QAbstractItemModel *sourceModel = completer->model();
    const QAbstractProxyModel *completionModel =
        qobject_cast<const QAbstractProxyModel *>(completer->completionModel());

    si = sourceModel->index(6, completionColumn); // "P3"
    QCOMPARE(si.data().toString(), QLatin1String("P3"));
    si2 = sourceModel->index(3, completionColumn, sourceModel->index(0, completionColumn)); // "P0,c3P0"
    QCOMPARE(si2.data().toString(), QLatin1String("c3P0"));
    si3 = sourceModel->index(10, completionColumn); // "p3,c3p3" (history)
    QCOMPARE(si3.data().toString(), QLatin1String("p3,c3p3"));

    // FILTERING ON
    // empty
    pi = completionModel->mapFromSource(si);
    QCOMPARE(completionModel->data(pi).toString(), QLatin1String("P3"));
    pi = completionModel->mapFromSource(si2);
    QCOMPARE(pi.isValid(), false);
    pi = completionModel->mapFromSource(si3);
   QCOMPARE(completionModel->data(pi).toString(), QLatin1String("p3,c3p3"));

    // some text
    completer->setCompletionPrefix("P");
    pi = completionModel->mapFromSource(si);
    QCOMPARE(completionModel->data(pi).toString(), QLatin1String("P3"));
    pi = completionModel->mapFromSource(si2);
    QCOMPARE(pi.isValid(), false);
    pi = completionModel->mapFromSource(si3);
    QCOMPARE(completionModel->data(pi).toString(), QLatin1String("p3,c3p3"));

    // invalid text
    completer->setCompletionPrefix("whatever");
    pi = completionModel->mapFromSource(si);
    QVERIFY(!pi.isValid());
    pi = completionModel->mapFromSource(si2);
    QVERIFY(!pi.isValid());

    // tree matching
    completer->setCompletionPrefix("P0,c");
    pi = completionModel->mapFromSource(si);
    QVERIFY(!pi.isValid());
    pi = completionModel->mapFromSource(si2);
    QCOMPARE(completionModel->data(pi).toString(), QLatin1String("c3P0"));
    pi = completionModel->mapFromSource(si3);
    QCOMPARE(pi.isValid(), false);

    // more tree matching
    completer->setCompletionPrefix("p3,");
    pi = completionModel->mapFromSource(si2);
    QVERIFY(!pi.isValid());
    pi = completionModel->mapFromSource(si3);
    QCOMPARE(completionModel->data(pi).toString(), QLatin1String("p3,c3p3"));

    // FILTERING OFF
    // empty
    completer->setCompletionPrefix("");
    completer->setCompletionMode(QCompleter::UnfilteredPopupCompletion);
    pi = completionModel->mapFromSource(si);
    QCOMPARE(completionModel->data(pi).toString(), QLatin1String("P3"));

    // some text
    completer->setCompletionPrefix("P");
    pi = completionModel->mapFromSource(si);
    QCOMPARE(completionModel->data(pi).toString(), QLatin1String("P3"));

    // more text
    completer->setCompletionPrefix("P3");
    pi = completionModel->mapFromSource(si);
    QCOMPARE(completionModel->data(pi).toString(), QLatin1String("P3"));

    // invalid text
    completer->setCompletionPrefix("whatever");
    pi = completionModel->mapFromSource(si);
    QCOMPARE(completionModel->data(pi).toString(), QLatin1String("P3"));
}

void tst_QCompleter::historySearch()
{
    delete completer;
    completer = new CsvCompleter;
    completer->setModelSorting(QCompleter::CaseInsensitivelySortedModel);
    completer->setCaseSensitivity(Qt::CaseSensitive);
    setSourceModel(HISTORY_MODEL);

    const QAbstractProxyModel *completionModel =
      qobject_cast<const QAbstractProxyModel *>(completer->completionModel());

    // "p3,c3p3" and "p2,c4p2" are added in the tree root

    // FILTERING ON
    // empty
    completer->setCurrentRow(10);
    QCOMPARE(completer->currentCompletion(), QLatin1String("p3,c3p3"));

    // more text
    completer->setCompletionPrefix("p2");
    completer->setCurrentRow(1);
    QCOMPARE(completer->currentCompletion(), QLatin1String("p2,c4p2"));

    // comma separated text
    completer->setCompletionPrefix("p2,c4");
    completer->setCurrentRow(1);
    QCOMPARE(completionModel->rowCount(), 2);
    QCOMPARE(completer->currentCompletion(), QLatin1String("p2,c4p2"));

    // invalid text
    completer->setCompletionPrefix("whatever");
    QCOMPARE(completer->currentCompletion(), QString());

    // FILTERING OFF
    completer->setCompletionPrefix("");
    completer->setCompletionMode(QCompleter::UnfilteredPopupCompletion);
    completer->setCurrentRow(10);
    QCOMPARE(completer->currentCompletion(), QLatin1String("p3,c3p3"));

    // more text
    completer->setCompletionPrefix("p2");
    completer->setCurrentRow(1);
    QCOMPARE(completer->currentCompletion(), QLatin1String("p2,c4p2"));

    // comma separated text
    completer->setCompletionPrefix("p2,c4");
    QCOMPARE(completionModel->rowCount(), 5);

    // invalid text
    completer->setCompletionPrefix("whatever");
    QCOMPARE(completer->currentCompletion(), QString());
}

void tst_QCompleter::setters()
{
    delete completer;
    completer = new CsvCompleter;
    QVERIFY(completer->popup() != 0);
    QPointer<QDirModel> dirModel = new QDirModel(completer);
    QAbstractItemModel *oldModel = completer->model();
    completer->setModel(dirModel);
    QVERIFY(completer->popup()->model() != oldModel);
    QVERIFY(completer->popup()->model() == completer->completionModel());
    completer->setPopup(new QListView);
    QVERIFY(completer->popup()->model() == completer->completionModel());
    completer->setModel(new QStringListModel(completer));
    QVERIFY(dirModel == 0); // must have been deleted

    completer->setModel(0);
    completer->setWidget(0);
}

void tst_QCompleter::modelDeletion()
{
    delete completer;
    completer = new CsvCompleter;
    QStringList list;
    list << "item1" << "item2" << "item3";
    QStringListModel *listModel = new QStringListModel(list);
    completer->setCompletionPrefix("i");
    completer->setModel(listModel);
    QVERIFY(completer->completionCount() == 3);
    QListView *view = new QListView;
    view->setModel(completer->completionModel());
    delete listModel;
    view->show();
    qApp->processEvents();
    delete view;
    QVERIFY(completer->completionCount() == 0);
    QVERIFY(completer->currentRow() == -1);
}

void tst_QCompleter::multipleWidgets()
{
    QStringList list;
    list << "item1" << "item2" << "item2";
    QCompleter completer(list);
    completer.setCompletionMode(QCompleter::InlineCompletion);

    QWidget window;
    window.show();
    QApplication::setActiveWindow(&window);
    QTest::qWaitForWindowShown(&window);
    QTRY_VERIFY(qApp->activeWindow() == &window);

    QFocusEvent focusIn(QEvent::FocusIn);
    QFocusEvent focusOut(QEvent::FocusOut);

    QComboBox *comboBox = new QComboBox(&window);
    comboBox->setEditable(true);
    comboBox->setCompleter(&completer);
    comboBox->setFocus();
    comboBox->show();
    window.activateWindow();
    QApplication::setActiveWindow(&window);
    QTest::qWait(50);
    QTRY_VERIFY(qApp->focusWidget() == comboBox);
    comboBox->lineEdit()->setText("it");
    QCOMPARE(comboBox->currentText(), QString("it")); // should not complete with setText
    QTest::keyPress(comboBox, 'e');
    QCOMPARE(comboBox->currentText(), QString("item1"));
    comboBox->clearEditText();
    QCOMPARE(comboBox->currentText(), QString("")); // combo box text must not change!

    QLineEdit *lineEdit = new QLineEdit(&window);
    lineEdit->setCompleter(&completer);
    lineEdit->show();
    lineEdit->setFocus();
    QTest::qWait(50);
    QTRY_VERIFY(qApp->focusWidget() == lineEdit);
    lineEdit->setText("it");
    QCOMPARE(lineEdit->text(), QString("it")); // should not completer with setText
    QCOMPARE(comboBox->currentText(), QString("")); // combo box text must not change!
    QTest::keyPress(lineEdit, 'e');
    QCOMPARE(lineEdit->text(), QString("item1"));
    QCOMPARE(comboBox->currentText(), QString("")); // combo box text must not change!
}

void tst_QCompleter::focusIn()
{
    QStringList list;
    list << "item1" << "item2" << "item2";
    QCompleter completer(list);

    QWidget window;
    window.show();
    QTest::qWait(100);
    window.activateWindow();
    QApplication::setActiveWindow(&window);
    QTest::qWait(100);

    QTRY_COMPARE(qApp->activeWindow(), &window);

    QComboBox *comboBox = new QComboBox(&window);
    comboBox->setEditable(true);
    comboBox->setCompleter(&completer);
    comboBox->show();
    comboBox->lineEdit()->setText("it");

    QLineEdit *lineEdit = new QLineEdit(&window);
    lineEdit->setCompleter(&completer);
    lineEdit->setText("it");
    lineEdit->show();

    QLineEdit *lineEdit2 = new QLineEdit(&window); // has no completer!
    lineEdit2->show();

    comboBox->setFocus();
    QTRY_VERIFY(completer.widget() == comboBox);
    lineEdit->setFocus();
    QTRY_VERIFY(completer.widget() == lineEdit);
    comboBox->setFocus();
    QTRY_VERIFY(completer.widget() == comboBox);
    lineEdit2->setFocus();
    QTRY_VERIFY(completer.widget() == comboBox);
}

void tst_QCompleter::dynamicSortOrder()
{
    QStandardItemModel model;
    QCompleter completer(&model);
    completer.setModelSorting(QCompleter::CaseSensitivelySortedModel);
    QStandardItem *root = model.invisibleRootItem();
    for (int i = 0; i < 20; i++) {
        root->appendRow(new QStandardItem(QString("%1").arg(i)));
    }
    root->appendRow(new QStandardItem("13"));
    root->sortChildren(0, Qt::AscendingOrder);
    completer.setCompletionPrefix("1");
    QCOMPARE(completer.completionCount(), 12);
    completer.setCompletionPrefix("13");
    QCOMPARE(completer.completionCount(), 2);

    root->sortChildren(0, Qt::DescendingOrder);
    completer.setCompletionPrefix("13");
    QCOMPARE(completer.completionCount(), 2);
    completer.setCompletionPrefix("1");
    QCOMPARE(completer.completionCount(), 12);
}

void tst_QCompleter::disabledItems()
{
    QLineEdit lineEdit;
    QStandardItemModel *model = new QStandardItemModel(&lineEdit);
    QStandardItem *suggestions = new QStandardItem("suggestions");
    suggestions->setEnabled(false);
    model->appendRow(suggestions);
    model->appendRow(new QStandardItem("suggestions Enabled"));
    QCompleter *completer = new QCompleter(model, &lineEdit);
    QSignalSpy spy(completer, SIGNAL(activated(const QString &)));
    lineEdit.setCompleter(completer);
    lineEdit.show();

    QTest::keyPress(&lineEdit, Qt::Key_S);
    QTest::keyPress(&lineEdit, Qt::Key_U);
    QAbstractItemView *view = lineEdit.completer()->popup();
    QVERIFY(view->isVisible());
    QTest::mouseClick(view->viewport(), Qt::LeftButton, 0, view->visualRect(view->model()->index(0, 0)).center());
    QCOMPARE(spy.count(), 0);
    QVERIFY(view->isVisible());
    QTest::mouseClick(view->viewport(), Qt::LeftButton, 0, view->visualRect(view->model()->index(1, 0)).center());
    QCOMPARE(spy.count(), 1);
    QVERIFY(!view->isVisible());
}

void tst_QCompleter::task178797_activatedOnReturn()
{
    QStringList words;
    words << "foobar1" << "foobar2";
    QLineEdit *ledit = new QLineEdit;
    QCompleter *completer = new QCompleter(words);
    ledit->setCompleter(completer);
    QSignalSpy spy(completer, SIGNAL(activated(const QString)));
    QCOMPARE(spy.count(), 0);
    ledit->show();
    QTest::keyClick(ledit, Qt::Key_F);
    qApp->processEvents();
    QVERIFY(qApp->activePopupWidget());
    QTest::keyClick(qApp->activePopupWidget(), Qt::Key_Down);
    qApp->processEvents();
    QTest::keyClick(qApp->activePopupWidget(), Qt::Key_Return);
    qApp->processEvents();
    QCOMPARE(spy.count(), 1);
}

class task189564_StringListModel : public QStringListModel
{
    const QString omitString;
    Qt::ItemFlags flags(const QModelIndex &index) const
    {
        Qt::ItemFlags flags = Qt::ItemIsEnabled;
        if (data(index, Qt::DisplayRole).toString() != omitString)
            flags |= Qt::ItemIsSelectable;
        return flags;
    }
public:
    task189564_StringListModel(const QString &omitString, QObject *parent = 0)
        : QStringListModel(parent)
        , omitString(omitString)
    {
    }
};

void tst_QCompleter::task189564_omitNonSelectableItems()
{
    const QString prefix("a");
    Q_ASSERT(!prefix.isEmpty());
    const int n = 5;
    Q_ASSERT(n > 0);

    QStringList strings;
    for (int i = 0; i < n; ++i)
        strings << QString("%1%2").arg(prefix).arg(i);
    const QString omitString(strings.at(n / 2));
    task189564_StringListModel model(omitString);
    model.setStringList(strings);
    QCompleter completer_(&model);
    completer_.setCompletionPrefix(prefix);

    QAbstractItemModel *completionModel = completer_.completionModel();
    QModelIndexList matches1 =
        completionModel->match(completionModel->index(0, 0), Qt::DisplayRole, prefix, -1);
    QCOMPARE(matches1.size(), n - 1);
    QModelIndexList matches2 =
        completionModel->match(completionModel->index(0, 0), Qt::DisplayRole, omitString);
    QVERIFY(matches2.isEmpty());
}

class task246056_ComboBox : public QComboBox
{
    Q_OBJECT
public:
    task246056_ComboBox()
    {
        setEditable(true);
        setInsertPolicy(NoInsert);
        Q_ASSERT(completer());
        completer()->setCompletionMode(QCompleter::PopupCompletion);
        completer()->setCompletionRole(Qt::DisplayRole);
        connect(lineEdit(), SIGNAL(editingFinished()), SLOT(setCompletionPrefix()));
    }
private slots:
    void setCompletionPrefix() { completer()->setCompletionPrefix(lineEdit()->text()); }
};

void tst_QCompleter::task246056_setCompletionPrefix()
{
    task246056_ComboBox *comboBox = new task246056_ComboBox;
    comboBox->addItem("");
    comboBox->addItem("a1");
    comboBox->addItem("a2");
    comboBox->show();
    comboBox->setFocus();
    QTest::qWait(100);
    QTest::keyPress(comboBox, 'a');
    QTest::keyPress(comboBox->completer()->popup(), Qt::Key_Down);
    QTest::keyPress(comboBox->completer()->popup(), Qt::Key_Down);
    QTest::keyPress(comboBox->completer()->popup(), Qt::Key_Enter); // don't crash!
}

class task250064_TextEdit : public QTextEdit
{
public:
    QCompleter *completer;

    task250064_TextEdit()
    {
        completer = new QCompleter;
        completer->setWidget(this);
    }

    void keyPressEvent (QKeyEvent *e)
    {
        completer->popup();
        QTextEdit::keyPressEvent(e);
    }
};

class task250064_Widget : public QWidget
{
    Q_OBJECT
public:
    task250064_TextEdit *textEdit;

    task250064_Widget(task250064_TextEdit *textEdit)
        : textEdit(textEdit)
    {
        QTabWidget *tabWidget = new QTabWidget;
        tabWidget->setFocusPolicy(Qt::ClickFocus);
        tabWidget->addTab(textEdit, "untitled");

        QVBoxLayout *layout = new QVBoxLayout(this);
        layout->addWidget(tabWidget);

        textEdit->setPlainText("bla bla bla");
        textEdit->setFocus();
    }

    void setCompletionModel()
    {
        textEdit->completer->setModel(0);
    }
};

void tst_QCompleter::task250064_lostFocus()
{
    task250064_TextEdit *textEdit = new task250064_TextEdit;
    task250064_Widget *widget = new task250064_Widget(textEdit);
    widget->show();
    QTest::qWait(100);
    QTest::keyPress(textEdit, 'a');
    Qt::FocusPolicy origPolicy = textEdit->focusPolicy();
    QVERIFY(origPolicy != Qt::NoFocus);
    widget->setCompletionModel();
    QCOMPARE(textEdit->focusPolicy(), origPolicy);
}

void tst_QCompleter::task253125_lineEditCompletion_data()
{
    QTest::addColumn<QStringList>("list");
    QTest::addColumn<int>("completionMode");

    QStringList list = QStringList()
        << "alpha" << "beta"    << "gamma"   << "delta" << "epsilon" << "zeta"
        << "eta"   << "theta"   << "iota"    << "kappa" << "lambda"  << "mu"
        << "nu"    << "xi"      << "omicron" << "pi"    << "rho"     << "sigma"
        << "tau"   << "upsilon" << "phi"     << "chi"   << "psi"     << "omega";

    QTest::newRow("Inline") << list << (int)QCompleter::InlineCompletion;
    QTest::newRow("Filtered") << list << (int)QCompleter::PopupCompletion;
    QTest::newRow("Unfiltered") << list << (int)QCompleter::UnfilteredPopupCompletion;
}

void tst_QCompleter::task253125_lineEditCompletion()
{
    QFETCH(QStringList, list);
    QFETCH(int, completionMode);

    QStringListModel *model = new QStringListModel;
    model->setStringList(list);

    QCompleter *completer = new QCompleter(list);
    completer->setModel(model);
    completer->setCompletionMode((QCompleter::CompletionMode)completionMode);

    QLineEdit edit;
    edit.setCompleter(completer);
    edit.show();
    edit.setFocus();

#ifdef Q_WS_X11
    qt_x11_wait_for_window_manager(&edit);
#endif
    QTest::qWait(10);
    QApplication::setActiveWindow(&edit);
    QTRY_COMPARE(QApplication::activeWindow(), static_cast<QWidget *>(&edit));

    QTest::keyClick(&edit, 'i');
    QCOMPARE(edit.completer()->currentCompletion(), QString("iota"));
    QTest::keyClick(edit.completer()->popup(), Qt::Key_Down);
    QTest::keyClick(edit.completer()->popup(), Qt::Key_Enter);

    QCOMPARE(edit.text(), QString("iota"));

    delete completer;
    delete model;
}

void tst_QCompleter::task247560_keyboardNavigation()
{
    QStandardItemModel model;

    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            model.setItem(i, j, new QStandardItem(QString("row %1 column %2").arg(i).arg(j)));
        }
    }


    QCompleter completer(&model);
    completer.setCompletionColumn(1);

    QLineEdit edit;
    edit.setCompleter(&completer);
    edit.show();
    edit.setFocus();

#ifdef Q_WS_X11
    qt_x11_wait_for_window_manager(&edit);
#endif

    QTest::qWait(10);
    QApplication::setActiveWindow(&edit);
    QTRY_COMPARE(QApplication::activeWindow(), static_cast<QWidget *>(&edit));

    QTest::keyClick(&edit, 'r');
    QTest::keyClick(edit.completer()->popup(), Qt::Key_Down);
    QTest::keyClick(edit.completer()->popup(), Qt::Key_Down);
    QTest::keyClick(edit.completer()->popup(), Qt::Key_Enter);

    QCOMPARE(edit.text(), QString("row 1 column 1"));

    edit.clear();

    QTest::keyClick(&edit, 'r');
    QTest::keyClick(edit.completer()->popup(), Qt::Key_Up);
    QTest::keyClick(edit.completer()->popup(), Qt::Key_Up);
    QTest::keyClick(edit.completer()->popup(), Qt::Key_Enter);

    QCOMPARE(edit.text(), QString("row 3 column 1"));
}

QTEST_MAIN(tst_QCompleter)
#include "tst_qcompleter.moc"
