#include <jni.h>
#include <string>
#include <vector>

#include "SynthEngine.h"


std::vector<int> convertJavaArrayToVector(JNIEnv *env, jintArray intArray) {
    std::vector<int> v;
    jsize length = env->GetArrayLength(intArray);
    if (length > 0) {
        jint *elements = env->GetIntArrayElements(intArray, nullptr);
        v.insert(v.end(), &elements[0], &elements[length]);
        // Unpin the memory for the array, or free the copy.
        env->ReleaseIntArrayElements(intArray, elements, 0);
    }
    return v;
}

extern "C" {
/**
 * Start the audio engine
 *
 * @param env
 * @param instance
 * @param jCpuIds - CPU core IDs which the audio process should affine to
 * @return a pointer to the audio engine. This should be passed to other methods
 */
JNIEXPORT jlong JNICALL
Java_ndr_brt_audio_MainActivity_startEngine(JNIEnv *env, jobject /*unused*/,
                                                         jintArray jCpuIds) {
    std::vector<int> cpuIds = convertJavaArrayToVector(env, jCpuIds);
    LOGD("cpu ids size: %d", static_cast<int>(cpuIds.size()));
    SynthEngine  *engine = new SynthEngine(std::move(cpuIds));
    LOGD("Engine Started");
    return reinterpret_cast<jlong>(engine);
}

JNIEXPORT void JNICALL
Java_ndr_brt_audio_MainActivity_stopEngine(JNIEnv *env, jobject instance,
        jlong jEngineHandle) {
    auto engine = reinterpret_cast<SynthEngine*>(jEngineHandle);
    if (engine) {
        delete engine;
    } else {
        LOGD("Engine invalid, call startEngine() to create");
    }
}


JNIEXPORT void JNICALL
Java_ndr_brt_audio_MainActivity_tap(JNIEnv *env, jobject instance,
        jlong jEngineHandle, jboolean isDown) {

    auto *engine = reinterpret_cast<SynthEngine*>(jEngineHandle);
    if (engine) {
        engine->tap(isDown);
    } else {
        LOGE("Engine handle is invalid, call createEngine() to create a new one");
    }
}

JNIEXPORT void JNICALL
Java_ndr_brt_audio_MainActivity_native_1setDefaultStreamValues(JNIEnv *env,
                                                                            jclass type,
                                                                            jint sampleRate,
                                                                            jint framesPerBurst) {
    oboe::DefaultStreamValues::SampleRate = (int32_t) sampleRate;
    oboe::DefaultStreamValues::FramesPerBurst = (int32_t) framesPerBurst;
}

} // extern "C"
extern "C"
JNIEXPORT void JNICALL
Java_ndr_brt_audio_MainActivity_setFrequency(JNIEnv *env, jobject thiz, jlong jEngineHandle, jfloat frequency) {
    auto engine = reinterpret_cast<SynthEngine*>(jEngineHandle);
    if (engine) {
        engine->setFrequency(frequency);
    } else {
        LOGE("Engine handle is invalid, call createEngine() to create a new one");
    }
}