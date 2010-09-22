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

#include <qcoreapplication.h>
#include <qdebug.h>
#include <qfiledialog.h>
#include <qabstractitemdelegate.h>
#include <qdirmodel.h>
#include <qitemdelegate.h>
#include <qlistview.h>
#include <qcombobox.h>
#include <qpushbutton.h>
#include <qtoolbutton.h>
#include <qtreeview.h>
#include <qheaderview.h>
#include <qcompleter.h>
#include <qaction.h>
#include <qdialogbuttonbox.h>
#include <qsortfilterproxymodel.h>
#include <qlineedit.h>
#include <qlayout.h>
#include "../../shared/util.h"
#include "../../../src/gui/dialogs/qsidebar_p.h"
#include "../../../src/gui/dialogs/qfilesystemmodel_p.h"
#include "../../../src/gui/dialogs/qfiledialog_p.h"

#include "../network-settings.h"

//TESTED_CLASS=
//TESTED_FILES=

#if defined(Q_OS_SYMBIAN)
# define STRINGIFY(x) #x
# define TOSTRING(x) STRINGIFY(x)
# define SRCDIR "C:/Private/" TOSTRING(SYMBIAN_SRCDIR_UID) "/"
#endif

#if defined QT_BUILD_INTERNAL
QT_BEGIN_NAMESPACE
Q_GUI_EXPORT bool qt_test_isFetchedRoot();
Q_GUI_EXPORT void qt_test_resetFetchedRoot();
QT_END_NAMESPACE
#endif

class QNonNativeFileDialog : public QFileDialog
{
    Q_OBJECT
public:
    QNonNativeFileDialog(QWidget *parent = 0, const QString &caption = QString(), const QString &directory = QString(), const QString &filter = QString())
        : QFileDialog(parent, caption, directory, filter)
    {
        setOption(QFileDialog::DontUseNativeDialog, true);
    }
};

class tst_QFileDialog2 : public QObject
{
Q_OBJECT

public:
    tst_QFileDialog2();
    virtual ~tst_QFileDialog2();

public slots:
    void init();
    void cleanup();

private slots:
    void listRoot();
    void heapCorruption();
    void deleteDirAndFiles();
    void filter();
    void showNameFilterDetails();
    void unc();
    void emptyUncPath();

    void task178897_minimumSize();
    void task180459_lastDirectory_data();
    void task180459_lastDirectory();
    void task227304_proxyOnFileDialog();
    void task227930_correctNavigationKeyboardBehavior();
#if defined(Q_OS_WIN) && !defined(Q_OS_WINCE)
    void task226366_lowerCaseHardDriveWindows();
#endif
    void completionOnLevelAfterRoot();
    void task233037_selectingDirectory();
    void task235069_hideOnEscape();
    void task236402_dontWatchDeletedDir();
    void task203703_returnProperSeparator();
    void task228844_ensurePreviousSorting();
    void task239706_editableFilterCombo();
    void task218353_relativePaths();
    void task251321_sideBarHiddenEntries();
    void task251341_sideBarRemoveEntries();
    void task254490_selectFileMultipleTimes();
    void task257579_sideBarWithNonCleanUrls();
    void task259105_filtersCornerCases();

    void QTBUG4419_lineEditSelectAll();
    void QTBUG6558_showDirsOnly();
    void QTBUG4842_selectFilterWithHideNameFilterDetails();

private:
    QByteArray userSettings;
};

tst_QFileDialog2::tst_QFileDialog2()
{
#if defined(Q_OS_WINCE)
    qApp->setAutoMaximizeThreshold(-1);
#endif
}

tst_QFileDialog2::~tst_QFileDialog2()
{
}

void tst_QFileDialog2::init()
{
    // Save the developers settings so they don't get mad when their sidebar folders are gone.
    QSettings settings(QSettings::UserScope, QLatin1String("Trolltech"));
    settings.beginGroup(QLatin1String("Qt"));
    userSettings = settings.value(QLatin1String("filedialog")).toByteArray();
    settings.remove(QLatin1String("filedialog"));

    // populate it with some default settings
    QNonNativeFileDialog fd;
#if defined(Q_OS_WINCE)
    QTest::qWait(1000);
#endif
}

void tst_QFileDialog2::cleanup()
{
    QSettings settings(QSettings::UserScope, QLatin1String("Trolltech"));
    settings.beginGroup(QLatin1String("Qt"));
    settings.setValue(QLatin1String("filedialog"), userSettings);
}

void tst_QFileDialog2::listRoot()
{
#if defined QT_BUILD_INTERNAL
    QFileInfoGatherer fileInfoGatherer;
    fileInfoGatherer.start();
    QTest::qWait(1500);
    qt_test_resetFetchedRoot();
    QString dir(QDir::currentPath());
    QNonNativeFileDialog fd(0, QString(), dir);
    fd.show();
    QCOMPARE(qt_test_isFetchedRoot(),false);
    fd.setDirectory("");
#ifdef Q_OS_WINCE
    QTest::qWait(1500);
#else
    QTest::qWait(500);
#endif
    QCOMPARE(qt_test_isFetchedRoot(),true);
#endif
}

void tst_QFileDialog2::heapCorruption()
{
    QVector<QNonNativeFileDialog*> dialogs;
    for (int i=0; i < 10; i++) {
        QNonNativeFileDialog *f = new QNonNativeFileDialog(NULL);
        dialogs << f;
    }
    qDeleteAll(dialogs);
}

struct FriendlyQFileDialog : public QNonNativeFileDialog
{
    friend class tst_QFileDialog2;
    Q_DECLARE_PRIVATE(QFileDialog)
};


