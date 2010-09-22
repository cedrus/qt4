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

class tst_QFiledialog : public QObject
{
Q_OBJECT

public:
    tst_QFiledialog();
    virtual ~tst_QFiledialog();

public slots:
    void init();
    void cleanup();

private slots:
    void currentChangedSignal();
    void directoryEnteredSignal();
    void filesSelectedSignal_data();
    void filesSelectedSignal();
    void filterSelectedSignal();

    void args();
    void directory();
    void completer_data();
    void completer();
    void completer_up();
    void acceptMode();
    void confirmOverwrite();
    void defaultSuffix();
    void fileMode();
    void filters();
    void history();
    void iconProvider();
    void isReadOnly();
    void itemDelegate();
    void labelText();
    void resolveSymlinks();
    void selectFile_data();
    void selectFile();
    void selectFiles();
    void selectFilter();
    void viewMode();
    void proxymodel();
    void setNameFilter();
    void focus();
    void caption();
    void historyBack();
    void historyForward();
    void disableSaveButton_data();
    void disableSaveButton();
    void saveButtonText_data();
    void saveButtonText();
    void clearLineEdit();
    void enableChooseButton();
    void hooks();

private:
    QByteArray userSettings;
};

tst_QFiledialog::tst_QFiledialog()
{
}

tst_QFiledialog::~tst_QFiledialog()
{
}

void tst_QFiledialog::init()
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

void tst_QFiledialog::cleanup()
{
    QSettings settings(QSettings::UserScope, QLatin1String("Trolltech"));
    settings.beginGroup(QLatin1String("Qt"));
    settings.setValue(QLatin1String("filedialog"), userSettings);
}

class MyAbstractItemDelegate : public QAbstractItemDelegate
{
public:
    MyAbstractItemDelegate() : QAbstractItemDelegate() {};
    void paint(QPainter *, const QStyleOptionViewItem &, const QModelIndex &) const {}
    QSize sizeHint(const QStyleOptionViewItem &, const QModelIndex &) const { return QSize(); }
};

// emited any time the selection model emits current changed
void tst_QFiledialog::currentChangedSignal()
{
    QNonNativeFileDialog fd;
    fd.setViewMode(QFileDialog::List);
    QSignalSpy spyCurrentChanged(&fd, SIGNAL(currentChanged(const QString &)));

    QListView* listView = qFindChild<QListView*>(&fd, "listView");
    QVERIFY(listView);
    fd.setDirectory(QDir::root());
    QModelIndex root = listView->rootIndex();
    QTRY_COMPARE(listView->model()->rowCount(root) > 0, true);

    QModelIndex folder;
    for (int i = 0; i < listView->model()->rowCount(root); ++i) {
        folder = listView->model()->index(i, 0, root);
        if (listView->model()->hasChildren(folder))
            break;
    }
    QVERIFY(listView->model()->hasChildren(folder));
    listView->setCurrentIndex(folder);

    QCOMPARE(spyCurrentChanged.count(), 1);
}

// only emited from the views, sidebar, or lookin combo
void tst_QFiledialog::directoryEnteredSignal()
{
#if defined QT_BUILD_INTERNAL
    QNonNativeFileDialog fd(0, "", QDir::root().path());
    fd.setOptions(QFileDialog::DontUseNativeDialog);
    fd.show();
    QTRY_COMPARE(fd.isVisible(), true);
    QSignalSpy spyDirectoryEntered(&fd, SIGNAL(directoryEntered(const QString &)));

    // sidebar
    QSidebar *sidebar = qFindChild<QSidebar*>(&fd, "sidebar");
    sidebar->setCurrentIndex(sidebar->model()->index(1, 0));
    QTest::keyPress(sidebar->viewport(), Qt::Key_Return);
    QCOMPARE(spyDirectoryEntered.count(), 1);
    spyDirectoryEntered.clear();

    // lookInCombo
    QComboBox *comboBox = qFindChild<QComboBox*>(&fd, "lookInCombo");
    comboBox->showPopup();
    QVERIFY(comboBox->view()->model()->index(1, 0).isValid());
    comboBox->view()->setCurrentIndex(comboBox->view()->model()->index(1, 0));
    QTest::keyPress(comboBox->view()->viewport(), Qt::Key_Return);
    QCOMPARE(spyDirectoryEntered.count(), 1);
    spyDirectoryEntered.clear();

    // view
    /*
    // platform specific
    fd.setViewMode(QFileDialog::ViewMode(QFileDialog::List));
    QListView* listView = qFindChild<QListView*>(&fd, "listView");
    QVERIFY(listView);
    QModelIndex root = listView->rootIndex();
    QTRY_COMPARE(listView->model()->rowCount(root) > 0, true);

    QModelIndex folder;
    for (int i = 0; i < listView->model()->rowCount(root); ++i) {
        folder = listView->model()->index(i, 0, root);
        if (listView->model()->hasChildren(folder))
            break;
    }
    QVERIFY(listView->model()->hasChildren(folder));
    listView->setCurrentIndex(folder);
    QTRY_COMPARE((listView->indexAt(listView->visualRect(folder).center())), folder);
    QTest::mouseDClick(listView->viewport(), Qt::LeftButton, 0, listView->visualRect(folder).center());
    QTRY_COMPARE(spyDirectoryEntered.count(), 1);
    */
#endif
}

Q_DECLARE_METATYPE(QFileDialog::FileMode)
void tst_QFiledialog::filesSelectedSignal_data()
{
    QTest::addColumn<QFileDialog::FileMode>("fileMode");
    QTest::newRow("any") << QFileDialog::AnyFile;
    QTest::newRow("existing") << QFileDialog::ExistingFile;
    QTest::newRow("directory") << QFileDialog::Directory;
    QTest::newRow("directoryOnly") << QFileDialog::DirectoryOnly;
    QTest::newRow("existingFiles") << QFileDialog::ExistingFiles;
}

