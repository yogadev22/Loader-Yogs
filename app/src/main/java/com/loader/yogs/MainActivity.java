package com.loader.yogs;

import android.Manifest;
import android.app.ActivityManager;
import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.graphics.Color;
import android.icu.text.SimpleDateFormat;
import android.net.Uri;
import android.os.Build;
import android.os.Bundle;
import android.os.Environment;
import android.os.Handler;
import android.os.Looper;
import android.provider.Settings;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.ActivityCompat;

import com.loader.yogs.databinding.ActivityMainBinding;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.Date;
import java.util.Locale;
import com.topjohnwu.superuser.Shell;

import org.lsposed.lsparanoid.Obfuscate;

@Obfuscate
public class MainActivity extends AppCompatActivity {
	
    static {
        System.loadLibrary("yogs");
    }
            
    private ActivityMainBinding binding;
	private static native String EXP();
    public static String daemon64;
    private String daemonPath64;
    private static final int REQUEST_MANAGE_STORAGE_PERMISSION = 100;
    private static final int REQUEST_MANAGE_UNKNOWN_APP_SOURCES = 200;

    
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
		binding = ActivityMainBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());

        if (!Settings.canDrawOverlays(this)) {
            Intent intent = new Intent(Settings.ACTION_MANAGE_OVERLAY_PERMISSION, Uri.parse("package:" + getPackageName()));
            startActivityForResult(intent, 123);
        }

        if (Shell.rootAccess()) {
            binding.mode.setText("Root");
            binding.mode.setTextColor(Color.RED);
            daemon64 = "su -c " + getFilesDir().getPath()  + "/fuck";
        } else {
            binding.mode.setText("Container");
            binding.mode.setTextColor(Color.GREEN);
            daemon64 = getFilesDir().getPath()  + "/fuck";
        }

        loadAssets64();

        binding.startbtn.setOnClickListener(v -> {
            if (!isServiceRunning()) {
                //openGame(MainActivity.this);
                Initialize(MainActivity.this);
            } else {
                stopService(new Intent(MainActivity.this, Floating.class));
                stopService(new Intent(MainActivity.this, Overlay.class));
            }
            // Beri sedikit delay agar sistem memperbarui status service sebelum UI diupdate
            new Handler().postDelayed(this::updateButtonUI, 200);
        });
    }
    
    private void Initialize(final Context context) {
        try {
            PackageManager packageManager = context.getPackageManager();
            Intent launchIntent = packageManager.getLaunchIntentForPackage("com.dts.freefiremax");
            if (launchIntent != null) {
                context.startActivity(launchIntent);
            }
            Thread.sleep(3000);
            startService(new Intent(MainActivity.this, Overlay.class));
            startService(new Intent(MainActivity.this, Floating.class));
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
        
    // Fungsi untuk memperbarui tampilan tombol
    private void updateButtonUI() {
        if (isServiceRunning()) {
            binding.startbtn.setText("STOP");
            binding.startbtn.setBackgroundColor(Color.parseColor("#D32F2F")); // Merah saat Running
        } else {
            binding.startbtn.setText("START");
            binding.startbtn.setBackgroundColor(Color.parseColor("#388E3C")); // Hijau saat Berhenti
        }
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
                            Hari.setText(String.format("%02d", days));
                        }
                        if (hours > 0) {
                            Jam.setText(String.format("%02d", hours));
                        }
                        if (minutes > 0) {
                            Menit.setText(String.format("%02d", minutes));
                        }
                        if (seconds > 0) {
                            Detik.setText(String.format("%02d", seconds));
                        }
                    }
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }
        };
        handler.postDelayed(runnable, 0);
    }

    private boolean isStoragePermissionGranted() {
        return Build.VERSION.SDK_INT < Build.VERSION_CODES.R || Environment.isExternalStorageManager();
    }

    private void requestStoragePermissionDirect() {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.R) {
            Intent intent = new Intent(Settings.ACTION_MANAGE_APP_ALL_FILES_ACCESS_PERMISSION);
            intent.setData(Uri.fromParts("package", getPackageName(), null));
            startActivityForResult(intent, REQUEST_MANAGE_STORAGE_PERMISSION);
        } else {
            ActivityCompat.requestPermissions(this,
                    new String[]{Manifest.permission.WRITE_EXTERNAL_STORAGE},
                    REQUEST_MANAGE_STORAGE_PERMISSION);
        }
    }

    private boolean canRequestPackageInstalls() {
        return Build.VERSION.SDK_INT < Build.VERSION_CODES.O
                || getPackageManager().canRequestPackageInstalls();
    }

    private void requestUnknownAppPermissionsDirect() {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
            Intent intent = new Intent(Settings.ACTION_MANAGE_UNKNOWN_APP_SOURCES,
                    Uri.parse("package:" + getPackageName()));
            startActivityForResult(intent, REQUEST_MANAGE_UNKNOWN_APP_SOURCES);
        }
    }

    private void loadAssets64() {
        String filesDir = getFilesDir().getPath()  + "/fuck";
        try {
            OutputStream myOutput = new FileOutputStream(filesDir);
            byte[] buffer = new byte[1024];
            int length;
            InputStream myInput = getAssets().open("fuck");
            while ((length = myInput.read(buffer)) != -1) {
                myOutput.write(buffer, 0, length);
            }
            myInput.close();
            myOutput.flush();
            myOutput.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
        daemonPath64 = getFilesDir().getPath()  + "/fuck";
        try {
            Runtime.getRuntime().exec("chmod 777 " + daemonPath64);
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
    
    @Override
    protected void onResume() {
        super.onResume();
        CountTimerAccout();
        updateButtonUI(); // Update UI saat aplikasi dibuka kembali
    }
    
    @Override
    protected void onDestroy() {
        super.onDestroy();
        this.binding = null;
        // Note: Jika ingin service tetap jalan meski activity ditutup, hapus baris stopService di bawah ini.
        stopService(new Intent(MainActivity.this, Floating.class));
        stopService(new Intent(MainActivity.this, Overlay.class));
    }
}
