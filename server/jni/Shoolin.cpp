#include "support.h"
#include "TouchInput.hpp"
bool isML;

bool isAim = false;
float touchX = 121.f;
float touchY = 1526.f;
float touchRange = 300.f;
bool ChargingPortLeft = false;
bool wasTouching = false;
std::thread retriThread;

void AutoRetri() {
    sleep(5);

    TouchInput::touchInputStart(screenHeight, screenWidth);

    float baseX = touchX;
    float baseY = touchY;

    bool hasTapped = false;

    while (keepAiming) {
        if (!isAim) {
            hasTapped = false;
            usleep(1000);
            continue;
        }

        if (!hasTapped) {
            // Cek apakah posisi touch masih di dalam range
            bool isInsideRange = (baseX >= touchX - touchRange) && (baseX <= touchX + touchRange) && (baseY >= touchY - touchRange) && (baseY <= touchY + touchRange);
            if (isInsideRange) {
                TouchInput::Tap((int)baseX, (int)baseY);
                hasTapped = true;
            }
        }

        usleep(1000);
    }
}

int CalculateRetriDamage(int m_Level, int m_KillWildTimes) {
    int AddRetriDmg, CalculateRetriDamage = (m_KillWildTimes>5) ? 520 + ((m_Level+5)*80), 520 + ((m_Level+5)*160), 520 + ((m_Level+5)*240), /*Next 7*/ 780 + ((m_Level+5)*480), 780 + ((m_Level+6)*600), 780 + ((m_Level+7)*720), 780 + ((m_Level+8)*840), 780 + ((m_Level+9)*960), 780 + ((m_Level+10)*1080), 780 + ((m_Level+11)*1200), 780 + ((m_Level+12)*1440), 780 + ((m_Level+13)*1560), 780 + ((m_Level+14)*1680), 780 + ((m_Level+15)*1800) : 0;
    if (m_KillWildTimes < 5) {
        return 530 + (80 * m_Level);
    } else {
        return 1.5 * (530 + (80 * m_Level)); + (380 + (40 * m_Level)) - 120;  
    }
}

struct Camera {
    Matrix4x4 worldToCameraMatrix;
    Matrix4x4 projectionMatrix;
};

