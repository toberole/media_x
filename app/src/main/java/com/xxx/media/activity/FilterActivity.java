package com.xxx.media.activity;

import androidx.appcompat.app.AppCompatActivity;
import butterknife.BindView;
import butterknife.ButterKnife;

import android.os.Bundle;
import android.view.Surface;
import android.view.SurfaceView;

import com.xxx.media.Media;
import com.xxx.media.R;
import com.xxx.media.Test;

// 利用AVfilter实现视频添加水印，滤镜等特效功能
public class FilterActivity extends AppCompatActivity {
    @BindView(R.id.surfaceView)
    SurfaceView surfaceView;
    private Surface surface;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_filter);
        ButterKnife.bind(this);

        surface = surfaceView.getHolder().getSurface();
        Media.test7_avfilter_1(Test.VIDEO, surface);
    }
}
