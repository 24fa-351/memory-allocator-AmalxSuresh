#include <stdio.h>
#include <stdlib.h>


#include "malloc.h"

char* my_malloc(size_t size) {
    char *ptr = malloc(size);
    if(ptr == NULL) {
        fprintf(stderr, __FILE__ ": %d malloc failed\n", __LINE__);
        exit(1);
    }
    fprintf(stderr, __FILE__ ": %d malloc (%lu) = %p\n", __LINE__, size, ptr);
    return ptr;
}

void my_free(void *ptr) {
    fprintf(stderr, __FILE__ ": free(%p)\n", ptr);
    free(ptr);
}