
#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include "createDefineSymbol.h"
#include "../Utils/stringUtils.h"
#include "../Utils/languageUtils.h"
#include "../global_constants.h"
#include "../Utils/memoryUtils.h"
char *strdup(const char *s);

/**
 * Function to create a .define symbol.
 * @param symbols The array of symbols.
 * @param symbol_count The number of symbols in the array.
 * @param line_params The array of line parameters.
 * @param line_params_count The number of line parameters in the array.
 * @param buffer The buffer to parse.
 * @return 0 if the symbol was created, an error code otherwise.
 */
int createDefineSymbol(Data_model *data_model, Line_params *line_params, size_t line_params_count, char **buffer)
{
    Symbol new_symbol;
    int value;
    char *endptr;
    char *param = NULL;
    char *define_string = mid(*buffer, strlen(".define "), 0);

    clean_white_space(&define_string);

    /*redo parsing of last line, now with = as a seperator*/
    (line_params_count)--;
    parse_line(line_params, &line_params_count, define_string, "=");

    /* printf("###%s### -> %s\n", (*line_params)[*line_params_count - 1].parsed_params[0], (*line_params)[*line_params_count - 1].parsed_params[1]);*/

    if ((legalLabel((*line_params).parsed_params[0], &data_model->symbols, data_model->symbol_count)) == 0)
    {
        new_symbol.type = MDEFINE;
        strcpy(new_symbol.name, (*line_params).parsed_params[0]);

        param = strdup((*line_params).parsed_params[1]);
        if (param == NULL)
        {
            EXIT_ON_MEM_ALLOC_FAIL
        }
        value = (int)strtol(param, &endptr, 10);

        if ((errno == ERANGE) || (errno != 0 && value == 0))
        {
            fprintf(stdout, "Define parameter isn't a number\n");
            safe_free(2, param, define_string);
            return ERR_VARIABLE_ISNT_INTEGER;
        }
        if (endptr == (*line_params).parsed_params[3])
        {
            fprintf(stdout, "No digits were found\n");
            safe_free(2, define_string, param);
            return ERR_VARIABLE_ISNT_INTEGER;
        }
        new_symbol.value = value;
        printf("Define symbol: %s, value: %d\n", new_symbol.name, new_symbol.value);
        push((void **)&(data_model->symbols), &(data_model->symbol_count), sizeof(Symbol), &new_symbol);
    }
    else
    {
        return ERR_LABEL_OR_NAME_IS_TAKEN;
    }
    safe_free(2, define_string, param);

    return 0;
}
