
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
 *
 * @param filename The name of the file to compile.
 * @return Returns an integer indicating the success or failure of the compilation process.
 */
int compileSecondStage(const char *filename, Data_model *data_model)
{
    int result = 0;
    int error_flag = 0;
    FILE *source;
    char *buffer = NULL;
    /*    int L = 0; */
    size_t line_params_count = 0;
    Line_params line_params;
    char *fullFileName = (char *)calloc(strlen(filename) + 4, sizeof(char));

    if (!fullFileName)
        EXIT_ON_MEM_ALLOC_FAIL

    strcpy(fullFileName, filename);
    strcat(fullFileName, ".am");

    if ((source = fopen(fullFileName, "r")) == NULL)
    {
        fprintf(stdout, "Error! Failed open file %s\n", fullFileName);
        free(fullFileName);
        return (ERR_OPEN_FILE);
    }

    /*step 1*/

    /* resume this after promoting IC correctly
    data_model->instruction_count = 0;
     */

    /* step 2 - read line */

    while (read_line(source, &buffer))
    {
        char *entry_label_name = NULL;

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
        result = updateOperands(data_model, &line_params, line_params_count);
        /*step 8 - IC = IC + L*/

        /* resume this when really promoring ic*/
        /*         if (result == 1)
                     data_model->instruction_count += 1;
          */
    }
    /*step 9 - free line_params*/
    if (DEBUG)
        fprintf(stderr, "Finished second stage for %s with error %d and result %d\n", filename, error_flag, result);

    if (error_flag != 0)
        fprintf(stderr, "Compilation terminated after second stage due to errors.\n");

    fclose(source);
    safe_free(2, buffer, fullFileName);
    return error_flag;
}
