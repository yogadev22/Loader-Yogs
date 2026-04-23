package com.loader.yogs;

import android.annotation.SuppressLint;
import android.app.AlertDialog;
import android.content.ClipData;
import android.content.ClipboardManager;
import android.content.Context;
import android.content.Intent;
import android.content.res.ColorStateList;
import android.graphics.Color;
import android.graphics.Typeface;
import android.graphics.drawable.ColorDrawable;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.text.InputType;
import android.view.Gravity;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.RelativeLayout;
import android.widget.Spinner;
import android.widget.TextView;
import android.widget.Toast;
import androidx.appcompat.app.AppCompatActivity;
import com.google.android.material.button.MaterialButton;
import com.google.android.material.textfield.TextInputEditText;

import com.google.android.material.textfield.TextInputLayout;
import com.topjohnwu.superuser.Shell;
import org.lsposed.lsparanoid.Obfuscate;

@Obfuscate
public class LoginActivity extends AppCompatActivity {

    static {
        System.loadLibrary("yogs");
    }

    private static final String USER = "USER";
    private static final String PASS = "PASS";
    private FPrefs prefs;
    String gamename = "";
    
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        prefs = FPrefs.with(LoginActivity.this);
        
        /*if (!Shell.rootAccess()) {
            AlertDialog newdial = new AlertDialog.Builder(this, AlertDialog.THEME_DEVICE_DEFAULT_LIGHT)
                    .setCancelable(false)
                    .setMessage("ROOT REQUIRED!")
                    .setPositiveButton("OK", (dialog, which) -> {
                        finish(); // Tutup activity
                        System.exit(0); // Hentikan aplikasi
                    })
                    .create();
            
            newdial.show();
        }*/
        
        RelativeLayout basepage = new RelativeLayout(this);
        basepage.setLayoutParams(new RelativeLayout.LayoutParams(RelativeLayout.LayoutParams.MATCH_PARENT, RelativeLayout.LayoutParams.MATCH_PARENT));
        
        LinearLayout loginlyt = new LinearLayout(this);
        RelativeLayout.LayoutParams loginParams = new RelativeLayout.LayoutParams(RelativeLayout.LayoutParams.MATCH_PARENT, RelativeLayout.LayoutParams.WRAP_CONTENT);
        loginParams.addRule(RelativeLayout.CENTER_IN_PARENT);
        loginParams.setMargins(25, 0, 25, 0);
        loginlyt.setLayoutParams(loginParams);
        loginlyt.setGravity(Gravity.CENTER);
        loginlyt.setPadding(30, 30, 30, 30);
        loginlyt.setOrientation(LinearLayout.VERTICAL);
        loginlyt.setBackground(getDrawable(R.drawable.bg_rounded));
        
        TextView loginTitle = new TextView(this);
        LinearLayout.LayoutParams titleparams = new LinearLayout.LayoutParams(LinearLayout.LayoutParams.MATCH_PARENT, LinearLayout.LayoutParams.WRAP_CONTENT);
        titleparams.setMargins(0, 0, 0, 20);
        loginTitle.setLayoutParams(titleparams);
        loginTitle.setTextSize(18);
        loginTitle.setText("Login Crendentials");
        loginTitle.setTextColor(Color.WHITE);
        loginTitle.setTypeface(null, Typeface.BOLD);
        loginTitle.setGravity(Gravity.CENTER);
        
        loginlyt.addView(loginTitle);
        
        TextInputLayout PasswordLayout = new TextInputLayout(this);
        PasswordLayout.setLayoutParams(new LinearLayout.LayoutParams(LinearLayout.LayoutParams.MATCH_PARENT, LinearLayout.LayoutParams.WRAP_CONTENT));
        PasswordLayout.setHint("License");
        PasswordLayout.setBoxStrokeColor(Color.parseColor("#2E8B57"));
        PasswordLayout.setBoxBackgroundColor(Color.WHITE);
        
        TextInputEditText textPassword = new TextInputEditText(this);
        textPassword.setLayoutParams(new LinearLayout.LayoutParams(LinearLayout.LayoutParams.MATCH_PARENT, LinearLayout.LayoutParams.WRAP_CONTENT));
        textPassword.setTextColor(Color.WHITE);
        textPassword.setInputType(InputType.TYPE_CLASS_TEXT | InputType.TYPE_TEXT_VARIATION_PASSWORD);
        
        PasswordLayout.addView(textPassword);
        loginlyt.addView(PasswordLayout);
        
        TextView textgame = new TextView(this);
        textgame.setLayoutParams(new LinearLayout.LayoutParams(LinearLayout.LayoutParams.MATCH_PARENT, LinearLayout.LayoutParams.WRAP_CONTENT));
        textgame.setText("Select Game");
        textgame.setTextColor(Color.WHITE);
        textgame.setTextSize(16);
        loginlyt.addView(textgame);
        
