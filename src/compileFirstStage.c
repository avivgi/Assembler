#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include "stringUtils.h"
#include "compileFirstStage.h"
#include "global_constants.h"
#include "datamodel.h"
#include "memoryUtils.h"
#include "language.h"

int legalLabel(char *label_name, Symbol **symbols, size_t symbol_count) /*not good enough*/
{
    As_Command assember_commands[NUM_OF_COMMANDS_IN_LANGUAGE] = AS_COMMAND_LIST;
    const char registers[NUNMER_OF_REGISTERS][3] = RESIGTERS;

    int i;
    if (isalpha(label_name[0]) == 0)
    {
        fprintf(stderr, "The first character of the label isn't a letter\n");
        return FIRST_LETTER_IS_NOT_A_LETTER;
    }
    for (i = 0; i < NUM_OF_COMMANDS_IN_LANGUAGE; i++)
    {
        if (strcmp(label_name, (assember_commands[i].command_name)) == 0)
        {
            fprintf(stderr, "The name of the label is already taken\n");
            return ERR_LABEL_OR_NAME_IS_TAKEN; /*illegal*/
        }
    }

    for (i = 0; i < NUNMER_OF_REGISTERS; i++)
    {
        if (strcmp(label_name, (registers[i])) == 0)
        {
            fprintf(stderr, "The name of the label is already taken\n");

            return ERR_LABEL_OR_NAME_IS_TAKEN; /*illegal*/
        }
    }
    for (i = 0; i < symbol_count; i++)
    {
        if (strcmp(label_name, (symbols[i]->name)) == 0)
        {
            fprintf(stderr, "The name of the label is already taken\n");
            return ERR_LABEL_OR_NAME_IS_TAKEN;
        }
    }
    /*for (i = 0; i < macro_amount;i++){
        if(strcmp(label_name, (macro_names[i])) != 0){
            return ERR_LABEL_OR_NAME_IS_TAKEN; ~illegal~}
    }*/

    if (strcmp(label_name, ".data") == 0)
    {
        fprintf(stderr, "Illegal name for label\n");
        return ERR_LABEL_OR_NAME_IS_TAKEN; /*illegal*/
    }
    if (strcmp(label_name, ".string") == 0)
    {
        fprintf(stderr, "Illegal name for label\n");
        return ERR_LABEL_OR_NAME_IS_TAKEN; /*illegal*/
    }
    if (strcmp(label_name, ".entry") == 0)
    {
        fprintf(stderr, "Illegal name for label\n");
        return ERR_LABEL_OR_NAME_IS_TAKEN; /*illegal*/
    }
    if (strcmp(label_name, ".extern") == 0)
    {
        fprintf(stderr, "Illegal name for label\n");
        return ERR_LABEL_OR_NAME_IS_TAKEN; /*illegal*/
    }

    return 0;
}

int compileFirstStage(const char *filename, Symbol **symbols, size_t *symbol_count, Opcode **opcodes, size_t *opcode_count, Line_params **line_params, size_t *line_params_count)
{
    char *buffer = NULL;
    int result = 0;
    int error_flag = 0;
    // int data_count = 0, instruction_count = 0;
    FILE *source;
    /*FILE *destination;*/
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
        Symbol new_Symbol;
        char *endptr;
        long numeralValue;
        parse_line(line_params, line_params_count, buffer);

        /* step 3 - if type== define*/
        if (strcmp((*line_params)[*line_params_count - 1].parsed_params[0], ".define") == 0)
        {
            if ((legalLabel((*line_params)[*line_params_count - 1].parsed_params[1], symbols, *symbol_count)) == 0)
            {
                new_Symbol.type = MDEFINE;
                strcpy(new_Symbol.name, (*line_params)[*line_params_count - 1].parsed_params[1]);
                // new_Symbol.value = atoi((line_params)[*line_params_count - 1]->parsed_params[3]);

                numeralValue = strtol((*line_params)[*line_params_count - 1].parsed_params[3], &endptr, 10);
                if ((errno == ERANGE && (numeralValue == LONG_MAX || numeralValue == LONG_MIN)) || (errno != 0 && numeralValue == 0))
                {
                    fprintf(stderr, "Define parmater isn't a number\n");
                    error_flag = -1;
                    continue;
                }
                if (endptr == (*line_params)[*line_params_count - 1].parsed_params[3])
                {
                    fprintf(stderr, "No digits were found\n");
                    error_flag = -1;
                    continue;
                }
                new_Symbol.value = numeralValue;
                push((void **)symbols, symbol_count, sizeof(Symbol), &new_Symbol);
            }
            else
            {
                error_flag = -1;
                result = -1;
            }
            printf("symbol_count %s\t", (*symbols)[*symbol_count - 1].name);
            fflush(stdout);

            // printf("%s\t", command);
            // printf("%s\n", first_param);

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
            printf("\n%d\n", result);
        }
    }
    printf("finish first stage\n");

    /*step 16- if errors stop*/
    /*step 17- update data with value IC+100 in symbol table*/
    free(fullFileName);
    fclose(source);
    // fclose(destination);
    return 0;
}
