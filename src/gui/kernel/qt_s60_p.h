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

#ifndef QT_S60_P_H
#define QT_S60_P_H

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

#include "QtGui/qwindowdefs.h"
#include "private/qcore_symbian_p.h"
#include "qhash.h"
#include "qpoint.h"
#include "QtGui/qfont.h"
#include "QtGui/qimage.h"
#include "QtGui/qevent.h"
#include "qpointer.h"
#include "qapplication.h"
#include "qelapsedtimer.h"
#include "QtCore/qthreadstorage.h"
#include <w32std.h>
#include <coecntrl.h>
#include <eikenv.h>
#include <eikappui.h>

#ifdef Q_WS_S60
#include <AknUtils.h>               // AknLayoutUtils
#include <avkon.hrh>                // EEikStatusPaneUidTitle
#include <akntitle.h>               // CAknTitlePane
#include <akncontext.h>             // CAknContextPane
#include <eikspane.h>               // CEikStatusPane
#include <AknPopupFader.h>          // MAknFadedComponent and TAknPopupFader
#endif

QT_BEGIN_NAMESPACE

// Application internal HandleResourceChangeL events,
// system events seems to start with 0x10
const TInt KInternalStatusPaneChange = 0x50000000;

//this macro exists because EColor16MAP enum value doesn't exist in Symbian OS 9.2
#define Q_SYMBIAN_ECOLOR16MAP TDisplayMode(13)

class Q_AUTOTEST_EXPORT QS60ThreadLocalData
{
public:
    QS60ThreadLocalData();
    ~QS60ThreadLocalData();
    bool usingCONEinstances;
    RWsSession wsSession;
    CWsScreenDevice *screenDevice;
};

class QS60Data
{
public:
    QS60Data();
    QThreadStorage<QS60ThreadLocalData *> tls;
    TUid uid;
    int screenDepth;
    QPoint lastCursorPos;
    QPoint lastPointerEventPos;
    QPointer<QWidget> lastPointerEventTarget;
    QPointer<QWidget> mousePressTarget;
    int screenWidthInPixels;
    int screenHeightInPixels;
    int screenWidthInTwips;
    int screenHeightInTwips;
    int defaultDpiX;
    int defaultDpiY;
    WId curWin;
    enum PressedKeys {
        Select = 0x1,
        Right = 0x2,
        Down = 0x4,
        Left = 0x8,
        Up = 0x10,
        LeftUp = 0x20,
        RightUp = 0x40,
        RightDown = 0x80,
        LeftDown = 0x100
    };
    int virtualMousePressedKeys; // of the above type, but avoids casting problems
    int virtualMouseAccelDX;
    int virtualMouseAccelDY;
    QElapsedTimer virtualMouseAccelTimeout;
    int virtualMouseMaxAccel;
#ifndef Q_SYMBIAN_FIXED_POINTER_CURSORS
    int brokenPointerCursors : 1;
#endif
    int hasTouchscreen : 1;
    int mouseInteractionEnabled : 1;
    int virtualMouseRequired : 1;
    int qtOwnsS60Environment : 1;
    int supportsPremultipliedAlpha : 1;
    int avkonComponentsSupportTransparency : 1;
    int menuBeingConstructed : 1;
    QApplication::QS60MainApplicationFactory s60ApplicationFactory; // typedef'ed pointer type

    enum ScanCodeState {
        Unpressed,
        KeyDown,
        KeyDownAndKey
    };
    QHash<TInt, ScanCodeState> scanCodeStates;

    static inline void updateScreenSize();
    inline RWsSession& wsSession();
    static inline RWindowGroup& windowGroup();
    inline CWsScreenDevice* screenDevice();
    static inline CCoeAppUi* appUi();
    static inline CEikMenuBar* menuBar();
#ifdef Q_WS_S60
    static inline CEikStatusPane* statusPane();
    static inline CCoeControl* statusPaneSubPane(TInt aPaneId);
    static inline CAknTitlePane* titlePane();
    static inline CAknContextPane* contextPane();
    static inline CEikButtonGroupContainer* buttonGroupContainer();
    static void setStatusPaneAndButtonGroupVisibility(bool statusPaneVisible, bool buttonGroupVisible);
#endif
    static void controlVisibilityChanged(CCoeControl *control, bool visible);

#ifdef Q_OS_SYMBIAN
    TTrapHandler *s60InstalledTrapHandler;
#endif
};

Q_AUTOTEST_EXPORT QS60Data* qGlobalS60Data();
#define S60 qGlobalS60Data()

class QAbstractLongTapObserver
{
public:
    virtual void HandleLongTapEventL( const TPoint& aPenEventLocation,
                                      const TPoint& aPenEventScreenLocation ) = 0;
};
class QLongTapTimer;


