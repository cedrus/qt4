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
#include "../../../src/gui/dialogs/qfilesystemmodel_p.h"
#include <QFileIconProvider>
#include <QTreeView>
#include <QHeaderView>
#include "../../shared/util.h"
#include <QTime>
#include <QStyle>
#include <QtGlobal>
#if defined(Q_OS_SYMBIAN)
# include <f32file.h>
#endif
//TESTED_CLASS=
//TESTED_FILES=

#define WAITTIME 1000

// Will try to wait for the condition while allowing event processing
// for a maximum of 5 seconds.
#define TRY_WAIT(expr) \
    do { \
        const int step = 50; \
        for (int __i = 0; __i < 5000 && !(expr); __i+=step) { \
            QTest::qWait(step); \
        } \
    } while(0)

#if defined(Q_OS_SYMBIAN)
static HBufC* qt_QString2HBufCNewL(const QString& aString)
{
    HBufC *buffer;
#ifdef QT_NO_UNICODE
    TPtrC8 ptr(reinterpret_cast<const TUint8*>(aString.toLocal8Bit().constData()));
    buffer = HBufC8::NewL(ptr.Length());
    buffer->Des().Copy(ptr);
#else
    TPtrC16 ptr(reinterpret_cast<const TUint16*>(aString.utf16()));
    buffer = HBufC16::NewL(ptr.Length());
    buffer->Des().Copy(ptr);
#endif
    return buffer;
}
#endif

class tst_QFileSystemModel : public QObject {
  Q_OBJECT

public:
    tst_QFileSystemModel();
    virtual ~tst_QFileSystemModel();

public Q_SLOTS:
    void init();
    void cleanup();

private slots:
    void indexPath();

    void rootPath();
    void naturalCompare_data();
    void naturalCompare();
    void readOnly();
    void iconProvider();

    void rowCount();

    void rowsInserted_data();
    void rowsInserted();

    void rowsRemoved_data();
    void rowsRemoved();

    void dataChanged_data();
    void dataChanged();

    void filters_data();
    void filters();

    void nameFilters();

    void setData_data();
    void setData();

    void sort_data();
    void sort();

    void mkdir();

    void caseSensitivity();

#if defined(Q_OS_WIN) && !defined(Q_OS_WINCE)
    void Win32LongFileName();
#endif

    void drives_data();
    void drives();
    void dirsBeforeFiles();

    void roleNames_data();
    void roleNames();

protected:
    bool createFiles(const QString &test_path, const QStringList &initial_files, int existingFileCount = 0, const QStringList &intial_dirs = QStringList(), const QString &baseDir = QDir::temp().absolutePath());

private:
    QFileSystemModel *model;
    QString flatDirTestPath;
#if defined(Q_OS_SYMBIAN)
    RFs rfs;
#endif
};

tst_QFileSystemModel::tst_QFileSystemModel() : model(0)
{
    qRegisterMetaType<QModelIndex>("QModelIndex");

    QTime midnight(0, 0, 0);
    qsrand(midnight.secsTo(QTime::currentTime()));
    // generating unique temporary directory name
    flatDirTestPath = QDir::temp().path() + '/' + QString("flatdirtest.") + QString::number(qrand());
#if defined(Q_OS_SYMBIAN)
    rfs.Connect();
#endif
}

tst_QFileSystemModel::~tst_QFileSystemModel()
{
#if defined(Q_OS_SYMBIAN)
    rfs.Close();
#endif
    QString tmp = flatDirTestPath;
    QDir dir(tmp);
    if (dir.exists() && !dir.rmdir(tmp))
        qWarning("failed to remove tmp dir %s", dir.dirName().toAscii().data());
}

void tst_QFileSystemModel::init()
{
    cleanup();
    QCOMPARE(model, (QFileSystemModel*)0);
    model = new QFileSystemModel;
}

void tst_QFileSystemModel::cleanup()
{
    delete model;
    model = 0;
    QString tmp = flatDirTestPath;
    QDir dir(tmp);
    if (dir.exists(tmp)) {
        QStringList list = dir.entryList(QDir::AllEntries | QDir::System | QDir::Hidden | QDir::NoDotAndDotDot);
        for (int i = 0; i < list.count(); ++i) {
            QFileInfo fi(dir.path() + '/' + list.at(i));
            if (fi.exists() && fi.isFile()) {
		        QFile p(fi.absoluteFilePath());
                p.setPermissions(QFile::ReadUser | QFile::ReadOwner | QFile::ExeOwner | QFile::ExeUser | QFile::WriteUser | QFile::WriteOwner | QFile::WriteOther);
		        QFile dead(dir.path() + '/' + list.at(i));
		        dead.remove();
	        }
	        if (fi.exists() && fi.isDir())
                QVERIFY(dir.rmdir(list.at(i)));
        }
        list = dir.entryList(QDir::AllEntries | QDir::System | QDir::Hidden | QDir::NoDotAndDotDot);
        QVERIFY(list.count() == 0);
    }
}

void tst_QFileSystemModel::indexPath()
{
#if !defined(Q_OS_WIN) && !defined(Q_OS_SYMBIAN)
    int depth = QDir::currentPath().count('/');
    model->setRootPath(QDir::currentPath());
    QTest::qWait(WAITTIME);
    QString backPath;
    for (int i = 0; i <= depth * 2 + 1; ++i) {
        backPath += "../";
        QModelIndex idx = model->index(backPath);
        QVERIFY(i != depth - 1 ? idx.isValid() : !idx.isValid());
    }
    QTest::qWait(WAITTIME * 3);
    qApp->processEvents();
#endif
}

