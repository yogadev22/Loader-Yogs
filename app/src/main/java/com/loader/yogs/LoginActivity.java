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
import android.net.Uri;
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
import com.loader.yogs.databinding.ActivityLoginBinding;

@Obfuscate
public class LoginActivity extends AppCompatActivity {

    static {
        System.loadLibrary("yogs");
    }
    public static void goLogin(Context context) {
        Intent i = new Intent(context, LoginActivity.class);
        i.setFlags(Intent.FLAG_ACTIVITY_CLEAR_TASK | Intent.FLAG_ACTIVITY_NEW_TASK);
        context.startActivity(i);
    }
    
    private ActivityLoginBinding binding;

    private static final String PASS = "PASS";
    private FPrefs prefs;
    
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        binding = ActivityLoginBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());

        prefs = FPrefs.with(LoginActivity.this);
        
        binding.textPassword.setText(prefs.read(PASS, ""));
        
        binding.loginBtn.setOnClickListener(view -> {
            if (!binding.textPassword.getText().toString().isEmpty()) {
                prefs.write(PASS, binding.textPassword.getText().toString());

                String passKey = binding.textPassword.getText().toString().trim();

                Login(LoginActivity.this, passKey);
            } else if (binding.textPassword.getText().toString().isEmpty()) {
                Toast.makeText(LoginActivity.this, "Please enter key!", Toast.LENGTH_SHORT).show();
            }
        });

        binding.getkey.setOnClickListener(view -> {
            Intent intent = new Intent(Intent.ACTION_VIEW);
            intent.setData(Uri.parse("https://yogspanel.indevs.in/keys/getkey?admin=YOGS"));
            startActivity(intent);
        });
    }

    private void Login(final LoginActivity m_Context, final String userKey) {
        AppFunction.ShowLoadingAnimation(m_Context);

        final Handler loginHandler = new Handler() {
            @SuppressLint("HandlerLeak")
            @Override
            public void handleMessage(Message msg) {
                if (msg.what == 0) {
                    MainActivity.goMain(m_Context);
                    Toast.makeText(m_Context, "Login Success", Toast.LENGTH_SHORT).show();
                    m_Context.finishActivity(0);
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
            String result = native_Check(m_Context, userKey);
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
    private static native String native_Check(Context context, String userKey);

    @Override
    protected void onDestroy() {
        super.onDestroy();
        binding = null;
    }
}
