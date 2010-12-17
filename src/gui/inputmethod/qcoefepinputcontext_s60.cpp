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

#ifndef QT_NO_IM

#include "qcoefepinputcontext_p.h"
#include <qapplication.h>
#include <qtextformat.h>
#include <qgraphicsview.h>
#include <qgraphicsscene.h>
#include <qgraphicswidget.h>
#include <qsymbianevent.h>
#include <private/qcore_symbian_p.h>

#include <fepitfr.h>
#include <hal.h>

#include <limits.h>
// You only find these enumerations on SDK 5 onwards, so we need to provide our own
// to remain compatible with older releases. They won't be called by pre-5.0 SDKs.

// MAknEdStateObserver::EAknCursorPositionChanged
#define QT_EAknCursorPositionChanged MAknEdStateObserver::EAknEdwinStateEvent(6)
// MAknEdStateObserver::EAknActivatePenInputRequest
#define QT_EAknActivatePenInputRequest MAknEdStateObserver::EAknEdwinStateEvent(7)

// EAknEditorFlagSelectionVisible is only valid from 3.2 onwards.
// Sym^3 AVKON FEP manager expects that this flag is used for FEP-aware editors
// that support text selection.
#define QT_EAknEditorFlagSelectionVisible 0x100000

QT_BEGIN_NAMESPACE

QCoeFepInputContext::QCoeFepInputContext(QObject *parent)
    : QInputContext(parent),
      m_fepState(q_check_ptr(new CAknEdwinState)),		// CBase derived object needs check on new
      m_lastImHints(Qt::ImhNone),
      m_textCapabilities(TCoeInputCapabilities::EAllText),
      m_inDestruction(false),
      m_pendingInputCapabilitiesChanged(false),
      m_cursorVisibility(1),
      m_inlinePosition(0),
      m_formatRetriever(0),
      m_pointerHandler(0),
      m_hasTempPreeditString(false)
{
    m_fepState->SetObjectProvider(this);
    if (QSysInfo::s60Version() > QSysInfo::SV_S60_5_0)
        m_fepState->SetFlags(EAknEditorFlagDefault | QT_EAknEditorFlagSelectionVisible);
    else
        m_fepState->SetFlags(EAknEditorFlagDefault);
    m_fepState->SetDefaultInputMode( EAknEditorTextInputMode );
    m_fepState->SetPermittedInputModes( EAknEditorAllInputModes );
    m_fepState->SetDefaultCase( EAknEditorLowerCase );
    m_fepState->SetPermittedCases( EAknEditorAllCaseModes );
    m_fepState->SetSpecialCharacterTableResourceId(R_AVKON_SPECIAL_CHARACTER_TABLE_DIALOG);
    m_fepState->SetNumericKeymap( EAknEditorStandardNumberModeKeymap );
}

QCoeFepInputContext::~QCoeFepInputContext()
{
    m_inDestruction = true;

    // This is to make sure that the FEP manager "forgets" about us,
    // otherwise we may get callbacks even after we're destroyed.
    // The call below is essentially equivalent to InputCapabilitiesChanged(),
    // but is synchronous, rather than asynchronous.
    CCoeEnv::Static()->SyncNotifyFocusObserversOfChangeInFocus();

    if (m_fepState)
        delete m_fepState;
}

void QCoeFepInputContext::reset()
{
    commitCurrentString(true);
}

void QCoeFepInputContext::ReportAknEdStateEvent(MAknEdStateObserver::EAknEdwinStateEvent aEventType)
{
    QT_TRAP_THROWING(m_fepState->ReportAknEdStateEventL(aEventType));
}

void QCoeFepInputContext::update()
{
    updateHints(false);

    // For pre-5.0 SDKs, we don't do text updates on S60 side.
    if (QSysInfo::s60Version() < QSysInfo::SV_S60_5_0) {
        return;
    }

    // Don't be fooled (as I was) by the name of this enumeration.
    // What it really does is tell the virtual keyboard UI that the text has been
    // updated and it should be reflected in the internal display of the VK.
    ReportAknEdStateEvent(QT_EAknCursorPositionChanged);
}

void QCoeFepInputContext::setFocusWidget(QWidget *w)
{
    commitCurrentString(true);

    QInputContext::setFocusWidget(w);

    updateHints(true);
}