// emited when the dialog closes with the selected files
void tst_QFiledialog::filesSelectedSignal()
{
    QNonNativeFileDialog fd;
    fd.setViewMode(QFileDialog::List);
    fd.setOptions(QFileDialog::DontUseNativeDialog);
    QDir testDir(SRCDIR"/../../..");
    fd.setDirectory(testDir);
    QFETCH(QFileDialog::FileMode, fileMode);
    fd.setFileMode(fileMode);
    QSignalSpy spyFilesSelected(&fd, SIGNAL(filesSelected(const QStringList &)));

    fd.show();
    QTest::qWait(500);
    QListView *listView = qFindChild<QListView*>(&fd, "listView");
    QVERIFY(listView);

    QModelIndex root = listView->rootIndex();
    QTRY_COMPARE(listView->model()->rowCount(root) > 0, true);
    QModelIndex file;
    for (int i = 0; i < listView->model()->rowCount(root); ++i) {
        file = listView->model()->index(i, 0, root);
        if (fileMode == QFileDialog::Directory || fileMode == QFileDialog::DirectoryOnly) {
            if (listView->model()->hasChildren(file))
                break;
        } else {
            if (!listView->model()->hasChildren(file))
                break;
        }
        file = QModelIndex();
    }
    QVERIFY(file.isValid());
    listView->selectionModel()->select(file, QItemSelectionModel::Select | QItemSelectionModel::Rows);
    listView->setCurrentIndex(file);

    QDialogButtonBox *buttonBox = qFindChild<QDialogButtonBox*>(&fd, "buttonBox");
    QPushButton *button = buttonBox->button(QDialogButtonBox::Open);
    QVERIFY(button);
    QVERIFY(button->isEnabled());
    button->animateClick();
    QTRY_COMPARE(fd.isVisible(), false);
    QCOMPARE(spyFilesSelected.count(), 1);
}

// only emited when the combo box is activated
void tst_QFiledialog::filterSelectedSignal()
{
    QNonNativeFileDialog fd;
    fd.setAcceptMode(QFileDialog::AcceptSave);
    fd.show();
    QSignalSpy spyFilterSelected(&fd, SIGNAL(filterSelected(const QString &)));

    QStringList filterChoices;
    filterChoices << "Image files (*.png *.xpm *.jpg)"
                  << "Text files (*.txt)"
                  << "Any files (*.*)";
    fd.setFilters(filterChoices);
    QCOMPARE(fd.filters(), filterChoices);

    QComboBox *filters = qFindChild<QComboBox*>(&fd, "fileTypeCombo");
    QVERIFY(filters);
    QVERIFY(filters->view());
    QCOMPARE(filters->isVisible(), true);

    QTest::keyPress(filters, Qt::Key_Down);

    QCOMPARE(spyFilterSelected.count(), 1);
}

void tst_QFiledialog::args()
{
    QWidget *parent = 0;
    QString caption = "caption";
    QString directory = QDir::tempPath();
    QString filter = "*.mp3";
    QNonNativeFileDialog fd(parent, caption, directory, filter);
    QCOMPARE(fd.parent(), (QObject *)parent);
    QCOMPARE(fd.windowTitle(), caption);
#ifndef Q_OS_WIN
    QCOMPARE(fd.directory(), QDir(directory));
#endif
    QCOMPARE(fd.filters(), QStringList(filter));
}

void tst_QFiledialog::directory()
{
    QNonNativeFileDialog fd;
    fd.setViewMode(QFileDialog::List);
    QFileSystemModel *model = qFindChild<QFileSystemModel*>(&fd, "qt_filesystem_model");
    QVERIFY(model);
    fd.setDirectory(QDir::currentPath());
    QSignalSpy spyCurrentChanged(&fd, SIGNAL(currentChanged(const QString &)));
    QSignalSpy spyDirectoryEntered(&fd, SIGNAL(directoryEntered(const QString &)));
    QSignalSpy spyFilesSelected(&fd, SIGNAL(filesSelected(const QStringList &)));
    QSignalSpy spyFilterSelected(&fd, SIGNAL(filterSelected(const QString &)));

    QCOMPARE(QDir::current().absolutePath(), fd.directory().absolutePath());
    QDir temp = QDir::temp();
    QString tempPath = temp.absolutePath();
#ifdef Q_OS_WIN
    // since the user can have lowercase temp dir, check that we are actually case-insensitive.
    tempPath = tempPath.toLower();
#endif
    fd.setDirectory(tempPath);
#ifndef Q_OS_WIN
    QCOMPARE(tempPath, fd.directory().absolutePath());
#endif
    QCOMPARE(spyCurrentChanged.count(), 0);
    QCOMPARE(spyDirectoryEntered.count(), 0);
    QCOMPARE(spyFilesSelected.count(), 0);
    QCOMPARE(spyFilterSelected.count(), 0);

    // Check my way
    QList<QListView*> list = qFindChildren<QListView*>(&fd, "listView");
    QVERIFY(list.count() > 0);
#ifdef Q_OS_WIN
    QCOMPARE(list.at(0)->rootIndex().data().toString().toLower(), temp.dirName().toLower());
#else
    QCOMPARE(list.at(0)->rootIndex().data().toString(), temp.dirName());
#endif
    QNonNativeFileDialog *dlg = new QNonNativeFileDialog(0, "", tempPath);
    QCOMPARE(model->index(tempPath), model->index(dlg->directory().absolutePath()));
    QCOMPARE(model->index(tempPath).data(QFileSystemModel::FileNameRole).toString(),
             model->index(dlg->directory().absolutePath()).data(QFileSystemModel::FileNameRole).toString());
    delete dlg;
    dlg = new QNonNativeFileDialog();
    QCOMPARE(model->index(tempPath), model->index(dlg->directory().absolutePath()));
    delete dlg;
}

void tst_QFiledialog::completer_data()
{
    QTest::addColumn<QString>("startPath");
    QTest::addColumn<QString>("input");
    QTest::addColumn<int>("expected");

    QTest::newRow("r, 10")   << "" << "r"   << 10;
    QTest::newRow("x, 0")    << "" << "x"   << 0;
    QTest::newRow("../, -1") << "" << "../" << -1;

    QTest::newRow("goto root")     << QString()        << QDir::rootPath() << -1;
    QTest::newRow("start at root") << QDir::rootPath() << QString()        << -1;

    QDir root = QDir::root();
    QStringList list = root.entryList();
    QString folder;
    for (int i = 0; i < list.count(); ++i) {
        if (list.at(0) == QChar('.'))
            continue;
        QFileInfo info(QDir::rootPath() + list[i]);
        if (info.isDir()) {
            folder = QDir::rootPath() + list[i];
            break;
        }
    }

    QTest::newRow("start at one below root r") << folder << "r" << -1;
    QTest::newRow("start at one below root ../") << folder << "../" << -1;
}

