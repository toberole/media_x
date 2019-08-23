package com.xxx.media.activity;


import android.app.ActivityManager;
import android.content.Context;
import android.content.pm.ConfigurationInfo;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.util.Log;

import com.xxx.media.R;
import com.xxx.media.opengl.MyRenderer;

import butterknife.BindView;
import butterknife.ButterKnife;

public class OpenGLActivity extends BaseActivity {
    public static final String TAG = OpenGLActivity.class.getSimpleName();

    @BindView(R.id.glSurfaceView)
    GLSurfaceView glSurfaceView;
    private MyRenderer myRenderer;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_open_gl);
        ButterKnife.bind(this);
        init();
    }

    private void init() {
        ActivityManager activityManager = (ActivityManager) getSystemService(Context.ACTIVITY_SERVICE);
        ConfigurationInfo configurationInfo = activityManager.getDeviceConfigurationInfo();
        int reqGlEsVersion = configurationInfo.reqGlEsVersion;

        Log.i(TAG, "reqGlEsVersion: " + reqGlEsVersion);

        glSurfaceView.setEGLContextClientVersion(2);
        myRenderer = new MyRenderer();
        glSurfaceView.setRenderer(myRenderer);

        // 设置渲染模式
        // RENDERMODE_WHEN_DIRTY表示被动渲染，只有在调用requestRender或者onResume等
        // 方法时才会进行渲染。RENDERMODE_CONTINUOUSLY表示持续渲染
        glSurfaceView.setRenderMode(GLSurfaceView.RENDERMODE_CONTINUOUSLY);
    }
}
