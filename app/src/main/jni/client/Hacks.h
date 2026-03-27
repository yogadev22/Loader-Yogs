#ifndef DESI_IMPORTANT_HACK_H
#define DESI_IMPORTANT_HACK_H

#include "socket.h"
#include "struct.h"
#include "ESP.h"

Request request;
Response response;

int isPlayerLine = 0, isPlayerBox = 0;
bool isPlayerLinee, isPlayerBoxx, isr360Alert, isPlayerHealth, isPlayerName, isPlayerDist, isNoBot;
Color clrEnemy, clrFilled, clrEdge, clrBox, clrSkeleton, clrHealth;
char extra[30];

Vector4 getPrecise(Vector3 head, Vector3 root)
{
    float height = fabs(head.Y - root.Y);
    float width = height * 0.5f; // rasio badan

    float centerX = (head.X + root.X) / 2.0f;

    float minX = centerX - width / 2.0f;
    float maxX = centerX + width / 2.0f;

    float minY = head.Y;
    float maxY = root.Y;

    return { minX, minY, maxX, maxY };
}

static auto WorldToScreen(D3DMatrix viewMatrix, Vector3 ScreenPos, int g_screenWidth, int g_screenHeight) {
    auto result = Vector3(-1, -1, -1);

    auto v9 = (ScreenPos.X * viewMatrix._11) + (ScreenPos.Y * viewMatrix._21) + (ScreenPos.Z * viewMatrix._31) + viewMatrix._41;
    auto v10 = (ScreenPos.X * viewMatrix._12) + (ScreenPos.Y * viewMatrix._22) + (ScreenPos.Z * viewMatrix._32) + viewMatrix._42;
    auto v12 = (ScreenPos.X * viewMatrix._14) + (ScreenPos.Y * viewMatrix._24) + (ScreenPos.Z * viewMatrix._34) + viewMatrix._44;
    auto v13 = (float)g_screenWidth / 2.0f;
    auto v14 = (float)g_screenHeight / 2.0f;

    result.X = v13 + (v13 * v9) / v12;
    result.Y = v14 - (v14 * v10) / v12;
    result.Z = v12;

    return result;
}

