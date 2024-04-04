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
#include "global_constants.h"
#include "compilationStages/createDefineSymbol.h"
#include "compilationStages/labels.h"
#include "compilationStages/externs.h"
#include "compilationStages/commands.h"
#include "compilationStages/updateDataSymbols.h"
#include "Utils/printUtils.h"

/**
 * Compiles the given file.
 *
 * @param filename The name of the file to compile.
 * @return Returns an integer indicating the success or failure of the compilation process.
 */
int compileFirstStage(const char *filename, Data_model *data_model)
{

    char *buffer = NULL;
    int result = 0;
    int error_flag = 0;
    FILE *source;
    /*FILE *destination;*/
    /* char first_param[MAX_PARAM_SIZE];*/
    char *fullFileName = (char *)calloc(strlen(filename) + 4, sizeof(char));

    if (!fullFileName)
        EXIT_ON_MEM_ALLOC_FAIL

    strcpy(fullFileName, filename);
    strcat(fullFileName, ".am");

    if ((source = fopen(fullFileName, "r")) == NULL)
    {
        fprintf(stdout, "Error! Failed open file %s\n", fullFileName);
        safe_free(1, fullFileName);
        return (ERR_OPEN_FILE);
    }

    /* step 2 - read line */
    while (read_line(source, &buffer))
    {
        size_t line_params_count = 0;
        Line_params line_params;
        parse_line(&line_params, &line_params_count, buffer, "\t\n\f\r ");

        /* step 3 && 4 - if type== define put define in mdefine table*/
        if (strcmp((line_params).parsed_params[0], ".define") == 0)
        {
            result = createDefineSymbol(data_model, &line_params, line_params_count, &buffer);
            error_flag += result;
            continue;
        }

        /* step 5+6  - labels   */
        result = labels(data_model, &line_params, line_params_count);
        if (result == ERR_LABEL_OR_NAME_IS_TAKEN)
            error_flag += 1;
        if (result == LABEL_DATA_WAS_FOUND)
            continue;

        /* step 7 - is data or string */
        /* step 8 - put symbol in symbol table */
        /* step 9 - identify data/params and put them in mem table (which?) update DC */
        /* step 10 - if extern or entry */
        /* step 11 - if extern put in etx table*/
        /* step 12 - if symbol put in symbol table*/

        result = externs(data_model, &line_params, line_params_count);

        if (result == EXTERN_FOUND_AND_ADDED_WITH_ERRORS)
            error_flag += 1;

        if (result == EXTERN_FOUND_AND_ADDED_WITH_ERRORS || result == EXTERN_FOUND_AND_ADDED)
            continue;

        /* not extern, a command*/
        /* step 13 - lookup operation in table*/
        /* step 14 - calculate L , build binary code of first word*/
        /* step 15 - IC = IC + L . goto #2*/

        result = commands(data_model, &line_params, line_params_count);
        if (result == ERR_WORD_NOT_FOUND)
        {
            error_flag += 1;
            continue;
        }

        /* printf("\n%d\n", result);*/
        safe_free_array((void *)(line_params).parsed_params, (line_params).param_count);
        safe_free(1, buffer);
    }
    update_data_address(data_model);
    printf("Finished first stage for %s with error %d and result %d\n", filename, error_flag, result);

    /*step 16- if errors stop*/
    if (error_flag != 0)
    {
        if (source == NULL)
        {
            fclose(source);
        }
        safe_free(1, fullFileName);
        return error_flag;
    }
    safe_free(1, fullFileName);

    /*step 17- update data with value IC+100 in symbol table*/
    if (source == NULL)
        fclose(source);

    return error_flag;
}
