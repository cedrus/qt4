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
#include <qdir.h>
#include <qpluginloader.h>
#include "theplugin/plugininterface.h"

// Helper macros to let us know if some suffixes are valid
#define bundle_VALID    false
#define dylib_VALID     false
#define sl_VALID        false
#define a_VALID         false
#define so_VALID        false
#define dll_VALID       false

#if defined(Q_OS_DARWIN)
# undef bundle_VALID
# undef dylib_VALID
# undef so_VALID
# define bundle_VALID   true
# define dylib_VALID    true
# define so_VALID       true
# define SUFFIX         ".dylib"
# define PREFIX         "lib"

#elif defined(Q_OS_HPUX) && !defined(__ia64)
# undef sl_VALID
# define sl_VALID       true
# define SUFFIX         ".sl"
# define PREFIX         "lib"

#elif defined(Q_OS_AIX)
# undef a_VALID
# undef so_VALID
# define a_VALID        true
# define so_VALID       true
# define SUFFIX         ".so"
# define PREFIX         "lib"

#elif defined(Q_OS_WIN)
# undef dll_VALID
# define dll_VALID      true
# ifdef QT_NO_DEBUG
#  define SUFFIX         ".dll"
# else
#  define SUFFIX         "d.dll"
# endif
# define PREFIX         ""

#elif defined(Q_OS_SYMBIAN)
# undef dll_VALID
# define dll_VALID      true
# define SUFFIX         ".dll"
# define PREFIX         ""

#else  // all other Unix
# undef so_VALID
# define so_VALID       true
# define SUFFIX         ".so"
# define PREFIX         "lib"
#endif

static QString sys_qualifiedLibraryName(const QString &fileName)
{
    QString currDir = QDir::currentPath();
    return currDir + "/bin/" + PREFIX + fileName + SUFFIX;
}

//TESTED_CLASS=
//TESTED_FILES=

QT_FORWARD_DECLARE_CLASS(QPluginLoader)
class tst_QPluginLoader : public QObject
{
    Q_OBJECT

public:
    tst_QPluginLoader();
    virtual ~tst_QPluginLoader();

private slots:
    void errorString();
    void loadHints();
    void deleteinstanceOnUnload();
    void checkingStubsFromDifferentDrives();
};

tst_QPluginLoader::tst_QPluginLoader()

{
}

tst_QPluginLoader::~tst_QPluginLoader()
{
}

//#define SHOW_ERRORS 1

void tst_QPluginLoader::errorString()
{
#if defined(Q_OS_WINCE)
    // On WinCE we need an QCoreApplication object for current dir
    int argc = 0;
    QCoreApplication app(argc,0);
#endif
    const QString unknown(QLatin1String("Unknown error"));

    {
    QPluginLoader loader; // default constructed
    bool loaded = loader.load();
#ifdef SHOW_ERRORS
    qDebug() << loader.errorString();
#endif
    QCOMPARE(loaded, false);
    QCOMPARE(loader.errorString(), unknown);

    QObject *obj = loader.instance();
#ifdef SHOW_ERRORS
    qDebug() << loader.errorString();
#endif
    QCOMPARE(obj, static_cast<QObject*>(0));
    QCOMPARE(loader.errorString(), unknown);

    bool unloaded = loader.unload();
#ifdef SHOW_ERRORS
    qDebug() << loader.errorString();
#endif
    QCOMPARE(unloaded, false);
    QCOMPARE(loader.errorString(), unknown);
    }
    {
    QPluginLoader loader( sys_qualifiedLibraryName("tst_qpluginloaderlib"));     //not a plugin
    bool loaded = loader.load();
#ifdef SHOW_ERRORS
    qDebug() << loader.errorString();
#endif
    QCOMPARE(loaded, false);
    QVERIFY(loader.errorString() != unknown);

    QObject *obj = loader.instance();
#ifdef SHOW_ERRORS
    qDebug() << loader.errorString();
#endif
    QCOMPARE(obj, static_cast<QObject*>(0));
    QVERIFY(loader.errorString() != unknown);

    bool unloaded = loader.unload();
#ifdef SHOW_ERRORS
    qDebug() << loader.errorString();
#endif
    QCOMPARE(unloaded, false);
    QVERIFY(loader.errorString() != unknown);
    }

    {
    QPluginLoader loader( sys_qualifiedLibraryName("nosuchfile"));     //not a file
    bool loaded = loader.load();
#ifdef SHOW_ERRORS
    qDebug() << loader.errorString();
#endif
    QCOMPARE(loaded, false);
    QVERIFY(loader.errorString() != unknown);

    QObject *obj = loader.instance();
#ifdef SHOW_ERRORS
    qDebug() << loader.errorString();
#endif
    QCOMPARE(obj, static_cast<QObject*>(0));
    QVERIFY(loader.errorString() != unknown);

    bool unloaded = loader.unload();
#ifdef SHOW_ERRORS
    qDebug() << loader.errorString();
#endif
    QCOMPARE(unloaded, false);
    QVERIFY(loader.errorString() != unknown);
    }

#if !defined Q_OS_WIN && !defined Q_OS_MAC && !defined Q_OS_HPUX && !defined Q_OS_SYMBIAN
    {
    QPluginLoader loader( sys_qualifiedLibraryName("almostplugin"));     //a plugin with unresolved symbols
    loader.setLoadHints(QLibrary::ResolveAllSymbolsHint);
    QCOMPARE(loader.load(), false);
#ifdef SHOW_ERRORS
    qDebug() << loader.errorString();
#endif
    QVERIFY(loader.errorString() != unknown);

    QCOMPARE(loader.instance(), static_cast<QObject*>(0));
#ifdef SHOW_ERRORS
    qDebug() << loader.errorString();
#endif
    QVERIFY(loader.errorString() != unknown);

    QCOMPARE(loader.unload(), false);
#ifdef SHOW_ERRORS
    qDebug() << loader.errorString();
#endif
    QVERIFY(loader.errorString() != unknown);
    }
#endif

    {
    QPluginLoader loader( sys_qualifiedLibraryName("theplugin"));     //a plugin
    QCOMPARE(loader.load(), true);
    QCOMPARE(loader.errorString(), unknown);

    QVERIFY(loader.instance() !=  static_cast<QObject*>(0));
    QCOMPARE(loader.errorString(), unknown);

    // Make sure that plugin really works
    PluginInterface* theplugin = qobject_cast<PluginInterface*>(loader.instance());
    QString pluginName = theplugin->pluginName();
    QCOMPARE(pluginName, QLatin1String("Plugin ok"));

    QCOMPARE(loader.unload(), true);
    QCOMPARE(loader.errorString(), unknown);
    }
}

