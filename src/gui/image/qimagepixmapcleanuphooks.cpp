/****************************************************************************
**
** Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the QtGui module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** No Commercial Usage
** This file contains pre-release code and may not be distributed.
** You may use this file in accordance with the terms and conditions
** contained in the Technology Preview License Agreement accompanying
** this package.
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
** If you have questions regarding the use of this file, please contact
** Nokia at qt-info@nokia.com.
**
**
**
**
**
**
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "qimagepixmapcleanuphooks_p.h"
#include "qpixmapdata_p.h"


QT_BEGIN_NAMESPACE

// Legacy, single instance hooks: ### Qt 5: remove
typedef void (*_qt_pixmap_cleanup_hook)(int);
typedef void (*_qt_pixmap_cleanup_hook_64)(qint64);
typedef void (*_qt_image_cleanup_hook)(int);
Q_GUI_EXPORT _qt_pixmap_cleanup_hook qt_pixmap_cleanup_hook = 0;
Q_GUI_EXPORT _qt_pixmap_cleanup_hook_64 qt_pixmap_cleanup_hook_64 = 0;
Q_GUI_EXPORT _qt_image_cleanup_hook qt_image_cleanup_hook = 0;
Q_GUI_EXPORT _qt_image_cleanup_hook_64 qt_image_cleanup_hook_64 = 0;


QImagePixmapCleanupHooks* qt_image_and_pixmap_cleanup_hooks = 0;


QImagePixmapCleanupHooks::QImagePixmapCleanupHooks()
{
    qt_image_and_pixmap_cleanup_hooks = this;
}

QImagePixmapCleanupHooks *QImagePixmapCleanupHooks::instance()
{
    if (!qt_image_and_pixmap_cleanup_hooks)
        qt_image_and_pixmap_cleanup_hooks = new QImagePixmapCleanupHooks;
    return qt_image_and_pixmap_cleanup_hooks;
}

void QImagePixmapCleanupHooks::addPixmapHook(_qt_pixmap_cleanup_hook_pm hook)
{
    pixmapHooks.append(hook);
}

void QImagePixmapCleanupHooks::addImageHook(_qt_image_cleanup_hook_64 hook)
{
    imageHooks.append(hook);
}

void QImagePixmapCleanupHooks::removePixmapHook(_qt_pixmap_cleanup_hook_pm hook)
{
    pixmapHooks.removeAll(hook);
}

void QImagePixmapCleanupHooks::removeImageHook(_qt_image_cleanup_hook_64 hook)
{
    imageHooks.removeAll(hook);
}


void QImagePixmapCleanupHooks::executePixmapHooks(QPixmap* pm)
{
    for (int i = 0; i < qt_image_and_pixmap_cleanup_hooks->pixmapHooks.count(); ++i)
        qt_image_and_pixmap_cleanup_hooks->pixmapHooks[i](pm);

    if (qt_pixmap_cleanup_hook_64)
        qt_pixmap_cleanup_hook_64(pm->cacheKey());
}


void QImagePixmapCleanupHooks::executeImageHooks(qint64 key)
{
    for (int i = 0; i < qt_image_and_pixmap_cleanup_hooks->imageHooks.count(); ++i)
        qt_image_and_pixmap_cleanup_hooks->imageHooks[i](key);

    if (qt_image_cleanup_hook_64)
        qt_image_cleanup_hook_64(key);
}

QT_END_NAMESPACE