#include "memory.h"
void *__mem_base;
void *memory_init() {
    SYSTEM_INFO sSysInfo;
    GetSystemInfo(&sSysInfo);     // Initialize the structure.

    LPVOID lpvResult = VirtualAlloc(NULL, sSysInfo.dwPageSize, MEM_RESERVE, PAGE_READWRITE);
    if(!lpvResult) {
        // TODO: Error Handling
    }
    __mem_base = lpvResult;
    return lpvResult;
}
void *memory_base() {
    return __mem_base;
}
void memory_expand(size_t incr, void *addr) {
    VirtualAlloc(addr, incr, MEM_COMMIT, PAGE_READWRITE);
}
void memory_change_base(void *addr);