void QCoeFepInputContext::widgetDestroyed(QWidget *w)
{
    // Make sure that the input capabilities of whatever new widget got focused are queried.
    CCoeControl *ctrl = w->effectiveWinId();
    if (ctrl->IsFocused()) {
        queueInputCapabilitiesChanged();
    }
}

QString QCoeFepInputContext::language()
{
    TLanguage lang = m_fepState->LocalLanguage();
    const QByteArray localeName = qt_symbianLocaleName(lang);
    if (!localeName.isEmpty()) {
        return QString::fromLatin1(localeName);
    } else {
        return QString::fromLatin1("C");
    }
}

bool QCoeFepInputContext::needsInputPanel()
{
    switch (QSysInfo::s60Version()) {
    case QSysInfo::SV_S60_3_1:
    case QSysInfo::SV_S60_3_2:
        // There are no touch phones for pre-5.0 SDKs.
        return false;
#ifdef Q_CC_NOKIAX86
    default:
        // For emulator we assume that we need an input panel, since we can't
        // separate between phone types.
        return true;
#else
    case QSysInfo::SV_S60_5_0: {
        // For SDK == 5.0, we need phone specific detection, since the HAL API
        // is no good on most phones. However, all phones at the time of writing use the
        // input panel, except N97 in landscape mode, but in this mode it refuses to bring
        // up the panel anyway, so we don't have to care.
        return true;
    }
    default:
        // For unknown/newer types, we try to use the HAL API.
        int keyboardEnabled;
        int keyboardType;
        int err[2];
        err[0] = HAL::Get(HAL::EKeyboard, keyboardType);
        err[1] = HAL::Get(HAL::EKeyboardState, keyboardEnabled);
        if (err[0] == KErrNone && err[1] == KErrNone
                && keyboardType != 0 && keyboardEnabled)
            // Means that we have some sort of keyboard.
            return false;

        // Fall back to using the input panel.
        return true;
#endif // !Q_CC_NOKIAX86
    }
}

bool QCoeFepInputContext::filterEvent(const QEvent *event)
{
    // The CloseSoftwareInputPanel event is not handled here, because the VK will automatically
    // close when it discovers that the underlying widget does not have input capabilities.

    if (!focusWidget())
        return false;

    switch (event->type()) {
    case QEvent::KeyPress:
        commitTemporaryPreeditString();
        // fall through intended
    case QEvent::KeyRelease:
        const QKeyEvent *keyEvent = static_cast<const QKeyEvent *>(event);
        switch (keyEvent->key()) {
        case Qt::Key_F20:
            Q_ASSERT(m_lastImHints == focusWidget()->inputMethodHints());
            if (m_lastImHints & Qt::ImhHiddenText) {
                // Special case in Symbian. On editors with secret text, F20 is for some reason
                // considered to be a backspace.
                QKeyEvent modifiedEvent(keyEvent->type(), Qt::Key_Backspace, keyEvent->modifiers(),
                        keyEvent->text(), keyEvent->isAutoRepeat(), keyEvent->count());
                QApplication::sendEvent(focusWidget(), &modifiedEvent);
                return true;
            }
            break;
        case Qt::Key_Select:
            if (!m_preeditString.isEmpty()) {
                commitCurrentString(true);
                return true;
            }
            break;
        default:
            break;
        }

        QString widgetText = focusWidget()->inputMethodQuery(Qt::ImSurroundingText).toString();
        bool validLength;
        int maxLength = focusWidget()->inputMethodQuery(Qt::ImMaximumTextLength).toInt(&validLength);
        if (!keyEvent->text().isEmpty() && validLength
                && widgetText.size() + m_preeditString.size() >= maxLength) {
            // Don't send key events with string content if the widget is "full".
            return true;
        }

        if (keyEvent->type() == QEvent::KeyPress
            && focusWidget()->inputMethodHints() & Qt::ImhHiddenText
            && !keyEvent->text().isEmpty()) {
            // Send some temporary preedit text in order to make text visible for a moment.
            m_preeditString = keyEvent->text();
            QList<QInputMethodEvent::Attribute> attributes;
            QInputMethodEvent imEvent(m_preeditString, attributes);
            sendEvent(imEvent);
            m_tempPreeditStringTimeout.start(1000, this);
            m_hasTempPreeditString = true;
            update();
            return true;
        }
        break;
    }

    if (!needsInputPanel())
        return false;

    if (event->type() == QEvent::RequestSoftwareInputPanel) {
        // Notify S60 that we want the virtual keyboard to show up.
        QSymbianControl *sControl;
        sControl = focusWidget()->effectiveWinId()->MopGetObject(sControl);
        Q_ASSERT(sControl);

        // The FEP UI temporarily steals focus when it shows up the first time, causing
        // all sorts of weird effects on the focused widgets. Since it will immediately give
        // back focus to us, we temporarily disable focus handling until the job's done.
        if (sControl) {
            sControl->setIgnoreFocusChanged(true);
        }

        ensureInputCapabilitiesChanged();
        m_fepState->ReportAknEdStateEventL(MAknEdStateObserver::QT_EAknActivatePenInputRequest);

        if (sControl) {
            sControl->setIgnoreFocusChanged(false);
        }
        return true;
    }

    return false;
}