void tst_QFileDialog2::deleteDirAndFiles()
{
#if defined QT_BUILD_INTERNAL
    QString tempPath = QDir::tempPath() + '/' + "QFileDialogTestDir4FullDelete";
    QDir dir;
    QVERIFY(dir.mkpath(tempPath + "/foo"));
    QVERIFY(dir.mkpath(tempPath + "/foo/B"));
    QVERIFY(dir.mkpath(tempPath + "/foo/B"));
    QVERIFY(dir.mkpath(tempPath + "/foo/c"));
    QVERIFY(dir.mkpath(tempPath + "/bar"));
    QFile(tempPath + "/foo/a");
    QTemporaryFile *t;
    t = new QTemporaryFile(tempPath + "/foo/aXXXXXX");
    t->setAutoRemove(false);
    t->open();
    t->close();
    delete t;

    t = new QTemporaryFile(tempPath + "/foo/B/yXXXXXX");
    t->setAutoRemove(false);
    t->open();
    t->close();
    delete t;
    FriendlyQFileDialog fd;
    fd.setOption(QFileDialog::DontUseNativeDialog);
    fd.d_func()->removeDirectory(tempPath);
    QFileInfo info(tempPath);
    QTest::qWait(2000);
    QVERIFY(!info.exists());
#endif
}

void tst_QFileDialog2::filter()
{
    QNonNativeFileDialog fd;
    QAction *hiddenAction = qFindChild<QAction*>(&fd, "qt_show_hidden_action");
    QVERIFY(hiddenAction);
    QVERIFY(hiddenAction->isEnabled());
    QVERIFY(!hiddenAction->isChecked());
    QDir::Filters filter = fd.filter();
    filter |= QDir::Hidden;
    fd.setFilter(filter);
    QVERIFY(hiddenAction->isChecked());
}

void tst_QFileDialog2::showNameFilterDetails()
{
    QNonNativeFileDialog fd;
    QComboBox *filters = qFindChild<QComboBox*>(&fd, "fileTypeCombo");
    QVERIFY(filters);
    QVERIFY(fd.isNameFilterDetailsVisible());


    QStringList filterChoices;
    filterChoices << "Image files (*.png *.xpm *.jpg)"
                  << "Text files (*.txt)"
                  << "Any files (*.*)";
    fd.setFilters(filterChoices);

    fd.setNameFilterDetailsVisible(false);
    QCOMPARE(filters->itemText(0), QString("Image files"));
    QCOMPARE(filters->itemText(1), QString("Text files"));
    QCOMPARE(filters->itemText(2), QString("Any files"));

    fd.setNameFilterDetailsVisible(true);
    QCOMPARE(filters->itemText(0), filterChoices.at(0));
    QCOMPARE(filters->itemText(1), filterChoices.at(1));
    QCOMPARE(filters->itemText(2), filterChoices.at(2));
}

void tst_QFileDialog2::unc()
{
#if defined(Q_OS_WIN) && !defined(Q_OS_WINCE)
    // Only test UNC on Windows./
    QString dir("\\\\"  + QtNetworkSettings::winServerName() + "\\testsharewritable");
#else
    QString dir(QDir::currentPath());
#endif
    QVERIFY(QFile::exists(dir));
    QNonNativeFileDialog fd(0, QString(), dir);
    QFileSystemModel *model = qFindChild<QFileSystemModel*>(&fd, "qt_filesystem_model");
    QVERIFY(model);
    QCOMPARE(model->index(fd.directory().absolutePath()), model->index(dir));
}

void tst_QFileDialog2::emptyUncPath()
{
    QNonNativeFileDialog fd;
	fd.show();
	QLineEdit *lineEdit = qFindChild<QLineEdit*>(&fd, "fileNameEdit");
    QVERIFY(lineEdit);
	 // press 'keys' for the input
    for (int i = 0; i < 3 ; ++i)
        QTest::keyPress(lineEdit, Qt::Key_Backslash);
    QFileSystemModel *model = qFindChild<QFileSystemModel*>(&fd, "qt_filesystem_model");
    QVERIFY(model);
}

void tst_QFileDialog2::task178897_minimumSize()
{
    QNonNativeFileDialog fd;
    QSize oldMs = fd.layout()->minimumSize();
    QStringList history = fd.history();
    history << QDir::toNativeSeparators("/verylongdirectory/"
            "aaaaaaaaaabbbbbbbbcccccccccccddddddddddddddeeeeeeeeeeeeffffffffffgggtggggggggghhhhhhhhiiiiiijjjk");
    fd.setHistory(history);
    fd.show();

    QSize ms = fd.layout()->minimumSize();
    QVERIFY(ms.width() <= oldMs.width());
}

void tst_QFileDialog2::task180459_lastDirectory_data()
{
    QTest::addColumn<QString>("path");
    QTest::addColumn<QString>("directory");
    QTest::addColumn<bool>("isEnabled");
    QTest::addColumn<QString>("result");

    QTest::newRow("path+file") << QDir::homePath() + QDir::separator() + "foo"
            << QDir::homePath()  << true
            << QDir::homePath() + QDir::separator() + "foo"  ;
    QTest::newRow("no path") << ""
            << QDir::tempPath() << false << QString();
    QTest::newRow("file") << "foo"
            << QDir::currentPath() << true
            << QDir::currentPath() + QDir::separator() + "foo"  ;
    QTest::newRow("path") << QDir::homePath()
            << QDir::homePath() << false << QString();
    QTest::newRow("path not existing") << "/usr/bin/foo/bar/foo/foo.txt"
            << QDir::tempPath() << true
            << QDir::tempPath() +  QDir::separator() + "foo.txt";

}

