package com.xxx.media;

import android.app.Application;

import java.io.File;

public class App extends Application {
    @Override
    public void onCreate() {
        super.onCreate();

        File file = new File(Constant.DIR);
        if (!file.exists()) {
            file.mkdirs();
        }
    }
}
