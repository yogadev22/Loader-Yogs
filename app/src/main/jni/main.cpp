#include <EGL/egl.h>
#include <GLES3/gl3.h>
#include <list>
#include <vector>
#include <string.h>
#include <android/log.h>
#include <pthread.h>
#include <thread>
#include <cstring>
#include <jni.h>
#include <unistd.h>
#include <fstream>
#include <iostream>
#include "imgui/imgui.h"
#include "imgui/imgui_impl_android.h"
#include "imgui/imgui_impl_opengl3.h"
#include <cstring>
#include <string.h>
#include "And64InlineHook/And64InlineHook.hpp"
#include <cstdio>
#include <cinttypes>
#include <cstdint>
#include <cstring>
#include <libgen.h>
#include <thread>
#include <sstream>
#include <chrono>
#include "obfuscate.h"
#include "patch/MemoryPatch.h"
#include "fake_dlfcn.h"
#include "Vector3.hpp"

bool initImGui = false;
int glWidth, glHeight;
uintptr_t il2cpp;
bool autoall, grenadehaha, gameinfosjs;
float density = 290.0f;

uintptr_t GetBaseAddress(const char *name) {
    uintptr_t base = 0;
    char line[512];

    FILE *f = fopen("/proc/self/maps", "r");

    if (!f) {
        return 0;
    }

    while (fgets(line, sizeof line, f)) {
        uintptr_t tmpBase;
        char tmpName[256];
        if (sscanf(line, "%" PRIXPTR "-%*" PRIXPTR " %*s %*s %*s %*s %s", &tmpBase, tmpName) > 0) {
            if (!strcmp(basename(tmpName), name)) {
                base = tmpBase;
                break;
            }
        }
    }

    fclose(f);
    return base;
}

uintptr_t string2Offset(const char *c) {
    int base = 16;
    // See if this function catches all possibilities.
    // If it doesn't, the function would have to be amended
    // whenever you add a combination of architecture and
    // compiler that is not yet addressed.
    static_assert(sizeof(uintptr_t) == sizeof(unsigned long)
                  || sizeof(uintptr_t) == sizeof(unsigned long long),
                  "Please add string to handle conversion for this architecture.");

    // Now choose the correct function ...
    if (sizeof(uintptr_t) == sizeof(unsigned long)) {
        return strtoul(c, nullptr, base);
    }

    // All other options exhausted, sizeof(uintptr_t) == sizeof(unsigned long long))
    return strtoull(c, nullptr, base);
}

#define HOOK_LIB(lib, offset, hook, orig) A64HookFunction((void *)(GetBaseAddress(OBFUSCATE(lib)) + string2Offset(OBFUSCATE(offset))), (void *)hook, (void **)&orig)
#define HOOK_LIB_NO_ORIG(lib, offset, ptr) A64HookFunction((void *)(GetBaseAddress(OBFUSCATE(lib)) + string2Offset(OBFUSCATE(offset))), (void *)ptr, NULL)

enum daLogType {
    daDEBUG = 3,
    daERROR = 6,
    daINFO = 4,
    daWARN = 5
};

#define TAG ("Putri")

#define LOGE(...) ((void)__android_log_print(daERROR, TAG, __VA_ARGS__))

std::vector<MemoryPatch> memoryPatches;
std::vector<uint64_t> offsetVector;

// Patching a offset without switch.
void patchOffset(const char *fileName, uint64_t offset, std::string hexBytes, bool isOn) {

    MemoryPatch patch = MemoryPatch::createWithHex(fileName, offset, hexBytes);

    //Check if offset exists in the offsetVector
    if (std::find(offsetVector.begin(), offsetVector.end(), offset) != offsetVector.end()) {
        //LOGE(OBFUSCATE("Already exists"));
        std::vector<uint64_t>::iterator itr = std::find(offsetVector.begin(), offsetVector.end(), offset);
        patch = memoryPatches[std::distance(offsetVector.begin(), itr)]; //Get index of memoryPatches vector
    } else {
        memoryPatches.push_back(patch);
        offsetVector.push_back(offset);
        //LOGI(OBFUSCATE("Added"));
    }

    if (!patch.isValid()) {
        LOGE(OBFUSCATE("Failing offset: 0x%llu, please re-check the hex"), offset);
        return;
    }
    if (isOn) {
        if (!patch.Modify()) {
            LOGE(OBFUSCATE("Something went wrong while patching this offset: 0x%llu"), offset);
        }
    } else {
        if (!patch.Restore()) {
            LOGE(OBFUSCATE("Something went wrong while restoring this offset: 0x%llu"), offset);
        }
    }
}

