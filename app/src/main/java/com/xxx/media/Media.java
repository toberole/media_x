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

}
