#include "support.h"
#include <android/log.h>

#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, "SOCKET", __VA_ARGS__)

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
    if (pid == 0) {
        printf("\nGame is not running");
        Close();
        return 0;
    }

    uintptr_t base = FindLibrary("libil2cpp.so", 1);

    Request request{};
    Response response{};

    while ((receive((void *) &request) > 0)) {
        response.Success = false;
        response.PlayerCount = 0;

        uintptr_t GameFacadeBase = Read<uintptr_t>(base + 0xC21A778);
        uintptr_t GameFacade_c = Read<uintptr_t>(GameFacadeBase + 0xB8);
        uintptr_t GameFacade = Read<uintptr_t>(GameFacade_c);
        uintptr_t match = Read<uintptr_t>(GameFacade + 0x90);
        if (match) {
            uintptr_t localPlayer = Read<uintptr_t>(match + 0xb0);
            if (localPlayer) {
                uintptr_t followcam = Read<uintptr_t>(localPlayer + 0x5b0);
                uintptr_t cambase = Read<uintptr_t>(followcam + 0x28);
                uintptr_t cam = Read<uintptr_t>(cambase + 0x10);
                response.matrix = Read<D3DMatrix>(cam + 0xD8);
                uintptr_t maincamtf = Read<uintptr_t>(localPlayer + 0x320);
                Vector3 myPos = GetPosition(maincamtf);
                uintptr_t entitybase = Read<uintptr_t>(match + 0x100);
                uintptr_t entity = Read<uintptr_t>(entitybase + 0x28) + 0x20;
                int entityCount = Read<int>(Read<uintptr_t>(entitybase + 0x28) + 0x18);

                if (entityCount <= 0) {
                    send((void *) &response, sizeof(response));
                    continue;
                }

                for (int i = 0; i < entityCount; i++) {
                    uintptr_t enemy = Read<uintptr_t>(entity + sizeof(uintptr_t) * i);

                    if (!enemy || enemy == localPlayer || Read<bool>(enemy + 0x74))
                        continue;

                    uintptr_t AvatarManager = Read<uintptr_t>(enemy + 0x690);
                    if (!AvatarManager)
                        continue;

                    uintptr_t UmaAvatarSimple = Read<uintptr_t>(AvatarManager + 0x118);
                    if (!UmaAvatarSimple)
                        continue;

                    bool IsVisible = Read<bool>(UmaAvatarSimple + 0xD8);
                    if (!IsVisible)
                        continue;

                    uintptr_t UmaData = Read<uintptr_t>(UmaAvatarSimple + 0x20);
                    if (!UmaData)
                        continue;

                    if (Read<bool>(UmaData + 0x79))
                        continue;

                    uintptr_t GetCurHP = Read<uintptr_t>(enemy + 0x68);
                    uintptr_t GetCur = Read<uintptr_t>(GetCurHP + 0x10);
                    uintptr_t HP = Read<uintptr_t>(GetCur + 0x20);

                    PlayerData *data = &response.Players[response.PlayerCount];

                    data->Health = (int) Read<short>(HP + 0x18);

                    uintptr_t HHCBNAPCKHF = Read<uintptr_t>(enemy + 0x1ee0);
                    uintptr_t maybeDead = Read<uintptr_t>(HHCBNAPCKHF + 0x18);
                    if (maybeDead && Read<int>(maybeDead + 0x10) == 8) {
                        data->isKnocked = true;
                    } else {
                        data->isKnocked = false;
                    }

                    uintptr_t HeadNode = Read<uintptr_t>(enemy + 0x5C0);
                    uintptr_t HeadTF = Read<uintptr_t>(HeadNode + 0x10);
                    data->HeadPos = GetPosition(HeadTF);

                    uintptr_t RootNode = Read<uintptr_t>(enemy + 0x5e8);
                    uintptr_t RootTF = Read<uintptr_t>(RootNode + 0x10);
                    data->RootPos = GetPosition(RootTF);

                    data->Distance = Vector3::Distance(myPos, GetPosition(HeadTF));

                    data->isBot = Read<bool>(enemy + 0x3D8);

                    memset(data->Name, 0, 64);
                    if (data->isBot) {
                        strcpy(data->Name, "Robot");
                    } else {
                        uintptr_t NamePtr = Read<uintptr_t>(enemy + 0x3d0);
                        if (NamePtr > 0x10000000) {
                            getUTF8(data->Name, NamePtr);
                        }

                        if (data->Name[0] == '\0')
                            strcpy(data->Name, "Enemy");
                    }

                    response.PlayerCount++;
                }
            }
        }

        if (response.PlayerCount > 0) {
            response.Success = true;
        }
        send((void *) &response, sizeof(response));
    }
}
