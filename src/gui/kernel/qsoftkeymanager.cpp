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

#include "qapplication.h"
#include "qevent.h"
#include "qbitmap.h"
#include "private/qsoftkeymanager_p.h"
#include "private/qaction_p.h"
#include "private/qsoftkeymanager_common_p.h"

#ifdef Q_WS_S60
#include "private/qsoftkeymanager_s60_p.h"
#endif

#ifndef QT_NO_SOFTKEYMANAGER
QT_BEGIN_NAMESPACE

QSoftKeyManager *QSoftKeyManagerPrivate::self = 0;

QString QSoftKeyManager::standardSoftKeyText(StandardSoftKey standardKey)
{
    QString softKeyText;
    switch (standardKey) {
    case OkSoftKey:
        softKeyText = QSoftKeyManager::tr("Ok");
        break;
    case SelectSoftKey:
        softKeyText = QSoftKeyManager::tr("Select");
        break;
    case DoneSoftKey:
        softKeyText = QSoftKeyManager::tr("Done");
        break;
    case MenuSoftKey:
        softKeyText = QSoftKeyManager::tr("Options");
        break;
    case CancelSoftKey:
        softKeyText = QSoftKeyManager::tr("Cancel");
        break;
    default:
        break;
    };

    return softKeyText;
}

QSoftKeyManager *QSoftKeyManager::instance()
{
    if (!QSoftKeyManagerPrivate::self)
        QSoftKeyManagerPrivate::self = new QSoftKeyManager;

    return QSoftKeyManagerPrivate::self;
}

QSoftKeyManager::QSoftKeyManager() :
#ifdef Q_WS_S60
    QObject(*(new QSoftKeyManagerPrivateS60), 0)
#else
    QObject(*(new QSoftKeyManagerPrivate), 0)
#endif
{
}

QAction *QSoftKeyManager::createAction(StandardSoftKey standardKey, QWidget *actionWidget)
{
    QAction *action = new QAction(standardSoftKeyText(standardKey), actionWidget);
    QAction::SoftKeyRole softKeyRole = QAction::NoSoftKey;
    switch (standardKey) {
    case MenuSoftKey: // FALL-THROUGH
        QActionPrivate::get(action)->menuActionSoftkeys = true;
    case OkSoftKey:
    case SelectSoftKey:
    case DoneSoftKey:
        softKeyRole = QAction::PositiveSoftKey;
        break;
    case CancelSoftKey:
        softKeyRole = QAction::NegativeSoftKey;
        break;
    }
    action->setSoftKeyRole(softKeyRole);
    action->setVisible(false);
    setForceEnabledInSoftkeys(action);
    return action;
}

/*! \internal

  Creates a QAction and registers the 'triggered' signal to send the given key event to
  \a actionWidget as a convenience.

*/
QAction *QSoftKeyManager::createKeyedAction(StandardSoftKey standardKey, Qt::Key key, QWidget *actionWidget)
{
#ifndef QT_NO_ACTION
    QScopedPointer<QAction> action(createAction(standardKey, actionWidget));

    connect(action.data(), SIGNAL(triggered()), QSoftKeyManager::instance(), SLOT(sendKeyEvent()));
    connect(action.data(), SIGNAL(destroyed(QObject*)), QSoftKeyManager::instance(), SLOT(cleanupHash(QObject*)));
    QSoftKeyManager::instance()->d_func()->keyedActions.insert(action.data(), key);
    return action.take();
#endif //QT_NO_ACTION
}

void QSoftKeyManager::cleanupHash(QObject *obj)
{
    Q_D(QSoftKeyManager);
    QAction *action = qobject_cast<QAction*>(obj);
    d->keyedActions.remove(action);
}

void QSoftKeyManager::sendKeyEvent()
{
    Q_D(QSoftKeyManager);
    QAction *action = qobject_cast<QAction*>(sender());

    if (!action)
        return;

    Qt::Key keyToSend = d->keyedActions.value(action, Qt::Key_unknown);

    if (keyToSend != Qt::Key_unknown)
        QApplication::postEvent(action->parentWidget(),
                                new QKeyEvent(QEvent::KeyPress, keyToSend, Qt::NoModifier));
}

