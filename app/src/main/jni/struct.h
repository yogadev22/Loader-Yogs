#ifndef PAKC_STRUCT_H
#define PAKC_STRUCT_H

#include <string>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include "Vector2.hpp"
#include "Vector3.hpp"

#define maxplayerCount 100
#define maxvehicleCount 50
#define maxitemsCount 400
#define maxgrenadeCount 10
#define maxboxitemsCount 150
#define maxzoneCount 10

class Color {
public:
    float r;
    float g;
    float b;
    float a;

    Color() {
        this->r = 0;
        this->g = 0;
        this->b = 0;
        this->a = 0;
    }

    Color(float r, float g, float b, float a) {
        this->r = r;
        this->g = g;
        this->b = b;
        this->a = a;
    }

    Color(float r, float g, float b) {
        this->r = r;
        this->g = g;
        this->b = b;
        this->a = 255;
    }

    static Color White(int alpha){
        return Color(255, 255, 255, alpha);
    }
    static Color Green(int alpha){
        return Color(0,255,0, alpha);
    }
    static Color Wood(int alpha){
        return Color(40,252,80, alpha);
    }
    static Color Black(int alpha){
        return Color(0,0,0, alpha);
    }
    static Color Red(int alpha)
    {
        return Color(255, 0, 0, alpha);
    }
    static Color Yellow(int alpha)
    {
        return Color(255, 255, 0, alpha);
    }
    static Color Orange(int alpha)
    {
        return Color(255, 165, 0, alpha);
    }
    static Color Blue(int alpha)
    {
        return Color(45, 255, 220, alpha);
    }
};

int randomer;
Color FRAGColor() {
    randomer += 4;
    if (randomer == 0) {
        return Color(255, 165, 0);
    } else if (randomer == 1) {
        return Color(255, 165, 0);
    }else if (randomer == 2) {
        return Color(255, 165, 0);
    }else if (randomer == 3) {
        return Color(255, 165, 0);
    }else if (randomer == 4) {
        return Color(255, 165, 0);
    }else if (randomer == 5) {
        return Color(255, 165, 0);
    }else if (randomer == 6) {
        return Color(255, 165, 0);
    }else if (randomer == 7) {
        return Color(255, 165, 0);
    }else if (randomer == 8) {
        return Color(255, 165, 0);
    }else if (randomer == 9) {
        return Color(255, 165, 0);
    }else if (randomer == 10) {
        return Color(255, 165, 0);
    }else if (randomer == 11) {
        return Color(255, 165, 0);
    }else if (randomer == 12) {
        return Color(255, 165, 0);
    }else if (randomer == 13) {
        return Color(255, 165, 0);
    }else if (randomer == 14) {
        return Color(255, 165, 0);
    }else if (randomer == 15) {
        return Color(255, 165, 0);
    }else if (randomer == 16) {
        return Color(255, 165, 0);
    }else if (randomer == 17) {
        return Color(255, 165, 0);
    } else if (randomer == 18) {
        return Color(255, 165, 0);
    } else if (randomer == 19) {
        return Color(255, 165, 0);
    } else if (randomer == 20) {
        return Color(255, 165, 0);
    } else if (randomer == 21) {
        return Color(255, 0, 0);
    } else if (randomer == 22) {
        return Color(255, 0, 0);
    } else if (randomer == 23) {
        return Color(255, 0, 0);
    } else if (randomer == 24) {
        return Color(255, 0, 0);
    } else if (randomer == 25) {
        return Color(255, 0, 0);
    } else if (randomer == 26) {
        return Color(255, 0, 0);
    } else if (randomer == 27) {
        return Color(255, 0, 0);
    } else if (randomer == 28) {
        return Color(255, 0, 0);
    } else if (randomer == 29) {
        return Color(255, 165, 0);
    } else if (randomer == 30) {
        return Color(255, 165, 0);
    } else if (randomer == 31) {
        return Color(255, 165, 0);
    } else if (randomer == 32) {
        return Color(255, 165, 0);
    } else if (randomer == 33) {
        return Color(255, 165, 0);
    } else if (randomer == 34) {
        return Color(255, 165, 0);
    } else if (randomer == 35) {
        return Color(255, 165, 0);
    } else if (randomer == 36) {
        return Color(255, 165, 0);
    } else if (randomer == 37) {
        return Color(255, 165, 0);
    } else if (randomer == 38) {
        return Color(255, 165, 0);
    } else if (randomer ==39) {
        return Color(255, 165, 0);
    } else if (randomer == 40) {
        return Color(255, 165, 0);
    } else if (randomer == 41) {
        return Color(255, 165, 0);
    } else if (randomer == 42) {
        return Color(255, 165, 0);
    }else if (randomer == 43) {
        return Color(255, 165, 0);
    }else if (randomer == 44) {
        return Color(255, 165, 0);
    }else if (randomer == 45) {
        return Color(255, 165, 0);
    }else if (randomer == 46) {
        return Color(255, 0, 0);
    }else if (randomer == 47) {
        return Color(255, 0, 0);
    }else if (randomer == 48) {
        return Color(255, 0, 0);
    }else if (randomer == 49) {
        return Color(255, 0, 0);
    }else if (randomer == 50) {
        return Color(255, 0, 0);
    }else if (randomer == 51) {
        return Color(255, 0, 0);
    }else if (randomer == 52) {
        return Color(255, 0, 0);
    }else if (randomer == 53) {
        return Color(255, 0, 0);
    }else if (randomer == 54) {
        return Color(255, 0, 0);
    }else if (randomer == 55) {
        return Color(255, 0, 0);
    }else if (randomer == 56) {
        return Color(255, 0, 0);

    }else if (randomer == 57) {
        return Color(255, 0, 0);
    }else if (randomer == 58) {
        return Color(255, 0, 0);
    }else if (randomer == 59) {
        return Color(255, 0, 0);
    }else if (randomer == 60) {
        return Color(255, 0, 0);
    }
    else {
        randomer = 0;
        return Color(255, 0, 0);
    }
}