void tst_QFileSystemModel::rootPath()
{
    QCOMPARE(model->rootPath(), QString(QDir().path()));

    QSignalSpy rootChanged(model, SIGNAL(rootPathChanged(const QString &)));
    QModelIndex root = model->setRootPath(model->rootPath());
    root = model->setRootPath("this directory shouldn't exist");
    QCOMPARE(rootChanged.count(), 0);

    QString oldRootPath = model->rootPath();
    root = model->setRootPath(QDir::homePath());

    QTRY_VERIFY(model->rowCount(root) >= 0);
    QCOMPARE(model->rootPath(), QString(QDir::homePath()));
    QCOMPARE(rootChanged.count(), oldRootPath == model->rootPath() ? 0 : 1);
    QCOMPARE(model->rootDirectory().absolutePath(), QDir::homePath());

    model->setRootPath(QDir::rootPath());
    int oldCount = rootChanged.count();
    oldRootPath = model->rootPath();
    root = model->setRootPath(QDir::homePath() + QLatin1String("/."));
    QTRY_VERIFY(model->rowCount(root) >= 0);
    QCOMPARE(model->rootPath(), QDir::homePath());
    QCOMPARE(rootChanged.count(), oldRootPath == model->rootPath() ? oldCount : oldCount + 1);
    QCOMPARE(model->rootDirectory().absolutePath(), QDir::homePath());

    QDir newdir = QDir::home();
    if (newdir.cdUp()) {
        oldCount = rootChanged.count();
        oldRootPath = model->rootPath();
        root = model->setRootPath(QDir::homePath() + QLatin1String("/.."));
        QTRY_VERIFY(model->rowCount(root) >= 0);
        QCOMPARE(model->rootPath(), newdir.path());
        QCOMPARE(rootChanged.count(), oldCount + 1);
        QCOMPARE(model->rootDirectory().absolutePath(), newdir.path());
    }
}

void tst_QFileSystemModel::naturalCompare_data()
{
    QTest::addColumn<QString>("s1");
    QTest::addColumn<QString>("s2");
    QTest::addColumn<int>("caseSensitive");
    QTest::addColumn<int>("result");
    QTest::addColumn<int>("swap");
    for (int j = 0; j < 4; ++j) { // <- set a prefix and a postfix string (not numbers)
        QString prefix = (j == 0 || j == 1) ? "b" : "";
        QString postfix = (j == 1 || j == 2) ? "y" : "";

        for (int k = 0; k < 3; ++k) { // <- make 0 not a special case
            QString num = QString("%1").arg(k);
            QString nump = QString("%1").arg(k + 1);
            for (int i = 10; i < 12; ++i) { // <- swap s1 and s2 and reverse the result
                QTest::newRow("basic") << prefix + "0" + postfix << prefix + "0" + postfix << int(Qt::CaseInsensitive) << 0;

                // s1 should always be less then s2
                QTest::newRow("just text")    << prefix + "fred" + postfix     << prefix + "jane" + postfix << int(Qt::CaseInsensitive) << i;
                QTest::newRow("just numbers") << prefix + num + postfix        << prefix + "9" + postfix << int(Qt::CaseInsensitive) << i;
                QTest::newRow("zero")         << prefix + num + postfix        << prefix + "0" + nump + postfix << int(Qt::CaseInsensitive) << i;
                QTest::newRow("space b")      << prefix + num + postfix        << prefix + " " + nump + postfix << int(Qt::CaseInsensitive) << i;
                QTest::newRow("space a")      << prefix + num + postfix        << prefix + nump + " " + postfix << int(Qt::CaseInsensitive) << i;
                QTest::newRow("tab b")        << prefix + num + postfix        << prefix + "    " + nump + postfix << int(Qt::CaseInsensitive) << i;
                QTest::newRow("tab a")        << prefix + num + postfix        << prefix + nump + "   " + postfix << int(Qt::CaseInsensitive) << i;
                QTest::newRow("10 vs 2")      << prefix + num + postfix        << prefix + "10" + postfix << int(Qt::CaseInsensitive) << i;
                QTest::newRow("diff len")     << prefix + num + postfix        << prefix + nump + postfix + "x" << int(Qt::CaseInsensitive) << i;
                QTest::newRow("01 before 1")  << prefix + "0" + num + postfix  << prefix + nump + postfix << int(Qt::CaseInsensitive) << i;
                QTest::newRow("mul nums 2nd") << prefix + "1-" + num + postfix << prefix + "1-" + nump + postfix << int(Qt::CaseInsensitive) << i;
                QTest::newRow("mul nums 2nd") << prefix + "10-" + num + postfix<< prefix + "10-10" + postfix << int(Qt::CaseInsensitive) << i;
                QTest::newRow("mul nums 2nd") << prefix + "10-0"+ num + postfix<< prefix + "10-10" + postfix << int(Qt::CaseInsensitive) << i;
                QTest::newRow("mul nums 2nd") << prefix + "10-" + num + postfix<< prefix + "10-010" + postfix << int(Qt::CaseInsensitive) << i;
                QTest::newRow("mul nums big") << prefix + "10-" + num + postfix<< prefix + "20-0" + postfix << int(Qt::CaseInsensitive) << i;
                QTest::newRow("mul nums big") << prefix + "2-" + num + postfix << prefix + "10-0" + postfix << int(Qt::CaseInsensitive) << i;
                QTest::newRow("mul alphabet") << prefix + num + "-a" + postfix << prefix + num + "-c" + postfix << int(Qt::CaseInsensitive) << i;
                QTest::newRow("mul alphabet2")<< prefix + num + "-a9" + postfix<< prefix + num + "-c0" + postfix << int(Qt::CaseInsensitive) << i;
                QTest::newRow("mul nums w\\0")<< prefix + num + "-"+ num + postfix<< prefix + num+"-0"+nump + postfix << int(Qt::CaseInsensitive) << i;
                QTest::newRow("num first")    << prefix + num + postfix  << prefix + "a" + postfix << int(Qt::CaseInsensitive) << i;
            }
        }
    }
}