bool QCoeFepInputContext::symbianFilterEvent(QWidget *keyWidget, const QSymbianEvent *event)
{
    Q_UNUSED(keyWidget);
    if (event->type() == QSymbianEvent::CommandEvent)
        // A command basically means the same as a button being pushed. With Qt buttons
        // that would normally result in a reset of the input method due to the focus change.
        // This should also happen for commands.
        reset();

    return false;
}

void QCoeFepInputContext::timerEvent(QTimerEvent *timerEvent)
{
    if (timerEvent->timerId() == m_tempPreeditStringTimeout.timerId())
        commitTemporaryPreeditString();
}

void QCoeFepInputContext::commitTemporaryPreeditString()
{
    if (m_tempPreeditStringTimeout.isActive())
        m_tempPreeditStringTimeout.stop();

    if (!m_hasTempPreeditString)
        return;

    commitCurrentString(false);
}

void QCoeFepInputContext::mouseHandler( int x, QMouseEvent *event)
{
    Q_ASSERT(focusWidget());

    if (event->type() == QEvent::MouseButtonPress && event->button() == Qt::LeftButton) {
        commitCurrentString(true);
        int pos = focusWidget()->inputMethodQuery(Qt::ImCursorPosition).toInt();

        QList<QInputMethodEvent::Attribute> attributes;
        attributes << QInputMethodEvent::Attribute(QInputMethodEvent::Selection, pos + x, 0, QVariant());
        QInputMethodEvent event(QLatin1String(""), attributes);
        sendEvent(event);
    }
}

TCoeInputCapabilities QCoeFepInputContext::inputCapabilities()
{
    if (m_inDestruction || !focusWidget()) {
        return TCoeInputCapabilities(TCoeInputCapabilities::ENone, 0, 0);
    }

    return TCoeInputCapabilities(m_textCapabilities, this, 0);
}

static QTextCharFormat qt_TCharFormat2QTextCharFormat(const TCharFormat &cFormat, bool validStyleColor)
{
    QTextCharFormat qFormat;

    if (validStyleColor) {
        QBrush foreground(QColor(cFormat.iFontPresentation.iTextColor.Internal()));
        qFormat.setForeground(foreground);
    }

    qFormat.setFontStrikeOut(cFormat.iFontPresentation.iStrikethrough == EStrikethroughOn);
    qFormat.setFontUnderline(cFormat.iFontPresentation.iUnderline == EUnderlineOn);

    return qFormat;
}

void QCoeFepInputContext::updateHints(bool mustUpdateInputCapabilities)
{
    QWidget *w = focusWidget();
    if (w) {
        Qt::InputMethodHints hints = w->inputMethodHints();
        if (hints != m_lastImHints) {
            m_lastImHints = hints;
            applyHints(hints);
        } else if (!mustUpdateInputCapabilities) {
            // Optimization. Return immediately if there was no change.
            return;
        }
    }
    queueInputCapabilitiesChanged();
}

