#include "log.h"
#include <SLES/OpenSLES.h>


/*
    Object 和 Interface
    OpenSL ES 有两个重要的概念 Object 和 Interface，“对象”和“接口”。
    (1) 每个 Object 可能会存在一个或者多个 Interface，官方为每一种 Object 都定义了一系列的 Interface。
    (2) 每个 Object 对象都提供了一些最基础的操作，比如：Realize，Resume，GetState，Destroy 等等，如果希望使用该对象支持的功能函数，则必须通过其 GetInterface 函数拿到 Interface 接口，然后通过 Interface 来访问功能函数。
    (3) 并不是每个系统上都实现了 OpenSL ES 为 Object 定义的所有 Interface，所以在获取 Interface 的时候需要做一些选择和判断。
 */

/*

    任何一个 OpenSL ES 的对象，创建成功后，都进入 SL_OBJECT_STATE_UNREALIZED 状态，这种状态下，系统不会为它分配任何资源。

    Realize 后的对象，就会进入 SL_OBJECT_STATE_REALIZED 状态，这是一种“可用”的状态，只有在这种状态下，对象的各个功能和资源才能正常地访问。
    当一些系统事件发生后，比如出现错误或者 Audio 设备被其他应用抢占，OpenSL ES 对象会进入 SL_OBJECT_STATE_SUSPENDED 状态，如果希望恢复正常使用，需要调用 Resume 函数。

    当调用对象的 Destroy 函数后，则会释放资源，并回到 SL_OBJECT_STATE_UNREALIZED 状态。
    Engine对象是OpenSL ES API的入口点，这个对象使你能够创建OpenSL ES中所有其他对象。
    Engine对象由全局的对象slCreateEngine()创建得到，创建的结果是Engine对象的一个SLObjectItf的接口。

 */


/*

Engine Object 和 SLEngineItf Interface
Engine Object是OpenSL ES 里面最核心的对象
它主要提供如下两个功能：
    (1) 管理 Audio Engine 的生命周期。
    (2) 提供管理接口: SLEngineItf，该接口可以用来创建所有其他的 Object 对象。
    (3) 提供设备属性查询接口：SLEngineCapabilitiesItf 和 SLAudioIODeviceCapabilitiesItf，这些接口可以查询设备的一些属性信息。

Engine Object创建方式：
    // 创建
    SLObjectItf engineObject;
    slCreateEngine(&engineObject, 0, nullptr, 0, nullptr, nullptr);
    // 初始化
    (*engineObject)->Realize(engineObject, SL_BOOLEAN_FALSE);
    // 销毁
    (*engineObject)->Destroy(engineObject);

slCreateEngine参数说明：
    pEngine：指向输出的engine对象的指针。
    numOptions：可选配置数组的大小。
    pEngineOptions：可选配置数组。
    numInterfaces：对象要求支持的接口数目，不包含隐含的接口。
    pInterfaceId：对象需要支持的接口id的数组。
    pInterfaceRequired：指定每个要求接口的接口是可选或者必须的标志位数组。如果要求的接口没有实现，创建对象会失败并返回错误码SL_RESULT_FEATURE_UNSUPPORTED。

获取管理接口：
    (*engineObject)->GetInterface(engineObject, SL_IID_ENGINE, &engineObject);

 */

