#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "assembler.h"
#include "preCompile.h"
#include "compile.h"
#include "Utils/memoryUtils.h"

/* AVIV GIDEONI & OMER RAZ OPEN UNIVERSITY 2024*/

/**
 * @file assembler.c
 * @brief This file contains the main function for the assembler program.
 *
 * The main function takes command line arguments and processes them as source files for the assembler.
 * It checks if any arguments are provided, and if not, it displays an error message and exits.
 * Then, it iterates through the provided source files and calls the handleSourceFile function for each file.
 * Finally, it exits the program.
 */

int main(int argc, char *argv[])
{
    int filesCount = argc - 1;
    int i = 1;
    int error_flag = 0;

    /* register_errors();*/
    if (filesCount < 1)
    {
        fprintf(stderr, "Error. No arguments received. Exiting.\n");
        exit(ERR_NO_ARGUMENTS);
    }

    while (filesCount--)
        error_flag += handleSourceFile(argv[i++]);

    exit(error_flag);
}

/**
 * Handles the given source file.
 * This function is called for each source file provided to the assembler.
 * It calls the preCompile and compile functions for the given source file.
 * @param arg The name of the source file to handle.
 */
int handleSourceFile(char *arg)
{
    int result;

    result = preCompile(arg);
    if (result != 0)
        return result;

    return compile(arg);
}
