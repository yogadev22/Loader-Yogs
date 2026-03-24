#include "support.h"

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

    uintptr_t base = getBase();

    Request request{};
    Response response{};

    while ((receive((void *) &request) > 0)) {
        response.Success = false;
        response.PlayerCount = 0;

        uintptr_t GameFacade = 0;
        uintptr_t GameFacadeBase = Read<uintptr_t>(base + 0xC3328F8);
        if (GameFacadeBase) {
            uintptr_t GameFacadeBase_c = Read<uintptr_t>(GameFacadeBase);
            if (GameFacadeBase_c) {
                uintptr_t GameFacade_c = Read<uintptr_t>(GameFacadeBase_c + 0xB8);
                if (GameFacade_c) {
                    GameFacade = Read<uintptr_t>(GameFacade_c);
                }
            }
        }

        uintptr_t localPlayer = 0;

        if (GameFacade) {
            uintptr_t match = Read<uintptr_t>(GameFacade + 0x90);
            if (match) {
                localPlayer = Read<uintptr_t>(match + 0xb0);
            }
        }

        if (localPlayer) {
            uintptr_t LocalHeadNode = Read<uintptr_t>(localPlayer + 0x5C0);
            uintptr_t LocalHeadTF = Read<uintptr_t>(LocalHeadNode + 0x10);
            Vector3 LocalHeadPos = GetPosition(LocalHeadTF);

            uintptr_t followcam = Read<uintptr_t>(localPlayer + 0x5b0);
            if (followcam) {
                uintptr_t cambase = Read<uintptr_t>(followcam + 0x28);
                if (cambase) {
                    uintptr_t cam = Read<uintptr_t>(cambase + 0x10);
                    if (cam) {
                        response.matrix = Read<D3DMatrix>(cam + 0xD8);
                    }
                }
            }

            uintptr_t entitybase = Read<uintptr_t>(GameFacade + 0xC0);
            if (entitybase) {
                uintptr_t entity = Read<uintptr_t>(entitybase + 0x28) + 0x20;
                int entityCount = Read<int>(entitybase + 0x38);

                if (entityCount <= 0) {
                    send((void *) &response, sizeof(response));
                    continue;
                }

                for (int i = 0; i < entityCount; i++) {
                    uintptr_t enemy = Read<uintptr_t>(entity + sizeof(uintptr_t) * i);
                    if (!enemy || enemy == localPlayer || Read<bool>(enemy + 0x74))
                        continue;

                    PlayerData *data = &response.Players[response.PlayerCount];

                    uintptr_t HeadNode = Read<uintptr_t>(enemy + 0x5C0);
                    uintptr_t HeadTF = Read<uintptr_t>(HeadNode + 0x10);
                    data->HeadPos = GetPosition(HeadTF);
                    data->Distance = getDistance(LocalHeadPos, data->HeadPos = GetPosition(HeadTF));
                    data->isBot = Read<bool>(enemy + 0x3D8);

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
