/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the QtMultimedia module of the Qt Toolkit.
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

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists for the convenience
// of other Qt classes.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//


#ifndef QAUDIODEVICEINFOALSA_H
#define QAUDIODEVICEINFOALSA_H

#include <alsa/asoundlib.h>

#include <QtCore/qbytearray.h>
#include <QtCore/qstringlist.h>
#include <QtCore/qlist.h>
#include <QtCore/qdebug.h>

#include <QtMultimedia/qaudio.h>
#include <QtMultimedia/qaudiodeviceinfo.h>
#include <QtMultimedia/qaudioengine.h>

QT_BEGIN_NAMESPACE

const unsigned int MAX_SAMPLE_RATES = 5;
const unsigned int SAMPLE_RATES[] =
    { 8000, 11025, 22050, 44100, 48000 };

class QAudioDeviceInfoInternal : public QAbstractAudioDeviceInfo
{
    Q_OBJECT
public:
    QAudioDeviceInfoInternal(QByteArray dev,QAudio::Mode mode);
    ~QAudioDeviceInfoInternal();

    bool testSettings(const QAudioFormat& format) const;
    void updateLists();
    QAudioFormat preferredFormat() const;
    bool isFormatSupported(const QAudioFormat& format) const;
    QAudioFormat nearestFormat(const QAudioFormat& format) const;
    QString deviceName() const;
    QStringList codecList();
    QList<int> frequencyList();
    QList<int> channelsList();
    QList<int> sampleSizeList();
    QList<QAudioFormat::Endian> byteOrderList();
    QList<QAudioFormat::SampleType> sampleTypeList();
    static QByteArray defaultInputDevice();
    static QByteArray defaultOutputDevice();
    static QList<QByteArray> availableDevices(QAudio::Mode);

private:
    bool open();
    void close();

    void checkSurround();
    bool surround40;
    bool surround51;
    bool surround71;

    QString device;
    QAudio::Mode mode;
    QAudioFormat nearest;
    QList<int> freqz;
    QList<int> channelz;
    QList<int> sizez;
    QList<QAudioFormat::Endian> byteOrderz;
    QStringList codecz;
    QList<QAudioFormat::SampleType> typez;
    snd_pcm_t* handle;
    snd_pcm_hw_params_t *params;
};

QT_END_NAMESPACE

#endif

