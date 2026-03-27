#ifndef PAKC_STRUCT_H
#define PAKC_STRUCT_H

#include <string>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include "Vector2.hpp"
#include "Vector3.hpp"

struct D3DMatrix {
    float _11, _12, _13, _14;
    float _21, _22, _23, _24;
    float _31, _32, _33, _34;
    float _41, _42, _43, _44;
};

struct Vector4 {
    float X, Y, Z, W;
};

enum Mode {
    InitMode = 1
};

struct Request {
    int Mode;
};

struct PlayerData {
    Vector3 HeadPos;
    Vector3 RootPos;
    bool isKnocked;
    int health;
    char Name[100];
    float Distance;
    bool isBot;
};

struct Response {
    bool Success;
    float fov;
    int PlayerCount;
    D3DMatrix matrix;
    PlayerData Players[100];
};

#endif