void tst_QFileSystemModel::naturalCompare()
{
#ifdef QT_BUILD_INTERNAL
    QFETCH(QString, s1);
    QFETCH(QString, s2);
    QFETCH(int, caseSensitive);
    QFETCH(int, result);

    if (result == 10)
        QCOMPARE(QFileSystemModelPrivate::naturalCompare(s1, s2, Qt::CaseSensitivity(caseSensitive)), -1);
    else
        if (result == 11)
            QCOMPARE(QFileSystemModelPrivate::naturalCompare(s2, s1, Qt::CaseSensitivity(caseSensitive)), 1);
    else
        QCOMPARE(QFileSystemModelPrivate::naturalCompare(s2, s1, Qt::CaseSensitivity(caseSensitive)), result);
#if defined(Q_OS_WINCE)
    // On Windows CE we need to wait after each test, otherwise no new threads can be
    // created. The scheduler takes its time to recognize ended threads.
    QTest::qWait(300);
#endif
#endif
}

void tst_QFileSystemModel::readOnly()
{
    QCOMPARE(model->isReadOnly(), true);
    QTemporaryFile file;
    file.open();
    QModelIndex root = model->setRootPath(QDir::tempPath());

    QTRY_VERIFY(model->rowCount(root) > 0);
    QVERIFY(!(model->flags(model->index(file.fileName())) & Qt::ItemIsEditable));
    model->setReadOnly(false);
    QCOMPARE(model->isReadOnly(), false);
    QVERIFY(model->flags(model->index(file.fileName())) & Qt::ItemIsEditable);
}

class CustomFileIconProvider : public QFileIconProvider
{
public:
    CustomFileIconProvider() : QFileIconProvider() {
        mb = qApp->style()->standardIcon(QStyle::SP_MessageBoxCritical);
        dvd = qApp->style()->standardIcon(QStyle::SP_DriveDVDIcon);
    }

    virtual QIcon icon(const QFileInfo &info) const
    {
        if (info.isDir())
            return mb;

        return QFileIconProvider::icon(info);
    }
    virtual QIcon icon(IconType type) const
    {
        if (type == QFileIconProvider::Folder)
            return dvd;

        return QFileIconProvider::icon(type);
    }
private:
    QIcon mb;
    QIcon dvd;
};

void tst_QFileSystemModel::iconProvider()
{
    QVERIFY(model->iconProvider());
    QFileIconProvider *p = new QFileIconProvider();
    model->setIconProvider(p);
    QCOMPARE(model->iconProvider(), p);
    model->setIconProvider(0);
    delete p;

    QFileSystemModel *myModel = new QFileSystemModel();
    myModel->setRootPath(QDir::homePath());
    //Let's wait to populate the model
    QTest::qWait(250);
    //We change the provider, icons must me updated
    CustomFileIconProvider *custom = new CustomFileIconProvider();
    myModel->setIconProvider(custom);

    QPixmap mb = qApp->style()->standardIcon(QStyle::SP_MessageBoxCritical).pixmap(50, 50);
    QCOMPARE(myModel->fileIcon(myModel->index(QDir::homePath())).pixmap(50, 50), mb);
    delete myModel;
    delete custom;
}

bool tst_QFileSystemModel::createFiles(const QString &test_path, const QStringList &initial_files, int existingFileCount, const QStringList &initial_dirs, const QString &dir)
{
    QDir baseDir(dir);
    if (!baseDir.exists(test_path)) {
        if (!baseDir.mkdir(test_path) && false) {
            qDebug() << "failed to create dir" << test_path;
            return false;
        }
    }
    //qDebug() << (model->rowCount(model->index(test_path))) << existingFileCount << initial_files;
    TRY_WAIT((model->rowCount(model->index(test_path)) == existingFileCount));
    for (int i = 0; i < initial_dirs.count(); ++i) {
        QDir dir(test_path);
        if (!dir.exists()) {
            qWarning() << "error" << test_path << "doesn't exists";
            return false;
        }
        if(!dir.mkdir(initial_dirs.at(i))) {
            qWarning() << "error" << "failed to make" << initial_dirs.at(i);
            return false;
        }
        //qDebug() << test_path + '/' + initial_dirs.at(i) << (QFile::exists(test_path + '/' + initial_dirs.at(i)));
    }
    for (int i = 0; i < initial_files.count(); ++i) {
        QFile file(test_path + '/' + initial_files.at(i));
        if (!file.open(QIODevice::WriteOnly | QIODevice::Append)) {
            qDebug() << "failed to open file" << initial_files.at(i);
            return false;
        }
        if (!file.resize(1024 + file.size())) {
            qDebug() << "failed to resize file" << initial_files.at(i);
            return false;
        }
        if (!file.flush()) {
            qDebug() << "failed to flush file" << initial_files.at(i);
            return false;
        }
        file.close();
#if defined(Q_OS_WIN) && !defined(Q_OS_WINCE)
        if (initial_files.at(i)[0] == '.')
            QProcess::execute(QString("attrib +h %1").arg(file.fileName()));
#elif defined(Q_OS_SYMBIAN)
        if (initial_files.at(i)[0] == '.') {
            HBufC* buffer = qt_QString2HBufCNewL(QDir::toNativeSeparators(file.fileName()));
            rfs.SetAtt(*buffer, KEntryAttHidden, 0);
            delete buffer;
            }
#endif
        //qDebug() << test_path + '/' + initial_files.at(i) << (QFile::exists(test_path + '/' + initial_files.at(i)));
    }
    return true;
}