void tst_QFileDialog2::task180459_lastDirectory()
{
    //first visit the temp directory and close the dialog
    QNonNativeFileDialog *dlg = new QNonNativeFileDialog(0, "", QDir::tempPath());
    QFileSystemModel *model = qFindChild<QFileSystemModel*>(dlg, "qt_filesystem_model");
    QVERIFY(model);
    QCOMPARE(model->index(QDir::tempPath()), model->index(dlg->directory().absolutePath()));
    delete dlg;

    QFETCH(QString, path);
    QFETCH(QString, directory);
    QFETCH(bool, isEnabled);
    QFETCH(QString, result);

    dlg = new QNonNativeFileDialog(0, "", path);
    model = qFindChild<QFileSystemModel*>(dlg, "qt_filesystem_model");
    QVERIFY(model);
    dlg->setAcceptMode(QFileDialog::AcceptSave);
    QCOMPARE(model->index(dlg->directory().absolutePath()), model->index(directory));

    QDialogButtonBox *buttonBox = qFindChild<QDialogButtonBox*>(dlg, "buttonBox");
    QPushButton *button = buttonBox->button(QDialogButtonBox::Save);
    QVERIFY(button);
    QCOMPARE(button->isEnabled(), isEnabled);
    if (isEnabled)
        QCOMPARE(model->index(result), model->index(dlg->selectedFiles().first()));

    delete dlg;
}



class FilterDirModel : public QSortFilterProxyModel
{

public:
      FilterDirModel(QString root, QObject* parent=0):QSortFilterProxyModel(parent), m_root(root)
      {}
      ~FilterDirModel()
      {};

protected:
      bool filterAcceptsRow(int source_row, const QModelIndex& source_parent) const
      {
            QModelIndex parentIndex;
            parentIndex = source_parent;

            QString path;
            path = parentIndex.child(source_row,0).data(Qt::DisplayRole).toString();

            do {
              path = parentIndex.data(Qt::DisplayRole).toString() + "/" + path;
              parentIndex = parentIndex.parent();
            } while(parentIndex.isValid());

            QFileInfo info(path);
            if (info.isDir() && (QDir(path) != m_root))
                return false;
            return true;
      }


private:
      QDir m_root;


};

class sortProxy : public QSortFilterProxyModel
{
public:
        sortProxy(QObject *parent) : QSortFilterProxyModel(parent)
        {
        }
protected:
        virtual bool lessThan(const QModelIndex &left, const QModelIndex &right) const
        {
            QFileSystemModel * const model = qobject_cast<QFileSystemModel *>(sourceModel());
            const QFileInfo leftInfo(model->fileInfo(left));
            const QFileInfo rightInfo(model->fileInfo(right));

            if (leftInfo.isDir() == rightInfo.isDir())
                return(leftInfo.filePath().compare(rightInfo.filePath(),Qt::CaseInsensitive) < 0);
            else if (leftInfo.isDir())
                return(false);
            else
                return(true);
        }
};

class CrashDialog : public QNonNativeFileDialog
{
        Q_OBJECT

public:
        CrashDialog(QWidget *parent, const QString &caption, const
QString &dir, const QString &filter)
                   : QNonNativeFileDialog(parent, caption, dir, filter)
        {
                sortProxy *proxyModel = new sortProxy(this);
                setProxyModel(proxyModel);
        }
};

void tst_QFileDialog2::task227304_proxyOnFileDialog()
{
#if defined QT_BUILD_INTERNAL
    QNonNativeFileDialog fd(0, "", QDir::currentPath(), 0);
    fd.setProxyModel(new FilterDirModel(QDir::currentPath()));
    fd.show();
    QLineEdit *edit = qFindChild<QLineEdit*>(&fd, "fileNameEdit");
    QTest::qWait(200);
    QTest::keyClick(edit, Qt::Key_T);
    QTest::keyClick(edit, Qt::Key_S);
    QTest::qWait(200);
    QTest::keyClick(edit->completer()->popup(), Qt::Key_Down);

    CrashDialog *dialog = new CrashDialog(0, QString("crash dialog test"), QDir::homePath(), QString("*") );
    dialog->setFileMode(QFileDialog::ExistingFile);
    dialog->show();

    QListView *list = qFindChild<QListView*>(dialog, "listView");
    QTest::qWait(200);
    QTest::keyClick(list, Qt::Key_Down);
    QTest::keyClick(list, Qt::Key_Return);
    QTest::qWait(200);

    dialog->close();
    fd.close();

    QNonNativeFileDialog fd2(0, "I should not crash with a proxy", QDir::tempPath(), 0);
    QSortFilterProxyModel *pm = new QSortFilterProxyModel;
    fd2.setProxyModel(pm);
    fd2.show();
    QSidebar *sidebar = qFindChild<QSidebar*>(&fd2, "sidebar");
    sidebar->setFocus();
    sidebar->selectUrl(QUrl::fromLocalFile(QDir::homePath()));
    QTest::mouseClick(sidebar->viewport(), Qt::LeftButton, 0, sidebar->visualRect(sidebar->model()->index(1, 0)).center());
    QTest::qWait(250);
    //We shouldn't crash
#endif
}

void tst_QFileDialog2::task227930_correctNavigationKeyboardBehavior()
{
    QDir current = QDir::currentPath();
    current.mkdir("test");
    current.cd("test");
    QFile file("test/out.txt");
    QFile file2("test/out2.txt");
    QVERIFY(file.open(QIODevice::WriteOnly | QIODevice::Text));
    QVERIFY(file2.open(QIODevice::WriteOnly | QIODevice::Text));
    current.cdUp();
    current.mkdir("test2");
    QNonNativeFileDialog fd;
    fd.setViewMode(QFileDialog::List);
    fd.setDirectory(current.absolutePath());
    fd.show();
    QListView *list = qFindChild<QListView*>(&fd, "listView");
    QTest::qWait(200);
    QTest::keyClick(list, Qt::Key_Down);
    QTest::keyClick(list, Qt::Key_Return);
    QTest::qWait(200);
    QTest::mouseClick(list->viewport(), Qt::LeftButton,0);
    QTest::keyClick(list, Qt::Key_Down);
    QTest::keyClick(list, Qt::Key_Backspace);
    QTest::qWait(200);
    QTest::keyClick(list, Qt::Key_Down);
    QTest::keyClick(list, Qt::Key_Down);
    QTest::keyClick(list, Qt::Key_Return);
    QTest::qWait(200);
    QCOMPARE(fd.isVisible(), true);
    QTest::qWait(200);
    file.close();
    file2.close();
    file.remove();
    file2.remove();
    current.rmdir("test");
    current.rmdir("test2");
}

