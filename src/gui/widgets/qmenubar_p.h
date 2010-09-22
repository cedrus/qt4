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

#ifndef QMENUBAR_P_H
#define QMENUBAR_P_H

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

#ifndef QMAC_Q3MENUBAR_CPP_FILE
#include "QtGui/qstyleoption.h"
#include <private/qmenu_p.h> // Mac needs what in this file!

#ifdef Q_WS_WINCE
#include "qguifunctions_wince.h"
#endif

#ifndef QT_NO_MENUBAR
#ifdef Q_WS_S60
class CCoeControl;
class CEikMenuBar;
#endif
#endif

QT_BEGIN_NAMESPACE

#ifndef QT_NO_MENUBAR
class QMenuBarExtension;
class QMenuBarPrivate : public QWidgetPrivate
{
    Q_DECLARE_PUBLIC(QMenuBar)
public:
    QMenuBarPrivate() : itemsDirty(0), currentAction(0), mouseDown(0),
                         closePopupMode(0), defaultPopDown(1), popupState(0), keyboardState(0), altPressed(0),
                         nativeMenuBar(-1), doChildEffects(false)
#ifdef QT3_SUPPORT
                         , doAutoResize(false)
#endif
#ifdef Q_WS_MAC
                         , mac_menubar(0)
#endif

#ifdef Q_WS_WINCE
                         , wce_menubar(0), wceClassicMenu(false)
#endif
#ifdef Q_WS_S60
                         , symbian_menubar(0)
#endif

        { }
    ~QMenuBarPrivate()
        {
#ifdef Q_WS_MAC
            delete mac_menubar;
#endif
#ifdef Q_WS_WINCE
            delete wce_menubar;
#endif
#ifdef Q_WS_S60
            delete symbian_menubar;
#endif
        }

    void init();
    QAction *getNextAction(const int start, const int increment) const;

    //item calculations
    uint itemsDirty : 1;

    QVector<int> shortcutIndexMap;
    mutable QVector<QRect> actionRects;
    void calcActionRects(int max_width, int start) const;
    QRect actionRect(QAction *) const;
    void updateGeometries();

    //selection
    QPointer<QAction>currentAction;
    uint mouseDown : 1, closePopupMode : 1, defaultPopDown;
    QAction *actionAt(QPoint p) const;
    void setCurrentAction(QAction *, bool =false, bool =false);
    void popupAction(QAction *, bool);

    //active popup state
    uint popupState : 1;
    QPointer<QMenu> activeMenu;

    //keyboard mode for keyboard navigation
    void focusFirstAction();
    void setKeyboardMode(bool);
    uint keyboardState : 1, altPressed : 1;
    QPointer<QWidget> keyboardFocusWidget;


    int nativeMenuBar : 3;  // Only has values -1, 0, and 1
    //firing of events
    void activateAction(QAction *, QAction::ActionEvent);

    void _q_actionTriggered();
    void _q_actionHovered();
    void _q_internalShortcutActivated(int);
    void _q_updateLayout();

#ifdef Q_WS_WINCE
    void _q_updateDefaultAction();
#endif

    //extra widgets in the menubar
    QPointer<QWidget> leftWidget, rightWidget;
    QMenuBarExtension *extension;
    bool isVisible(QAction *action);

    //menu fading/scrolling effects
    bool doChildEffects;

    QRect menuRect(bool) const;

    // reparenting
    void handleReparent();
    QWidget *oldParent;
    QWidget *oldWindow;

    QList<QAction*> hiddenActions;
    //default action
    QPointer<QAction> defaultAction;

    QBasicTimer autoReleaseTimer;
#ifdef QT3_SUPPORT
    bool doAutoResize;
#endif
#ifdef Q_WS_MAC
    //mac menubar binding
    struct QMacMenuBarPrivate {
        QList<QMacMenuAction*> actionItems;
        OSMenuRef menu, apple_menu;
        QMacMenuBarPrivate();
        ~QMacMenuBarPrivate();

