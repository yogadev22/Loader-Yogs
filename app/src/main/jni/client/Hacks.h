#ifndef DESI_IMPORTANT_HACK_H
#define DESI_IMPORTANT_HACK_H

#include "socket.h"
#include "struct.h"
#include "ESP.h"

Request request;
Response response;

bool isPlayerLinee;

static auto WorldToScreen(D3DMatrix viewMatrix, Vector3 ScreenPos, int g_screenWidth, int g_screenHeight) {
    auto result = Vector3(-1, -1, -1);

    auto v9 = (ScreenPos.X * viewMatrix._11) + (ScreenPos.Y * viewMatrix._21) + (ScreenPos.Z * viewMatrix._31) + viewMatrix._41;
    auto v10 = (ScreenPos.X * viewMatrix._12) + (ScreenPos.Y * viewMatrix._22) + (ScreenPos.Z * viewMatrix._32) + viewMatrix._42;
    auto v12 = (ScreenPos.X * viewMatrix._14) + (ScreenPos.Y * viewMatrix._24) + (ScreenPos.Z * viewMatrix._34) + viewMatrix._44;

    if (v12 >= 0.001f) {
        auto v13 = (float)g_screenWidth / 2.0f;
        auto v14 = (float)g_screenHeight / 2.0f;

        result.X = v13 + (v13 * v9) / v12;
        result.Y = v14 - (v14 * v10) / v12;
    }
    return result;
}

void DrawESP(ESP esp, int screenWidth, int screenHeight) {
    int botCount = 0, playerCount = 0;
    request.Mode = 1;

    send((void *) &request, sizeof(request));
    receive((void *) &response);

    if (response.Success) {
        float x, y;

        for (int i = 0; i < response.PlayerCount; i++) {
            PlayerData player = response.Players[i];

            Vector3 HeadLocation = WorldToScreen(response.matrix, response.Players[i].HeadPos,
                                                 screenWidth, screenHeight);

            x = HeadLocation.X;
            y = HeadLocation.Y;

            if (player.isBot) {
                botCount++;
            } else {
                playerCount++;
            }

            //if (HeadLocation.Z != 1) {
                if (isPlayerLinee) {
                    esp.DrawLine(Color(255, 255, 255, 255), 1.0f, Vector2(screenWidth / 2, 0),
                                 Vector2(x, y));
                }
            //}
        }
    }

    int ENEM_ICON = 2;
    int BOT_ICON = 3;

    if (playerCount == 0) {
        ENEM_ICON = 0;
    }
    if (botCount == 0) {
        BOT_ICON = 1;
    }

    char cn[10];
    sprintf(cn, "%d", playerCount);

    char bt[10];
    sprintf(bt, "%d", botCount);


    esp.DrawOTH(Vector2(screenWidth / 2 - (80), 60), ENEM_ICON);
    esp.DrawOTH(Vector2(screenWidth / 2, 60), BOT_ICON);

    esp.DrawText(Color(255, 255, 255, 255), cn, Vector2(screenWidth / 2 - (20), 87), 23);

    esp.DrawText(Color(255, 255, 255, 255), bt, Vector2(screenWidth / 2 + (50), 87), 23);
}
#endif // DESI_IMPORTANT_HACK_H
