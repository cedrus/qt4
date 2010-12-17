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

#ifndef MGRAPHICSSYSTEM_H
#define MGRAPHICSSYSTEM_H

#include <private/qgraphicssystem_p.h>

class QMeeGoGraphicsSystem : public QGraphicsSystem
{
public:
    QMeeGoGraphicsSystem();
    ~QMeeGoGraphicsSystem();

    virtual QWindowSurface *createWindowSurface(QWidget *widget) const;
    virtual QPixmapData *createPixmapData(QPixmapData::PixelType) const;
    virtual QPixmapData *createPixmapData(QPixmapData *origin);

    static QPixmapData *wrapPixmapData(QPixmapData *pmd);
    static void setSurfaceFixedSize(int width, int height);
    static void setSurfaceScaling(int x, int y, int width, int height);
    static void setTranslucent(bool translucent);

    static QPixmapData *pixmapDataFromEGLSharedImage(Qt::HANDLE handle, const QImage &softImage);
    static QPixmapData *pixmapDataWithGLTexture(int w, int h);
    static void updateEGLSharedImagePixmap(QPixmap *pixmap);

private:
    static bool meeGoRunning();

    static bool surfaceWasCreated;
};

/* C api */

extern "C" {
    Q_DECL_EXPORT int qt_meego_image_to_egl_shared_image(const QImage &image);
    Q_DECL_EXPORT QPixmapData* qt_meego_pixmapdata_from_egl_shared_image(Qt::HANDLE handle, const QImage &softImage);
    Q_DECL_EXPORT QPixmapData* qt_meego_pixmapdata_with_gl_texture(int w, int h);
    Q_DECL_EXPORT void qt_meego_update_egl_shared_image_pixmap(QPixmap *pixmap);
    Q_DECL_EXPORT bool qt_meego_destroy_egl_shared_image(Qt::HANDLE handle);
    Q_DECL_EXPORT void qt_meego_set_surface_fixed_size(int width, int height);
    Q_DECL_EXPORT void qt_meego_set_surface_scaling(int x, int y, int width, int height);
    Q_DECL_EXPORT void qt_meego_set_translucent(bool translucent);
}

#endif 
