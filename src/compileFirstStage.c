#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "stringUtils.h"
#include "compileFirstStage.h"
#include "global_constants.h"
#include "datamodel.h"
#include "memoryUtils.h"

int compileFirstStage(const char *filename, Symbol **symbols, size_t *symbol_count, Opcode **opcodes, size_t *opcode_count)
{
    char *buffer = NULL;
    int result;
    int data_count = 0, instruction_count = 0;
    FILE *source, *destination;
    Symbol new_Symbol;
    char command[MAX_PARAM_SIZE];
    char first_param[MAX_PARAM_SIZE];
    char *fullFileName = (char *)calloc(strlen(filename) + 4, sizeof(char));
    /* step 2 - read line */

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
            new_Symbol.type = MDEFINE;
            strcpy(new_Symbol.name, first_param);
            new_Symbol.value = 0;
            push((void **)symbols, symbol_count, sizeof(Symbol), &new_Symbol);
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
    printf("finish first stage\n");

    /*step 16- if errors stop*/
    /*step 17- update data with value IC+100 in symbol table*/
    free(fullFileName);
    fclose(source);
    // fclose(destination);
    return 0;
}
