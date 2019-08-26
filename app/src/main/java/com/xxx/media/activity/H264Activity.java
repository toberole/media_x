package com.xxx.media.activity;

import androidx.appcompat.app.AppCompatActivity;
import butterknife.ButterKnife;
import butterknife.OnClick;

import android.os.Bundle;

import com.xxx.media.Media;
import com.xxx.media.R;
import com.xxx.media.Test;

public class H264Activity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_h264);
        ButterKnife.bind(this);
    }

    @OnClick(R.id.extract_h264)
    void extract_h264() {
        Media.test9_extract_h264(Test.VIDEO, "/mnt/sdcard/aaa_media/temp.h264");
    }
}
