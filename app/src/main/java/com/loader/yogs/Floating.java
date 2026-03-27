package com.loader.yogs;

import android.view.animation.Animation;
import android.view.animation.AnimationUtils;
import android.widget.Button;
import android.widget.RadioButton;
import android.widget.RadioGroup;
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

    int currentTab = 0;
    
    TextView texttab;
    LinearLayout espmenu, aimmenu, setmenu;

    private static native void SettingValue(int i, boolean z);
    private static native void SettingValueI(int code, int value);

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
        SettingValue(1, getConfig((String) line.getText()));
        line.setOnCheckedChangeListener((buttonView, isChecked) -> {
            setValue(String.valueOf(line.getText()), line.isChecked());
            SettingValue(1, isChecked);
        });
        
        Switch box = mFloatingView.findViewById(R.id.espbox);
        box.setChecked(getConfig((String) box.getText()));
        SettingValue(2, getConfig((String) box.getText()));
        box.setOnCheckedChangeListener((buttonView, isChecked) -> {
            setValue(String.valueOf(box.getText()), box.isChecked());
            SettingValue(2, isChecked);
        });
        
        Switch health = mFloatingView.findViewById(R.id.esphealth);
        health.setChecked(getConfig((String) health.getText()));
        SettingValue(3, getConfig((String) health.getText()));
        health.setOnCheckedChangeListener((buttonView, isChecked) -> {
            setValue(String.valueOf(line.getText()), health.isChecked());
            SettingValue(3, isChecked);
        });
        
        Switch name = mFloatingView.findViewById(R.id.espname);
        name.setChecked(getConfig((String) name.getText()));
        SettingValue(4, getConfig((String) name.getText()));
        name.setOnCheckedChangeListener((buttonView, isChecked) -> {
            setValue(String.valueOf(name.getText()), name.isChecked());
            SettingValue(4, isChecked);
        });
        
        Switch dist = mFloatingView.findViewById(R.id.espdistance);
        dist.setChecked(getConfig((String) dist.getText()));
        SettingValue(5, getConfig((String) dist.getText()));
        dist.setOnCheckedChangeListener((buttonView, isChecked) -> {
            setValue(String.valueOf(dist.getText()), dist.isChecked());
            SettingValue(5, isChecked);
        });
        
        Switch alert = mFloatingView.findViewById(R.id.esp360alert);
        alert.setChecked(getConfig((String) alert.getText()));
        SettingValue(6, getConfig((String) alert.getText()));
        alert.setOnCheckedChangeListener((buttonView, isChecked) -> {
            setValue(String.valueOf(alert.getText()), alert.isChecked());
            SettingValue(6, isChecked);
        });
        
        Switch nobot = mFloatingView.findViewById(R.id.espskipbot);
        nobot.setChecked(getConfig((String) nobot.getText()));
        SettingValue(7, getConfig((String) nobot.getText()));
        nobot.setOnCheckedChangeListener((buttonView, isChecked) -> {
            setValue(String.valueOf(nobot.getText()), nobot.isChecked());
            SettingValue(7, isChecked);
        });
        
        final SharedPreferences prefs = mContext.getSharedPreferences("MyPrefs", Context.MODE_PRIVATE);

        int savedIdd = prefs.getInt("radio_box_id", -1);
        if (savedIdd != -1) {
            RadioButton savedBtnn = mFloatingView.findViewById(savedIdd);
            if (savedBtnn != null) {
                savedBtnn.setChecked(true);
                SettingValueI(1, Integer.parseInt(savedBtnn.getTag().toString()));
            }
        }

        final RadioGroup radiooBox = mFloatingView.findViewById(R.id.radiobox);
        radiooBox.setOnCheckedChangeListener(new RadioGroup.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(RadioGroup radioGroup, int i) {
                int chkdId = radiooBox.getCheckedRadioButtonId();
                RadioButton btn = mFloatingView.findViewById(chkdId);
                SettingValueI(1, Integer.parseInt(btn.getTag().toString()));

                SharedPreferences.Editor editor = prefs.edit();
                editor.putInt("radio_box_id", chkdId);
                editor.apply();
            }
        });

        int savedId = prefs.getInt("radio_line_id", -1);
        if (savedId != -1) {
            RadioButton savedBtn = mFloatingView.findViewById(savedId);
            if (savedBtn != null) {
                savedBtn.setChecked(true);
                SettingValueI(2, Integer.parseInt(savedBtn.getTag().toString()));
            }
        }

        final RadioGroup radiooLine = mFloatingView.findViewById(R.id.radioline);
        radiooLine.setOnCheckedChangeListener(new RadioGroup.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(RadioGroup radioGroup, int i) {
                int chkdId = radiooLine.getCheckedRadioButtonId();
                RadioButton btn = mFloatingView.findViewById(chkdId);

                // Pakai SettingValueI seperti biasa
                SettingValueI(2, Integer.parseInt(btn.getTag().toString()));

                SharedPreferences.Editor editor = prefs.edit();
                editor.putInt("radio_line_id", chkdId);
                editor.apply();
            }
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

        Button prevBtn = mFloatingView.findViewById(R.id.prevtab);
        Button nextBtn = mFloatingView.findViewById(R.id.nexttab);
        
        texttab = mFloatingView.findViewById(R.id.texttab);
        
        texttab.setText("ESP");
        
        espmenu = mFloatingView.findViewById(R.id.espmenu);
        aimmenu = mFloatingView.findViewById(R.id.aimmenu);
        setmenu = mFloatingView.findViewById(R.id.setmenu);
        
        espView.setVisibility(View.GONE);

        int totalTab = 3;

        nextBtn.setOnClickListener(v -> {
            int newTab = (currentTab + 1) % totalTab;
            showTab(newTab, true);
        });
        
        prevBtn.setOnClickListener(v -> {
            int newTab = (currentTab - 1 + totalTab) % totalTab;
            showTab(newTab, false);
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
    
    private LinearLayout getTab(int index) {
        switch (index) {
            case 0: 
                texttab.setText("ESP");
                return espmenu;
            case 1: 
                texttab.setText("AIM");
                return aimmenu;
            case 2: 
                texttab.setText("SETTING");
                return setmenu;
        }
        return espmenu;
    }
    
    private void showTab(int newTab, boolean isNext) {
        LinearLayout current = getTab(currentTab);
        LinearLayout next = getTab(newTab);
    
        Animation in, out;
    
        if (isNext) {
            in = AnimationUtils.loadAnimation(this, R.anim.slide_in_right);
            out = AnimationUtils.loadAnimation(this, R.anim.slide_out_left);
        } else {
            in = AnimationUtils.loadAnimation(this, R.anim.slide_in_left);
            out = AnimationUtils.loadAnimation(this, R.anim.slide_out_right);
        }
    
        // SET NEXT DULU
        next.setVisibility(View.VISIBLE);
        next.startAnimation(in);
    
        // HANDLE CURRENT
        out.setAnimationListener(new Animation.AnimationListener() {
            @Override
            public void onAnimationEnd(Animation animation) {
                current.setVisibility(View.GONE); // ✅ pindah ke sini
            }
    
            @Override public void onAnimationStart(Animation animation) {}
            @Override public void onAnimationRepeat(Animation animation) {}
        });
    
        current.startAnimation(out);
    
        currentTab = newTab;
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
