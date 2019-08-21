#include "FFmpegMusic.h"

AVFormatContext *pFormatCtx;

AVCodecContext *pCodecCtx;

AVCodec *pCodex;

AVPacket *packet;

AVFrame *frame;

SwrContext *swrContext;

uint8_t *out_buffer;

int out_channer_nb;
int audio_stream_idx = -1;

int createFFmpeg(int *rate, int *channel, char *input) {
    av_register_all();
    pFormatCtx = avformat_alloc_context();
    int error;
    char buf[] = "";
    if (error = avformat_open_input(&pFormatCtx, input, NULL, NULL) < 0) {
        av_strerror(error, buf, 1024);
        LOGE("Couldn't open file %s: %d(%s)", input, error, buf);
        LOGE("打开视频失败")
    }

    //3.获取视频信息
    if (avformat_find_stream_info(pFormatCtx, NULL) < 0) {
        LOGE("%s", "获取视频信息失败");
        return -1;
    }

    for (int i = 0; i < pFormatCtx->nb_streams; ++i) {
        if (pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_AUDIO) {
            LOGE("找到音频id %d", pFormatCtx->streams[i]->codec->codec_type);
            audio_stream_idx = i;
            break;
        }
    }

    // mp3的解码器 获取音频编解码器
    pCodecCtx = pFormatCtx->streams[audio_stream_idx]->codec;

    pCodex = avcodec_find_decoder(pCodecCtx->codec_id);

    if (avcodec_open2(pCodecCtx, pCodex, NULL) < 0) {
        LOGE("avcodec_open2 error");
        return -1;
    }
    packet = (AVPacket *) av_malloc(sizeof(AVPacket));
    // av_init_packet(packet);
    // 音频数据

    frame = av_frame_alloc();

    // mp3里面所包含的编码格式转换成pcm SwcContext
    swrContext = swr_alloc();

    // 44100*2
    out_buffer = (uint8_t *) av_malloc(44100 * 2);
    uint64_t out_ch_layout = AV_CH_LAYOUT_STEREO;
    // 输出采样位数  16位
    enum AVSampleFormat out_formart = AV_SAMPLE_FMT_S16;
    // 输出的采样率必须与输入相同
    int out_sample_rate = pCodecCtx->sample_rate;

    swr_alloc_set_opts(
            swrContext, out_ch_layout,
            out_formart, out_sample_rate,
            pCodecCtx->channel_layout, pCodecCtx->sample_fmt,
            pCodecCtx->sample_rate, 0, NULL);

    swr_init(swrContext);
    // 获取通道数  2
    out_channer_nb = av_get_channel_layout_nb_channels(AV_CH_LAYOUT_STEREO);
    *rate = pCodecCtx->sample_rate;
    *channel = pCodecCtx->channels;
    return 0;
}

int getPcm(void **pcm, size_t *pcm_size) {
    int frameCount = 0;
    int got_frame;
    while (av_read_frame(pFormatCtx, packet) >= 0) {
        if (packet->stream_index == audio_stream_idx) {
            // 解码mp3编码格式frame----pcm   frame
            avcodec_decode_audio4(pCodecCtx, frame, &got_frame, packet);
            if (got_frame) {
                /**
                 * int swr_convert(struct SwrContext *s, uint8_t **out, int out_count,
                                const uint8_t **in , int in_count);
                 */
                swr_convert(swrContext, &out_buffer, 44100 * 2, (const uint8_t **) frame->data,
                            frame->nb_samples);
                // 缓冲区的大小
                int size = av_samples_get_buffer_size(NULL, out_channer_nb, frame->nb_samples,
                                                      AV_SAMPLE_FMT_S16, 1);
                *pcm = out_buffer;
                *pcm_size = size;
                break;
            }
        }
    }
    return 0;
}


void realseFFmpeg() {
    av_free_packet(packet);
    av_free(out_buffer);
    av_frame_free(&frame);
    swr_free(&swrContext);
    avcodec_close(pCodecCtx);
    avformat_close_input(&pFormatCtx);
}
