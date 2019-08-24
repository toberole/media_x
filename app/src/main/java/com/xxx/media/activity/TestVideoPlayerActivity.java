package com.xxx.media.activity;

import butterknife.BindView;
import butterknife.ButterKnife;
import butterknife.OnClick;

import android.graphics.PixelFormat;
import android.os.Bundle;
import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

import com.xxx.media.Media;
import com.xxx.media.R;
import com.xxx.media.Test;

public class TestVideoPlayerActivity extends BaseActivity {
    @BindView(R.id.surfaceView)
    SurfaceView surfaceView;
    private Surface surface;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_test_video_player);
        ButterKnife.bind(this);
        SurfaceHolder surfaceHolder = surfaceView.getHolder();
        surfaceHolder.setFormat(PixelFormat.RGBA_8888);
        surface = surfaceHolder.getSurface();
    }

    @OnClick(R.id.btn_playVideo)
    void btn_playVideo() {
        new Thread(new Runnable() {
            @Override
            public void run() {
                Media.test3(Test.VIDEO, surface);
            }
        }).start();
    }

    @OnClick(R.id.btn_mux1)
    void btn_mux1() {
        new Thread(new Runnable() {
            @Override
            public void run() {
                Media.test8_mux_1(Test.VIDEO, "", "");
            }
        }).start();
    }
}
