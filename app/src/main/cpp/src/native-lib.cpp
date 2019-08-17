#include <jni.h>
#include <string>
#include "com_xxx_media_Media.h"
#include "log.h"

extern "C" {
//编码
#include "libavcodec/avcodec.h"
//封装格式处理
#include "libavformat/avformat.h"
//像素处理
#include "libswscale/swscale.h"

#include <android/native_window_jni.h>
#include <unistd.h>
}


extern "C"
JNIEXPORT void JNICALL
Java_com_xxx_media_Media_render(JNIEnv *env, jclass jclazz, jstring inputStr_, jobject surface) {

    const char *inputPath = env->GetStringUTFChars(inputStr_, NULL);
    LOGE("inputPath: %s", inputPath);

    av_register_all();

    // avformat_network_init();

    //获取上下文
    AVFormatContext *avFormatContext = avformat_alloc_context();

    char buf[] = "";
    //打开视频地址并获取里面的内容(解封装)
    int error = avformat_open_input(&avFormatContext, inputPath, NULL, NULL);
    if (error < 0) {
        av_strerror(error, buf, 1024);
        LOGE("Couldn't open file %s: %d(%s)", inputPath, error, buf);
        return;
    }
    if (avformat_find_stream_info(avFormatContext, NULL) < 0) {
        LOGE("获取内容失败");
        return;
    }

    //获取到整个内容过后找到里面的视频流
    int video_index = -1;
    for (int i = 0; i < avFormatContext->nb_streams; ++i) {
        if (avFormatContext->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO) {
            video_index = i;
        }
    }
    LOGE("成功找到视频流 video_index = %d", video_index);

    //对视频流进行解码
    //获取解码器上下文
    AVCodecContext *avCodecContext = avFormatContext->streams[video_index]->codec;
    //获取解码器
    AVCodec *avCodec = avcodec_find_decoder(avCodecContext->codec_id);
    //打开解码器
    if (avcodec_open2(avCodecContext, avCodec, NULL) < 0) {
        LOGE("打开失败");
        return;
    }

    //申请AVPacket
    AVPacket *packet = (AVPacket *) av_malloc(sizeof(AVPacket));
    av_init_packet(packet);
    //申请AVFrame
    //分配一个AVFrame结构体,AVFrame结构体一般用于存储原始数据，指向解码后的原始帧
    AVFrame *frame = av_frame_alloc();
    //分配一个AVFrame结构体，指向存放转换成rgb后的帧
    AVFrame *rgb_frame = av_frame_alloc();
    //输出文件
    //FILE *fp = fopen(outputPath,"wb");

    //缓存区
    uint8_t *out_buffer = (uint8_t *) av_malloc(
            avpicture_get_size(AV_PIX_FMT_RGBA,
                               avCodecContext->width,
                               avCodecContext->height));
    //与缓存区相关联，设置rgb_frame缓存区
    avpicture_fill((AVPicture *) rgb_frame, out_buffer,
                   AV_PIX_FMT_RGBA, avCodecContext->width,
                   avCodecContext->height);


    SwsContext *swsContext = sws_getContext(
            avCodecContext->width,
            avCodecContext->height,
            avCodecContext->pix_fmt,
            avCodecContext->width,
            avCodecContext->height,
            AV_PIX_FMT_RGBA,
            SWS_BICUBIC, NULL, NULL, NULL);

    //取到nativewindow
    ANativeWindow *nativeWindow = ANativeWindow_fromSurface(env, surface);
    if (nativeWindow == 0) {
        LOGE("nativewindow取到失败");
        return;
    }
    //视频缓冲区
    ANativeWindow_Buffer native_outBuffer;

    int frameCount;
    while (av_read_frame(avFormatContext, packet) >= 0) {
        LOGE("解码 stream_index = %d", packet->stream_index);

        if (packet->stream_index == video_index) {
            //视频流 解码
            avcodec_decode_video2(avCodecContext, frame, &frameCount, packet);
            LOGE("解码中....  %d", frameCount);
            if (frameCount) {
                //有内容 绘制之前配置nativewindow
                ANativeWindow_setBuffersGeometry(
                        nativeWindow, avCodecContext->width,
                        avCodecContext->height, WINDOW_FORMAT_RGBA_8888);
                //上锁
                ANativeWindow_lock(nativeWindow, &native_outBuffer, NULL);
                //转换为rgb格式
                sws_scale(swsContext, (const uint8_t *const *) frame->data, frame->linesize, 0,
                          frame->height, rgb_frame->data,
                          rgb_frame->linesize);
                // rgb_frame是有画面数据
                uint8_t *dst = (uint8_t *) native_outBuffer.bits;
                // 拿到一行有多少个字节 RGBA
                int destStride = native_outBuffer.stride * 4;
                // 像素数据的首地址
                uint8_t *src = rgb_frame->data[0];
                // 实际内存一行数量
                int srcStride = rgb_frame->linesize[0];
                //int i=0;
                for (int i = 0; i < avCodecContext->height; ++i) {
                    // memcpy(void *dest, const void *src, size_t n)
                    //将rgb_frame中每一行的数据复制给nativewindow
                    memcpy(dst + i * destStride, src + i * srcStride, srcStride);
                }

                //解锁
                ANativeWindow_unlockAndPost(nativeWindow);
                usleep(1000 * 16);
            }
        }
        av_free_packet(packet);
    }

    //释放
    ANativeWindow_release(nativeWindow);
    av_frame_free(&frame);
    av_frame_free(&rgb_frame);
    avcodec_close(avCodecContext);
    avformat_free_context(avFormatContext);
    env->ReleaseStringUTFChars(inputStr_, inputPath);
}