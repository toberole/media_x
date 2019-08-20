#include "log.h"

extern "C" {
#include <libavformat/avformat.h>
#include <libswresample/swresample.h>
}

extern "C"
JNIEXPORT void JNICALL
Java_com_xxx_media_Media_test4(JNIEnv *env, jclass type, jstring in_path_, jobject obj) {
    const char *in_path = env->GetStringUTFChars(in_path_, 0);
    int res = 0;
    av_register_all();
    AVFormatContext *pFormatContext = avformat_alloc_context();
    avformat_open_input(&pFormatContext, in_path, NULL, NULL);
    avformat_find_stream_info(pFormatContext, NULL);

    int audio_index = -1;
    for (int i = 0; i < pFormatContext->nb_streams; ++i) {
        if (pFormatContext->streams[i]->codec->codec_type == AVMEDIA_TYPE_AUDIO) {
            audio_index = i;
            break;
        }
    }

    if (audio_index < 0) {
        LOGI("查找音频流失败");
        return;
    }

    AVCodecContext *pCodecContext = pFormatContext->streams[audio_index]->codec;
    AVCodec *pCodec = avcodec_find_decoder(pCodecContext->codec_id);
    res = avcodec_open2(pCodecContext, pCodec, NULL);

    if (res < 0) {
        LOGI("打开音频解码器失败");
        return;
    }

    LOGI("音频channel_layouts: %lld", pCodec->channel_layouts[0]);

    // 设置转码后的输出参数
    // 转后的通道数 单通道
    uint64_t out_channel_layout = AV_CH_LAYOUT_MONO;
    // 采样格式
    enum AVSampleFormat sample_fmt = AV_SAMPLE_FMT_S16;
    //采样率
    int out_sample_rate = pCodecContext->sample_rate;
    //通道数
    int out_channels = av_get_channel_layout_nb_channels(out_channel_layout);
    LOGI("out_channels = %d\n", out_channels);

    // 缓冲区 创建buffer
    // pCodecContext->frame_size
    // Number of samples per channel in an audio frame.
    int out_nb_samples = pCodecContext->frame_size;
    LOGI("frame_size = %d", out_nb_samples);
    int linesize = -1;
    int buffer_size = av_samples_get_buffer_size(&linesize, out_channels,
                                                 out_nb_samples, sample_fmt, 0);
    uint8_t *out_buffer = static_cast<uint8_t *>(av_malloc(buffer_size));
    LOGI("buffer_size = %d,linesize = %d", buffer_size, linesize);

    int64_t in_channel_layout = av_get_default_channel_layout(pCodecContext->channels);

    // 音频格式转换
    struct SwrContext *swrContext = swr_alloc();

    //swr_alloc_set_opts将源文件的采样格式转换为自己希望的采样格式
    swr_alloc_set_opts(swrContext, out_channel_layout, sample_fmt, out_sample_rate,
                       in_channel_layout, pCodecContext->sample_fmt, pCodecContext->sample_rate, 0,
                       NULL);
    swr_init(swrContext);

    // 获取通道数
    int out_channer_nb = av_get_channel_layout_nb_channels(AV_CH_LAYOUT_MONO);
    LOGI("out_channer_nb = %d\n", out_channer_nb);

    // 反射得到Class类型
    jclass david_player = env->GetObjectClass(obj);
    // 反射得到createAudio方法
    jmethodID createAudio = env->GetMethodID(david_player, "createTrack", "(II)V");
    // 反射调用createAudio
    env->CallVoidMethod(obj, createAudio, 16000, out_channer_nb);
    jmethodID audio_write = env->GetMethodID(david_player, "playTrack", "([BI)V");

    AVPacket *packet = static_cast<AVPacket *>(av_malloc(sizeof(AVPacket)));
    av_init_packet(packet);
    AVFrame *frame = av_frame_alloc();

    int got_frame;
    while (av_read_frame(pFormatContext, packet) >= 0) {
        if (packet->stream_index == audio_index) {
            avcodec_decode_audio4(pCodecContext, frame, &got_frame, packet);
            if (got_frame) {
                swr_convert(swrContext, &out_buffer, buffer_size,
                            (const uint8_t **) (frame->data), frame->nb_samples);

                // LOGI("frame.nb_samples = %d", frame->nb_samples);

                jbyteArray audio_sample_array = env->NewByteArray(buffer_size);
                env->SetByteArrayRegion(audio_sample_array, 0, buffer_size,
                                        (const jbyte *) out_buffer);
                env->CallVoidMethod(obj, audio_write, audio_sample_array, buffer_size);
                env->DeleteLocalRef(audio_sample_array);
            }
        }
    }
    av_frame_free(&frame);
    swr_free(&swrContext);
    avcodec_close(pCodecContext);
    avformat_close_input(&pFormatContext);
    env->ReleaseStringUTFChars(in_path_, in_path);
}