#if defined(Q_OS_WIN) && !defined(Q_OS_WINCE)
void tst_QFileDialog2::task226366_lowerCaseHardDriveWindows()
{
    QNonNativeFileDialog fd;
    fd.setDirectory(QDir::root().path());
    fd.show();
    QLineEdit *edit = qFindChild<QLineEdit*>(&fd, "fileNameEdit");
    QToolButton *buttonParent = qFindChild<QToolButton*>(&fd, "toParentButton");
    QTest::qWait(200);
    QTest::mouseClick(buttonParent, Qt::LeftButton,0,QPoint(0,0));
    QTest::qWait(2000);
    QTest::keyClick(edit, Qt::Key_C);
    QTest::qWait(200);
    QTest::keyClick(edit->completer()->popup(), Qt::Key_Down);
    QTest::qWait(200);
    QCOMPARE(edit->text(), QString("C:"));
    QTest::qWait(2000);
    //i clear my previous selection in the completer
    QTest::keyClick(edit->completer()->popup(), Qt::Key_Down);
    edit->clear();
    QTest::keyClick(edit, (char)(Qt::Key_C | Qt::SHIFT));
    QTest::qWait(200);
    QTest::keyClick(edit->completer()->popup(), Qt::Key_Down);
    QCOMPARE(edit->text(), QString("C:"));
}
#endif

void tst_QFileDialog2::completionOnLevelAfterRoot()
{
    QNonNativeFileDialog fd;
#if defined(Q_OS_WIN) && !defined(Q_OS_WINCE)
    fd.setDirectory("C:");
    QDir current = fd.directory();
    current.mkdir("completionOnLevelAfterRootTest");
#else
    fd.setFilter(QDir::Hidden | QDir::AllDirs | QDir::Files | QDir::System);
    fd.setDirectory("/");
    QDir etc("/etc");
    if (!etc.exists())
        QSKIP("This test requires to have an etc directory under /", SkipAll);
#endif
    fd.show();
    QLineEdit *edit = qFindChild<QLineEdit*>(&fd, "fileNameEdit");
    QTest::qWait(2000);
#if defined(Q_OS_WIN) && !defined(Q_OS_WINCE)
    //I love testlib :D
    QTest::keyClick(edit, Qt::Key_C);
    QTest::keyClick(edit, Qt::Key_O);
    QTest::keyClick(edit, Qt::Key_M);
    QTest::keyClick(edit, Qt::Key_P);
    QTest::keyClick(edit, Qt::Key_L);
#else
    QTest::keyClick(edit, Qt::Key_E);
    QTest::keyClick(edit, Qt::Key_T);
#endif
    QTest::qWait(200);
    QTest::keyClick(edit->completer()->popup(), Qt::Key_Down);
    QTest::qWait(200);
#if defined(Q_OS_WIN) && !defined(Q_OS_WINCE)
    QCOMPARE(edit->text(), QString("completionOnLevelAfterRootTest"));
    current.rmdir("completionOnLevelAfterRootTest");
#else
    QCOMPARE(edit->text(), QString("etc"));
#endif
}

void tst_QFileDialog2::task233037_selectingDirectory()
{
    QDir current = QDir::currentPath();
    current.mkdir("test");
    QNonNativeFileDialog fd;
    fd.setViewMode(QFileDialog::List);
    fd.setDirectory(current.absolutePath());
    fd.setAcceptMode( QFileDialog::AcceptSave);
    fd.show();
    QListView *list = qFindChild<QListView*>(&fd, "listView");
    QTest::qWait(3000); // Wait for sort to settle (I need a signal).
#ifdef QT_KEYPAD_NAVIGATION
    list->setEditFocus(true);
#endif
    QTest::keyClick(list, Qt::Key_Down);
    QTest::qWait(100);
    QDialogButtonBox *buttonBox = qFindChild<QDialogButtonBox*>(&fd, "buttonBox");
    QPushButton *button = buttonBox->button(QDialogButtonBox::Save);
    QVERIFY(button);
    QCOMPARE(button->isEnabled(), true);
    current.rmdir("test");
}

void tst_QFileDialog2::task235069_hideOnEscape()
{
    QDir current = QDir::currentPath();
    QNonNativeFileDialog fd;
    fd.setViewMode(QFileDialog::List);
    fd.setDirectory(current.absolutePath());
    fd.setAcceptMode( QFileDialog::AcceptSave);
    fd.show();
    QListView *list = qFindChild<QListView*>(&fd, "listView");
    list->setFocus();
    QTest::qWait(200);
    QTest::keyClick(list, Qt::Key_Escape);
    QCOMPARE(fd.isVisible(), false);
    QNonNativeFileDialog fd2;
    fd2.setDirectory(current.absolutePath());
    fd2.setAcceptMode( QFileDialog::AcceptSave);
    fd2.show();
    QLineEdit *edit = qFindChild<QLineEdit*>(&fd2, "fileNameEdit");
    QTest::keyClick(edit, Qt::Key_Escape);
    QCOMPARE(fd2.isVisible(), false);
}

