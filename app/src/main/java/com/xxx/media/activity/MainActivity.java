package com.xxx.media.activity;

import android.os.Bundle;
import android.view.View;
import android.widget.Button;

import com.xxx.media.view.FFmpegVideoView;
import com.xxx.media.Media;
import com.xxx.media.MusicPlay;
import com.xxx.media.R;


public class MainActivity extends BaseActivity implements View.OnClickListener {
    public static final String TAG = MainActivity.class.getSimpleName();

    private FFmpegVideoView videoView;
    private String path = "/mnt/sdcard/media_x/2019_08_16_20_41_01.mp4";
    private Button play_video;
    private Button play_audio;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        play_video = findViewById(R.id.play_video);
        videoView = findViewById(R.id.videoView);
        play_audio = findViewById(R.id.play_audio);
        play_video.setOnClickListener(this);
        play_audio.setOnClickListener(this);
    }

    @Override
    public void onClick(View v) {
        switch (v.getId()) {
            case R.id.play_audio:
                MusicPlay musicPlay = new MusicPlay();
                Media.playSound(musicPlay, path);
                break;
            case R.id.play_video:
                videoView.play(path);
                break;
            default:
                break;
        }
    }
}
