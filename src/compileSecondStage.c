
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
char *strdup(const char *s);

/**
 * Compiles the given file.
 * step 1 - reset IC
 * step 2 - read line
 * step 3 - if type == define continue
 * step 4 - if first word is label, skip it, if this is .string .extern or .data continue
 * step 5 - is it .entry label? if so mark symbols as entry
 * step 6 - update 2nd - 4th operands
 * step 8 - free line_params
 * step 9 - repeat from step 2
 * step 10 - free buffer and fullFileName
 * step 11 - return error_flag
 * @param filename The name of the file to compile.
 * @return Returns an integer indicating the success or failure of the compilation process.
 */
int compileSecondStage(const char *filename, Data_model *data_model)
{
    int error_flag = 0;
    int result = 0;
    FILE *source;
    char *buffer = NULL;
    /*    int L = 0; */
    size_t line_params_count = 0;
    Line_params line_params;
    char *fullFileName = (char *)calloc(strlen(filename) + 4, sizeof(char));
    data_model->line_number = 0;

    if (!fullFileName)
        EXIT_ON_MEM_ALLOC_FAIL

    strcpy(fullFileName, filename);
    strcat(fullFileName, ".am");

    if ((source = fopen(fullFileName, "r")) == NULL)
    {
        fprintf(stderr, "Error: Error! Failed open file %s\n", fullFileName);
        free(fullFileName);
        return (ERR_OPEN_FILE);
    }

    /*step 1*/
    data_model->instruction_count = 0;

    /* step 2 - read line */

    while (read_line(source, &buffer))
    {
        char *entry_label_name = NULL;

        data_model->line_number++;

        parse_line(&line_params, &line_params_count, buffer, "\t\n\f\r ");

        /* step 3 && 4 - if type== define continue*/
        if (strcmp((line_params).parsed_params[0], ".define") == 0)
            continue;

        /*step 3-4 if first word is label, skip it, if this is .string .extern or .data continue*/
        if (is_label_data_extern_string(line_params, line_params_count) == 1)
            continue;

        /* step 5-6 is it .entry label? if so mark symbols as entry*/
        entry_label_name = strdup(get_label_entry(*data_model, line_params, line_params_count));
        if (entry_label_name == NULL)
            EXIT_ON_MEM_ALLOC_FAIL

        if (strcmp(entry_label_name, "0") != 0) /* this is a valid entry symbol */
        {
            update_entry_symbol(data_model, entry_label_name);
            continue;
        }
        safe_free(1, entry_label_name);

        /* step 7 update 2nd - 4th operands */
        printf("line number %d\n", data_model->line_number);
        result += updateOperands(data_model, &line_params, line_params_count);
        if (result < 0)
        {
            error_flag += 1;
            continue;
        }

        /*step 8 - IC = IC + L*/
    }
    /*step 9 - free line_params*/
    if (DEBUG)
        fprintf(stderr, "Finished second stage for %s with error(s) %d.\n", filename, error_flag);

    fclose(source);
    safe_free(2, buffer, fullFileName);
    safe_free_array((void *)(line_params).parsed_params, (line_params).param_count);
    return error_flag;
}
