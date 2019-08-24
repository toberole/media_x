package com.xxx.media;

public class Media {
    static {
        System.loadLibrary("avcodec-56");
        System.loadLibrary("avdevice-56");
        System.loadLibrary("avfilter-5");
        System.loadLibrary("avformat-56");
        System.loadLibrary("avutil-54");
        System.loadLibrary("postproc-53");
        System.loadLibrary("swresample-1");
        System.loadLibrary("swscale-3");

        System.loadLibrary("native-lib");
    }

    public native static void render(String path, Object surface);

    public native static void playSound(MusicPlay object, String path);

    public native static void test1(String in_path, String out_path);

    public native static void test2(String in_path, String out_path);

    /**
     * ffmpeg 播放视频
     */
    public native static void test3(String in_path, Object surface);

    /**
     * audioTrack 播放音频
     */
    public native static void test4(String in_path, Object obj);

    /**
     * opensl es播放音频
     */
    public native static void test5_opensl_es_start(String in_path);

    public native static void test5_opensl_es_stop();

    /**
     * opensl es录制音频
     */
    public native static void test6_opensl_es_record(String in_path);

    public native static void test6_opensl_es_stoprecord();


    /**
     * AVFilter
     */
    public native static void test7_avfilter_1(String in_path, Object surface);


    /**
     * 音视频合成
     */
    public native static void test8_mux_1(String in_path_video, String in_path_audeo, String out_path);


    public native static void test8_mp42avi(String video_mp4, String video_avi);
}
