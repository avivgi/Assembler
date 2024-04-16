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

/**
 * @brief Handles errors that occur while reading a file.
 *
 * This function is called when an error occurs while reading a file. It prints an error message
 * along with the error number and exits the program with the given error number.
 *
 * @param signum The error number.
 */
void errors(int signum)
{
    printf("Error: an error occoured while compiling. Error code: %d. Existing.\n", signum);
    exit(signum);
}

/**
 * Registers signal handlers for various error signals.
 * When any of the specified signals are received, the 'errors' function will be called.
 */
void register_errors(void)
{
    signal(SIGSEGV, errors);
    signal(SIGFPE, errors);
    signal(SIGILL, errors);
    signal(SIGABRT, errors);
    signal(SIGTRAP, errors);
    signal(SIGBUS, errors);
    signal(SIGSYS, errors);
    signal(SIGXCPU, errors);
    signal(SIGXFSZ, errors);
}

/**
 * Safely frees a dynamically allocated array and its elements.
 *
 * This function takes a pointer to a dynamically allocated array and its length as parameters.
 * It iterates over each element of the array, frees the memory allocated for it, and sets the element to NULL.
 * Finally, it frees the memory allocated for the array itself.
 *
 * @param array   Pointer to the dynamically allocated array.
 * @param length  Length of the array.
 */
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