void tst_QFileSystemModel::rowCount()
{
    QString tmp = flatDirTestPath;
    QVERIFY(createFiles(tmp, QStringList()));

    QSignalSpy spy2(model, SIGNAL(rowsInserted(const QModelIndex &, int, int)));
    QSignalSpy spy3(model, SIGNAL(rowsAboutToBeInserted(const QModelIndex &, int, int)));

#if !defined(Q_OS_WINCE)
    QStringList files = QStringList() <<  "b" << "d" << "f" << "h" << "j" << ".a" << ".c" << ".e" << ".g";
    QString l = "b,d,f,h,j,.a,.c,.e,.g";
#else
    // Cannot hide them on CE
    QStringList files = QStringList() <<  "b" << "d" << "f" << "h" << "j";
    QString l = "b,d,f,h,j";
#endif
    QVERIFY(createFiles(tmp, files));

    QModelIndex root = model->setRootPath(tmp);
    QTRY_COMPARE(model->rowCount(root), 5);
    QVERIFY(spy2.count() > 0);
    QVERIFY(spy3.count() > 0);
}

void tst_QFileSystemModel::rowsInserted_data()
{
    QTest::addColumn<int>("count");
    QTest::addColumn<int>("assending");
    for (int i = 0; i < 4; ++i) {
        QTest::newRow(QString("Qt::AscendingOrder %1").arg(i).toLocal8Bit().constData())  << i << (int)Qt::AscendingOrder;
        QTest::newRow(QString("Qt::DescendingOrder %1").arg(i).toLocal8Bit().constData()) << i << (int)Qt::DescendingOrder;
    }
}

void tst_QFileSystemModel::rowsInserted()
{
#if defined(Q_OS_WINCE)
    QSKIP("Watching directories does not work on CE(see #137910)", SkipAll);
#endif
    QString tmp = flatDirTestPath;
    rowCount();
    QModelIndex root = model->index(model->rootPath());

    QFETCH(int, assending);
    QFETCH(int, count);
    model->sort(0, (Qt::SortOrder)assending);

    QSignalSpy spy0(model, SIGNAL(rowsInserted(const QModelIndex &, int, int)));
    QSignalSpy spy1(model, SIGNAL(rowsAboutToBeInserted(const QModelIndex &, int, int)));
    int oldCount = model->rowCount(root);
    QStringList files;
    for (int i = 0; i < count; ++i)
        files.append(QString("c%1").arg(i));
    QVERIFY(createFiles(tmp, files, 5));
    TRY_WAIT(model->rowCount(root) == oldCount + count);
    QTRY_COMPARE(model->rowCount(root), oldCount + count);
    QTest::qWait(100); // Let the sort settle.
    int totalRowsInserted = 0;
    for (int i = 0; i < spy0.count(); ++i) {
        int start = spy0[i].value(1).toInt();
        int end = spy0[i].value(2).toInt();
        totalRowsInserted += end - start + 1;
    }
    QCOMPARE(totalRowsInserted, count);
    if (assending == (Qt::SortOrder)Qt::AscendingOrder) {
        QString letter = model->index(model->rowCount(root) - 1, 0, root).data().toString();
        QCOMPARE(letter, QString("j"));
    } else {
        QCOMPARE(model->index(model->rowCount(root) - 1, 0, root).data().toString(), QString("b"));
    }
    if (spy0.count() > 0) {
        if (count == 0)
            QCOMPARE(spy0.count(), 0);
        else
            QVERIFY(spy0.count() >= 1);
    }
    if (count == 0) QCOMPARE(spy1.count(), 0); else QVERIFY(spy1.count() >= 1);

    QVERIFY(createFiles(tmp, QStringList(".hidden_file"), 5 + count));

    if (count != 0) QTRY_VERIFY(spy0.count() >= 1); else QTRY_VERIFY(spy0.count() == 0);
    if (count != 0) QTRY_VERIFY(spy1.count() >= 1); else QTRY_VERIFY(spy1.count() == 0);
}

void tst_QFileSystemModel::rowsRemoved_data()
{
    rowsInserted_data();
}

void tst_QFileSystemModel::rowsRemoved()
{
#if defined(Q_OS_WINCE)
    QSKIP("Watching directories does not work on CE(see #137910)", SkipAll);
#endif
    QString tmp = flatDirTestPath;
    rowCount();
    QModelIndex root = model->index(model->rootPath());

    QFETCH(int, count);
    QFETCH(int, assending);
    model->sort(0, (Qt::SortOrder)assending);
    QTest::qWait(WAITTIME);

    QSignalSpy spy0(model, SIGNAL(rowsRemoved(const QModelIndex &, int, int)));
    QSignalSpy spy1(model, SIGNAL(rowsAboutToBeRemoved(const QModelIndex &, int, int)));
    int oldCount = model->rowCount(root);
    for (int i = count - 1; i >= 0; --i) {
        //qDebug() << "removing" <<  model->index(i, 0, root).data().toString();
        QVERIFY(QFile::remove(tmp + '/' + model->index(i, 0, root).data().toString()));
    }
    for (int i = 0 ; i < 10; ++i) {
        QTest::qWait(WAITTIME);
        qApp->processEvents();
        if (count != 0) {
            if (i == 10 || spy0.count() != 0) {
                QVERIFY(spy0.count() >= 1);
                QVERIFY(spy1.count() >= 1);
            }
        } else {
            if (i == 10 || spy0.count() == 0) {
                QVERIFY(spy0.count() == 0);
                QVERIFY(spy1.count() == 0);
            }
        }
        QStringList lst;
        for (int i = 0; i < model->rowCount(root); ++i)
            lst.append(model->index(i, 0, root).data().toString());
        if (model->rowCount(root) == oldCount - count)
            break;
        qDebug() << "still have:" << lst << QFile::exists(tmp + '/' + QString(".a"));
        QDir tmpLister(tmp);
        qDebug() << tmpLister.entryList();
    }
    QTRY_COMPARE(model->rowCount(root), oldCount - count);

    QVERIFY(QFile::exists(tmp + '/' + QString(".a")));
    QVERIFY(QFile::remove(tmp + '/' + QString(".a")));
    QVERIFY(QFile::remove(tmp + '/' + QString(".c")));
    QTest::qWait(WAITTIME);

    if (count != 0) QVERIFY(spy0.count() >= 1); else QVERIFY(spy0.count() == 0);
    if (count != 0) QVERIFY(spy1.count() >= 1); else QVERIFY(spy1.count() == 0);
}

