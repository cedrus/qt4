/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the QtCore module of the Qt Toolkit.
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

#include "qplatformdefs.h"
#include "qfsfileengine_iterator_p.h"

#include <QtCore/qvariant.h>

#ifndef QT_NO_FSFILEENGINE

QT_BEGIN_NAMESPACE

class QFSFileEngineIteratorPlatformSpecificData
{
public:
    inline QFSFileEngineIteratorPlatformSpecificData()
        : dir(0), dirEntry(0), done(false)
#if defined(_POSIX_THREAD_SAFE_FUNCTIONS) && !defined(Q_OS_CYGWIN) && !defined(Q_OS_SYMBIAN)
          , mt_file(0)
#endif
    {}

    QT_DIR *dir;
    QT_DIRENT *dirEntry;
    bool done;

#if defined(_POSIX_THREAD_SAFE_FUNCTIONS) && !defined(Q_OS_CYGWIN) && !defined(Q_OS_SYMBIAN)
    // for readdir_r
    QT_DIRENT *mt_file;
#endif
};

void QFSFileEngineIterator::advance()
{
    currentEntry = platform->dirEntry ? QFile::decodeName(QByteArray(platform->dirEntry->d_name)) : QString();

    if (!platform->dir)
        return;

#if defined(_POSIX_THREAD_SAFE_FUNCTIONS) && !defined(Q_OS_CYGWIN) && !defined(Q_OS_SYMBIAN)
    if (QT_READDIR_R(platform->dir, platform->mt_file, &platform->dirEntry) != 0)
        platform->done = true;
#else
    // ### add local lock to prevent breaking reentrancy
    platform->dirEntry = QT_READDIR(platform->dir);
#endif // _POSIX_THREAD_SAFE_FUNCTIONS
    if (!platform->dirEntry) {
        QT_CLOSEDIR(platform->dir);
        platform->dir = 0;
        platform->done = true;
#if defined(_POSIX_THREAD_SAFE_FUNCTIONS) && !defined(Q_OS_CYGWIN) && !defined(Q_OS_SYMBIAN)
        delete [] platform->mt_file;
        platform->mt_file = 0;
#endif
    }
}

void QFSFileEngineIterator::newPlatformSpecifics()
{
    platform = new QFSFileEngineIteratorPlatformSpecificData;
}

void QFSFileEngineIterator::deletePlatformSpecifics()
{
    if (platform->dir) {
        QT_CLOSEDIR(platform->dir);
#if defined(_POSIX_THREAD_SAFE_FUNCTIONS) && !defined(Q_OS_CYGWIN) && !defined(Q_OS_SYMBIAN)
        delete [] platform->mt_file;
        platform->mt_file = 0;
#endif
    }
    delete platform;
    platform = 0;
}

bool QFSFileEngineIterator::hasNext() const
{
    if (!platform->done && !platform->dir) {
        QFSFileEngineIterator *that = const_cast<QFSFileEngineIterator *>(this);
        if ((that->platform->dir = QT_OPENDIR(QFile::encodeName(path()).data())) == 0) {
            that->platform->done = true;
        } else {
            // ### Race condition; we should use fpathconf and dirfd().
            long maxPathName = ::pathconf(QFile::encodeName(path()).data(), _PC_NAME_MAX);
            if ((int) maxPathName == -1)
                maxPathName = FILENAME_MAX;
            maxPathName += sizeof(QT_DIRENT) + 1;
#if defined(_POSIX_THREAD_SAFE_FUNCTIONS) && !defined(Q_OS_CYGWIN) && !defined(Q_OS_SYMBIAN)
            if (that->platform->mt_file)
                delete [] that->platform->mt_file;
            that->platform->mt_file = (QT_DIRENT *)new char[maxPathName];
#endif

            that->advance();
        }
    }
    return !platform->done;
}

QT_END_NAMESPACE

#endif // QT_NO_FSFILEENGINE
