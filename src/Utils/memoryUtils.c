#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../global_constants.h"

/**
 * Function to push a new element into an array.
 *
 * @param array The array to push the element into.
 * @param size The size of the array.
 * @param element_size The size of each element in the array.
 * @param new_element The new element to be pushed into the array.
 */
void push(void **array, size_t *size, size_t element_size, const void *new_element)
{
    (*size)++;

    *array = realloc(*array, (*size) * element_size);
    if (*array == NULL)
    {
        fprintf(stdout, "Memory allocation failed\n");
        exit(ERR_MEMORY_ALLOCATION_ERROR);
    }
    memcpy((char *)(*array) + ((*size) - 1) * element_size, new_element, element_size);
}