void tst_QFileSystemModel::dataChanged_data()
{
    rowsInserted_data();
}

void tst_QFileSystemModel::dataChanged()
{
    // This can't be tested right now sense we don't watch files, only directories
    return;

    /*
    QString tmp = flatDirTestPath;
    rowCount();
    QModelIndex root = model->index(model->rootPath());

    QFETCH(int, count);
    QFETCH(int, assending);
    model->sort(0, (Qt::SortOrder)assending);

    QSignalSpy spy(model, SIGNAL(dataChanged (const QModelIndex &, const QModelIndex &)));
    QStringList files;
    for (int i = 0; i < count; ++i)
        files.append(model->index(i, 0, root).data().toString());
    createFiles(tmp, files);

    QTest::qWait(WAITTIME);

    if (count != 0) QVERIFY(spy.count() >= 1); else QVERIFY(spy.count() == 0);
    */
}

void tst_QFileSystemModel::filters_data()
{
    QTest::addColumn<QStringList>("files");
    QTest::addColumn<QStringList>("dirs");
    QTest::addColumn<int>("dirFilters");
    QTest::addColumn<QStringList>("nameFilters");
    QTest::addColumn<int>("rowCount");
#if !defined(Q_OS_WINCE) && !defined(Q_OS_SYMBIAN)
    QTest::newRow("no dirs") << (QStringList() << "a" << "b" << "c") << QStringList() << (int)(QDir::Dirs) << QStringList() << 2;
    QTest::newRow("one dir - dotdot") << (QStringList() << "a" << "b" << "c") << (QStringList() << "Z") << (int)(QDir::Dirs | QDir::NoDotAndDotDot) << QStringList() << 1;
    QTest::newRow("one dir") << (QStringList() << "a" << "b" << "c") << (QStringList() << "Z") << (int)(QDir::Dirs) << QStringList() << 3;
    QTest::newRow("no dir + hidden") << (QStringList() << "a" << "b" << "c") << QStringList() << (int)(QDir::Dirs | QDir::Hidden) << QStringList() << 2;
    QTest::newRow("dir+hid+files") << (QStringList() << "a" << "b" << "c") << QStringList() <<
                         (int)(QDir::Dirs | QDir::Files | QDir::Hidden) << QStringList() << 5;
    QTest::newRow("dir+file+hid-dot .A") << (QStringList() << "a" << "b" << "c") << (QStringList() << ".A") <<
                         (int)(QDir::Dirs | QDir::Files | QDir::Hidden | QDir::NoDotAndDotDot) << QStringList() << 4;
    QTest::newRow("dir+files+hid+dot A") << (QStringList() << "a" << "b" << "c") << (QStringList() << "AFolder") <<
                         (int)(QDir::Dirs | QDir::Files | QDir::Hidden | QDir::NoDotAndDotDot) << (QStringList() << "A*") << 2;
    QTest::newRow("dir+files+hid+dot+cas1") << (QStringList() << "a" << "b" << "c") << (QStringList() << "Z") <<
                         (int)(QDir::Dirs | QDir::Files | QDir::Hidden | QDir::NoDotAndDotDot | QDir::CaseSensitive) << (QStringList() << "Z") << 1;
    QTest::newRow("dir+files+hid+dot+cas2") << (QStringList() << "a" << "b" << "c") << (QStringList() << "Z") <<
                         (int)(QDir::Dirs | QDir::Files | QDir::Hidden | QDir::NoDotAndDotDot | QDir::CaseSensitive) << (QStringList() << "a") << 1;
    QTest::newRow("dir+files+hid+dot+cas+alldir") << (QStringList() << "a" << "b" << "c") << (QStringList() << "Z") <<
                         (int)(QDir::Dirs | QDir::Files | QDir::Hidden | QDir::NoDotAndDotDot | QDir::CaseSensitive | QDir::AllDirs) << (QStringList() << "Z") << 1;
#else
    QTest::qWait(3000); // We need to calm down a bit...
    QTest::newRow("no dirs") << (QStringList() << "a" << "b" << "c") << QStringList() << (int)(QDir::Dirs) << QStringList() << 0;
    QTest::newRow("one dir - dotdot") << (QStringList() << "a" << "b" << "c") << (QStringList() << "Z") << (int)(QDir::Dirs | QDir::NoDotAndDotDot) << QStringList() << 1;
    QTest::newRow("one dir") << (QStringList() << "a" << "b" << "c") << (QStringList() << "Z") << (int)(QDir::Dirs) << QStringList() << 1;
    QTest::newRow("no dir + hidden") << (QStringList() << "a" << "b" << "c") << QStringList() << (int)(QDir::Dirs | QDir::Hidden) << QStringList() << 0;
    QTest::newRow("dir+hid+files") << (QStringList() << "a" << "b" << "c") << QStringList() <<
                         (int)(QDir::Dirs | QDir::Files | QDir::Hidden) << QStringList() << 3;
#if defined(Q_OS_SYMBIAN)
    // Some symbian envs have a bug that causes "A" and ".A" to be considered same name in file system.
    QTest::newRow("dir+file+hid-dot .D") << (QStringList() << "a" << "b" << "c") << (QStringList() << ".D") <<
#else
    QTest::newRow("dir+file+hid-dot .A") << (QStringList() << "a" << "b" << "c") << (QStringList() << ".A") <<
#endif
                         (int)(QDir::Dirs | QDir::Files | QDir::Hidden | QDir::NoDotAndDotDot) << QStringList() << 4;
    QTest::newRow("dir+files+hid+dot A") << (QStringList() << "a" << "b" << "c") << (QStringList() << "AFolder") <<
                         (int)(QDir::Dirs | QDir::Files | QDir::Hidden | QDir::NoDotAndDotDot) << (QStringList() << "A*") << 2;
    QTest::newRow("dir+files+hid+dot+cas1") << (QStringList() << "a" << "b" << "c") << (QStringList() << "Z") <<
                         (int)(QDir::Dirs | QDir::Files | QDir::Hidden | QDir::NoDotAndDotDot | QDir::CaseSensitive) << (QStringList() << "Z") << 1;
    QTest::newRow("dir+files+hid+dot+cas2") << (QStringList() << "a" << "b" << "c") << (QStringList() << "Z") <<
                         (int)(QDir::Dirs | QDir::Files | QDir::Hidden | QDir::NoDotAndDotDot | QDir::CaseSensitive) << (QStringList() << "a") << 1;
    QTest::newRow("dir+files+hid+dot+cas+alldir") << (QStringList() << "a" << "b" << "c") << (QStringList() << "Z") <<
                         (int)(QDir::Dirs | QDir::Files | QDir::Hidden | QDir::NoDotAndDotDot | QDir::CaseSensitive | QDir::AllDirs) << (QStringList() << "Z") << 1;
#endif

    QTest::newRow("case sensitive") << (QStringList() << "Antiguagdb" << "Antiguamtd"
        << "Antiguamtp" << "afghanistangdb" << "afghanistanmtd")
        << QStringList() << (int)(QDir::Files) << QStringList() << 5;
}

