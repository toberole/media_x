#include <jni.h>
#include <string>
#include "log.h"

extern "C"{
//编码
#include "libavcodec/avcodec.h"
//封装格式处理
#include "libavformat/avformat.h"
//像素处理
#include "libswscale/swscale.h"
#include <android/native_window_jni.h>
#include <unistd.h>
}

/*
 * Class:     com_xxx_media_Media
 * Method:    start
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_com_xxx_media_Media_start
        (JNIEnv *env, jclass jclazz, jstring jstr) {
    const char *path = env->GetStringUTFChars(jstr, NULL);
    // 注册各大组件
    avcodec_register_all();

    // 获取上下文
    AVFormatContext *avFormatContext = avformat_alloc_context();

    // 解封装
    avformat_open_input(&avFormatContext, path, NULL, NULL);

    // 打开地址并且获取里面的内容
    avformat_find_stream_info(avFormatContext, NULL);

    // 在一个音视频中包括了音频流，视频流和字幕流
    // 所以在所有的内容当中，应当找出相对应的视频流。
    int video_index = -1;
    for (int i = 0; i < avFormatContext->nb_streams; ++i) {
        avFormatContext->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO){
            video_index = i;
            break;
        }
    }

    LOGI("video index: %d", video_index);

    // 对视频流进行解码、转换和绘制
    // a.要进行解码，得有解码器并打开解码器。
    // 获取解码器上下文
    AVCodecContext *avCodecContext = avFormatContext->streams[video_index]->codec;
    // 获取解码器
    AVCodec *avCodec = avcodec_find_decoder(avCodecContext->codec_id);
    // 打开解码器
    if (avcodec_open2(avCodecContext, avCodec, NULL) < 0) {
        LOGI("打开失败");
        return;
    }

    // b.申请AVPacket和AVFrame
    // 其中AVPacket的作用是：保存解码之前的数据和一些附加信息，如显示时间戳（pts）、解码时间戳（dts）、数据时长，所在媒体流的索引等；
    // AVFrame的作用是：存放解码过后的数据。
    // 申请AVPacket
    AVPacket *packet = (AVPacket *) av_malloc(sizeof(AVPacket));
    av_init_packet(packet);
    // 申请AVFrame
    // 分配一个AVFrame结构体,AVFrame结构体一般用于存储原始数据，指向解码后的原始帧
    AVFrame *frame = av_frame_alloc();
    // 分配一个AVFrame结构体，指向存放转换成rgb后的帧
    AVFrame *rgb_frame = av_frame_alloc();
    // rgb_frame是一个缓存区域 需要设置
    uint8_t *out_buffer = (uint8_t *) av_malloc(
            avpicture_get_size(AV_PIX_FMT_RGBA, avCodecContext->width, avCodecContext->height));
    // 关联缓冲区
    avpicture_fill((AVPicture *) rgb_frame, out_buffer, AV_PIX_FMT_RGBA, avCodecContext->width,
                   avCodecContext->height);

    // 原生绘制 需要ANativeWindow。
    //取到nativewindow
    ANativeWindow *nativeWindow = ANativeWindow_fromSurface(env, surface);
    if (nativeWindow == 0) {
        LOGI("nativewindow取到失败");
        return;
    }
    //视频缓冲区
    ANativeWindow_Buffer native_outBuffer;

    int frameCount;
    while (av_read_frame(avFormatContext, packet) >= 0) {
        LOGI("解码 %d", packet->stream_index);
        LOGI("VINDEX %d", video_index);
        if (packet->stream_index == video_index) {
            LOGI("解码 ......");
            //视频流 解码
            avcodec_decode_video2(avCodecContext, frame, &frameCount, packet);
        }
        av_free_packet(packet);

        if (frameCount) {
            LOGE("转换并绘制");
            //说明有内容 绘制之前配置nativewindow
            ANativeWindow_setBuffersGeometry(nativeWindow, avCodecContext->width,
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
            //像素数据的首地址
            uint8_t *src = rgb_frame->data[0];
            // 实际内存一行数量
            int srcStride = rgb_frame->linesize[0];
            //int i=0;
            for (int i = 0; i < avCodecContext->height; ++i) {
                // memcpy(void *dest, const void *src, size_t n)
                // 将rgb_frame中每一行的数据复制给nativewindow
                memcpy(dst + i * destStride, src + i * srcStride, srcStride);
            }
            //解锁
            ANativeWindow_unlockAndPost(nativeWindow);
            usleep(1000 * 16);

        }
    }
}
