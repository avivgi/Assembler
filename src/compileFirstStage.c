#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "stringUtils.h"
#include "compileFirstStage.h"
#include "global_constants.h"
#include "datamodel.h"
#include "memoryUtils.h"
#include "language.h"

int compileFirstStage(const char *filename, Data_model *data_model, Language *language)
{
    char *buffer = NULL;
    int result;
    // int data_count = 0, instruction_count = 0;
    FILE *source; // , *destination;
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
        parse_line(buffer, data_model, language);
        printf("%s  \n", data_model->line_params[0].parsed_params[0]);
        result = parse_command(buffer, command, first_param);

        /* step 3 - if type== define*/

        if (strcmp(command, ".define") == 0)
        {
            new_Symbol.type = MDEFINE;
            strcpy(new_Symbol.name, first_param);
            new_Symbol.value = 0;
            push((void **)data_model->symbols, &data_model->symbol_count, sizeof(Symbol), &new_Symbol);
        }

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
        printf("%d\n", result);
    }
    printf("finish first stage\n");

    /*step 16- if errors stop*/
    /*step 17- update data with value IC+100 in symbol table*/
    free(fullFileName);
    fclose(source);
    // fclose(destination);
    return 0;
}

int legalLabel(char *label)
{

    return 0;
}
