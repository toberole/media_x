package com.xxx.media.activity;

import android.graphics.SurfaceTexture;
import android.hardware.Camera;
import android.media.MediaRecorder;
import android.os.Bundle;
import android.util.Log;
import android.view.TextureView;
import android.widget.Button;


import com.xxx.media.Constant;
import com.xxx.media.R;

import java.io.File;
import java.io.IOException;

import butterknife.BindView;
import butterknife.ButterKnife;
import butterknife.OnClick;

public class MediaRecorderActivity extends BaseActivity implements TextureView.SurfaceTextureListener {
    public static final String TAG = MediaRecorderActivity.class.getSimpleName();

    @BindView(R.id.tv_preView)
    TextureView tv_preView;

    @BindView(R.id.btn_record)
    Button btn_record;

    private SurfaceTexture surface;

    private boolean textureAvailable = false;

    private boolean isStop = true;

    private Camera camera;

    private File file;

    private MediaRecorder mediaRecorder;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_media_recorder);
        ButterKnife.bind(this);
        init();
    }

    private void init() {
        tv_preView.setSurfaceTextureListener(this);
    }

    @Override
    public void onSurfaceTextureAvailable(SurfaceTexture surface, int width, int height) {
        this.surface = surface;
        textureAvailable = true;
        openCamera();
    }

    private void openCamera() {
        camera = Camera.open(Camera.CameraInfo.CAMERA_FACING_BACK);
        Camera.Parameters parameters = camera.getParameters();//设置参数
        try {
            parameters.setFocusMode(Camera.Parameters.FOCUS_MODE_CONTINUOUS_PICTURE);//设置自动对焦
            parameters.setPreviewSize(1920, 1080);//设置预览尺寸，为了全屏展示，我们选择最大尺寸，同时TextureView也应该是match_parent全屏
            camera.setParameters(parameters);//设置相机的参数
            camera.setDisplayOrientation(90);//设置显示翻转，为0则是水平录像，90为竖屏
            camera.setPreviewTexture(surface);//将onSurfaceTextureAvailable监听中的surface传入进来，设置预览的控件
        } catch (Exception t) {
            Log.i(TAG, "onSurfaceTextureAvailable: IO异常");
            return;
        }
        camera.startPreview();
        tv_preView.setAlpha(1.0f);
    }

    @OnClick(R.id.btn_record)
    void btn_record() {
        if (textureAvailable) {
            if (isStop) {
                record();
                isStop = true;
                btn_record.setText("stop");
            } else {
                isStop = true;
                btn_record.setText("start");
                if (mediaRecorder != null) {
                    mediaRecorder.setOnErrorListener(null);
                    mediaRecorder.setPreviewDisplay(null);
                    mediaRecorder.reset();
                    mediaRecorder.release();
                    mediaRecorder = null;
                }
            }
        } else {
            toast("textureAvailable false");
        }
    }

    public void record() {
        camera.unlock();
        mediaRecorder = new MediaRecorder();
        mediaRecorder.setCamera(camera);
        mediaRecorder.setAudioSource(MediaRecorder.AudioSource.MIC);
        mediaRecorder.setVideoSource(MediaRecorder.VideoSource.DEFAULT);
        mediaRecorder.setOutputFormat(MediaRecorder.OutputFormat.MPEG_4);  // 设置视频的输出格式 为MP4

        mediaRecorder.setAudioSamplingRate(16000);
        mediaRecorder.setAudioChannels(1);
        mediaRecorder.setAudioEncoder(MediaRecorder.AudioEncoder.AAC);

        mediaRecorder.setVideoEncoder(MediaRecorder.VideoEncoder.H264); // 设置视频的编码格式
        mediaRecorder.setVideoSize(1920, 1080);  // 设置视频大小
        mediaRecorder.setVideoEncodingBitRate(5 * 1024 * 1024);
        mediaRecorder.setVideoFrameRate(60); // 设置帧率
        mediaRecorder.setOrientationHint(90);
        file = new File(Constant.DIR);
        if (!file.exists()) {
            file.mkdirs();
        }
        mediaRecorder.setOutputFile(file.getPath() + File.separator + System.currentTimeMillis() + ".mp4");
        try {
            mediaRecorder.prepare();
            mediaRecorder.start();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    @Override
    public void onSurfaceTextureSizeChanged(SurfaceTexture surface, int width, int height) {

    }

    @Override
    public boolean onSurfaceTextureDestroyed(SurfaceTexture surface) {
        return false;
    }

    @Override
    public void onSurfaceTextureUpdated(SurfaceTexture surface) {

    }
}
