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

#include "qmeegopixmapdata.h"
#include "qmeegoextensions.h"
#include <private/qimage_p.h>
#include <private/qwindowsurface_gl_p.h>
#include <private/qeglcontext_p.h>
#include <private/qapplication_p.h>
#include <private/qgraphicssystem_runtime_p.h>

static EGLint preserved_image_attribs[] = { EGL_IMAGE_PRESERVED_KHR, EGL_TRUE, EGL_NONE };

QHash <void*, QMeeGoImageInfo*> QMeeGoPixmapData::sharedImagesMap;

/* Public */

QMeeGoPixmapData::QMeeGoPixmapData() : QGLPixmapData(QPixmapData::PixmapType)
{
}

void QMeeGoPixmapData::fromTexture(GLuint textureId, int w, int h, bool alpha)
{
    resize(w, h); 
    texture()->id = textureId;
    m_hasAlpha = alpha;
    softImage = QImage();
}

QImage QMeeGoPixmapData::toImage() const
{
    return softImage;
}

void QMeeGoPixmapData::fromImage(const QImage &image,
                                 Qt::ImageConversionFlags flags)
{
    void *rawResource = static_cast <void *> (((QImage &) image).data_ptr()->data);

    if (sharedImagesMap.contains(rawResource)) {
        QMeeGoImageInfo *info = sharedImagesMap.value(rawResource);
        fromEGLSharedImage(info->handle, image);
    } else {
        // This should *never* happen since the graphics system should never
        // create a QMeeGoPixmapData for an origin that doesn't contain a raster
        // image we know about. But...
        qWarning("QMeeGoPixmapData::fromImage called on non-know resource. Falling back...");
        QGLPixmapData::fromImage(image, flags);
    }
}

void QMeeGoPixmapData::fromEGLSharedImage(Qt::HANDLE handle, const QImage &si)
{
    if (si.isNull())
        qFatal("Trying to build pixmap with an empty/null softimage!");
        
    QGLShareContextScope ctx(qt_gl_share_widget()->context());
   
    QMeeGoExtensions::ensureInitialized();
 
    bool textureIsBound = false;
    GLuint newTextureId;
    GLint newWidth, newHeight;

    glGenTextures(1, &newTextureId);
    glBindTexture(GL_TEXTURE_2D, newTextureId);
    
    glFinish();
    EGLImageKHR image = QEgl::eglCreateImageKHR(QEgl::display(), EGL_NO_CONTEXT, EGL_SHARED_IMAGE_NOK,
                                                (EGLClientBuffer)handle, preserved_image_attribs);

    if (image != EGL_NO_IMAGE_KHR) {
        glFinish();
        glEGLImageTargetTexture2DOES(GL_TEXTURE_2D, image);
        GLint err = glGetError();
        if (err == GL_NO_ERROR)
            textureIsBound = true;
        
        QMeeGoExtensions::eglQueryImageNOK(QEgl::display(), image, EGL_WIDTH, &newWidth);
        QMeeGoExtensions::eglQueryImageNOK(QEgl::display(), image, EGL_HEIGHT, &newHeight);
          
        QEgl::eglDestroyImageKHR(QEgl::display(), image);
        glFinish();
    }
        
    if (textureIsBound) {
        // FIXME Remove this ugly hasAlphaChannel check when Qt lands the NoOpaqueCheck flag fix
        // for QGLPixmapData.
        fromTexture(newTextureId, newWidth, newHeight, 
                    (si.hasAlphaChannel() && const_cast<QImage &>(si).data_ptr()->checkForAlphaPixels()));
        softImage = si;
        QMeeGoPixmapData::registerSharedImage(handle, softImage);
    } else {
        qWarning("Failed to create a texture from a shared image!");
        glDeleteTextures(1, &newTextureId);
    }
}

Qt::HANDLE QMeeGoPixmapData::imageToEGLSharedImage(const QImage &image)
{
    QGLShareContextScope ctx(qt_gl_share_widget()->context());

    QMeeGoExtensions::ensureInitialized();

    glFinish();
    QGLPixmapData pixmapData(QPixmapData::PixmapType);
    pixmapData.fromImage(image, 0);
    GLuint textureId = pixmapData.bind();

    glFinish();
    EGLImageKHR eglimage = QEgl::eglCreateImageKHR(QEgl::display(), QEglContext::currentContext(QEgl::OpenGL)->context(),
                                                                                                EGL_GL_TEXTURE_2D_KHR,
                                                                                                (EGLClientBuffer) textureId,
                                                                                                preserved_image_attribs);
    glFinish();

    if (eglimage) {
        EGLNativeSharedImageTypeNOK handle = QMeeGoExtensions::eglCreateSharedImageNOK(QEgl::display(), eglimage, NULL);
        QEgl::eglDestroyImageKHR(QEgl::display(), eglimage);
        glFinish();
        return (Qt::HANDLE) handle;
    } else {
        qWarning("Failed to create shared image from pixmap/texture!");
        return 0;
    }
}

void QMeeGoPixmapData::updateFromSoftImage()
{
    m_dirty = true;
    m_source = softImage;
    ensureCreated();
    
    if (softImage.width() != w || softImage.height() != h)
        qWarning("Ooops, looks like softImage changed dimensions since last updated! Corruption ahead?!");
}

bool QMeeGoPixmapData::destroyEGLSharedImage(Qt::HANDLE h)
{
    QGLShareContextScope ctx(qt_gl_share_widget()->context());   
    QMeeGoExtensions::ensureInitialized();

    QMutableHashIterator <void*, QMeeGoImageInfo*> i(sharedImagesMap);
    while (i.hasNext()) {
        i.next();
        if (i.value()->handle == h)
            i.remove();
    }

    return QMeeGoExtensions::eglDestroySharedImageNOK(QEgl::display(), (EGLNativeSharedImageTypeNOK) h);
}

void QMeeGoPixmapData::registerSharedImage(Qt::HANDLE handle, const QImage &si)
{
    void *raw = static_cast <void *> (((QImage) si).data_ptr()->data);
    QMeeGoImageInfo *info;
    
    if (! sharedImagesMap.contains(raw)) {
        info = new QMeeGoImageInfo;
        info->handle = handle;
        info->rawFormat = si.format();
        sharedImagesMap.insert(raw, info);
    } else {
        info = sharedImagesMap.value(raw);
        if (info->handle != handle || info->rawFormat != si.format())
            qWarning("Inconsistency detected: overwriting entry in sharedImagesMap but handle/format different");
    }
}
