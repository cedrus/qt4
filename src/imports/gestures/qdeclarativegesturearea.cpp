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

#include "qdeclarativegesturearea_p.h"

#include <qdeclarativeexpression.h>
#include <qdeclarativecontext.h>
#include <qdeclarativeinfo.h>

#include <private/qdeclarativeproperty_p.h>
#include <private/qdeclarativeitem_p.h>

#include <QtCore/qdebug.h>
#include <QtCore/qstringlist.h>

#include <QtGui/qevent.h>

#include <private/qobject_p.h>

#ifndef QT_NO_GESTURES

QT_BEGIN_NAMESPACE

class QDeclarativeGestureAreaPrivate : public QDeclarativeItemPrivate
{
    Q_DECLARE_PUBLIC(QDeclarativeGestureArea)
public:
    QDeclarativeGestureAreaPrivate() : componentcomplete(false), gesture(0) {}

    typedef QMap<Qt::GestureType,QDeclarativeExpression*> Bindings;
    Bindings bindings;

    bool componentcomplete;

    QByteArray data;

    QGesture *gesture;

    bool gestureEvent(QGestureEvent *event);
};

/*!
    \qmlclass GestureArea QDeclarativeGestureArea
    \brief The GestureArea item enables simple gesture handling.
    \inherits Item

    A GestureArea is like a MouseArea, but it has signals for gesture events.

    \e {Elements in the Qt.labs module are not guaranteed to remain compatible
    in future versions.}

    \e {This element is only functional on devices with touch input.}

    \qml
    import Qt.labs.gestures 0.1

    GestureArea {
        anchors.fill: parent
        onPan: ... gesture.acceleration ...
        onPinch: ... gesture.rotationAngle ...
        onSwipe: ...
        onTapAndHold: ...
        onTap: ...
        onGesture: ...
    }
    \endqml

    Each signal has a \e gesture parameter that has the
    properties of the gesture.

    \table
    \header \o Signal \o Type \o Property \o Description
    \row \o onTap \o point \o position \o the position of the tap
    \row \o onTapAndHold \o point \o position \o the position of the tap
    \row \o onPan \o real \o acceleration \o the acceleration of the pan
    \row \o onPan \o point \o delta \o the offset from the previous input position to the current input
    \row \o onPan \o point \o offset \o the total offset from the first input position to the current input position
    \row \o onPan \o point \o lastOffset \o the previous value of offset
    \row \o onPinch \o point \o centerPoint \o the midpoint between the two input points
    \row \o onPinch \o point \o lastCenterPoint \o the previous value of centerPoint
    \row \o onPinch \o point \o startCenterPoint \o the first value of centerPoint
    \row \o onPinch \o real \o rotationAngle \o the angle covered by the gesture motion
    \row \o onPinch \o real \o lastRotationAngle \o the previous value of rotationAngle
    \row \o onPinch \o real \o totalRotationAngle \o the complete angle covered by the gesture
    \row \o onPinch \o real \o scaleFactor \o the change in distance between the two input points
    \row \o onPinch \o real \o lastScaleFactor \o the previous value of scaleFactor
    \row \o onPinch \o real \o totalScaleFactor \o the complete scale factor of the gesture
    \row \o onSwipe \o real \o swipeAngle \o the angle of the swipe
    \endtable

    Custom gestures, handled by onGesture, will have custom properties.

    GestureArea is an invisible item: it is never painted.

    \sa MouseArea, {declarative/touchinteraction/gestures}{Gestures example}
*/

/*!
    \internal
    \class QDeclarativeGestureArea
    \brief The QDeclarativeGestureArea class provides simple gesture handling.

*/
QDeclarativeGestureArea::QDeclarativeGestureArea(QDeclarativeItem *parent) :
    QDeclarativeItem(*(new QDeclarativeGestureAreaPrivate), parent)
{
    setAcceptedMouseButtons(Qt::LeftButton);
    setAcceptTouchEvents(true);
}

QDeclarativeGestureArea::~QDeclarativeGestureArea()
{
}

