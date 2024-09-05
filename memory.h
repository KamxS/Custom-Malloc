#ifdef __linux__
#include <sys/types.h>
#include <unistd.h>
#endif

void *memory_base();
void memory_expand(size_t incr);
void memory_change_base(void *addr);
