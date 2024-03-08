#ifndef __MEMORY_UTILS__
#define __MEMORY_UTILS__
#include <stdlib.h>

void *realloc_safe(void *ptr, size_t size);
void *malloc_safe(size_t size);
void *calloc_safe(size_t nmemb, size_t size);
void free_safe(void *ptr);

#define EXIT_ON_MEM_ALLOC_FAIL                                   \
    fprintf(stderr, "Error. Memory allocating failed. Exit.\n"); \
    exit(ERR_MEMORY_ALLOCATION_ERROR);

void push(void **array, size_t *size, size_t element_size, const void *new_element);

#endif
