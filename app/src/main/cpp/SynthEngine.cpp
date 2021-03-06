#include <memory>
#include "SynthEngine.h"

/**
 * Main audio engine. It is responsible for:
 *
 * - Creating the callback object which will be supplied when constructing the audio stream
 * - Setting the CPU core IDs to which the callback thread should bind to
 * - Creating the playback stream, including setting the callback object
 * - Creating `Synth` which will render the audio inside the callback
 * - Starting the playback stream
 * - Restarting the playback stream when `restart()` is called by the callback object
 *
 * @param cpuIds
 */
SynthEngine::SynthEngine(std::vector<int> cpuIds) {

    createCallback(cpuIds);
    start();
}

void SynthEngine::tap(bool isDown) {
    mAudioSource->tap(isDown);
}

void SynthEngine::restart() {
    start();
}

// Create the playback stream
oboe::Result SynthEngine::createPlaybackStream() {
    oboe::AudioStreamBuilder builder;
    return builder.setSharingMode(oboe::SharingMode::Exclusive)
            ->setPerformanceMode(oboe::PerformanceMode::LowLatency)
            ->setFormat(oboe::AudioFormat::Float)
            ->setCallback(mCallback.get())
            ->openManagedStream(mStream);
}

// Create the callback and set its thread affinity to the supplied CPU core IDs
void SynthEngine::createCallback(std::vector<int> cpuIds){
    // Create the callback, we supply ourselves as the parent so that we can restart the stream
    // when it's disconnected
    mCallback = std::make_unique<DefaultAudioStreamCallback>(*this);

    // Bind the audio callback to specific CPU cores as this can help avoid underruns caused by
    // core migrations
    mCallback->setCpuIds(cpuIds);
    mCallback->setThreadAffinityEnabled(true);
}

void SynthEngine::start(){
    auto result = createPlaybackStream();
    if (result == Result::OK){
        // Create our synthesizer audio source using the properties of the stream
        mAudioSource = std::make_shared<Synth>(mStream->getSampleRate(), mStream->getChannelCount());
        mCallback->setSource(std::dynamic_pointer_cast<IRenderableAudio>(mAudioSource));
        mStream->start();
    } else {
        LOGE("Failed to create the playback stream. Error: %s", convertToText(result));
    }
}

void SynthEngine::setFrequency(float value) {
    mAudioSource->setFrequency(value);
}
