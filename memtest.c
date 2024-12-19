#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifdef SYSTEM_MALLOC
#define xfree free
#define xmalloc malloc
#define xrealloc realloc
#else
#include "malloc.h"
#endif

int random_range(int min, int max) {
    return rand() % (max - min + 1) + min;
}

void small_malloc_test(char *test_string, char *ptrs[]);
void big_malloc_test(char *test_string, char *ptrs[]);
void equal_size_malloc_test(char *test_string, char *ptrs[]);
void small_free_test(char *ptrs[]);
void realloc_test(char *test_string, char *ptrs[], size_t new_size);


#define TEST_SIZE 5
#define MIN(a, b) ((a) < (b) ? (a) : (b))

int main(int argc, char *argv[]) {
    srand(time(NULL));

    char *test_string = "This is a test for all the words and numbers and stuff to show up here.";

    if (argc > 1) {
        test_string = argv[1];
    }

    char *ptrs[TEST_SIZE] = {NULL}; // Initialize array to NULL

    // Allocate memory and copy test strings
    printf("small malloc test --------------------------------\n");
    small_malloc_test(test_string, ptrs);

    printf("big malloc test ------------------------------------\n");
    big_malloc_test(test_string, ptrs);
    // Free allocated memory
    printf("equal size malloc test -----------------------------\n");
    equal_size_malloc_test(test_string, ptrs);
    printf("free test -------------------------------------\n");
    small_free_test(ptrs);
    printf("realloc test -------------------------------------\n");
    realloc_test(test_string, ptrs, 10);

    return 0;
}

void small_malloc_test(char *test_string, char *ptrs[]) {
    for (int ix = 0; ix < TEST_SIZE; ix++) {
        int size = random_range(1, 30);
        fprintf(stderr, "[%d] size: %d\n", ix, size);

        ptrs[ix] = xmalloc(size);
        if (ptrs[ix] == NULL) {
            printf("[%d] malloc failed\n", ix);
            exit(1);
        }

        int len_to_copy = MIN(strlen(test_string), size - 1);

        fprintf(stderr, "[%d] ptrs[%d]: %p, going to copy %d chars\n", ix, ix, ptrs[ix], len_to_copy);

        strncpy(ptrs[ix], test_string, len_to_copy);
        ptrs[ix][len_to_copy] = '\0';

        fprintf(stderr, "[%d] %s\n", ix, ptrs[ix]);
    }
}

void big_malloc_test(char *test_string, char *ptrs[]) {
    for (int ix = 0; ix < TEST_SIZE; ix++) {
        int size = random_range(1024, 4096); // Allocate between 1KB and 4KB
        fprintf(stderr, "[%d] size: %d\n", ix, size);

        ptrs[ix] = xmalloc(size);
        if (ptrs[ix] == NULL) {
            printf("[%d] malloc failed\n", ix);
            exit(1);
        }

        int len_to_copy = MIN(strlen(test_string), size - 1);

        fprintf(stderr, "[%d] ptrs[%d]: %p, going to copy %d chars\n", ix, ix, ptrs[ix], len_to_copy);

        strncpy(ptrs[ix], test_string, len_to_copy);
        ptrs[ix][len_to_copy] = '\0';

        fprintf(stderr, "[%d] %s\n", ix, ptrs[ix]);
    }
}
 

 void equal_size_malloc_test(char *test_string, char *ptrs[]) {
    int size = strlen(test_string); // Fixed size for all allocations
    for (int ix = 0; ix < TEST_SIZE; ix++) {
        fprintf(stderr, "[%d] size: %d\n", ix, size);

        ptrs[ix] = xmalloc(size);
        if (ptrs[ix] == NULL) {
            printf("[%d] malloc failed\n", ix);
            exit(1);
        }

        int len_to_copy = MIN(strlen(test_string), size - 1);

        fprintf(stderr, "[%d] ptrs[%d]: %p, going to copy %d chars\n", ix, ix, ptrs[ix], len_to_copy);

        strncpy(ptrs[ix], test_string, len_to_copy);
        ptrs[ix][len_to_copy] = '\0';

        fprintf(stderr, "[%d] %s\n", ix, ptrs[ix]);
    }
}

void small_free_test(char* ptrs[]) {
    for (int ix = 0; ix < TEST_SIZE; ix++) {
        if (ptrs[ix] != NULL) { // Check for NULL before freeing
            void* temp = ptrs[ix]; // Store the address before freeing
            fprintf(stderr, "[%d] freeing %p (%s)\n", ix, temp, ptrs[ix]);
            xfree(ptrs[ix]);
            fprintf(stderr, "[%d] freed %p\n", ix, temp); // Print the original address
            ptrs[ix] = NULL; // Nullify the pointer to avoid dangling
        } else {
            fprintf(stderr, "[%d] pointer is NULL, skipping\n", ix);
        }
    }
}

void realloc_test(char *test_string, char *ptrs[], size_t new_size) {
    int size = strlen(test_string); 
    for (int ix = 0; ix < TEST_SIZE; ix++) {
        fprintf(stderr, "[%d] Initial size: %d\n", ix, size);

        ptrs[ix] = xmalloc(size);
        if (ptrs[ix] == NULL) {
            printf("[%d] malloc failed\n", ix);
            exit(1);
        }

        int len_to_copy = MIN(strlen(test_string), size - 1);
        strncpy(ptrs[ix], test_string, len_to_copy);
        ptrs[ix][len_to_copy] = '\0';
        fprintf(stderr, "[%d] Original content: %s, at address: %p\n", ix, ptrs[ix], ptrs[ix]);

        char *new_ptr = xrealloc(ptrs[ix], new_size);
        if (new_ptr == NULL) {
            printf("[%d] realloc failed\n", ix);
            exit(1);
        }

        ptrs[ix] = new_ptr;

        fprintf(stderr, "[%d] Reallocated ptr: %p, New size: %zu\n", ix, ptrs[ix], new_size);
        fprintf(stderr, "[%d] Content after realloc: %s\n", ix, ptrs[ix]);

        ptrs[ix][new_size - 1] = '\0';

        fprintf(stderr, "[%d] Final content after realloc: %s\n", ix, ptrs[ix]);
    }
}


