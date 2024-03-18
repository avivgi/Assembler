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

/**
 * Function to create an extern symbol.
 * @param symbols The array of symbols.
 * @param symbol_count The number of symbols in the array.
 * @param line_params The array of line parameters.
 * @param line_params_count The number of line parameters in the array.
 * @param instruction_count The number of instructions.
 * @param data_count The number of data.
 * @return 0 if the symbol was created, an error code otherwise.
 */
int createExtern(Data_model *data_model, Line_params **line_params, size_t *line_params_count)
{
    int str_len = 0;
    Symbol new_symbol;
    char *label_name;
    label_name = strdup((*line_params)[*line_params_count - 1].parsed_params[1]);
    if (label_name == NULL)
        EXIT_ON_MEM_ALLOC_FAIL

    str_len = strlen(label_name);
    if (str_len < 2)
    {
        free(label_name);
        return ERR_WORD_NOT_FOUND;
    }

    if ((legalLabel(label_name, &data_model->symbols, data_model->symbol_count)) != 0)
        return ERR_LABEL_OR_NAME_IS_TAKEN;

    if (strcmp((*line_params)[*line_params_count - 1].parsed_params[0], ".extern") == 0)
    {
        new_symbol.type = EXTERN; /*(~extern~)*/
        new_symbol.value = 0;     /*unclear what the value should be*/
        strcpy(new_symbol.name, label_name);
        push((void **)&(data_model->symbols), &(data_model->symbol_count), sizeof(Symbol), &new_symbol);
        free(label_name);
        return SYMBOL_WAS_FOUND;
    }
    /*
    else if (strcmp((*line_params)[*line_params_count - 1].parsed_params[0], ".entry") == 0)
    {
        new_symbol.type = ENTRY;
        new_symbol.value = 0; /*unclear what the value should be*/

    /*
    strcpy(new_symbol.name, label_name);
    push((void **)&(data_model->symbols), &(data_model->symbol_count), sizeof(Symbol), &new_symbol);
    free(label_name);
    return SYMBOL_WAS_FOUND;
    }
    */

    free(label_name);
    return ERR_WORD_NOT_FOUND;
}
