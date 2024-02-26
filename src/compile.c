#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "stringUtils.h"
#include "compile.h"
#include "global_constants.h"

int preCompile(const char *arg)
{

    /* step 1*/
    int data_count = 0, instruction_count = 0;
             FILE *source, *destination;
             char buffer [MAX_LINE_LENGTH];

    char *fileName = (char *)calloc(strlen(arg) + 4, sizeof(char));

    if (!fileName)
    {
        fprintf(stderr, "Failed allocating memory, existing.\n");
        exit(1);
    }
    strcpy(fileName, arg);
    strcat(fileName, ".am");

    if ((source = fopen(fileName, "r")) == NULL)
    {
        fprintf(stderr, "Error! Failed open file %s\n", fileName);
        free(fileName);
        exit(1);
    }
    /* step 2 - read line */

    while (read_line(source, **buffer))
    { 
        char buffer[MAX_LINE_LENGTH];
        read_line(source, **buffer);
        printf(buffer);
    }

    strcat(fileName, ".ob");
    if ((destination = fopen(fileName, "rw")) == NULL)
    {
        fprintf(stderr, "Error! Failed open file %s\n", fileName);
        free(fileName);
        exit(1);
    }

    return 0;
}