QByteArray
QDeclarativeGestureAreaParser::compile(const QList<QDeclarativeCustomParserProperty> &props)
{
    QByteArray rv;
    QDataStream ds(&rv, QIODevice::WriteOnly);

    for(int ii = 0; ii < props.count(); ++ii)
    {
        QString propName = QString::fromUtf8(props.at(ii).name());
        Qt::GestureType type;

        if (propName == QLatin1String("onTap")) {
            type = Qt::TapGesture;
        } else if (propName == QLatin1String("onTapAndHold")) {
            type = Qt::TapAndHoldGesture;
        } else if (propName == QLatin1String("onPan")) {
            type = Qt::PanGesture;
        } else if (propName == QLatin1String("onPinch")) {
            type = Qt::PinchGesture;
        } else if (propName == QLatin1String("onSwipe")) {
            type = Qt::SwipeGesture;
        } else if (propName == QLatin1String("onGesture")) {
            type = Qt::CustomGesture;
        } else {
            error(props.at(ii), QDeclarativeGestureArea::tr("Cannot assign to non-existent property \"%1\"").arg(propName));
            return QByteArray();
        }

        QList<QVariant> values = props.at(ii).assignedValues();

        for (int i = 0; i < values.count(); ++i) {
            const QVariant &value = values.at(i);

            if (value.userType() == qMetaTypeId<QDeclarativeCustomParserNode>()) {
                error(props.at(ii), QDeclarativeGestureArea::tr("GestureArea: nested objects not allowed"));
                return QByteArray();
            } else if (value.userType() == qMetaTypeId<QDeclarativeCustomParserProperty>()) {
                error(props.at(ii), QDeclarativeGestureArea::tr("GestureArea: syntax error"));
                return QByteArray();
            } else {
                QDeclarativeParser::Variant v = qvariant_cast<QDeclarativeParser::Variant>(value);
                if (v.isScript()) {
                    ds << propName;
                    ds << int(type);
                    ds << v.asScript();
                } else {
                    error(props.at(ii), QDeclarativeGestureArea::tr("GestureArea: script expected"));
                    return QByteArray();
                }
            }
        }
    }

    return rv;
}

void QDeclarativeGestureAreaParser::setCustomData(QObject *object,
                                            const QByteArray &data)
{
    QDeclarativeGestureArea *ga = static_cast<QDeclarativeGestureArea*>(object);
    ga->d_func()->data = data;
}


void QDeclarativeGestureArea::connectSignals()
{
    Q_D(QDeclarativeGestureArea);
    if (!d->componentcomplete)
        return;

    QDataStream ds(d->data);
    while (!ds.atEnd()) {
        QString propName;
        ds >> propName;
        int gesturetype;
        ds >> gesturetype;
        QString script;
        ds >> script;
        QDeclarativeExpression *exp = new QDeclarativeExpression(qmlContext(this), 0, script);
        d->bindings.insert(Qt::GestureType(gesturetype),exp);
        grabGesture(Qt::GestureType(gesturetype));
    }
}

void QDeclarativeGestureArea::componentComplete()
{
    QDeclarativeItem::componentComplete();
    Q_D(QDeclarativeGestureArea);
    d->componentcomplete=true;
    connectSignals();
}

QGesture *QDeclarativeGestureArea::gesture() const
{
    Q_D(const QDeclarativeGestureArea);
    return d->gesture;
}

bool QDeclarativeGestureArea::sceneEvent(QEvent *event)
{
    Q_D(QDeclarativeGestureArea);
    if (event->type() == QEvent::Gesture)
        return d->gestureEvent(static_cast<QGestureEvent*>(event));
    return QDeclarativeItem::sceneEvent(event);
}

bool QDeclarativeGestureAreaPrivate::gestureEvent(QGestureEvent *event)
{
    bool accept = true;
    for (Bindings::Iterator it = bindings.begin(); it != bindings.end(); ++it) {
        if ((gesture = event->gesture(it.key()))) {
            it.value()->evaluate();
            event->setAccepted(true); // XXX only if value returns true?
        }
    }
    return accept;
}

QT_END_NAMESPACE

#endif // QT_NO_GESTURES
