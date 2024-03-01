#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void push(void **array, size_t *size, size_t element_size, const void *new_element)
{
    (*size)++;

    *array = realloc(*array, (*size) * element_size);
    if (*array == NULL)
    {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    memcpy((char *)(*array) + ((*size) - 1) * element_size, new_element, element_size);
}