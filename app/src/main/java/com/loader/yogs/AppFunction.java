package com.loader.yogs;

import android.app.AlertDialog;
import android.content.Context;
import android.graphics.Color;
import android.graphics.drawable.ColorDrawable;
import android.view.Gravity;
import android.view.LayoutInflater;
import android.view.View;
import android.widget.FrameLayout;
import android.widget.ImageView;
import android.widget.LinearLayout;
import com.airbnb.lottie.LottieAnimationView;
import com.airbnb.lottie.LottieDrawable;
import java.io.IOException;

public class AppFunction {

    static AlertDialog dialogloading = null;

    public static void ShowLoadingAnimation(Context ctx) {
        FrameLayout viewloading = new FrameLayout(ctx);
        viewloading.setLayoutParams(new LinearLayout.LayoutParams(LinearLayout.LayoutParams.MATCH_PARENT, LinearLayout.LayoutParams.MATCH_PARENT));
        viewloading.setBackgroundColor(Color.TRANSPARENT);
        
        LinearLayout loadinglyt = new LinearLayout(ctx);

        FrameLayout.LayoutParams anjay = new FrameLayout.LayoutParams(150, 150);
        anjay.gravity = Gravity.CENTER;
        loadinglyt.setLayoutParams(anjay);
        loadinglyt.setBackgroundColor(Color.TRANSPARENT);
        loadinglyt.setGravity(Gravity.CENTER);
        loadinglyt.setPadding(24, 24, 24, 24);
        loadinglyt.setOrientation(LinearLayout.VERTICAL);
        
        LottieAnimationView loadingAnim = new LottieAnimationView(ctx);
        loadingAnim.setLayoutParams(new LinearLayout.LayoutParams(LinearLayout.LayoutParams.WRAP_CONTENT, LinearLayout.LayoutParams.WRAP_CONTENT));
        loadingAnim.setScaleType(ImageView.ScaleType.CENTER_INSIDE);
        loadingAnim.setAnimation(R.raw.loginanimation);
        loadingAnim.setSpeed(1.0f);
        loadingAnim.playAnimation();
        loadingAnim.setRepeatCount(LottieDrawable.INFINITE);
        
        loadinglyt.addView(loadingAnim);
        viewloading.addView(loadinglyt);
        
        dialogloading =
                new AlertDialog.Builder(ctx, 5)
                        .setView(viewloading)
                        .setCancelable(false)
                        .create();
        dialogloading.getWindow().setBackgroundDrawable(new ColorDrawable(Color.TRANSPARENT));
        dialogloading.show();
    }

    public static void HideLoadingAnimation(Context ctx) {
        dialogloading.dismiss();
    }
}
