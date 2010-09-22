/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the test suite of the Qt Toolkit.
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

#include "nativeeventlist.h"

NativeEventList::NativeEventList(int defaultWaitMs)
    : playbackMultiplier(1.0)
    , currIndex(-1)
    , wait(false)
    , defaultWaitMs(defaultWaitMs)
{
    QString multiplier = qgetenv("NATIVEDEBUG");
    if (!multiplier.isEmpty())
        setTimeMultiplier(multiplier.toFloat());
}

NativeEventList::~NativeEventList()
{
    for (int i=0; i<eventList.size(); i++)
        delete eventList.takeAt(i).second;
}

void NativeEventList::sendNextEvent()
{
    QNativeEvent *e = eventList.at(currIndex).second;
    if (e)
        QNativeInput::sendNativeEvent(*e);
    waitNextEvent();
}

void NativeEventList::waitNextEvent()
{
    if (++currIndex >= eventList.size()){
        emit done();
        stop();
        return;
    }

    int interval = eventList.at(currIndex).first;
    QTimer::singleShot(interval * playbackMultiplier, this, SLOT(sendNextEvent()));
}

void NativeEventList::append(QNativeEvent *event)
{
    eventList.append(QPair<int, QNativeEvent *>(defaultWaitMs, event));
}

void NativeEventList::append(int waitMs, QNativeEvent *event)
{
    eventList.append(QPair<int, QNativeEvent *>(waitMs, event));
}

void NativeEventList::play(Playback playback)
{
    waitNextEvent();

    wait = (playback == WaitUntilFinished);
    while (wait)
        QCoreApplication::processEvents(QEventLoop::WaitForMoreEvents);
}

void NativeEventList::stop()
{
    wait = false;
    QAbstractEventDispatcher::instance()->interrupt();
}

void NativeEventList::setTimeMultiplier(float multiplier)
{
    playbackMultiplier = multiplier;
}

