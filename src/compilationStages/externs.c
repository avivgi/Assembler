#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include "externs.h"

#include "../Utils/stringUtils.h"
#include "../Utils/languageUtils.h"
#include "../global_constants.h"
#include "../Utils/memoryUtils.h"
#include "../Utils/stringUtils.h"
#include "../Utils/languageUtils.h"
#include "../global_constants.h"
#include "../Utils/memoryUtils.h"

char *strdup(const char *s);

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
int externs(Data_model *data_model, Line_params *line_params, size_t line_params_count)
{

    /*
    possible returns:
    NOT EXTERN
    EXTERN FOUND AND ADDED
    EXTERN FOUND AND ADDED WITH ERRORS
    */
    Symbol new_symbol;
    char *label_name;
    int result = -1;
    Bool error_flag = false;
    Bool found_label = false;
    int i = 0;

    while (i < (*line_params).param_count && result == -1)
    {
        if (strcmp((*line_params).parsed_params[i], ".extern") == 0)
            result = i;
        i++;
    }
    if (result == -1)
        return ERR_WORD_NOT_FOUND;

    i = result + 1;
    /* found an .extern, now loading all labels after it*/
    while (i < (*line_params).param_count)
    {
        label_name = strdup((*line_params).parsed_params[i]);
        if (label_name == NULL)
            EXIT_ON_MEM_ALLOC_FAIL

        if ((legalLabel(label_name, &data_model->symbols, data_model->symbol_count)) != 0)
        {
            error_flag = true;
            i++;
            found_label = true;
            continue;
        }

        new_symbol.type = EXTERN; /*(~extern~)*/
        new_symbol.value = 0;     /*unclear what the value should be*/
        strcpy(new_symbol.name, label_name);
        push((void **)&(data_model->symbols), &(data_model->symbol_count), sizeof(Symbol), &new_symbol);
        found_label = true;
        i++;
        safe_free(1, label_name);
    }

    return found_label
               ? (error_flag ? EXTERN_FOUND_AND_ADDED_WITH_ERRORS : EXTERN_FOUND_AND_ADDED)
               : NOT_EXTERN;
}
