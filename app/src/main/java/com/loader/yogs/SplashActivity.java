package com.loader.yogs;

import android.app.Activity;
import android.content.Intent;
import android.content.SharedPreferences;
import android.os.Build;
import android.os.Bundle;
import android.os.Handler;
import android.view.View;
import android.view.WindowManager;
import android.widget.ProgressBar;
import android.widget.TextView;
import org.lsposed.lsparanoid.Obfuscate;

@Obfuscate
public class SplashActivity extends Activity {

    private ProgressBar progressBar;
    private TextView progressText;
    private int progress = 0;
    private Handler handler = new Handler();
    private FPrefs prefs;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        //hideSystemUI();
        //Thread.setDefaultUncaughtExceptionHandler(new CrashHandler(this));
        setContentView(R.layout.activity_splash);
        progressBar = findViewById(R.id.progressBar);
        progressText = findViewById(R.id.progressText);
        prefs = FPrefs.with(this);
        boolean isFirstLaunch = prefs.readBoolean("first_launch", true);
        if (isFirstLaunch) {
            progressText.setText("Checking resources...");
            startProgress(5000, true);
        } else {
            progressText.setText("Verified ✅");
            startProgress(2000, false);
        }
    }

    private void startProgress(int totalTime, boolean isFirstLaunch) {
        final int interval = 100;
        final int steps = totalTime / interval;
        final int progressStep = 100 / steps;

        handler.postDelayed(new Runnable() {
            @Override
            public void run() {
                if (progress < 100) {
                    progress += progressStep;
                    progressBar.setProgress(progress);
                    updateProgressText(progress);
                    handler.postDelayed(this, interval);
                } else {
                    if (isFirstLaunch) {
                        prefs.writeBoolean("first_launch", false);
                    }
                    LoginActivity.goLogin(SplashActivity.this);
                    finish();
                }
            }
        }, interval);
    }

    private void updateProgressText(int progress) {
        if (progress < 50) {
            progressText.setText("Checking resources...");
        } else {
            progressText.setText("Finalizing setup...");
        }
    }

    private void goToLogin() {
        startActivity(new Intent(SplashActivity.this, LoginActivity.class));
        finish();
    }
    
    private void hideSystemUI() {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.R) {
            getWindow().getDecorView().getWindowInsetsController().hide(
            android.view.WindowInsets.Type.statusBars() | android.view.WindowInsets.Type.navigationBars());
        } else {
            getWindow().getDecorView().setSystemUiVisibility(
            View.SYSTEM_UI_FLAG_FULLSCREEN | View.SYSTEM_UI_FLAG_HIDE_NAVIGATION | View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY);
            getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,
            WindowManager.LayoutParams.FLAG_FULLSCREEN);
        }
    }
}