void DrawESP(ESP esp, int screenWidth, int screenHeight) {
    int botCount = 0, playerCount = 0;
    request.Mode = 1;

    send((void *) &request, sizeof(request));
    receive((void *) &response);

    float mScaleX = screenWidth / (float) 2340;
    float mScaleY = screenHeight / (float) 1080;
    Vector2 screen = Vector2(screenWidth, screenHeight);
    Vector2 center = Vector2(screenWidth / 2, screenHeight / 2);

    if (response.Success) {
        float x, y;

        float textsize = screenHeight / 50;

        float fov = 70.0f;

        for (int i = 0; i < response.PlayerCount; i++) {
            PlayerData player = response.Players[i];

            if (isNoBot && player.isBot)
                continue;

            Vector3 HeadLocation = WorldToScreen(response.matrix, response.Players[i].HeadPos,
                                                 screenWidth, screenHeight);

            Vector3 RootLocation = WorldToScreen(response.matrix, response.Players[i].RootPos,
                                                 screenWidth, screenHeight);

            x = HeadLocation.X;
            y = HeadLocation.Y;

            if (player.isBot) {
                botCount++;
                clrFilled = Color(255, 255, 255, 40);
                clrEnemy = Color(255, 255, 255, 255);
                clrEdge = Color(255, 255, 255, 150);
                clrSkeleton = Color(255, 255, 255, 200);
                clrBox = Color(255, 255, 255, 200);
            } else {
                playerCount++;
                clrFilled = Color(255, 0, 0, 40);
                clrEnemy = Color(255, 0, 0, 255);
                clrEdge = Color(255, 0, 0, 150);
                clrSkeleton = Color(255, 0, 0, 200);
                clrBox = Color(255, 0, 0, 200);
            }

            float dist = response.Players[i].Distance;
            float mx = screenWidth / (dist * 0.8f);
            float my = screenWidth / (dist * 0.5f);
            float top = y - my;
            float bottom = RootLocation.Y + my;

            if (HeadLocation.Z > 0) {
                if (isPlayerLinee) {
                    if (isPlayerLine == 0) {
                        esp.DrawLine(clrEnemy, (mScaleY * 1.6f),
                                     Vector2(center.X, (mScaleY * 118)), Vector2(x, y));
                    } else if (isPlayerLine == 1) {
                        esp.DrawLine(clrEnemy, (mScaleY * 1.6f), center,
                                     Vector2(x, y));
                    } else if (isPlayerLine == 2) {
                        esp.DrawLine(clrEnemy, (mScaleY * 1.6f), Vector2(center.X, screenHeight),
                                     Vector2(HeadLocation.X, bottom));
                    }
                }

                if (isPlayerBoxx) {
                    if (isPlayerBox == 0) {
                        esp.DrawRect(clrBox,
                                     screenHeight / 500, Vector2(x - mx, top),
                                     Vector2(x + mx, bottom));
                    } else if (isPlayerBox == 1) {
                        esp.DrawFilledRect(clrFilled,
                                           Vector2(x - mx, top),
                                           Vector2(x + mx, bottom));
                        esp.DrawRect(clrBox,
                                     screenHeight / 500, Vector2(x - mx, top),
                                     Vector2(x + mx, bottom));
                    }
                }

                if (isPlayerHealth) {
                    float healthLength = screenHeight / 30;

                    if (healthLength < mx)
                        healthLength = mx;

                    if (player.isKnocked) {
                        clrHealth = Color(255, 0, 0, 110);
                    } else {
                        clrHealth = Color(0, 255, 0, 110);
                    }

                    esp.DrawFilledRect(clrHealth,
                                       Vector2(x - healthLength,
                                            top - screenHeight / 110),
                                       Vector2(x - healthLength +
                                            (2 * healthLength) *
                                            response.Players[i].Health / 200,
                                            top - screenHeight / 225));
                    esp.DrawRect(Color(0, 0, 0), 1.5,
                                 Vector2(x - healthLength, top - screenHeight / 110),
                                 Vector2(x + healthLength, top - screenHeight / 225));
                }

                if (isPlayerName) {
                    int TeamID = 0;
                    esp.DrawName(Color(255, 255, 255, 255), response.Players[i].Name,
                                 TeamID,
                                 Vector2(HeadLocation.X,
                                         top - screenHeight / 65), screenHeight / 60);
                }

                if (isPlayerDist) {
                    sprintf(extra, "%0.0f M", response.Players[i].Distance);
                    esp.DrawText(Color(255, 255, 255, 255), extra,
                                 Vector2(x, bottom + screenHeight / 45),
                                 textsize);


                }
            }

            if (HeadLocation.Z < 0) {
                if (!isr360Alert)
                    continue;

                if (x > screenWidth - screenWidth / 12)
                    x = screenWidth - screenWidth / 120;
                else if (x < screenWidth / 120)
                    x = screenWidth / 12;

                if (y < screenHeight / 1) {
                    esp.DrawRect(Color(255, 255, 255), 2,
                                 Vector2(screenWidth - x - 100, screenHeight - 48),
                                 Vector2(screenWidth - x + 100, screenHeight + 2));
                    esp.DrawFilledRect(Color(255, 0, 0, 140),
                                       Vector2(screenWidth - x - 100, screenHeight - 48),
                                       Vector2(screenWidth - x + 100, screenHeight + 2));
                    sprintf(extra, "%0.0f m", response.Players[i].Distance);
                    esp.DrawText(Color(255, 255, 255, 255), extra,
                                 Vector2(screenWidth - x, screenHeight - 20),
                                 textsize);
                } else {
                    esp.DrawRect(Color(255, 255, 255), 2,
                                 Vector2(screenWidth - x - 100, 48),
                                 Vector2(screenWidth - x + 100, -2));
                    esp.DrawFilledRect(Color(255, 0, 0, 140),
                                       Vector2(screenWidth - x - 100, 48),
                                       Vector2(screenWidth - x + 100, -2));
                    sprintf(extra, "%0.0f m", response.Players[i].Distance);
                    esp.DrawText(Color(255, 255, 255, 255), extra,
                                 Vector2(screenWidth - x, 25), textsize);
                }
            } else if (x < -screenWidth / 10 || x > screenWidth + screenWidth / 10) {
                if (!isr360Alert)
                    continue;

                if (y > screenHeight - screenHeight / 12)
                    y = screenHeight - screenHeight / 120;
                else if (y < screenHeight / 120)
                    y = screenHeight / 12;

                if (x > screenWidth / 2) {
                    esp.DrawRect(Color(255, 255, 255), 2,
                                 Vector2(screenWidth - 88, y - 35),
                                 Vector2(screenWidth + 2, y + 35));
                    esp.DrawFilledRect(Color(255, 0, 0, 140),
                                       Vector2(screenWidth - 88, y - 35),
                                       Vector2(screenWidth + 2, y + 35));
                    sprintf(extra, "%0.0f m", response.Players[i].Distance);
                    esp.DrawText(Color(255, 255, 255, 255), extra,
                                 Vector2(screenWidth - screenWidth / 80, y + 10),
                                 textsize);
                } else {
                    esp.DrawRect(Color(255, 255, 255), 2,
                                 Vector2(0 + 88, y - 35), Vector2(0 - 2, y + 35));
                    esp.DrawFilledRect(Color(255, 0, 0, 140),
                                       Vector2(0 + 88, y - 35), Vector2(0 - 2, y + 35));
                    sprintf(extra, "%0.0f m", response.Players[i].Distance);
                    esp.DrawText(Color(255, 255, 255, 255), extra,
                                 Vector2(screenWidth / 80, y + 10), textsize);
                }
            } else if (y < -screenHeight / 10 || y > screenHeight + screenHeight / 10) {
                if (!isr360Alert)
                    continue;

                if (x > screenWidth - screenWidth / 12)
                    x = screenWidth - screenWidth / 120;
                else if (x < screenWidth / 120)
                    x = screenWidth / 12;

                if (y > screenHeight / 2.5) {
                    esp.DrawRect(Color(255, 255, 255), 2,
                                 Vector2(x - 100, screenHeight - 48), Vector2(x + 100,
                                                                              screenHeight + 2));
                    esp.DrawFilledRect(Color(255, 0, 0, 140),
                                       Vector2(x - 100, screenHeight - 48),
                                       Vector2(x + 100, screenHeight + 2));
                    sprintf(extra, "%0.0f m", response.Players[i].Distance);
                    esp.DrawText(Color(255, 255, 255, 255), extra,
                                 Vector2(x, screenHeight - 20), textsize);
                } else {
                    esp.DrawRect(Color(255, 255, 255), 2,
                                 Vector2(x - 100, 48), Vector2(x + 100, -2));
                    esp.DrawFilledRect(Color(255, 0, 0, 140),
                                       Vector2(x - 100, 48), Vector2(x + 100, -2));
                    sprintf(extra, "%0.0f m", response.Players[i].Distance);
                    esp.DrawText(Color(255, 255, 255, 255), extra,
                                 Vector2(x, 25), textsize);
                }
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
}
#endif // DESI_IMPORTANT_HACK_H
