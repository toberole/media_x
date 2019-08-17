package com.xxx.media;

import android.content.Context;
import android.graphics.PixelFormat;
import android.util.AttributeSet;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

public class FFmpegVideoView extends SurfaceView {

    private SurfaceHolder holder;

    public FFmpegVideoView(Context context) {
        this(context, null);
    }

    public FFmpegVideoView(Context context, AttributeSet attrs) {
        super(context, attrs, 0);
        init();
    }

    private void init() {
        holder = getHolder();
        holder.setFormat(PixelFormat.RGBA_8888);
    }

    public void play(final String input) {
        new Thread(new Runnable() {
            @Override
            public void run() {
                Media.render(input, holder.getSurface());
            }
        }).start();
    }
}
