#include "support.h"
bool bLobby;

void *bypass(void *) {
    uintptr_t anogs = 0;

    while (!anogs) {
        anogs = FindLibrary("libanogs.so", 1);
        sleep(1);
    }

    if (!bLobby) {
        ApplyPatch(anogs + oxorany(0x320A48), oxorany("00 00 80 D2 C0 03 5F D6"));
        ApplyPatch(anogs + oxorany(0x4B5F00), oxorany("00 00 80 D2 C0 03 5F D6"));
        ApplyPatch(anogs + oxorany(0x4B5F08), oxorany("00 00 80 D2 C0 03 5F D6"));
        ApplyPatch(anogs + oxorany(0x1EC088), oxorany("00 00 80 D2 C0 03 5F D6"));
        ApplyPatch(anogs + oxorany(0x203F90), oxorany("00 00 80 D2 C0 03 5F D6"));
        ApplyPatch(anogs + oxorany(0x2029C8), oxorany("00 00 80 D2 C0 03 5F D6"));
        ApplyPatch(anogs + oxorany(0x2031DC), oxorany("00 00 80 D2 C0 03 5F D6"));
        ApplyPatch(anogs + oxorany(0x21283C), oxorany("00 00 80 D2 C0 03 5F D6"));
        ApplyPatch(anogs + oxorany(0x244550), oxorany("00 00 80 D2 C0 03 5F D6"));
        ApplyPatch(anogs + oxorany(0x25C17C), oxorany("00 00 80 D2 C0 03 5F D6"));
        ApplyPatch(anogs + oxorany(0x265C0C), oxorany("00 00 80 D2 C0 03 5F D6"));
        ApplyPatch(anogs + oxorany(0x266C40), oxorany("00 00 80 D2 C0 03 5F D6"));
        ApplyPatch(anogs + oxorany(0x268064), oxorany("00 00 80 D2 C0 03 5F D6"));
        ApplyPatch(anogs + oxorany(0x27CE88), oxorany("00 00 80 D2 C0 03 5F D6"));
        ApplyPatch(anogs + oxorany(0x27E8CC), oxorany("00 00 80 D2 C0 03 5F D6"));
        ApplyPatch(anogs + oxorany(0x27ED10), oxorany("00 00 80 D2 C0 03 5F D6"));
        ApplyPatch(anogs + oxorany(0x2C0C20), oxorany("00 00 80 D2 C0 03 5F D6"));
        ApplyPatch(anogs + oxorany(0x2C1164), oxorany("00 00 80 D2 C0 03 5F D6"));
        ApplyPatch(anogs + oxorany(0x2D0CA4), oxorany("00 00 80 D2 C0 03 5F D6"));
        ApplyPatch(anogs + oxorany(0x2D4D10), oxorany("00 00 80 D2 C0 03 5F D6"));
        ApplyPatch(anogs + oxorany(0x2E4E98), oxorany("00 00 80 D2 C0 03 5F D6"));
        ApplyPatch(anogs + oxorany(0x2E5EA0), oxorany("00 00 80 D2 C0 03 5F D6"));
        ApplyPatch(anogs + oxorany(0x447698), oxorany("00 00 80 D2 C0 03 5F D6"));
        ApplyPatch(anogs + oxorany(0x448A88), oxorany("00 00 80 D2 C0 03 5F D6"));
        ApplyPatch(anogs + oxorany(0x4493AC), oxorany("00 00 80 D2 C0 03 5F D6"));
        ApplyPatch(anogs + oxorany(0x44A010), oxorany("00 00 80 D2 C0 03 5F D6"));
        ApplyPatch(anogs + oxorany(0x44B8B0), oxorany("00 00 80 D2 C0 03 5F D6"));
        ApplyPatch(anogs + oxorany(0x497A64), oxorany("00 00 80 D2 C0 03 5F D6"));
        ApplyPatch(anogs + oxorany(0x36E384), oxorany("00 00 80 D2 C0 03 5F D6")); //Segment Check
        ApplyPatch(anogs + oxorany(0x453188), oxorany("00 00 80 D2 C0 03 5F D6")); //Memory Check
        ApplyPatch(anogs + oxorany(0x362E98), oxorany("00 00 80 D2 C0 03 5F D6")); //Day Fix
        ApplyPatch(anogs + oxorany(0x4B9EBC), oxorany("00 00 80 D2 C0 03 5F D6")); //Integrity Check
        ApplyPatch(anogs + oxorany(0x47BC40), oxorany("00 00 80 D2 C0 03 5F D6"));
        bLobby = true;
    }
    
    return nullptr;
}

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
        Close();
        return 0;
    }
    
    char lj[64];
	sprintf(lj, "/proc/%d/mem", pid);
	handle = open(lj, O_RDWR);
	if (handle == -1) {
		 exit(1);
	}
    
    ApplyPatch(base + oxorany(0xa0ac2d8), oxorany("00 00 80 D2 C0 03 5F D6"));
    ApplyPatch(base + oxorany(0x6da90c8), oxorany("00 00 80 D2 C0 03 5F D6")); //LogEventTypeBannedPlayerAppeal
    ApplyPatch(base + oxorany(0x6da0414), oxorany("00 00 80 D2 C0 03 5F D6")); //LogClanReport
    ApplyPatch(base + oxorany(0x6d84f88), oxorany("00 00 80 D2 C0 03 5F D6")); //LogReportCheatInHistory
    ApplyPatch(base + oxorany(0x5e08578), oxorany("00 00 80 D2 C0 03 5F D6")); //GetBanReasonDesc
    ApplyPatch(base + oxorany(0x6359a44), oxorany("00 00 80 D2 C0 03 5F D6")); //Report
    ApplyPatch(base + oxorany(0x635a664), oxorany("00 00 80 D2 C0 03 5F D6")); //Report
    ApplyPatch(base + oxorany(0x6da41e0), oxorany("00 00 80 D2 C0 03 5F D6")); //CacheMinorVoiceReport
    ApplyPatch(base + oxorany(0x6d84580), oxorany("00 00 80 D2 C0 03 5F D6")); //LogReportCheat
    ApplyPatch(base + oxorany(0x6da3bc4), oxorany("00 00 80 D2 C0 03 5F D6")); //LogEventTypeAntiAddictionBanAck_IP_US
    ApplyPatch(base + oxorany(0x6da3de0), oxorany("00 00 80 D2 C0 03 5F D6")); //CacheMinorChatReport
    
    Request request{};
    Response response{};
    
    pthread_t t;
    pthread_create(&t, nullptr, bypass, nullptr);

    while ((receive((void *) &request) > 0)) {
        screenWidth = request.screenWidth;
        screenHeight = request.screenHeight;
        response.Success = false;
        response.PlayerCount = 0;
        
        if (request.options.ResetGuest) {
            ApplyPatch(base + oxorany(0x635efdc), oxorany("20 00 80 D2 C0 03 5F D6"));
        } else {
            RestorePatch(base + oxorany(0x635efdc));
        }
        
        if (request.options.GhostHack) {
            ApplyPatch(base + oxorany(0x70243a4), oxorany("00 00 80 D2 C0 03 5F D6"));
        } else {
            RestorePatch(base + oxorany(0x70243a4));
        }
           
        float nearest = -1.0f;
        uintptr_t GameFacadeBase = Read<uintptr_t>(base + 0xA989BA0);
        uintptr_t GameFacade_c = Read<uintptr_t>(GameFacadeBase + 0xB8);
        uintptr_t MatchGame = Read<uintptr_t>(GameFacade_c + 0x8);
        uintptr_t match = Read<uintptr_t>(MatchGame + 0x90);
        if (match) {
            uintptr_t localPlayer = Read<uintptr_t>(match + 0xd8);
            if (localPlayer) {
                uintptr_t TimeSerive = Read<uintptr_t>(MatchGame + 0x20);
                if (TimeSerive) {
                    if (request.options.SpeedHack) {
                        Write<float>(TimeSerive + 0x2C, 0.05900000036f);
                    } else {
                        Write<float>(TimeSerive + 0x2C, 0.03299999982f);
                    }
                }
                    
                uintptr_t followcam = Read<uintptr_t>(localPlayer + 0x638);
                uintptr_t cambase = Read<uintptr_t>(followcam + 0x30);
                uintptr_t cam = Read<uintptr_t>(cambase + 0x10);
                
                Matrix4x4 ViewMatrix = Read<Matrix4x4>(cam + 0x80);
                Matrix4x4 ProjMatrix = Read<Matrix4x4>(cam + 0xC0);
                Matrix4x4 _vMatrix = ProjMatrix * ViewMatrix;
                    
                Vector3 targetAimPos = Vector3::Zero();
                bool isFiring = Read<bool>(localPlayer + 0x7e0);
    
                Vector3 myPos = GetPosition(Read<uintptr_t>(Read<uintptr_t>(localPlayer + 0x648) + 0x10));
                
                static bool saved = false;
                
                uintptr_t lroottf = Read<uintptr_t>(Read<uintptr_t>(localPlayer + 0x670) + 0x10);
                uintptr_t transformObjValue = Read<uintptr_t>(lroottf + 0x10);
                uintptr_t matrixValue = Read<uintptr_t>(transformObjValue + 0x38);
                      
                if (request.options.flyhack && !Read<bool>(localPlayer + 0x7c)) {
                    if (!saved) {
                        savedPos = Read<Vector3>(matrixValue + 0x90);
                        saved = true;
                    }
                    
                    uintptr_t HHCBNAPCKHF = Read<uintptr_t>(localPlayer + 0x19f8);
                    uintptr_t maybeDead = Read<uintptr_t>(HHCBNAPCKHF + 0x20);
                    Write<int>(maybeDead + 0x10, 10);
                    
                    ApplyPatch(base + oxorany(0x69052d8), oxorany("20 00 80 D2 C0 03 5F D6"));
                    ApplyPatch(base + oxorany(0x72e6fd4), oxorany("20 00 80 D2 C0 03 5F D6"));
                    ApplyPatch(base + oxorany(0x9dd515c), oxorany("20 00 80 D2 C0 03 5F D6"));
                    ApplyPatch(base + oxorany(0x68a1fd4), oxorany("20 00 80 D2 C0 03 5F D6"));
                    
                    if (!flyThreadRunning) {
                        flyThreadRunning = true;
                        flyThread = std::thread(FlyLoop, localPlayer);
                        flyThread.detach();
                    }
                } else {
                    if (saved) {
                        flyThreadRunning = false;
                        
                        Write<Vector3>(matrixValue + 0x90, savedPos);
                        
                        saved = false;
                        
                        RestorePatch(base + oxorany(0x69052d8));
                        RestorePatch(base + oxorany(0x72e6fd4));
                        RestorePatch(base + oxorany(0x9dd515c));
                        RestorePatch(base + oxorany(0x68a1fd4));
                    }
                }
    
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
                        
                    if (request.options.skipbot && Read<bool>(enemy + 0x450))
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
                    Vector3 RootPos = GetPosition(Read<uintptr_t>(Read<uintptr_t>(enemy + 0x670) + 0x10));
                        
                    data->Distance = Distance;
                    data->HeadPos = WorldToScreen(_vMatrix, HeadPos);
                    data->RootPos = WorldToScreen(_vMatrix, RootPos);
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

        if (response.PlayerCount > 0) {
            response.Success = true;
        }
        
        send((void *) &response, sizeof(response));
    }
}