void tst_QFileSystemModel::filters()
{
    QString tmp = flatDirTestPath;
    QVERIFY(createFiles(tmp, QStringList()));
    QModelIndex root = model->setRootPath(tmp);
    QFETCH(QStringList, files);
    QFETCH(QStringList, dirs);
    QFETCH(int, dirFilters);
    QFETCH(QStringList, nameFilters);
    QFETCH(int, rowCount);

    if (nameFilters.count() > 0)
        model->setNameFilters(nameFilters);
    model->setNameFilterDisables(false);
    model->setFilter((QDir::Filters)dirFilters);

    QVERIFY(createFiles(tmp, files, 0, dirs));
    QTRY_COMPARE(model->rowCount(root), rowCount);

    // Make sure that we do what QDir does
    QDir xFactor(tmp);
    QDir::Filters  filters = (QDir::Filters)dirFilters;
    if (nameFilters.count() > 0)
        QCOMPARE(xFactor.entryList(nameFilters, filters).count(), rowCount);
    else
        QVERIFY(xFactor.entryList(filters).count() == rowCount);

#ifdef Q_OS_LINUX
    if (files.count() >= 3 && rowCount >= 3 && rowCount != 5) {
        QString fileName1 = (tmp + '/' + files.at(0));
        QString fileName2 = (tmp + '/' + files.at(1));
        QString fileName3 = (tmp + '/' + files.at(2));
        QFile::Permissions originalPermissions = QFile::permissions(fileName1);
        QVERIFY(QFile::setPermissions(fileName1, QFile::WriteOwner));
        QVERIFY(QFile::setPermissions(fileName2, QFile::ReadOwner));
        QVERIFY(QFile::setPermissions(fileName3, QFile::ExeOwner));

        model->setFilter((QDir::Files | QDir::Readable));
        QTRY_COMPARE(model->rowCount(root), 1);

        model->setFilter((QDir::Files | QDir::Writable));
        QTRY_COMPARE(model->rowCount(root), 1);

        model->setFilter((QDir::Files | QDir::Executable));
        QTRY_COMPARE(model->rowCount(root), 1);

        // reset permissions
        QVERIFY(QFile::setPermissions(fileName1, originalPermissions));
        QVERIFY(QFile::setPermissions(fileName2, originalPermissions));
        QVERIFY(QFile::setPermissions(fileName3, originalPermissions));
    }
#endif
}

void tst_QFileSystemModel::nameFilters()
{
    QStringList list;
    list << "a" << "b" << "c";
    model->setNameFilters(list);
    model->setNameFilterDisables(false);
    QCOMPARE(model->nameFilters(), list);

    QString tmp = flatDirTestPath;
    QVERIFY(createFiles(tmp, list));
    QModelIndex root = model->setRootPath(tmp);
    QTRY_COMPARE(model->rowCount(root), 3);

    QStringList filters;
    filters << "a" << "b";
    model->setNameFilters(filters);
    QTRY_COMPARE(model->rowCount(root), 2);
}
void tst_QFileSystemModel::setData_data()
{
    QTest::addColumn<QStringList>("files");
    QTest::addColumn<QString>("oldFileName");
    QTest::addColumn<QString>("newFileName");
    QTest::addColumn<bool>("success");
    /*QTest::newRow("outside current dir") << (QStringList() << "a" << "b" << "c")
              << flatDirTestPath + '/' + "a"
              << QDir::temp().absolutePath() + '/' + "a"
              << false;
    */
    QTest::newRow("in current dir") << (QStringList() << "a" << "b" << "c")
              << "a"
              << "d"
              << true;
}

