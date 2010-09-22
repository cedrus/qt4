/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the QtOpenGL module of the Qt Toolkit.
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

#ifndef QTRIANGULATOR_P_H
#define QTRIANGULATOR_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <QtCore/qvector.h>
#include <QtGui/private/qvectorpath_p.h>

QT_BEGIN_NAMESPACE

#define Q_TRIANGULATE_END_OF_POLYGON quint32(-1)

struct QTriangleSet
{
    inline QTriangleSet() { }
    inline QTriangleSet(const QTriangleSet &other) : vertices(other.vertices), indices(other.indices) { }
    QTriangleSet &operator = (const QTriangleSet &other) {vertices = other.vertices; indices = other.indices; return *this;}

    // The vertices of a triangle are given by: (x[i[n]], y[i[n]]), (x[j[n]], y[j[n]]), (x[k[n]], y[k[n]]), n = 0, 1, ...
    QVector<qreal> vertices; // [x[0], y[0], x[1], y[1], x[2], ...]
    QVector<quint32> indices; // [i[0], j[0], k[0], i[1], j[1], k[1], i[2], ...]
};

struct QPolylineSet
{
    inline QPolylineSet() { }
    inline QPolylineSet(const QPolylineSet &other) : vertices(other.vertices), indices(other.indices) { }
    QPolylineSet &operator = (const QPolylineSet &other) {vertices = other.vertices; indices = other.indices; return *this;}

    QVector<qreal> vertices; // [x[0], y[0], x[1], y[1], x[2], ...]
    QVector<quint32> indices;

};

// The vertex coordinates of the returned triangle set will be rounded to a grid with a mesh size
// of 1/32. The polygon is first transformed, then scaled by 32, the coordinates are rounded to
// integers, the polygon is triangulated, and then scaled back by 1/32.
// 'hint' should be a combination of QVectorPath::Hints.
// 'lod' is the level of detail. Default is 1. Curves are split into more lines when 'lod' is higher.
QTriangleSet qTriangulate(const qreal *polygon, int count, uint hint = QVectorPath::PolygonHint | QVectorPath::OddEvenFill, const QTransform &matrix = QTransform());
QTriangleSet qTriangulate(const QVectorPath &path, const QTransform &matrix = QTransform(), qreal lod = 1);
QTriangleSet qTriangulate(const QPainterPath &path, const QTransform &matrix = QTransform(), qreal lod = 1);
QPolylineSet qPolyline(const QVectorPath &path, const QTransform &matrix = QTransform(), qreal lod = 1);
QPolylineSet qPolyline(const QPainterPath &path, const QTransform &matrix = QTransform(), qreal lod = 1);

QT_END_NAMESPACE

#endif