class Rect {
    public:
	float x;
    float y;
    float width;
    float height;
	
    Rect() {
        this -> x = 0;
        this -> y = 0;
        this -> width = 0;
        this -> height = 0;
    }
	
    Rect(float x, float y, float width, float height) {
        this -> x = x;
        this -> y = y;
        this -> width = width;
        this -> height = height;
    }
	
    bool operator == (const Rect & src) const {
        return (src.x == this -> x && src.y == this -> y && src.height == this -> height && src.width == this -> width);
    }
	
    bool operator != (const Rect & src) const {
        return (src.x != this -> x && src.y != this -> y && src.height != this -> height && src.width != this -> width);
    }
};

struct PlayerBone {
    bool isBone = false;
    Vector3 head;
    Vector3 neck;
    Vector3 cheast;
    Vector3 pelvis;
    Vector3 lSh;
    Vector3 rSh;
    Vector3 lElb;
    Vector3 rElb;
    Vector3 lWr;
    Vector3 rWr;
    Vector3 lTh;
    Vector3 rTh;
    Vector3 lKn;
    Vector3 rKn;
    Vector3 lAn;
    Vector3 rAn;
    Vector3 root;
};

struct Vector4 {
    float X, Y, Z, W;
};

Vector4 getPrecise(PlayerBone b, Vector3 head, Vector3 root)
{
    float minX = 3000.0f;
    float minY = 3000.0f;
    float maxX = 0.0f;
    float maxY = 0.0f;

    float x[40];
    PlayerBone n = b;

    int iter = 0;
    x[30] = root.X;
    x[31] = root.Y;
    x[32] = head.X;
    x[33] = head.Y;
    memcpy(&x, &n.neck, 120);

    for (int k = 0; k < 17; k++)
    {
        minX = std::min(minX, x[iter]);
        minY = std::min(minY, x[iter + 1]);
        maxX = std::max(maxX, x[iter]);
        maxY = std::max(maxY, x[iter + 1]);
        iter += 2;
    }

    return { minX - 5.0f, minY - 5.0f, maxX + 5.0f, maxY + 5.0f };
}

struct FMatrix {
    float M[4][4];
};

struct PlayerWeapon {
    bool isWeapon = false;
    int id;
    int ammo;
    int maxammo;
};

enum Mode {
    InitMode = 1
};

struct Options {
    int aimbotmode;
    int openState;
    bool tracingStatus;
    int priority;
    bool pour;
    int aimingRange;
    int wideView;
    bool isMetroMode;
    bool isRadar;
	bool lobbybypass;
};

struct Memory {
    bool LessRecoil;
    bool ZeroRecoil;
    bool InstantHit;
    bool SmallCrosshair;
    bool FastParachute;
    bool NoShake;
    bool HitEffect;
	bool Aimbotsdk;
};

struct Request {
    int Mode;
    Options options;
    Memory memory;
    int ScreenWidth;
    int ScreenHeight;
    int GameType;
};

struct VehicleData {
    char VehicleName[50];
    float Distance;
    float Fuel;
    float Health;
    Vector3 Location;
};

struct ItemData {
    char ItemName[50];
    float Distance;
    Vector3 Location;
};

struct GrenadeData {
    int type;
    float Distance;
    Vector3 Location;
};

struct BoxItemData {
    int ItemType;
    int ItemID[40];
    int ItemCount;
    int Count[40];
    Vector3 Location;
};

struct PlayerData {
    char PlayerNameByte[100];
    int TeamID;
    float Health;
    float Distance;
    bool isBot;
    bool isKnocked;
	bool isVisible;
    Vector4 Precise;
	Vector3 HeadLocation;
	Vector3 Location;
    Vector2 RadarLocation;
    PlayerWeapon Weapon;
    PlayerBone Bone;
};

struct Response {
    bool Success;
	bool Identified;
	float fov;
    int PlayerCount;
    Vector3 camLocation;
    FMatrix matrix;
    PlayerData Players[maxplayerCount];
};
#endif
