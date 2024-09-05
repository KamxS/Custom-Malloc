#include "memory.h"
inline void *memory_base() {
    return sbrk(0);
}
inline void memory_expand(size_t incr) {
    sbrk(incr);
}
inline void memory_change_base(void *addr) {
    brk(addr);
}
