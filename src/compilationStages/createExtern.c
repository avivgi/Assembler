#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include "createSymbols.h"
#include "../Utils/stringUtils.h"
#include "../Utils/languageUtils.h"
#include "../global_constants.h"
#include "../Utils/memoryUtils.h"

#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include "createSymbols.h"
#include "../Utils/stringUtils.h"
#include "../Utils/languageUtils.h"
#include "../global_constants.h"
#include "../Utils/memoryUtils.h"

int createExtern(Symbol **symbols, size_t *symbol_count, Line_params **line_params, size_t *line_params_count, int *instruction_count, int *data_count)
{
    int str_len = 0;
    Symbol new_symbol;
    char *label_name;
    label_name = strdup((*line_params)[*line_params_count - 1].parsed_params[1]);
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

    if ((legalLabel(label_name, symbols, *symbol_count)) != 0)
        return ERR_LABEL_OR_NAME_IS_TAKEN;

    if (strcmp((*line_params)[*line_params_count - 1].parsed_params[0], ".extern") == 0)
    {
        new_symbol.type = EXTERN; /*(~extern~)*/
        new_symbol.value = 0;     /*unclear what the value should be*/
        strcpy(new_symbol.name, label_name);
        push((void **)symbols, symbol_count, sizeof(Symbol), &new_symbol);
        free(label_name);
        return SYMBOL_WAS_FOUND;
    }
    else if (strcmp((*line_params)[*line_params_count - 1].parsed_params[0], ".entry") == 0)
    {
        new_symbol.type = ENTRY;
        new_symbol.value = 0; /*unclear what the value should be*/
        strcpy(new_symbol.name, label_name);
        push((void **)symbols, symbol_count, sizeof(Symbol), &new_symbol);
        free(label_name);
        return SYMBOL_WAS_FOUND;
    }

    free(label_name);
    return ERR_WORD_NOT_FOUND;
}