/*
Media Object
    Media Object代表着多媒体处理功能的抽象，如呈现和捕获媒体流的对象player、recorder 等等。
可以通过 SLEngineItf 提供的 CreateAudioPlayer 方法来创建一个 player 对象实例，可以通过 SLEngineItf 提供的 CreateAudioRecorder 方法来创建一个 recorder 实例。

Data Source 和 Data Sink
数据源(Data source)是媒体对象的输入参数，指定媒体对象将从何处接收特定类型的数据（例如采样的音频或MIDI数据）。 数据接收器(Data sink)是媒体对象的输入参数，指定媒体对象将发送特定类型数据的位置。
OpenSL ES 里面，这两个结构体均是作为创建 Media Object 对象时的参数而存在的，Data source 代表着输入源的信息，即数据从哪儿来、输入的数据参数是怎样的；而 Data Sink 代表着输出的信息，即数据输出到哪儿、以什么样的参数来输出。

typedef struct SLDataSource_ {
	void *pLocator;
	void *pFormat;
} SLDataSource;


typedef struct SLDataSink_ {
	void *pLocator;
	void *pFormat;
} SLDataSink;

pLocator 主要有如下几种：
    SLDataLocator_Address
    SLDataLocator_BufferQueue
    SLDataLocator_IODevice
    SLDataLocator_MIDIBufferQueue
    SLDataLocator_URI

Media Object 对象的输入源/输出源，既可以是 URL，也可以 Device，或者来自于缓冲区队列等等，
完全是由 Media Object 对象的具体类型和应用场景来配置。数据格式(data format)标识数据流的特征，
包括以下几种类型：
    1、基于MIME类型的格式
    2、PCM格式

 */


/*
Metadata Extractor Object
播放器对象支持读取媒体内容的元数据。但是有时候只是读取元数据而不播放媒体内容是很有用处的。
Metadata Extractor Object可以用于读取元数据而不需要分配用于媒体播放的资源。





*/

extern "C"
JNIEXPORT void JNICALL
Java_com_xxx_media_Media_test5(JNIEnv *env, jclass type, jstring in_path_) {
    const char *in_path = env->GetStringUTFChars(in_path_, 0);

    // 创建
    SLObjectItf engineObject;
    slCreateEngine(&engineObject, 0, nullptr, 0, nullptr, nullptr);
    // 初始化
    (*engineObject)->Realize(engineObject, SL_BOOLEAN_FALSE);
    // 销毁
    (*engineObject)->Destroy(engineObject);
    // 获取管理接口
    (*engineObject)->GetInterface(engineObject, SL_IID_ENGINE, &engineObject);

    SLDataSource slDataSource;

    env->ReleaseStringUTFChars(in_path_, in_path);
}

/**
 * opensl 播放pcm
 *    1.创建EngineObject
 *    2.设置DataSource
 *    3.设置DataSink
 *    4.创建播放器
 *    5.设置缓冲队列和回调函数
 *    6.设置播放状态
 *    7.启动回调函数
 */
// 1 创建接口对象
SLresult createEngine() {
    SLObjectItf engineObject;
    SLresult result = slCreateEngine(&engineObject, 0, nullptr, 0, nullptr, nullptr);
    if (result != SL_RESULT_SUCCESS) {
        LOGI("slCreateEngine 创建引擎失败 %d", result);
        return result;
    }

    result = (*engineObject)->Realize(engineObject, SL_BOOLEAN_FALSE);
    if (result != SL_RESULT_SUCCESS) {
        LOGI("Realize 失败 %d", result);
        return result;
    }

    result = (*engineObject)->GetInterface(engineObject, SL_IID_ENGINE, &engineObject);
    if (result != SL_RESULT_SUCCESS) {
        LOGI("GetInterface 失败 %d", result);
    }

    return result;
}

// 2 设置混音器
void setDataSource() {
    SLDataLocator_AndroidSimpleBufferQueue android_queue = {SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE,
                                                            2};
    SLDataFormat_PCM sLDataFormat_pcm = {
            SL_DATAFORMAT_PCM,
            2,
            SL_SAMPLINGRATE_44_1,
            SL_PCMSAMPLEFORMAT_FIXED_16,
            SL_PCMSAMPLEFORMAT_FIXED_16,
            SL_SPEAKER_FRONT_LEFT | SL_SPEAKER_FRONT_RIGHT,//立体声（前左前右）
            SL_BYTEORDER_LITTLEENDIAN
    };
    SLDataSource slDataSource = {&android_queue, &sLDataFormat_pcm};
}

