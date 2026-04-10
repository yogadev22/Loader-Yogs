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

using json = nlohmann::ordered_json;
std::string g_Auth, g_Token,ts;
bool xConnected = false;
bool check;
int modekey = 1;

#include <openssl/sha.h>
std::string sha256(const std::string &input) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256((unsigned char*)input.c_str(), input.size(), hash);

    char outputBuffer[65];
    for(int i = 0; i < SHA256_DIGEST_LENGTH; i++)
        sprintf(outputBuffer + (i * 2), "%02x", hash[i]);

    outputBuffer[64] = 0;
    return std::string(outputBuffer);
}

extern "C" JNIEXPORT jstring JNICALL native_Check(JNIEnv *env, jclass clazz, jobject mContext, jstring mUserKey) {
    const char* user_key = env->GetStringUTFChars(mUserKey, 0);
    //const char* mode_select = env->GetStringUTFChars(mModeSelect, 0);
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
        sprintf(lol, OBFUSCATE("http://yogspanel.indevs.in/connect"));

        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
        curl_easy_setopt(curl, CURLOPT_URL, lol);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
        curl_easy_setopt(curl, CURLOPT_DEFAULT_PROTOCOL, "https");
        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Accept: application/json");
        headers = curl_slist_append(headers,"Content-Type: application/x-www-form-urlencoded");
        headers = curl_slist_append(headers, "Charset: UTF-8");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        char data[4096];
        sprintf(data, "game=PUBG&user_key=%s&serial=%s", user_key, UUID.c_str());
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
                auto STATUS = std::string{"status"};
                if (result[STATUS] == true) {
                    auto DATA = std::string{"data"};
                    auto TOKEN = std::string{"token"};
                    auto RNG = std::string{"rng"};
                    std::string token = result[DATA][TOKEN].get<std::string>();
                    //ONOFFBGMI = result["data"]["ON_OFF_BGMI"].get<std::string>();
                    time_t rng = result[DATA][RNG].get<time_t>();
                    ts = result["data"]["EXP"].get<std::string>();
                    if (rng + 30 > time(0)) {
                        std::string auth = "PUBG";
                        auth += "-";
                        auth += user_key;
                        auth += "-";
                        auth += UUID;
                        auth += "-";
                        auth += "Vm8Lk7Uj2JmsjCPVPVjrLa7zgfx3uz9E";
                        std::string outputAuth = Tools::CalcMD5(auth);
                        g_Token = token;
                        g_Auth = outputAuth;
                        xConnected = g_Token == g_Auth;
                    }
                } else {
                    auto REASON = std::string{"reason"};
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
    return xConnected ? env->NewStringUTF(StrEnc("8q", "\x77\x3A", 2).c_str()) : env->NewStringUTF(errMsg.c_str());
}

extern "C" JNIEXPORT jstring JNICALL EXP(JNIEnv *env, jclass clazz) {
    return env->NewStringUTF(ts.c_str());
}

#endif