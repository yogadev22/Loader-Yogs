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
        uintptr_t localPlayer = Read<uintptr_t>(match + 0xb0);
        uintptr_t LocalHeadNode = Read<uintptr_t>(localPlayer + 0x5C0);
        uintptr_t LocalHeadTF = Read<uintptr_t>(LocalHeadNode + 0x10);
        Vector3 LocalHeadPos = GetPosition(LocalHeadTF);

        uintptr_t followcam = Read<uintptr_t>(localPlayer + 0x5b0);
        uintptr_t cambase = Read<uintptr_t>(followcam + 0x28);
        uintptr_t cam = Read<uintptr_t>(cambase + 0x10);
        response.matrix = Read<D3DMatrix>(cam + 0xD8);
            

        uintptr_t entitybase = Read<uintptr_t>(GameFacade + 0xC0);
        uintptr_t entity = Read<uintptr_t>(entitybase + 0x28) + 0x20;
        int entityCount = Read<int>(entitybase + 0x38);

        if (entityCount <= 0) {
            send((void *) &response, sizeof(response));
            continue;
        }

        for (int i = 0; i < entityCount; i++) {
            uintptr_t enemy = Read<uintptr_t>(
                    entity + sizeof(uintptr_t) * i);
            if (!enemy || Read<bool>(enemy + 0x74))
                continue;
            PlayerData *data = &response.Players[response.PlayerCount];
            uintptr_t HeadNode = Read<uintptr_t>(
                    enemy + 0x5C0);
            uintptr_t HeadTF = Read<uintptr_t>(
                    HeadNode + 0x10);
            data->HeadPos = GetPosition(HeadTF);
            data->Distance = getDistance(LocalHeadPos,
                                         data->HeadPos = GetPosition(
                                                 HeadTF));
            data->isBot = Read<bool>(enemy + 0x3D8);
        }

        if (response.PlayerCount > 0) {
            response.Success = true;
        }
        send((void *) &response, sizeof(response));
    }
}
