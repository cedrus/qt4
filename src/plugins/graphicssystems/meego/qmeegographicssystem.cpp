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

#include <QDebug>
#include <private/qpixmap_raster_p.h>
#include <private/qwindowsurface_gl_p.h>
#include <private/qegl_p.h>
#include <private/qglextensions_p.h>
#include <private/qgl_p.h>
#include <private/qimagepixmapcleanuphooks_p.h>
#include <private/qapplication_p.h>
#include <private/qgraphicssystem_runtime_p.h>
#include <private/qimage_p.h>
#include <private/qeglproperties_p.h>
#include <private/qeglcontext_p.h>

#include "qmeegopixmapdata.h"
#include "qmeegographicssystem.h"
#include "qmeegoextensions.h"

bool QMeeGoGraphicsSystem::surfaceWasCreated = false;

QMeeGoGraphicsSystem::QMeeGoGraphicsSystem()
{
    qDebug("Using the meego graphics system");
}

QMeeGoGraphicsSystem::~QMeeGoGraphicsSystem()
{
    qDebug("Meego graphics system destroyed");
    qt_destroy_gl_share_widget();
}

QWindowSurface* QMeeGoGraphicsSystem::createWindowSurface(QWidget *widget) const
{
    QMeeGoGraphicsSystem::surfaceWasCreated = true;
    QWindowSurface *surface = new QGLWindowSurface(widget);
    return surface;
}

QPixmapData *QMeeGoGraphicsSystem::createPixmapData(QPixmapData::PixelType type) const
{
    // Long story short: without this it's possible to hit an 
    // unitialized paintDevice due to a Qt bug too complex to even 
    // explain here... not to mention fix without going crazy. 
    // MDK
    QGLShareContextScope ctx(qt_gl_share_widget()->context());
 
    return new QRasterPixmapData(type);
}

QPixmapData *QMeeGoGraphicsSystem::createPixmapData(QPixmapData *origin)
{
    // If the pixmap is a raster type...
    // and if the pixmap pointer matches our mapping...
    // create a shared image instead with the given handle.

    if (origin->classId() == QPixmapData::RasterClass) {
        QRasterPixmapData *rasterClass = static_cast <QRasterPixmapData *> (origin);
        void *rawResource = static_cast <void *> (rasterClass->buffer()->data_ptr()->data);

        if (QMeeGoPixmapData::sharedImagesMap.contains(rawResource))
            return new QMeeGoPixmapData();
    } 
        
    return new QRasterPixmapData(origin->pixelType());
}

QPixmapData* QMeeGoGraphicsSystem::wrapPixmapData(QPixmapData *pmd)
{
    QString name = QApplicationPrivate::instance()->graphics_system_name;
    if (name == "runtime") {
        QRuntimeGraphicsSystem *rsystem = (QRuntimeGraphicsSystem *) QApplicationPrivate::instance()->graphics_system;
        QRuntimePixmapData *rt = new QRuntimePixmapData(rsystem, pmd->pixelType());;
        rt->m_data = pmd;
        rt->readBackInfo();
        rsystem->m_pixmapDatas << rt;
        return rt;
    } else
        return pmd;
}

void QMeeGoGraphicsSystem::setSurfaceFixedSize(int /*width*/, int /*height*/)
{
    if (QMeeGoGraphicsSystem::surfaceWasCreated)
        qWarning("Trying to set surface fixed size but surface already created!");

#ifdef QT_WAS_PATCHED
    QEglProperties *properties = new QEglProperties();
    properties->setValue(EGL_FIXED_WIDTH_NOK, width);
    properties->setValue(EGL_FIXED_HEIGHT_NOK, height);
    QGLContextPrivate::setExtraWindowSurfaceCreationProps(properties);
#endif
}

void QMeeGoGraphicsSystem::setSurfaceScaling(int x, int y, int width, int height)
{
    QMeeGoExtensions::ensureInitialized();
    QMeeGoExtensions::eglSetSurfaceScalingNOK(QEgl::display(), QEglContext::currentContext(QEgl::OpenGL)->currentSurface, x, y, width, height);
}

void QMeeGoGraphicsSystem::setTranslucent(bool translucent)
{
    QGLWindowSurface::surfaceFormat.setSampleBuffers(false);
    QGLWindowSurface::surfaceFormat.setSamples(0);
    QGLWindowSurface::surfaceFormat.setAlpha(translucent);
}