void tst_QFiledialog::completer()
{
    QFETCH(QString, input);
    QFETCH(QString, startPath);
    QFETCH(int, expected);

    QString tempPath = QDir::tempPath() + '/' + "QFileDialogTestDir";
    if (startPath.isEmpty())
        startPath = tempPath;

    startPath = QDir::cleanPath(startPath);

    // make temp dir and files
    {
        QDir cleanup(tempPath);
        QStringList x = cleanup.entryList();
        for (int i = 0; i < x.count(); ++i)
            QFile::remove(tempPath + '/' + x[i]);
        cleanup.rmdir(tempPath);
    }
    QDir tmp(QDir::tempPath());
    if (!tmp.exists(tempPath))
        QVERIFY(tmp.mkdir("QFileDialogTestDir"));
    QList<QTemporaryFile*> files;
    QT_TRY {
    for (int i = 0; i < 10; ++i) {
        QScopedPointer<QTemporaryFile> file(new QTemporaryFile(tempPath + "/rXXXXXX"));
        file->open();
        files.append(file.take());
    }

    // ### flesh this out more
    QNonNativeFileDialog fd(0,QString("Test it"),startPath);
    fd.setOptions(QFileDialog::DontUseNativeDialog);
    fd.show();
    QVERIFY(fd.isVisible());
    QFileSystemModel *model = qFindChild<QFileSystemModel*>(&fd, "qt_filesystem_model");
    QVERIFY(model);
    QLineEdit *lineEdit = qFindChild<QLineEdit*>(&fd, "fileNameEdit");
    QVERIFY(lineEdit);
    QCompleter *completer = lineEdit->completer();
    QVERIFY(completer);
    QAbstractItemModel *cModel = completer->completionModel();
    QVERIFY(cModel);

    //wait a bit
    QTest::qWait(500);

    // path C:\depot\qt\examples\dialogs\standarddialogs
    // files
    //       [debug] [release] [tmp] dialog dialog main makefile makefile.debug makefile.release standarddialgos
    //
    // d      -> D:\ debug dialog.cpp dialog.h
    // ..\    -> ..\classwizard ..\configdialog ..\dialogs.pro
    // c      -> C:\ control panel
    // c:     -> C:\ (nothing more)
    // C:\    -> C:\, C:\_viminfo, ...
    // \      -> \_viminfo
    // c:\depot  -> 'nothing'
    // c:\depot\ -> C:\depot\devtools, C:\depot\dteske
    QCOMPARE(model->index(fd.directory().path()), model->index(startPath));

    if (input.isEmpty()) {
        QModelIndex r = model->index(model->rootPath());
        QVERIFY(model->rowCount(r) > 0);
        QModelIndex idx = model->index(0, 0, r);
        input = idx.data().toString().at(0);
    }

    // press 'keys' for the input
    for (int i = 0; i < input.count(); ++i)
        QTest::keyPress(lineEdit, input[i].toAscii());

    QStringList expectedFiles;
    if (expected == -1) {
        QString fullPath = startPath.isEmpty() ? tempPath : startPath;
        if (!fullPath.endsWith(QLatin1Char('/')))
            fullPath.append(QLatin1Char('/'));
        fullPath.append(input);
        bool inputStartsWithRootPath = false;
        if (input.startsWith(QDir::rootPath())) {
            fullPath = input;
            input.clear();
            inputStartsWithRootPath = true;
        }

        QFileInfo fi(fullPath);
        QDir x(fi.absolutePath());
        expectedFiles = x.entryList(model->filter());
        expected = 0;
        if (input.startsWith(".."))
            input.clear();
        for (int ii = 0; ii < expectedFiles.count(); ++ii) {
#if defined(Q_OS_WIN) || defined(Q_OS_SYMBIAN)
            if (expectedFiles.at(ii).startsWith(input,Qt::CaseInsensitive))
#else
            if (expectedFiles.at(ii).startsWith(input))
#endif
                ++expected;
        }
    }

    QTest::qWait(1000);
    if (cModel->rowCount() != expected) {
        for (int i = 0; i < cModel->rowCount(); ++i) {
            QString file = cModel->index(i, 0).data().toString();
            expectedFiles.removeAll(file);
        }
        //qDebug() << expectedFiles;
    }


    // ### FIXME: This will fail on Symbian on some tests and some environments until the file engine and QFileSystemModel
    // are fixed to properly capitalize paths, so that some folders are not duplicated in QFileSystemModel.
#if defined(Q_OS_SYMBIAN)
    QSKIP("This will fail on Symbian on some tests and some environments until the file engine and QFileSystemModel are fixed to properly capitalize paths", SkipAll);
#endif
    QTRY_COMPARE(cModel->rowCount(), expected);
    } QT_CATCH(...) {
        qDeleteAll(files);
        QT_RETHROW;
    }
    qDeleteAll(files);
}

void tst_QFiledialog::completer_up()
{
    QNonNativeFileDialog fd;
    fd.setOptions(QFileDialog::DontUseNativeDialog);
    QSignalSpy spyCurrentChanged(&fd, SIGNAL(currentChanged(const QString &)));
    QSignalSpy spyDirectoryEntered(&fd, SIGNAL(directoryEntered(const QString &)));
    QSignalSpy spyFilesSelected(&fd, SIGNAL(filesSelected(const QStringList &)));
    QSignalSpy spyFilterSelected(&fd, SIGNAL(filterSelected(const QString &)));

    fd.show();
    QLineEdit *lineEdit = qFindChild<QLineEdit*>(&fd, "fileNameEdit");
    QVERIFY(lineEdit);
    QCOMPARE(spyFilesSelected.count(), 0);
    int depth = QDir::currentPath().split('/').count();
    for (int i = 0; i <= depth * 3 + 1; ++i) {
        lineEdit->insert("../");
        qApp->processEvents();
    }
    QCOMPARE(spyCurrentChanged.count(), 0);
    QCOMPARE(spyDirectoryEntered.count(), 0);
    QCOMPARE(spyFilesSelected.count(), 0);
    QCOMPARE(spyFilterSelected.count(), 0);
}

void tst_QFiledialog::acceptMode()
{
    QNonNativeFileDialog fd;
    fd.show();

    QToolButton* newButton = qFindChild<QToolButton*>(&fd, "newFolderButton");
    QVERIFY(newButton);

    // default
    QCOMPARE(fd.acceptMode(), QFileDialog::AcceptOpen);
    QCOMPARE(newButton && newButton->isVisible(), true);

    //fd.setDetailsExpanded(true);
    fd.setAcceptMode(QFileDialog::AcceptSave);
    QCOMPARE(fd.acceptMode(), QFileDialog::AcceptSave);
    QCOMPARE(newButton->isVisible(), true);

    fd.setAcceptMode(QFileDialog::AcceptOpen);
    QCOMPARE(fd.acceptMode(), QFileDialog::AcceptOpen);
    QCOMPARE(newButton->isVisible(), true);
}