void tst_QFileSystemModel::setData()
{
    QSignalSpy spy(model, SIGNAL(fileRenamed(const QString&, const QString&, const QString&)));
    QString tmp = flatDirTestPath;
    QFETCH(QStringList, files);
    QFETCH(QString, oldFileName);
    QFETCH(QString, newFileName);
    QFETCH(bool, success);

    QVERIFY(createFiles(tmp, files));
    QModelIndex root = model->setRootPath(tmp);
    QTRY_COMPARE(model->rowCount(root), files.count());

    QModelIndex idx = model->index(tmp + '/' + oldFileName);
    QCOMPARE(idx.isValid(), true);
    QCOMPARE(model->setData(idx, newFileName), false);

    model->setReadOnly(false);
    QCOMPARE(model->setData(idx, newFileName), success);
    if (success) {
        QCOMPARE(spy.count(), 1);
        QList<QVariant> arguments = spy.takeFirst();
        QCOMPARE(model->data(idx, QFileSystemModel::FileNameRole).toString(), newFileName);
        QCOMPARE(model->index(arguments.at(0).toString()), model->index(tmp));
        QCOMPARE(arguments.at(1).toString(), oldFileName);
        QCOMPARE(arguments.at(2).toString(), newFileName);
        QCOMPARE(QFile::rename(tmp + '/' + newFileName, tmp + '/' + oldFileName), true);
    }
    QTRY_COMPARE(model->rowCount(root), files.count());
}

class MyFriendFileSystemModel : public QFileSystemModel
{
    friend class tst_QFileSystemModel;
    Q_DECLARE_PRIVATE(QFileSystemModel)
};

void tst_QFileSystemModel::sort_data()
{
    QTest::addColumn<bool>("fileDialogMode");
    QTest::newRow("standard usage") << false;
    QTest::newRow("QFileDialog usage") << true;
}

void tst_QFileSystemModel::sort()
{
    QTemporaryFile file;
    file.open();
    QModelIndex root = model->setRootPath(QDir::tempPath());
    QTRY_VERIFY(model->rowCount(root) > 0);

    QPersistentModelIndex idx = model->index(0, 1, root);
    model->sort(0, Qt::AscendingOrder);
    model->sort(0, Qt::DescendingOrder);
    QVERIFY(idx.column() != 0);

    model->setRootPath(QDir::homePath());

    QFETCH(bool, fileDialogMode);

    MyFriendFileSystemModel *myModel = new MyFriendFileSystemModel();
    QTreeView *tree = new QTreeView();

    if (fileDialogMode)
        myModel->d_func()->disableRecursiveSort = true;

    QDir dir(QDir::tempPath());
    //initialize the randomness
    qsrand(QDateTime::currentDateTime().toTime_t());
    QString tempName = QLatin1String("sortTemp.") + QString::number(qrand());
    dir.mkdir(tempName);
    dir.cd(tempName);
    QTRY_VERIFY(dir.exists());

    const QString dirPath = dir.absolutePath();
    QVERIFY(dir.exists());

    //Create a file that will be at the end when sorting by name (For Mac, the default)
    //but if we sort by size descending it will be the first
    QFile tempFile(dirPath + "/plop2.txt");
    tempFile.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&tempFile);
    out << "The magic number is: " << 49 << "\n";
    tempFile.close();

    QFile tempFile2(dirPath + "/plop.txt");
    tempFile2.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out2(&tempFile2);
    out2 << "The magic number is : " << 49 << " but i write some stuff in the file \n";
    tempFile2.close();

    myModel->setRootPath("");
    myModel->setFilter(QDir::AllEntries | QDir::System | QDir::Hidden);
    tree->setSortingEnabled(true);
    tree->setModel(myModel);
    tree->show();
    tree->resize(800, 800);
    QTest::qWait(500);
    tree->header()->setSortIndicator(1,Qt::DescendingOrder);
    tree->header()->setResizeMode(0, QHeaderView::ResizeToContents);
    QStringList dirsToOpen;
    do
    {
        dirsToOpen<<dir.absolutePath();
    } while (dir.cdUp());

    for (int i = dirsToOpen.size() -1 ; i > 0 ; --i) {
        QString path = dirsToOpen[i];
        QTest::qWait(500);
        tree->expand(myModel->index(path, 0));
    }
    tree->expand(myModel->index(dirPath, 0));
    QTest::qWait(500);
    QModelIndex parent = myModel->index(dirPath, 0);
    QList<QString> expectedOrder;
    expectedOrder << tempFile2.fileName() << tempFile.fileName() << dirPath + QChar('/') + "." << dirPath + QChar('/') + "..";
    //File dialog Mode means sub trees are not sorted, only the current root
    if (fileDialogMode) {
       QList<QString> actualRows;
        for(int i = 0; i < myModel->rowCount(parent); ++i)
        {
            actualRows << dirPath + QChar('/') + myModel->index(i, 1, parent).data(QFileSystemModel::FileNameRole).toString();
        }
        QVERIFY(actualRows != expectedOrder);
    } else {
        for(int i = 0; i < myModel->rowCount(parent); ++i)
        {
            QTRY_COMPARE(dirPath + QChar('/') + myModel->index(i, 1, parent).data(QFileSystemModel::FileNameRole).toString(), expectedOrder.at(i));
        }
    }

    delete tree;
    delete myModel;

    dir.setPath(QDir::tempPath());
    dir.cd(tempName);
    tempFile.remove();
    tempFile2.remove();
    dir.cdUp();
    dir.rmdir(tempName);

}

