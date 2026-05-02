#ifndef LOGIN_H
#define LOGIN_H

#include "StrEnc.h"
#include "Includes.h"
#include "curl/curl.h"
#include "Tools.h"
#include "json.hpp"
#include "Log.h"
#include "LicenseTools.h"
#include <jni.h>
#include <string>
#include "oxorany.h"

using json = nlohmann::ordered_json;
std::string g_Auth, g_Token,ts;
bool xConnected = false;
bool check;
int modekey = 1;

extern "C" JNIEXPORT jstring JNICALL native_Check(JNIEnv *env, jclass clazz, jobject mContext, jstring mUserKey) {
    const char* user_key = env->GetStringUTFChars(mUserKey, 0);
    std::string hwid = user_key;
    hwid += GetAndroidID(env, mContext);
    hwid += GetDeviceModel(env);
    hwid += GetDeviceBrand(env);
    std::string UUID = GetDeviceUniqueIdentifier(env, hwid.c_str());
    std::string errMsg;
    struct MemoryStruct chunk{};
    chunk.memory = (char *) malloc(1);
    chunk.size = 0;

    CURL *curl;
    CURLcode res;
    curl = curl_easy_init();

    if (curl) {
        char lol[100];
        sprintf(lol, oxorany("https://yogspanel.indevs.in/connect"));

        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, oxorany("POST"));
        curl_easy_setopt(curl, CURLOPT_URL, lol);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
        curl_easy_setopt(curl, CURLOPT_DEFAULT_PROTOCOL, oxorany("https"));
        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, oxorany("Accept: application/json"));
        headers = curl_slist_append(headers, oxorany("Content-Type: application/x-www-form-urlencoded"));
        headers = curl_slist_append(headers, oxorany("Charset: UTF-8"));
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        char data[4096];
        sprintf(data, oxorany("game=FFMAX&user_key=%s&serial=%s"), user_key, UUID.c_str());
        //printf("%s\n",data);
        curl_easy_setopt(curl, CURLOPT_POST, 1);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *) &chunk);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 2);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYSTATUS, 0);
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "AbsoluteX/2.0");

        res = curl_easy_perform(curl);
        if (res == CURLE_OK) {
            try {
                json result = json::parse(chunk.memory);
                auto STATUS = std::string{oxorany("status")};
                if (result[STATUS] == true) {
                    auto DATA = std::string{oxorany("data")};
                    auto TOKEN = std::string{oxorany("token")};
                    auto RNG = std::string{oxorany("rng")};
                    std::string token = result[DATA][TOKEN].get<std::string>();
                    time_t rng = result[DATA][RNG].get<time_t>();
                    ts = result[oxorany("data")][oxorany("EXP")].get<std::string>();
                    if (rng + 30 > time(0)) {
                        std::string auth = oxorany("FFMAX");
                        auth += oxorany("-");
                        auth += user_key;
                        auth += oxorany("-");
                        auth += UUID;
                        auth += oxorany("-");
                        auth += oxorany("https://t.me/+kEL-bgkV_nkzYzk1YOGSONTHETOP356265362g3KJ52761");
                        std::string outputAuth = Tools::CalcMD5(auth);
                        g_Token = token;
                        g_Auth = outputAuth;
                        xConnected = g_Token == g_Auth;
                    }
                } else {
                    auto REASON = std::string{oxorany("reason")};
                    errMsg = result[REASON].get<std::string>();
                }
            } catch (json::exception &e) {
                errMsg = e.what();
            }
        } else {
            errMsg = curl_easy_strerror(res);
        }
    }
    curl_easy_cleanup(curl);
    return xConnected ? env->NewStringUTF(StrEnc(oxorany("8q"), oxorany("\x77\x3A"), 2).c_str()) : env->NewStringUTF(errMsg.c_str());
}

extern "C" JNIEXPORT jstring JNICALL EXP(JNIEnv *env, jclass clazz) {
    return env->NewStringUTF(ts.c_str());
}

#endif