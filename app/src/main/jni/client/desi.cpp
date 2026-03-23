#include <jni.h>
#include <string>
#include "obfuscate.h"
#include "../sharedjni/Login.h"
#include "Hacks.h"

ESP espOverlay;
int type = 1, utype = 2;

extern "C" JNIEXPORT void JNICALL DrawOn(JNIEnv * env, jclass, jobject espView, jobject canvas) {
	espOverlay = ESP(env, espView, canvas);
	if (espOverlay.isValid()) {
		DrawESP(espOverlay, espOverlay.getWidth(), espOverlay.getHeight());
	}
}

extern "C" JNIEXPORT void JNICALL Closee(JNIEnv *, jobject) {
	Close();
}

extern "C" JNIEXPORT void JNICALL SettingValue(JNIEnv *, jclass, jint code, jboolean jboolean1) {
	switch ((int)code) {
	case 1:
		isPlayerLinee = jboolean1;
		break;
	}
}

extern "C" JNIEXPORT jboolean JNICALL getReady(JNIEnv *, jobject) {
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
			{"native_Check", "(Landroid/content/Context;Ljava/lang/String;)Ljava/lang/String;", (void *) native_Check}
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
			{"getReady", "()Z", (void *) getReady}
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
			{"SettingValue", "(IZ)V", (void *) SettingValue},
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