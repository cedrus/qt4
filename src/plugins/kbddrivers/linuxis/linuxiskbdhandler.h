/****************************************************************************
**
** Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the plugins of the Qt Toolkit.
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
** In addition, as a special exception, Nokia gives you certain
** additional rights.  These rights are described in the Nokia Qt LGPL
** Exception version 1.1, included in the file LGPL_EXCEPTION.txt in this
** package.
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

#ifndef LINUXISKBDHANDLER_H
#define LINUXISKBDHANDLER_H

#include <QObject>
#include <QWSKeyboardHandler>

class QSocketNotifier;
class LinuxInputSubsystemKbdHandler : public QObject, public QWSKeyboardHandler {
    Q_OBJECT
public:
    LinuxInputSubsystemKbdHandler(const QString &device = QString("/dev/input/event0"));
    ~LinuxInputSubsystemKbdHandler();

    struct keytable_s {
	int code;
	int unicode;
	int keycode;
    };

    struct keymap_s {
	int unicode;
	int keycode;
    };

private:
    void initmap();

    QSocketNotifier *m_notify;
    int  kbdFD;
    bool shift;

    static struct keytable_s keytable[];
    static struct keymap_s keymap[];

private Q_SLOTS:
    void readKbdData();
};

#endif // LINUXISKBDHANDLER_H