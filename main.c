#include <stdio.h>
#include <stdlib.h>

typedef struct cmalloc_chunk_meta cmalloc_chunk_meta;

void *memory = NULL;
cmalloc_chunk_meta *mem_base = NULL;
char *mem_top = NULL;
size_t mem_max_size = 256;

struct cmalloc_chunk_meta {
    int free;
    size_t size;
    cmalloc_chunk_meta *next;
};

int memory_expand(size_t size) {
    if(mem_top+size>=(char*)mem_base+mem_max_size) return 1;
    mem_top+=size;
    return 0;
}

void cmalloc_dump_blocks() {

}

void *cmalloc(size_t size) {
    if(mem_base) {
        cmalloc_chunk_meta *chunk=mem_base;
        do {
            if(!chunk->free) continue;

        }while(chunk->next);
        if(memory_expand(size+sizeof(cmalloc_chunk_meta))!=0) 
            return NULL;
        *(cmalloc_chunk_meta*)mem_top = (cmalloc_chunk_meta){1,size,NULL};
        return (cmalloc_chunk_meta*)mem_top+1;
    }else {
        mem_base=memory;
        mem_top=memory;
        if(memory_expand(size+sizeof(cmalloc_chunk_meta))!=0) 
            return NULL;
        *mem_base=(cmalloc_chunk_meta){1,size,NULL};
        return mem_base+1;
    }
}

int main() {
    memory = malloc(mem_max_size);

    int *vec3 = cmalloc(3*sizeof(int));
    vec3[2] = 1;
    int *vec2 = cmalloc(2*sizeof(int));
    vec2[0] = 10;
    vec2[1] = 20;
    printf("%d %d %d\n", vec3[2], vec2[0],vec2[1]);

    free(memory);
    return 0;
}
