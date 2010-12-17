/*  This file is part of the KDE project.

Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).

This library is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 2.1 or 3 of the License.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with this library.  If not, see <http://www.gnu.org/licenses/>.

*/

#include <QResource>
#include <QUrl>

#include "abstractmediaplayer.h"
#include "defs.h"
#include "mediaobject.h"
#include "utils.h"

QT_BEGIN_NAMESPACE

using namespace Phonon;
using namespace Phonon::MMF;

/*! \class MMF::AbstractMediaPlayer
  \internal
*/

//-----------------------------------------------------------------------------
// Constants
//-----------------------------------------------------------------------------

const int       NullMaxVolume = -1;
const int       BufferStatusTimerInterval = 100; // ms


//-----------------------------------------------------------------------------
// Constructor / destructor
//-----------------------------------------------------------------------------

MMF::AbstractMediaPlayer::AbstractMediaPlayer
    (MediaObject *parent, const AbstractPlayer *player)
        :   AbstractPlayer(player)
        ,   m_parent(parent)
        ,   m_pending(NothingPending)
        ,   m_positionTimer(new QTimer(this))
        ,   m_bufferStatusTimer(new QTimer(this))
        ,   m_mmfMaxVolume(NullMaxVolume)
        ,   m_prefinishMarkSent(false)
        ,   m_aboutToFinishSent(false)
{
    connect(m_positionTimer.data(), SIGNAL(timeout()), this, SLOT(positionTick()));
    connect(m_bufferStatusTimer.data(), SIGNAL(timeout()), this, SLOT(bufferStatusTick()));
}

//-----------------------------------------------------------------------------
// MediaObjectInterface
//-----------------------------------------------------------------------------

void MMF::AbstractMediaPlayer::play()
{
    TRACE_CONTEXT(AbstractMediaPlayer::play, EAudioApi);
    TRACE_ENTRY("state %d", privateState());

    switch (privateState()) {
    case GroundState:
        setError(tr("Not ready to play"));
        break;

    case LoadingState:
        setPending(PlayPending);
        break;

    case StoppedState:
    case PausedState:
        startPlayback();
        break;

    case PlayingState:
    case BufferingState:
    case ErrorState:
        // Do nothing
        break;

        // Protection against adding new states and forgetting to update this switch
    default:
        TRACE_PANIC(InvalidStatePanic);
    }

    TRACE_EXIT("state %d", privateState());
}

void MMF::AbstractMediaPlayer::pause()
{
    TRACE_CONTEXT(AbstractMediaPlayer::pause, EAudioApi);
    TRACE_ENTRY("state %d", privateState());

    stopTimers();

    switch (privateState()) {
    case GroundState:
    case LoadingState:
    case StoppedState:
        setPending(PausePending);
        break;

    case PausedState:
        // Do nothing
        break;

    case PlayingState:
    case BufferingState:
        changeState(PausedState);
        // Fall through
    case ErrorState:
        doPause();
        break;

        // Protection against adding new states and forgetting to update this switch
    default:
        TRACE_PANIC(InvalidStatePanic);
    }

    TRACE_EXIT("state %d", privateState());
}

void MMF::AbstractMediaPlayer::stop()
{
    TRACE_CONTEXT(AbstractMediaPlayer::stop, EAudioApi);
    TRACE_ENTRY("state %d", privateState());

    setPending(NothingPending);
    stopTimers();

    switch (privateState()) {
    case GroundState:
    case LoadingState:
    case StoppedState:
    case ErrorState:
        // Do nothing
        break;

    case PlayingState:
    case BufferingState:
    case PausedState:
        doStop();
        changeState(StoppedState);
        break;

        // Protection against adding new states and forgetting to update this switch
    default:
        TRACE_PANIC(InvalidStatePanic);
    }

    TRACE_EXIT("state %d", privateState());
}

void MMF::AbstractMediaPlayer::seek(qint64 ms)
{
    TRACE_CONTEXT(AbstractMediaPlayer::seek, EAudioApi);
    TRACE_ENTRY("state %d pos %Ld", state(), ms);

    switch (privateState()) {
    // Fallthrough all these
    case GroundState:
    case StoppedState:
    case PausedState:
    case PlayingState:
    case LoadingState:
    {
        bool wasPlaying = false;
        if (state() == PlayingState) {
            stopPositionTimer();
            doPause();
            wasPlaying = true;
        }

        doSeek(ms);
        resetMarksIfRewound();

        if(wasPlaying && state() != ErrorState) {
            doPlay();
            startPositionTimer();
        }

        break;
    }
    case BufferingState:
    // Fallthrough
    case ErrorState:
        // Do nothing
        break;
    }

    TRACE_EXIT_0();
}

bool MMF::AbstractMediaPlayer::isSeekable() const
{
    return true;
}

