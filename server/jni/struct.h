#ifndef PAKC_STRUCT_H
#define PAKC_STRUCT_H

#include <string>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include "Vector2.hpp"
#include "Vector3.hpp"

struct Vector4 {
    float X, Y, Z, W;
    
    Vector4() : X(0), Y(0), Z(0), W(0) {}
    Vector4(float x, float y, float z, float w) : X(x), Y(y), Z(z), W(w) {}
};

struct Options {
    bool SilentAim;
    int fov;
    int AimPos;
    bool AimLine;
    bool AimCollider;
    bool AutoRetri;
    int touchSize;
    int touchX;
    int touchY;
    bool InputInversion;
    bool ResetGuest;
    bool BypassLobby;
    bool noreload;
    bool SpeedHack;
    bool AimMagnet;
    bool GhostHack;
    bool NoSwapWp;
    bool flyhack;
};

struct Request {
    int screenWidth;
    int screenHeight;
    Options options;
};

struct PlayerData {
    Vector3 HeadPos;
    Vector3 RootPos;
    int Health;
    int HealthMax;
    bool isKnocked;
    char Name[64];
    float Distance;
    bool isBot;
    std::string WeaponName;
};

struct Response {
    bool Success;
    int PlayerCount;
    int MonsterCount;
    Vector3 targetPos;
    PlayerData Players[100];
};

#endif
