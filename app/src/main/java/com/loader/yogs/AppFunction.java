package com.loader.yogs;

import android.app.AlertDialog;
import android.content.Context;
import android.graphics.Color;
import android.graphics.drawable.ColorDrawable;
import android.view.LayoutInflater;
import android.view.View;

public class AppFunction {

    static AlertDialog dialogloading = null;

    public static void ShowLoadingAnimation(Context ctx) {
        LayoutInflater inflater = LayoutInflater.from(ctx);
        View viewloading = inflater.inflate(R.layout.loading_animation, null);
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
