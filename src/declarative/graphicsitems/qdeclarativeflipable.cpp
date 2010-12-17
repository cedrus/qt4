/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the QtDeclarative module of the Qt Toolkit.
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

#include "private/qdeclarativeflipable_p.h"

#include "private/qdeclarativeitem_p.h"
#include "private/qdeclarativeguard_p.h"

#include <qdeclarativeinfo.h>

#include <QtGui/qgraphicstransform.h>

QT_BEGIN_NAMESPACE

class QDeclarativeFlipablePrivate : public QDeclarativeItemPrivate
{
    Q_DECLARE_PUBLIC(QDeclarativeFlipable)
public:
    QDeclarativeFlipablePrivate() : current(QDeclarativeFlipable::Front), front(0), back(0) {}

    void updateSceneTransformFromParent();
    void setBackTransform();

    QDeclarativeFlipable::Side current;
    QDeclarativeGuard<QGraphicsObject> front;
    QDeclarativeGuard<QGraphicsObject> back;

    bool wantBackXFlipped;
    bool wantBackYFlipped;
};

/*!
    \qmlclass Flipable QDeclarativeFlipable
    \since 4.7
    \ingroup qml-basic-interaction-elements
    \brief The Flipable item provides a surface that can be flipped.
    \inherits Item

    Flipable is an item that can be visibly "flipped" between its front and
    back sides, like a card. It is used together with \l Rotation, \l State
    and \l Transition elements to produce a flipping effect.

    The \l front and \l back properties are used to hold the items that are
    shown respectively on the front and back sides of the flipable item.

    \section1 Example Usage

    \beginfloatright
    \inlineimage flipable.gif
    \endfloat

    The following example shows a Flipable item that flips whenever it is
    clicked, rotating about the y-axis.

    The \l Rotation element is used to specify the angle and axis of the flip.
    When \c flipped is true, the item changes to the "back" state, where
    the angle is changed to 180 degrees to produce the flipping effect. 

    \clearfloat
    \snippet doc/src/snippets/declarative/flipable/flipable-snippet.qml 0

    The \l Transition creates the animation that changes the angle over the
    duration of one second. When the item changes between its "back" and
    default states, the NumberAnimation animates the angle between
    its old and new values.

    See the \l {QML States} and \l {QML Animation} documentation for more
    details on state changes and how animations work within transitions.

    \sa {declarative/ui-components/flipable}{Flipable example}
*/

QDeclarativeFlipable::QDeclarativeFlipable(QDeclarativeItem *parent)
: QDeclarativeItem(*(new QDeclarativeFlipablePrivate), parent)
{
}

QDeclarativeFlipable::~QDeclarativeFlipable()
{
}

/*!
  \qmlproperty Item Flipable::front
  \qmlproperty Item Flipable::back

  The front and back sides of the flipable.
*/

QGraphicsObject *QDeclarativeFlipable::front()
{
    Q_D(const QDeclarativeFlipable);
    return d->front;
}

void QDeclarativeFlipable::setFront(QGraphicsObject *front)
{
    Q_D(QDeclarativeFlipable);
    if (d->front) {
        qmlInfo(this) << tr("front is a write-once property");
        return;
    }
    d->front = front;
    d->front->setParentItem(this);
    if (Back == d->current)
        d->front->setOpacity(0.);
}

QGraphicsObject *QDeclarativeFlipable::back()
{
    Q_D(const QDeclarativeFlipable);
    return d->back;
}

void QDeclarativeFlipable::setBack(QGraphicsObject *back)
{
    Q_D(QDeclarativeFlipable);
    if (d->back) {
        qmlInfo(this) << tr("back is a write-once property");
        return;
    }
    d->back = back;
    d->back->setParentItem(this);
    if (Front == d->current)
        d->back->setOpacity(0.);
    connect(back, SIGNAL(widthChanged()),
            this, SLOT(retransformBack()));
    connect(back, SIGNAL(heightChanged()),
            this, SLOT(retransformBack()));
}

void QDeclarativeFlipable::retransformBack()
{
    Q_D(QDeclarativeFlipable);
    if (d->current == QDeclarativeFlipable::Back && d->back)
        d->setBackTransform();
}

/*!
  \qmlproperty enumeration Flipable::side

  The side of the Flippable currently visible. Possible values are \c
  Flippable.Front and \c Flippable.Back.
*/
QDeclarativeFlipable::Side QDeclarativeFlipable::side() const
{
    Q_D(const QDeclarativeFlipable);
    if (d->dirtySceneTransform)
        const_cast<QDeclarativeFlipablePrivate *>(d)->ensureSceneTransform();

    return d->current;
}

// determination on the currently visible side of the flipable
// has to be done on the complete scene transform to give
// correct results.
void QDeclarativeFlipablePrivate::updateSceneTransformFromParent()
{
    Q_Q(QDeclarativeFlipable);

    QDeclarativeItemPrivate::updateSceneTransformFromParent();
    QPointF p1(0, 0);
    QPointF p2(1, 0);
    QPointF p3(1, 1);

    QPointF scenep1 = sceneTransform.map(p1);
    QPointF scenep2 = sceneTransform.map(p2);
    QPointF scenep3 = sceneTransform.map(p3);
    p1 = q->mapToParent(p1);
    p2 = q->mapToParent(p2);
    p3 = q->mapToParent(p3);

    qreal cross = (scenep1.x() - scenep2.x()) * (scenep3.y() - scenep2.y()) -
                  (scenep1.y() - scenep2.y()) * (scenep3.x() - scenep2.x());

    wantBackYFlipped = p1.x() >= p2.x();
    wantBackXFlipped = p2.y() >= p3.y();

    QDeclarativeFlipable::Side newSide;
    if (cross > 0) {
        newSide = QDeclarativeFlipable::Back;
    } else {
        newSide = QDeclarativeFlipable::Front;
    }

    if (newSide != current) {
        current = newSide;
        if (current == QDeclarativeFlipable::Back && back)
            setBackTransform();
        if (front)
            front->setOpacity((current==QDeclarativeFlipable::Front)?1.:0.);
        if (back)
            back->setOpacity((current==QDeclarativeFlipable::Back)?1.:0.);
        emit q->sideChanged();
    }
}

/* Depends on the width/height of the back item, and so needs reevaulating
   if those change.
*/
void QDeclarativeFlipablePrivate::setBackTransform()
{
    QTransform mat;
    QGraphicsItemPrivate *dBack = QGraphicsItemPrivate::get(back);
    mat.translate(dBack->width()/2,dBack->height()/2);
    if (dBack->width() && wantBackYFlipped)
        mat.rotate(180, Qt::YAxis);
    if (dBack->height() && wantBackXFlipped)
        mat.rotate(180, Qt::XAxis);
    mat.translate(-dBack->width()/2,-dBack->height()/2);
    back->setTransform(mat);
}

QT_END_NAMESPACE
