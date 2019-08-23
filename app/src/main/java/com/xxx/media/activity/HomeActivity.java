package com.xxx.media.activity;

import butterknife.ButterKnife;
import butterknife.OnClick;

import android.os.Bundle;

import com.xxx.media.R;

public class HomeActivity extends BaseActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_home);
        ButterKnife.bind(this);
    }

    @OnClick(R.id.btn_OpenGLActivity)
    void btn_OpenGLActivity() {
        startPage(OpenGLActivity.class);
    }
}