// 3 设置dataslink
SLresult setDataSink() {
    //set DataSink
    const SLInterfaceID mids[1] = {SL_IID_ENVIRONMENTALREVERB};
    const SLboolean mreq[1] = {SL_BOOLEAN_FALSE};
    SLresult ret = (*engineEngine)->CreateOutputMix(engineEngine, &outputMixObject, 1, mids, mreq);
    if (ret != SL_RESULT_SUCCESS) {
        LOGD("CreateOutputMix failed, ret=%d", ret);
        return ret;
    }
    ret = (*outputMixObject)->Realize(outputMixObject, SL_BOOLEAN_FALSE);
    if (ret != SL_RESULT_SUCCESS) {
        LOGD("Realize failed, result=%d", ret);
        return ret;
    }
    SLDataLocator_OutputMix outputMix = {SL_DATALOCATOR_OUTPUTMIX, outputMixObject};
    SLDataSink audioSnk = {&outputMix, NULL};
}

// 4 创建播放器
void createPlayer() {
    const SLInterfaceID ids[1] = {SL_IID_BUFFERQUEUE};
    const SLboolean req[1] = {SL_BOOLEAN_TRUE};
    ret = (*engineEngine)->CreateAudioPlayer(engineEngine, &playerObject, &slDataSource, &audioSnk,
                                             1, ids, req);
    if (ret != SL_RESULT_SUCCESS) {
        LOGD("CreateAudioPlayer() failed.");
        return ret;
    }
    ret = (*playerObject)->Realize(playerObject, SL_BOOLEAN_FALSE);
    if (ret != SL_RESULT_SUCCESS) {
        LOGD("playerObject Realize() failed.");
        return ret;
    }
    ret = (*playerObject)->GetInterface(playerObject, SL_IID_PLAY, &playerPlay);
    if (ret != SL_RESULT_SUCCESS) {
        LOGD("playerObject GetInterface(SL_IID_PLAY) failed.");
        return ret;
    }
}

// 5 设置缓冲队列和回调函数
void setBuffer() {
    ret = (*playerObject)->GetInterface(playerObject, SL_IID_BUFFERQUEUE, &simpleBufferQueue);
    if (ret != SL_RESULT_SUCCESS) {
        LOGD("playerObject GetInterface(SL_IID_BUFFERQUEUE) failed.");
        return ret;
    }

    ret = (*simpleBufferQueue)->RegisterCallback(simpleBufferQueue, pcmBufferCallBack, this);
    if (ret != SL_RESULT_SUCCESS) {
        LOGD("SLAndroidSimpleBufferQueueItf RegisterCallback() failed.");
        return ret;
    }
    return ret;
}

int64_t getPcmData(void **pcm, FILE *pcmFile, uint8_t *out_buffer) {
    while(!feof(pcmFile)) {
        size_t size = fread(out_buffer, 1, 44100 * 2 * 2, pcmFile);
        *pcm = out_buffer;
        return size;
    }
    return 0;
}

void pcmBufferCallBack(SLAndroidSimpleBufferQueueItf bf, void * context) {
    int32_t size = getPcmData(&buffer, pcmFile, out_buffer);
    LOGD("pcmBufferCallBack, size=%d", size);
    if (NULL != buffer && size > 0) {
        SLresult result = (*simpleBufferQueue)->Enqueue(simpleBufferQueue, buffer, size);
    }
}

// 6 设置播放状态
void OpenGLESPlayer::start() {
    (*playerPlay)->SetPlayState(playerPlay, SL_PLAYSTATE_PLAYING);
//    主动调用回调函数开始工作
    pcmBufferCallBack(simpleBufferQueue, this);
}

void OpenGLESPlayer::stop() {
    LOGD("stop");
    (*playerPlay)->SetPlayState(playerPlay, SL_PLAYSTATE_STOPPED);
}

// 7 启动回调函数
void OpenGLESPlayer::start() {
    (*playerPlay)->SetPlayState(playerPlay, SL_PLAYSTATE_PLAYING);
//    主动调用回调函数开始工作
    pcmBufferCallBack(simpleBufferQueue, this);
}











































