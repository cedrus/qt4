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

#include "private/qabstractbutton_p.h"

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

QT_BEGIN_NAMESPACE

class QDialog;
class QPushButton;

class QPushButtonPrivate : public QAbstractButtonPrivate
{
    Q_DECLARE_PUBLIC(QPushButton)
public:
    enum AutoDefaultValue { Off = 0, On = 1, Auto = 2 };

    QPushButtonPrivate()
        : QAbstractButtonPrivate(QSizePolicy::PushButton), autoDefault(Auto),
          defaultButton(false), flat(false), menuOpen(false), lastAutoDefault(false) {}

    inline void init() { resetLayoutItemMargins(); }
    static QPushButtonPrivate* get(QPushButton *b) { return b->d_func(); }
#ifdef Q_WS_MAC
    bool hitButton(const QPoint &pos);
#endif
#ifndef QT_NO_MENU
    QPoint adjustedMenuPosition();
#endif
    void resetLayoutItemMargins();
    void _q_popupPressed();
    QDialog *dialogParent() const;

    QPointer<QMenu> menu;
    uint autoDefault : 2;
    uint defaultButton : 1;
    uint flat : 1;
    uint menuOpen : 1;
    mutable uint lastAutoDefault : 1;
};

QT_END_NAMESPACE