void tst_QFileDialog2::task236402_dontWatchDeletedDir()
{
#if defined QT_BUILD_INTERNAL
    //THIS TEST SHOULD NOT DISPLAY WARNINGS
    QDir current = QDir::currentPath();
    //make sure it is the first on the list
    current.mkdir("aaaaaaaaaa");
    FriendlyQFileDialog fd;
    fd.setViewMode(QFileDialog::List);
    fd.setDirectory(current.absolutePath());
    fd.setAcceptMode( QFileDialog::AcceptSave);
    fd.show();
    QListView *list = qFindChild<QListView*>(&fd, "listView");
    list->setFocus();
    QTest::qWait(200);
    QTest::keyClick(list, Qt::Key_Return);
    QTest::qWait(200);
    QTest::keyClick(list, Qt::Key_Backspace);
    QTest::keyClick(list, Qt::Key_Down);
    QTest::qWait(200);
    fd.d_func()->removeDirectory(current.absolutePath() + "/aaaaaaaaaa/");
    QTest::qWait(1000);
#endif
}

void tst_QFileDialog2::task203703_returnProperSeparator()
{
    QDir current = QDir::currentPath();
    current.mkdir("aaaaaaaaaaaaaaaaaa");
    QNonNativeFileDialog fd;
    fd.setDirectory(current.absolutePath());
    fd.setViewMode(QFileDialog::List);
    fd.setFileMode(QFileDialog::Directory);
    fd.show();
    QTest::qWait(500);
    QListView *list = qFindChild<QListView*>(&fd, "listView");
    list->setFocus();
    QTest::qWait(200);
    QTest::keyClick(list, Qt::Key_Return);
    QTest::qWait(1000);
    QDialogButtonBox *buttonBox = qFindChild<QDialogButtonBox*>(&fd, "buttonBox");
    QPushButton *button = buttonBox->button(QDialogButtonBox::Cancel);
    QTest::keyClick(button, Qt::Key_Return);
    QTest::qWait(500);
    QString result = fd.selectedFiles().first();
    QVERIFY(result.at(result.count() - 1) != '/');
    QVERIFY(!result.contains('\\'));
    current.rmdir("aaaaaaaaaaaaaaaaaa");
}

void tst_QFileDialog2::task228844_ensurePreviousSorting()
{
    QDir current = QDir::currentPath();
    current.mkdir("aaaaaaaaaaaaaaaaaa");
    current.cd("aaaaaaaaaaaaaaaaaa");
    current.mkdir("a");
    current.mkdir("b");
    current.mkdir("c");
    current.mkdir("d");
    current.mkdir("e");
    current.mkdir("f");
    current.mkdir("g");
    QTemporaryFile *tempFile = new QTemporaryFile(current.absolutePath() + "/rXXXXXX");
    tempFile->open();
    current.cdUp();

    QNonNativeFileDialog fd;
    fd.setDirectory(current.absolutePath());
    fd.setViewMode(QFileDialog::Detail);
    fd.show();
#if defined(Q_OS_WINCE)
    QTest::qWait(1500);
#else
    QTest::qWait(500);
#endif
    QTreeView *tree = qFindChild<QTreeView*>(&fd, "treeView");
    tree->header()->setSortIndicator(3,Qt::DescendingOrder);
    QTest::qWait(200);
    QDialogButtonBox *buttonBox = qFindChild<QDialogButtonBox*>(&fd, "buttonBox");
    QPushButton *button = buttonBox->button(QDialogButtonBox::Open);
    QTest::mouseClick(button, Qt::LeftButton);
#if defined(Q_OS_WINCE)
    QTest::qWait(1500);
#else
    QTest::qWait(500);
#endif
    QNonNativeFileDialog fd2;
    fd2.setFileMode(QFileDialog::Directory);
    fd2.restoreState(fd.saveState());
    current.cd("aaaaaaaaaaaaaaaaaa");
    fd2.setDirectory(current.absolutePath());
    fd2.show();
#if defined(Q_OS_WINCE)
    QTest::qWait(1500);
#else
    QTest::qWait(500);
#endif
    QTreeView *tree2 = qFindChild<QTreeView*>(&fd2, "treeView");
    tree2->setFocus();

    QCOMPARE(tree2->rootIndex().data(QFileSystemModel::FilePathRole).toString(),current.absolutePath());

    QDialogButtonBox *buttonBox2 = qFindChild<QDialogButtonBox*>(&fd2, "buttonBox");
    QPushButton *button2 = buttonBox2->button(QDialogButtonBox::Open);
    fd2.selectFile("g");
    QTest::mouseClick(button2, Qt::LeftButton);
#if defined(Q_OS_WINCE)
    QTest::qWait(1500);
#else
    QTest::qWait(500);
#endif
    QCOMPARE(fd2.selectedFiles().first(), current.absolutePath() + QChar('/') + QLatin1String("g"));

    QNonNativeFileDialog fd3(0, "This is a third file dialog", tempFile->fileName());
    fd3.restoreState(fd.saveState());
    fd3.setFileMode(QFileDialog::Directory);
    fd3.show();
#if defined(Q_OS_WINCE)
    QTest::qWait(1500);
#else
    QTest::qWait(500);
#endif
    QTreeView *tree3 = qFindChild<QTreeView*>(&fd3, "treeView");
    tree3->setFocus();

    QCOMPARE(tree3->rootIndex().data(QFileSystemModel::FilePathRole).toString(), current.absolutePath());

    QDialogButtonBox *buttonBox3 = qFindChild<QDialogButtonBox*>(&fd3, "buttonBox");
    QPushButton *button3 = buttonBox3->button(QDialogButtonBox::Open);
    QTest::mouseClick(button3, Qt::LeftButton);
#if defined(Q_OS_WINCE)
    QTest::qWait(1500);
#else
    QTest::qWait(500);
#endif
    QCOMPARE(fd3.selectedFiles().first(), tempFile->fileName());

    current.cd("aaaaaaaaaaaaaaaaaa");
    current.rmdir("a");
    current.rmdir("b");
    current.rmdir("c");
    current.rmdir("d");
    current.rmdir("e");
    current.rmdir("f");
    current.rmdir("g");
    tempFile->close();
    delete tempFile;
    current.cdUp();
    current.rmdir("aaaaaaaaaaaaaaaaaa");
}


