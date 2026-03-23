#ifndef DESI_IMPORTANT_HACK_H
#define DESI_IMPORTANT_HACK_H

#include "socket.h"

float x, y;
Request request;
Response response;
char extra[30];
int botCount, playerCount;
Color clrEnemy, clrFilled, clrEdge, clrBox, clrSkeleton, clrHealth;
Options options { 1, -1, false, 1, false, 201, false };

Vector3 WorldToScreen(FMatrix viewMatrix, Vector3 pos, int width, int height) {
    Vector3 result(-1, -1, -1);

    Vector3 XAxis(viewMatrix.M[0][0], viewMatrix.M[0][1], viewMatrix.M[0][2]);
    Vector3 YAxis(viewMatrix.M[1][0], viewMatrix.M[1][1], viewMatrix.M[1][2]);
    Vector3 ZAxis(viewMatrix.M[2][0], viewMatrix.M[2][1], viewMatrix.M[2][2]);

    Vector3 vDelta = pos - response.camLocation;
    Vector3 vTransformed(Vector3::Dot(vDelta, YAxis), Vector3::Dot(vDelta, ZAxis), Vector3::Dot(vDelta, XAxis));

    float fov = response.fov;
    float screenCenterX = width / 2.0f;
    float screenCenterY = height / 2.0f;

    result.X = (screenCenterX + vTransformed.X * (screenCenterX / tanf(fov * ((float)3.141592653589793238 / 360.0f))) / vTransformed.Z);
    result.Y = (screenCenterY + vTransformed.Y * (screenCenterX / tanf(fov * ((float)3.141592653589793238 / 360.0f))) / vTransformed.Z);
    result.Z = vTransformed.Z;

    return result;
}

bool isPositionValid(Vector2 position) {
    return (position.X > request.ScreenWidth || position.Y > request.ScreenHeight || position.X < 0.0f || position.Y < 0.0f);
}

bool isDotUnderCircle(Vector2 pos, float radius) {
    float diffX = abs(pos.X), diffY = abs(pos.Y);
    return (diffX * diffX + diffY * diffY) <= radius;
}

int isOutsideSafezone(Vector3 pos, Vector3 screen) {
    Vector2 mSafezoneTopLeft(screen.X * 0.04f, screen.Y * 0.04f);
    Vector2 mSafezoneBottomRight(screen.X * 0.96f, screen.Y * 0.96f);

    int result = 0;
    if (pos.Y < mSafezoneTopLeft.Y) {
        result |= 1;
    }
    if (pos.X > mSafezoneBottomRight.X) {
        result |= 2;
    }
    if (pos.Y > mSafezoneBottomRight.Y) {
        result |= 4;
    }
    if (pos.X < mSafezoneTopLeft.X) {
        result |= 8;
    }
    return result;
}

Vector2 pushToScreenBorder(Vector2 Pos, Vector2 screen, int borders, int offset) {
    int x = (int) Pos.X;
    int y = (int) Pos.Y;
    if ((borders & 1) == 1) {
        y = 0 - offset;
    }
    if ((borders & 2) == 2) {
        x = (int) screen.X + offset;
    }
    if ((borders & 4) == 4) {
        y = (int) screen.Y + offset;
    }
    if ((borders & 8) == 8) {
        x = 0 - offset;
    }
    return Vector2(x, y);
}