QPixmapData *QMeeGoGraphicsSystem::pixmapDataFromEGLSharedImage(Qt::HANDLE handle, const QImage &softImage)
{
    if (softImage.format() != QImage::Format_ARGB32_Premultiplied &&
        softImage.format() != QImage::Format_ARGB32) {
        qFatal("For egl shared images, the soft image has to be ARGB32 or ARGB32_Premultiplied");
        return NULL;
    }
    
    if (QMeeGoGraphicsSystem::meeGoRunning()) {
        QMeeGoPixmapData *pmd = new QMeeGoPixmapData;
        pmd->fromEGLSharedImage(handle, softImage);
        return QMeeGoGraphicsSystem::wrapPixmapData(pmd);
    } else {
        QRasterPixmapData *pmd = new QRasterPixmapData(QPixmapData::PixmapType);
        pmd->fromImage(softImage, Qt::NoOpaqueDetection);

        // Make sure that the image was not converted in any way
        if (pmd->buffer()->data_ptr()->data !=
            const_cast<QImage &>(softImage).data_ptr()->data)
            qFatal("Iternal misalignment of raster data detected. Prolly a QImage copy fail.");

        QMeeGoPixmapData::registerSharedImage(handle, softImage);
        return QMeeGoGraphicsSystem::wrapPixmapData(pmd);
    }
}

void QMeeGoGraphicsSystem::updateEGLSharedImagePixmap(QPixmap *pixmap)
{
    QMeeGoPixmapData *pmd = (QMeeGoPixmapData *) pixmap->pixmapData();
    
    // Basic sanity check to make sure this is really a QMeeGoPixmapData...
    if (pmd->classId() != QPixmapData::OpenGLClass) 
        qFatal("Trying to updated EGLSharedImage pixmap but it's not really a shared image pixmap!");

    pmd->updateFromSoftImage();
}

QPixmapData *QMeeGoGraphicsSystem::pixmapDataWithGLTexture(int w, int h)
{
    QGLPixmapData *pmd = new QGLPixmapData(QPixmapData::PixmapType);
    pmd->resize(w, h);
    return QMeeGoGraphicsSystem::wrapPixmapData(pmd);
}

bool QMeeGoGraphicsSystem::meeGoRunning()
{
    if (! QApplicationPrivate::instance()) {
        qWarning("Application not running just yet... hard to know what system running!");
        return false;
    }

    QString name = QApplicationPrivate::instance()->graphics_system_name;
    if (name == "runtime") {
        QRuntimeGraphicsSystem *rsystem = (QRuntimeGraphicsSystem *) QApplicationPrivate::instance()->graphics_system;
        name = rsystem->graphicsSystemName();
    }

    return (name == "meego");
}

/* C API */

int qt_meego_image_to_egl_shared_image(const QImage &image)
{
    return QMeeGoPixmapData::imageToEGLSharedImage(image);
}

QPixmapData* qt_meego_pixmapdata_from_egl_shared_image(Qt::HANDLE handle, const QImage &softImage)
{
    return QMeeGoGraphicsSystem::pixmapDataFromEGLSharedImage(handle, softImage);
}

QPixmapData* qt_meego_pixmapdata_with_gl_texture(int w, int h)
{
    return QMeeGoGraphicsSystem::pixmapDataWithGLTexture(w, h);
}

bool qt_meego_destroy_egl_shared_image(Qt::HANDLE handle)
{
    return QMeeGoPixmapData::destroyEGLSharedImage(handle);
}

void qt_meego_set_surface_fixed_size(int width, int height)
{
    QMeeGoGraphicsSystem::setSurfaceFixedSize(width, height);
}

void qt_meego_set_surface_scaling(int x, int y, int width, int height)
{
    QMeeGoGraphicsSystem::setSurfaceScaling(x, y, width, height);
}

void qt_meego_set_translucent(bool translucent)
{
    QMeeGoGraphicsSystem::setTranslucent(translucent);
}

void qt_meego_update_egl_shared_image_pixmap(QPixmap *pixmap)
{
    QMeeGoGraphicsSystem::updateEGLSharedImagePixmap(pixmap);
}
