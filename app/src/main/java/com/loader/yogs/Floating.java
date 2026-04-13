package com.loader.yogs;

import android.app.ActivityManager;
import android.graphics.Typeface;
import android.text.TextUtils;
import android.view.Gravity;
import android.view.ViewConfiguration;
import android.view.animation.Animation;
import android.view.animation.AnimationUtils;
import android.widget.Button;
import android.widget.FrameLayout;
import android.widget.RadioButton;
import android.widget.RadioGroup;
import android.widget.ScrollView;
import android.widget.SeekBar;
import android.widget.Toast;
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

import androidx.cardview.widget.CardView;
import java.io.IOException;
import java.util.Arrays;
import java.util.LinkedList;
import java.util.List;
import org.lsposed.lsparanoid.Obfuscate;

@Obfuscate
public class Floating extends Service {

    static {
        System.loadLibrary("yogs");
    }
        
    Context mContext;
    private FrameLayout mFloatingView;
    private WindowManager mWindowManager;
    int currentTab = 0;
    
    TextView texttab;
    LinearLayout espmenu, aimmenu, setmenu;
    
    private FPrefs prefs;

    private static native void Changes(Context ctx, int featNum, String featName, int value, long Lvalue, boolean b, String text);
    native String[] GetFeatureList();
    
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
        createOver();
    }

    @SuppressLint("InflateParams")
    void createOver() {
        mFloatingView = new FrameLayout(this);
        mFloatingView.setLayoutParams(new LinearLayout.LayoutParams(LinearLayout.LayoutParams.WRAP_CONTENT, LinearLayout.LayoutParams.WRAP_CONTENT));
        
        RelativeLayout logoView = new RelativeLayout(this);
        logoView.setLayoutParams(new LinearLayout.LayoutParams(dp(50), dp(50)));
        ImageView logoImage = new ImageView(this);
        logoImage.setLayoutParams(new LinearLayout.LayoutParams(LinearLayout.LayoutParams.MATCH_PARENT, LinearLayout.LayoutParams.MATCH_PARENT));
        logoImage.setImageDrawable(getDrawable(R.drawable.yogs));
        logoView.addView(logoImage);
        mFloatingView.addView(logoView);
        
        CardView espView = new CardView(this);
        espView.setLayoutParams(new LinearLayout.LayoutParams(dp(350), dp(350)));
        espView.setCardElevation(10);
        espView.setCardBackgroundColor(getColor(R.color.dark));
        espView.setRadius(16);
        
        LinearLayout mnulyt = new LinearLayout(this);
        LinearLayout.LayoutParams anjinnn = new LinearLayout.LayoutParams(LinearLayout.LayoutParams.MATCH_PARENT, LinearLayout.LayoutParams.WRAP_CONTENT);
        anjinnn.setMargins(0, dp(8), 0, 0);
        mnulyt.setLayoutParams(anjinnn);
        mnulyt.setPadding(dp(8), dp(8), dp(8), dp(8));
        mnulyt.setOrientation(LinearLayout.VERTICAL);
        
        LinearLayout titlelyt = new LinearLayout(this);
        titlelyt.setLayoutParams(new LinearLayout.LayoutParams(LinearLayout.LayoutParams.MATCH_PARENT, LinearLayout.LayoutParams.WRAP_CONTENT));
        titlelyt.setPadding(dp(10), dp(10), dp(10), dp(10));
        titlelyt.setGravity(Gravity.CENTER);
        titlelyt.setBackground(getDrawable(R.drawable.bg_button));
        
        TextView title = new TextView(this);
        title.setLayoutParams(new LinearLayout.LayoutParams(LinearLayout.LayoutParams.WRAP_CONTENT, LinearLayout.LayoutParams.WRAP_CONTENT));
        title.setTextSize(16);
        title.setTextColor(Color.WHITE);
        title.setText("YOGS INJECTOR");
        title.setTypeface(null, Typeface.BOLD);
        
        titlelyt.addView(title);
        mnulyt.addView(titlelyt);
        
        LinearLayout featurelyt = new LinearLayout(this);
        LinearLayout.LayoutParams params1 = new LinearLayout.LayoutParams(
                LinearLayout.LayoutParams.MATCH_PARENT,
                LinearLayout.LayoutParams.WRAP_CONTENT
        );
        params1.setMargins(dp(8), dp(8), dp(8), dp(8));
        featurelyt.setLayoutParams(params1);
        featurelyt.setOrientation(LinearLayout.VERTICAL);
        
        ScrollView scrl = new ScrollView(this);
        LinearLayout.LayoutParams scrollParams = new LinearLayout.LayoutParams(
                LinearLayout.LayoutParams.MATCH_PARENT,
                0
        );
        scrollParams.weight = 1f;
        scrl.setLayoutParams(scrollParams);
        scrl.setScrollBarSize(0);
        
        LinearLayout featurelyt2 = new LinearLayout(this);
        featurelyt2.setLayoutParams(new LinearLayout.LayoutParams(LinearLayout.LayoutParams.MATCH_PARENT, LinearLayout.LayoutParams.WRAP_CONTENT));
        featurelyt2.setOrientation(LinearLayout.VERTICAL);
        
        Button n = new Button(this);
        LinearLayout.LayoutParams paramss2ss = new LinearLayout.LayoutParams(LinearLayout.LayoutParams.MATCH_PARENT, LinearLayout.LayoutParams.WRAP_CONTENT);
        paramss2ss.setMargins(dp(6), dp(6), dp(6), dp(6));
        n.setLayoutParams(paramss2ss);
        n.setBackgroundResource(R.drawable.bg_switch);
        n.setPadding(dp(5), dp(5), dp(5), dp(5));
        n.setTextSize(15);
        n.setTextColor(Color.WHITE);
        n.setText("START MENU");
        
        n.setOnClickListener(view -> {
            startService(new Intent(this, Overlay.class));
            n.setVisibility(View.GONE);
            featureList(GetFeatureList(), featurelyt2);
        });
        
        featurelyt2.addView(n);
        
        scrl.addView(featurelyt2);
        featurelyt.addView(scrl);
        mnulyt.addView(featurelyt);
        espView.addView(mnulyt);
        mFloatingView.addView(espView);
        
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

        title.setOnClickListener(view -> {
            espView.setVisibility(View.GONE);
            logoView.setVisibility(View.VISIBLE);
        });

        espView.setVisibility(View.GONE);

        final GestureDetector gestureDetector = new GestureDetector(this, new SingleTapConfirm());

        logoView.setOnTouchListener(new View.OnTouchListener() {
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
                        /*case MotionEvent.ACTION_UP:
                            alphaAnimator = ObjectAnimator.ofFloat(v, "alpha", 0.5f);
                            alphaAnimator.setDuration(500); // Adjust the duration as desired
                            alphaAnimator.start();
                            return true;*/
                    }
                    return false;
                }
            }
        });


        espView.setOnTouchListener(new View.OnTouchListener() {
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
    
    private void featureList(String[] listFT, LinearLayout linearLayout) {
        //Currently looks messy right now. Let me know if you have improvements
        int featNum, subFeat = 0;
        LinearLayout llBak = linearLayout;

        for (int i = 0; i < listFT.length; i++) {
            boolean switchedOn = false;
            //Log.i("featureList", listFT[i]);
            String feature = listFT[i];
            if (feature.contains("_True")) {
                switchedOn = true;
                feature = feature.replaceFirst("_True", "");
            }

            linearLayout = llBak;
            String[] str = feature.split("_");

            //Assign feature number
            if (TextUtils.isDigitsOnly(str[0]) || str[0].matches("-[0-9]*")) {
                featNum = Integer.parseInt(str[0]);
                feature = feature.replaceFirst(str[0] + "_", "");
                subFeat++;
            } else {
                //Subtract feature number. We don't want to count ButtonLink, Category, RichTextView and RichWebView
                featNum = i - subFeat;
            }
            String[] strSplit = feature.split("_");
            switch (strSplit[0]) {
                case "Toggle":
                    addSwitch(strSplit[1], featNum, switchedOn, strSplit[2], linearLayout);
                    break;
                case "Seekbar":
                    addSeekbar(strSplit[1], featNum, switchedOn, strSplit[3], Integer.parseInt(strSplit[2]), linearLayout);
                    break;
                case "RadioButton":
                    addRadioButton(strSplit[1], featNum, switchedOn, strSplit[3], strSplit[2], linearLayout);
                    break;
                case "TitleMenu":
                    addText(strSplit[1], linearLayout);
                    break;
            }
        }
    }
    
    private void addSwitch(String name, int featnum, boolean isautosave, String gamename, LinearLayout view) {
        Switch n = new Switch(this);
        LinearLayout.LayoutParams params = new LinearLayout.LayoutParams(LinearLayout.LayoutParams.MATCH_PARENT, LinearLayout.LayoutParams.WRAP_CONTENT);
        params.setMargins(dp(6), dp(6), dp(6), dp(6));
        n.setLayoutParams(params);
        n.setBackgroundResource(R.drawable.bg_switch);
        n.setPadding(dp(10), dp(10), dp(10), dp(10));
        n.setTextSize(15);
        n.setTextColor(Color.WHITE);
        n.setText(name);
        if (isautosave) {
            n.setChecked(prefs.readBoolean(gamename + "_" + name));
            Changes(this, featnum, name, 0, 0, prefs.readBoolean(gamename + "_" + name), null);
        }
        
        n.setOnCheckedChangeListener((buttonView, isChecked) -> {
            Changes(this, featnum, name, 0, 0, isChecked, null);
            if (isautosave) {
                prefs.writeBoolean(gamename + "_" + name, n.isChecked());
            }
        });
        
        view.addView(n);
    }
    
    private void addSeekbar(String name, int featnum, boolean isautosave, String gamename, int max, LinearLayout view) {
        LinearLayout texlyt = new LinearLayout(this);
        LinearLayout.LayoutParams paramss = new LinearLayout.LayoutParams(LinearLayout.LayoutParams.MATCH_PARENT, LinearLayout.LayoutParams.WRAP_CONTENT);
        paramss.setMargins(dp(6), dp(6), dp(6), dp(6));
        texlyt.setLayoutParams(paramss);
        texlyt.setBackgroundResource(R.drawable.bg_switch);
        texlyt.setPadding(dp(10), dp(10), dp(10), dp(10));
        texlyt.setOrientation(LinearLayout.VERTICAL);
        
        LinearLayout textlyt = new LinearLayout(this);
        LinearLayout.LayoutParams paramsss = new LinearLayout.LayoutParams(LinearLayout.LayoutParams.MATCH_PARENT, LinearLayout.LayoutParams.WRAP_CONTENT);
        textlyt.setLayoutParams(paramsss);
        textlyt.setOrientation(LinearLayout.HORIZONTAL);
        
        TextView sTitle = new TextView(this);
        LinearLayout.LayoutParams paramssss = new LinearLayout.LayoutParams(LinearLayout.LayoutParams.WRAP_CONTENT, LinearLayout.LayoutParams.WRAP_CONTENT);
        sTitle.setLayoutParams(paramssss);
        sTitle.setText(name + ": ");
        sTitle.setTextSize(15);
        sTitle.setTextColor(Color.WHITE);
        
        TextView sTitlee = new TextView(this);
        sTitlee.setLayoutParams(paramssss);
        if (isautosave) {
            sTitlee.setText(String.valueOf(prefs.readInt(gamename + "_" + name, -1)));
        }
        sTitlee.setTextSize(15);
        sTitlee.setTextColor(Color.WHITE);
        
        textlyt.addView(sTitle);
        textlyt.addView(sTitlee);
        texlyt.addView(textlyt);
        
        SeekBar s = new SeekBar(this);
        LinearLayout.LayoutParams params = new LinearLayout.LayoutParams(LinearLayout.LayoutParams.MATCH_PARENT, LinearLayout.LayoutParams.MATCH_PARENT);
        s.setLayoutParams(params);
        s.setMax(max);
        
        int slidervalue = 0;

        if (isautosave) {
            slidervalue = prefs.readInt(gamename + "_" + name, 0);
        }
        
        sTitlee.setText(String.valueOf(slidervalue));
        s.setProgress(slidervalue);
        Changes(mContext, featnum, name, slidervalue, 0, false, null);
        
        s.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
                Changes(mContext, featnum, name, progress, 0, false, null);
                sTitlee.setText(String.valueOf(progress));
                prefs.writeInt(gamename + "_" + name, progress);
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
        
        texlyt.addView(s);
        view.addView(texlyt);
    }
    
    private void addRadioButton(String name, int featnum, boolean isautosave, String gamename, String list, LinearLayout view) {
        List<String> lists = new LinkedList<>(Arrays.asList(list.split(",")));
    
        LinearLayout texlyt = new LinearLayout(this);
        LinearLayout.LayoutParams paramss = new LinearLayout.LayoutParams(
                LinearLayout.LayoutParams.MATCH_PARENT,
                LinearLayout.LayoutParams.WRAP_CONTENT
        );
        paramss.setMargins(dp(6), dp(6), dp(6), dp(6));
        texlyt.setLayoutParams(paramss);
        texlyt.setBackgroundResource(R.drawable.bg_switch);
        texlyt.setPadding(dp(10), dp(10), dp(10), dp(10));
        texlyt.setOrientation(LinearLayout.VERTICAL);
    
        TextView title = new TextView(this);
        LinearLayout.LayoutParams paramsss = new LinearLayout.LayoutParams(
                LinearLayout.LayoutParams.MATCH_PARENT,
                LinearLayout.LayoutParams.WRAP_CONTENT
        );
        title.setLayoutParams(paramsss);
        title.setText(name);
        title.setTextSize(14);
        title.setTextColor(Color.GREEN);
    
        RadioGroup group = new RadioGroup(this);
        group.setLayoutParams(paramsss);
        group.setOrientation(RadioGroup.VERTICAL);
    
        // 🔹 Buat RadioButton
        for (int i = 0; i < lists.size(); i++) {
            RadioButton button = new RadioButton(this);
            button.setLayoutParams(paramsss);
            button.setText(lists.get(i));
            button.setTextSize(14);
            button.setTextColor(Color.WHITE);
            button.setTag(i); // simpan index
            group.addView(button);
        }
    
        // 🔹 LOAD DATA (autosave)
        int savedIndex = 0;
        if (isautosave) {
            savedIndex = prefs.readInt(gamename + "_" + name, 0);
        }
    
        // 🔹 Set checked sesuai savedIndex
        if (group.getChildCount() > savedIndex) {
            RadioButton btn = (RadioButton) group.getChildAt(savedIndex);
            btn.setChecked(true);
            Changes(mContext, featnum, name, Integer.parseInt(btn.getTag().toString()), 0, false, null);
        }
    
        // 🔹 Listener perubahan
        group.setOnCheckedChangeListener((grp, checkedId) -> {
            if (checkedId != -1) {
                RadioButton btn = grp.findViewById(checkedId);
    
                if (btn != null) {
                    Object tag = btn.getTag();
                    if (tag != null) {
                        int index = Integer.parseInt(tag.toString());
    
                        // kirim ke native / fitur
                        Changes(mContext, featnum, name, index, 0, false, null);
    
                        // autosave
                        if (isautosave) {
                            prefs.writeInt(gamename + "_" + name, index);
                        }
                    }
                }
            }
        });
    
        // 🔹 Tambahkan ke layout
        texlyt.addView(title);
        texlyt.addView(group);
        view.addView(texlyt);
    }
    
    private void addText(String name, LinearLayout view) {
        LinearLayout texlyt = new LinearLayout(this);
        LinearLayout.LayoutParams paramss = new LinearLayout.LayoutParams(
                LinearLayout.LayoutParams.MATCH_PARENT,
                LinearLayout.LayoutParams.WRAP_CONTENT
        );
        texlyt.setLayoutParams(paramss);
        texlyt.setGravity(Gravity.CENTER);
        
        TextView text = new TextView(mContext);
        text.setLayoutParams(paramss);
        text.setText(name);
        text.setTextSize(14);
        text.setTextColor(Color.WHITE);
        text.setGravity(Gravity.CENTER);
        
        texlyt.addView(text);
        view.addView(texlyt);
    }
    
    private int dp(int value) {
        return (int) (value * getResources().getDisplayMetrics().density);
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