void tst_QFiledialog::confirmOverwrite()
{
    QNonNativeFileDialog fd;
    QCOMPARE(fd.confirmOverwrite(), true);
    fd.setConfirmOverwrite(true);
    QCOMPARE(fd.confirmOverwrite(), true);
    fd.setConfirmOverwrite(false);
    QCOMPARE(fd.confirmOverwrite(), false);
    fd.setConfirmOverwrite(true);
    QCOMPARE(fd.confirmOverwrite(), true);
}

void tst_QFiledialog::defaultSuffix()
{
    QNonNativeFileDialog fd;
    QCOMPARE(fd.defaultSuffix(), QString());
    fd.setDefaultSuffix("txt");
    QCOMPARE(fd.defaultSuffix(), QString("txt"));
    fd.setDefaultSuffix(QString());
    QCOMPARE(fd.defaultSuffix(), QString());
}

void tst_QFiledialog::fileMode()
{
    QNonNativeFileDialog fd;
    QCOMPARE(fd.fileMode(), QFileDialog::AnyFile);
    fd.setFileMode(QFileDialog::ExistingFile);
    QCOMPARE(fd.fileMode(), QFileDialog::ExistingFile);
    fd.setFileMode(QFileDialog::Directory);
    QCOMPARE(fd.fileMode(), QFileDialog::Directory);
    fd.setFileMode(QFileDialog::DirectoryOnly);
    QCOMPARE(fd.fileMode(), QFileDialog::DirectoryOnly);
    fd.setFileMode(QFileDialog::ExistingFiles);
    QCOMPARE(fd.fileMode(), QFileDialog::ExistingFiles);
}

void tst_QFiledialog::caption()
{
    QNonNativeFileDialog fd;
    fd.setWindowTitle("testing");
    fd.setFileMode(QFileDialog::Directory);
    QCOMPARE(fd.windowTitle(), QString("testing"));
}

void tst_QFiledialog::filters()
{
    QNonNativeFileDialog fd;
    fd.setOptions(QFileDialog::DontUseNativeDialog);
    QSignalSpy spyCurrentChanged(&fd, SIGNAL(currentChanged(const QString &)));
    QSignalSpy spyDirectoryEntered(&fd, SIGNAL(directoryEntered(const QString &)));
    QSignalSpy spyFilesSelected(&fd, SIGNAL(filesSelected(const QStringList &)));
    QSignalSpy spyFilterSelected(&fd, SIGNAL(filterSelected(const QString &)));
    QCOMPARE(fd.filters(), QStringList("All Files (*)"));

    // effects
    QList<QComboBox*> views = qFindChildren<QComboBox*>(&fd, "fileTypeCombo");
    QVERIFY(views.count() == 1);
    QCOMPARE(views.at(0)->isVisible(), false);

    QStringList filters;
    filters << "Image files (*.png *.xpm *.jpg)"
         << "Text files (*.txt)"
         << "Any files (*.*)";
    fd.setFilters(filters);
    QCOMPARE(views.at(0)->isVisible(), false);
    fd.show();
    fd.setAcceptMode(QFileDialog::AcceptSave);
    QCOMPARE(views.at(0)->isVisible(), true);
    QCOMPARE(fd.filters(), filters);
    fd.setFilter("Image files (*.png *.xpm *.jpg);;Text files (*.txt);;Any files (*.*)");
    QCOMPARE(fd.filters(), filters);
    QCOMPARE(spyCurrentChanged.count(), 0);
    QCOMPARE(spyDirectoryEntered.count(), 0);
    QCOMPARE(spyFilesSelected.count(), 0);
    QCOMPARE(spyFilterSelected.count(), 0);

    // setting shouldn't emit any signals
    for (int i = views.at(0)->currentIndex(); i < views.at(0)->count(); ++i)
        views.at(0)->setCurrentIndex(i);
    QCOMPARE(spyFilterSelected.count(), 0);

    //Let check if filters with whitespaces
    QNonNativeFileDialog fd2;
    QStringList expected;
    expected << "C++ Source Files(*.cpp)";
    expected << "Any(*.*)";
    fd2.setFilter("C++ Source Files(*.cpp);;Any(*.*)");
    QCOMPARE(expected, fd2.filters());
    fd2.setFilter("C++ Source Files(*.cpp) ;;Any(*.*)");
    QCOMPARE(expected, fd2.filters());
    fd2.setFilter("C++ Source Files(*.cpp);; Any(*.*)");
    QCOMPARE(expected, fd2.filters());
    fd2.setFilter(" C++ Source Files(*.cpp);; Any(*.*)");
    QCOMPARE(expected, fd2.filters());
    fd2.setFilter("C++ Source Files(*.cpp) ;; Any(*.*)");
    QCOMPARE(expected, fd2.filters());
}

void tst_QFiledialog::selectFilter()
{
    QNonNativeFileDialog fd;
    QSignalSpy spyFilterSelected(&fd, SIGNAL(filterSelected(const QString &)));
    QCOMPARE(fd.selectedFilter(), QString("All Files (*)"));
    QStringList filters;
    filters << "Image files (*.png *.xpm *.jpg)"
         << "Text files (*.txt)"
         << "Any files (*.*)";
    fd.setFilters(filters);
    QCOMPARE(fd.selectedFilter(), filters.at(0));
    fd.selectFilter(filters.at(1));
    QCOMPARE(fd.selectedFilter(), filters.at(1));
    fd.selectFilter(filters.at(2));
    QCOMPARE(fd.selectedFilter(), filters.at(2));

    fd.selectFilter("bob");
    QCOMPARE(fd.selectedFilter(), filters.at(2));
    fd.selectFilter("");
    QCOMPARE(fd.selectedFilter(), filters.at(2));
    QCOMPARE(spyFilterSelected.count(), 0);
}

