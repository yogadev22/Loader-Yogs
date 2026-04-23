#ifndef SUPPORT_H
#define SUPPORT_H

#include <inttypes.h>
#include "socket.h"
#include "struct.h"
#include "Quaternion.hpp"
#include "Matrix4x4.hpp"
#include <unordered_map>

#define PI 3.141592653589793238

#if defined(__aarch64__)
    // 64-bit ARM
    #define SYS_process_vm_readv 270
    #define SYS_process_vm_writev 271
    #define SYS_openat 56
#elif defined(__x86_64__)
    // 64-bit x86
    #define SYS_process_vm_readv 310
    #define SYS_process_vm_writev 311
    #define SYS_open 2
#elif defined(__arm__)
    // 32-bit ARM
    #define SYS_process_vm_readv 376
    #define SYS_process_vm_writev 377
    #define SYS_open 5
#elif defined(__i386__)
    // 32-bit x86
    #define SYS_process_vm_readv 347
    #define SYS_process_vm_writev 348
    #define SYS_open 5
#else
    #error "Architecture not supported"
#endif

int screenWidth, screenHeight;
int handle;

uintptr_t FindLibrary(const char* name, int index) {
    int i = 0;
    uintptr_t start = 0;
    char line[1024] = {0};
    char dname[128], fname[128];

    snprintf(dname, sizeof(dname), "%s", name);
    snprintf(fname, sizeof(fname), "/proc/%d/maps", pid);
    FILE* p = fopen(fname, "r");
    if (p) {
        while (fgets(line, sizeof(line), p)) {
            if (strstr(line, dname) != NULL) {
                i++;
                if (i == index) {
                    sscanf(line, "%" SCNxPTR, &start);
                    break;
                }
            }
        }
        fclose(p);
    }
    return start;
}

template<class T>
T Read(uintptr_t address) {
    T buf{};
    struct iovec local, remote;
    
    local.iov_base = &buf;
    local.iov_len = sizeof(T);
    remote.iov_base = reinterpret_cast<void*>(address);
    remote.iov_len = sizeof(T);
    
    ssize_t bytes = syscall(SYS_process_vm_readv, pid, &local, 1, &remote, 1, 0);
    if (bytes != sizeof(T)) {
        memset(&buf, 0, sizeof(T));
    }
    return buf;
}

int edithex(long int addr, const char *hex)
{
    unsigned char bytes[32];
    int count = 0;

    char buffer[128];
    strcpy(buffer, hex); // copy dulu

    char *token = strtok(buffer, " ");
    while (token != NULL)
    {
        bytes[count++] = (unsigned char)strtoul(token, NULL, 16);
        token = strtok(NULL, " ");
    }

    pwrite64(handle, bytes, count, addr);
    return 0;
}

template<class T>
bool Write(uintptr_t address, T value) {
    struct iovec local, remote;
    
    local.iov_base = &value;
    local.iov_len = sizeof(T);
    remote.iov_base = reinterpret_cast<void*>(address);
    remote.iov_len = sizeof(T);
    
    ssize_t bytes = syscall(SYS_process_vm_writev, pid, &local, 1, &remote, 1, 0);
    return (bytes == sizeof(T));
}

pid_t getPid(char * name) {
	char text[69];
	pid_t pid = 0;
	sprintf(text,"pidof %s",name);
	FILE *chkRun = popen(text, "r");
	if (chkRun) {
		char output[10];
		fgets(output ,10,chkRun);
		pclose(chkRun);
		pid= atoi(output);
	}
	if (pid < 10) {
		DIR* dir = NULL;
		struct dirent* ptr = NULL;
		FILE* fp = NULL;
		char filepath[256];
		char filetext[128];
		dir = opendir("/proc");
		if (NULL != dir) {
			while ((ptr = readdir(dir)) != NULL) {
				if ((strcmp(ptr->d_name, ".") == 0) || (strcmp(ptr->d_name, "..") == 0))
					continue;
				if (ptr->d_type != DT_DIR)
					continue;
				sprintf(filepath, "/proc/%s/cmdline", ptr->d_name);
				fp = fopen(filepath, "r");
				if (NULL != fp) {
					fgets(filetext, sizeof(filetext), fp);
					if (strcmp(filetext, name) == 0) {
						fclose(fp);
						break;
					}
					fclose(fp);
				}
			}
		}
		if (readdir(dir) == NULL) {
			closedir(dir);
			return 0;
		}
		closedir(dir);
		pid= atoi(ptr->d_name);
	}
	return pid;
}

struct Matrix {
	Vector4 Position;
	Quaternion Rotation;
	Vector4 Scale;
};

