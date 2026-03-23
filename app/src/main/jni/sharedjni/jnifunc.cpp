#include "Login.h"

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

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {
    JNIEnv *env;
    vm->GetEnv((void **) &env, JNI_VERSION_1_6);
    if (Register1(env) != 0)
        return -1;
    if (Register2(env) != 0)
        return -1;
    return JNI_VERSION_1_6;
}