/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the plugins of the Qt Toolkit.
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

#include "qmeegoruntime.h"

#include <private/qlibrary_p.h>
#include <private/qfactoryloader_p.h>
#include <private/qgraphicssystemplugin_p.h>

#define ENSURE_INITIALIZED {if (!initialized) initialize();}

bool QMeeGoRuntime::initialized = false;

typedef int (*QMeeGoImageToEglSharedImageFunc) (const QImage&);
typedef QPixmapData* (*QMeeGoPixmapDataFromEglSharedImageFunc) (Qt::HANDLE handle, const QImage&);
typedef QPixmapData* (*QMeeGoPixmapDataWithGLTextureFunc) (int w, int h);
typedef bool (*QMeeGoDestroyEGLSharedImageFunc) (Qt::HANDLE handle);
typedef void (*QMeeGoUpdateEglSharedImagePixmapFunc) (QPixmap*);
typedef void (*QMeeGoSetSurfaceFixedSizeFunc) (int w, int h);
typedef void (*QMeeGoSetSurfaceScalingFunc) (int x, int y, int w, int h);
typedef void (*QMeeGoSetTranslucentFunc) (bool translucent);

static QMeeGoImageToEglSharedImageFunc qt_meego_image_to_egl_shared_image = NULL;
static QMeeGoPixmapDataFromEglSharedImageFunc qt_meego_pixmapdata_from_egl_shared_image = NULL;
static QMeeGoPixmapDataWithGLTextureFunc qt_meego_pixmapdata_with_gl_texture = NULL;
static QMeeGoDestroyEGLSharedImageFunc qt_meego_destroy_egl_shared_image = NULL;
static QMeeGoUpdateEglSharedImagePixmapFunc qt_meego_update_egl_shared_image_pixmap = NULL;
static QMeeGoSetSurfaceFixedSizeFunc qt_meego_set_surface_fixed_size = NULL;
static QMeeGoSetSurfaceScalingFunc qt_meego_set_surface_scaling = NULL;
static QMeeGoSetTranslucentFunc qt_meego_set_translucent = NULL;

void QMeeGoRuntime::initialize()
{
    QFactoryLoader loader(QGraphicsSystemFactoryInterface_iid, QLatin1String("/graphicssystems"), Qt::CaseInsensitive);

    QLibraryPrivate *libraryPrivate = loader.library(QLatin1String("meego"));
    Q_ASSERT(libraryPrivate);

    QLibrary library(libraryPrivate->fileName, libraryPrivate->fullVersion);

    bool success = library.load();

    if (success) {
        qt_meego_image_to_egl_shared_image = (QMeeGoImageToEglSharedImageFunc) library.resolve("qt_meego_image_to_egl_shared_image");
        qt_meego_pixmapdata_from_egl_shared_image = (QMeeGoPixmapDataFromEglSharedImageFunc)  library.resolve("qt_meego_pixmapdata_from_egl_shared_image");
        qt_meego_pixmapdata_with_gl_texture = (QMeeGoPixmapDataWithGLTextureFunc) library.resolve("qt_meego_pixmapdata_with_gl_texture");
        qt_meego_destroy_egl_shared_image = (QMeeGoDestroyEGLSharedImageFunc) library.resolve("qt_meego_destroy_egl_shared_image");
        qt_meego_update_egl_shared_image_pixmap = (QMeeGoUpdateEglSharedImagePixmapFunc) library.resolve("qt_meego_update_egl_shared_image_pixmap");
        qt_meego_set_surface_fixed_size = (QMeeGoSetSurfaceFixedSizeFunc) library.resolve("qt_meego_set_surface_fixed_size");
        qt_meego_set_surface_scaling = (QMeeGoSetSurfaceScalingFunc) library.resolve("qt_meego_set_surface_scaling");
        qt_meego_set_translucent = (QMeeGoSetTranslucentFunc) library.resolve("qt_meego_set_translucent");

        if (qt_meego_image_to_egl_shared_image && qt_meego_pixmapdata_from_egl_shared_image && qt_meego_pixmapdata_with_gl_texture
            && qt_meego_destroy_egl_shared_image && qt_meego_update_egl_shared_image_pixmap && qt_meego_set_surface_fixed_size
            && qt_meego_set_surface_scaling && qt_meego_set_translucent)
        {
            qDebug("Successfully resolved MeeGo graphics system: %s %s\n", qPrintable(libraryPrivate->fileName), qPrintable(libraryPrivate->fullVersion));
        }
    } else {
        Q_ASSERT(false);
    }

    initialized = true;
}

Qt::HANDLE QMeeGoRuntime::imageToEGLSharedImage(const QImage &image)
{
    ENSURE_INITIALIZED;
    Q_ASSERT(qt_meego_image_to_egl_shared_image);
    return qt_meego_image_to_egl_shared_image(image);
}

QPixmap QMeeGoRuntime::pixmapFromEGLSharedImage(Qt::HANDLE handle, const QImage &softImage)
{
    ENSURE_INITIALIZED;
    Q_ASSERT(qt_meego_pixmapdata_from_egl_shared_image);
    return QPixmap(qt_meego_pixmapdata_from_egl_shared_image(handle, softImage));
}

QPixmap QMeeGoRuntime::pixmapWithGLTexture(int w, int h)
{
    ENSURE_INITIALIZED;
    Q_ASSERT(qt_meego_pixmapdata_with_gl_texture);
    return QPixmap(qt_meego_pixmapdata_with_gl_texture(w, h));
}

bool QMeeGoRuntime::destroyEGLSharedImage(Qt::HANDLE handle)
{
    ENSURE_INITIALIZED;
    Q_ASSERT(qt_meego_destroy_egl_shared_image);
    return qt_meego_destroy_egl_shared_image(handle);
}

void QMeeGoRuntime::updateEGLSharedImagePixmap(QPixmap *p)
{
    ENSURE_INITIALIZED;
    Q_ASSERT(qt_meego_update_egl_shared_image_pixmap);
    return qt_meego_update_egl_shared_image_pixmap(p);
}

void QMeeGoRuntime::setSurfaceFixedSize(int w, int h)
{
    ENSURE_INITIALIZED;
    Q_ASSERT(qt_meego_set_surface_fixed_size);
    qt_meego_set_surface_fixed_size(w, h);
}

void QMeeGoRuntime::setSurfaceScaling(int x, int y, int w, int h)
{
    ENSURE_INITIALIZED;
    Q_ASSERT(qt_meego_set_surface_scaling);
    qt_meego_set_surface_scaling(x, y, w, h);
}

void QMeeGoRuntime::setTranslucent(bool translucent)
{
    ENSURE_INITIALIZED;
    Q_ASSERT(qt_meego_set_translucent);
    qt_meego_set_translucent(translucent);
}
