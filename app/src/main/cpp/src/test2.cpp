#include "log.h"

/**
 * 解析视频中的音频
 */

extern "C"
JNIEXPORT void JNICALL
Java_com_xxx_media_Media_test2(JNIEnv *env, jclass type, jstring in_path_, jstring out_path_) {
    const char *in_path = env->GetStringUTFChars(in_path_, 0);
    const char *out_path = env->GetStringUTFChars(out_path_, 0);





    env->ReleaseStringUTFChars(in_path_, in_path);
    env->ReleaseStringUTFChars(out_path_, out_path);
}

