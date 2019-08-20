package com.xxx.media.activity;

import butterknife.ButterKnife;
import butterknife.OnClick;

import android.os.Bundle;

import com.xxx.media.Media;
import com.xxx.media.R;
import com.xxx.media.Test;

public class TestActivity1 extends BaseActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_test1);

        ButterKnife.bind(this);
    }

    @OnClick(R.id.btn_test1)
    void btn_test1() {
        new Thread(new Runnable() {
            @Override
            public void run() {
                Media.test1(Test.VIDEO, "/mnt/sdcard/aaa_media/xxx.yuv");
            }
        }).start();

    }
}
