package com.xxx.media.opengl;

import android.opengl.GLSurfaceView;
import android.util.Log;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public class MyRenderer implements GLSurfaceView.Renderer {
    public static final String TAG = MyRenderer.class.getSimpleName();

    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
        Log.i(TAG, "MyRenderer#onSurfaceCreated");
    }

    @Override
    public void onSurfaceChanged(GL10 gl, int width, int height) {
        Log.i(TAG, "MyRenderer#onSurfaceChanged");
    }

    @Override
    public void onDrawFrame(GL10 gl) {
        Log.i(TAG, "MyRenderer#onDrawFrame");
    }
}