void QCoeFepInputContext::applyHints(Qt::InputMethodHints hints)
{
    using namespace Qt;

    commitTemporaryPreeditString();

    const bool anynumbermodes = hints & (ImhDigitsOnly | ImhFormattedNumbersOnly | ImhDialableCharactersOnly);
    const bool anytextmodes = hints & (ImhUppercaseOnly | ImhLowercaseOnly | ImhEmailCharactersOnly | ImhUrlCharactersOnly);
    const bool numbersOnly = anynumbermodes && !anytextmodes;
    const bool noOnlys = !(hints & ImhExclusiveInputMask);
    // if alphanumeric input, or if multiple incompatible number modes are selected;
    // then make all symbols available in numeric mode too.
    const bool needsCharMap= !numbersOnly || ((hints & ImhFormattedNumbersOnly) && (hints & ImhDialableCharactersOnly));
    TInt flags;
    Qt::InputMethodHints oldHints = hints;

    // Some sanity checking. Make sure that only one preference is set.
    InputMethodHints prefs = ImhPreferNumbers | ImhPreferUppercase | ImhPreferLowercase;
    prefs &= hints;
    if (prefs != ImhPreferNumbers && prefs != ImhPreferUppercase && prefs != ImhPreferLowercase) {
        hints &= ~prefs;
    }
    if (!noOnlys) {
        // Make sure that the preference is within the permitted set.
        if (hints & ImhPreferNumbers && !anynumbermodes) {
            hints &= ~ImhPreferNumbers;
        } else if (hints & ImhPreferUppercase && !(hints & ImhUppercaseOnly)) {
            hints &= ~ImhPreferUppercase;
        } else if (hints & ImhPreferLowercase && !(hints & ImhLowercaseOnly)) {
            hints &= ~ImhPreferLowercase;
        }
        // If there is no preference, set it to something within the permitted set.
        if (!(hints & ImhPreferNumbers || hints & ImhPreferUppercase || hints & ImhPreferLowercase)) {
            if (hints & ImhLowercaseOnly) {
                hints |= ImhPreferLowercase;
            } else if (hints & ImhUppercaseOnly) {
                hints |= ImhPreferUppercase;
            } else if (numbersOnly) {
                hints |= ImhPreferNumbers;
            }
        }
    }

    if (hints & ImhPreferNumbers) {
        m_fepState->SetDefaultInputMode(EAknEditorNumericInputMode);
        m_fepState->SetCurrentInputMode(EAknEditorNumericInputMode);
    } else {
        m_fepState->SetDefaultInputMode(EAknEditorTextInputMode);
        m_fepState->SetCurrentInputMode(EAknEditorTextInputMode);
    }
    flags = 0;
    if (noOnlys || (anynumbermodes && anytextmodes)) {
        flags = EAknEditorAllInputModes;
    }
    else if (anynumbermodes) {
        flags |= EAknEditorNumericInputMode;
        if (QSysInfo::s60Version() > QSysInfo::SV_S60_5_0
            && ((hints & ImhFormattedNumbersOnly) || (hints & ImhDialableCharactersOnly))) {
            //workaround - the * key does not launch the symbols menu, making it impossible to use these modes unless text mode is enabled.
            flags |= EAknEditorTextInputMode;
        }
    }
    else if (anytextmodes) {
        flags |= EAknEditorTextInputMode;
    }
    else {
        flags = EAknEditorAllInputModes;
    }
    m_fepState->SetPermittedInputModes(flags);
    ReportAknEdStateEvent(MAknEdStateObserver::EAknEdwinStateInputModeUpdate);

    if (hints & ImhPreferLowercase) {
        m_fepState->SetDefaultCase(EAknEditorLowerCase);
        m_fepState->SetCurrentCase(EAknEditorLowerCase);
    } else if (hints & ImhPreferUppercase) {
        m_fepState->SetDefaultCase(EAknEditorUpperCase);
        m_fepState->SetCurrentCase(EAknEditorUpperCase);
    } else if (hints & ImhNoAutoUppercase) {
        m_fepState->SetDefaultCase(EAknEditorLowerCase);
        m_fepState->SetCurrentCase(EAknEditorLowerCase);
    } else {
        m_fepState->SetDefaultCase(EAknEditorTextCase);
        m_fepState->SetCurrentCase(EAknEditorTextCase);
    }
    flags = 0;
    if (hints & ImhUppercaseOnly) {
        flags |= EAknEditorUpperCase;
    }
    if (hints & ImhLowercaseOnly) {
        flags |= EAknEditorLowerCase;
    }
    if (flags == 0) {
        flags = EAknEditorAllCaseModes;
        if (hints & ImhNoAutoUppercase) {
            flags &= ~EAknEditorTextCase;
        }
    }
    m_fepState->SetPermittedCases(flags);
    ReportAknEdStateEvent(MAknEdStateObserver::EAknEdwinStateCaseModeUpdate);

    if (QSysInfo::s60Version() > QSysInfo::SV_S60_5_0)
        flags = QT_EAknEditorFlagSelectionVisible;
    else 
        flags = 0;
    if (hints & ImhUppercaseOnly && !(hints & ImhLowercaseOnly)
            || hints & ImhLowercaseOnly && !(hints & ImhUppercaseOnly)) {
        flags |= EAknEditorFlagFixedCase;
    }
    // Using T9 and hidden text together may actually crash the FEP, so check for hidden text too.
    if (hints & ImhNoPredictiveText || hints & ImhHiddenText) {
        flags |= EAknEditorFlagNoT9;
    }
    if (needsCharMap)
        flags |= EAknEditorFlagUseSCTNumericCharmap;
    m_fepState->SetFlags(flags);
    ReportAknEdStateEvent(MAknEdStateObserver::EAknEdwinStateFlagsUpdate);

    if (hints & ImhDialableCharactersOnly) {
        // This is first, because if (ImhDialableCharactersOnly | ImhFormattedNumbersOnly)
        // is specified, this one is more natural (# key enters a #)
        flags = EAknEditorStandardNumberModeKeymap;
    } else if (hints & ImhFormattedNumbersOnly) {
        // # key enters decimal point
        flags = EAknEditorCalculatorNumberModeKeymap;
    } else if (hints & ImhDigitsOnly) {
        // This is last, because it is most restrictive (# key is inactive)
        flags = EAknEditorPlainNumberModeKeymap;
    } else {
        flags = EAknEditorStandardNumberModeKeymap;
    }
    m_fepState->SetNumericKeymap(static_cast<TAknEditorNumericKeymap>(flags));

    if (hints & ImhUrlCharactersOnly) {
        // URL characters is everything except space, so a superset of the other restrictions
        m_fepState->SetSpecialCharacterTableResourceId(R_AVKON_URL_SPECIAL_CHARACTER_TABLE_DIALOG);
    } else if (hints & ImhEmailCharactersOnly) {
        m_fepState->SetSpecialCharacterTableResourceId(R_AVKON_EMAIL_ADDR_SPECIAL_CHARACTER_TABLE_DIALOG);
    } else if (needsCharMap) {
        m_fepState->SetSpecialCharacterTableResourceId(R_AVKON_SPECIAL_CHARACTER_TABLE_DIALOG);
    } else {
        m_fepState->SetSpecialCharacterTableResourceId(0);
    }

    if (hints & ImhHiddenText) {
        m_textCapabilities = TCoeInputCapabilities::EAllText | TCoeInputCapabilities::ESecretText;
    } else {
        m_textCapabilities = TCoeInputCapabilities::EAllText;
    }
}

