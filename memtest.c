#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "malloc.h"

int random_range(int min, int max) {
    return rand() % (max - min + 1) + min;
}

#define TEST_SIZE 10

#define MIN(a, b) ((a) < (b) ? (a) : (b))

int main(int argc, char *argv[]) {
    srand(time(NULL));

    char* test_string = "This is a test for all the words and numbers and stuff to show up here.";

    if(argc > 1) {
        test_string = argv[1];
    }

    char *ptrs[TEST_SIZE];

    for(int ix = 0; ix < TEST_SIZE; ix++) {
        int size = random_range(1, 30);
        fprintf(stderr, "[%d] size: %d\n", ix, size);

        ptrs[ix] = malloc(size);
        if(ptrs[ix] == NULL) {
            printf("[%d] malloc failed\n", ix);
            exit(1);
        }

        int len_to_copy = MIN(strlen(test_string), size - 1);

        fprintf(stderr, "[%d] ptrs[%d]: %p, going to copy %d chars\n", ix, ix, ptrs[ix], len_to_copy);

        strncpy(ptrs[ix], test_string, len_to_copy);
        ptrs[ix][len_to_copy] = '\0';

        fprintf(stderr, "[%x] %s\n", ix, ptrs[ix]);
    }

    for(int ix = 0; ix < TEST_SIZE; ix++) {
        fprintf(stderr, "[%d] freeing %p (%s)\n", ix, ptrs[ix], ptrs[ix]);
        free(ptrs[ix]);
        fprintf(stderr, "[%d] freed %p\n", ix, ptrs[ix]);
    }


}