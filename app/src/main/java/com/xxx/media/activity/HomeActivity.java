package com.xxx.media.activity;

import butterknife.ButterKnife;
import butterknife.OnClick;

import android.os.Bundle;

import com.xxx.media.R;

public class HomeActivity extends BaseActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_home);
        ButterKnife.bind(this);
    }

    @OnClick(R.id.btn_OpenGLActivity)
    void btn_OpenGLActivity() {
        startPage(OpenGLActivity.class);
    }

    @OnClick(R.id.btn_FilterActivity)
    void btn_FilterActivity() {
        startPage(FilterActivity.class);
    }

    @OnClick(R.id.btn_TestVideoPlayerActivity)
    void btn_TestVideoPlayerActivity() {
        startPage(TestVideoPlayerActivity.class);
    }

    @OnClick(R.id.btn_MediaRecorderActivity)
    void btn_MediaRecorderActivity() {
        startPage(MediaRecorderActivity.class);
    }

    @OnClick(R.id.btn_H264Activity)
    void btn_H264Activity() {
        startPage(H264Activity.class);
    }


}
