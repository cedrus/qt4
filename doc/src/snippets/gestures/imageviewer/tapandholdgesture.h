/****************************************************************************
**
** Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** No Commercial Usage
** This file contains pre-release code and may not be distributed.
** You may use this file in accordance with the terms and conditions
** contained in the Technology Preview License Agreement accompanying
** this package.
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
** If you have questions regarding the use of this file, please contact
** Nokia at qt-info@nokia.com.
**
**
**
**
**
**
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef TAPANDHOLDGESTURE_H
#define TAPANDHOLDGESTURE_H

#include <QtCore/QBasicTimer>
#include <QtGui/QGesture>
#include <QtGui/QWidget>

class TapAndHoldGesture : public QGesture
{
    Q_OBJECT
    Q_PROPERTY(QPoint pos READ pos)

public:
    TapAndHoldGesture(QWidget *parent);

    bool filterEvent(QEvent *event);
    void reset();

    QPoint pos() const;

protected:
    void timerEvent(QTimerEvent *event);

private:
    QBasicTimer timer;
    int iteration;
    QPoint position;
    QPoint startPosition;
    static const int iterationCount;
    static const int iterationTimeout;
};

#endif // TAPANDHOLDGESTURE_H