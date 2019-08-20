package com.xxx.media.activity;

import butterknife.ButterKnife;
import butterknife.OnClick;

import android.os.Bundle;

import com.xxx.media.Media;
import com.xxx.media.MusicPlay;
import com.xxx.media.R;
import com.xxx.media.Test;

public class TestAudioPlayerActivity extends BaseActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_test_audio_player);
        ButterKnife.bind(this);
    }

    @OnClick(R.id.btn_playAudio1)
    void btn_playAudio1() {
        new Thread(new Runnable() {
            @Override
            public void run() {
                MusicPlay musicPlay = new MusicPlay();
                Media.test4(Test.VIDEO, musicPlay);
            }
        }).start();
    }

    @OnClick(R.id.btn_playAudio2)
    void btn_playAudio2() {

    }
}