        void addAction(QAction *, QMacMenuAction* =0);
        void addAction(QMacMenuAction *, QMacMenuAction* =0);
        void syncAction(QMacMenuAction *);
        inline void syncAction(QAction *a) { syncAction(findAction(a)); }
        void removeAction(QMacMenuAction *);
        inline void removeAction(QAction *a) { removeAction(findAction(a)); }
        inline QMacMenuAction *findAction(QAction *a) {
            for(int i = 0; i < actionItems.size(); i++) {
                QMacMenuAction *act = actionItems[i];
                if(a == act->action)
                    return act;
            }
            return 0;
        }
    } *mac_menubar;
    static bool macUpdateMenuBarImmediatly();
    bool macWidgetHasNativeMenubar(QWidget *widget);
    void macCreateMenuBar(QWidget *);
    void macDestroyMenuBar();
    OSMenuRef macMenu();
#endif
#ifdef Q_WS_WINCE
    void wceCreateMenuBar(QWidget *);
    void wceDestroyMenuBar();
    struct QWceMenuBarPrivate {
        QList<QWceMenuAction*> actionItems;
        QList<QWceMenuAction*> actionItemsLeftButton;
        QList<QList<QWceMenuAction*>> actionItemsClassic;
        HMENU menuHandle;
        HMENU leftButtonMenuHandle;
        HWND menubarHandle;
        HWND parentWindowHandle;
        bool leftButtonIsMenu;
        QPointer<QAction> leftButtonAction;
        QMenuBarPrivate *d;
        int leftButtonCommand;

        QWceMenuBarPrivate(QMenuBarPrivate *menubar);
        ~QWceMenuBarPrivate();
        void addAction(QAction *, QWceMenuAction* =0);
        void addAction(QWceMenuAction *, QWceMenuAction* =0);
        void syncAction(QWceMenuAction *);
        inline void syncAction(QAction *a) { syncAction(findAction(a)); }
        void removeAction(QWceMenuAction *);
        void rebuild();
        inline void removeAction(QAction *a) { removeAction(findAction(a)); }
        inline QWceMenuAction *findAction(QAction *a) {
            for(int i = 0; i < actionItems.size(); i++) {
                QWceMenuAction *act = actionItems[i];
                if(a == act->action)
                    return act;
            }
            return 0;
        }
    } *wce_menubar;
    bool wceClassicMenu;
    void wceCommands(uint command);
    void wceRefresh();
    bool wceEmitSignals(QList<QWceMenuAction*> actions, uint command);
#endif
#ifdef Q_WS_S60
    void symbianCreateMenuBar(QWidget *);
    void symbianDestroyMenuBar();
    void reparentMenuBar(QWidget *oldParent, QWidget *newParent);
    struct QSymbianMenuBarPrivate {
        QList<QSymbianMenuAction*> actionItems;
        QMenuBarPrivate *d;
        QSymbianMenuBarPrivate(QMenuBarPrivate *menubar);
        ~QSymbianMenuBarPrivate();
        void addAction(QAction *, QSymbianMenuAction* =0);
        void addAction(QSymbianMenuAction *, QSymbianMenuAction* =0);
        void syncAction(QSymbianMenuAction *);
        inline void syncAction(QAction *a) { syncAction(findAction(a)); }
        void removeAction(QSymbianMenuAction *);
        void rebuild();
        inline void removeAction(QAction *a) { removeAction(findAction(a)); }
        inline QSymbianMenuAction *findAction(QAction *a) {
            for(int i = 0; i < actionItems.size(); i++) {
                QSymbianMenuAction *act = actionItems[i];
                if(a == act->action)
                    return act;
            }
            return 0;
        }
        void insertNativeMenuItems(const QList<QAction*> &actions);

    } *symbian_menubar;
    static int symbianCommands(int command);
#endif
#ifdef QT_SOFTKEYS_ENABLED
    QAction *menuBarAction;
#endif
};
#endif

#endif // QT_NO_MENUBAR

QT_END_NAMESPACE

#endif // QMENUBAR_P_H
