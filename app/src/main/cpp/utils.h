#include <cstdio>
#include <string>
#include <cstring>
#include <unistd.h>
#include <sys/mman.h>
#include <cstdlib>
#include <inttypes.h>

#include "logger.h"

uintptr_t basePtr = 0;

const char* lib = "libil2cpp.so";

uintptr_t get_lib_start(const char* lib) {
    uintptr_t basePtr = 0;
    char line[1024];
    FILE* fp = fopen("/proc/self/maps", "re");

    if(fp) {
        while(fgets(line, sizeof line, fp)) {
            if(strstr(line, lib)) {
                LOGD("%s", line);

                //7cdd288000-7cdd478000 rw-p
                basePtr = std::stoul(line, NULL, 16);
                LOGD("BasePtr: %p", basePtr);
                return basePtr;
            }
        }
    }
    return basePtr;
}

void* getAbsoluteAddress(uintptr_t relativeAddr, bool recheck=false) {
    while(basePtr == 0) {
        basePtr = get_lib_start(lib);
        LOGD("basePtr: %p", basePtr);
    }
    if(recheck)
        basePtr = get_lib_start(lib);
    return (void*)(basePtr + relativeAddr);
}

// https://stackoverflow.com/a/20382831
int setPageProtection(void* addr, size_t len, int protection) {
    uintptr_t addr_start = ((uintptr_t) addr);
    uintptr_t addr_end = addr_start + len;

    size_t pageSize = sysconf(_SC_PAGESIZE);
    uintptr_t pageStart = addr_start & -pageSize;

    // layout: [pageStart] -- [addr_start] -- [addr_end] -- [pageEnd]
    // so change protection for region from pageStart till addr_end
    return mprotect((void*)pageStart, addr_end - pageStart, protection);
};

bool memWrite(void *addr, void *buffer, size_t len) {
    bool ret = true;
    if(setPageProtection(addr, len, PROT_READ | PROT_WRITE | PROT_EXEC) < 0) {
        LOGD("Couldn't set write protection");
        return false;
    }
    if (memcpy(addr, buffer, len) == NULL) {
        LOGD("Could not copy buffer!");
        ret = false;
    }
    if (setPageProtection(addr, len, PROT_READ | PROT_EXEC) < 0) {
        LOGD("Couldn’t restore protection");
        return false;
    }
    return ret;
}

bool memRead(void *memaddr, void *dest, size_t len) {
    return memcpy((void*) dest, (void*) memaddr, len) != NULL;
}

// Convert String to uint16 array
void readStr2Uint16(const char *buffer, uint16_t *dest, size_t len) {
    char temp[len * 2 + 1];
    memset(temp, 0, len * 2 + 1);

    for (int i = 0, t = 0; i < len; i++, t = t + 2 ) {
        sprintf(&temp[t], "%c", (unsigned char) buffer[i]);
    }

    memRead(&temp[0], (void*)dest, len * 2 + 1);
}

// Convert Uint16 array to String
std::string readUint162Str(void *addr, size_t len) {
    char temp[len * 4 + 1];
    memset(temp, 0, len);

    char buffer[len];
    memset(buffer, 0, len);

    std::string ret;

    if (!memRead(addr, temp, len * 4 + 1))
        return ret;

    for (int i = 0, t = 0; i < len; i++, t = t + 2 ) {
        sprintf(&buffer[i], "%c", (unsigned char) temp[t]);
    }
    ret += buffer;
    return ret;
}

std::string read2HexStr(void *addr, size_t len) {
    char temp[len];
    memset(temp, 0, len);

    const size_t bufferLen = len * 2 + 1;
    char buffer[bufferLen];
    memset(buffer, 0, bufferLen);

    std::string ret;

    if (!memRead(addr, temp, len))
        return ret;

    for (int i = 0; i < len; i++) {
        sprintf(&buffer[i * 2], "%02X", (unsigned char) temp[i]);
    }

    ret += buffer;
    return ret;
}