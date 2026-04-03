#ifndef DESI_IMPORTANT_HACK_H
#define DESI_IMPORTANT_HACK_H

#include "socket.h"
#include "struct.h"
#include "ESP.h"

Request request;
Response response;
Options options{false, 0, 0, false};

int isPlayerLine = 0, isPlayerBox = 0;
bool isPlayerLinee, isPlayerBoxx, isr360Alert, isPlayerHealth, isPlayerName, isPlayerDist, isNoBot;
Color clrEnemy, clrFilled, clrEdge, clrBox, clrSkeleton, clrHealth;
char extra[30];

void DrawESP(ESP esp, int screenWidth, int screenHeight) {
    if (connected) {
        int botCount = 0, playerCount = 0;
        request.screenWidth = screenWidth;
        request.screenHeight = screenHeight;
        request.options = options;
    
        send((void *) &request, sizeof(request));
        receive((void *) &response);
    
        if (response.Success) {
            float x, y;
            
            if (options.SilentAim) {
                esp.DrawCircle(Color(255, 0, 0, 255), Vector2(screenWidth / 2, screenHeight / 2), options.fov, 1.0f);
            }
            
            if (options.AimLine) {
                if (response.targetPos.Z > 0) {
                    esp.DrawLine(Color(255, 255, 255, 255), 1.0f, Vector2(screenWidth / 2, screenHeight / 2), Vector2(response.targetPos.X, response.targetPos.Y));
                }
            }
    
            for (int i = 0; i < response.PlayerCount; i++) {
                PlayerData player = response.Players[i];
    
                x = player.HeadPos.X;
                y = player.HeadPos.Y;
    
                if (isNoBot && player.isBot)
                    continue;
    
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
    
                float height = abs(player.HeadPos.Y - player.RootPos.Y) * 1.2f;
                float width = height * 0.48f;
                Rect rect = Rect(player.HeadPos.X - width / 2.f, player.HeadPos.Y - 10, width, height);
    
                if (player.HeadPos.Z > 0) {
                    if (isPlayerLinee) {
                        if (isPlayerLine == 0) {
                            esp.DrawLine(clrEnemy, 1.5f, Vector2(screenWidth / 2, 105), Vector2(rect.x + rect.w / 2.0f, rect.y - 16));
                        } else if (isPlayerLine == 1) {
                            esp.DrawLine(clrEnemy, 1.5f, Vector2(screenWidth / 2, screenHeight / 2), Vector2(rect.x + rect.w / 2.0f, rect.y));
                        } else if (isPlayerLine == 2) {
                            esp.DrawLine(clrEnemy, 1.5f, Vector2(screenWidth / 2, screenHeight), Vector2(rect.x + rect.w / 2.0f, rect.y + rect.h));
                        }
                    }
    
                    if (isPlayerBoxx) {
                        if (isPlayerBox == 0) {
                            esp.DrawRect(clrBox, 1.5f, Vector2(rect.x, rect.y), Vector2(rect.x + rect.w, rect.y + rect.h));
                        } else if (isPlayerBox == 1) {
                            esp.DrawFilledRect(clrFilled, Vector2(rect.x, rect.y), Vector2(rect.x + rect.w, rect.y + rect.h));
                            esp.DrawRect(clrBox, 1.5f, Vector2(rect.x, rect.y), Vector2(rect.x + rect.w, rect.y + rect.h));
                        }
                    }
    
                    if (isPlayerHealth) {
                        int currentHP = player.Health;
                        int maxHP = 200;
                        
                        float hpPercent = (float)currentHP / (float)maxHP;
                    
                        Color hpColor;
                        if (player.isKnocked) {
                            hpColor = Color(255, 0, 0, 255); 
                        } else {
                            hpColor = Color(0, 255, 0, 255);
                        }
                    
                        float barWidth = 5.0f; 
                        float barGap = 2.0f;
                    
                        float barX = rect.x - barGap - barWidth;
                    
                        esp.DrawFilledRect(Color(0, 0, 0, 150), Vector2(barX, rect.y), Vector2(barX + barWidth, rect.y + rect.h));
                        esp.DrawFilledRect(hpColor, Vector2(barX, rect.y + (rect.h - (rect.h * hpPercent))), Vector2(barX + barWidth, rect.y + rect.h));
                    }
    
                    if (isPlayerName) {
                        esp.DrawText(Color(255, 255, 255, 255), player.Name, Vector2(rect.x + rect.w / 2.0f, rect.y - 5), 15);
                    }
    
                    if (isPlayerDist) {
                        sprintf(extra, "%0.0fM", player.Distance);
                        float centerX = rect.x + rect.w / 2.0f;
                        float centerY = rect.y + rect.h + 15;
                        
                        esp.DrawText(Color(255, 255, 255, 255), extra, Vector2(centerX, centerY), 15);
                    }
                }
    
                if (isr360Alert && player.HeadPos.Z < 0) {
                    if (!isr360Alert)
                        continue;
    
                    if (rect.x > screenWidth - screenWidth / 12)
                        rect.x = screenWidth - screenWidth / 120;
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
                                    20);
                    } else {
                        esp.DrawRect(Color(255, 255, 255), 2,
                                    Vector2(screenWidth - x - 100, 48),
                                    Vector2(screenWidth - x + 100, -2));
                        esp.DrawFilledRect(Color(255, 0, 0, 140),
                                        Vector2(screenWidth - x - 100, 48),
                                        Vector2(screenWidth - x + 100, -2));
                        sprintf(extra, "%0.0f m", response.Players[i].Distance);
                        esp.DrawText(Color(255, 255, 255, 255), extra,
                                    Vector2(screenWidth - x, 25), 20);
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
                                    20);
                    } else {
                        esp.DrawRect(Color(255, 255, 255), 2,
                                    Vector2(0 + 88, y - 35), Vector2(0 - 2, y + 35));
                        esp.DrawFilledRect(Color(255, 0, 0, 140),
                                        Vector2(0 + 88, y - 35), Vector2(0 - 2, y + 35));
                        sprintf(extra, "%0.0f m", response.Players[i].Distance);
                        esp.DrawText(Color(255, 255, 255, 255), extra,
                                    Vector2(screenWidth / 80, y + 10), 20);
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
                                    Vector2(x, screenHeight - 20), 20);
                    } else {
                        esp.DrawRect(Color(255, 255, 255), 2,
                                    Vector2(x - 100, 48), Vector2(x + 100, -2));
                        esp.DrawFilledRect(Color(255, 0, 0, 140),
                                        Vector2(x - 100, 48), Vector2(x + 100, -2));
                        sprintf(extra, "%0.0f m", response.Players[i].Distance);
                        esp.DrawText(Color(255, 255, 255, 255), extra,
                                    Vector2(x, 25), 20);
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
    
            esp.DrawOTH(Vector2(screenWidth / 2 - (80), 67), ENEM_ICON);
            esp.DrawOTH(Vector2(screenWidth / 2, 67), BOT_ICON);
    
            esp.DrawText(Color(255, 255, 255, 255), cn, Vector2(screenWidth / 2 - (20), 94), 23);
    
            esp.DrawText(Color(255, 255, 255, 255), bt, Vector2(screenWidth / 2 + (50), 94), 23);
        }
    }
}
#endif // DESI_IMPORTANT_HACK_H
