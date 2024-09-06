#include "memory.h"
void *__mem_base;
inline void *memory_init() {
    __mem_base=sbrk(0);
    return __mem_base;
}
inline void *memory_base() {
    return __mem_base;
}
inline void memory_expand(size_t incr, void *addr) {
    sbrk(incr);
}
inline void memory_change_base(void *addr) {
    brk(addr);
}
