/*
 *
 * Copyright 2010 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
 * @file        ULPAudioPlayer.h
 * @brief
 * @author      Yunji Kim (yunji.kim@samsung.com)
 *              HyeYeon Chung (hyeon.chung@samsung.com)
 * @version     2.0
 * @history
 *   2010.12.17 : Create
 *   2011.01.28 : Re-design for EOS non-blocking mode
 */

#ifndef ULP_AUDIO_PLAYER_H_

#define ULP_AUDIO_PLAYER_H_

#include <media/MediaPlayerInterface.h>
#include <media/stagefright/MediaBuffer.h>
#include <media/stagefright/TimeSource.h>
#include <utils/threads.h>

#include <media/stagefright/AudioPlayerBase.h>

namespace android {

class MediaSource;
class AudioTrack;
class AwesomePlayer;

class ULPAudioPlayer : public AudioPlayerBase {
public:
    enum {
        REACHED_EOS,
        SEEK_COMPLETE
    };

    typedef enum _ULPState{
        INIT = 1,
        START,
        PAUSE,
        STOP
    } ULPState;

    ULPAudioPlayer(const sp<MediaPlayerBase::AudioSink> &audioSink,
                AwesomePlayer *audioObserver = NULL);
    virtual ~ULPAudioPlayer();

    /* Caller retains ownership of "source". */
    void setSource(const sp<MediaSource> &source);

    /* Return time in us. */
    virtual int64_t getRealTimeUs();

    status_t start(bool sourceAlreadyStarted = false);

    void pause(bool playPendingSamples = false);
    void resume();

    /* Returns the timestamp of the last buffer played (in us). */
    int64_t getMediaTimeUs();

    /*
     * Returns true iff a mapping is established, i.e. the ULPAudioPlayer
     * has played at least one frame of audio.
     */
    bool getMediaTimeMapping(int64_t *realtime_us, int64_t *mediatime_us);

    status_t seekTo(int64_t time_us);

    bool isSeeking();
    bool reachedEOS(status_t *finalStatus);

    status_t initCheck() const;

private:
    ULPState mState;

    /**************************************************************************/
    /* a small internal class to handle the buffer thread
     **************************************************************************/
    class ULPAudioThread : public Thread
    {
    public:
        ULPAudioThread(ULPAudioPlayer& receiver, bool bCanCallJava = false);
    private:
        friend class ULPAudioPlayer;
        virtual bool        threadLoop();
        ULPAudioPlayer& mReceiver;
        Mutex       mLock;
    };

    bool processAudioBuffer(const sp<ULPAudioThread>& thread);

    sp<ULPAudioThread>    mULPAudioThread;
    /**************************************************************************/

    sp<MediaSource> mSource;
    AudioTrack *mAudioTrack;

    MediaBuffer *mInputBuffer;

    int mSampleRate;
    int64_t mLatencyUs;
    size_t mFrameSize;
    size_t mFrameCount;

    Mutex mLock;
    int64_t mNumFramesPlayed;

    int64_t mPositionTimeMediaUs;
    int64_t mPositionTimeRealUs;

    bool mSeeking;
    bool mReachedEOS;
    status_t mFinalStatus;
    int64_t mSeekTimeUs;

    bool mStarted;

    bool mIsFirstBuffer;
    status_t mFirstBufferResult;
    MediaBuffer *mFirstBuffer;

    AwesomePlayer *mObserver;

    /* for RP Driver */
    unsigned long decodedFrame;
    unsigned long decodedFrame_old;
    unsigned long framesWritten;
    unsigned long isRPStopped;
    bool isRPEos;
    int isRPEosDecode;
    bool isRPCreated;

    /* codec control to open pcm */
    unsigned char pAudioHWdata[128 * 1024];
    bool CodecOn;

    int64_t getRealTimeUsLocked() const;

    void reset();

    ULPAudioPlayer(const ULPAudioPlayer &);
    ULPAudioPlayer &operator=(const ULPAudioPlayer &);
};

}  /* namespace android */

#endif  /* ULP_AUDIO_PLAYER_H_ */
