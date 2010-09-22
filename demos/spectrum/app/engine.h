/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Nokia Corporation and its Subsidiary(-ies) nor
**     the names of its contributors may be used to endorse or promote
**     products derived from this software without specific prior written
**     permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef ENGINE_H
#define ENGINE_H

#include "spectrum.h"
#include "spectrumanalyser.h"
#include "wavfile.h"

#include <QObject>
#include <QByteArray>
#include <QBuffer>
#include <QVector>
#include <QtMultimedia/QAudioDeviceInfo>
#include <QtMultimedia/QAudioFormat>

#ifdef DUMP_CAPTURED_AUDIO
#define DUMP_DATA
#endif

#ifdef DUMP_SPECTRUM
#define DUMP_DATA
#endif

#ifdef DUMP_DATA
#include <QDir>
#endif

class FrequencySpectrum;
QT_FORWARD_DECLARE_CLASS(QAudioInput)
QT_FORWARD_DECLARE_CLASS(QAudioOutput)
QT_FORWARD_DECLARE_CLASS(QFile)

/**
 * This class interfaces with the QtMultimedia audio classes, and also with
 * the SpectrumAnalyser class.  Its role is to manage the capture and playback
 * of audio data, meanwhile performing real-time analysis of the audio level
 * and frequency spectrum.
 */
class Engine : public QObject {
    Q_OBJECT
public:
    Engine(QObject *parent = 0);
    ~Engine();

    const QList<QAudioDeviceInfo>& availableAudioInputDevices() const
                                    { return m_availableAudioInputDevices; }

    const QList<QAudioDeviceInfo>& availableAudioOutputDevices() const
                                    { return m_availableAudioOutputDevices; }

    QAudio::Mode mode() const       { return m_mode; }
    QAudio::State state() const     { return m_state; }

    /**
     * \return Reference to internal audio buffer
     * \note This reference is valid for the lifetime of the Engine
     */
    const QByteArray& buffer() const    { return m_buffer; }

    /**
     * \return Current audio format
     * \note May be QAudioFormat() if engine is not initialized
     */
    const QAudioFormat& format() const  { return m_format; }

    /**
     * Stop any ongoing recording or playback, and reset to ground state.
     */
    void reset();

    /**
     * Load data from WAV file
     */
    bool loadFile(const QString &fileName);

    /**
     * Generate tone
     */
    bool generateTone(const Tone &tone);

    /**
     * Generate tone
     */
    bool generateSweptTone(qreal amplitude);

    /**
     * Initialize for recording
     */
    bool initializeRecord();

    /**
     * Position of the audio input device.
     * \return Position in microseconds.
     */
    qint64 recordPosition() const   { return m_recordPosition; }

    /**
     * RMS level of the most recently processed set of audio samples.
     * \return Level in range (0.0, 1.0)
     */
    qreal rmsLevel() const          { return m_rmsLevel; }

    /**
     * Peak level of the most recently processed set of audio samples.
     * \return Level in range (0.0, 1.0)
     */
    qreal peakLevel() const         { return m_peakLevel; }

    /**
     * Position of the audio output device.
     * \return Position in microseconds.
     */
    qint64 playPosition() const     { return m_playPosition; }

    /**
     * Length of the internal engine buffer.
     * \return Buffer length in microseconds.
     */
    qint64 bufferDuration() const;

    /**
     * Amount of data held in the buffer.
     * \return Data duration in microseconds.
     */
    qint64 dataDuration() const;

    /**
     * Returns the size of the underlying audio buffer in bytes.
     * This should be an approximation of the capture latency.
     */
    qint64 audioBufferLength() const;

    /**
     * Set window function applied to audio data before spectral analysis.
     */
    void setWindowFunction(WindowFunction type);

public slots:
    void startRecording();
    void startPlayback();
    void suspend();
    void setAudioInputDevice(const QAudioDeviceInfo &device);
    void setAudioOutputDevice(const QAudioDeviceInfo &device);

signals:
    void stateChanged(QAudio::Mode mode, QAudio::State state);

    /**
     * Informational message for non-modal display
     */
    void infoMessage(const QString &message, int durationMs);

    /**
     * Error message for modal display
     */
    void errorMessage(const QString &heading, const QString &detail);

    /**
     * Format of audio data has changed
     */
    void formatChanged(const QAudioFormat &format);

    /**
     * Length of buffer has changed.
     * \param duration Duration in microseconds
     */
    void bufferDurationChanged(qint64 duration);

    /**
     * Amount of data in buffer has changed.
     * \param duration Duration of data in microseconds
     */
    void dataDurationChanged(qint64 duration);

    /**
     * Position of the audio input device has changed.
     * \param position Position in microseconds
     */
    void recordPositionChanged(qint64 position);

    /**
     * Position of the audio output device has changed.
     * \param position Position in microseconds
     */
    void playPositionChanged(qint64 position);

    /**
     * Level changed
     * \param rmsLevel RMS level in range 0.0 - 1.0
     * \param peakLevel Peak level in range 0.0 - 1.0
     * \param numSamples Number of audio samples analysed
     */
    void levelChanged(qreal rmsLevel, qreal peakLevel, int numSamples);

    /**
     * Spectrum has changed.
     * \param position Position of start of window in microseconds
     * \param length   Length of window in microseconds
     * \param spectrum Resulting frequency spectrum
     */
    void spectrumChanged(qint64 position, qint64 length, const FrequencySpectrum &spectrum);

private slots:
    void audioNotify();
    void audioStateChanged(QAudio::State state);
    void audioDataReady();
    void spectrumChanged(const FrequencySpectrum &spectrum);

private:
    bool initialize();
    bool selectFormat();
    void stopRecording();
    void stopPlayback();
    void setState(QAudio::State state);
    void setState(QAudio::Mode mode, QAudio::State state);
    void setFormat(const QAudioFormat &format);
    void setRecordPosition(qint64 position, bool forceEmit = false);
    void setPlayPosition(qint64 position, bool forceEmit = false);
    void calculateLevel(qint64 position, qint64 length);
    void calculateSpectrum(qint64 position);
    void setLevel(qreal rmsLevel, qreal peakLevel, int numSamples);

#ifdef DUMP_DATA
    void createOutputDir();
    QString outputPath() const { return m_outputDir.path(); }
#endif

#ifdef DUMP_CAPTURED_AUDIO
    void dumpData();
#endif

private:
    QAudio::Mode        m_mode;
    QAudio::State       m_state;

    bool                m_generateTone;
    SweptTone           m_tone;

    QFile*              m_file;
    WavFile             m_wavFile;

    QAudioFormat        m_format;

    const QList<QAudioDeviceInfo> m_availableAudioInputDevices;
    QAudioDeviceInfo    m_audioInputDevice;
    QAudioInput*        m_audioInput;
    QIODevice*          m_audioInputIODevice;
    qint64              m_recordPosition;

    const QList<QAudioDeviceInfo> m_availableAudioOutputDevices;
    QAudioDeviceInfo    m_audioOutputDevice;
    QAudioOutput*       m_audioOutput;
    qint64              m_playPosition;
    QBuffer             m_audioOutputIODevice;

    QByteArray          m_buffer;
    qint64              m_dataLength;

    qreal               m_rmsLevel;
    qreal               m_peakLevel;

    int                 m_spectrumLengthBytes;
    QByteArray          m_spectrumBuffer;
    SpectrumAnalyser    m_spectrumAnalyser;
    qint64              m_spectrumPosition;

    int                 m_count;

#ifdef DUMP_DATA
    QDir                m_outputDir;
#endif

};

#endif // ENGINE_H
