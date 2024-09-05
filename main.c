#include <stdio.h>
#include <string.h>
#include "memory.h"

typedef struct cmalloc_chunk_meta cmalloc_chunk_meta;

void *memory = NULL;
cmalloc_chunk_meta *mem_base = NULL;

#define CHUNK_META(chunk) ((cmalloc_chunk_meta*)chunk-1)

struct cmalloc_chunk_meta {
    int free;
    size_t size;
    cmalloc_chunk_meta *prev;
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
    cmalloc_chunk_meta *chunk = CHUNK_META(ptr);
    if(chunk->prev) {
        if(chunk->prev->free) {
            cmalloc_chunk_meta *chunk_prev=chunk->prev;
            chunk_prev->size+=chunk->size+sizeof(cmalloc_chunk_meta);
            chunk_prev->next=chunk->next;
            chunk=chunk_prev;
        }
    }
    if(chunk->next) {
        if(chunk->next->free) {
            cmalloc_chunk_meta *chunk_next=chunk->next;
            chunk->size+=chunk->next->size+sizeof(cmalloc_chunk_meta);
            chunk->next= chunk_next->next;
        }
    }
    CHUNK_META(ptr)->free=1;
}

void *cmalloc(size_t size) {
    if(mem_base) {
        cmalloc_chunk_meta *chunk=mem_base;
        while(1) {
            if(chunk->free && chunk->size>=size) {
                if(chunk->size>size+sizeof(cmalloc_chunk_meta)) {
                    cmalloc_chunk_meta *new_chunk = (cmalloc_chunk_meta*)((char*)(chunk+1)+size);
                    *new_chunk = (cmalloc_chunk_meta){1,chunk->size-size-sizeof(cmalloc_chunk_meta),chunk,chunk->next};
                    chunk->size=size;
                    chunk->next=new_chunk;
                }
                break;
            }
            if(!chunk->next) {
                memory_expand(size+sizeof(cmalloc_chunk_meta));
                cmalloc_chunk_meta *new_chunk = (cmalloc_chunk_meta*)((char*)(chunk+1)+chunk->size);
                chunk->next = new_chunk;
                new_chunk->size=size;
                new_chunk->prev=chunk;
                new_chunk->next=NULL;
                chunk=new_chunk;
                break;
            }
            chunk=chunk->next;
        }
        chunk->free=0;
        return chunk+1;
    }else {
        mem_base=memory_base();
        memory_expand(size+sizeof(cmalloc_chunk_meta));
        *mem_base=(cmalloc_chunk_meta){0,size,NULL, NULL};
        return mem_base+1;
    }
}

int main() {
    int *vec1 = cmalloc(2*sizeof(int)+sizeof(cmalloc_chunk_meta));
    int *vec2 = cmalloc(1*sizeof(int));
    int *vec3 = cmalloc(1*sizeof(int));
    cmalloc_dump();
    cmalloc_free(vec1);
    cmalloc_free(vec2);
    cmalloc_free(vec3);
    printf("\n");
    vec1 = cmalloc(2*sizeof(int)+sizeof(cmalloc_chunk_meta));
    vec2 = cmalloc(1*sizeof(int));
    vec3 = cmalloc(1*sizeof(int));
    cmalloc_dump();
    printf("NO Cradsh\n");
    return 0;
}

// new_value = ( (old_value - old_min) / (old_max - old_min) ) * (new_max - new_min) + new_min
