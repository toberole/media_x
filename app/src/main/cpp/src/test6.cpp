#include <SLES/OpenSLES.h>
#include "log.h"


extern "C"
JNIEXPORT void JNICALL
Java_com_xxx_media_Media_test6_1opensl_1es_1record(JNIEnv *env, jclass type, jstring in_path_) {
    const char *in_path = env->GetStringUTFChars(in_path_, 0);
    SLresult result;
    SLObjectItf objectEngine;
    // 创建引擎对象
    result = slCreateEngine(&objectEngine, 0, nullptr, 0, nullptr, nullptr);

    if (result != SL_RESULT_SUCCESS) {
        LOGI("slCreateEngine error");
        return;
    }

    // 实现
    (*objectEngine)->Realize(objectEngine, SL_BOOLEAN_FALSE);

    // 需要使用什么功能 就需要获取对应的接口
    // 获取引擎接口
    (*objectEngine)->GetInterface(objectEngine, SL_IID_ENGINE, &objectEngine);


    env->ReleaseStringUTFChars(in_path_, in_path);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_xxx_media_Media_test6_1opensl_1es_1stoprecord(JNIEnv *env, jclass type) {


}