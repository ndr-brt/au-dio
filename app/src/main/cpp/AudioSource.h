#ifndef AUDIO_SOURCE_H
#define AUDIO_SOURCE_H

#include "../../../../oboe/samples/shared/IRenderableAudio.h"
#include "../../../../oboe/samples/shared/ITappable.h"

/**
 * This class renders Float audio, but can be tapped to control.
 * It also contains members for sample rate and channel count
 */
class AudioSource : public IRenderableAudio, public ITappable {
public:
    AudioSource(int sampleRate, int channelCount) :
            mSampleRate(sampleRate), mChannelCount(channelCount) { }

    virtual void setFrequency(float value) = 0;
    int mSampleRate;
    int mChannelCount;
};

#endif //AUDIO_SOURCE_H