#include <jni.h>
#include <string>
#include "obfuscate.h"
const char* name = "";
#include "rLogin/Login.h"
#include "Hacks.h"

ESP espOverlay;
int type = 1, utype = 2;

void DrawOn(JNIEnv * env, jclass, jobject espView, jobject canvas) {
	espOverlay = ESP(env, espView, canvas);
	if (espOverlay.isValid()) {
		DrawESP(espOverlay, espOverlay.getWidth(), espOverlay.getHeight());
	}
}

void Closee(JNIEnv *, jobject) {
	Close();
}

jobjectArray GetFeatureList(JNIEnv *env, jobject context) {
    jobjectArray ret;
    
    if (strcmp(name, "FFMAX") == 0) {
        const char *features[] = {
            OBFUSCATE("TitleMenu_ESP FEATURES"),
            OBFUSCATE("0_Toggle_True_ESP Line_FFMAX"),
            OBFUSCATE("1_Toggle_True_ESP Box_FFMAX"),
            OBFUSCATE("2_Toggle_True_ESP Name_FFMAX"),
            OBFUSCATE("3_Toggle_True_ESP Distance_FFMAX"),
            OBFUSCATE("4_Toggle_True_ESP Health_FFMAX"),
            OBFUSCATE("5_Toggle_True_ESP 360 Alert_FFMAX"),
            OBFUSCATE("6_Toggle_True_ESP Skip Bot_FFMAX"),
            OBFUSCATE("7_RadioButton_True_Line Position_Top, Mid, Bottom_FFMAX"),
            OBFUSCATE("8_RadioButton_True_Box Style_Stroke, Filled_FFMAX"),
            OBFUSCATE("TitleMenu_AIM FEATURES"),
            OBFUSCATE("9_Toggle_Silent Aim_FFMAX"),
            OBFUSCATE("14_Toggle_Aim Collider_FFMAX"),
            OBFUSCATE("10_RadioButton_True_Target Position_Head, Neck, Chest_FFMAX"),
            OBFUSCATE("11_Seekbar_True_Aim Fov_500_FFMAX"),
            OBFUSCATE("12_Toggle_Aim Line_FFMAX"),
            OBFUSCATE("TitleMenu_MEMORY FEATURES"),
            OBFUSCATE("23_Toggle_Speed Hack_FFMAX"),
            OBFUSCATE("TitleMenu_OTHER FEATURES"),
            OBFUSCATE("21_Toggle_Reset Guest_FFMAX"),
            OBFUSCATE("22_Toggle_Bypass Lobby_FFMAX")
        };
        
        int Total_Feature = (sizeof features / sizeof features[0]);
        ret = (jobjectArray)
            env->NewObjectArray(Total_Feature, env->FindClass(OBFUSCATE("java/lang/String")),
                                env->NewStringUTF(""));

        for (int i = 0; i < Total_Feature; i++)
            env->SetObjectArrayElement(ret, i, env->NewStringUTF(features[i]));
    } else if (strcmp(name, "MLBB") == 0) {
        const char *features[] = {
            OBFUSCATE("TitleMenu_ESP FEATURES"),
            OBFUSCATE("0_Toggle_True_ESP Line_MLBB"),
            OBFUSCATE("1_Toggle_True_ESP Box_MLBB"),
            OBFUSCATE("2_Toggle_True_ESP Name_MLBB"),
            OBFUSCATE("3_Toggle_True_ESP Distance_MLBB"),
            OBFUSCATE("4_Toggle_True_ESP Health_MLBB"),
            OBFUSCATE("5_Toggle_True_ESP 360 Alert_MLBB"),
            OBFUSCATE("6_Toggle_True_ESP Skip Bot_MLBB"),
            OBFUSCATE("7_RadioButton_True_Line Position_Top, Mid, Bottom_MLBB"),
            OBFUSCATE("8_RadioButton_True_Box Style_Stroke, Filled_MLBB"),
            OBFUSCATE("TitleMenu_AIM FEATURES"),
            OBFUSCATE("15_Toggle_Auto Retri_MLBB"),
            OBFUSCATE("20_Toggle_Show Touch_MLBB"),
            OBFUSCATE("16_Seekbar_True_Touch Range_600_MLBB"),
            OBFUSCATE("17_Seekbar_True_Touch Pos Y_1000_MLBB"),
            OBFUSCATE("18_Seekbar_True_Touch Pos X_1700_MLBB"),
            OBFUSCATE("19_Toggle_True_Rotation Screen_MLBB")
        };
        
        int Total_Feature = (sizeof features / sizeof features[0]);
        ret = (jobjectArray)
            env->NewObjectArray(Total_Feature, env->FindClass(OBFUSCATE("java/lang/String")),
                                env->NewStringUTF(""));

        for (int i = 0; i < Total_Feature; i++)
            env->SetObjectArrayElement(ret, i, env->NewStringUTF(features[i]));
    } else {
        ret = env->NewObjectArray(0, env->FindClass("java/lang/String"), env->NewStringUTF(""));
    }
    
    return (ret);
}

