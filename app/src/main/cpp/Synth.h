#ifndef AUDIO_SYNTH_H
#define AUDIO_SYNTH_H

#include <array>
#include <TappableAudioSource.h>

#include <Oscillator.h>
#include <Mixer.h>
#include <MonoToStereo.h>
#include "AudioSource.h"

constexpr int kNumOscillators = 100;
constexpr float kOscBaseFrequency = 116.0;
constexpr float kOscDivisor = 33;
constexpr float kOscAmplitude = 0.009;


class Synth : public AudioSource {
public:

    Synth(int32_t sampleRate, int32_t channelCount) : AudioSource(sampleRate, channelCount) {
        for (int i = 0; i < kNumOscillators; ++i) {
            mOscs[i].setSampleRate(mSampleRate);
            mOscs[i].setFrequency(frequency + (static_cast<float>(i) / kOscDivisor));
            mOscs[i].setAmplitude(kOscAmplitude);
            mMixer.addTrack(&mOscs[i]);
        }
        if (mChannelCount == oboe::ChannelCount::Stereo) {
            mOutputStage = &mConverter;
        } else {
            mOutputStage = &mMixer;
        }
    }

    void tap(bool isOn) override {
        for (auto &osc : mOscs) osc.setWaveOn(isOn);
    };

    // From IRenderableAudio
    void renderAudio(float *audioData, int32_t numFrames) override {
        mOutputStage->renderAudio(audioData, numFrames);
    };

    void setFrequency(float value) override {
        frequency = value;
        for (int i = 0; i < kNumOscillators; ++i) {
            mOscs[i].setFrequency(frequency + (static_cast<float>(i) / kOscDivisor));
        }

    }

    virtual ~Synth() {}

private:
    // Rendering objects
    float frequency = kOscBaseFrequency;
    std::array<Oscillator, kNumOscillators> mOscs;
    Mixer mMixer;
    MonoToStereo mConverter = MonoToStereo(&mMixer);
    IRenderableAudio *mOutputStage; // This will point to either the mixer or converter, so it needs to be raw
};


#endif //AUDIO_SYNTH_H
