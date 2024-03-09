#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include "Utils/stringUtils.h"
#include "compileFirstStage.h"
#include "global_constants.h"
#include "datamodel.h"
#include "Utils/memoryUtils.h"
#include "language.h"
#include "Utils/languageUtils.h"
#include "compilationStages/createDefineSymbol.h"
#include "compilationStages/createSymbols.h"
#include "compilationStages/createExtern.h"

/**
 * Compiles the given file.
 *
 * @param filename The name of the file to compile.
 * @return Returns an integer indicating the success or failure of the compilation process.
 */
int compileFirstStage(const char *filename,
                      Symbol **symbols,
                      size_t *symbol_count,
                      Assembly_code **assembly_codes,
                      size_t *assembly_code_count,
                      Line_params **line_params,
                      size_t *line_params_count)
{
    char *buffer = NULL;
    int result = 0;
    int error_flag = 0;
    int is_symbol = 0;
    int str_len = 0;
    int data_count = 0;
    int instruction_count = 0;
    FILE *source;
    int i = 0;
    As_Command assembler_commands[NUM_OF_COMMANDS_IN_LANGUAGE] = AS_COMMAND_LIST;
    /*FILE *destination;*/
    /* char first_param[MAX_PARAM_SIZE];*/
    char *fullFileName = (char *)calloc(strlen(filename) + 4, sizeof(char));
    /* step 2 - read line */

    if (!fullFileName)
    {
        fprintf(stdout, "Failed allocating memory, existing.\n");
        exit(1);
    }
    strcpy(fullFileName, filename);
    strcat(fullFileName, ".am");

    if ((source = fopen(fullFileName, "r")) == NULL)
    {
        fprintf(stdout, "Error! Failed open file %s\n", fullFileName);
        free(fullFileName);
        exit(1);
    }

    while (read_line(source, &buffer))
    {

        parse_line(line_params, line_params_count, buffer, "\t\n\f\r ");

        /* step 3 && 4 - if type== define*/
        if (strcmp((*line_params)[*line_params_count - 1].parsed_params[0], ".define") == 0)
        {
            result = createDefineSymbol(symbols, symbol_count, line_params, line_params_count, &buffer);
            error_flag += result;
            continue;
        }

        /* printf("symbol_count %s\t", (*symbols)[*symbol_count - 1].name);
         fflush(stdout);

         printf("%s\t", command);
         printf("%s\n", first_param);*/

        /* step 4 - put define in mdefine table.*/
        /* step 5+6  - is sybmol ? */
        result = createSymbols(symbols, symbol_count, line_params, line_params_count, &instruction_count, &data_count, assembly_codes, assembly_code_count);
        /* step 7 - is data or string */
        /* step 8 - put symbol in symbol table */
        /* step 9 - identify data/params and put them in mem table (which?) update DC */
        /* step 10 - if extern or entry */
        if (result != 1)
            result = createExtern(symbols, symbol_count, line_params, line_params_count, &instruction_count, &data_count);
        if (result == 1)
        {
            continue;
        }

        /* step 11 - if extern put in etx table*/
        /* step 12 - if symbol put in symbol table*/
        /* step 13 - lookup operation in table*/
        result = ERR_WORD_NOT_FOUND;
        for (i = 0; i < NUM_OF_COMMANDS_IN_LANGUAGE; i++)
        {
            if (strcmp((*line_params)[*line_params_count - 1].parsed_params[0],
                       (assembler_commands[i].command_name)) == 0)
            {
                result = SYMBOL_WAS_FOUND;
                break;
            }
        }
        if (result == ERR_WORD_NOT_FOUND)
        {
            fprintf(stdout, "Error. Didn't find command name: %s\n",
                    (*line_params)[*line_params_count - 1].parsed_params[0]);
            continue;
        }
        /* step 14 - calculate L , build binary code of first word*/
        /* step 15 - IC = IC + L . goto #2*/
        /* printf("\n%d\n", result);*/
    }

    printf("finish first stage with error %d and result %d\n", error_flag, result);

    /*step 16- if errors stop*/
    /*step 17- update data with value IC+100 in symbol table*/
    free(fullFileName);
    fclose(source);
    /* fclose(destination);*/
    return 0;
}
