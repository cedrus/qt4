/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the Qt Assistant of the Qt Toolkit.
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

#ifndef REMOTECONTROL_H
#define REMOTECONTROL_H

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QUrl>

QT_BEGIN_NAMESPACE

class HelpEngineWrapper;
class MainWindow;

class RemoteControl : public QObject
{
    Q_OBJECT

public:
    RemoteControl(MainWindow *mainWindow);

private slots:
    void receivedData();
    void handleCommandString(const QString &cmdString);
    void applyCache();

private:
    void clearCache();
    void splitInputString(const QString &input, QString &cmd, QString &arg);
    void handleDebugCommand(const QString &arg);
    void handleShowOrHideCommand(const QString &arg, bool show);
    void handleSetSourceCommand(const QString &arg);
    void handleSyncContentsCommand();
    void handleActivateKeywordCommand(const QString &arg);
    void handleActivateIdentifierCommand(const QString &arg);
    void handleExpandTocCommand(const QString &arg);
    void handleSetCurrentFilterCommand(const QString &arg);
    void handleRegisterCommand(const QString &arg);
    void handleUnregisterCommand(const QString &arg);

private:
    MainWindow *m_mainWindow;
    bool m_debug;

    bool m_caching;
    QUrl m_setSource;
    bool m_syncContents;
    QString m_activateKeyword;
    QString m_activateIdentifier;
    int m_expandTOC;
    QString m_currentFilter;
    HelpEngineWrapper &helpEngine;
};

QT_END_NAMESPACE

#endif
