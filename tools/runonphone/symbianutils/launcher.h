/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the tools applications of the Qt Toolkit.
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

#ifndef LAUNCHER_H
#define LAUNCHER_H

#include "trkdevice.h"
#include "trkutils.h"

#include <QtCore/QObject>
#include <QtCore/QVariant>
#include <QtCore/QSharedPointer>

namespace trk {

struct TrkResult;
struct TrkMessage;
struct LauncherPrivate;

typedef QSharedPointer<TrkDevice> TrkDevicePtr;

class SYMBIANUTILS_EXPORT Launcher : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(Launcher)
public:
    typedef void (Launcher::*TrkCallBack)(const TrkResult &);

    enum Actions {
        ActionPingOnly = 0x0,
        ActionCopy = 0x1,
        ActionInstall = 0x2,
        ActionCopyInstall = ActionCopy | ActionInstall,
        ActionRun = 0x4,
        ActionDownload = 0x8,
        ActionCopyRun = ActionCopy | ActionRun,
        ActionInstallRun = ActionInstall | ActionRun,
        ActionCopyInstallRun = ActionCopy | ActionInstall | ActionRun
    };

    enum State { Disconnected, Connecting, Connected,
                 WaitingForTrk, // This occurs only if the initial ping times out after
                                // a reasonable timeout, indicating that Trk is not
                                // running. Note that this will never happen with
                                // Bluetooth as communication immediately starts
                                // after connecting.
                 DeviceDescriptionReceived };

    explicit Launcher(trk::Launcher::Actions startupActions = trk::Launcher::ActionPingOnly,
                      const TrkDevicePtr &trkDevice = TrkDevicePtr(),
                      QObject *parent = 0);
    ~Launcher();

    State state() const;

    void addStartupActions(trk::Launcher::Actions startupActions);
    void setTrkServerName(const QString &name);
    QString trkServerName() const;
    void setFileName(const QString &name);
    void setCopyFileName(const QString &srcName, const QString &dstName);
    void setDownloadFileName(const QString &srcName, const QString &dstName);
    void setInstallFileName(const QString &name);
    void setCommandLineArgs(const QStringList &args);
    bool startServer(QString *errorMessage);
    void setVerbose(int v);
    void setSerialFrame(bool b);
    bool serialFrame() const;
    // Close device or leave it open
    bool closeDevice() const;
    void setCloseDevice(bool c);

    TrkDevicePtr trkDevice() const;

    // becomes valid after successful execution of ActionPingOnly
    QString deviceDescription(unsigned verbose = 0u) const;

    // Acquire a device from SymbianDeviceManager, return 0 if not available.
    // The device will be released on destruction.
    static Launcher *acquireFromDeviceManager(const QString &serverName,
                                              QObject *parent,
                                              QString *errorMessage);
    // Preliminary release of device, disconnecting the signal.
    static void releaseToDeviceManager(Launcher *l);

    // Create Trk message to start a process.
    static QByteArray startProcessMessage(const QString &executable,
                                          const QStringList &arguments);
    // Create Trk message to read memory
    static QByteArray readMemoryMessage(uint pid, uint tid, uint from, uint len);
    static QByteArray readRegistersMessage(uint pid, uint tid);
    // Parse a TrkNotifyStopped message
    static bool parseNotifyStopped(const QByteArray &a,
                                   uint *pid, uint *tid, uint *address,
                                   QString *why = 0);
    // Helper message
    static QString msgStopped(uint pid, uint tid, uint address, const QString &why);

signals:
    void deviceDescriptionReceived(const QString &port, const QString &description);
    void copyingStarted();
    void canNotConnect(const QString &errorMessage);
    void canNotCreateFile(const QString &filename, const QString &errorMessage);
    void canNotOpenFile(const QString &filename, const QString &errorMessage);
    void canNotOpenLocalFile(const QString &filename, const QString &errorMessage);
    void canNotWriteFile(const QString &filename, const QString &errorMessage);
    void canNotCloseFile(const QString &filename, const QString &errorMessage);
    void installingStarted();
    void canNotInstall(const QString &packageFilename, const QString &errorMessage);
    void installingFinished();
    void startingApplication();
    void applicationRunning(uint pid);
    void canNotRun(const QString &errorMessage);
    void finished();
    void applicationOutputReceived(const QString &output);
    void copyProgress(int percent);
    void stateChanged(int);
    void processStopped(uint pc, uint pid, uint tid, const QString& reason);
    void processResumed(uint pid, uint tid);
    void libraryLoaded(const trk::Library &lib);
    void libraryUnloaded(const trk::Library &lib);
    void registersAndCallStackReadComplete(const QList<uint>& registers, const QByteArray& stack);
    // Emitted by the destructor, for releasing devices of SymbianDeviceManager by name
    void destroyed(const QString &serverName);

public slots:
    void terminate();
    void resumeProcess(uint pid, uint tid);
    //can be used to obtain traceback after a breakpoint / exception
    void getRegistersAndCallStack(uint pid, uint tid);

private slots:
    void handleResult(const trk::TrkResult &data);
    void slotWaitingForTrk();

private:
    // kill process and breakpoints
    void cleanUp();
    void disconnectTrk();

    void handleRemoteProcessKilled(const TrkResult &result);
    void handleConnect(const TrkResult &result);
    void handleFileCreation(const TrkResult &result);
    void handleFileOpened(const TrkResult &result);
    void handleCopy(const TrkResult &result);
    void handleRead(const TrkResult &result);
    void continueCopying(uint lastCopiedBlockSize = 0);
    void continueReading();
    void closeRemoteFile(bool failed = false);
    void handleFileCopied(const TrkResult &result);
    void handleInstallPackageFinished(const TrkResult &result);
    void handleCpuType(const TrkResult &result);
    void handleCreateProcess(const TrkResult &result);
    void handleWaitForFinished(const TrkResult &result);
    void handleStop(const TrkResult &result);
    void handleSupportMask(const TrkResult &result);
    void handleTrkVersion(const TrkResult &result);
    void handleReadRegisters(const TrkResult &result);
    void handleReadStack(const TrkResult &result);

    void copyFileToRemote();
    void copyFileFromRemote();
    void installRemotePackageSilently();
    void startInferiorIfNeeded();
    void handleFinished();

    void logMessage(const QString &msg);
    void setState(State s);

    LauncherPrivate *d;
};

} // namespace Trk

#endif // LAUNCHER_H