static auto GetPosition(uintptr_t Transform) {
	auto pos = Vector3::Zero();

	auto transformObjValue = Read<uintptr_t>(Transform + 0x10);
	if (transformObjValue == 0) {
		return pos;
	}

	auto indexValue = Read<uintptr_t>(transformObjValue + 0x40);
	auto matrixValue = Read<uintptr_t>(transformObjValue + 0x38);

	auto matrixListValue = Read<uintptr_t>(matrixValue + 0x18);
	auto matrixIndicesValue = Read<uintptr_t>(matrixValue + 0x20);

	auto resultValue = Read<Vector3>(matrixListValue + (indexValue * 0x30)); // FIXED: 64-bit pointer arithmetic

	auto maxTries = 50;
	auto tries = 0;

	// FIXED: Proper 64-bit pointer arithmetic
	auto transformIndexValue = Read<int>(matrixIndicesValue + (indexValue * 4));

	while (transformIndexValue >= 0 && tries < maxTries) {
		tries++;

		// FIXED: Proper 64-bit pointer arithmetic
		auto tMatrixValue = Read<Matrix>(matrixListValue + (transformIndexValue * 0x30));

		auto rotX = tMatrixValue.Rotation.X;
		auto rotY = tMatrixValue.Rotation.Y;
		auto rotZ = tMatrixValue.Rotation.Z;
		auto rotW = tMatrixValue.Rotation.W;

		auto scaleX = resultValue.X * tMatrixValue.Scale.X;
		auto scaleY = resultValue.Y * tMatrixValue.Scale.Y;
		auto scaleZ = resultValue.Z * tMatrixValue.Scale.Z;

		resultValue.X = tMatrixValue.Position.X + scaleX +
						(scaleX * ((rotY * rotY * -2.0) - (rotZ * rotZ * 2.0))) +
						(scaleY * ((rotW * rotZ * -2.0) - (rotY * rotX * -2.0))) +
						(scaleZ * ((rotZ * rotX * 2.0) - (rotW * rotY * -2.0)));
		resultValue.Y = tMatrixValue.Position.Y + scaleY +
						(scaleX * ((rotX * rotY * 2.0) - (rotW * rotZ * -2.0))) +
						(scaleY * ((rotZ * rotZ * -2.0) - (rotX * rotX * 2.0))) +
						(scaleZ * ((rotW * rotX * -2.0) - (rotZ * rotY * -2.0)));
		resultValue.Z = tMatrixValue.Position.Z + scaleZ +
						(scaleX * ((rotW * rotY * -2.0) - (rotX * rotZ * -2.0))) +
						(scaleY * ((rotY * rotZ * 2.0) - (rotW * rotX * -2.0))) +
						(scaleZ * ((rotX * rotX * -2.0) - (rotY * rotY * 2.0)));

		// FIXED: Proper 64-bit pointer arithmetic
		transformIndexValue = Read<int>(matrixIndicesValue + (transformIndexValue * 4));
	}

	if (tries < maxTries) {
		pos = resultValue;
	}

	return pos;
}

void getUTF8(char* dst, uintptr_t addr) {
    if (addr < 0x10000000) {
        dst[0] = '\0';
        return;
    }

    int stringLen = Read<int>(addr + 0x10);
    if (stringLen <= 0 || stringLen > 64) {
        stringLen = 32; 
    }

    unsigned char raw[128]; 
    for (int i = 0; i < (stringLen * 2); i++) {
        raw[i] = Read<unsigned char>(addr + 0x14 + i);
    }

    int j = 0;
    for (int i = 0; i < (stringLen * 2); i += 2) {
        unsigned short unicode = raw[i] | (raw[i + 1] << 8);
        if (unicode == 0) break;
        if (unicode < 0x80) {
            dst[j++] = (char)unicode;
        } else if (unicode < 0x800) {
            dst[j++] = (char)((unicode >> 6) | 0xC0);
            dst[j++] = (char)((unicode & 0x3F) | 0x80);
        } else if (unicode >= 0xD800 && unicode <= 0xDBFF) { 
            i += 2;
            unsigned short low = raw[i] | (raw[i + 1] << 8);
            unsigned int codepoint = 0x10000 + ((unicode - 0xD800) << 10) + (low - 0xDC00);
            dst[j++] = (char)((codepoint >> 18) | 0xF0);
            dst[j++] = (char)(((codepoint >> 12) & 0x3F) | 0x80);
            dst[j++] = (char)(((codepoint >> 6) & 0x3F) | 0x80);
            dst[j++] = (char)((codepoint & 0x3F) | 0x80);
        } else {
            dst[j++] = (char)((unicode >> 12) | 0xE0);
            dst[j++] = (char)(((unicode >> 6) & 0x3F) | 0x80);
            dst[j++] = (char)((unicode & 0x3F) | 0x80);
        }
        if (j >= 60) break; 
    }
    dst[j] = '\0';
}

std::atomic<bool> keepAiming{false};
std::thread aimThread;
Vector3 currentBestTargetPos = Vector3::Zero();

void ContinuousAim(uintptr_t oneself) {
    while (keepAiming) {
        uint64_t aimingInfo = Read<uint64_t>(oneself + 0xd80);
        if (aimingInfo) {
            Vector3 startPos = Read<Vector3>(aimingInfo + 0x4c); // old
            Vector3 dir = currentBestTargetPos - startPos;  
            Write<Vector3>(aimingInfo + 0x40, dir);
        }
    }  
}

Vector3 WorldToScreen(Matrix4x4 _vMatrix, Vector3 from) {
    Vector3 to = Vector3::Zero();
    auto viewMatrix = _vMatrix.MultiplyPoint(from);
    auto screenPos = Vector3(viewMatrix.X + 1.0f, viewMatrix.Y + 1.0f, viewMatrix.Z + 1.0f) / 2.0f;
    to = Vector3(screenPos.X * screenWidth, screenHeight - (screenPos.Y * screenHeight), viewMatrix.Z);
    return to;
}

#endif