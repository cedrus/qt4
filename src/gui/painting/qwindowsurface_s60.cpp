/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the QtGui module of the Qt Toolkit.
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

#include <qglobal.h> // for Q_WS_WIN define (non-PCH)

#include <QtGui/qpaintdevice.h>
#include <private/qwidget_p.h>
#include <private/qwindowsurface_s60_p.h>
#include <private/qpixmap_s60_p.h>
#include <private/qt_s60_p.h>
#include <private/qapplication_p.h>
#include <private/qdrawhelper_p.h>

#ifdef QT_GRAPHICSSYSTEM_RUNTIME
#include <private/qgraphicssystem_runtime_p.h>
#endif

QT_BEGIN_NAMESPACE

struct QS60WindowSurfacePrivate
{
    QPixmap device;
    QList<QImage*> bufferImages;
};

QS60WindowSurface::QS60WindowSurface(QWidget* widget)
    : QWindowSurface(widget), d_ptr(new QS60WindowSurfacePrivate)
{

    TDisplayMode mode = S60->screenDevice()->DisplayMode();
    bool isOpaque = qt_widget_private(widget)->isOpaque;
    if (isOpaque) {
        mode = EColor16MU;
    } else  {
        if (QSysInfo::symbianVersion() >= QSysInfo::SV_SF_3)
            mode = Q_SYMBIAN_ECOLOR16MAP; // Symbian^3 WServ has support for ARGB32_PRE
        else
            mode = EColor16MA; // Symbian prior to Symbian^3 sw accelerates EColor16MA
    }

    // We create empty CFbsBitmap here -> it will be resized in setGeometry
    CFbsBitmap *bitmap = q_check_ptr(new CFbsBitmap);	// CBase derived object needs check on new
    qt_symbian_throwIfError( bitmap->Create( TSize(0, 0), mode ) );

    QS60PixmapData *data = new QS60PixmapData(QPixmapData::PixmapType);
    if (data) {
        data->fromSymbianBitmap(bitmap, true);
        d_ptr->device = QPixmap(data);
    }

    setStaticContentsSupport(true);
}

QS60WindowSurface::~QS60WindowSurface()
{
#if defined(QT_GRAPHICSSYSTEM_RUNTIME) && defined(Q_SYMBIAN_SUPPORTS_SURFACES)
    if(QApplicationPrivate::runtime_graphics_system) {
        QRuntimeGraphicsSystem *runtimeGraphicsSystem =
                static_cast<QRuntimeGraphicsSystem*>(QApplicationPrivate::graphics_system);
        if(runtimeGraphicsSystem->graphicsSystemName() == QLatin1String("openvg")) {

            // Graphics system has been switched from raster to openvg.
            // Issue empty redraw to clear the UI surface

            QWidget *w = window();
            RWindow *const window = static_cast<RWindow *>(w->winId()->DrawableWindow());
            window->BeginRedraw();
            window->EndRedraw();
        }
    }
#endif

    delete d_ptr;
}

void QS60WindowSurface::beginPaint(const QRegion &rgn)
{
#ifdef Q_SYMBIAN_SUPPORTS_SURFACES
    S60->wsSession().Finish();
#endif

    if (!qt_widget_private(window())->isOpaque) {
        QS60PixmapData *pixmapData = static_cast<QS60PixmapData *>(d_ptr->device.data_ptr().data());
        pixmapData->beginDataAccess();

        QPainter p(&pixmapData->image);
        p.setCompositionMode(QPainter::CompositionMode_Source);
        const QVector<QRect> rects = rgn.rects();
        const QColor blank = Qt::transparent;
        for (QVector<QRect>::const_iterator it = rects.begin(); it != rects.end(); ++it) {
            p.fillRect(*it, blank);
        }

        pixmapData->endDataAccess();
    }
}

void QS60WindowSurface::endPaint(const QRegion &)
{
    qDeleteAll(d_ptr->bufferImages);
    d_ptr->bufferImages.clear();
}

QImage* QS60WindowSurface::buffer(const QWidget *widget)
{
    if (widget->window() != window())
        return 0;

    QPaintDevice *pdev = paintDevice();
    if (!pdev)
        return 0;

    const QPoint off = offset(widget);
    QImage *img = &(static_cast<QS60PixmapData *>(d_ptr->device.data_ptr().data())->image);

    QRect rect(off, widget->size());
    rect &= QRect(QPoint(), img->size());

    if (rect.isEmpty())
        return 0;

    img = new QImage(img->scanLine(rect.y()) + rect.x() * img->depth() / 8,
                     rect.width(), rect.height(),
                     img->bytesPerLine(), img->format());
    d_ptr->bufferImages.append(img);

    return img;
}

void QS60WindowSurface::flush(QWidget *widget, const QRegion &region, const QPoint &)
{
    QWidget *window = widget->window();
    Q_ASSERT(window);
    QTLWExtra *topExtra = window->d_func()->maybeTopData();
    Q_ASSERT(topExtra);
    QRect qr = region.boundingRect();
    if (!topExtra->inExpose) {
        topExtra->inExpose = true; // Prevent DrawNow() from calling syncBackingStore() again
        TRect tr = qt_QRect2TRect(qr);
        widget->winId()->DrawNow(tr);
        topExtra->inExpose = false;
    } else {
        // This handles the case when syncBackingStore updates content outside of the
        // original drawing rectangle. This might happen if there are pending update()
        // events at the same time as we get a Draw() from Symbian.
        QRect drawRect = qt_TRect2QRect(widget->winId()->DrawableWindow()->GetDrawRect());
        if (!drawRect.contains(qr))
            widget->winId()->DrawDeferred();
    }
}

bool QS60WindowSurface::scroll(const QRegion &area, int dx, int dy)
{
    QRect rect = area.boundingRect();

    if (dx == 0 && dy == 0)
        return false;

    if (d_ptr->device.isNull())
        return false;

    QS60PixmapData *data = static_cast<QS60PixmapData*>(d_ptr->device.data_ptr().data());
    data->scroll(dx, dy, rect);

    return true;
}

QPaintDevice* QS60WindowSurface::paintDevice()
{
    return &d_ptr->device;
}

void QS60WindowSurface::setGeometry(const QRect& rect)
{
    if (rect == geometry())
        return;

    QS60PixmapData *data = static_cast<QS60PixmapData*>(d_ptr->device.data_ptr().data());
    data->resize(rect.width(), rect.height());

    QWindowSurface::setGeometry(rect);
}

CFbsBitmap* QS60WindowSurface::symbianBitmap() const
{
    QS60PixmapData *data = static_cast<QS60PixmapData*>(d_ptr->device.data_ptr().data());
    return data->cfbsBitmap;
}

QT_END_NAMESPACE

