
#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include "createDefineSymbol.h"
#include "../Utils/stringUtils.h"
#include "../Utils/languageUtils.h"
#include "../global_constants.h"
#include "../Utils/memoryUtils.h"

int createDefineSymbol(Symbol **symbols, size_t *symbol_count, Line_params **line_params, size_t *line_params_count, char **buffer)
{
    Symbol new_symbol;
    int value;
    char *endptr;
    char *param = NULL;
    char *define_string = mid(*buffer, strlen(".define "), 0);

    clean_white_space(&define_string);

    /*redo parsing of last line, now with = as a seperator*/
    (*line_params_count)--;
    parse_line(line_params, line_params_count, define_string, "=");

    /* printf("###%s### -> %s\n", (*line_params)[*line_params_count - 1].parsed_params[0], (*line_params)[*line_params_count - 1].parsed_params[1]);*/

    if ((legalLabel((*line_params)[*line_params_count - 1].parsed_params[0], symbols, *symbol_count)) == 0)
    {
        new_symbol.type = MDEFINE;
        strcpy(new_symbol.name, (*line_params)[*line_params_count - 1].parsed_params[0]);

        param = strdup((*line_params)[*line_params_count - 1].parsed_params[1]);
        if (param == NULL)
        {
            free(define_string);
            EXIT_ON_MEM_ALLOC_FAIL
        }
        value = (int)strtol(param, &endptr, 10);

        if ((errno == ERANGE) || (errno != 0 && value == 0))
        {
            fprintf(stderr, "Define parameter isn't a number\n");
            free(param);
            free(define_string);
            return ERR_VARIABLE_ISNT_INTEGER;
        }
        if (endptr == (*line_params)[*line_params_count - 1].parsed_params[3])
        {
            fprintf(stderr, "No digits were found\n");
            free(define_string);
            free(param);
            return ERR_VARIABLE_ISNT_INTEGER;
        }
        new_symbol.value = value;
        push((void **)symbols, symbol_count, sizeof(Symbol), &new_symbol);
    }
    else
    {
        return ERR_LABEL_OR_NAME_IS_TAKEN;
    }
    free(define_string);
    free(param);
    return 0;
}
