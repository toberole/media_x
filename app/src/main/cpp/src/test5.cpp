#include "log.h"

extern "C"
JNIEXPORT void JNICALL
Java_com_xxx_media_Media_test5(JNIEnv *env, jclass type, jstring in_path_) {
    const char *in_path = env->GetStringUTFChars(in_path_, 0);


    env->ReleaseStringUTFChars(in_path_, in_path);
}
