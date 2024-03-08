
#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include "createSymbols.h"
#include "../Utils/stringUtils.h"
#include "../Utils/languageUtils.h"
#include "../global_constants.h"
#include "../Utils/memoryUtils.h"

int createSymbols(Symbol **symbols, size_t *symbol_count, Line_params **line_params, size_t *line_params_count, int *instruction_count, int *data_count)
{
    int str_len = 0;
    int is_symbol = 0;
    Symbol new_symbol;
    char *label_name;
    label_name = strdup((*line_params)[*line_params_count - 1].parsed_params[0]);
    if (label_name == NULL)
    {
        fprintf(stderr, "Error Allocating Memory, exiting\n");
        exit(ERR_MEMORY_ALLOCATION_ERROR);
    }
    str_len = strlen(label_name);
    if (str_len < 2)
    {
        free(label_name);
        return ERR_WORD_NOT_FOUND;
    }

    if ((*line_params)[*line_params_count - 1].parsed_params[0][str_len - 1] != ':')
    {
        free(label_name);
        return ERR_WORD_NOT_FOUND;
    }
    label_name[str_len - 1] = '\0';
    str_len--;
    is_symbol = 1;

    if ((legalLabel(label_name, symbols, *symbol_count)) != 0)
        return ERR_LABEL_OR_NAME_IS_TAKEN;

    /*check if data or string*/
    if ((strcmp((*line_params)[*line_params_count - 1].parsed_params[1], ".data") == 0) ||
        (strcmp((*line_params)[*line_params_count - 1].parsed_params[1], ".string") == 0))
    {
        new_symbol.type = DATA;
        new_symbol.value = *data_count;
        // fix to reflect size of data
        (*data_count)++;
    }
    else
    {
        new_symbol.type = CODE;
        new_symbol.value = *instruction_count;
        // fix to reflect size of instruction
        (*instruction_count)++;
    }
    strcpy(new_symbol.name, label_name);
    push((void **)symbols, symbol_count, sizeof(Symbol), &new_symbol);
    free(label_name);
    return SYMBOL_WAS_FOUND;
}
