package com.loader.yogs;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Paint.Align;
import android.graphics.Paint.Style;
import android.graphics.PorterDuff;
import android.graphics.Typeface;
import android.os.SystemClock;
import android.util.Log;
import android.view.View;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.Locale;
import android.os.Build;
import android.view.Surface;
import android.graphics.drawable.GradientDrawable;
import android.graphics.Rect;
import java.util.Random;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Matrix;

import org.lsposed.lsparanoid.Obfuscate;

@Obfuscate
public class ESPView extends View implements Runnable {

    private Paint boxFillPaint,
	mTextPaint,
	mPaintBitmap,
	mPaintBitmap1,
	linePaint,
	boxPaint,
    mStrokePaint;
    private Bitmap botBitmap, lootBitmap, airdropBitmap, vehicleBitmap, boatBitmap;
    private Thread mThread;
    static long sleepTime;
    private Date time;
    private SimpleDateFormat formatter;
    private int mFPS = 0;
    private int itemCount = 2;
    private int mFPSCounter = 0;
    private long mFPSTime = 0;
    private boolean isAr;
	Paint p;

    private float mScaleX = 1;
    private float mScaleY = 1;

    private String[] TeamColors = {
		"#00ffff",
		"#ffa3ff",
		"#b3b9ff",
		"#ffc96b",
		"#a4ff73"
    };
	

    private boolean mHardwareAccelerated = false;

    public static void ChangeFps(int fps) {
        sleepTime = 1000 / (15 + fps);
    }

	public void ResetItemCount() {
		itemCount = 2;
	}

