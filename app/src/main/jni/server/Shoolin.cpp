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

        uintptr_t GameFacade = Read<uintptr_t>(base + 0xC2132B8);
        if (GameFacade) {
            auto P2 = Read<uintptr_t>(GameFacade + 0xB8);
            if (P2) {
                auto BaseGame = Read<uintptr_t>(P2);
                if (BaseGame) {
                    auto m_Match = Read<uintptr_t>(BaseGame + 0x90);
                    if (m_Match) {
                        auto MatchIsRunning = Read<int>(m_Match + 0xA4);
                        if (MatchIsRunning != 4 && MatchIsRunning != 0 && MatchIsRunning == 1) {
                            auto localPlayer = Read<uintptr_t>(m_Match + 0xb0);
                            if (localPlayer != 0) {
                                auto FollowCamera = Read<uintptr_t>(localPlayer + 0x5b0);
                                if (FollowCamera != 0) {
                                    auto Camera = Read<uintptr_t>(FollowCamera + 0x28);
                                    if (Camera != 0) {
                                        auto IntPtrCam = Read<uintptr_t>(Camera + 0x10);
                                        if (IntPtrCam != 0) {
                                            response.matrix = Read<D3DMatrix>(IntPtrCam + 0xD8);

                                            auto dictionary = Read<MonoDictionary *>(
                                                    BaseGame + 0xc0);
                                            if (dictionary) {

                                                Vector3 myPos = GetNodePosition(
                                                        GetPlayerHeadTF(localPlayer));

                                                int numValues = dictionary->getNumValues();
                                                for (int x = 0; x < numValues; x++) {
                                                    auto enemyList = Read<uintptr_t>(
                                                            dictionary->getValues() + 0x8 * x);
                                                    if (enemyList != 0) {
                                                        PlayerData *player = &response.Players[response.PlayerCount];
                                                        player->isBot = Read<bool>(
                                                                enemyList + 0x3d8);
                                                        player->HeadPos = GetNodePosition(
                                                                GetPlayerHeadTF(enemyList));
                                                        player->Distance = getDistance(myPos,
                                                                                       player->HeadPos);
                                                        response.PlayerCount++;
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
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
