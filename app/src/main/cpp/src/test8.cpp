extern "C" {
#include <libavformat/avformat.h>
}


#include "log.h"
// 可以参看 ffmepg源码demo doc/examples muxing.c


extern "C"
JNIEXPORT void JNICALL
Java_com_xxx_media_Media_test8_1mux_11(JNIEnv *env, jclass type, jstring in_path_video_,
                                       jstring in_path_audeo_, jstring out_path_) {
    LOGI("音视频合成");
    int ret = -1;

    const char *in_path_video = env->GetStringUTFChars(in_path_video_, 0);
    const char *in_path_audeo = env->GetStringUTFChars(in_path_audeo_, 0);
    const char *out_path = env->GetStringUTFChars(out_path_, 0);
/*
    // 1
    av_register_all();

    // 2 申请AVFormatContext
    AVFormatContext *oc;
    AVOutputFormat *fmt;
    const char *filename = out_path;
    avformat_alloc_output_context2(&oc, NULL, "flv", filename);
    fmt = oc->oformat;

    // 3 申请一个AVStream AVStream流主要是作为存放音频、视频、字幕流数据
    AVStream *st;
    AVCodecContext *c;
    st = avformat_new_stream(oc, NULL);
    if (!st) {
        LOGI("avformat_new_stream error");
        return;
    }

    st->id = oc->nb_streams;
    c->codec_id = AV_CODEC_ID_FLV1;
    c->bit_rate = 400000;
    c->width = 352;
    c->height = 288;
    st->time_base = (AVRational) {1, 25};
    c->time_base = st->time_base;
    c->gop_size = 12;
    c->pix_fmt = AV_PIX_FMT_YUV420P;

    // 4 增加目标容器头信息
    AVDictionary *opt;
    avformat_write_header(oc, &opt);

    // 5 写入帧数据
    // 在FFmpeg操作数据时，均采用写帧操作进行音视频数据包的写入
    // 而每一帧在常规情况下均使用AVpacket结构进行音视频数据的存储
    // AVPacket结构中包含了PTS、DTS、Data等信息，数据在写入封装时 会根据封装的特性写入对应的信息
    AVFormatContext *ifmt_ctx = NULL;
    AVIOContext *read_in = avio_alloc_context(inbuffer, 32 * 1024, 0, NULL, get_input_buffer, NULL,
                                              NULL);
    ifmt_ctx->pb = read_in;
    ifmt_ctx->flags = AVFMT_FLAG_CUSTOM_IO;
    ret = avformat_open_input(&ifmt_ctx, "h264", NULL, NULL);
    if (ret < 0) {
        LOGI("get h264 data error");
        return;
    }


    //从内存中读取数据 需要通过avio_alloc_context接口中获得的buffer与AVFormatContext
    //建立联系 然后像操作文件一样进行操作即可，接下来就可以从AVFormatContext中获得packet
    //然后将packet通过av_interleaved_write_frame写入到输出的封装格式中。
    while (1) {
        AVPacket pkt = {0};
        av_init_packet(&pkt);
        ret = av_read_frame(ifmt_ctx, &pkt);
        if (ret < 0)break;
        // rescale output packet timestap values from codec to stream timebase
        av_packet_rescale_ts(pkt, *time_base, st->time_base);
        pkt.stream_index = st->index;
        // write the compressed frame to the media file
        av_interleaved_write_frame(ifmt_ctx, pkt);
    }

    // 6 写入容器尾信息
    // 在写入数据即将结束时 需要进行收尾工作 比如写入封装格式的结束标记等信息
    av_write_trailer(oc);

    */
    env->ReleaseStringUTFChars(in_path_video_, in_path_video);
    env->ReleaseStringUTFChars(in_path_audeo_, in_path_audeo);
    env->ReleaseStringUTFChars(out_path_, out_path);
}



extern "C"
JNIEXPORT void JNICALL
Java_com_xxx_media_Media_test8_1mp42avi(JNIEnv *env, jclass type, jstring video_mp4_,
                                        jstring video_avi_) {
    const char *video_mp4 = env->GetStringUTFChars(video_mp4_, 0);
    const char *video_avi = env->GetStringUTFChars(video_avi_, 0);

    LOGI("test8_1mp42avi");

    av_register_all();

    AVFormatContext *ifmt_ctx = avformat_alloc_context();

    int ret;
    char errbuf[1024] = {0};

    ret = avformat_open_input(&ifmt_ctx, video_mp4, NULL, NULL);

    if (ret < 0) {
        av_strerror(ret, errbuf, 1024);
        LOGI("avformat_open_input error info: %s", errbuf);
        return;
    }

    ret = avformat_find_stream_info(ifmt_ctx, NULL);
    if (ret < 0) {
        av_strerror(ret, errbuf, 1024);
        LOGI("avformat_find_stream_info error info: %s", errbuf);
        return;
    }

    LOGI("-----------");

    /**
     * MP4中使用的是H264编码
     * H264编码有两种编码方式
     * 1、annexb模式,它是传统模式，有startcode、sps、pps在Element Stream中
     * 2、mp4模式，一般在mp4、mkv、avi都没有startcode、sps和pps以及其他信息被封装在容器中
     *
     * 每一帧前面是这一帧的长度值，很多解码器只支持annexb模式
     * 因此需要对MP4模式做转换，在FFmpeg中使用h264_mp4toannexb_filter 进行模式转换
     */

    AVOutputFormat *ofmt;
    AVBitStreamFilterContext *vbsf;

    vbsf = av_bitstream_filter_init("h264_mp4toannexb");

    av_dump_format(ifmt_ctx, 0, video_mp4, 0);


    AVFormatContext *ofmt_ctx;

    env->ReleaseStringUTFChars(video_mp4_, video_mp4);
    env->ReleaseStringUTFChars(video_avi_, video_avi);
}