void tst_QFiledialog::history()
{
    QNonNativeFileDialog fd;
    fd.setViewMode(QFileDialog::List);
    QFileSystemModel *model = qFindChild<QFileSystemModel*>(&fd, "qt_filesystem_model");
    QVERIFY(model);
    QSignalSpy spyCurrentChanged(&fd, SIGNAL(currentChanged(const QString &)));
    QSignalSpy spyDirectoryEntered(&fd, SIGNAL(directoryEntered(const QString &)));
    QSignalSpy spyFilesSelected(&fd, SIGNAL(filesSelected(const QStringList &)));
    QSignalSpy spyFilterSelected(&fd, SIGNAL(filterSelected(const QString &)));
    QCOMPARE(model->index(fd.history().first()), model->index(QDir::toNativeSeparators(fd.directory().absolutePath())));
    fd.setDirectory(QDir::current().absolutePath());
    QStringList history;
    history << QDir::toNativeSeparators(QDir::current().absolutePath())
            << QDir::toNativeSeparators(QDir::home().absolutePath())
            << QDir::toNativeSeparators(QDir::temp().absolutePath());
    fd.setHistory(history);
    if (fd.history() != history) {
        qDebug() << fd.history() << history;
        // quick and dirty output for windows failure.
        QListView* list = qFindChild<QListView*>(&fd, "listView");
        QVERIFY(list);
        QModelIndex root = list->rootIndex();
        while (root.isValid()) {
            qDebug() << root.data();
            root = root.parent();
        }
    }
    QCOMPARE(fd.history(), history);

    QStringList badHistory;
    badHistory << "junk";
    fd.setHistory(badHistory);
    badHistory << QDir::toNativeSeparators(QDir::current().absolutePath());
    QCOMPARE(fd.history(), badHistory);

    QCOMPARE(spyCurrentChanged.count(), 0);
    QCOMPARE(spyDirectoryEntered.count(), 0);
    QCOMPARE(spyFilesSelected.count(), 0);
    QCOMPARE(spyFilterSelected.count(), 0);
}

void tst_QFiledialog::iconProvider()
{
    QNonNativeFileDialog *fd = new QNonNativeFileDialog();
    QVERIFY(fd->iconProvider() != 0);
    QFileIconProvider *ip = new QFileIconProvider();
    fd->setIconProvider(ip);
    QCOMPARE(fd->iconProvider(), ip);
    delete fd;
    delete ip;
}

void tst_QFiledialog::isReadOnly()
{
    QNonNativeFileDialog fd;

    QPushButton* newButton = qFindChild<QPushButton*>(&fd, "newFolderButton");
    QAction* renameAction = qFindChild<QAction*>(&fd, "qt_rename_action");
    QAction* deleteAction = qFindChild<QAction*>(&fd, "qt_delete_action");

    QCOMPARE(fd.isReadOnly(), false);

    // This is dependent upon the file/dir, find cross platform way to test
    //fd.setDirectory(QDir::home());
    //QCOMPARE(newButton && newButton->isEnabled(), true);
    //QCOMPARE(renameAction && renameAction->isEnabled(), true);
    //QCOMPARE(deleteAction && deleteAction->isEnabled(), true);

    fd.setReadOnly(true);
    QCOMPARE(fd.isReadOnly(), true);

    QCOMPARE(newButton && newButton->isEnabled(), false);
    QCOMPARE(renameAction && renameAction->isEnabled(), false);
    QCOMPARE(deleteAction && deleteAction->isEnabled(), false);
}

void tst_QFiledialog::itemDelegate()
{
    QNonNativeFileDialog fd;
    QVERIFY(fd.itemDelegate() != 0);
    QItemDelegate *id = new QItemDelegate(&fd);
    fd.setItemDelegate(id);
    QCOMPARE(fd.itemDelegate(), (QAbstractItemDelegate *)id);
}

void tst_QFiledialog::labelText()
{
    QNonNativeFileDialog fd;
    QDialogButtonBox buttonBox;
    QPushButton *cancelButton = buttonBox.addButton(QDialogButtonBox::Cancel);
    QCOMPARE(fd.labelText(QFileDialog::LookIn), QString("Look in:"));
    QCOMPARE(fd.labelText(QFileDialog::FileName), QString("File &name:"));
    QCOMPARE(fd.labelText(QFileDialog::FileType), QString("Files of type:"));
    QCOMPARE(fd.labelText(QFileDialog::Accept), QString("&Open")); ///### see task 241462
    QCOMPARE(fd.labelText(QFileDialog::Reject), cancelButton->text());

    fd.setLabelText(QFileDialog::LookIn, "1");
    QCOMPARE(fd.labelText(QFileDialog::LookIn), QString("1"));
    fd.setLabelText(QFileDialog::FileName, "2");
    QCOMPARE(fd.labelText(QFileDialog::FileName), QString("2"));
    fd.setLabelText(QFileDialog::FileType, "3");
    QCOMPARE(fd.labelText(QFileDialog::FileType), QString("3"));
    fd.setLabelText(QFileDialog::Accept, "4");
    QCOMPARE(fd.labelText(QFileDialog::Accept), QString("4"));
    fd.setLabelText(QFileDialog::Reject, "5");
    QCOMPARE(fd.labelText(QFileDialog::Reject), QString("5"));
}

void tst_QFiledialog::resolveSymlinks()
{
    QNonNativeFileDialog fd;

    // default
    QCOMPARE(fd.resolveSymlinks(), true);
    fd.setResolveSymlinks(false);
    QCOMPARE(fd.resolveSymlinks(), false);
    fd.setResolveSymlinks(true);
    QCOMPARE(fd.resolveSymlinks(), true);

    // the file dialog doesn't do anything based upon this, just passes it to the model
    // the model should fully test it, don't test it here
}

void tst_QFiledialog::selectFile_data()
{
    QTest::addColumn<QString>("file");
    QTest::addColumn<int>("count");
    QTest::newRow("null") << QString() << 1;
    QTest::newRow("file") << "foo" << 1;
    QTest::newRow("tmp") << "temp" << 1;
}

void tst_QFiledialog::selectFile()
{
    QFETCH(QString, file);
    QFETCH(int, count);
    QNonNativeFileDialog fd;
    QFileSystemModel *model = qFindChild<QFileSystemModel*>(&fd, "qt_filesystem_model");
    QVERIFY(model);
    fd.setDirectory(QDir::currentPath());
    // default value
    QCOMPARE(fd.selectedFiles().count(), 1);

    QTemporaryFile tempFile(QDir::tempPath() + "/aXXXXXX");
    bool inTemp = (file == "temp");
    if (inTemp) {
        tempFile.open();
        file = tempFile.fileName();
    }

    fd.selectFile(file);
    QCOMPARE(fd.selectedFiles().count(), count);
    if (inTemp) {
        QCOMPARE(model->index(fd.directory().path()), model->index(QDir::tempPath()));
    } else {
        QCOMPARE(model->index(fd.directory().path()), model->index(QDir::currentPath()));
    }
}

