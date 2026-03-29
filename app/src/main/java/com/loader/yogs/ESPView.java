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
	DistancePaint,
	mTextPaint,
	mPaintBitmap,
	mPaintBitmap1,
	mNamePaint,
	mTeamPaint,
	weaponPaint,
	linePaint,
	boxPaint,
	mItemsPaint,
	mVehiclesPaint;
	Paint mStrokePaint;
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
    
    Typeface typeface;

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

        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
            // Safe to call getFont() on API 26+
            typeface = getResources().getFont(R.font.mfontx);
        }
    }

    @Override
    protected void onDraw(Canvas canvas) {
		int rotation = getDisplay().getRotation();
        if (canvas == null || rotation == Surface.ROTATION_0 || rotation == Surface.ROTATION_180) {
            return;
        }
        if (!mHardwareAccelerated) {
            // Clear the canvas only if hardware acceleration is not enabled
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
        // =======================================================
        // Credit Text
        // =======================================================
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

        // =======================================================
        // Enemy Box
        // =======================================================
        boxPaint = new Paint();
        boxPaint.setAntiAlias(true);
        boxPaint.setStyle(Style.STROKE);

        boxFillPaint = new Paint();
        boxFillPaint.setAntiAlias(true);
        boxFillPaint.setStyle(Style.FILL);

        // =======================================================
        mVehiclesPaint = new Paint();
        mVehiclesPaint.setAntiAlias(true);
        mVehiclesPaint.setTextAlign(Align.CENTER);
		int shadowColor5 = Color.argb(200, 0, 0, 0);
        mVehiclesPaint.setShadowLayer(7, 0, 0, shadowColor5);
        mVehiclesPaint.setColor(Color.rgb(255, 168, 207));
        mVehiclesPaint.setTypeface(typeface);

        // =======================================================
        mItemsPaint = new Paint();
        mItemsPaint.setAntiAlias(true);
		int shadowColor4 = Color.argb(200, 0, 0, 0);
        mItemsPaint.setShadowLayer(7, 0, 0, shadowColor4);
		mItemsPaint.setColor(Color.rgb(255, 168, 207));
        mItemsPaint.setTextAlign(Align.CENTER);
        mItemsPaint.setTypeface(typeface);

        // =======================================================
        mTextPaint = new Paint();
        mTextPaint.setStyle(Style.FILL_AND_STROKE);
	    mTextPaint.setAntiAlias(true);
        mTextPaint.setColor(Color.rgb(0, 0, 0));
		mStrokePaint.setStrokeWidth(0.5f);
        mTextPaint.setTextAlign(Align.CENTER);

        // =======================================================
        mNamePaint = new Paint();
        mNamePaint.setAntiAlias(true);
		int shadowColor3 = Color.argb(200, 0, 0, 0);
        mNamePaint.setShadowLayer(7, 0, 0, shadowColor3);
        mNamePaint.setTextAlign(Align.CENTER);
		mNamePaint.setColor(Color.WHITE);
		mNamePaint.setTextSize(mScaleY * 30);
		mNamePaint.setAlpha(200);
        mNamePaint.setTypeface(Typeface.create(typeface, Typeface.NORMAL));
		
		mTeamPaint = new Paint();
        mTeamPaint.setAntiAlias(true);
		int shadowColor6 = Color.argb(200, 0, 0, 0);
        mTeamPaint.setShadowLayer(7, 0, 0, shadowColor6);
        mTeamPaint.setTextAlign(Align.CENTER);
		mTeamPaint.setTextSize(mScaleY * 31);
		mTeamPaint.setAlpha(200);
        mTeamPaint.setTypeface(Typeface.create(typeface, Typeface.BOLD));

        // =======================================================
        DistancePaint = new Paint();
        DistancePaint.setAntiAlias(true);
		int shadowColor = Color.argb(200, 0, 0, 0); // 128 is the alpha value (0-255)
        DistancePaint.setShadowLayer(7, 0, 0, shadowColor);
        DistancePaint.setTextAlign(Align.CENTER);
        DistancePaint.setColor(Color.rgb(255, 175, 20));
		DistancePaint.setTextSize(mScaleY * 30);
		DistancePaint.setAlpha(208);
        DistancePaint.setTypeface(Typeface.create(typeface, Typeface.BOLD));

        // =======================================================

        weaponPaint = new Paint();
        weaponPaint.setAntiAlias(true);
        weaponPaint.setTextAlign(Align.CENTER);
		int shadowColor1 = Color.argb(200, 0, 0, 0);
        weaponPaint.setShadowLayer(7, 0, 0, shadowColor1);
        weaponPaint.setColor(Color.rgb(255, 175, 20));
		weaponPaint.setAlpha(208);
		weaponPaint.setTextSize(mScaleY * 29);
        weaponPaint.setTypeface(Typeface.create(typeface, Typeface.BOLD));
		
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
	
	public void DrawRect2(Canvas cvs, int a, int r, int g, int b, float stroke, float x, float y, float width, float height) {
        boxPaint.setStrokeWidth(stroke);
        boxPaint.setColor(Color.rgb(r, g, b));
        boxPaint.setAlpha(a);
        cvs.drawRect(x, y, width, height, boxPaint);
    }

    public void DrawFilledRect(Canvas cvs, int a, int r, int g, int b, float x, float y, float width, float height) {
        boxFillPaint.setARGB(a, r, g, b);
        cvs.drawRect(x, y, width, height, boxFillPaint);
    }
	
	public void DrawFilledRect2(Canvas cvs, int a, int r, int g, int b, float x, float y, float width, float height) {
        boxFillPaint.setColor(Color.rgb(r, g, b));
        boxFillPaint.setAlpha(a);
        cvs.drawRect(x, y, width, height, boxFillPaint);
    }

    public void DebugText(String s) {
        System.out.println(s);
    }

    public void DrawText(Canvas cvs, int a, int r, int g, int b, String txt, float posX, float posY, float size) {
        mTextPaint.setARGB(a, r, g, b);
        mTextPaint.setTextSize(size);
        cvs.drawText(txt, posX, posY, mTextPaint);
    }

    public void DrawText1(Canvas cvs, int a, int r, int g, int b, String txt, float posX, float posY, float size) {
        mTextPaint.setARGB(a, r, g, b);
        mTextPaint.setShadowLayer(7, 0, 0, Color.BLACK);

        if (getRight() > 1920 || getBottom() > 1920)
            mTextPaint.setTextSize(4 + size);
        else if (getRight() == 1920 || getBottom() == 1920)
            mTextPaint.setTextSize(2 + size);
        else
            mTextPaint.setTextSize(size);

        cvs.drawText(txt, posX, posY, mTextPaint);
    }

    public void DrawWeapon(Canvas cvs, int a, int r, int g, int b, int id, int ammo, int maxammo, float posX, float posY, float size) {
        String wname = getWeapon(id);
        if (wname != null) {
			weaponPaint.setARGB(a, r, g, b);
			weaponPaint.setTextSize(size);
            cvs.drawText(wname, posX, posY, weaponPaint);
        }
    }

    public void DrawTextName(Canvas cvs, int a, int r, int g, int b, float posX, float posY, float size, boolean isInGame) {
        mTextPaint.setARGB(a, r, g, b);
        mTextPaint.setTextSize(size);
		mTextPaint.setShadowLayer(0, 0, 0, Color.TRANSPARENT);
        if (SystemClock.uptimeMillis() - mFPSTime > 1000) {
            mFPSTime = SystemClock.uptimeMillis();
            mFPS = mFPSCounter;
            mFPSCounter = 0;
        } else {
            mFPSCounter++;
        }
        cvs.drawText("", posX, posY, mTextPaint);
    }

    public void DrawDistance(Canvas cvs, float distance, float posX, float posY, float size) {
        cvs.drawText(String.valueOf((int) distance + "m"), posX, posY, DistancePaint);
    }

    public void DrawName(Canvas cvs, int a, int r, int g, int b, String nametxt, int teamid, float posX, float posY, float size) {
		// decode nama
		String[] namesp = nametxt.split(":");
		char[] nameint = new char[namesp.length];
		for (int i = 0; i < namesp.length; i++)
			nameint[i] = (char) Integer.parseInt(namesp[i]);
		String realname = new String(nameint);
		String teamidi = String.valueOf(teamid);

		// atur ukuran teks
		mTextPaint.setTextSize(size);
		mTextPaint.setARGB(a, r, g, b);
		mTextPaint.setTypeface(Typeface.create(typeface, Typeface.BOLD));
		cvs.drawText(teamidi + " " + realname, posX, posY, mTextPaint);
	}

    public void DrawEnemyCount(Canvas cvs, int a, int r, int g, int b, int x, int y, int width, int height) {
        int colors[] = { Color.TRANSPARENT, Color.rgb(r, g, b), Color.TRANSPARENT };
        GradientDrawable mDrawable = new GradientDrawable(GradientDrawable.Orientation.RIGHT_LEFT, colors);
        mDrawable.setShape(GradientDrawable.RECTANGLE);
        mDrawable.setGradientRadius(2.0f * 60);
        Rect mRect = new Rect(x, y, width, height);
        mDrawable.setBounds(mRect);
        cvs.save();
        mDrawable.setGradientType(GradientDrawable.LINEAR_GRADIENT);
        mDrawable.draw(cvs);
        cvs.restore();
    }

    public void DrawItems(Canvas cvs, String itemName, float distance, float posX, float posY) {
        /*sAr = false;
        String realItemName = getItemName(itemName);
        if (realItemName != null && !realItemName.equals("")) {
            mItemsPaint.setTextSize(mScaleY * 25);
            if (realItemName.equals("Loot")) {
                if (distance < 150) {
                    cvs.drawBitmap(lootBitmap, posX - 25, posY - (54 * mScaleY), mPaintBitmap1);
                    cvs.drawText(realItemName + " (" + (int) distance + ")", posX, posY - 8, mItemsPaint);
                }
            }
            else if (realItemName.equals("DropPlane")) {
                cvs.drawText(realItemName + " (" + (int) distance + ")", posX, posY - 8, mItemsPaint);
            }
            else if (realItemName.equals("AirDrop")) {
                cvs.drawBitmap(airdropBitmap, posX - 25, posY - (54 * mScaleY), mPaintBitmap1);
                cvs.drawText(realItemName + " (" + (int) distance + ")", posX, posY - 8, mItemsPaint);
            }
            else {
                mItemsPaint.setARGB(135, 44, 234, 115);
                mItemsPaint.setShadowLayer(3, 0, 0, Color.TRANSPARENT);
                cvs.drawCircle(posX, posY, 7, mItemsPaint);
                mItemsPaint.setColor(Color.rgb(255, 168, 207));
                mItemsPaint.setShadowLayer(6, 0, 0, Color.BLACK);
                cvs.drawText(realItemName + " (" + (int) distance + ")", posX, posY - 8, mItemsPaint);
            }
        }*/
    }

    public void DrawListItem(Canvas cvs, int a, int r, int g, int b, int itemID, int count, float posX, float posY) {
        /*String realItemName = getItemName(itemID);
        mTextPaint.setARGB(a, r, g, b);
        mTextPaint.setTextSize(24);
        if (realItemName != null && !realItemName.equals("")) {
            itemCount = itemCount + 1;
            if (count == 1) {
                cvs.drawText(realItemName, posX, posY - itemCount * 24, mTextPaint);
            } else {
                cvs.drawText(realItemName + " • " + count, posX, posY - itemCount * 24, mTextPaint);
            }
        }*/
    }

    public void DrawVehicles(Canvas cvs, String itemName, float distance, float health, float fuel, float posX, float posY) {
        /*String realVehicleName = getVehicleName(itemName);
        mVehiclesPaint.setTextSize(mScaleY * 26);
        if (realVehicleName != null && !realVehicleName.equals("")) {
            if (realVehicleName.equals("Boat")) {
                cvs.drawBitmap(boatBitmap, posX - 25, posY - (56 * mScaleY), mPaintBitmap1);
                cvs.drawText(realVehicleName + " (" + (int) distance + ")", posX, posY - 8, mVehiclesPaint);
            }
            else if (realVehicleName.equals("AquaRail")) {
                cvs.drawBitmap(boatBitmap, posX - 25, posY - (56 * mScaleY), mPaintBitmap1);
                cvs.drawText(realVehicleName + " (" + (int) distance + ")", posX, posY - 8, mVehiclesPaint);
            }
            else {
                cvs.drawBitmap(vehicleBitmap, posX - 25, posY - (56 * mScaleY), mPaintBitmap1);
                cvs.drawText(realVehicleName + " (" + (int) distance + ")", posX, posY - 8, mVehiclesPaint);
            }
        }*/
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

    private String getWeapon(int id) {
        // AR and SMG
        if (id == 101006)
            return "AUG";
        if (id == 101008)
            return "M762";
        if (id == 101003)
            return "SCARL";
        if (id == 101004)
            return "M416";
        if (id == 101002)
            return "M16A4";
        if (id == 101009)
            return "Mk47";
        if (id == 101010)
            return "G36C";
        if (id == 101007)
            return "QBZ";
        if (id == 101001)
            return "AKM";
        if (id == 101005)
            return "Groza";
        if (id == 102005)
            return "Bizon";
        if (id == 102004)
            return "TommyGun";
        if (id == 102007)
            return "MP5K";
        if (id == 102002)
            return "UMP45";
        if (id == 102003)
            return "Vector";
        if (id == 102001)
            return "Uzi";
        if (id == 105002)
            return "DP28";
        if (id == 105001)
            return "M249";

        // Snipers
        if (id == 103003)
            return "AWM";
        if (id == 103010)
            return "QBU";
        if (id == 103009)
            return "SLR";
        if (id == 103004)
            return "SKS";
        if (id == 103006)
            return "Mini14";
        if (id == 103002)
            return "M24";
        if (id == 103001)
            return "Kar98";
        if (id == 103005)
            return "VSS";
        if (id == 103008)
            return "Win94";
        if (id == 103007)
            return "Mk14";

        // Shotguns and Hand weapons
        if (id == 104003)
            return "S12K";
        if (id == 104004)
            return "DBS";
        if (id == 104001)
            return "S686";
        if (id == 104002)
            return "S1897";
        if (id == 108003)
            return "Sickle";
        if (id == 108001)
            return "Machete";
        if (id == 108002)
            return "Crowbar";
        if (id == 107001)
            return "CrossBow";
        if (id == 108004)
            return "Pan";

        // Pistols
        if (id == 106006)
            return "SawedOff";
        if (id == 106003)
            return "R1895";
        if (id == 106008)
            return "Vz61";
        if (id == 106001)
            return "P92";
        if (id == 106004)
            return "P18C";
        if (id == 106005)
            return "R45";
        if (id == 106002)
            return "P1911";
        if (id == 106010)
            return "DesertEagle";

        return null;
    }
	
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