void MMF::AbstractMediaPlayer::doSetTickInterval(qint32 interval)
{
    TRACE_CONTEXT(AbstractMediaPlayer::doSetTickInterval, EAudioApi);
    TRACE_ENTRY("state %d m_interval %d interval %d", privateState(), tickInterval(), interval);

    m_positionTimer->setInterval(interval);

    TRACE_EXIT_0();
}

void MMF::AbstractMediaPlayer::open()
{
    TRACE_CONTEXT(AbstractMediaPlayer::open, EAudioApi);
    const MediaSource source = m_parent->source();
    TRACE_ENTRY("state %d source.type %d", privateState(), source.type());

    close();
    changeState(GroundState);

    TInt symbianErr = KErrNone;
    QString errorMessage;

    switch (source.type()) {
    case MediaSource::LocalFile: {
        RFile *const file = m_parent->file();
        Q_ASSERT(file);
        symbianErr = openFile(*file);
        if (KErrNone != symbianErr)
            errorMessage = tr("Error opening file");
        break;
    }

    case MediaSource::Url: {
        const QUrl url(source.url());
        if (url.scheme() == QLatin1String("file")) {
            RFile *const file = m_parent->file();
            Q_ASSERT(file);
            symbianErr = openFile(*file);
            if (KErrNone != symbianErr)
                errorMessage = tr("Error opening file");
        } else {
            symbianErr = openUrl(url.toString());
            if (KErrNone != symbianErr)
                errorMessage = tr("Error opening URL");
        }

        break;
    }

    case MediaSource::Stream: {
        QResource *const resource = m_parent->resource();
        if (resource) {
            m_buffer.Set(resource->data(), resource->size());
            symbianErr = openDescriptor(m_buffer);
            if (KErrNone != symbianErr)
                errorMessage = tr("Error opening resource");
        } else {
            errorMessage = tr("Error opening source: resource not opened");
        }
        break;
    }

    // Other source types are handled in MediaObject::createPlayer

    // Protection against adding new media types and forgetting to update this switch
    default:
        TRACE_PANIC(InvalidMediaTypePanic);
    }

    if (errorMessage.isEmpty()) {
        changeState(LoadingState);
    } else {
        if (symbianErr)
            setError(errorMessage, symbianErr);
        else
            setError(errorMessage);
    }

    TRACE_EXIT_0();
}

void MMF::AbstractMediaPlayer::volumeChanged(qreal volume)
{
    TRACE_CONTEXT(AbstractMediaPlayer::volumeChanged, EAudioInternal);
    TRACE_ENTRY("state %d", privateState());

    AbstractPlayer::volumeChanged(volume);
    doVolumeChanged();

    TRACE_EXIT_0();
}

//-----------------------------------------------------------------------------
// Private functions
//-----------------------------------------------------------------------------

void MMF::AbstractMediaPlayer::startPositionTimer()
{
    m_positionTimer->start(tickInterval());
}

void MMF::AbstractMediaPlayer::stopPositionTimer()
{
    m_positionTimer->stop();
}

void MMF::AbstractMediaPlayer::startBufferStatusTimer()
{
    m_bufferStatusTimer->start(BufferStatusTimerInterval);
}

void MMF::AbstractMediaPlayer::stopBufferStatusTimer()
{
    m_bufferStatusTimer->stop();
}

void MMF::AbstractMediaPlayer::stopTimers()
{
    stopPositionTimer();
    stopBufferStatusTimer();
}

void MMF::AbstractMediaPlayer::doVolumeChanged()
{
    switch (privateState()) {
    case GroundState:
    case LoadingState:
    case ErrorState:
        // Do nothing
        break;

    case StoppedState:
    case PausedState:
    case PlayingState:
    case BufferingState: {
        const qreal volume = (m_volume * m_mmfMaxVolume) + 0.5;
        const int err = setDeviceVolume(volume);

        if (KErrNone != err) {
            setError(tr("Setting volume failed"), err);
        }
        break;
    }

    // Protection against adding new states and forgetting to update this
    // switch
    default:
        Utils::panic(InvalidStatePanic);
    }
}

//-----------------------------------------------------------------------------
// Protected functions
//-----------------------------------------------------------------------------

void MMF::AbstractMediaPlayer::bufferingStarted()
{
    m_stateBeforeBuffering = privateState();
    changeState(BufferingState);
    bufferStatusTick();
    startBufferStatusTimer();
}

void MMF::AbstractMediaPlayer::bufferingComplete()
{
    stopBufferStatusTimer();
    emit MMF::AbstractPlayer::bufferStatus(100);
    changeState(m_stateBeforeBuffering);
}

void MMF::AbstractMediaPlayer::maxVolumeChanged(int mmfMaxVolume)
{
    m_mmfMaxVolume = mmfMaxVolume;
    doVolumeChanged();
}

void MMF::AbstractMediaPlayer::loadingComplete(int error)
{
    Q_ASSERT(Phonon::LoadingState == state());

    if (KErrNone == error) {
        updateMetaData();
        changeState(StoppedState);
    } else {
        setError(tr("Loading clip failed"), error);
    }
}