void DrawESP(ESP esp, int screenWidth, int screenHeight) {
    botCount = 0;
    playerCount = 0;
    request.ScreenHeight = screenHeight;
    request.ScreenWidth = screenWidth;
    request.options = options;
    request.Mode = InitMode;

    send((void *) &request, sizeof(request));
    receive((void *) &response);

    float mScaleX = screenWidth / (float) 2340;
    float mScaleY = screenHeight / (float) 1080;
    Vector2 screen = Vector2(screenWidth, screenHeight);
    Vector2 center = Vector2(screenWidth / 2, screenHeight / 2);

    esp.DrawTextName(Color(255, 0, 0, 255), Vector2(210, 50), (mScaleY * 35), response.Identified);

    if (response.Success) {
        if (options.openState == 0) {
            esp.DrawCircle(Color(250, 0, 0), center, request.options.aimingRange, (mScaleY * 1.4f));
        }

        float textsize = screenHeight / 50;

        for (int i = 0; i < response.PlayerCount; i++) {
            PlayerData player = response.Players[i];

            response.Players[i].HeadLocation = WorldToScreen(response.matrix, response.Players[i].Bone.head, screenWidth, screenHeight);

            x = response.Players[i].HeadLocation.X;
            y = response.Players[i].HeadLocation.Y;

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

            if (isNoBot && player.isBot) continue;

            float magic_number = (response.Players[i].Distance * response.fov);
            float mx = (screenWidth / 4) / magic_number;
            float my = (screenWidth / 1.38) / magic_number;
            float top = y - my + (screenWidth / 1.7) / magic_number;
            float bottom = response.Players[i].Bone.lAn.Y + my - (screenWidth / 1.7) / magic_number;

            if (response.Players[i].HeadLocation.Z != 1) {
                if (x > -50 && x < screenWidth + 50) {
                    if (isSkelton && player.Bone.isBone) {
                        float skelSize = (mScaleY * 3.5f);
                        float headsize = (mScaleY * 7.0f);
                        float distanceFromCamera = player.Distance;
                        float minHeadSize = (mScaleY * 2.0f);
                        // Decrease headsize by 0.1 for every unit beyond 10 meters, with a minimum size of 4.0
                        headsize = std::max(minHeadSize, headsize - std::min(
                                (distanceFromCamera - 10.0f) * 0.1f, 0.1f));
                        float boxWidth = 7.f - response.Players[i].Distance * 0.03;
                        esp.DrawCircle(clrSkeleton, Vector2(player.Bone.head.X, player.Bone.head.Y),
                                       boxWidth, 2);
                        //std::cout << "Dynamic Head Size: " << headsize << " - Distance: " << distanceFromCamera << "m" << std::endl;
                        esp.DrawLine(clrSkeleton, skelSize,
                                     Vector2(player.Bone.head.X, player.Bone.head.Y),
                                     Vector2(player.Bone.neck.X, player.Bone.neck.Y));
                        esp.DrawLine(clrSkeleton, skelSize,
                                     Vector2(player.Bone.neck.X, player.Bone.neck.Y),
                                     Vector2(player.Bone.cheast.X, player.Bone.cheast.Y));
                        esp.DrawLine(clrSkeleton, skelSize,
                                     Vector2(player.Bone.cheast.X, player.Bone.cheast.Y),
                                     Vector2(player.Bone.pelvis.X, player.Bone.pelvis.Y));
                        esp.DrawLine(clrSkeleton, skelSize,
                                     Vector2(player.Bone.neck.X, player.Bone.neck.Y),
                                     Vector2(player.Bone.lSh.X, player.Bone.lSh.Y));
                        esp.DrawLine(clrSkeleton, skelSize,
                                     Vector2(player.Bone.neck.X, player.Bone.neck.Y),
                                     Vector2(player.Bone.rSh.X, player.Bone.rSh.Y));
                        esp.DrawLine(clrSkeleton, skelSize,
                                     Vector2(player.Bone.lSh.X, player.Bone.lSh.Y),
                                     Vector2(player.Bone.lElb.X, player.Bone.lElb.Y));
                        esp.DrawLine(clrSkeleton, skelSize,
                                     Vector2(player.Bone.rSh.X, player.Bone.rSh.Y),
                                     Vector2(player.Bone.rElb.X, player.Bone.rElb.Y));
                        esp.DrawLine(clrSkeleton, skelSize,
                                     Vector2(player.Bone.lElb.X, player.Bone.lElb.Y),
                                     Vector2(player.Bone.lWr.X, player.Bone.lWr.Y));
                        esp.DrawLine(clrSkeleton, skelSize,
                                     Vector2(player.Bone.rElb.X, player.Bone.rElb.Y),
                                     Vector2(player.Bone.rWr.X, player.Bone.rWr.Y));
                        esp.DrawLine(clrSkeleton, skelSize,
                                     Vector2(player.Bone.pelvis.X, player.Bone.pelvis.Y),
                                     Vector2(player.Bone.lTh.X, player.Bone.lTh.Y));
                        esp.DrawLine(clrSkeleton, skelSize,
                                     Vector2(player.Bone.pelvis.X, player.Bone.pelvis.Y),
                                     Vector2(player.Bone.rTh.X, player.Bone.rTh.Y));
                        esp.DrawLine(clrSkeleton, skelSize,
                                     Vector2(player.Bone.lTh.X, player.Bone.lTh.Y),
                                     Vector2(player.Bone.lKn.X, player.Bone.lKn.Y));
                        esp.DrawLine(clrSkeleton, skelSize,
                                     Vector2(player.Bone.rTh.X, player.Bone.rTh.Y),
                                     Vector2(player.Bone.rKn.X, player.Bone.rKn.Y));
                        esp.DrawLine(clrSkeleton, skelSize,
                                     Vector2(player.Bone.lKn.X, player.Bone.lKn.Y),
                                     Vector2(player.Bone.lAn.X, player.Bone.lAn.Y));
                        esp.DrawLine(clrSkeleton, skelSize,
                                     Vector2(player.Bone.rKn.X, player.Bone.rKn.Y),
                                     Vector2(player.Bone.rAn.X, player.Bone.rAn.Y));
                    }

                    if (isPlayerBoxx) {
                        if (isPlayerBox == 0) {
                            esp.DrawRect(clrBox, (mScaleY * 3.5f),
                                         Vector2(player.Precise.X, player.Precise.Y),
                                         Vector2(player.Precise.Z, player.Precise.W));
                            esp.DrawFilledRect(clrFilled, Vector2(player.Precise.X, player.Precise.Y),
                                               Vector2(player.Precise.Z, player.Precise.W));
                        } else if (isPlayerBox == 1) {
                            esp.DrawRect(clrBox, (mScaleY * 3.5f),
                                         Vector2(player.Precise.X, player.Precise.Y),
                                         Vector2(player.Precise.Z, player.Precise.W));
                        }
                    }

                    if (isPlayerLinee) {
                        if (isPlayerLine == 0) {
                            esp.DrawLine(clrEnemy, (mScaleY * 1.6f),
                                         Vector2(center.X, (mScaleY * 118)), Vector2(x, y));
                        } else if (isPlayerLine == 1) {
                            esp.DrawLine(clrEnemy, (mScaleY * 1.6f), center,
                                         Vector2(player.Bone.head.X, player.Precise.Y));
                        } else if (isPlayerLine == 2) {
                            esp.DrawLine(clrEnemy, (mScaleY * 1.6f), Vector2(center.X, screenHeight),
                                         Vector2(player.Bone.head.X, bottom));
                        }
                    }

                    float boxCenterX = (player.Precise.X + player.Precise.Z) / 2;

                    if (isPlayerHealth) {
                        float healthLength = screenHeight / 13;
                        float cicing = (mScaleY * 112);
                        if (healthLength < mx)
                            healthLength = mx;
                        if (player.isKnocked) {
                            clrHealth = Color(255, 0, 0, 110);
                        } else {
                            clrHealth = Color(0, 255, 0, 110);
                        }

                        esp.DrawFilledRect(clrHealth,
                                           Vector2(x - healthLength, top - screenHeight / 30),
                                           Vector2(x - healthLength +
                                                (2 * healthLength) *
                                                response.Players[i].Health / 100,
                                                top - screenHeight / 110));
                        esp.DrawRect(Color(20, 20, 20), 1,
                                     Vector2(x - healthLength, top - screenHeight / 30),
                                     Vector2(x + healthLength, top - screenHeight / 110));
                    }

                    if (isPlayerName) {
                        esp.DrawName(Color(255, 255, 255, 255), response.Players[i].PlayerNameByte,
                                     response.Players[i].TeamID,
                                     Vector2(response.Players[i].HeadLocation.X,
                                          top - screenHeight / 65), screenHeight / 60);
                    }

                    if (isPlayerDist) {
                        sprintf(extra, "%0.0f M", response.Players[i].Distance);
                        esp.DrawText(Color(255, 255, 255, 255), extra,
                                     Vector2(x, bottom + screenHeight / 45),
                                     textsize);


                    }

                    if (isEnemyWeapon) {
                        esp.DrawWeapon(Color(255, 255, 255, 255), player.Weapon.id,
                                       player.Weapon.ammo, player.Weapon.maxammo,
                                       Vector2(x, bottom + screenHeight / 23), textsize);
                    }
                }
            }
            if (response.Players[i].HeadLocation.Z == 1.0f) {
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

        /*for (int i = 0; i < response.GrenadeCount; i++) {
            Color grenadeColor;
            GrenadeData grenade = response.Grenade[i];
            if (!isGrenadeWarning || grenade.Location.z == 1.0f) {
                continue;
            }
            const char *grenadeTypeText;
            switch (grenade.type) {
                case 1:
                    grenadeColor = Color::Red(255);
                    grenadeTypeText = "Grenade";
                    break;
                case 2:
                    grenadeColor = Color::Orange(255);
                    grenadeTypeText = "Molotov";
                    break;
                case 3:
                    grenadeColor = Color::Yellow(255);
                    grenadeTypeText = "Stun";
                    break;
                case 4:
                    grenadeColor = Color::White(255);
                    grenadeTypeText = "Smoke";
            }
            sprintf(extra, "%s (%0.0f m)", grenadeTypeText, grenade.Distance);
            esp.DrawText(grenadeColor, extra,
                         Vector2(grenade.Location.X, grenade.Location.Y + (screenHeight / 50)),
                         textsize);
            int WARNING = 4;
            esp.DrawOTH(Vector2(screenWidth / 2 - 160, 120), WARNING);
            esp.DrawText(Color(255, 255, 255), OBFUSCATE("Warning : Throwable"),
                         Vector2(screenWidth / 2 + 20, 145), 21);
            esp.DrawCircle(grenadeColor, Vector2(grenade.Location.X, grenade.Location.Y), 5, 4);
        }*/
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
