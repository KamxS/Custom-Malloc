#ifndef CMALLOC_HEADER
#define CMALLOC_HEADER

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#include <windows.h>
#elif __linux__
#include <sys/types.h>
#include <unistd.h>
#endif

void *memory_init();
void *memory_base();
void memory_expand(size_t incr, void *addr);

#endif