void QCoeFepInputContext::applyFormat(QList<QInputMethodEvent::Attribute> *attributes)
{
    TCharFormat cFormat;
    QColor styleTextColor;
    if (QWidget *focused = focusWidget()) {
        QGraphicsView *gv = qobject_cast<QGraphicsView*>(focused);
        if (!gv) // could be either the QGV or its viewport that has focus
            gv = qobject_cast<QGraphicsView*>(focused->parentWidget());
        if (gv) {
            if (QGraphicsScene *scene = gv->scene()) {
                if (QGraphicsItem *focusItem = scene->focusItem()) {
                    if (focusItem->isWidget()) {
                        styleTextColor = static_cast<QGraphicsWidget*>(focusItem)->palette().text().color();
                    }
                }
            }
        } else {
            styleTextColor = focused->palette().text().color();
        }
    } else {
        styleTextColor = QApplication::palette("QLineEdit").text().color();
    }

    if (styleTextColor.isValid()) {
        const TLogicalRgb fontColor(TRgb(styleTextColor.red(), styleTextColor.green(), styleTextColor.blue(), styleTextColor.alpha()));
        cFormat.iFontPresentation.iTextColor = fontColor;
    }

    TInt numChars = 0;
    TInt charPos = 0;
    int oldSize = attributes->size();
    while (m_formatRetriever) {
        m_formatRetriever->GetFormatOfFepInlineText(cFormat, numChars, charPos);
        if (numChars <= 0) {
            // This shouldn't happen according to S60 docs, but apparently does sometimes.
            break;
        }
        attributes->append(QInputMethodEvent::Attribute(QInputMethodEvent::TextFormat,
                                                        charPos,
                                                        numChars,
                                                        QVariant(qt_TCharFormat2QTextCharFormat(cFormat, styleTextColor.isValid()))));
        charPos += numChars;
        if (charPos >= m_preeditString.size()) {
            break;
        }
    }

    if (attributes->size() == oldSize) {
        // S60 didn't provide any format, so let's give our own instead.
        attributes->append(QInputMethodEvent::Attribute(QInputMethodEvent::TextFormat,
                                                        0,
                                                        m_preeditString.size(),
                                                        standardFormat(PreeditFormat)));
    }
}