void tst_QFiledialog::selectFiles()
{
    QNonNativeFileDialog fd;
    fd.setViewMode(QFileDialog::List);
    QString tempPath = QDir::tempPath() + '/' + "QFileDialogTestDir4SelectFiles";
    QDir dir;
    QVERIFY(dir.mkpath(tempPath));
    fd.setDirectory(tempPath);
    QSignalSpy spyCurrentChanged(&fd, SIGNAL(currentChanged(const QString &)));
    QSignalSpy spyDirectoryEntered(&fd, SIGNAL(directoryEntered(const QString &)));
    QSignalSpy spyFilesSelected(&fd, SIGNAL(filesSelected(const QStringList &)));
    QSignalSpy spyFilterSelected(&fd, SIGNAL(filterSelected(const QString &)));
    fd.show();
    fd.setFileMode(QFileDialog::ExistingFiles);

    QString filesPath = fd.directory().absolutePath();
    for (int i=0; i < 5; ++i) {
        QFile file(filesPath + QString::fromLatin1("/qfiledialog_auto_test_not_pres_%1").arg(i));
        file.open(QIODevice::WriteOnly);
        file.resize(1024);
        file.flush();
        file.close();
    }

    // Get a list of files in the view and then get the corresponding index's
    QStringList list = fd.directory().entryList(QDir::Files);
    QModelIndexList toSelect;
    QVERIFY(list.count() > 1);
    QListView* listView = qFindChild<QListView*>(&fd, "listView");
    QVERIFY(listView);
    for (int i = 0; i < list.count(); ++i) {
        fd.selectFile(fd.directory().path() + "/" + list.at(i));
#if defined(QT_MAC_USE_COCOA) || defined(Q_WS_WIN) || defined(Q_OS_SYMBIAN)
    QEXPECT_FAIL("", "This test does not work on Mac, Windows, or Symbian", Abort);
#endif
        QTRY_VERIFY(!listView->selectionModel()->selectedRows().isEmpty());
        toSelect.append(listView->selectionModel()->selectedRows().last());
    }
    QCOMPARE(spyFilesSelected.count(), 0);

    listView->selectionModel()->clear();
    QCOMPARE(spyFilesSelected.count(), 0);

    // select the indexes
    for (int i = 0; i < toSelect.count(); ++i) {
        listView->selectionModel()->select(toSelect.at(i),
                QItemSelectionModel::Select | QItemSelectionModel::Rows);
    }
    QCOMPARE(fd.selectedFiles().count(), toSelect.count());
    QCOMPARE(spyCurrentChanged.count(), 0);
    QCOMPARE(spyDirectoryEntered.count(), 0);
    QCOMPARE(spyFilesSelected.count(), 0);
    QCOMPARE(spyFilterSelected.count(), 0);
    for (int i=0; i < 5; ++i)
        QFile::remove(filesPath + QString::fromLatin1("/qfiledialog_auto_test_not_pres_%1").arg(i));

    //If the selection is invalid then we fill the line edit but without the /
    QNonNativeFileDialog * dialog = new QNonNativeFileDialog( 0, "Save" );
    dialog->setFileMode( QFileDialog::AnyFile );
    dialog->setAcceptMode( QFileDialog::AcceptSave );
    QString temporary = QDir::tempPath() + QLatin1String("/blah");
    dialog->selectFile(temporary);
    dialog->show();
    QTest::qWait(500);
    QLineEdit *lineEdit = qFindChild<QLineEdit*>(dialog, "fileNameEdit");
    QVERIFY(lineEdit);
    QCOMPARE(lineEdit->text(),QLatin1String("blah"));
    delete dialog;
}

void tst_QFiledialog::viewMode()
{
    QNonNativeFileDialog fd;
    fd.setViewMode(QFileDialog::List);
    fd.show();

    // find widgets
    QList<QTreeView*> treeView = qFindChildren<QTreeView*>(&fd, "treeView");
    QCOMPARE(treeView.count(), 1);
    QList<QListView*> listView = qFindChildren<QListView*>(&fd, "listView");
    QCOMPARE(listView.count(), 1);
    QList<QToolButton*> listButton = qFindChildren<QToolButton*>(&fd, "listModeButton");
    QCOMPARE(listButton.count(), 1);
    QList<QToolButton*> treeButton = qFindChildren<QToolButton*>(&fd, "detailModeButton");
    QCOMPARE(treeButton.count(), 1);

    // default value
    QCOMPARE(fd.viewMode(), QFileDialog::List);

    // detail
    fd.setViewMode(QFileDialog::ViewMode(QFileDialog::Detail));

    QCOMPARE(QFileDialog::ViewMode(QFileDialog::Detail), fd.viewMode());
    QCOMPARE(listView.at(0)->isVisible(), false);
    QCOMPARE(listButton.at(0)->isDown(), false);
    QCOMPARE(treeView.at(0)->isVisible(), true);
    QCOMPARE(treeButton.at(0)->isDown(), true);

    // list
    fd.setViewMode(QFileDialog::ViewMode(QFileDialog::List));

    QCOMPARE(QFileDialog::ViewMode(QFileDialog::List), fd.viewMode());
    QCOMPARE(treeView.at(0)->isVisible(), false);
    QCOMPARE(treeButton.at(0)->isDown(), false);
    QCOMPARE(listView.at(0)->isVisible(), true);
    QCOMPARE(listButton.at(0)->isDown(), true);
}

void tst_QFiledialog::proxymodel()
{
    QNonNativeFileDialog fd;
    QCOMPARE(fd.proxyModel(), (QAbstractProxyModel*)0);

    fd.setProxyModel(0);
    QCOMPARE(fd.proxyModel(), (QAbstractProxyModel*)0);

    QSortFilterProxyModel *proxyModel = new QSortFilterProxyModel(&fd);
    fd.setProxyModel(proxyModel);
    QCOMPARE(fd.proxyModel(), (QAbstractProxyModel *)proxyModel);

    fd.setProxyModel(0);
    QCOMPARE(fd.proxyModel(), (QAbstractProxyModel*)0);
}

void tst_QFiledialog::setNameFilter()
{
    QNonNativeFileDialog fd;
    fd.setFilter(QString());
    fd.setFilters(QStringList());
}

void tst_QFiledialog::focus()
{
    QNonNativeFileDialog fd;
    fd.setDirectory(QDir::currentPath());
    fd.show();
    QApplication::setActiveWindow(&fd);
    QTest::qWaitForWindowShown(&fd);
    QTRY_COMPARE(fd.isVisible(), true);
    QTRY_COMPARE(QApplication::activeWindow(), static_cast<QWidget*>(&fd));
    qApp->processEvents();

    // make sure the tests work with focus follows mouse
    QCursor::setPos(fd.geometry().center());
    QApplication::syncX();

    QList<QWidget*> treeView = qFindChildren<QWidget*>(&fd, "fileNameEdit");
    QCOMPARE(treeView.count(), 1);
    QVERIFY(treeView.at(0));
    QTRY_COMPARE(treeView.at(0)->hasFocus(), true);
    QCOMPARE(treeView.at(0)->hasFocus(), true);
}


