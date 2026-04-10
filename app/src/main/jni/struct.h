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
};

struct Options {
    bool SilentAim;
    int fov;
    int AimPos;
    bool AimLine;
    bool AimCollider;
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
    bool isKnocked;
    char Name[64];
    float Distance;
    bool isBot;
    std::string WeaponName;
};

struct Response {
    bool Success;
    int PlayerCount;
    Vector3 targetPos;
    PlayerData Players[100];
};

#endif