void QCoeFepInputContext::queueInputCapabilitiesChanged()
{
    if (m_pendingInputCapabilitiesChanged)
        return;

    // Call ensureInputCapabilitiesChanged asynchronously. This is done to improve performance
    // by not updating input capabilities too often. The reason we don't call the Symbian
    // asynchronous version of InputCapabilitiesChanged is because we need to ensure that it
    // is synchronous in some specific cases. Those will call ensureInputCapabilitesChanged.
    QMetaObject::invokeMethod(this, "ensureInputCapabilitiesChanged", Qt::QueuedConnection);
    m_pendingInputCapabilitiesChanged = true;
}

void QCoeFepInputContext::ensureInputCapabilitiesChanged()
{
    if (!m_pendingInputCapabilitiesChanged)
        return;

    // The call below is essentially equivalent to InputCapabilitiesChanged(),
    // but is synchronous, rather than asynchronous.
    CCoeEnv::Static()->SyncNotifyFocusObserversOfChangeInFocus();
    m_pendingInputCapabilitiesChanged = false;
}

void QCoeFepInputContext::StartFepInlineEditL(const TDesC& aInitialInlineText,
        TInt aPositionOfInsertionPointInInlineText, TBool aCursorVisibility, const MFormCustomDraw* /*aCustomDraw*/,
        MFepInlineTextFormatRetriever& aInlineTextFormatRetriever,
        MFepPointerEventHandlerDuringInlineEdit& aPointerEventHandlerDuringInlineEdit)
{
    QWidget *w = focusWidget();
    if (!w)
        return;

    commitTemporaryPreeditString();

    QList<QInputMethodEvent::Attribute> attributes;

    m_cursorVisibility = aCursorVisibility ? 1 : 0;
    m_inlinePosition = aPositionOfInsertionPointInInlineText;
    m_preeditString = qt_TDesC2QString(aInitialInlineText);

    m_formatRetriever = &aInlineTextFormatRetriever;
    m_pointerHandler = &aPointerEventHandlerDuringInlineEdit;

    // With T9 aInitialInlineText is typically empty when StartFepInlineEditL is called,
    // but FEP requires that selected text is always removed at StartFepInlineEditL.
    // Let's remove the selected text if aInitialInlineText is empty and there is selected text
    if (m_preeditString.isEmpty()) {
        int anchor = w->inputMethodQuery(Qt::ImAnchorPosition).toInt();
        int cursorPos = w->inputMethodQuery(Qt::ImCursorPosition).toInt();
        int replacementLength = qAbs(cursorPos-anchor);
        if (replacementLength > 0) {
            int replacementStart = cursorPos < anchor ? 0 : -replacementLength;
            QList<QInputMethodEvent::Attribute> clearSelectionAttributes;
            QInputMethodEvent clearSelectionEvent(QLatin1String(""), clearSelectionAttributes);
            clearSelectionEvent.setCommitString(QLatin1String(""), replacementStart, replacementLength);
            sendEvent(clearSelectionEvent);
        }
    }

    applyFormat(&attributes);

    attributes.append(QInputMethodEvent::Attribute(QInputMethodEvent::Cursor,
                                                   m_inlinePosition,
                                                   m_cursorVisibility,
                                                   QVariant()));
    QInputMethodEvent event(m_preeditString, attributes);
    sendEvent(event);
}

void QCoeFepInputContext::UpdateFepInlineTextL(const TDesC& aNewInlineText,
        TInt aPositionOfInsertionPointInInlineText)
{
    QWidget *w = focusWidget();
    if (!w)
        return;

    m_inlinePosition = aPositionOfInsertionPointInInlineText;

    QList<QInputMethodEvent::Attribute> attributes;
    applyFormat(&attributes);
    attributes.append(QInputMethodEvent::Attribute(QInputMethodEvent::Cursor,
                                                   m_inlinePosition,
                                                   m_cursorVisibility,
                                                   QVariant()));
    QString newPreeditString = qt_TDesC2QString(aNewInlineText);
    QInputMethodEvent event(newPreeditString, attributes);
    if (newPreeditString.isEmpty() && m_preeditString.isEmpty()) {
        // In Symbian world this means "erase last character".
        event.setCommitString(QLatin1String(""), -1, 1);
    }
    m_preeditString = newPreeditString;
    sendEvent(event);
}

