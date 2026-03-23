package com.loader.yogs;

import android.annotation.SuppressLint;
import android.app.AlertDialog;
import android.content.Context;
import android.content.Intent;
import android.graphics.Color;
import android.graphics.drawable.ColorDrawable;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.view.LayoutInflater;
import android.view.View;
import android.widget.TextView;
import android.widget.Toast;
import androidx.appcompat.app.AppCompatActivity;
import com.google.android.material.button.MaterialButton;
import com.google.android.material.textfield.TextInputEditText;
import com.loader.yogs.databinding.ActivityLoginBinding;

public class LoginActivity extends AppCompatActivity {

    static {
        System.loadLibrary("yogs");
    }

    private ActivityLoginBinding binding;

    private static final String USER = "USER";
    private static final String PASS = "PASS";
    private FPrefs prefs;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        binding = ActivityLoginBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());

        prefs = FPrefs.with(LoginActivity.this);

        binding.textUsername.setText(prefs.read(USER, ""));
        binding.textPassword.setText(prefs.read(PASS, ""));

        binding.loginBtn.setOnClickListener(view -> {
            if (!binding.textUsername.getText().toString().isEmpty() && !binding.textPassword.getText().toString().isEmpty()) {
                prefs.write(USER, binding.textUsername.getText().toString());
                prefs.write(PASS, binding.textPassword.getText().toString());

                String userKey = binding.textUsername.getText().toString().trim();
                String passKey = binding.textPassword.getText().toString().trim();

                String combinedKey = userKey + ":" + passKey;

                Login(LoginActivity.this, combinedKey);
            } else if (binding.textUsername.getText().toString().isEmpty()) {
                Toast.makeText(LoginActivity.this, "Please enter username!", Toast.LENGTH_SHORT).show();
            } else if (binding.textPassword.getText().toString().isEmpty()) {
                Toast.makeText(LoginActivity.this, "Please enter password!", Toast.LENGTH_SHORT).show();
            } else {
                Toast.makeText(LoginActivity.this, "Please enter username and password!", Toast.LENGTH_SHORT).show();
            }
        });
    }

    private void Login(final Context m_Context, final String userKey) {
        AppFunction.ShowLoadingAnimation(m_Context);

        final Handler loginHandler =
                new Handler() {
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
                            builder.setPositiveButton("OK", (dialog, which) -> {
                            });
                            builder.show();
                        }
                        AppFunction.HideLoadingAnimation(m_Context);
                    }
                };

        new Thread(() -> {
            String result = native_Check(m_Context, userKey);
            if ("yogaganteng11".equals(result)) {
                loginHandler.sendEmptyMessage(0);
            } else {
                Message msg = new Message();
                msg.what = 1;
                msg.obj = result;
                loginHandler.sendMessage(msg);
            }
        }).start();
    }

    private static native String native_Check(Context context, String userKey);

    @Override
    protected void onDestroy() {
        super.onDestroy();
        this.binding = null;
    }
}
