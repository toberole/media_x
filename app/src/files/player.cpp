
#include <libavdevice/avdevice.h>
#include <log.h>

void *begin(void *args) {
    av_register_all();
    avformat_network_init();

    // 封装格式上下文
    AVFormatContext *pFormatCtx = avformat_alloc_context();
    int ret = avformat_open_input(&pFormatCtx, input, NULL, NULL);
    if (ret != 0) {
        LOGI("打开文件失败");
        return NULL;
    }
    // 获取视频信息
    ret = avformat_find_stream_info(pFormatCtx, NULL);
    if (ret < 0) {
        LOGI("获取视频信息失败");
        return NULL;
    }

    for (int i = 0; i < pFormatCtx->nb_streams; ++i) {
        // 获取解码器
        AVCodecContext *avCodecContext = pFormatCtx->streams[i]->codec;
        AVCodec *avCodec = avcodec_find_decoder(avCodecContext->codec_id);


    }

}