#define PATCH_LIB(lib, offset, hex) patchOffset(OBFUSCATE(lib), string2Offset(OBFUSCATE(offset)), OBFUSCATE(hex), true)

template <typename T>
struct monoArray
{
    void* klass;
    void* monitor;
    void* bounds;
    int   max_length;
    void* vector [1];
    int getLength()
    {
        return max_length;
    }
    T getPointer()
    {
        return (T)vector;
    }
};

template <typename T>
struct monoList {
    void *unk0;
    void *unk1;
    monoArray<T> *items;
    int size;
    int version;

    T getItems(){
        return items->getPointer();
    }

    int getSize(){
        return size;
    }

    int getVersion(){
        return version;
    }
};

template <typename K, typename V>
struct monoDictionary {
    void *unk0;
    void *unk1;
    monoArray<int **> *table;
    monoArray<void **> *linkSlots;
    monoArray<K> *keys;
    monoArray<V> *values;
    int touchedSlots;
    int emptySlot;
    int size;

    K getKeys(){
        return keys->getPointer();
    }

    V getValues(){
        return values->getPointer();
    }

    int getNumKeys(){
        return keys->getLength();
    }

    int getNumValues(){
        return values->getLength();
    }

    int getSize(){
        return size;
    }
};
union intfloat {
    int i;
    float f;
};

typedef struct _monoString
{
    void* klass;
    void* monitor;
    int length;    
    char chars[1];   
    int getLength()
    {
      return length;
    }
    char* getChars()
    {
        return chars;
    }
}monoString;

void *Curent_Match() {
    auto _Curent_Match = (void *(*)())(il2cpp + 0x67DAD38);
    return _Curent_Match();
}

Vector3 get_position(uintptr_t transform) {
    auto _get_position = (Vector3 (*)(uintptr_t))(il2cpp + 0x96E23CC);
    return _get_position(transform);
}

void *get_main() {
    auto _get_main = (void *(*)())(il2cpp + 0x968DF34);
    return _get_main();
}

Vector3 WorldToScreen(Vector3 pos) {
    auto _w2s = (Vector3(*)(void *, Vector3))(il2cpp + 0x968DBB8);
    return _w2s(get_main(), pos);
}

std::vector<uintptr_t> getPlayers() {
    std::vector<uintptr_t> list;

    auto Match = Curent_Match();
    if (!Match) 
        return {};

    monoDictionary <uint8_t *, void **> *players = *(monoDictionary <uint8_t *, void **> **)((uintptr_t)Match + 0x100);
    if (players->getNumValues() <= 0) 
        return {};

    for (int i = 0; i < players->getNumValues(); i++) {
        auto Actor = players->getValues()[i];
        
        if (Actor)
            list.push_back((uintptr_t)Actor);
    }
    return list;
}

