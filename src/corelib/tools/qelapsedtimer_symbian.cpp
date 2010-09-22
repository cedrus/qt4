/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the QtCore module of the Qt Toolkit.
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

#include "qelapsedtimer.h"
#include "qpair.h"
#include <e32std.h>
#include <sys/time.h>
#include <hal.h>

QT_BEGIN_NAMESPACE

// return quint64 to avoid sign-extension
static quint64 getMicrosecondFromTick()
{
    static TInt nanokernel_tick_period;
    if (!nanokernel_tick_period)
        HAL::Get(HAL::ENanoTickPeriod, nanokernel_tick_period);

    static quint32 highdword = 0;
    static quint32 lastval = 0;
    quint32 val = User::NTickCount();
    if (val < lastval)
        ++highdword;
    lastval = val;

    return nanokernel_tick_period * (val | (quint64(highdword) << 32));
}

static quint64 getMillisecondFromTick()
{
    return getMicrosecondFromTick() / 1000;
}

timeval qt_gettime()
{
    timeval tv;
    quint64 now = getMicrosecondFromTick();
    tv.tv_sec = now / 1000000;
    tv.tv_usec = now % 1000000;

    return tv;
}

QElapsedTimer::ClockType QElapsedTimer::clockType()
{
    return TickCounter;
}

bool QElapsedTimer::isMonotonic()
{
    return true;
}

void QElapsedTimer::start()
{
    t1 = getMillisecondFromTick();
    t2 = 0;
}

qint64 QElapsedTimer::restart()
{
    qint64 oldt1 = t1;
    t1 = getMillisecondFromTick();
    t2 = 0;
    return t1 - oldt1;
}

qint64 QElapsedTimer::elapsed() const
{
    return getMillisecondFromTick() - t1;
}

qint64 QElapsedTimer::msecsSinceReference() const
{
    return t1;
}

qint64 QElapsedTimer::msecsTo(const QElapsedTimer &other) const
{
    return other.t1 - t1;
}

qint64 QElapsedTimer::secsTo(const QElapsedTimer &other) const
{
    return msecsTo(other) / 1000;
}

bool operator<(const QElapsedTimer &v1, const QElapsedTimer &v2)
{
    return (v1.t1 - v2.t1) < 0;
}

QT_END_NAMESPACE