void QCoeFepInputContext::SetInlineEditingCursorVisibilityL(TBool aCursorVisibility)
{
    QWidget *w = focusWidget();
    if (!w)
        return;

    m_cursorVisibility = aCursorVisibility ? 1 : 0;

    QList<QInputMethodEvent::Attribute> attributes;
    attributes.append(QInputMethodEvent::Attribute(QInputMethodEvent::Cursor,
                                                   m_inlinePosition,
                                                   m_cursorVisibility,
                                                   QVariant()));
    QInputMethodEvent event(m_preeditString, attributes);
    sendEvent(event);
}

void QCoeFepInputContext::CancelFepInlineEdit()
{
    QList<QInputMethodEvent::Attribute> attributes;
    QInputMethodEvent event(QLatin1String(""), attributes);
    event.setCommitString(QLatin1String(""), 0, 0);
    m_preeditString.clear();
    sendEvent(event);
}

TInt QCoeFepInputContext::DocumentLengthForFep() const
{
    QWidget *w = focusWidget();
    if (!w)
        return 0;

    QVariant variant = w->inputMethodQuery(Qt::ImSurroundingText);
    return variant.value<QString>().size() + m_preeditString.size();
}

TInt QCoeFepInputContext::DocumentMaximumLengthForFep() const
{
    QWidget *w = focusWidget();
    if (!w)
        return 0;

    QVariant variant = w->inputMethodQuery(Qt::ImMaximumTextLength);
    int size;
    if (variant.isValid()) {
        size = variant.toInt();
    } else {
        size = INT_MAX; // Sensible default for S60.
    }
    return size;
}

void QCoeFepInputContext::SetCursorSelectionForFepL(const TCursorSelection& aCursorSelection)
{
    QWidget *w = focusWidget();
    if (!w)
        return;

    commitTemporaryPreeditString();

    int pos = aCursorSelection.iAnchorPos;
    int length = aCursorSelection.iCursorPos - pos;

    QList<QInputMethodEvent::Attribute> attributes;
    attributes << QInputMethodEvent::Attribute(QInputMethodEvent::Selection, pos, length, QVariant());
    QInputMethodEvent event(m_preeditString, attributes);
    sendEvent(event);
}

void QCoeFepInputContext::GetCursorSelectionForFep(TCursorSelection& aCursorSelection) const
{
    QWidget *w = focusWidget();
    if (!w) {
        aCursorSelection.SetSelection(0,0);
        return;
    }

    int cursor = w->inputMethodQuery(Qt::ImCursorPosition).toInt() + m_preeditString.size();
    int anchor = w->inputMethodQuery(Qt::ImAnchorPosition).toInt() + m_preeditString.size();
    QString text = w->inputMethodQuery(Qt::ImSurroundingText).value<QString>();
    int combinedSize = text.size() + m_preeditString.size();
    if (combinedSize < anchor || combinedSize < cursor) {
        // ### TODO! FIXME! QTBUG-5050
        // This is a hack to prevent crashing in 4.6 with QLineEdits that use input masks.
        // The root problem is that cursor position is relative to displayed text instead of the
        // actual text we get.
        //
        // To properly fix this we would need to know the displayText of QLineEdits instead
        // of just the text, which on itself should be a trivial change. The difficulties start
        // when we need to commit the changes back to the QLineEdit, which would have to be somehow
        // able to handle displayText, too.
        //
        // Until properly fixed, the cursor and anchor positions will not reflect correct positions
        // for masked QLineEdits, unless all the masked positions are filled in order so that
        // cursor position relative to the displayed text matches position relative to actual text.
        aCursorSelection.iAnchorPos = combinedSize;
        aCursorSelection.iCursorPos = combinedSize;
    } else {
        aCursorSelection.iAnchorPos = anchor;
        aCursorSelection.iCursorPos = cursor;
    }
}