void tst_QFileDialog2::task239706_editableFilterCombo()
{
    QNonNativeFileDialog d;
    d.setNameFilter("*.cpp *.h");

    d.show();
    QTest::qWait(500);

    QList<QComboBox *> comboList = d.findChildren<QComboBox *>();
    QComboBox *filterCombo = 0;
    foreach (QComboBox *combo, comboList) {
        if (combo->objectName() == QString("fileTypeCombo")) {
            filterCombo = combo;
            break;
        }
    }
    Q_ASSERT(filterCombo);
    filterCombo->setEditable(true);
    QTest::mouseClick(filterCombo, Qt::LeftButton);
    QTest::keyPress(filterCombo, Qt::Key_X);
    QTest::keyPress(filterCombo, Qt::Key_Enter); // should not trigger assertion failure
}

void tst_QFileDialog2::task218353_relativePaths()
{
    QDir appDir = QDir::current();
    QVERIFY(appDir.cdUp() != false);
    QNonNativeFileDialog d(0, "TestDialog", "..");
    QCOMPARE(d.directory().absolutePath(), appDir.absolutePath());

    d.setDirectory(appDir.absolutePath() + QLatin1String("/non-existing-directory/../another-non-existing-dir/../"));
    QCOMPARE(d.directory().absolutePath(), appDir.absolutePath());

    QDir::current().mkdir("test");
    appDir = QDir::current();
    d.setDirectory(appDir.absolutePath() + QLatin1String("/test/../test/../"));
    QCOMPARE(d.directory().absolutePath(), appDir.absolutePath());
    appDir.rmdir("test");
}

void tst_QFileDialog2::task251321_sideBarHiddenEntries()
{
#if defined QT_BUILD_INTERNAL
    QNonNativeFileDialog fd;

    QDir current = QDir::currentPath();
    current.mkdir(".hidden");
    QDir hiddenDir = QDir(".hidden");
    hiddenDir.mkdir("subdir");
    QDir hiddenSubDir = QDir(".hidden/subdir");
    hiddenSubDir.mkdir("happy");
    hiddenSubDir.mkdir("happy2");

    QList<QUrl> urls;
    urls << QUrl::fromLocalFile(hiddenSubDir.absolutePath());
    fd.setSidebarUrls(urls);
    fd.show();
    QTest::qWait(250);

    QSidebar *sidebar = qFindChild<QSidebar*>(&fd, "sidebar");
    sidebar->setFocus();
    sidebar->selectUrl(QUrl::fromLocalFile(hiddenSubDir.absolutePath()));
    QTest::mouseClick(sidebar->viewport(), Qt::LeftButton, 0, sidebar->visualRect(sidebar->model()->index(0, 0)).center());
    // give the background processes more time on windows mobile
#ifdef Q_OS_WINCE
    QTest::qWait(1000);
#else
    QTest::qWait(250);
#endif

    QFileSystemModel *model = qFindChild<QFileSystemModel*>(&fd, "qt_filesystem_model");
    QCOMPARE(model->rowCount(model->index(hiddenSubDir.absolutePath())), 2);

    hiddenSubDir.rmdir("happy2");
    hiddenSubDir.rmdir("happy");
    hiddenDir.rmdir("subdir");
    current.rmdir(".hidden");
#endif
}

#if defined QT_BUILD_INTERNAL
class MyQSideBar : public QSidebar
{
public :
    MyQSideBar(QWidget *parent = 0) : QSidebar(parent)
    {}

    void removeSelection() {
        QList<QModelIndex> idxs = selectionModel()->selectedIndexes();
        QList<QPersistentModelIndex> indexes;
        for (int i = 0; i < idxs.count(); i++)
            indexes.append(idxs.at(i));

        for (int i = 0; i < indexes.count(); ++i)
            if (!indexes.at(i).data(Qt::UserRole + 1).toUrl().path().isEmpty())
                model()->removeRow(indexes.at(i).row());
    }
};
#endif

void tst_QFileDialog2::task251341_sideBarRemoveEntries()
{
#if defined QT_BUILD_INTERNAL
    QNonNativeFileDialog fd;

    QDir current = QDir::currentPath();
    current.mkdir("testDir");
    QDir testSubDir = QDir("testDir");

    QList<QUrl> urls;
    urls << QUrl::fromLocalFile(testSubDir.absolutePath());
    urls << QUrl::fromLocalFile("NotFound");
    fd.setSidebarUrls(urls);
    fd.show();
    QTest::qWait(250);

    QSidebar *sidebar = qFindChild<QSidebar*>(&fd, "sidebar");
    sidebar->setFocus();
    //We enter in the first bookmark
    sidebar->selectUrl(QUrl::fromLocalFile(testSubDir.absolutePath()));
    QTest::mouseClick(sidebar->viewport(), Qt::LeftButton, 0, sidebar->visualRect(sidebar->model()->index(0, 0)).center());
    QTest::qWait(250);

    QFileSystemModel *model = qFindChild<QFileSystemModel*>(&fd, "qt_filesystem_model");
    //There is no file
    QCOMPARE(model->rowCount(model->index(testSubDir.absolutePath())), 0);
    //Icon is not enabled QUrlModel::EnabledRole
    QVariant value = sidebar->model()->index(0, 0).data(Qt::UserRole + 2);
    QCOMPARE(qvariant_cast<bool>(value), true);

    sidebar->setFocus();
    //We enter in the second bookmark which is invalid
    sidebar->selectUrl(QUrl::fromLocalFile("NotFound"));
    QTest::mouseClick(sidebar->viewport(), Qt::LeftButton, 0, sidebar->visualRect(sidebar->model()->index(1, 0)).center());
    QTest::qWait(250);

    //We fallback to root because the entry in the bookmark is invalid
    QCOMPARE(model->rowCount(model->index("NotFound")), model->rowCount(model->index(model->rootPath())));
    //Icon is not enabled QUrlModel::EnabledRole
    value = sidebar->model()->index(1, 0).data(Qt::UserRole + 2);
    QCOMPARE(qvariant_cast<bool>(value), false);

    MyQSideBar mySideBar;
    mySideBar.init(model, urls);
    mySideBar.show();
    mySideBar.selectUrl(QUrl::fromLocalFile(testSubDir.absolutePath()));
    QTest::qWait(1000);
    mySideBar.removeSelection();

    //We remove the first entry
    QList<QUrl> expected;
    expected << QUrl::fromLocalFile("NotFound");
    QCOMPARE(mySideBar.urls(), expected);

    mySideBar.selectUrl(QUrl::fromLocalFile("NotFound"));
    mySideBar.removeSelection();

    //We remove the second entry
    expected.clear();
    QCOMPARE(mySideBar.urls(), expected);

    current.rmdir("testDir");
#endif
}

