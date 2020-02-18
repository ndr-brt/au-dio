#ifndef SYNTH_ENGINE_H
#define SYNTH_ENGINE_H


#include <oboe/Oboe.h>
#include <vector>

#include "Synth.h"
#include <DefaultAudioStreamCallback.h>
#include <TappableAudioSource.h>
#include <IRestartable.h>

using namespace oboe;

class SynthEngine : public IRestartable {

public:
    SynthEngine(std::vector<int> cpuIds);

    virtual ~SynthEngine() = default;

    void tap(bool isDown);

    // from IRestartable
    virtual void restart() override;

private:
    oboe::ManagedStream mStream;
    std::shared_ptr<TappableAudioSource> mAudioSource;
    std::unique_ptr<DefaultAudioStreamCallback> mCallback;

    oboe::Result createPlaybackStream();
    void createCallback(std::vector<int> cpuIds);
    void start();
};


#endif //SYNTH_ENGINE_H