void DrawESP(ImDrawList *draw) {
	ImGui::PushFontShadow(0xFF000000);

    int totalEnemies = 0, totalBots = 0;
    
    auto Actors = getPlayers();

    uintptr_t localPlayer = 0;
	
	void *Match = Curent_Match();
    
    if (Match) {
        localPlayer = *(uintptr_t *)((uintptr_t)Match + 0xB0);
	}
    
	if (localPlayer) {
        void *CameraMain = get_main();
        
        if (!CameraMain) {
            Vector3 MyPos = *(Vector3 *)(localPlayer + 0x1A0);
            
            for (int i = 0; i < Actors.size(); i++) {
                auto Player = Actors[i];					
                
                if (!Player || Player == localPlayer)
                    continue;
                
                bool isDead = *(bool *)(Player + 0x7C);
                if (isDead)
                    continue;
                
                Vector3 EnemyPos = *(Vector3 *)(Player + 0x1A0);
                float Distance = Vector3::Distance(EnemyPos, MyPos) / 100.0f;
                        
                int Colorhealth, healthOutline;
			    long LineColor;
                
                bool isClientBot = *(bool *)(Player + 0x3D8);
                
                if (isClientBot) {
                    totalBots++;
				    Colorhealth = IM_COL32(0, 255, 0, 120);//143, 242, 212, 110
				    healthOutline = IM_COL32(255, 255, 255, 255);
				    LineColor = IM_COL32(255, 255, 255, 255);
                } else {
                    totalEnemies++;
				    Colorhealth = IM_COL32(255, 0, 255, 120);
				    healthOutline = IM_COL32(255, 255, 255, 255);
                    LineColor = IM_COL32(255, 0, 0, 255);
                }               
                                
                uintptr_t HeadNode = *(uintptr_t *)(Player + 0x5C0);
                uintptr_t HeadTF = *(uintptr_t *)(HeadNode + 0x10);
                
                if (!HeadTF)
                    continue;
                
			    Vector3 HeadPos = get_position(HeadTF);
                Vector3 HeadPosSC = WorldToScreen(HeadPos);
                
                uintptr_t RootNode = *(uintptr_t *)(Player + 0x5E8);
                uintptr_t RootTF = *(uintptr_t *)(HeadNode + 0x10);
                if (!RootTF)
                    continue;
                    
                Vector3 RootPos = get_position(RootTF);
                Vector3 RootPosSC = WorldToScreen(RootPos);
                            
                if (HeadPosSC.Z > 0 && RootPosSC.Z > 0) {
                    if (autoall) {
                        draw->AddLine({(float) glWidth / 2, 93}, {HeadPosSC.X, HeadPosSC.Y - 53}, LineColor, 0);
                    }    
            
                    /*if (autoall) {
                        int CurHP = (int) std::max(0, std::min((int) Player->Health, (int) Player->HealthMax));
                        int MaxHP = (int) Player->HealthMax;
        
					    long HPColor = IM_COL32(0, 255, 0, 200);
					    long HPColorS = IM_COL32(255, 255, 255, 200);
        
                        if (Player->Health == 0.0f && !Player->bDead) {
                            HPColor = IM_COL32(255, 0, 0, 200);
						    HPColorS = IM_COL32(255, 0, 0, 200);
        
                            CurHP = Player->NearDeathBreath;
                            if (Player->NearDeatchComponent) {
                                MaxHP = Player->NearDeatchComponent->BreathMax;
                            }
                        }
        
                        float boxWidth = density / 6.0f;
                        boxWidth -= std::min(((boxWidth / 2) / 700.0f) * Distance, boxWidth / 2);
                        float boxHeight = boxWidth * 0.15f;
        
        
                        ImVec2 vStart = {HeadPosSC.x - (boxWidth / 2), HeadPosSC.y - (boxHeight * 2.5f)};
        
                        ImVec2 vEndFilled = {vStart.x + (CurHP * boxWidth / MaxHP), vStart.y + boxHeight};
                        ImVec2 vEndRect = {vStart.x + boxWidth, vStart.y + boxHeight};
                                            
                        if (isClientBot) {                                       
						    draw->AddRectFilled(vStart, vEndRect, IM_COL32(0, 0, 0, 150), 0.0f);
						    draw->AddRectFilled(vStart, vEndFilled, HPColorS, 0.0f);
					    } else {
						    draw->AddRectFilled(vStart, vEndRect, IM_COL32(0, 0, 0, 150), 0.0f);
						    draw->AddRectFilled(vStart, vEndFilled, HPColor, 0.0f);
                        }
                    }											
        
				    if (autoall) {
                        std::string s;
					    s += "[";
                        s += std::to_string(Player->TeamID).substr(0, 2);
                        s += "] ";
					    if (isClientBot) {
                            s += "Robot";
                        } else {
                            s += Player->PlayerName.ToString();
                        }
                                    
					    auto textSize = ImGui::CalcTextSize2(s.c_str(), 0, 14.f);
					    draw->AddText2(NULL, 14.f, {HeadPosSC.x - (textSize.x / 2), HeadPosSC.y - 35.0f}, IM_COL32(255, 255, 255, 255), s.c_str());
					    draw->AddText(NULL, 14.f, {HeadPosSC.x - (textSize.x / 2), HeadPosSC.y - 35.0f}, IM_COL32(255, 255, 255, 255), s.c_str());
                                    
					    std::string dista;
					    dista += std::to_string((int)Distance);
					    dista += "m";
                                        
					    auto textSizee = ImGui::CalcTextSize2(dista.c_str(), 0, 14.f);
                                        
					    draw->AddText2(NULL, 14.f, {RootPosSC.x - (textSizee.x / 2), RootPosSC.y + 3.f}, IM_COL32(255, 255, 255, 255), dista.c_str());
					    draw->AddText(NULL, 14.f, {RootPosSC.x - (textSizee.x / 2), RootPosSC.y + 3.f}, IM_COL32(255, 255, 255, 255), dista.c_str());
				    }*/
			    }
		    }
        }
        
		std::string s = "Player ";
        s += std::to_string(totalEnemies);
		s += " | ";
		s += std::to_string(totalBots);
		s += " Robot";

		auto textSize = ImGui::CalcTextSize2(s.c_str(), 0, 18.0f);
		draw->AddText2(NULL, 18.0f, {glWidth / 2 - (textSize.x / 2), 75}, IM_COL32(255, 0, 0, 255), s.c_str());

   	if (totalEnemies || totalBots > 0) {
	   	autoall = true;
   	} else {
	   	autoall = false;
   	}
	}
}

