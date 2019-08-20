package com.xxx.media;

import android.Manifest;
import android.os.Environment;

public class Constant {
    public static final String DIR = Environment.getExternalStorageDirectory() + "/aaa_media";

    public static final String[] PS = new String[]{
            android.Manifest.permission.INTERNET,
            Manifest.permission.CAMERA,
            Manifest.permission.WRITE_EXTERNAL_STORAGE,
            Manifest.permission.RECORD_AUDIO,
            android.Manifest.permission.READ_EXTERNAL_STORAGE};
}
