#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct cmalloc_chunk_meta cmalloc_chunk_meta;

void *memory = NULL;
cmalloc_chunk_meta *mem_base = NULL;
char *mem_end = NULL;

struct cmalloc_chunk_meta {
    int free;
    size_t size;
    cmalloc_chunk_meta *next;
};

void cmalloc_dump() {
    cmalloc_chunk_meta *chunk=mem_base;
    int i=0;

    while(chunk) {
        char is_free = 'U';
        if(chunk->free) is_free='F';
        printf("Chunk %d - %c %d\n",++i, is_free, chunk->size);
        chunk = chunk->next;
    }
}

void cmalloc_free(void *ptr) {
    ((cmalloc_chunk_meta*)ptr-1)->free=1;
}

void *cmalloc(size_t size) {
    if(mem_base) {
        cmalloc_chunk_meta *chunk=mem_base;
        while(1) {
            if(chunk->free && chunk->size>=size) {
                break;
            }else if(!chunk->next) {
                chunk->next = (cmalloc_chunk_meta*)mem_end;
                chunk=(cmalloc_chunk_meta*)mem_end;
                chunk->next=NULL;
                chunk->size=size;
                mem_end +=size+sizeof(cmalloc_chunk_meta);
                break;
            }
            chunk=chunk->next;
        }
        chunk->free=0;
        return chunk->next+1;
    }else {
        mem_base=memory;
        mem_end = memory+size+sizeof(cmalloc_chunk_meta);
        *mem_base=(cmalloc_chunk_meta){0,size,NULL};
        return mem_base+1;
    }
}

int main() {
    memory = malloc(1024);

    int *vec1 = cmalloc(3*sizeof(int));
    int *vec2 = cmalloc(1*sizeof(int));
    int *vec3 = cmalloc(1*sizeof(int));
    cmalloc_free(vec1);
    int *vec4 = cmalloc(1*sizeof(int));
    cmalloc_dump();
    printf("NO Cradsh");

    free(memory);
    return 0;
}

// new_value = ( (old_value - old_min) / (old_max - old_min) ) * (new_max - new_min) + new_min