void QSoftKeyManager::updateSoftKeys()
{
    QSoftKeyManager::instance()->d_func()->pendingUpdate = true;
    QEvent *event = new QEvent(QEvent::UpdateSoftKeys);
    QApplication::postEvent(QSoftKeyManager::instance(), event);
}

bool QSoftKeyManager::appendSoftkeys(const QWidget &source, int level)
{
    Q_D(QSoftKeyManager);
    bool ret = false;
    foreach(QAction *action, source.actions()) {
        if (action->softKeyRole() != QAction::NoSoftKey
            && (action->isVisible() || isForceEnabledInSofkeys(action))) {
            d->requestedSoftKeyActions.insert(level, action);
            ret = true;
        }
    }
    return ret;
}


static bool isChildOf(const QWidget *c, const QWidget *p)
{
    while (c) {
        if (c == p)
            return true;
        c = c->parentWidget();
    }
    return false;
}

QWidget *QSoftKeyManager::softkeySource(QWidget *previousSource, bool& recursiveMerging)
{
    Q_D(QSoftKeyManager);
    QWidget *source = NULL;
    if (!previousSource) {
        // Initial source is primarily focuswidget and secondarily activeWindow
        QWidget *focus = QApplication::focusWidget();
        QWidget *popup = QApplication::activePopupWidget();
        if (popup) {
            if (isChildOf(focus, popup))
                source = focus;
            else
                source = popup;
        }
        if (!source) {
            QWidget *modal = QApplication::activeModalWidget();
            if (modal) {
                if (isChildOf(focus, modal))
                    source = focus;
                else
                    source = modal;
            }
        }
        if (!source) {
            source = focus;
            if (!source)
                source = QApplication::activeWindow();
        }
    } else {
        // Softkey merging is based on four criterias
        // 1. Implicit merging is used whenever focus widget does not specify any softkeys
        bool implicitMerging = d->requestedSoftKeyActions.isEmpty();
        // 2. Explicit merging with parent is used whenever WA_MergeSoftkeys widget attribute is set
        bool explicitMerging = previousSource->testAttribute(Qt::WA_MergeSoftkeys);
        // 3. Explicit merging with all parents
        recursiveMerging |= previousSource->testAttribute(Qt::WA_MergeSoftkeysRecursively);
        // 4. Implicit and explicit merging always stops at window boundary
        bool merging = (implicitMerging || explicitMerging || recursiveMerging) && !previousSource->isWindow();

        source = merging ? previousSource->parentWidget() : NULL;
    }
    return source;
}

bool QSoftKeyManager::handleUpdateSoftKeys()
{
    Q_D(QSoftKeyManager);
    int level = 0;
    d->requestedSoftKeyActions.clear();
    bool recursiveMerging = false;
    QWidget *source = softkeySource(NULL, recursiveMerging);
    d->initialSoftKeySource = source;
    while (source) {
        if (appendSoftkeys(*source, level))
            ++level;
        source = softkeySource(source, recursiveMerging);
    }

    d->updateSoftKeys_sys();
    d->pendingUpdate = false;
    return true;
}

void QSoftKeyManager::setForceEnabledInSoftkeys(QAction *action)
{
    QActionPrivate::get(action)->forceEnabledInSoftkeys = true;
}

bool QSoftKeyManager::isForceEnabledInSofkeys(QAction *action)
{
    return QActionPrivate::get(action)->forceEnabledInSoftkeys;
}

bool QSoftKeyManager::event(QEvent *e)
{
#ifndef QT_NO_ACTION
    if (e->type() == QEvent::UpdateSoftKeys)
        return handleUpdateSoftKeys();
#endif //QT_NO_ACTION
    return false;
}

#ifdef Q_WS_S60
bool QSoftKeyManager::handleCommand(int command)
{
    if (QSoftKeyManager::instance()->d_func()->pendingUpdate)
        (void)QSoftKeyManager::instance()->handleUpdateSoftKeys();

    return static_cast<QSoftKeyManagerPrivateS60*>(QSoftKeyManager::instance()->d_func())->handleCommand(command);
}
#endif

QT_END_NAMESPACE
#endif //QT_NO_SOFTKEYMANAGER