    public ESPView(Context context) {
        super(context, null, 0);
        InitializePaints();
        setFocusableInTouchMode(false);
        setBackgroundColor(Color.TRANSPARENT);
        time = new Date();
        formatter = new SimpleDateFormat("HH:mm:ss", Locale.getDefault());
        sleepTime = 1000 / 60;
        mThread = new Thread(this);
        mThread.start();

        // Enable hardware acceleration if available
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.HONEYCOMB) {
            setLayerType(View.LAYER_TYPE_HARDWARE, null);
            mHardwareAccelerated = true;
        }
    }

    @Override
    protected void onDraw(Canvas canvas) {
		int rotation = getDisplay().getRotation();
        if (canvas == null || rotation == Surface.ROTATION_0 || rotation == Surface.ROTATION_180) {
            return;
        }
        
        if (!mHardwareAccelerated) {
            ClearCanvas(canvas);
        }

        time.setTime(System.currentTimeMillis());    
        Overlay.DrawOn(this, canvas);
    }

    @Override
    public void run() {
        android.os.Process.setThreadPriority(android.os.Process.THREAD_PRIORITY_FOREGROUND);
        while (mThread.isAlive() && !mThread.isInterrupted()) {
            try {
                long t1 = System.currentTimeMillis();
                postInvalidate();
                long td = System.currentTimeMillis() - t1;
                Thread.sleep(Math.max(Math.min(0, sleepTime - td), sleepTime));
            } catch (InterruptedException it) {
                Log.e("OverlayThread", it.getMessage());
            }
        }
    }

	@Override
	protected void onLayout(boolean changed, int left, int top, int right, int bottom) {
		super.onLayout(changed, left, top, right, bottom);
		mScaleX = getWidth() / (float) 2340;
        mScaleY = getHeight() / (float) 1080;
		botBitmap = Bitmap.createScaledBitmap(BitmapFactory.decodeResource(getResources(), R.drawable.bot), (int) (42 * mScaleY), (int) (30 * mScaleY), false);
        lootBitmap = Bitmap.createScaledBitmap(BitmapFactory.decodeResource(getResources(), R.drawable.lootx), (int) (40 * mScaleY), (int) (40 * mScaleY), false);
        airdropBitmap = Bitmap.createScaledBitmap(BitmapFactory.decodeResource(getResources(), R.drawable.airdrop), (int) (40 * mScaleY), (int) (40 * mScaleY), false);
        vehicleBitmap = Bitmap.createScaledBitmap(BitmapFactory.decodeResource(getResources(), R.drawable.vehicle), (int) (42 * mScaleY) , (int) (42 * mScaleY), false);
        boatBitmap = Bitmap.createScaledBitmap(BitmapFactory.decodeResource(getResources(), R.drawable.boat), (int) (42 * mScaleY), (int) (42 * mScaleY), false);
	}

    public void InitializePaints() {
        mStrokePaint = new Paint();
        mStrokePaint.setStyle(Style.STROKE);
        mStrokePaint.setAntiAlias(true);
        mStrokePaint.setColor(Color.rgb(0, 0, 0));
		mStrokePaint.setTextAlign(Align.CENTER);
		
		botBitmap = Bitmap.createScaledBitmap(BitmapFactory.decodeResource(getResources(), R.drawable.bot), (int) (42 * mScaleY), (int) (30 * mScaleY), false);
        lootBitmap = Bitmap.createScaledBitmap(BitmapFactory.decodeResource(getResources(), R.drawable.lootx), (int) (40 * mScaleY), (int) (40 * mScaleY), false);
        airdropBitmap = Bitmap.createScaledBitmap(BitmapFactory.decodeResource(getResources(), R.drawable.airdrop), (int) (40 * mScaleY), (int) (40 * mScaleY), false);
        vehicleBitmap = Bitmap.createScaledBitmap(BitmapFactory.decodeResource(getResources(), R.drawable.vehicle), (int) (42 * mScaleY) , (int) (42 * mScaleY), false);
        boatBitmap = Bitmap.createScaledBitmap(BitmapFactory.decodeResource(getResources(), R.drawable.boat), (int) (42 * mScaleY), (int) (42 * mScaleY), false);
		
        mPaintBitmap = new Paint();
        mPaintBitmap.setAlpha(180);

        mPaintBitmap1 = new Paint();
        mPaintBitmap1.setAlpha(120);

        linePaint = new Paint();
        linePaint.setAntiAlias(true);
		linePaint.setAlpha(200);
        linePaint.setStyle(Style.STROKE);

        boxPaint = new Paint();
        boxPaint.setAntiAlias(true);
        boxPaint.setStyle(Style.STROKE);

        boxFillPaint = new Paint();
        boxFillPaint.setAntiAlias(true);
        boxFillPaint.setStyle(Style.FILL);

        mTextPaint = new Paint();
        mTextPaint.setStyle(Style.FILL_AND_STROKE);
	    mTextPaint.setAntiAlias(true);
        mTextPaint.setColor(Color.rgb(0, 0, 0));
		mStrokePaint.setStrokeWidth(0.5f);
        mTextPaint.setTextAlign(Align.CENTER);
        
		p = new Paint();
        final int bitmap_count_oth = OTHER.length;
        for (int i = 0 ; i < bitmap_count_oth ; i++) {
            OTHER[i] = BitmapFactory.decodeResource(getResources(), OTH_NAME[i]);
            if (i == 4) {
                OTHER[i] = scale(OTHER[i], 400, 35);
            } else if (i == 5) {
				OTHER[i] = scale(OTHER[i], 22, 22);
            } else {
                OTHER[i] = scale(OTHER[i], 80, 80);
            }
        }
    }

    public void ClearCanvas(Canvas cvs) {
        cvs.drawColor(Color.TRANSPARENT, PorterDuff.Mode.CLEAR);
    }

    public void DrawLine(Canvas cvs, int a, int r, int g, int b, float lineWidth, float fromX, float fromY, float toX, float toY) {
        linePaint.setStrokeWidth(lineWidth);
        linePaint.setARGB(a, r, g, b);
        cvs.drawLine(fromX, fromY, toX, toY, linePaint);
    }

    public void DrawRect(Canvas cvs, int a, int r, int g, int b, float stroke, float x, float y, float width, float height) {
        boxPaint.setStrokeWidth(stroke);
        boxPaint.setARGB(a, r, g, b);
        cvs.drawRect(x, y, width, height, boxPaint);
    }
	
    public void DrawFilledRect(Canvas cvs, int a, int r, int g, int b, float x, float y, float width, float height) {
        boxFillPaint.setARGB(a, r, g, b);
        cvs.drawRect(x, y, width, height, boxFillPaint);
    }

    public void DrawText(Canvas cvs, int a, int r, int g, int b, String txt, float posX, float posY, float size) {
        mTextPaint.setARGB(a, r, g, b);
        mTextPaint.setTextSize(size);
        cvs.drawText(txt, posX, posY, mTextPaint);
    }

    public void DrawCircle(Canvas cvs, int a, int r, int g, int b, float posX, float posY, float radius, float strokeZ) {
        linePaint.setStrokeWidth(strokeZ);
        linePaint.setColor(Color.rgb(r, g, b));
        linePaint.setAlpha(a);
        cvs.drawCircle(posX, posY, radius, linePaint);
    }

    public void DrawFilledCircle(Canvas cvs, int a, int r, int g, int b, float posX, float posY, float radius) {
        boxFillPaint.setColor(Color.rgb(r, g, b));
        boxFillPaint.setAlpha(a);
        cvs.drawCircle(posX, posY, radius, boxFillPaint);
    }
	
	public void DrawOTH(Canvas cvs, int image_number, float X, float Y) {

        cvs.drawBitmap(OTHER[image_number], X, Y, p);

    }

    Bitmap[] OTHER = new Bitmap[6];

    private static final int[] OTH_NAME = {
        R.drawable.ic_clear_pro,
        R.drawable.ic_clear_noob,
        R.drawable.ic_warn_pro,
        R.drawable.ic_warn_noob,
        R.drawable.ic_warning,
        R.drawable.ic_boot
    };
	
	public static Bitmap scale(Bitmap bitmap, int maxWidth, int maxHeight) {
        // Determine the constrained dimension, which determines both dimensions.
        int width;
        int height;
        float widthRatio = (float)bitmap.getWidth() / maxWidth;
        float heightRatio = (float)bitmap.getHeight() / maxHeight;
        // Width constrained.
        if (widthRatio >= heightRatio) {
            width = maxWidth;
            height = (int)(((float)width / bitmap.getWidth()) * bitmap.getHeight());
        } else {
            height = maxHeight;
            width = (int)(((float)height / bitmap.getHeight()) * bitmap.getWidth());
        }
        Bitmap scaledBitmap = Bitmap.createBitmap(width, height, Bitmap.Config.ARGB_8888);

        float ratioX = (float)width / bitmap.getWidth();
        float ratioY = (float)height / bitmap.getHeight();
        float middleX = width / 2.0f;
        float middleY = height / 2.0f;
        Matrix scaleMatrix = new Matrix();
        scaleMatrix.setScale(ratioX, ratioY, middleX, middleY);

        Canvas canvas = new Canvas(scaledBitmap);
        canvas.setMatrix(scaleMatrix);
        canvas.drawBitmap(bitmap, middleX - bitmap.getWidth() / 2, middleY - bitmap.getHeight() / 2, new Paint(Paint.FILTER_BITMAP_FLAG));
        return scaledBitmap;
    }
}