        Spinner gameSelection = new Spinner(this);
        gameSelection.setLayoutParams(new LinearLayout.LayoutParams(LinearLayout.LayoutParams.MATCH_PARENT, LinearLayout.LayoutParams.WRAP_CONTENT));
        gameSelection.setBackgroundColor(Color.TRANSPARENT);
        gameSelection.setGravity(Gravity.CENTER);
        gameSelection.setPadding(0, 5, 0, 15);
        String[] items = {"PUBG", "Free Fire", "Mobile Legends"};
        ArrayAdapter<String> adapter = new ArrayAdapter<String>(this, android.R.layout.simple_spinner_item, items) {

            @Override
            public View getView(int position, View convertView, ViewGroup parent) {
                TextView view = (TextView) super.getView(position, convertView, parent);
        
                // Style item yang tampil (selected)
                view.setTextSize(18); // BESARIN TEXT
                view.setTextColor(Color.WHITE);
                view.setGravity(Gravity.LEFT);
                view.setPadding(10, 10, 10, 10);
        
                return view;
            }
        
            @Override
            public View getDropDownView(int position, View convertView, ViewGroup parent) {
                TextView view = (TextView) super.getDropDownView(position, convertView, parent);
        
                // Style dropdown list
                view.setTextSize(18); // lebih besar lagi
                view.setTextColor(Color.WHITE);
                view.setPadding(20, 20, 20, 20);
        
                // Tinggi item dropdown (optional)
                view.setMinHeight(100);
        
                return view;
            }
        };
        gameSelection.setAdapter(adapter);
        gameSelection.setOnItemSelectedListener(new AdapterView.OnItemSelectedListener() {
            @Override
            public void onItemSelected(AdapterView<?> parent, View view, int position, long id) {
                String selectedText = parent.getItemAtPosition(position).toString();
                if (selectedText.equals("PUBG")) {
                    gamename = "PUBG";
                    MainActivity.gamepackage = "com.tencent.ig";
                } else if (selectedText.equals("Free Fire")) {
                    gamename = "FFMAX";
                    MainActivity.gamepackage = "com.dts.freefiremax";
                } else if (selectedText.equals("Mobile Legends")) {
                    gamename = "MLBB";
                    MainActivity.gamepackage = "com.mobile.legends";
                }
            }
        
            @Override
            public void onNothingSelected(AdapterView<?> parent) {}
        });
        loginlyt.addView(gameSelection);
        
        MaterialButton loginBtn = new MaterialButton(this);
        loginBtn.setLayoutParams(new LinearLayout.LayoutParams(LinearLayout.LayoutParams.MATCH_PARENT, 95));
        loginBtn.setGravity(Gravity.CENTER);
        loginBtn.setTextSize(14);
        loginBtn.setCornerRadius(10);
        loginBtn.setBackgroundTintList(ColorStateList.valueOf(Color.parseColor("#2E8B57")));
        loginBtn.setTextColor(Color.WHITE);
        loginBtn.setText("LOGIN");
        loginBtn.setTypeface(null, Typeface.BOLD);
        
        loginlyt.addView(loginBtn);
        basepage.addView(loginlyt);
        
        setContentView(basepage);
        
        textPassword.setText(prefs.read(PASS, ""));

        loginBtn.setOnClickListener(view -> {
            if (!textPassword.getText().toString().isEmpty()) {
                prefs.write(PASS, textPassword.getText().toString());

                String passKey = textPassword.getText().toString().trim();

                Login(LoginActivity.this, passKey, gamename);
            } else if (textPassword.getText().toString().isEmpty()) {
                Toast.makeText(LoginActivity.this, "Please enter license!", Toast.LENGTH_SHORT).show();
            }
        });
    }

    private void Login(final Context m_Context, final String userKey, final String game) {
        AppFunction.ShowLoadingAnimation(m_Context);

        final Handler loginHandler = new Handler() {
            @SuppressLint("HandlerLeak")
            @Override
            public void handleMessage(Message msg) {
                if (msg.what == 0) {
                    try {
                        Intent intent = new Intent(m_Context, MainActivity.class);
                        m_Context.startActivity(intent);
                    } catch (Exception e) {
                        e.printStackTrace();
                    }
                    Toast.makeText(m_Context, "Login Success", Toast.LENGTH_SHORT).show();
                    finish();
                } else if (msg.what == 1) {
                    AlertDialog.Builder builder = new AlertDialog.Builder(m_Context, 5);
                    builder.setTitle("ERROR");
                    builder.setMessage(msg.obj.toString());
                    builder.setCancelable(false);
                    builder.setPositiveButton("OK", (dialog, which) -> {});
                    builder.show();
                }
                AppFunction.HideLoadingAnimation(m_Context);
            }
        };

        new Thread(() -> {
            String result = native_Check(m_Context, userKey, game);
            if ("OK".equals(result)) {
                loginHandler.sendEmptyMessage(0);
            } else {
                Message msg = new Message();
                msg.what = 1;
                msg.obj = result;
                loginHandler.sendMessage(msg);
            }
        }).start();
    }

    private static native String native_Check(Context context, String userKey, String GameSelected);

    @Override
    protected void onDestroy() {
        super.onDestroy();
    }
}
