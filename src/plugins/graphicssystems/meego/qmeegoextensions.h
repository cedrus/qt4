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

#ifndef MEXTENSIONS_H
#define MEXTENSIONS_H

#include <private/qgl_p.h>
#include <private/qeglcontext_p.h>
#include <private/qpixmapdata_gl_p.h>
#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

/* Extensions decls */

#ifndef EGL_SHARED_IMAGE_NOK
#define EGL_SHARED_IMAGE_NOK 0x30DA
typedef void* EGLNativeSharedImageTypeNOK;
#endif

#ifndef EGL_GL_TEXTURE_2D_KHR
#define EGL_GL_TEXTURE_2D_KHR 0x30B1
#endif

#ifndef EGL_FIXED_WIDTH_NOK
#define EGL_FIXED_WIDTH_NOK 0x30DB
#define EGL_FIXED_HEIGHT_NOK 0x30DC
#endif

/* Class */

class QMeeGoExtensions
{
public:
    static void ensureInitialized();

    static EGLNativeSharedImageTypeNOK eglCreateSharedImageNOK(EGLDisplay dpy, EGLImageKHR image, EGLint *props);
    static bool eglQueryImageNOK(EGLDisplay dpy, EGLImageKHR image, EGLint prop, EGLint *v);
    static bool eglDestroySharedImageNOK(EGLDisplay dpy, EGLNativeSharedImageTypeNOK img);
    static bool eglSetSurfaceScalingNOK(EGLDisplay dpy, EGLSurface surface, int x, int y, int width, int height);

private:
    static void initialize();

    static bool initialized;
    static bool hasImageShared;
    static bool hasSurfaceScaling;
};

#endif
