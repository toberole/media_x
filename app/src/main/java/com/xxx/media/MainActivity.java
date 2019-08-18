package com.xxx.media;

import android.os.Bundle;
import android.support.v4.app.ActivityCompat;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.widget.Button;


public class MainActivity extends AppCompatActivity implements View.OnClickListener {
    public static final String TAG = MainActivity.class.getSimpleName();

    private FFmpegVideoView videoView;
    private String path = "/mnt/sdcard/media_x/2019_08_16_20_41_01.mp4";
    private Button play_video;
    private Button play_audio;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        ActivityCompat.requestPermissions(this, new String[]{
                android.Manifest.permission.INTERNET,
                android.Manifest.permission.READ_EXTERNAL_STORAGE
        }, 100);

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
