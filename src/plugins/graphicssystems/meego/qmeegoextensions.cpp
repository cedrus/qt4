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

#include "qmeegoextensions.h"
#include <private/qeglcontext_p.h>
#include <private/qpixmapdata_gl_p.h>

bool QMeeGoExtensions::initialized = false;
bool QMeeGoExtensions::hasImageShared = false;
bool QMeeGoExtensions::hasSurfaceScaling = false;

/* Extension funcs */

typedef EGLBoolean (EGLAPIENTRY *eglQueryImageNOKFunc)(EGLDisplay, EGLImageKHR, EGLint, EGLint*);
typedef EGLNativeSharedImageTypeNOK (EGLAPIENTRY *eglCreateSharedImageNOKFunc)(EGLDisplay, EGLImageKHR, EGLint*);
typedef EGLBoolean (EGLAPIENTRY *eglDestroySharedImageNOKFunc)(EGLDisplay, EGLNativeSharedImageTypeNOK);
typedef EGLBoolean (EGLAPIENTRY *eglSetSurfaceScalingNOKFunc)(EGLDisplay, EGLSurface, EGLint, EGLint, EGLint, EGLint);

static eglQueryImageNOKFunc _eglQueryImageNOK = 0;
static eglCreateSharedImageNOKFunc _eglCreateSharedImageNOK = 0;
static eglDestroySharedImageNOKFunc _eglDestroySharedImageNOK = 0;
static eglSetSurfaceScalingNOKFunc _eglSetSurfaceScalingNOK = 0;

/* Public */

void QMeeGoExtensions::ensureInitialized()
{
    if (!initialized)
        initialize();

    initialized = true;
}

EGLNativeSharedImageTypeNOK QMeeGoExtensions::eglCreateSharedImageNOK(EGLDisplay dpy, EGLImageKHR image, EGLint *props)
{
    if (! hasImageShared)
        qFatal("EGL_NOK_image_shared not found but trying to use capability!");
        
    return _eglCreateSharedImageNOK(dpy, image, props);
}

bool QMeeGoExtensions::eglQueryImageNOK(EGLDisplay dpy, EGLImageKHR image, EGLint prop, EGLint *v)
{
    if (! hasImageShared)
        qFatal("EGL_NOK_image_shared not found but trying to use capability!");

    return _eglQueryImageNOK(dpy, image, prop, v);
}

bool QMeeGoExtensions::eglDestroySharedImageNOK(EGLDisplay dpy, EGLNativeSharedImageTypeNOK img)
{
    if (! hasImageShared)
        qFatal("EGL_NOK_image_shared not found but trying to use capability!");

    return _eglDestroySharedImageNOK(dpy, img);
}

bool QMeeGoExtensions::eglSetSurfaceScalingNOK(EGLDisplay dpy, EGLSurface surface, int x, int y, int width, int height)
{
    if (! hasSurfaceScaling)
        qFatal("EGL_NOK_surface_scaling not found but trying to use capability!");

   return _eglSetSurfaceScalingNOK(dpy, surface, x, y, width, height);
}

/* Private */

void QMeeGoExtensions::initialize()
{
    QGLContext *ctx = (QGLContext *) QGLContext::currentContext();
    qt_resolve_eglimage_gl_extensions(ctx);
    
    if (QEgl::hasExtension("EGL_NOK_image_shared")) {
        qDebug("MeegoGraphics: found EGL_NOK_image_shared");
        _eglQueryImageNOK = (eglQueryImageNOKFunc) eglGetProcAddress("eglQueryImageNOK");
        _eglCreateSharedImageNOK = (eglCreateSharedImageNOKFunc) eglGetProcAddress("eglCreateSharedImageNOK");
        _eglDestroySharedImageNOK = (eglDestroySharedImageNOKFunc) eglGetProcAddress("eglDestroySharedImageNOK");
        
        Q_ASSERT(_eglQueryImageNOK && _eglCreateSharedImageNOK && _eglDestroySharedImageNOK);
        hasImageShared = true;
    }
    
    if (QEgl::hasExtension("EGL_NOK_surface_scaling")) {
        qDebug("MeegoGraphics: found EGL_NOK_surface_scaling");
        _eglSetSurfaceScalingNOK = (eglSetSurfaceScalingNOKFunc) eglGetProcAddress("eglSetSurfaceScalingNOK");
        
        Q_ASSERT(_eglSetSurfaceScalingNOK);
        hasSurfaceScaling = true;
    }
}

