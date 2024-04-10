#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../global_constants.h"
#include <stdarg.h>

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
        fprintf(stderr, "Error: Memory allocation failed\n");
        exit(ERR_MEMORY_ALLOCATION_ERROR);
    }
    memcpy((char *)(*array) + ((*size) - 1) * element_size, new_element, element_size);
}

/**
 * Safely frees memory allocated for passed variables.
 *
 * @param num_args The number of arguments to free.
 * @param ... The list of pointers to free.
 */
void safe_free(int num_args, ...)
{
    int i;
    va_list args;
    va_start(args, num_args);
    for (i = 0; i < num_args; i++)
    {
        void *ptr = va_arg(args, void *);
        if (ptr)
        {
            free(ptr);
            ptr = NULL;
        }
    }
    va_end(args);
}

void safe_free_array(void **array, size_t length)
{
    int i;
    if (array == NULL)
        return;

    for (i = 0; i < length; i++)
    {
        if (array[i] == NULL)
            continue;
        free(array[i]);
        array[i] = NULL;
    }
    free(array);
}