void Changes(JNIEnv *env, jclass clazz, jobject obj, jint featNum, jstring featName, jint value, jlong Lvalue, jboolean boolean, jstring text) {
    switch (featNum) {
        case 0:
            isPlayerLinee = boolean;
            break;
        case 1:
            isPlayerBoxx = boolean;
            break;
        case 2:
            isPlayerName = boolean;
            break;
        case 3:
            isPlayerDist = boolean;
            break;
        case 4:
            isPlayerHealth = boolean;
            break;
        case 5:
            isr360Alert = boolean;
            break;
        case 6:
            isNoBot = boolean;
            break;
        case 7:
            isPlayerLine = value;
            break;
        case 8:
            isPlayerBox = value;
            break;
        case 9:
            options.SilentAim = boolean;
            break;
        case 10:
            options.AimPos = value;
            break;
        case 11:
            options.fov = value;
            break;
        case 12:
            options.AimLine = boolean;
            break;
        case 14:
            options.AimCollider = boolean;
            break;
        case 15:
            options.AutoRetri = boolean;
            break;
        case 16:
            options.touchSize = value;
            break;
        case 17:
            options.touchX = value;
            break;
        case 18:
            options.touchY = value;
            break;
        case 19:
            options.InputInversion = boolean;
            break;
        case 20:
            isShowTouch = boolean;
            break;
        case 21:
            options.ResetGuest = boolean;
            break;
        case 22:
            options.BypassLobby = boolean;
            break;
        case 23:
            options.AimMagnet = boolean;
            break;
        default:
            break;
    }
}

bool isConnected(JNIEnv *, jobject) {
    return connected;
}

bool getReady(JNIEnv *, jobject) {
	int sockCheck = 1;
	if (!Create()) {
		perror("Creation failed");
		return false;
	}
	
	setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &sockCheck, sizeof(int));
	if (!Bind()) {
		perror("Bind failed");
		return false;
	}

	if (!Listen()) {
		perror("Listen failed");
		return false;
	}
	
	if (Accept()) {
		return true;
	}
}

int Register1(JNIEnv *env) {
	JNINativeMethod methods[] = {
			{"native_Check", "(Landroid/content/Context;Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;", (void *) native_Check}
	};
	jclass clazz = env->FindClass("com/loader/yogs/LoginActivity");
	if (!clazz)
		return -1;

	if (env->RegisterNatives(clazz, methods, sizeof(methods) / sizeof(methods[0])) != 0)
		return -1;

	return 0;
}

int Register2(JNIEnv *env) {
	JNINativeMethod methods[] = {
			{"EXP", "()Ljava/lang/String;", (void *) EXP}
	};
	jclass clazz = env->FindClass("com/loader/yogs/MainActivity");
	if (!clazz)
		return -1;

	if (env->RegisterNatives(clazz, methods, sizeof(methods) / sizeof(methods[0])) != 0)
		return -1;

	return 0;
}

int Register3(JNIEnv *env) {
	JNINativeMethod methods[] = {
			{"DrawOn", "(Lcom/loader/yogs/ESPView;Landroid/graphics/Canvas;)V", (void *) DrawOn},
			{"Close", "()V", (void *) Closee},
			{"getReady", "()Z", (void *) getReady},
            {"isConnected", "()Z", (void *) isConnected}
	};
	jclass clazz = env->FindClass("com/loader/yogs/Overlay");
	if (!clazz)
		return -1;

	if (env->RegisterNatives(clazz, methods, sizeof(methods) / sizeof(methods[0])) != 0)
		return -1;

	return 0;
}

int Register4(JNIEnv *env) {
	JNINativeMethod methods[] = {
        {OBFUSCATE("GetFeatureList"), OBFUSCATE("()[Ljava/lang/String;"), reinterpret_cast<void *>(GetFeatureList)},
        {OBFUSCATE("Changes"), OBFUSCATE("(Landroid/content/Context;ILjava/lang/String;IJZLjava/lang/String;)V"), reinterpret_cast<void *>(Changes)}
	};
	jclass clazz = env->FindClass("com/loader/yogs/Floating");
	if (!clazz)
		return -1;

	if (env->RegisterNatives(clazz, methods, sizeof(methods) / sizeof(methods[0])) != 0)
		return -1;

	return 0;
}

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {
	JNIEnv *env;
	vm->GetEnv((void **) &env, JNI_VERSION_1_6);
	if (Register1(env) != 0)
		return -1;
	if (Register2(env) != 0)
		return -1;
	if (Register3(env) != 0)
		return -1;
	if (Register4(env) != 0)
		return -1;
	return JNI_VERSION_1_6;
}