#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../global_constants.h"

void push(void **array, size_t *size, size_t element_size, const void *new_element)
{
    (*size)++;

    *array = realloc(*array, (*size) * element_size);
    if (*array == NULL)
    {
        fprintf(stderr, "Memory allocation failed\n");
        exit(ERR_MEMORY_ALLOCATION_ERROR);
    }
    memcpy((char *)(*array) + ((*size) - 1) * element_size, new_element, element_size);
}
