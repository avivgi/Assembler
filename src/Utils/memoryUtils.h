#ifndef __MEMORY_UTILS__
#define __MEMORY_UTILS__
#include <stdlib.h>
#define EXIT_ON_MEM_ALLOC_FAIL       \
    do                               \
    {                                \
        free_data_model(data_model); \
        EXIT_ON_MEM_ALLOC_FAIL_EARLY \
    } while (0);

#define EXIT_ON_MEM_ALLOC_FAIL_EARLY                                 \
    do                                                               \
    {                                                                \
        fprintf(stderr, "Error: Memory allocating failed. Exit.\n"); \
        exit(ERR_MEMORY_ALLOCATION_ERROR);                           \
    } while (0);

void push(void **array, size_t *size, size_t element_size, const void *new_element);
void safe_free(int num_args, ...);
void safe_free_array(void **array, size_t length);
void register_errors(void);
void errors(int signum);
#endif