void tst_QFileSystemModel::mkdir()
{
    QString tmp = QDir::tempPath();
    QString newFolderPath = QDir::toNativeSeparators(tmp + '/' + "NewFoldermkdirtest4");
    QModelIndex tmpDir = model->index(tmp);
    QVERIFY(tmpDir.isValid());
    QDir bestatic(newFolderPath);
    if (bestatic.exists()) {
        if (!bestatic.rmdir(newFolderPath))
            qWarning() << "unable to remove" << newFolderPath;
        QTest::qWait(WAITTIME);
    }
    model->mkdir(tmpDir, "NewFoldermkdirtest3");
    model->mkdir(tmpDir, "NewFoldermkdirtest5");
    QModelIndex idx = model->mkdir(tmpDir, "NewFoldermkdirtest4");
    QVERIFY(idx.isValid());
    int oldRow = idx.row();
    QTest::qWait(WAITTIME);
    idx = model->index(newFolderPath);
    QDir cleanup(tmp);
    QVERIFY(cleanup.rmdir(QLatin1String("NewFoldermkdirtest3")));
    QVERIFY(cleanup.rmdir(QLatin1String("NewFoldermkdirtest5")));
    bestatic.rmdir(newFolderPath);
    QVERIFY(0 != idx.row());
    QCOMPARE(oldRow, idx.row());
}

void tst_QFileSystemModel::caseSensitivity()
{
    QString tmp = flatDirTestPath;
    QStringList files;
    files << "a" << "c" << "C";
    QVERIFY(createFiles(tmp, files));
    QModelIndex root = model->index(tmp);
    QCOMPARE(model->rowCount(root), 0);
    for (int i = 0; i < files.count(); ++i) {
        QVERIFY(model->index(tmp + '/' + files.at(i)).isValid());
    }
}

#if defined(Q_OS_WIN) && !defined(Q_OS_WINCE)
void tst_QFileSystemModel::Win32LongFileName()
{
    QString tmp = flatDirTestPath;
    QStringList files;
    files << "aaaaaaaaaa" << "bbbbbbbbbb" << "cccccccccc";
    QVERIFY(createFiles(tmp, files));
    QModelIndex root = model->setRootPath(tmp);
    QTRY_VERIFY(model->index(tmp + QLatin1String("/aaaaaa~1")).isValid());
    QTRY_VERIFY(model->index(tmp + QLatin1String("/bbbbbb~1")).isValid());
    QTRY_VERIFY(model->index(tmp + QLatin1String("/cccccc~1")).isValid());
}
#endif

void tst_QFileSystemModel::drives_data()
{
    QTest::addColumn<QString>("path");
    QTest::newRow("current") << QDir::currentPath();
    QTest::newRow("slash") << "/";
    QTest::newRow("My Computer") << "My Computer";
}

void tst_QFileSystemModel::drives()
{
    QFETCH(QString, path);
    QFileSystemModel model;
    model.setRootPath(path);
    model.fetchMore(QModelIndex());
    QFileInfoList drives = QDir::drives();
    QTest::qWait(5000);
    QTRY_COMPARE(model.rowCount(), drives.count());
}

void tst_QFileSystemModel::dirsBeforeFiles()
{
    const QString dirPath = QString("%1/task221717_sortedOrder_test_dir").arg(QDir::tempPath());
    QDir dir(dirPath);
    // clean up from last time
    if (dir.exists()) {
        for (int i = 0; i < 3; ++i) {
            QLatin1Char c('a' + i);
            dir.rmdir(QString("%1-dir").arg(c));
            QFile::remove(dirPath + QString("/%1-file").arg(c));
        }
        dir.rmdir(dirPath);
    }
    dir.mkpath(dirPath);
    QVERIFY(dir.exists());

    for (int i = 0; i < 3; ++i) {
        QLatin1Char c('a' + i);
        dir.mkdir(QString("%1-dir").arg(c));
        QFile file(dirPath + QString("/%1-file").arg(c));
        file.open(QIODevice::ReadWrite);
        file.close();
    }

    QModelIndex root = model->setRootPath(dirPath);
    QTest::qWait(1000); // allow model to be notified by the file system watcher

    // ensure that no file occurs before a directory
    for (int i = 0; i < model->rowCount(root); ++i) {
#ifndef Q_OS_MAC
        QVERIFY(i == 0 ||
                !(model->fileInfo(model->index(i - 1, 0, root)).isFile()
                  && model->fileInfo(model->index(i, 0, root)).isDir()));
#else
        QVERIFY(i == 0 ||
                model->fileInfo(model->index(i - 1, 0, root)).fileName() <
                model->fileInfo(model->index(i, 0, root)).fileName());
#endif
    }
}

void tst_QFileSystemModel::roleNames_data()
{
    QTest::addColumn<int>("role");
    QTest::addColumn<QByteArray>("roleName");
    QTest::newRow("decoration") << int(Qt::DecorationRole) << QByteArray("decoration");
    QTest::newRow("display") << int(Qt::DisplayRole) << QByteArray("display");
    QTest::newRow("fileIcon") << int(QFileSystemModel::FileIconRole) << QByteArray("fileIcon");
    QTest::newRow("filePath") << int(QFileSystemModel::FilePathRole) << QByteArray("filePath");
    QTest::newRow("fileName") << int(QFileSystemModel::FileNameRole) << QByteArray("fileName");
    QTest::newRow("filePermissions") << int(QFileSystemModel::FilePermissions) << QByteArray("filePermissions");
}

void tst_QFileSystemModel::roleNames()
{
    QFileSystemModel model;
    QHash<int, QByteArray> roles = model.roleNames();

    QFETCH(int, role);
    QVERIFY(roles.contains(role));

    QFETCH(QByteArray, roleName);
    QList<QByteArray> values = roles.values(role);
    QVERIFY(values.contains(roleName));
}

QTEST_MAIN(tst_QFileSystemModel)
#include "tst_qfilesystemmodel.moc"

