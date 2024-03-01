#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "stringUtils.h"
#include "compileFirstStage.h"
#include "global_constants.h"
#include "datamodel.h"

int compileFirstStage(const char *filename, Symbol *symbols, Opcode *opcodes)
{
    char *buffer = NULL;
    int result;

    /* step 2 - read line */
    int data_count = 0, instruction_count = 0;
    FILE *source, *destination;
    char command[MAX_PARAM_SIZE];
    char first_param[MAX_PARAM_SIZE];

    char *fullFileName = (char *)calloc(strlen(filename) + 4, sizeof(char));

    if (!fullFileName)
    {
        fprintf(stderr, "Failed allocating memory, existing.\n");
        exit(1);
    }
    strcpy(fullFileName, filename);
    strcat(fullFileName, ".am");

    if ((source = fopen(fullFileName, "r")) == NULL)
    {
        fprintf(stderr, "Error! Failed open file %s\n", fullFileName);
        free(fullFileName);
        exit(1);
    }

    while (read_line(source, &buffer))
    {

        // char *token;

        // token = strtok(lineCopy, "\t\n\f\r ");
        // while (token != NULL)
        // {
        //     printf("%s\n", token);
        //     token = strtok(NULL, " \t\n\f\r");
        // }

        result = parse_command(buffer, command, first_param);
        //    printf("%s\t --> ", buffer);

        /* step 3 - if type== define*/

        if (strcmp(command, ".define") == 0)
        {
            // symbol.type = MDEFINE;
            // push(symbols, symbol);
        }
        printf("%s\t", command);
        printf("%s\n", first_param);

        /* step 4 - put define in mdefine table.*/
        /* step 5+6  - is sybmol ? */
        /* step 7 - is data or string */
        /* step 8 - put symbol in symbol table */
        /* step 9 - identify data/params and put them in mem table (which?) update DC */
        /* step 10 - if extern or entry */
        /* step 11 - if extern put in etx table*/
        /* step 12 - if symbol put in symbol table*/
        /* step 13 - lookup operation in table*/
        /* step 14 - calculate L , build binary code of first word*/
        /* step 15 - IC = IC + L . goto #2*/
    }
    /*step 16- if errors stop*/
    /*step 17- update data with value IC+100 in symbol table*/
    free(fullFileName);
    fclose(source);
    // fclose(destination);
    return 0;
}
