package com.xxx.media.activity;

import android.os.Bundle;
import android.widget.Toast;

import com.xxx.media.Constant;

import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.ActivityCompat;

public class BaseActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        ActivityCompat.requestPermissions(BaseActivity.this, Constant.PS, 1000);
    }

    public void toast(String msg) {
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                Toast.makeText(BaseActivity.this, msg, Toast.LENGTH_SHORT).show();
            }
        });
    }
}
