#ifndef SUPPORT_H
#define SUPPORT_H

#include "socket.h"
#include "struct.h"
#include "Quaternion.hpp"

#define PI 3.141592653589793238

ssize_t process_v(pid_t __pid, struct iovec* __local_iov, unsigned long __local_iov_count, struct iovec* __remote_iov, unsigned long __remote_iov_count, unsigned long __flags) {
	return syscall(process_vm_readv_syscall, __pid, __local_iov, __local_iov_count, __remote_iov, __remote_iov_count, __flags);
}

ssize_t process_v2(pid_t __pid, struct iovec* __local_iov, unsigned long __local_iov_count, struct iovec* __remote_iov, unsigned long __remote_iov_count, unsigned long __flags) {
	return syscall(process_vm_writev_syscall, __pid, __local_iov, __local_iov_count, __remote_iov, __remote_iov_count, __flags);
}

int pvm(uintptr_t address, void* buffer,int size) {
	struct iovec local[1];
	struct iovec remote[1];

	local[0].iov_base = (void*)buffer;
	local[0].iov_len = size;
	remote[0].iov_base = (void*)address;
	remote[0].iov_len = size;
	ssize_t bytes = process_v(pid, local, 1, remote, 1, 0);
	return bytes == size;
}

void pvm2(uintptr_t address, void* buffer,int size) {
	struct iovec local[1];
	struct iovec remote[1];

	local[0].iov_base = (void*)buffer;
	local[0].iov_len = size;
	remote[0].iov_base = (void*)address;
	remote[0].iov_len = size;

	process_v2(pid, local, 1, remote, 1, 0);
}

uintptr_t getBase() {
    FILE *fp;
    uintptr_t addr = 0;
    char filename[40], buffer[1024];
    snprintf(filename, sizeof(filename), "/proc/%d/maps", pid);
    fp = fopen(filename, "rt");
    if (fp != NULL) {
        while (fgets(buffer, sizeof(buffer), fp)) {
            if (strstr(buffer, "libil2cpp.so")) {
                addr = (uintptr_t)strtoul(buffer, NULL, 16);
                break;
            }
        }
        fclose(fp);
    }
    return addr;
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

ssize_t process_v3(pid_t __pid, const struct iovec *__local_iov, unsigned long __local_iov_count,
				  const struct iovec *__remote_iov, unsigned long __remote_iov_count,
				  unsigned long __flags, bool iswrite)
{
	return syscall((iswrite ? process_vm_writev_syscall : process_vm_readv_syscall), __pid,
				   __local_iov, __local_iov_count, __remote_iov, __remote_iov_count, __flags);
}


bool pvm3(void *address, void *buffer, size_t size, bool iswrite)
{
	struct iovec local[1];
	struct iovec remote[1];
	local[0].iov_base = buffer;
	local[0].iov_len = size;
	remote[0].iov_base = address;
	remote[0].iov_len = size;
	if (pid < 0)
	{
		return false;
	}
	ssize_t bytes = process_v3(pid, local, 1, remote, 1, 0, iswrite);
	return bytes == size;
}

template<typename T>
T Read(unsigned long address) {
    T data;
    pvm3(reinterpret_cast < void *>(address), reinterpret_cast<void *>(&data), sizeof(T), false);
    return data;
}

float getDistance(struct Vector3 mxyz, struct Vector3 exyz)
{
	return sqrt((mxyz.X-exyz.X) * (mxyz.X-exyz.X) + (mxyz.Y-exyz.Y) * (mxyz.Y-exyz.Y) + (mxyz.Z-exyz.Z) * (mxyz.Z-exyz.Z)) / 100;
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

#endif
