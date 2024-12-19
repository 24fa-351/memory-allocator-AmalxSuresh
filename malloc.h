#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#ifndef MALLOC_H
#define MALLOC_H



typedef struct chunk_on_heap {
    int size;                
    char* pointer_to_start;  
} chunk;

//unsigned int heap_count = 0;          

void* xmalloc(size_t size);
void xfree(void *ptr);
void* xrealloc(void *ptr, size_t new_size);

void swap(chunk* chunk_one, chunk* chunk_two);

unsigned int parent(unsigned int heap_chunk);
unsigned int left_child(unsigned int heap_chunk);
unsigned int right_child(unsigned int heap_chunk);
void heap_insert(chunk place_chunk);

#endif