class QSymbianControl : public CCoeControl, public QAbstractLongTapObserver
#ifdef Q_WS_S60
, public MAknFadedComponent, public MEikStatusPaneObserver
#endif
{
public:
    DECLARE_TYPE_ID(0x51740000) // Fun fact: the two first values are "Qt" in ASCII.

public:
    QSymbianControl(QWidget *w);
    void ConstructL(bool isWindowOwning = false, bool desktop = false);
    ~QSymbianControl();
    void HandleResourceChange(int resourceType);
    void HandlePointerEventL(const TPointerEvent& aPointerEvent);
    TKeyResponse OfferKeyEventL(const TKeyEvent& aKeyEvent,TEventCode aType);
#if !defined(QT_NO_IM) && defined(Q_WS_S60)
    TCoeInputCapabilities InputCapabilities() const;
#endif
    TTypeUid::Ptr MopSupplyObject(TTypeUid id);

    inline QWidget* widget() const { return qwidget; }
    void setWidget(QWidget *w);
    void sendInputEvent(QWidget *widget, QInputEvent *inputEvent);
    void setIgnoreFocusChanged(bool enabled) { m_ignoreFocusChanged = enabled; }
    void CancelLongTapTimer();

    void setFocusSafely(bool focus);

#ifdef Q_WS_S60
    void FadeBehindPopup(bool fade){ popupFader.FadeBehindPopup( this, this, fade); }
    void HandleStatusPaneSizeChange();

protected: // from MAknFadedComponent
    TInt CountFadedComponents() {return 1;}
    CCoeControl* FadedComponent(TInt /*aIndex*/) {return this;}
#else
    // #warning No fallback implementation for QSymbianControl::FadeBehindPopup
    void FadeBehindPopup(bool /*fade*/){ }
#endif

protected:
    void Draw(const TRect& aRect) const;
    void SizeChanged();
    void PositionChanged();
    void FocusChanged(TDrawNow aDrawNow);

private:
    void HandlePointerEvent(const TPointerEvent& aPointerEvent);
    TKeyResponse OfferKeyEvent(const TKeyEvent& aKeyEvent,TEventCode aType);
    TKeyResponse sendSymbianKeyEvent(const TKeyEvent &keyEvent, QEvent::Type type);
    TKeyResponse sendKeyEvent(QWidget *widget, QKeyEvent *keyEvent);
    TKeyResponse handleVirtualMouse(const TKeyEvent& keyEvent,TEventCode type);
    bool sendMouseEvent(QWidget *widget, QMouseEvent *mEvent);
    void sendMouseEvent(
            QWidget *receiver,
            QEvent::Type type,
            const QPoint &globalPos,
            Qt::MouseButton button,
            Qt::KeyboardModifiers modifiers);
    void processTouchEvent(int pointerNumber, TPointerEvent::TType type, QPointF screenPos, qreal pressure);
    void HandleLongTapEventL( const TPoint& aPenEventLocation, const TPoint& aPenEventScreenLocation );
#ifdef QT_SYMBIAN_SUPPORTS_ADVANCED_POINTER
    void translateAdvancedPointerEvent(const TAdvancedPointerEvent *event);
#endif

public:
    void handleClientAreaChange();

private:
    static QSymbianControl *lastFocusedControl;

private:
    QWidget *qwidget;
    QLongTapTimer* m_longTapDetector;
    QElapsedTimer m_doubleClickTimer;
    bool m_ignoreFocusChanged : 1;
    bool m_symbianPopupIsOpen : 1;

#ifdef Q_WS_S60
    // Fader object used to fade everything except this menu and the CBA.
    TAknPopupFader popupFader;
#endif
};

inline QS60Data::QS60Data()
: uid(TUid::Null()),
  screenDepth(0),
  screenWidthInPixels(0),
  screenHeightInPixels(0),
  screenWidthInTwips(0),
  screenHeightInTwips(0),
  defaultDpiX(0),
  defaultDpiY(0),
  curWin(0),
  virtualMousePressedKeys(0),
  virtualMouseAccelDX(0),
  virtualMouseAccelDY(0),
  virtualMouseMaxAccel(0),
#ifndef Q_SYMBIAN_FIXED_POINTER_CURSORS
  brokenPointerCursors(0),
#endif
  hasTouchscreen(0),
  mouseInteractionEnabled(0),
  virtualMouseRequired(0),
  qtOwnsS60Environment(0),
  supportsPremultipliedAlpha(0),
  avkonComponentsSupportTransparency(0),
  menuBeingConstructed(0),
  s60ApplicationFactory(0)
#ifdef Q_OS_SYMBIAN
  ,s60InstalledTrapHandler(0)
#endif
{
}

