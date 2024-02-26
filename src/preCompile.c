#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "preCompile.h"

int preCompile(const char *arg)
{
    FILE *source, *destination;

    char *fileName = (char *)calloc(strlen(arg) + 4, sizeof(char));
    if (!fileName)
    {
        fprintf(stderr, "Failed allocating memory, existing.\n");
        exit(1);
    }
    if ((source = fopen(fileName, "r")) == NULL)
    {
        fprintf(stderr, "Error! Failed open file %s\n", fileName);
        free(fileName);
        exit(1);
    }
    strcat(fileName, ".as");
    return 0;
}