void tst_QPluginLoader::loadHints()
{
    QPluginLoader loader;
    QCOMPARE(loader.loadHints(), (QLibrary::LoadHints)0);   //Do not crash
    loader.setLoadHints(QLibrary::ResolveAllSymbolsHint);
    loader.setFileName( sys_qualifiedLibraryName("theplugin"));     //a plugin
    QCOMPARE(loader.loadHints(), QLibrary::ResolveAllSymbolsHint);
}

void tst_QPluginLoader::deleteinstanceOnUnload()
{
    for (int pass = 0; pass < 2; ++pass) {
        QPluginLoader loader1;
        loader1.setFileName( sys_qualifiedLibraryName("theplugin"));     //a plugin
        if (pass == 0)
            loader1.load(); // not recommended, instance() should do the job.
        PluginInterface *instance1 = qobject_cast<PluginInterface*>(loader1.instance());
        QVERIFY(instance1);
        QCOMPARE(instance1->pluginName(), QLatin1String("Plugin ok"));

        QPluginLoader loader2;
        loader2.setFileName( sys_qualifiedLibraryName("theplugin"));     //a plugin
        if (pass == 0)
            loader2.load(); // not recommended, instance() should do the job.
        PluginInterface *instance2 = qobject_cast<PluginInterface*>(loader2.instance());
        QCOMPARE(instance2->pluginName(), QLatin1String("Plugin ok"));

        QSignalSpy spy1(loader1.instance(), SIGNAL(destroyed()));
        QSignalSpy spy2(loader2.instance(), SIGNAL(destroyed()));
        QCOMPARE(loader1.unload(), false);  // refcount not reached 0, not really unloaded
        QCOMPARE(spy1.count(), 0);
        QCOMPARE(spy2.count(), 0);
        QCOMPARE(instance1->pluginName(), QLatin1String("Plugin ok"));
        QCOMPARE(instance2->pluginName(), QLatin1String("Plugin ok"));
        QCOMPARE(loader2.unload(), true);   // refcount reached 0, did really unload
        QCOMPARE(spy1.count(), 1);
        QCOMPARE(spy2.count(), 1);
    }
}

void tst_QPluginLoader::checkingStubsFromDifferentDrives()
{
#if defined(Q_OS_SYMBIAN)

    // This test needs C-drive + some additional drive (driveForStubs)

    const QString driveForStubs("E:/");// != "C:/"
    const QString stubDir("system/temp/stubtest/");
    const QString stubName("dummyStub.qtplugin");
    const QString fullStubFileName(stubDir + stubName);
    QDir dir(driveForStubs);
    bool test1(false); bool test2(false);

    // initial clean up
    QFile::remove(driveForStubs + fullStubFileName);
    dir.rmdir(driveForStubs + stubDir);

    // create a stub dir and do stub drive check
    if (!dir.mkpath(stubDir))
        QSKIP("Required drive not available for this test", SkipSingle);

    {// test without stub, should not be found
    QPluginLoader loader("C:/" + fullStubFileName);
    test1 = !loader.fileName().length();
    }

    // create a stub to defined drive
    QFile tempFile(driveForStubs + fullStubFileName);
    tempFile.open(QIODevice::ReadWrite);
    QFileInfo fileInfo(tempFile);

    {// now should be found even tried to find from C:
    QPluginLoader loader("C:/" + fullStubFileName);
    test2 = (loader.fileName() == fileInfo.absoluteFilePath());
    }

    // clean up
    tempFile.close();
    if (!QFile::remove(driveForStubs + fullStubFileName))
        QWARN("Could not remove stub file");
    if (!dir.rmdir(driveForStubs + stubDir))
        QWARN("Could not remove stub directory");

    // test after cleanup
    QVERIFY(test1);
    QVERIFY(test2);

#endif//Q_OS_SYMBIAN
}

QTEST_APPLESS_MAIN(tst_QPluginLoader)
#include "tst_qpluginloader.moc"