void QCoeFepInputContext::GetEditorContentForFep(TDes& aEditorContent, TInt aDocumentPosition,
        TInt aLengthToRetrieve) const
{
    QWidget *w = focusWidget();
    if (!w) {
        aEditorContent.FillZ(aLengthToRetrieve);
        return;
    }

    QString text = w->inputMethodQuery(Qt::ImSurroundingText).value<QString>();
    // FEP expects the preedit string to be part of the editor content, so let's mix it in.
    int cursor = w->inputMethodQuery(Qt::ImCursorPosition).toInt();
    text.insert(cursor, m_preeditString);
    aEditorContent.Copy(qt_QString2TPtrC(text.mid(aDocumentPosition, aLengthToRetrieve)));
}

void QCoeFepInputContext::GetFormatForFep(TCharFormat& aFormat, TInt /* aDocumentPosition */) const
{
    QWidget *w = focusWidget();
    if (!w) {
        aFormat = TCharFormat();
        return;
    }

    QFont font = w->inputMethodQuery(Qt::ImFont).value<QFont>();
    QFontMetrics metrics(font);
    //QString name = font.rawName();
    QString name = font.defaultFamily(); // TODO! FIXME! Should be the above.
    QHBufC hBufC(name);
    aFormat = TCharFormat(hBufC->Des(), metrics.height());
}

void QCoeFepInputContext::GetScreenCoordinatesForFepL(TPoint& aLeftSideOfBaseLine, TInt& aHeight,
        TInt& aAscent, TInt /* aDocumentPosition */) const
{
    QWidget *w = focusWidget();
    if (!w) {
        aLeftSideOfBaseLine = TPoint(0,0);
        aHeight = 0;
        aAscent = 0;
        return;
    }

    QRect rect = w->inputMethodQuery(Qt::ImMicroFocus).value<QRect>();
    aLeftSideOfBaseLine.iX = rect.left();
    aLeftSideOfBaseLine.iY = rect.bottom();

    QFont font = w->inputMethodQuery(Qt::ImFont).value<QFont>();
    QFontMetrics metrics(font);
    aHeight = metrics.height();
    aAscent = metrics.ascent();
}

void QCoeFepInputContext::DoCommitFepInlineEditL()
{
    commitCurrentString(false);
    if (QSysInfo::s60Version() > QSysInfo::SV_S60_5_0)
        ReportAknEdStateEvent(QT_EAknCursorPositionChanged);

}

void QCoeFepInputContext::commitCurrentString(bool cancelFepTransaction)
{
    QList<QInputMethodEvent::Attribute> attributes;
    QInputMethodEvent event(QLatin1String(""), attributes);
    event.setCommitString(m_preeditString, 0, 0);
    m_preeditString.clear();
    sendEvent(event);

    m_hasTempPreeditString = false;

    if (cancelFepTransaction) {
        CCoeFep* fep = CCoeEnv::Static()->Fep();
        if (fep)
            fep->CancelTransaction();
    }
}

MCoeFepAwareTextEditor_Extension1* QCoeFepInputContext::Extension1(TBool& aSetToTrue)
{
    aSetToTrue = ETrue;
    return this;
}

void QCoeFepInputContext::SetStateTransferingOwnershipL(MCoeFepAwareTextEditor_Extension1::CState* aState,
        TUid /*aTypeSafetyUid*/)
{
    // Note: The S60 docs are wrong! See the State() function.
    if (m_fepState)
        delete m_fepState;
    m_fepState = static_cast<CAknEdwinState *>(aState);
}

MCoeFepAwareTextEditor_Extension1::CState* QCoeFepInputContext::State(TUid /*aTypeSafetyUid*/)
{
    // Note: The S60 docs are horribly wrong when describing the
    // SetStateTransferingOwnershipL function and this function. They say that the former
    // sets a CState object identified by the TUid, and the latter retrieves it.
    // In reality, the CState is expected to always be a CAknEdwinState (even if it was not
    // previously set), and the TUid is ignored. All in all, there is a single CAknEdwinState
    // per QCoeFepInputContext, which should be deleted if the SetStateTransferingOwnershipL
    // function is used to set a new one.
    return m_fepState;
}

TTypeUid::Ptr QCoeFepInputContext::MopSupplyObject(TTypeUid /*id*/)
{
    return TTypeUid::Null();
}

MObjectProvider *QCoeFepInputContext::MopNext()
{
    QWidget *w = focusWidget();
    if (w)
        return w->effectiveWinId();
    return 0;
}

QT_END_NAMESPACE

#endif // QT_NO_IM
