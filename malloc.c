#include <stdio.h>
#include <stdlib.h>


#include "malloc.h"

static chunk minheap[MAX_HEAP_SIZE];
static size_t free_chunks = 0; 

void* xmalloc(size_t size) {
    chunk allocated_chunk = {0, NULL};

    //Search for a suitable chunk in the minheap
    for (size_t ix = 0; ix < free_chunks; ix++) {
        if (minheap[ix].size >= size) {
            allocated_chunk = minheap[ix];

            // Remove the chunk from the heap
            for (size_t iy = ix; iy < free_chunks - 1; iy++) {
                minheap[iy] = minheap[iy + 1];
            }
            free_chunks--;
            break;
        }
    }

    // If no suitable chunk is found, allocate new memory
    if (allocated_chunk.pointer_to_start == NULL) {
        void* current_break = sbrk(0); 
        void* new_memory = sbrk(size + sizeof(chunk)); 

        if (new_memory == (void*)-1) { 
            fprintf(stderr, "Out of memory\n");
            return NULL;
        }

        allocated_chunk.pointer_to_start = (char*)new_memory;
        allocated_chunk.size = size + sizeof(chunk);
    }

    //If the allocated chunk is larger than requested, split it
    if (allocated_chunk.size > size + sizeof(chunk)) {
        chunk leftover = {
            .size = allocated_chunk.size - size - sizeof(chunk),
            .pointer_to_start = allocated_chunk.pointer_to_start + size + sizeof(chunk),
        };

        allocated_chunk.size = size;
        heap_insert(leftover); 
    }

    return allocated_chunk.pointer_to_start;
}

//free memory -> put back in heap
void xfree(void *ptr) {
    if(ptr == NULL) {
        return;
    }

    chunk *freed_memory = (chunk*)((char*)ptr - sizeof(chunk));

    heap_insert(*freed_memory);
}

void* xrealloc(void *ptr, size_t new_size) {
    if (ptr == NULL) {
        return xmalloc(new_size);
    }

    if (new_size == 0) {
        xfree(ptr);
        return NULL;
    }

    chunk *current_chunk = (chunk *)((char *)ptr - sizeof(chunk));

    if (current_chunk -> size >= new_size) {
        return ptr;
    }

    void *new_block = xmalloc(new_size);
    if (new_block == NULL) {
        return NULL;
    }

    memcpy(new_block, ptr, current_chunk -> size);
    xfree(ptr);

    return new_block;
}


static void swap(chunk* chunk_one, chunk* chunk_two) {
    chunk temp = *chunk_one;
    *chunk_one = *chunk_two;
    *chunk_two = temp;
}

static size_t parent(size_t heap_chunk) {
    return (heap_chunk - 1) / 2;
}
static size_t left_child(size_t heap_chunk) {
    return 2 * heap_chunk + 1;
}
static size_t right_child(size_t heap_chunk) {
    return 2 * heap_chunk + 2;
}
void heap_insert(chunk place_chunk) {
    if(free_chunks >= MAX_HEAP_SIZE) {
        fprintf(stderr, "Heap overflow\n");
        return;
    }

    minheap[free_chunks] = place_chunk;
    size_t current = free_chunks++;

    while (current > 0 && minheap[current].size < minheap[parent(current)].size) {
        swap(&minheap[current], &minheap[parent(current)]);
        current = parent(current);
    }
}
