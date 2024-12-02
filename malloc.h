#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#ifndef MALLOC_H
#define MALLOC_H


#define MAX_HEAP_SIZE 1024  

typedef struct chunk_on_heap {
    int size;                
    char* pointer_to_start;  
} chunk;

static chunk minheap[MAX_HEAP_SIZE];
static size_t heap_count = 0;          

void* xmalloc(size_t size);
void xfree(void *ptr);
void* xrealloc(void *ptr, size_t new_size);

static void swap(chunk* chunk_one, chunk* chunk_two);

static size_t parent(size_t heap_chunk);
static size_t left_child(size_t heap_chunk);
static size_t right_child(size_t heap_chunk);
void heap_insert(chunk place_chunk);

#endif