inline void QS60Data::updateScreenSize()
{
    TPixelsTwipsAndRotation params;
    int mode = S60->screenDevice()->CurrentScreenMode();
    S60->screenDevice()->GetScreenModeSizeAndRotation(mode, params);
    S60->screenWidthInPixels = params.iPixelSize.iWidth;
    S60->screenHeightInPixels = params.iPixelSize.iHeight;
    S60->screenWidthInTwips = params.iTwipsSize.iWidth;
    S60->screenHeightInTwips = params.iTwipsSize.iHeight;

    S60->virtualMouseMaxAccel = qMax(S60->screenHeightInPixels, S60->screenWidthInPixels) / 10;

    TReal inches = S60->screenHeightInTwips / (TReal)KTwipsPerInch;
    S60->defaultDpiY = S60->screenHeightInPixels / inches;
    inches = S60->screenWidthInTwips / (TReal)KTwipsPerInch;
    S60->defaultDpiX = S60->screenWidthInPixels / inches;
}

inline RWsSession& QS60Data::wsSession()
{
    if(!tls.hasLocalData()) {
        tls.setLocalData(new QS60ThreadLocalData);
    }
    return tls.localData()->wsSession;
}

inline RWindowGroup& QS60Data::windowGroup()
{
    return CCoeEnv::Static()->RootWin();
}

inline CWsScreenDevice* QS60Data::screenDevice()
{
    if(!tls.hasLocalData()) {
        tls.setLocalData(new QS60ThreadLocalData);
    }
    return tls.localData()->screenDevice;
}

inline CCoeAppUi* QS60Data::appUi()
{
    return CCoeEnv::Static()-> AppUi();
}

inline CEikMenuBar* QS60Data::menuBar()
{
    return CEikonEnv::Static()->AppUiFactory()->MenuBar();
}

#ifdef Q_WS_S60
inline CEikStatusPane* QS60Data::statusPane()
{
    return CEikonEnv::Static()->AppUiFactory()->StatusPane();
}

// Returns the application's status pane control, if not present returns NULL.
inline CCoeControl* QS60Data::statusPaneSubPane( TInt aPaneId )
{
    const TUid paneUid = { aPaneId };
    CEikStatusPane* statusPane = S60->statusPane();
    if (statusPane && statusPane->PaneCapabilities(paneUid).IsPresent()) {
        CCoeControl* control = NULL;
        // ControlL shouldn't leave because the pane is present
        TRAPD(err, control = statusPane->ControlL(paneUid));
        return err != KErrNone ? NULL : control;
    }
    return NULL;
}

// Returns the application's title pane, if not present returns NULL.
inline CAknTitlePane* QS60Data::titlePane()
{
    return static_cast<CAknTitlePane*>(S60->statusPaneSubPane(EEikStatusPaneUidTitle));
}

// Returns the application's title pane, if not present returns NULL.
inline CAknContextPane* QS60Data::contextPane()
{
    return static_cast<CAknContextPane*>(S60->statusPaneSubPane(EEikStatusPaneUidContext));
}

inline CEikButtonGroupContainer* QS60Data::buttonGroupContainer()
{
    return CEikonEnv::Static()->AppUiFactory()->Cba();
}
#endif // Q_WS_S60

static inline QFont qt_TFontSpec2QFontL(const TFontSpec &fontSpec)
{
    return QFont(
        qt_TDesC2QString(fontSpec.iTypeface.iName),
        fontSpec.iHeight / KTwipsPerPoint,
        fontSpec.iFontStyle.StrokeWeight() == EStrokeWeightNormal ? QFont::Normal : QFont::Bold,
        fontSpec.iFontStyle.Posture() == EPostureItalic
    );
}

static inline QImage::Format qt_TDisplayMode2Format(TDisplayMode mode)
{
    QImage::Format format;
    switch(mode) {
    case EGray2:
        format = QImage::Format_MonoLSB;
        break;
    case EColor256:
    case EGray256:
        format = QImage::Format_Indexed8;
        break;
    case EColor4K:
        format = QImage::Format_RGB444;
        break;
    case EColor64K:
        format = QImage::Format_RGB16;
        break;
    case EColor16M:
        format = QImage::Format_RGB888;
        break;
    case EColor16MU:
        format = QImage::Format_RGB32;
        break;
    case EColor16MA:
        format = QImage::Format_ARGB32;
        break;
    case Q_SYMBIAN_ECOLOR16MAP:
        format = QImage::Format_ARGB32_Premultiplied;
        break;
    default:
        format = QImage::Format_Invalid;
        break;
    }
    return format;
}

#ifndef QT_NO_CURSOR
void qt_symbian_setWindowCursor(const QCursor &cursor, const CCoeControl* wid);
void qt_symbian_setWindowGroupCursor(const QCursor &cursor, RWindowTreeNode &node);
void qt_symbian_setGlobalCursor(const QCursor &cursor);
void qt_symbian_set_cursor_visible(bool visible);
bool qt_symbian_is_cursor_visible();
#endif

QT_END_NAMESPACE

#endif // QT_S60_P_H