void tst_QFiledialog::historyBack()
{
    QNonNativeFileDialog fd;
    QFileSystemModel *model = qFindChild<QFileSystemModel*>(&fd, "qt_filesystem_model");
    QVERIFY(model);
    QToolButton *backButton = qFindChild<QToolButton*>(&fd, "backButton");
    QVERIFY(backButton);
    QToolButton *forwardButton = qFindChild<QToolButton*>(&fd, "forwardButton");
    QVERIFY(forwardButton);

    QSignalSpy spy(model, SIGNAL(rootPathChanged(const QString &)));

    QString home = fd.directory().absolutePath();
    QString desktop = QDir::homePath();
    QString temp = QDir::tempPath();

    QCOMPARE(backButton->isEnabled(), false);
    QCOMPARE(forwardButton->isEnabled(), false);
    fd.setDirectory(temp);
    qApp->processEvents();
    QCOMPARE(backButton->isEnabled(), true);
    QCOMPARE(forwardButton->isEnabled(), false);
    fd.setDirectory(desktop);
    QCOMPARE(spy.count(), 2);

    backButton->click();
    qApp->processEvents();
    QCOMPARE(backButton->isEnabled(), true);
    QCOMPARE(forwardButton->isEnabled(), true);
    QCOMPARE(spy.count(), 3);
    QString currentPath = qVariantValue<QString>(spy.last().first());
    QCOMPARE(model->index(currentPath), model->index(temp));

    backButton->click();
    currentPath = qVariantValue<QString>(spy.last().first());
    QCOMPARE(currentPath, home);
    QCOMPARE(backButton->isEnabled(), false);
    QCOMPARE(forwardButton->isEnabled(), true);
    QCOMPARE(spy.count(), 4);

    // nothing should change at this point
    backButton->click();
    QCOMPARE(spy.count(), 4);
    QCOMPARE(backButton->isEnabled(), false);
    QCOMPARE(forwardButton->isEnabled(), true);
}

void tst_QFiledialog::historyForward()
{
    QNonNativeFileDialog fd;
    fd.setDirectory(QDir::currentPath());
    QToolButton *backButton = qFindChild<QToolButton*>(&fd, "backButton");
    QVERIFY(backButton);
    QToolButton *forwardButton = qFindChild<QToolButton*>(&fd, "forwardButton");
    QVERIFY(forwardButton);

    QFileSystemModel *model = qFindChild<QFileSystemModel*>(&fd, "qt_filesystem_model");
    QVERIFY(model);
    QSignalSpy spy(model, SIGNAL(rootPathChanged(const QString &)));

    QString home = fd.directory().absolutePath();
    QString desktop = QDir::homePath();
    QString temp = QDir::tempPath();

    fd.setDirectory(home);
    fd.setDirectory(temp);
    fd.setDirectory(desktop);

    backButton->click();
    QCOMPARE(forwardButton->isEnabled(), true);
    QCOMPARE(model->index(qVariantValue<QString>(spy.last().first())), model->index(temp));

    forwardButton->click();
    QCOMPARE(model->index(qVariantValue<QString>(spy.last().first())), model->index(desktop));
    QCOMPARE(backButton->isEnabled(), true);
    QCOMPARE(forwardButton->isEnabled(), false);
    QCOMPARE(spy.count(), 4);

    backButton->click();
    QCOMPARE(model->index(qVariantValue<QString>(spy.last().first())), model->index(temp));
    QCOMPARE(backButton->isEnabled(), true);

    backButton->click();
    QCOMPARE(model->index(qVariantValue<QString>(spy.last().first())), model->index(home));
    QCOMPARE(backButton->isEnabled(), false);
    QCOMPARE(forwardButton->isEnabled(), true);
    QCOMPARE(spy.count(), 6);

    forwardButton->click();
    QCOMPARE(model->index(qVariantValue<QString>(spy.last().first())), model->index(temp));
    backButton->click();
    QCOMPARE(model->index(qVariantValue<QString>(spy.last().first())), model->index(home));
    QCOMPARE(spy.count(), 8);

    forwardButton->click();
    QCOMPARE(model->index(qVariantValue<QString>(spy.last().first())), model->index(temp));
    forwardButton->click();
    QCOMPARE(model->index(qVariantValue<QString>(spy.last().first())), model->index(desktop));

    backButton->click();
    QCOMPARE(model->index(qVariantValue<QString>(spy.last().first())), model->index(temp));
    backButton->click();
    QCOMPARE(model->index(qVariantValue<QString>(spy.last().first())), model->index(home));
    fd.setDirectory(desktop);
    QCOMPARE(forwardButton->isEnabled(), false);
}

void tst_QFiledialog::disableSaveButton_data()
{
    QTest::addColumn<QString>("path");
    QTest::addColumn<bool>("isEnabled");

    QTest::newRow("valid path") << QDir::temp().absolutePath() + QDir::separator() + "qfiledialog.new_file" << true;
    QTest::newRow("no path") << "" << false;
#if defined(Q_OS_UNIX) && !defined(Q_OS_MAC) && !defined(Q_OS_OPENBSD)
    QTest::newRow("too long path") << "iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii" << false;
#endif
    QTest::newRow("file") << "foo.html" << true;
}

void tst_QFiledialog::disableSaveButton()
{
    QFETCH(QString, path);
    QFETCH(bool, isEnabled);

    QNonNativeFileDialog fd(0, "caption", path);
    fd.setAcceptMode(QFileDialog::AcceptSave);
    QDialogButtonBox *buttonBox = qFindChild<QDialogButtonBox*>(&fd, "buttonBox");
    QPushButton *button = buttonBox->button(QDialogButtonBox::Save);
    QVERIFY(button);
    QCOMPARE(button->isEnabled(), isEnabled);
}

void tst_QFiledialog::saveButtonText_data()
{
    QTest::addColumn<QString>("path");
    QTest::addColumn<QString>("label");
    QTest::addColumn<QString>("caption");

    QTest::newRow("empty path") << "" << QString() << QFileDialog::tr("&Save");
    QTest::newRow("file path") << "qfiledialog.new_file" << QString() << QFileDialog::tr("&Save");
    QTest::newRow("dir") << QDir::temp().absolutePath() << QString() << QFileDialog::tr("&Open");
    QTest::newRow("setTextLabel") << "qfiledialog.new_file" << "Mooo" << "Mooo";
    QTest::newRow("dir & label") << QDir::temp().absolutePath() << "Poo" << QFileDialog::tr("&Open");
}

