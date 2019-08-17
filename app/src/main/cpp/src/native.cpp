#include <jni.h>
#include <string>
#include <libavcodec/avcodec.h>

extern "C" JNIEXPORT jstring JNICALL
Java_com_xxx_media_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}
