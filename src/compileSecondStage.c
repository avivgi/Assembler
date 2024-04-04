
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

    /* step 2 - read line */
    while (read_line(source, &buffer))
    {
        size_t line_params_count = 0;
        Line_params **line_params = (Line_params **)calloc(1, sizeof(Line_params *));

        if (!line_params)
            EXIT_ON_MEM_ALLOC_FAIL

        parse_line(*line_params, &line_params_count, buffer, "\t\n\f\r ");

        free(buffer);

        update_data_address(data_model);

        printf("finish first stage with error %d and result %d\n", error_flag, result);

        free(fullFileName);
        fclose(source);
        /* fclose(destination);*/
        return error_flag;
    }
    return 0;
}