void tst_QFileDialog2::task254490_selectFileMultipleTimes()
{
    QString tempPath = QDir::tempPath();
    QTemporaryFile *t;
    t = new QTemporaryFile;
    t->open();
    QNonNativeFileDialog fd(0, "TestFileDialog");

    fd.setDirectory(tempPath);
    fd.setViewMode(QFileDialog::List);
    fd.setAcceptMode(QFileDialog::AcceptSave);
    fd.setFileMode(QFileDialog::AnyFile);

    //This should select the file in the QFileDialog
    fd.selectFile(t->fileName());

    //This should clear the selection and write it into the filename line edit
    fd.selectFile("new_file.txt");

    fd.show();
    QTest::qWait(250);

    QLineEdit *lineEdit = qFindChild<QLineEdit*>(&fd, "fileNameEdit");
    QVERIFY(lineEdit);
    QCOMPARE(lineEdit->text(),QLatin1String("new_file.txt"));
    QListView *list = qFindChild<QListView*>(&fd, "listView");
    QVERIFY(list);
    QCOMPARE(list->selectionModel()->selectedRows(0).count(), 0);

    t->deleteLater();
}

void tst_QFileDialog2::task257579_sideBarWithNonCleanUrls()
{
#if defined QT_BUILD_INTERNAL
    QDir tempDir = QDir::temp();
    QLatin1String dirname("autotest_task257579");
    tempDir.rmdir(dirname); //makes sure it doesn't exist any more
    QVERIFY(tempDir.mkdir(dirname));
    QString url = QString::fromLatin1("%1/%2/..").arg(tempDir.absolutePath()).arg(dirname);
    QNonNativeFileDialog fd;
    fd.setSidebarUrls(QList<QUrl>() << QUrl::fromLocalFile(url));
    QSidebar *sidebar = qFindChild<QSidebar*>(&fd, "sidebar");
    QCOMPARE(sidebar->urls().count(), 1);
    QVERIFY(sidebar->urls().first().toLocalFile() != url);
    QCOMPARE(sidebar->urls().first().toLocalFile(), QDir::cleanPath(url));

#ifdef Q_OS_WIN
    QCOMPARE(sidebar->model()->index(0,0).data().toString().toLower(), tempDir.dirName().toLower());
#else
    QCOMPARE(sidebar->model()->index(0,0).data().toString(), tempDir.dirName());
#endif

    //all tests are finished, we can remove the temporary dir
    QVERIFY(tempDir.rmdir(dirname));
#endif
}

void tst_QFileDialog2::task259105_filtersCornerCases()
{
    QNonNativeFileDialog fd(0, "TestFileDialog");
    fd.setNameFilter(QLatin1String("All Files! (*);;Text Files (*.txt)"));
    fd.setOption(QFileDialog::HideNameFilterDetails, true);
    fd.show();
    QTest::qWait(250);

    //Extensions are hidden
    QComboBox *filters = qFindChild<QComboBox*>(&fd, "fileTypeCombo");
    QVERIFY(filters);
    QCOMPARE(filters->currentText(), QLatin1String("All Files!"));
    filters->setCurrentIndex(1);
    QCOMPARE(filters->currentText(), QLatin1String("Text Files"));

    //We should have the full names
    fd.setOption(QFileDialog::HideNameFilterDetails, false);
    QTest::qWait(250);
    filters->setCurrentIndex(0);
    QCOMPARE(filters->currentText(), QLatin1String("All Files! (*)"));
    filters->setCurrentIndex(1);
    QCOMPARE(filters->currentText(), QLatin1String("Text Files (*.txt)"));

    //Corner case undocumented of the task
    fd.setNameFilter(QLatin1String("\352 (I like cheese) All Files! (*);;Text Files (*.txt)"));
    QCOMPARE(filters->currentText(), QLatin1String("\352 (I like cheese) All Files! (*)"));
    filters->setCurrentIndex(1);
    QCOMPARE(filters->currentText(), QLatin1String("Text Files (*.txt)"));

    fd.setOption(QFileDialog::HideNameFilterDetails, true);
    filters->setCurrentIndex(0);
    QTest::qWait(500);
    QCOMPARE(filters->currentText(), QLatin1String("\352 (I like cheese) All Files!"));
    filters->setCurrentIndex(1);
    QCOMPARE(filters->currentText(), QLatin1String("Text Files"));

    fd.setOption(QFileDialog::HideNameFilterDetails, true);
    filters->setCurrentIndex(0);
    QTest::qWait(500);
    QCOMPARE(filters->currentText(), QLatin1String("\352 (I like cheese) All Files!"));
    filters->setCurrentIndex(1);
    QCOMPARE(filters->currentText(), QLatin1String("Text Files"));
}

