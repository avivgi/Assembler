
#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include <ctype.h>
#include "createSymbols.h"
#include "../Utils/stringUtils.h"
#include "../Utils/languageUtils.h"
#include "../global_constants.h"
#include "../Utils/memoryUtils.h"
#include "../Utils/stringUtils.h"
#include "../datamodel.h"

int createSymbols(Symbol **symbols,
                  size_t *symbol_count,
                  Line_params **line_params,
                  size_t *line_params_count,
                  int *instruction_count,
                  int *data_count,
                  Assembly_code **assembly_code,
                  size_t *assembly_code_count)
{
    int str_len = 0;
    int is_symbol = 0;
    Symbol new_symbol;
    char *label_name;

    label_name = strdup((*line_params)[*line_params_count - 1].parsed_params[0]);

    if (label_name == NULL)
        EXIT_ON_MEM_ALLOC_FAIL
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
    if (strcmp((*line_params)[*line_params_count - 1].parsed_params[1], ".data") == 0)
    {
        new_symbol.type = DATA;
        new_symbol.value = *data_count;

        add_int_array_to_assembly(assembly_code, assembly_code_count, *(*line_params + (*line_params_count - 1)), line_params_count, data_count, *symbols, *symbol_count);
    }
    else if (strcmp((*line_params)[*line_params_count - 1].parsed_params[1], ".string") == 0)
    {
        new_symbol.type = DATA;
        new_symbol.value = *data_count;
        add_char_array_to_assembly(assembly_code, assembly_code_count, *(*line_params + *line_params_count - 1), line_params_count, data_count);
    }
    else
    {
        new_symbol.type = CODE;
        new_symbol.value = *instruction_count;
        /* fix to reflect size of instruction*/
        (*instruction_count)++;
    }
    strcpy(new_symbol.name, label_name);
    push((void **)symbols, symbol_count, sizeof(Symbol), &new_symbol);
    free(label_name);
    return SYMBOL_WAS_FOUND;
}

/* pass array of integers and push them into assembly_code table*/
int add_int_array_to_assembly(Assembly_code **assembly_code,
                              size_t *assembly_code_count,
                              Line_params line_params,
                              size_t *line_params_count,
                              int *data_count,
                              Symbol *symbols,
                              size_t symbol_count)
{
    int i;
    int result;
    int *arr = NULL;

    size_t array_size = 0;
    Assembly_code data_assembly;

    result = parse_string_into_int_array((const char *)(line_params).parsed_params[2], &arr, ",", &array_size, symbols, symbol_count);
    if (result != 0)
    {
        fprintf(stderr, "Error! Not a number\n");
        free(arr);
        return ERR_VARIABLE_ISNT_INTEGER;
    }

    for (i = 0; i < array_size; i++)
    {
        data_assembly.address = (*data_count)++;
        data_assembly.binary_code = arr[i];
        push((void **)assembly_code, assembly_code_count, sizeof(Assembly_code), &data_assembly);
    }

    free(arr);
    return 0;
}

/* pass array of integers and push them into assembly_code table*/
int add_char_array_to_assembly(Assembly_code **assembly_code,
                               size_t *assembly_code_count,
                               Line_params line_params,
                               size_t *line_params_count,
                               int *data_count)
{
    int i;
    int result;
    Assembly_code data_assembly;
    const char *str = (line_params).parsed_params[2];
    size_t str_len = strlen(str);

    data_assembly.address = *data_count;
    for (i = 0; i < str_len; i++)
    {
        if (str[i] == 34) /* " sign" */
            continue;
        data_assembly.binary_code = (int)str[i];
        push((void **)assembly_code, assembly_code_count, sizeof(Assembly_code), &data_assembly);
        (*data_count)++;
    }

    data_assembly.binary_code = '\0';
    push((void **)assembly_code, assembly_code_count, sizeof(Assembly_code), &data_assembly);
    (*data_count)++;

    return 0;
}

/*this function parses a string using a delimier, convert the items into integers if possible and returns array of integers. if it cannot convert into integer one of the item it returns error */
int parse_string_into_int_array(const char *buffer,
                                int **result_array,
                                const char *delimitors,
                                size_t *count,
                                Symbol *symbols,
                                size_t symbol_count)
{
    char *token;
    int i = 0;
    int temp;
    int result;
    char *buffer_c = NULL;
    buffer_c = strdup(buffer);
    if (buffer_c == NULL)
        EXIT_ON_MEM_ALLOC_FAIL

    token = strtok(buffer_c, delimitors);
    if (!token)
    {
        free(buffer_c);
        EXIT_ON_MEM_ALLOC_FAIL
    }
    while (!token)
    {
        if (!is_number(token, &temp)) /* a literal*/
        {
            if (isLabelExist(token, symbols, symbol_count)) /* a define*/
                temp = getLabelAddress(token, symbols, symbol_count);
            else /* just text*/
            {
                fprintf(stderr, "Variable %s is not an integer or a data label.\n", token);
                free(buffer_c);
                return ERR_VARIABLE_ISNT_INTEGER;
            }
        }
        /* integer resolved, allocate*/
        (*result_array) = realloc((*result_array), (i + 1) * sizeof(int));
        if ((*result_array) == NULL)
        {
            free(buffer_c);
            free(token);
            EXIT_ON_MEM_ALLOC_FAIL
        }

        (*result_array)[i++] = temp;
        token = strtok(NULL, delimitors);
    }
    *count = i;
    free(buffer_c);
    return 0;
}
