#include "log.h"

extern "C" {
#include <libswscale/swscale.h>
#include <libavformat/avformat.h>
}

/**
 * 读取视频的每一帧 去除视频 做转换
 */

extern "C"
JNIEXPORT void JNICALL
Java_com_xxx_media_Media_test1(JNIEnv *env, jclass type, jstring in_path_, jstring out_path_) {
    int res = 0;
    const char *in_path = env->GetStringUTFChars(in_path_, 0);
    const char *out_path = env->GetStringUTFChars(out_path_, 0);

    int video_index = -1;

    // 1 注册所有组件
    av_register_all();

    // 2 打开视频文件
    AVFormatContext *avFormatContext = avformat_alloc_context();
    avformat_open_input(&avFormatContext, in_path, NULL, NULL);

    // 3 获取视频文件信息
    res = avformat_find_stream_info(avFormatContext, NULL);
    if (res < 0) {
        LOGI("打开失败");
        return;
    }

    // 4 找到视频流
    for (int i = 0; i < avFormatContext->nb_streams; ++i) {
        if (avFormatContext->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO) {
            video_index = i;
            break;
        }
    }

    if (video_index == -1) {
        LOGI("没找到视频流");
        return;
    }

    LOGI("视频流 video_index = %d", video_index);

    // 5 获取解码器
    // Codec context associated with this stream. Allocated and freed by libavformat.
    AVCodecContext *codecContext = avFormatContext->streams[video_index]->codec;
    AVCodec *avCodec = avcodec_find_decoder(codecContext->codec_id);
    LOGI("codec_id: %d", codecContext->codec_id);

    // 6 打开解码器
    res = avcodec_open2(codecContext, avCodec, NULL);
    if (res < 0) {
        LOGI("打开解码器失败");
        return;
    }

    // 原始文件中的编码数据
    AVPacket *packet = (AVPacket *) av_malloc(sizeof(AVPacket));
    // 转码前像素数据
    AVFrame *frame = av_frame_alloc();
    // 转码后像素数据
    AVFrame *yuvFrame = av_frame_alloc();

    //只有指定了AVFrame的像素格式、画面大小才能真正分配内存
    //缓冲区分配内存
    uint8_t *out_buffer = (uint8_t *) av_malloc(
            avpicture_get_size(
                    AV_PIX_FMT_YUV420P,
                    codecContext->width,
                    codecContext->height)
    );
    //初始化缓冲区
    avpicture_fill(
            (AVPicture *) yuvFrame,
            out_buffer, AV_PIX_FMT_YUV420P,
            codecContext->width, codecContext->height);

    FILE *fp_yuv = fopen(out_path, "wb");
    /**
     *
     * struct SwsContext *sws_getContext(
     *              int srcW, int srcH, enum AVPixelFormat srcFormat,
                    int dstW, int dstH, enum AVPixelFormat dstFormat,
                    int flags, SwsFilter *srcFilter,
                    SwsFilter *dstFilter, const double *param);
     */
    struct SwsContext *sws_ctx = sws_getContext(codecContext->width, codecContext->height,
                                                codecContext->pix_fmt, codecContext->width,
                                                codecContext->height, AV_PIX_FMT_YUV420P,
                                                SWS_BILINEAR, NULL, NULL, NULL);
    LOGI("width = %d,height = %d", codecContext->width, codecContext->height);

    int len = 0;
    int got_frame = 0;
    int frameCount = 0;
    while (av_read_frame(avFormatContext, packet) >= 0) {
        // 解码AVPacket->AVFrame
        len = avcodec_decode_video2(codecContext, frame, &got_frame, packet);
        if (got_frame) {// 非零
            // frame->yuvFrame (YUV420P)
            // 转为指定的YUV420P像素帧
            sws_scale(sws_ctx, frame->data, frame->linesize, 0,
                      frame->height, yuvFrame->data, yuvFrame->linesize);
            // 向YUV文件保存解码之后的帧数据
            // AVFrame->YUV
            // 一个像素包含一个Y
            int y_size = codecContext->width * codecContext->height;
            fwrite(yuvFrame->data[0], 1, y_size, fp_yuv);
            fwrite(yuvFrame->data[1], 1, y_size / 4, fp_yuv);
            fwrite(yuvFrame->data[2], 1, y_size / 4, fp_yuv);

            LOGI("解码帧数：%d", ++frameCount);
        }

        av_free_packet(packet);
    }

    fclose(fp_yuv);
    av_frame_free(&frame);
    avcodec_close(codecContext);
    avformat_free_context(avFormatContext);

    env->ReleaseStringUTFChars(in_path_, in_path);
    env->ReleaseStringUTFChars(out_path_, out_path);
    LOGI("--------------end--------------");
    return;
}