EGLBoolean (*orig_eglSwapBuffers)(EGLDisplay dpy, EGLSurface surface);
EGLBoolean _eglSwapBuffers(EGLDisplay dpy, EGLSurface surface) {
    eglQuerySurface(dpy, surface, EGL_WIDTH, &glWidth);
    eglQuerySurface(dpy, surface, EGL_HEIGHT, &glHeight);
   
	if (!initImGui) {
    	ImGui::CreateContext();
		ImGui::GetStyle().ScaleAllSizes(2.0f);
      
        ImGui_ImplAndroid_Init();
        ImGui_ImplOpenGL3_Init(OBFUSCATE("#version 300 es"));

        ImGuiIO &io = ImGui::GetIO();

        io.ConfigWindowsMoveFromTitleBarOnly = true;
        io.IniFilename = NULL;
	
        ImFontConfig cfg;
        cfg.SizePixels = 20.0f;
        io.Fonts->AddFontDefault(&cfg);
		io.Fonts->Flags |= ImFontAtlasFlags_NoPowerOfTwoHeight;
        
        initImGui = true;
    }
	
    ImGuiIO &io = ImGui::GetIO();
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplAndroid_NewFrame(glWidth, glHeight);
    ImGui::NewFrame();
    DrawESP(ImGui::GetBackgroundDrawList());
	
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    return orig_eglSwapBuffers(dpy, surface);
}

void *bypass_thread(void *) {
	do {
		sleep(1);
	} while (!il2cpp);

	return NULL;
}

void *main_thread(void *) {
	il2cpp = GetBaseAddress("libil2cpp.so");
    while (!il2cpp){
    	il2cpp = GetBaseAddress("libil2cpp.so");
        sleep(1);
	}

    A64HookFunction((void *) dlsym_ex(dlopen_ex("libEGL.so", 0), "eglSwapBuffers"), (void *) _eglSwapBuffers, (void **) &orig_eglSwapBuffers);
	return NULL;
}

__attribute__((constructor)) void _init() {
    pthread_t t;
    pthread_create(&t, NULL, main_thread, NULL);
    //pthread_create(&t, NULL, bypass_thread, NULL);
}
