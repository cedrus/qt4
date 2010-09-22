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

#include "qfont.h"
#include <private/qt_s60_p.h>
#include <private/qpixmap_s60_p.h>
#include "qmutex.h"

QT_BEGIN_NAMESPACE

#if 1
#ifdef QT_NO_FREETYPE
Q_GLOBAL_STATIC(QMutex, lastResortFamilyMutex);
#endif // QT_NO_FREETYPE

QString QFont::lastResortFamily() const
{
#ifdef QT_NO_FREETYPE
    QMutexLocker locker(lastResortFamilyMutex());
    static QString family;
    if (family.isEmpty()) {
        
        QSymbianFbsHeapLock lock(QSymbianFbsHeapLock::Unlock);
        
        CFont *font;
        const TInt err = S60->screenDevice()->GetNearestFontInTwips(font, TFontSpec());
        Q_ASSERT(err == KErrNone);
        const TFontSpec spec = font->FontSpecInTwips();
        family = QString((const QChar *)spec.iTypeface.iName.Ptr(), spec.iTypeface.iName.Length());
        S60->screenDevice()->ReleaseFont(font);
        
        lock.relock();
    }
    return family;
#else
    // For the FreeType case we just hard code the face name, since otherwise on
    // East Asian systems we may get a name for a stroke based (non-ttf) font.

    // TODO: Get the type face name in a proper way

    const bool isJapaneseOrChineseSystem =
        User::Language() == ELangJapanese || User::Language() == ELangPrcChinese;

    return QLatin1String(isJapaneseOrChineseSystem?"Heisei Kaku Gothic S60":"Series 60 Sans");
#endif // QT_NO_FREETYPE
}
#else // 0
QString QFont::lastResortFamily() const
{
    return QLatin1String("Series 60 Sans");
}
#endif // 0

QString QFont::defaultFamily() const
{
    return lastResortFamily();
}

QT_END_NAMESPACE