void tst_QFiledialog::saveButtonText()
{
    QFETCH(QString, path);
    QFETCH(QString, label);
    QFETCH(QString, caption);

    QNonNativeFileDialog fd(0, "auto test", QDir::temp().absolutePath());
    fd.setAcceptMode(QFileDialog::AcceptSave);
    if (!label.isNull())
        fd.setLabelText(QFileDialog::Accept, label);
    fd.setDirectory(QDir::temp());
    fd.selectFile(path);
    QDialogButtonBox *buttonBox = qFindChild<QDialogButtonBox*>(&fd, "buttonBox");
    QVERIFY(buttonBox);
    QPushButton *button = buttonBox->button(QDialogButtonBox::Save);
    QVERIFY(button);
    QCOMPARE(button->text(), caption);
}

void tst_QFiledialog::clearLineEdit()
{
    QNonNativeFileDialog fd(0, "caption", "foo");
    fd.setViewMode(QFileDialog::List);
    fd.setFileMode(QFileDialog::AnyFile);
    fd.setOptions(QFileDialog::DontUseNativeDialog);
    fd.show();

    //play it really safe by creating a directory
    QDir::home().mkdir("_____aaaaaaaaaaaaaaaaaaaaaa");

    QLineEdit *lineEdit = qFindChild<QLineEdit*>(&fd, "fileNameEdit");
    QVERIFY(lineEdit);
    QVERIFY(lineEdit->text() == "foo");
    fd.setDirectory(QDir::home());

    QListView* list = qFindChild<QListView*>(&fd, "listView");
    QVERIFY(list);

    // saving a file the text shouldn't be cleared
    fd.setDirectory(QDir::home());

    QTest::qWait(1000);
#ifdef QT_KEYPAD_NAVIGATION
    list->setEditFocus(true);
#endif
    QTest::keyClick(list, Qt::Key_Down);
#ifndef Q_WS_MAC
    QTest::keyClick(list, Qt::Key_Return);
#else
    QTest::keyClick(list, Qt::Key_O, Qt::ControlModifier);
#endif

    QTest::qWait(2000);
    QVERIFY(fd.directory().absolutePath() != QDir::home().absolutePath());
    QVERIFY(!lineEdit->text().isEmpty());

    // selecting a dir the text should be cleared so one can just hit ok
    // and it selects that directory
    fd.setFileMode(QNonNativeFileDialog::Directory);
    fd.setDirectory(QDir::home());

    QTest::qWait(1000);
    QTest::keyClick(list, Qt::Key_Down);
#ifndef Q_WS_MAC
    QTest::keyClick(list, Qt::Key_Return);
#else
    QTest::keyClick(list, Qt::Key_O, Qt::ControlModifier);
#endif

    QTest::qWait(2000);
    QVERIFY(fd.directory().absolutePath() != QDir::home().absolutePath());
    QVERIFY(lineEdit->text().isEmpty());

    //remove the dir
    QDir::home().rmdir("_____aaaaaaaaaaaaaaaaaaaaaa");
}

void tst_QFiledialog::enableChooseButton()
{
    QNonNativeFileDialog fd;
    fd.setFileMode(QFileDialog::Directory);
    fd.show();
    QDialogButtonBox *buttonBox = qFindChild<QDialogButtonBox*>(&fd, "buttonBox");
    QPushButton *button = buttonBox->button(QDialogButtonBox::Open);
    QVERIFY(button);
    QCOMPARE(button->isEnabled(), true);
}

QT_BEGIN_NAMESPACE
typedef QString (*_qt_filedialog_existing_directory_hook)(QWidget *parent, const QString &caption, const QString &dir, QFileDialog::Options options);
extern Q_GUI_EXPORT _qt_filedialog_existing_directory_hook qt_filedialog_existing_directory_hook;
QT_END_NAMESPACE
QString existing(QWidget *, const QString &, const QString &, QFileDialog::Options) {
    return "dir";
}

QT_BEGIN_NAMESPACE
typedef QString (*_qt_filedialog_open_filename_hook)(QWidget * parent, const QString &caption, const QString &dir, const QString &filter, QString *selectedFilter, QFileDialog::Options options);
extern Q_GUI_EXPORT _qt_filedialog_open_filename_hook qt_filedialog_open_filename_hook;
QT_END_NAMESPACE
QString openName(QWidget *, const QString &, const QString &, const QString &, QString *, QFileDialog::Options) {
    return "openName";
}

QT_BEGIN_NAMESPACE
typedef QStringList (*_qt_filedialog_open_filenames_hook)(QWidget * parent, const QString &caption, const QString &dir, const QString &filter, QString *selectedFilter, QFileDialog::Options options);
extern Q_GUI_EXPORT _qt_filedialog_open_filenames_hook qt_filedialog_open_filenames_hook;
QT_END_NAMESPACE
QStringList openNames(QWidget *, const QString &, const QString &, const QString &, QString *, QFileDialog::Options) {
    return QStringList("openNames");
}

QT_BEGIN_NAMESPACE
typedef QString (*_qt_filedialog_save_filename_hook)(QWidget * parent, const QString &caption, const QString &dir, const QString &filter, QString *selectedFilter, QFileDialog::Options options);
extern Q_GUI_EXPORT _qt_filedialog_save_filename_hook qt_filedialog_save_filename_hook;
QT_END_NAMESPACE
QString saveName(QWidget *, const QString &, const QString &, const QString &, QString *, QFileDialog::Options) {
    return "saveName";
}


void tst_QFiledialog::hooks()
{
    qt_filedialog_existing_directory_hook = &existing;
    qt_filedialog_save_filename_hook = &saveName;
    qt_filedialog_open_filename_hook = &openName;
    qt_filedialog_open_filenames_hook = &openNames;

    QCOMPARE(QFileDialog::getExistingDirectory(), QString("dir"));
    QCOMPARE(QFileDialog::getOpenFileName(), QString("openName"));
    QCOMPARE(QFileDialog::getOpenFileNames(), QStringList("openNames"));
    QCOMPARE(QFileDialog::getSaveFileName(), QString("saveName"));
}

QTEST_MAIN(tst_QFiledialog)
#include "tst_qfiledialog.moc"