void MMF::AbstractMediaPlayer::playbackComplete(int error)
{
    stopTimers();

    if (KErrNone == error && !m_aboutToFinishSent) {
        const qint64 total = totalTime();
        emit MMF::AbstractPlayer::tick(total);
        m_aboutToFinishSent = true;
        emit aboutToFinish();
    }

    if (KErrNone == error) {
        changeState(PausedState);

        // MediaObject::switchToNextSource deletes the current player, so we
        // call it via delayed slot invokation to ensure that this object does
        // not get deleted during execution of a member function.
        QMetaObject::invokeMethod(m_parent, "switchToNextSource", Qt::QueuedConnection);
    }
    else {
        setError(tr("Playback complete"), error);
        emit finished();
    }
}

qint64 MMF::AbstractMediaPlayer::toMilliSeconds(const TTimeIntervalMicroSeconds &in)
{
    return in.Int64() / 1000;
}

//-----------------------------------------------------------------------------
// Slots
//-----------------------------------------------------------------------------

void MMF::AbstractMediaPlayer::positionTick()
{
    const qint64 current = currentTime();
    emitMarksIfReached(current);
    emit MMF::AbstractPlayer::tick(current);
}

void MMF::AbstractMediaPlayer::emitMarksIfReached(qint64 current)
{
    const qint64 total = totalTime();
    const qint64 remaining = total - current;

    if (prefinishMark() && !m_prefinishMarkSent) {
        if (remaining < (prefinishMark() + tickInterval()/2)) {
            m_prefinishMarkSent = true;
            emit prefinishMarkReached(remaining);
        }
    }

    if (!m_aboutToFinishSent) {
        if (remaining < tickInterval()) {
            m_aboutToFinishSent = true;
            emit aboutToFinish();
        }
    }
}

void MMF::AbstractMediaPlayer::resetMarksIfRewound()
{
    const qint64 current = currentTime();
    const qint64 total = totalTime();
    const qint64 remaining = total - current;

    if (prefinishMark() && m_prefinishMarkSent)
        if (remaining >= (prefinishMark() + tickInterval()/2))
            m_prefinishMarkSent = false;

    if (m_aboutToFinishSent)
        if (remaining >= tickInterval())
            m_aboutToFinishSent = false;
}

void MMF::AbstractMediaPlayer::setPending(Pending pending)
{
    const Phonon::State oldState = state();
    m_pending = pending;
    const Phonon::State newState = state();
    if (newState != oldState)
        emit stateChanged(newState, oldState);
}

void MMF::AbstractMediaPlayer::startPlayback()
{
    doPlay();
    startPositionTimer();
    changeState(PlayingState);
}

void MMF::AbstractMediaPlayer::bufferStatusTick()
{
    emit MMF::AbstractPlayer::bufferStatus(bufferStatus());
}

Phonon::State MMF::AbstractMediaPlayer::phononState(PrivateState state) const
{
    Phonon::State result = AbstractPlayer::phononState(state);

    if (PausePending == m_pending) {
        Q_ASSERT(Phonon::StoppedState == result || Phonon::LoadingState == result);
        result = Phonon::PausedState;
    }

    return result;
}

void MMF::AbstractMediaPlayer::changeState(PrivateState newState)
{
    TRACE_CONTEXT(AbstractMediaPlayer::changeState, EAudioInternal);

    const Phonon::State oldPhononState = phononState(privateState());
    const Phonon::State newPhononState = phononState(newState);

    if (LoadingState == oldPhononState && StoppedState == newPhononState) {
        switch (m_pending) {
        case NothingPending:
            AbstractPlayer::changeState(newState);
            break;

        case PlayPending:
            changeState(PlayingState); // necessary in order to apply initial volume
            doVolumeChanged();
            startPlayback();
            break;

        case PausePending:
            AbstractPlayer::changeState(PausedState);
            break;
        }

        setPending(NothingPending);
    } else {
        AbstractPlayer::changeState(newState);
    }
}

void MMF::AbstractMediaPlayer::updateMetaData()
{
    TRACE_CONTEXT(AbstractMediaPlayer::updateMetaData, EAudioInternal);
    TRACE_ENTRY_0();

    m_metaData.clear();

    const int numberOfEntries = numberOfMetaDataEntries();
    for(int i=0; i<numberOfEntries; ++i) {
        const QPair<QString, QString> entry = metaDataEntry(i);

        // Note that we capitalize the key, as required by the Ogg Vorbis
        // metadata standard to which Phonon adheres:
        // http://xiph.org/vorbis/doc/v-comment.html
        m_metaData.insert(entry.first.toUpper(), entry.second);
    }

    emit metaDataChanged(m_metaData);

    TRACE_EXIT_0();
}

QT_END_NAMESPACE

