package com.loader.yogs;

import android.view.animation.Animation;
import android.view.animation.AnimationUtils;
import android.widget.Button;
import android.widget.RadioButton;
import android.widget.RadioGroup;
import android.widget.SeekBar;
import android.widget.Toast;
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

import org.lsposed.lsparanoid.Obfuscate;

@Obfuscate
public class Floating extends Service {

    Context mContext;
    private View mFloatingView, espView, logoView;
    private WindowManager mWindowManager;

    int currentTab = 0;
    
    TextView texttab;
    LinearLayout espmenu, aimmenu, setmenu;
    
    private FPrefs prefs;

    private static native void SettingValue(int i, boolean z);
    private static native void SettingValueI(int code, int value);
    private static native void Range(int i);

    @Override
    public IBinder onBind(Intent intent) {
        return null;
    }

    @SuppressLint("CutPasteId")
    @Override
    public void onCreate() {
        super.onCreate();
        this.mContext = this;
        prefs = FPrefs.with(this);
        mFloatingView = LayoutInflater.from(this).inflate(R.layout.floating, null);
        logoView = mFloatingView.findViewById(R.id.logo);
        espView = mFloatingView.findViewById(R.id.menulayout);
        createOver();
        features();
    }

    private void features() {
        if (Overlay.isConnected()) {
            Switch line = mFloatingView.findViewById(R.id.espline);
            line.setChecked(prefs.readBoolean(String.valueOf(line.getText())));
            SettingValue(1, prefs.readBoolean(String.valueOf(line.getText())));
            line.setOnCheckedChangeListener((buttonView, isChecked) -> {
                prefs.writeBoolean(String.valueOf(line.getText()), line.isChecked());
                SettingValue(1, isChecked);
            });
            
            Switch box = mFloatingView.findViewById(R.id.espbox);
            box.setChecked(prefs.readBoolean((String) box.getText()));
            SettingValue(2, prefs.readBoolean((String) box.getText()));
            box.setOnCheckedChangeListener((buttonView, isChecked) -> {
                prefs.writeBoolean(String.valueOf(box.getText()), box.isChecked());
                SettingValue(2, isChecked);
            });
            
            Switch health = mFloatingView.findViewById(R.id.esphealth);
            health.setChecked(prefs.readBoolean((String) health.getText()));
            SettingValue(3, prefs.readBoolean((String) health.getText()));
            health.setOnCheckedChangeListener((buttonView, isChecked) -> {
                prefs.writeBoolean(String.valueOf(health.getText()), health.isChecked());
                SettingValue(3, isChecked);
            });
            
            Switch name = mFloatingView.findViewById(R.id.espname);
            name.setChecked(prefs.readBoolean((String) name.getText()));
            SettingValue(4, prefs.readBoolean((String) name.getText()));
            name.setOnCheckedChangeListener((buttonView, isChecked) -> {
                prefs.writeBoolean(String.valueOf(name.getText()), name.isChecked());
                SettingValue(4, isChecked);
            });
            
            Switch dist = mFloatingView.findViewById(R.id.espdistance);
            dist.setChecked(prefs.readBoolean((String) dist.getText()));
            SettingValue(5, prefs.readBoolean((String) dist.getText()));
            dist.setOnCheckedChangeListener((buttonView, isChecked) -> {
                prefs.writeBoolean(String.valueOf(dist.getText()), dist.isChecked());
                SettingValue(5, isChecked);
            });
            
            Switch alert = mFloatingView.findViewById(R.id.esp360alert);
            alert.setChecked(prefs.readBoolean((String) alert.getText()));
            SettingValue(6, prefs.readBoolean((String) alert.getText()));
            alert.setOnCheckedChangeListener((buttonView, isChecked) -> {
                prefs.writeBoolean(String.valueOf(alert.getText()), alert.isChecked());
                SettingValue(6, isChecked);
            });
            
            Switch nobot = mFloatingView.findViewById(R.id.espskipbot);
            nobot.setChecked(prefs.readBoolean((String) nobot.getText()));
            SettingValue(7, prefs.readBoolean((String) nobot.getText()));
            nobot.setOnCheckedChangeListener((buttonView, isChecked) -> {
                prefs.writeBoolean(String.valueOf(nobot.getText()), nobot.isChecked());
                SettingValue(7, isChecked);
            });
            
            int savedIdd = prefs.readInt("radio_box_id", -1);
            if (savedIdd != -1) {
                View v = mFloatingView.findViewById(savedIdd);
            
                if (v instanceof RadioButton) {
                    RadioButton savedBtnn = (RadioButton) v;
                    savedBtnn.setChecked(true);
            
                    Object tag = savedBtnn.getTag();
                    if (tag != null) {
                        SettingValueI(1, Integer.parseInt(tag.toString()));
                    }
                }
            }
            
            RadioGroup radiooBox = mFloatingView.findViewById(R.id.radiobox);
            radiooBox.setOnCheckedChangeListener((group, checkedId) -> {
                if (checkedId != -1) {
                    View v = mFloatingView.findViewById(checkedId);
            
                    if (v instanceof RadioButton) {
                        RadioButton btn = (RadioButton) v;
            
                        Object tag = btn.getTag();
                        if (tag != null) {
                            SettingValueI(1, Integer.parseInt(tag.toString()));
                        }
            
                        prefs.writeInt("radio_box_id", checkedId);
                    }
                }
            });
            
            int savedId = prefs.readInt("radio_line_id", -1);
            if (savedId != -1) {
                View v = mFloatingView.findViewById(savedId);
            
                if (v instanceof RadioButton) {
                    RadioButton savedBtn = (RadioButton) v;
                    savedBtn.setChecked(true);
            
                    Object tag = savedBtn.getTag();
                    if (tag != null) {
                        SettingValueI(2, Integer.parseInt(tag.toString()));
                    }
                }
            }
            
            RadioGroup radiooLine = mFloatingView.findViewById(R.id.radioline);
            radiooLine.setOnCheckedChangeListener((group, checkedId) -> {
                if (checkedId != -1) {
                    View v = mFloatingView.findViewById(checkedId);
            
                    if (v instanceof RadioButton) {
                        RadioButton btn = (RadioButton) v;
            
                        Object tag = btn.getTag();
                        if (tag != null) {
                            SettingValueI(2, Integer.parseInt(tag.toString()));
                        }
            
                        prefs.writeInt("radio_line_id", checkedId);
                    }
                }
            });
            
            Switch aimsilent = mFloatingView.findViewById(R.id.AimSilent);
            aimsilent.setOnCheckedChangeListener((buttonView, isChecked) -> {
                SettingValue(8, isChecked);
            });
            
            int savedId2 = prefs.readInt("radio_aimpos_id", -1);
            if (savedId2 != -1) {
                View v = mFloatingView.findViewById(savedId2);
            
                if (v instanceof RadioButton) {
                    RadioButton savedBtn = (RadioButton) v;
                    savedBtn.setChecked(true);
            
                    Object tag = savedBtn.getTag();
                    if (tag != null) {
                        SettingValueI(3, Integer.parseInt(tag.toString()));
                    }
                }
            }
            
            RadioGroup radioAimPos = mFloatingView.findViewById(R.id.radioaimpos);
            radioAimPos.setOnCheckedChangeListener((group, checkedId) -> {
                if (checkedId != -1) {
                    View v = mFloatingView.findViewById(checkedId);
            
                    if (v instanceof RadioButton) {
                        RadioButton btn = (RadioButton) v;
            
                        Object tag = btn.getTag();
                        if (tag != null) {
                            SettingValueI(3, Integer.parseInt(tag.toString()));
                        }
            
                        prefs.writeInt("radio_aimpos_id", checkedId);
                    }
                }
            });
            
            final TextView rangs = mFloatingView.findViewById(R.id.textfov);
            final SeekBar range = mFloatingView.findViewById(R.id.aimrange); // Dapatkan referensi SeekBar "range" di sini
    
            int savedFovValue = prefs.readInt("fov", -1); // Gunakan default value yang masuk akal, misal 50 jika -1 tidak cocok
            if (savedFovValue != -1) {
                range.setProgress(savedFovValue); // Atur progress SeekBar yang benar
                Range(savedFovValue);             // Panggil native method Anda dengan nilai yang dimuat
                rangs.setText(String.valueOf(savedFovValue)); // Perbarui TextView
            }
    
            range.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
                @Override
                public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
                    Range(progress);
                    rangs.setText(String.valueOf(progress));
                    prefs.writeInt("fov", progress);
                }
    
                @Override
                public void onStartTrackingTouch(SeekBar seekBar) {
                    // Tidak ada perubahan di sini
                }
    
                @Override
                public void onStopTrackingTouch(SeekBar seekBar) {
                    // Tidak ada perubahan di sini
                }
            });
        }
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
        
        if (Overlay.isConnected()) {
            espmenu.setVisibility(View.VISIBLE);
            aimmenu.setVisibility(View.GONE);
            setmenu.setVisibility(View.GONE);
        } else {
            espmenu.setVisibility(View.GONE);
            aimmenu.setVisibility(View.GONE);
            setmenu.setVisibility(View.GONE);
        }
        
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
