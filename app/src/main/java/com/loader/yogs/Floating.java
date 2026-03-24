package com.loader.yogs;

import static com.loader.yogs.Overlay.getConfig;

import android.animation.ObjectAnimator;
import android.annotation.SuppressLint;
import android.app.Service;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.PixelFormat;
import android.os.Build;
import android.os.IBinder;
import android.view.GestureDetector;
import android.view.LayoutInflater;
import android.view.MotionEvent;
import android.view.View;
import android.view.WindowManager;
import android.widget.CheckBox;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.RelativeLayout;
import android.widget.Switch;
import android.widget.TextView;

public class Floating extends Service {

    Context mContext;
    private View mFloatingView, espView, logoView;
    private WindowManager mWindowManager;

    private static native void SettingValue(int i, boolean z);

    @Override
    public IBinder onBind(Intent intent) {
        return null;
    }

    @SuppressLint("CutPasteId")
    @Override
    public void onCreate() {
        super.onCreate();
        this.mContext = this;
        mFloatingView = LayoutInflater.from(this).inflate(R.layout.floating, null);
        logoView = mFloatingView.findViewById(R.id.logo);
        espView = mFloatingView.findViewById(R.id.menulayout);
        createOver();
        features();
    }

    private void features() {
        Switch enableesp = mFloatingView.findViewById(R.id.enableesp);
        enableesp.setOnCheckedChangeListener((buttonView, isChecked) -> {
            startService(new Intent(Floating.this, Overlay.class));
        });

        Switch line = mFloatingView.findViewById(R.id.espline);
        line.setChecked(getConfig((String) line.getText()));
        SettingValue(9, getConfig((String) line.getText()));
        line.setOnCheckedChangeListener((buttonView, isChecked) -> {
            setValue(String.valueOf(line.getText()), line.isChecked());
            SettingValue(1, isChecked);
        });
    }

    private void setValue(String key, boolean value) {
        SharedPreferences sp = getSharedPreferences("espValue", Context.MODE_PRIVATE);
        SharedPreferences.Editor ed = sp.edit();
        ed.putBoolean(key, value);
        ed.apply();
    }

    private boolean getConfig(String key) {
        SharedPreferences sp = getSharedPreferences("espValue", Context.MODE_PRIVATE);
        return sp.getBoolean(key, false);
    }

    @SuppressLint("InflateParams")
    void createOver() {
        int LAYOUT_FLAG;
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
            LAYOUT_FLAG = WindowManager.LayoutParams.TYPE_APPLICATION_OVERLAY;
        } else {
            LAYOUT_FLAG = WindowManager.LayoutParams.TYPE_PHONE;
        }

        final WindowManager.LayoutParams params = new WindowManager.LayoutParams(
                WindowManager.LayoutParams.WRAP_CONTENT, WindowManager.LayoutParams.WRAP_CONTENT,
                LAYOUT_FLAG, WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE, PixelFormat.RGBA_8888
        );

        mWindowManager = (WindowManager) getSystemService(WINDOW_SERVICE);
        mWindowManager.addView(mFloatingView, params);

        TextView closeBtn = mFloatingView.findViewById(R.id.closetext);
        closeBtn.setOnClickListener(view -> {
            espView.setVisibility(View.GONE);
            logoView.setVisibility(View.VISIBLE);
        });

        ImageView espBtn = mFloatingView.findViewById(R.id.esptab);
        ImageView settingBtn = mFloatingView.findViewById(R.id.settingtab);
        ImageView trackBtn = mFloatingView.findViewById(R.id.aimtab);

        LinearLayout espmenu = mFloatingView.findViewById(R.id.espmenu);

        espBtn.setOnClickListener(view -> {
            settingBtn.setBackgroundColor(Color.parseColor("#FF17202E"));
            espBtn.setBackgroundColor(Color.parseColor("#2E8B57"));
            settingBtn.setBackgroundColor(Color.parseColor("#FF17202E"));
            trackBtn.setBackgroundColor(Color.parseColor("#FF17202E"));
            espmenu.setVisibility(View.VISIBLE);
        });

        final GestureDetector gestureDetector = new GestureDetector(this, new SingleTapConfirm());

        mFloatingView.findViewById(R.id.logo).setOnTouchListener(new View.OnTouchListener() {
            private int initialX;
            private int initialY;
            private float initialTouchX;
            private float initialTouchY;
            private ObjectAnimator alphaAnimator;

            @SuppressLint("ClickableViewAccessibility")
            @Override
            public boolean onTouch(View v, MotionEvent event) {
                if (gestureDetector.onTouchEvent(event)) {
                    espView.setVisibility(View.VISIBLE);
                    logoView.setVisibility(View.GONE);
                    return true;
                } else {
                    switch (event.getAction()) {
                        case MotionEvent.ACTION_DOWN:
                            initialX = params.x;
                            initialY = params.y;
                            initialTouchX = event.getRawX();
                            initialTouchY = event.getRawY();
                            v.setAlpha(1f);
                            return true;
                        case MotionEvent.ACTION_MOVE:
                            params.x = initialX + (int) (event.getRawX() - initialTouchX);
                            params.y = initialY + (int) (event.getRawY() - initialTouchY);
                            mWindowManager.updateViewLayout(mFloatingView, params);
                            return true;
                        case MotionEvent.ACTION_UP:
                            alphaAnimator = ObjectAnimator.ofFloat(v, "alpha", 0.5f);
                            alphaAnimator.setDuration(500); // Adjust the duration as desired
                            alphaAnimator.start();
                            return true;
                    }
                    return false;
                }
            }
        });


        mFloatingView.findViewById(R.id.menulayout).setOnTouchListener(new View.OnTouchListener() {
            private int initialX;
            private int initialY;
            private float initialTouchX;
            private float initialTouchY;

            @SuppressLint("ClickableViewAccessibility")
            @Override
            public boolean onTouch(View v, MotionEvent event) {
                switch (event.getAction()) {
                    case MotionEvent.ACTION_DOWN:
                        initialX = params.x;
                        initialY = params.y;
                        initialTouchX = event.getRawX();
                        initialTouchY = event.getRawY();
                        return true;
                    case MotionEvent.ACTION_MOVE:
                        params.x = initialX + (int) (event.getRawX() - initialTouchX);
                        params.y = initialY + (int) (event.getRawY() - initialTouchY);
                        mWindowManager.updateViewLayout(mFloatingView, params);
                        return true;
                }
                return false;
            }
        });
    }

    class SingleTapConfirm extends GestureDetector.SimpleOnGestureListener {
        @Override
        public boolean onSingleTapUp(MotionEvent event) {
            return true;
        }
    }

    @Override
    public void onDestroy() {
        super.onDestroy();
        if (mFloatingView != null) {
            mWindowManager.removeView(mFloatingView);
        }
    }
}
