package com.loader.yogs;

import android.app.ActivityManager;
import android.content.Context;
import android.content.Intent;
import android.graphics.Color;
import android.icu.text.SimpleDateFormat;
import android.net.Uri;
import android.os.Bundle;
import android.os.Handler;
import android.provider.Settings;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;
import androidx.appcompat.app.AppCompatActivity;
import com.loader.yogs.databinding.ActivityMainBinding;
import java.util.Date;
import java.util.Locale;

public class MainActivity extends AppCompatActivity {
	
    static {
        System.loadLibrary("yogs");
    }
            
    private ActivityMainBinding binding;
	private static native String EXP();
    
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
		binding = ActivityMainBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());

        if (!Settings.canDrawOverlays(this)) {
            Intent intent = new Intent(Settings.ACTION_MANAGE_OVERLAY_PERMISSION, Uri.parse("package:" + getPackageName()));
            startActivityForResult(intent, 123);
        }

        Button startBtn = binding.startbtn;

        startBtn.setOnClickListener(v -> {
            if (!isServiceRunning()) {
                startService(new Intent(MainActivity.this, Floating.class));
            } else {
                Toast.makeText(this, "Service is already running", Toast.LENGTH_SHORT).show();
            }
        });
    }

    private boolean isServiceRunning() {
        ActivityManager manager = (ActivityManager) getSystemService(Context.ACTIVITY_SERVICE);
        if (manager != null) {
            for (ActivityManager.RunningServiceInfo service : manager.getRunningServices(Integer.MAX_VALUE)) {
                if (Floating.class.getName().equals(service.service.getClassName())) {
                    return true;
                }
            }
        }
        return false;
    }
    
    private void CountTimerAccout() {
        Handler handler = new Handler();
        Runnable runnable = new Runnable() {
            @Override
            public void run() {
                try {
                    handler.postDelayed(this, 1000);
                    SimpleDateFormat dateFormat = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss", Locale.getDefault());
                    Date expiryDate = dateFormat.parse(EXP());

                    long now = System.currentTimeMillis();
                    long distance = expiryDate.getTime() - now;
                    long days = distance / (24 * 60 * 60 * 1000);
                    long hours = distance / (60 * 60 * 1000) % 24;
                    long minutes = distance / (60 * 1000) % 60;
                    long seconds = distance / 1000 % 60;
                    
                    if (distance > 0) {
                        TextView Hari = binding.textday;
                        TextView Jam = binding.texthour;
                        TextView Menit = binding.textminute;
                        TextView Detik = binding.textsecond;
                        if (days > 0) {
                            Hari.setText(" " + String.format("%02d", days));
                        }
                        if (hours > 0) {
                            Jam.setText(" " + String.format("%02d", hours));
                        }
                        if (minutes > 0) {
                            Menit.setText(" " + String.format("%02d", minutes));
                        }
                        if (seconds > 0) {
                            Detik.setText(" " + String.format("%02d", seconds));
                        }
                    }
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }
        };
        handler.postDelayed(runnable, 0);
    }
    
    @Override
    protected void onResume() {
        super.onResume();
        CountTimerAccout();
    }
    
    @Override
    protected void onDestroy() {
        super.onDestroy();
        this.binding = null;
        if (isServiceRunning()) {
            stopService(new Intent(MainActivity.this, Floating.class));
        }
    }
}