int main(int argc, char *argv[]) {
    if (!Create()) {
        perror("Creation Failed");
        return 0;
    }

    if (!Connect()) {
        perror("Connection Failed");
        return 0;
    }
    
    pid = getPid("com.dts.freefiremax");
    uintptr_t base = FindLibrary("libil2cpp.so", 2);
    
    if (pid == 0 || base == 0) {
        pid = getPid("com.mobile.legends:UnityKillsMe");
        base = FindLibrary("libcsharp.so", 1);
        isML = true;
        
        if (pid == 0 || base == 0) {
            Close();
            return 0;
        }
    }
    
    Request request{};
    Response response{};

    while ((receive((void *) &request) > 0)) {
        screenWidth = request.screenWidth;
        screenHeight = request.screenHeight;
        response.Success = false;
        response.PlayerCount = 0;
        
        if (isML) {
            touchX = (float)request.options.touchX;
            touchY = (float)request.options.touchY;
            ChargingPortLeft = request.options.InputInversion;
            touchRange = (float)request.options.touchSize;
            
            uintptr_t a1 = Read<uintptr_t>(base + 0x6F06DB8);
            uintptr_t a2 = Read<uintptr_t>(a1 + 0xB8);
            uintptr_t a32 = Read<uintptr_t>(a2 & 0x7FFFFFFFFFFFFFFF);
            
            uintptr_t localPlayer = Read<uintptr_t>(a32 + 0x50);
            if (localPlayer) {
                uintptr_t main_cam = Read<uintptr_t>(base + 0x6EF4430);
                uintptr_t main_cam2 = Read<uintptr_t>(main_cam + 0xB8);
                uintptr_t main_cam3 = Read<uintptr_t>(main_cam2 + 0x8);
                uintptr_t camera = Read<uintptr_t>(main_cam3 + 0x10);
                Camera ViewMatrix = Read<Camera>(camera + 0x5C);
                Matrix4x4 _vMatrix = ViewMatrix.projectionMatrix * ViewMatrix.worldToCameraMatrix;
            
                Vector3 mycachepos = Read<Vector3>(localPlayer + 0x294);
                
                uintptr_t dict = Read<uintptr_t>(a32 + 0x68);
                uintptr_t entries = Read<uintptr_t>(dict + 0x18);
                int count = Read<int>(entries + 0x18);
                uintptr_t array = entries + 0x20;
                
                for (int i = 0; i < count; i++) {
                    uintptr_t entry = array + (i * 0x18);
                        
                    int hash = Read<int>(entry + 0x0);
                    if (hash < 0) 
                        continue;
                        
                    uintptr_t enemy = Read<uintptr_t>(entry + 0x10);
                    if (!enemy || enemy == localPlayer || Read<bool>(enemy + 0xcd) || Read<bool>(enemy + 0x2b1))
                        continue;
                            
                    bool isBot;
                    if (Read<bool>(enemy + 0x419) || Read<bool>(enemy + 0x9b)) {
                        isBot = true;
                    } else {
                        isBot = false;
                    }
                        
                    Vector3 cachepos = Read<Vector3>(enemy + 0x294);
                        
                    float Distance = Vector3::Distance(mycachepos, cachepos);
                            
                    Vector3 PosWS = WorldToScreen(_vMatrix, cachepos);
                            
                    int HP = Read<int>(enemy + 0x1ac);
                    int HPMax = Read<int>(enemy + 0x1b0);
                            
                    PlayerData *data = &response.Players[response.PlayerCount];
                    data->Distance = Distance;
                    data->HeadPos = Vector3(PosWS.X, PosWS.Y - (screenHeight / 6.70), PosWS.Z);
                    data->RootPos = PosWS;
                    data->isBot = isBot;
                    data->Health = HP;
                    data->HealthMax = HPMax;
                        
                    uintptr_t NamePtr = Read<uintptr_t>(enemy + 0x918); 
                    if (NamePtr > 0x10000000) {
                        getUTF8(data->Name, NamePtr);
                    }
                    if (data->Name[0] == '\0') 
                        strcpy(data->Name, "Unknown");
                        
                    response.PlayerCount++;
                }
                    
                int iCalculateDamage = CalculateRetriDamage(Read<int>(localPlayer + 0x198), Read<int>(localPlayer + 0xa60));
                    
                uintptr_t dictmon = Read<uintptr_t>(a32 + 0x70);
                uintptr_t entriesmon = Read<uintptr_t>(dictmon + 0x18);
                int countmon = Read<int>(entriesmon + 0x18);
                uintptr_t arraymon = entriesmon + 0x20;
                
                for (int i = 0; i < countmon; i++) {
                    uintptr_t entry = arraymon + (i * 0x18);
                        
                    int hash = Read<int>(entry + 0x0);
                    if (hash < 0) 
                        continue;
                        
                    uintptr_t monster = Read<uintptr_t>(entry + 0x10);
                    if (!monster || Read<bool>(monster + 0xcd) || Read<bool>(monster + 0x2b1))
                        continue;
                        
                    int m_ID = Read<int>(monster + 0x194);
                    if (m_ID == 2002 || m_ID == 2003) {
                        Vector3 cachepos = Read<Vector3>(monster + 0x294);
                        float Distance = Vector3::Distance(mycachepos, cachepos);
                                
                        int CurHP = Read<int>(monster + 0x1ac);
                        int MaxHP = Read<int>(monster + 0x1b0);
                        
                        Vector3 enemyHead = WorldToScreen(_vMatrix, cachepos);
                            
                        if (Distance < 5.4f && CurHP < MaxHP) {
                            if (CurHP <= iCalculateDamage) {
                                isAim = true;
                            } else {
                                isAim = false;
                            }
                        }
                        
                        response.MonsterCount++;
                    }
                }
                    
                if (request.options.AutoRetri) {
                    if (!keepAiming) {
                        keepAiming = true;
                        if (retriThread.joinable()) {
                            retriThread.join();
                        }
                        retriThread = std::thread(AutoRetri);
                    }
                } else {
                    if (keepAiming) {
                        keepAiming = false;
                        if (retriThread.joinable()) 
                            retriThread.join();  
                            
                        isAim = false;
                    }
                }
            }
        } else {
            float nearest = -1.0f;
            uintptr_t GameFacadeBase = Read<uintptr_t>(base + 0xA989BA0);
            uintptr_t GameFacade_c = Read<uintptr_t>(GameFacadeBase + 0xB8);
            uintptr_t MatchGame = Read<uintptr_t>(GameFacade_c);
            uintptr_t match = Read<uintptr_t>(MatchGame + 0x90);
            if (match) {
                uintptr_t localPlayer = Read<uintptr_t>(match + 0xd8);
                
                if (localPlayer) {
                    uintptr_t followcam = Read<uintptr_t>(localPlayer + 0x638);
                    uintptr_t cambase = Read<uintptr_t>(followcam + 0x30);
                    uintptr_t cam = Read<uintptr_t>(cambase + 0x10);
                    
                    Matrix4x4 ViewMatrix = Read<Matrix4x4>(cam + 0x80);
                    Matrix4x4 ProjMatrix = Read<Matrix4x4>(cam + 0xC0);
                    Matrix4x4 _vMatrix = ProjMatrix * ViewMatrix;
                    
                    Vector3 targetAimPos = Vector3::Zero();
                    bool isFiring = Read<bool>(localPlayer + 0x7e0);
    
                    Vector3 myPos = GetPosition(Read<uintptr_t>(Read<uintptr_t>(localPlayer + 0x648) + 0x10));
    
                    uintptr_t dict = Read<uintptr_t>(match + 0x128);
                    uintptr_t entries = Read<uintptr_t>(dict + 0x18);
                    int count = Read<int>(entries + 0x18);
    
                    if (count <= 0) {
                        send((void *) &response, sizeof(response));
                        continue;
                    }
    
                    uintptr_t array = entries + 0x20;
                    
                    for (int i = 0; i < count; i++) {
                        uintptr_t entry = array + (i * 0x28);
                    
                        int hash = Read<int>(entry + 0x0);
                        if (hash < 0) 
                            continue;
                    
                        uintptr_t enemy = Read<uintptr_t>(entry + 0x20);
                            
                        if (!enemy || enemy == localPlayer || Read<bool>(enemy + 0x7c))
                            continue;
    
                        float Distance = Vector3::Distance(myPos, GetPosition(Read<uintptr_t>(Read<uintptr_t>(enemy + 0x648) + 0x10)));
                        if (Distance > 150)
                            continue;
    
                        uintptr_t AvatarManager = Read<uintptr_t>(enemy + 0x718);
                        if (!AvatarManager)
                            continue;
    
                        uintptr_t UmaAvatarSimple = Read<uintptr_t>(AvatarManager + 0x128);
                        if (!UmaAvatarSimple)
                            continue;
    
                        bool IsVisible = Read<bool>(UmaAvatarSimple + 0x101);
                        if (!IsVisible)
                            continue;
    
                        uintptr_t UmaData = Read<uintptr_t>(UmaAvatarSimple + 0x28);
                        if (!UmaData)
                            continue;
    
                        if (Read<bool>(UmaData + 0x81))
                            continue;
                            
                        PlayerData *data = &response.Players[response.PlayerCount];
    
                        uintptr_t GetCurHP = Read<uintptr_t>(enemy + 0x70);
                        if (GetCurHP) {
                            uintptr_t GetCur = Read<uintptr_t>(GetCurHP + 0x10);
                            if (GetCur) {
                                uintptr_t HP = Read<uintptr_t>(GetCur + 0x20);
                                if (HP) data->Health = (int) Read<short>(HP + 0x18);
                            }
                        }
                        
                        data->HealthMax = 200;
    
                        uintptr_t HHCBNAPCKHF = Read<uintptr_t>(enemy + 0x19f8);
                        if (HHCBNAPCKHF) {
                            uintptr_t maybeDead = Read<uintptr_t>(HHCBNAPCKHF + 0x20);
                            if (maybeDead) {
                                int pose = Read<int>(maybeDead + 0x10);
                                if (pose == 8) {
                                    data->isKnocked = true;
                                } else {
                                    data->isKnocked = false;
                                }
                            }
                        }
                        
                        Vector3 HeadPos = GetPosition(Read<uintptr_t>(Read<uintptr_t>(enemy + 0x648) + 0x10));
                        
                        data->Distance = Distance;
                        data->HeadPos = WorldToScreen(_vMatrix, HeadPos);
                        data->RootPos = WorldToScreen(_vMatrix, GetPosition(Read<uintptr_t>(Read<uintptr_t>(enemy + 0x670) + 0x10)));
                        data->isBot = Read<bool>(enemy + 0x450);
                        
                        memset(data->Name, 0, 64);
                        if (data->isBot) {
                            strcpy(data->Name, "Bot");
                        } else {
                            uintptr_t NamePtr = Read<uintptr_t>(enemy + 0x448); 
                            if (NamePtr > 0x10000000) {
                                getUTF8(data->Name, NamePtr);
                            }
                            if (data->Name[0] == '\0') 
                                strcpy(data->Name, "Enemy");
                        }
                        
                        response.PlayerCount++;
                        
                        uintptr_t HedColider = Read<uintptr_t>(enemy + 0x658);
                        if (request.options.AimCollider)
                            Write<uintptr_t>(enemy + 0x80, HedColider);
                            
                        auto weapon = Read<uintptr_t>(localPlayer + 0x540);
                        if (request.options.AimCollider && weapon != 0) {
                            float newAimLock = 0.0f;
                            Write<float>(weapon + 0x4dc, newAimLock);
                        }
                        
                        if (request.options.SilentAim && !data->isKnocked) {
                            float centerDist = sqrt((data->HeadPos.X - request.screenWidth / 2) * (data->HeadPos.X - request.screenWidth / 2) + (data->HeadPos.Y - request.screenHeight / 2) * (data->HeadPos.Y - request.screenHeight / 2));
                            if (centerDist < request.options.fov) {
                                if (nearest > centerDist || nearest < 0) {
                                    nearest = centerDist;
                                    
                                    if (request.options.AimPos == 0) {
                                        targetAimPos = HeadPos; // head
                                    } else if (request.options.AimPos == 1) {
                                        targetAimPos = HeadPos + Vector3(0, -0.1f, 0); // neck
                                    } else if (request.options.AimPos == 2) {
                                        targetAimPos = HeadPos + Vector3(0, -0.3f, 0); // chest
                                    }
                                }
                            }
                        }
                    }
                    
                    if (request.options.AimLine) {
                        if (nearest > 0) {
                            response.targetPos = WorldToScreen(_vMatrix, targetAimPos);
                        } else {
                            response.targetPos = Vector3::Zero();
                        }
                    }
                    
                    if (request.options.SilentAim && (nearest > 0 && isFiring)) {
                        currentBestTargetPos = targetAimPos;
                        if (!keepAiming) {
                            keepAiming = true;
                            if (aimThread.joinable()) 
                                aimThread.join();  
                                aimThread = std::thread([&]() {ContinuousAim(localPlayer);});
                        }
                    } else {
                        if (keepAiming) {
                            keepAiming = false;
                            if (aimThread.joinable()) 
                                aimThread.join();  
                                currentBestTargetPos = Vector3::Zero();
                        }
                    }
                }
            }
        }

        if (response.PlayerCount + response.MonsterCount > 0) {
            response.Success = true;
        }
        send((void *) &response, sizeof(response));
    }
}
