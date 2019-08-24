
#include <zconf.h>
#include "log.h"

extern "C" {
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <android/native_window.h>
#include <android/native_window_jni.h>
}
/**
 * 播放视频
 */

extern "C"
JNIEXPORT void JNICALL
Java_com_xxx_media_Media_test3(JNIEnv *env, jclass type, jstring in_path_, jobject surface) {
    const char *in_path = env->GetStringUTFChars(in_path_, 0);

    int res = 0;

    av_register_all();

    // int avformat_open_input(AVFormatContext **ps, const char *url, AVInputFormat *fmt, AVDictionary **options);
    AVFormatContext *pFormatContext = avformat_alloc_context();
    res = avformat_open_input(&pFormatContext, in_path, NULL, NULL);
    if (res < 0) {
        LOGI("打开视频失败");
        return;
    }

    // 打印参数
    // av_dump_format(pFormatContext, 0, in_path, 0);

    // 获取流信息
    res = avformat_find_stream_info(pFormatContext, NULL);
    if (res < 0) {
        LOGI("获取流信息失败");
        return;
    }

    int video_index = -1;
    for (int i = 0; i < pFormatContext->nb_streams; ++i) {
        if (pFormatContext->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO) {
            video_index = i;
            break;
        }
    }

    if (video_index < 0) {
        LOGI("没有找到视频流");
        return;
    }

    pFormatContext->streams[video_index]->time_base;
    LOGI("pFormatContext->duration = %lld", pFormatContext->duration);

    // 获取解码器
    AVCodecContext *pCodecContext = pFormatContext->streams[video_index]->codec;
    AVCodec *pCodec = avcodec_find_decoder(pCodecContext->codec_id);
    res = avcodec_open2(pCodecContext, pCodec, NULL);
    if (res < 0) {
        LOGI("打开解码器失败");
        return;
    }

    LOGI("pCodecContext->pix_fmt = %d", pCodecContext->pix_fmt);
    LOGI("pCodecContext->pix_fmt = %d", pCodecContext->pix_fmt == AV_PIX_FMT_YUVJ420P);


    AVPacket *pkt = (AVPacket *) av_malloc(sizeof(AVPacket));
    av_init_packet(pkt);

    // 原始数据
    AVFrame *frame = av_frame_alloc();
    // 转换后的数据
    AVFrame *rgb_frame = av_frame_alloc();

    // 分配缓冲区
    int buffer_size = avpicture_get_size(AV_PIX_FMT_RGBA, pCodecContext->width,
                                         pCodecContext->height);
    uint8_t *out_buffer = static_cast<uint8_t *>(av_malloc(buffer_size * sizeof(uint8_t)));
    // 前面的av_frame_alloc函数，只是为这个AVFrame结构体分配了内存，而该结构体的成员变量指针指向的内存还没分配。
    // 这里把av_malloc得到的内存和AVFrame关联起来。其还会设置AVFrame的其他成员
    // 关联缓冲区
    avpicture_fill(reinterpret_cast<AVPicture *>(rgb_frame), out_buffer, AV_PIX_FMT_RGBA,
                   pCodecContext->width,
                   pCodecContext->height);

    // 格式转换
    SwsContext *swsContext = sws_getContext(pCodecContext->width,
                                            pCodecContext->height,
                                            pCodecContext->pix_fmt,
                                            pCodecContext->width,
                                            pCodecContext->height,
                                            AV_PIX_FMT_RGBA,
                                            SWS_BICUBIC, NULL, NULL, NULL);

    // 关联java层的surface
    ANativeWindow *nativeWindow = ANativeWindow_fromSurface(env, surface);
    if (NULL == nativeWindow) {
        LOGI("关联到java surface失败");
        return;
    }

    // 视频缓冲区
    ANativeWindow_Buffer nativeWindow_buffer;
    int frameCount = 0;

    // 配置nativeWindow_buffer
    ANativeWindow_setBuffersGeometry(nativeWindow, pCodecContext->width,
                                     pCodecContext->height, WINDOW_FORMAT_RGBA_8888);
    int n = 0;
    while (av_read_frame(pFormatContext, pkt) >= 0) {
        if (pkt->stream_index == video_index) {
            res = avcodec_decode_video2(pCodecContext, frame, &frameCount, pkt);
            if (res > 0) {
//                for (int i = 0; i < 1; ++i) {
//                    LOGI("xxxxx = %d: ", frame->data[0]);
//                }
                // av_rescale_q(pkt->pts, pCodec, pCodecContext->time_base)
                double pts = av_frame_get_best_effort_timestamp(frame);
                LOGI("pts = %lld,key_frame = %d,pict_type = %d,n = %d",
                     pts,
                     frame->key_frame,
                     frame->pict_type,
                     ++n);
                // lock
                ANativeWindow_lock(nativeWindow, &nativeWindow_buffer, NULL);

                // 格式转换
                sws_scale(swsContext, frame->data, frame->linesize, 0, frame->height,
                          rgb_frame->data, rgb_frame->linesize);

                uint8_t *dst = static_cast<uint8_t *>(nativeWindow_buffer.bits);

                // 一行的字节数
                int destStride = nativeWindow_buffer.stride * 4;
                // 像素数组的地址
                uint8_t *src = rgb_frame->data[0];
                // 实际内存一行数量
                int srcStride = rgb_frame->linesize[0];

                // 向 nativeWindow_buffer拷贝数据
                // 一行一行的拷贝
                for (int i = 0; i < pCodecContext->height; ++i) {
                    memcpy(dst + i * destStride, src + i * srcStride, srcStride);
                }

                ANativeWindow_unlockAndPost(nativeWindow);

                usleep(1000 * 16);
            }
        }

        av_free_packet(pkt);
    }

    ANativeWindow_release(nativeWindow);
    av_frame_free(&frame);
    av_frame_free(&rgb_frame);
    avcodec_close(pCodecContext);
    avformat_free_context(pFormatContext);
    env->ReleaseStringUTFChars(in_path_, in_path);
}