void tst_QFileDialog2::QTBUG4419_lineEditSelectAll()
{
    QString tempPath = QDir::tempPath();
    QTemporaryFile *t;
    t = new QTemporaryFile;
    t->open();
    QNonNativeFileDialog fd(0, "TestFileDialog", t->fileName());

    fd.setDirectory(tempPath);
    fd.setViewMode(QFileDialog::List);
    fd.setAcceptMode(QFileDialog::AcceptSave);
    fd.setFileMode(QFileDialog::AnyFile);

    fd.show();
    QApplication::setActiveWindow(&fd);
    QTest::qWaitForWindowShown(&fd);
    QTRY_COMPARE(fd.isVisible(), true);
    QTRY_COMPARE(QApplication::activeWindow(), static_cast<QWidget*>(&fd));

    QTest::qWait(250);
    QLineEdit *lineEdit = qFindChild<QLineEdit*>(&fd, "fileNameEdit");

    QCOMPARE(tempPath + QChar('/') + lineEdit->text(), t->fileName());
    QCOMPARE(tempPath + QChar('/') + lineEdit->selectedText(), t->fileName());
}

void tst_QFileDialog2::QTBUG6558_showDirsOnly()
{
    const QString tempPath = QDir::tempPath();
    QDir dirTemp(tempPath);
    const QString tempName = QLatin1String("showDirsOnly.") + QString::number(qrand());
    dirTemp.mkdir(tempName);
    dirTemp.cd(tempName);
    QTRY_VERIFY(dirTemp.exists());

    const QString dirPath = dirTemp.absolutePath();
    QDir dir(dirPath);

    //We create two dirs
    dir.mkdir("a");
    dir.mkdir("b");

    //Create a file
    QFile tempFile(dirPath + "/plop.txt");
    tempFile.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&tempFile);
    out << "The magic number is: " << 49 << "\n";
    tempFile.close();

    QNonNativeFileDialog fd(0, "TestFileDialog");

    fd.setDirectory(dir.absolutePath());
    fd.setViewMode(QFileDialog::List);
    fd.setAcceptMode(QFileDialog::AcceptSave);
    fd.setOption(QFileDialog::ShowDirsOnly, true);
    fd.show();

    QApplication::setActiveWindow(&fd);
    QTest::qWaitForWindowShown(&fd);
    QTRY_COMPARE(fd.isVisible(), true);
    QTRY_COMPARE(QApplication::activeWindow(), static_cast<QWidget*>(&fd));

    QFileSystemModel *model = qFindChild<QFileSystemModel*>(&fd, "qt_filesystem_model");
    QTRY_COMPARE(model->rowCount(model->index(dir.absolutePath())), 2);

    fd.setOption(QFileDialog::ShowDirsOnly, false);
    QTRY_COMPARE(model->rowCount(model->index(dir.absolutePath())), 3);

    fd.setOption(QFileDialog::ShowDirsOnly, true);
    QTRY_COMPARE(model->rowCount(model->index(dir.absolutePath())), 2);

    fd.setFileMode(QFileDialog::DirectoryOnly);
    QTRY_COMPARE(model->rowCount(model->index(dir.absolutePath())), 2);
    QTRY_COMPARE(bool(fd.options() & QFileDialog::ShowDirsOnly), true);

    fd.setFileMode(QFileDialog::AnyFile);
    QTRY_COMPARE(model->rowCount(model->index(dir.absolutePath())), 3);
    QTRY_COMPARE(bool(fd.options() & QFileDialog::ShowDirsOnly), false);

    fd.setDirectory(QDir::homePath());

    //We remove the dirs
    dir.rmdir("a");
    dir.rmdir("b");

    //we delete the file
    tempFile.remove();

    dirTemp.cdUp();
    dirTemp.rmdir(tempName);
}

void tst_QFileDialog2::QTBUG4842_selectFilterWithHideNameFilterDetails()
{
    QStringList filtersStr;
    filtersStr << "Images (*.png *.xpm *.jpg)" << "Text files (*.txt)" << "XML files (*.xml)";
    QString chosenFilterString("Text files (*.txt)");

    QNonNativeFileDialog fd(0, "TestFileDialog");
    fd.setAcceptMode(QFileDialog::AcceptSave);
    fd.setOption(QFileDialog::HideNameFilterDetails, true);
    fd.setNameFilters(filtersStr);
    fd.selectNameFilter(chosenFilterString);
    fd.show();

    QApplication::setActiveWindow(&fd);
    QTest::qWaitForWindowShown(&fd);
    QTRY_COMPARE(fd.isVisible(), true);
    QTRY_COMPARE(QApplication::activeWindow(), static_cast<QWidget*>(&fd));

    QComboBox *filters = qFindChild<QComboBox*>(&fd, "fileTypeCombo");
    //We compare the current combobox text with the stripped version
    QCOMPARE(filters->currentText(), QString("Text files"));

    QNonNativeFileDialog fd2(0, "TestFileDialog");
    fd2.setAcceptMode(QFileDialog::AcceptSave);
    fd2.setOption(QFileDialog::HideNameFilterDetails, false);
    fd2.setNameFilters(filtersStr);
    fd2.selectNameFilter(chosenFilterString);
    fd2.show();

    QApplication::setActiveWindow(&fd2);
    QTest::qWaitForWindowShown(&fd2);
    QTRY_COMPARE(fd2.isVisible(), true);
    QTRY_COMPARE(QApplication::activeWindow(), static_cast<QWidget*>(&fd2));

    QComboBox *filters2 = qFindChild<QComboBox*>(&fd2, "fileTypeCombo");
    //We compare the current combobox text with the non stripped version
    QCOMPARE(filters2->currentText(), chosenFilterString);

}

QTEST_MAIN(tst_QFileDialog2)
#include "tst_qfiledialog2.moc"
