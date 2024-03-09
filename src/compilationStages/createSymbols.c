
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

/**
 * Function to create a symbol.
 * @param symbols The array of symbols.
 * @param symbol_count The number of symbols in the array.
 * @param line_params The array of line parameters.
 * @param line_params_count The number of line parameters in the array.
 * @param instruction_count The number of instructions in the array.
 * @param data_count data counter .
 * @param assembly_code The array of assembly code.
 * @param assembly_code_count The number of assembly code in the array.
 * @return 0 if the symbol was created, an error code otherwise.
 */
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

/**
 * Function to add an integer array of a .data instruction to the assembly code table.
 * @param assembly_code The array of assembly code.
 * @param assembly_code_count The number of assembly code in the array.
 * @param line_params The line parameters.
 * @param line_params_count The number of line parameters.
 * @param data_count The data counter.
 * @param symbols The array of symbols.
 * @param symbol_count The number of symbols in the array.
 * @return 0 if the integer array was added successfully, an error code otherwise.
 */
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
        fprintf(stdout, "Error! Not a number\n");
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

/**
 * @brief Parses a string into an integer array.
 *
 * This function takes a string buffer and splits it into tokens using the specified delimiters.
 * Each token is later converted to an integer and stored in the result_array.
 * If a token is not a valid integer, it checks if it is a defined label and retrieves its address.
 * If the token is neither a valid integer nor a defined label, an error is returned.
 *
 * @param buffer The string buffer to parse.
 * @param result_array A pointer to the integer array where the parsed integers will be stored.
 * @param delimiters The delimiters used to split the string buffer into tokens.
 * @param count A pointer to the variable that will store the number of parsed integers.
 * @param symbols An array of Symbol structs representing defined labels.
 * @param symbol_count The number of symbols in the symbols array.
 * @return 0 if successful, otherwise an error code.
 */
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
                fprintf(stdout, "Variable %s is not an integer or a data label.\n", token);
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

/**
 * Function to add a character array of a .string instruction to the assembly code table.
 * @param assembly_code The array of assembly code.
 * @param assembly_code_count The number of assembly code in the array.
 * @param line_params The line parameters.
 * @param line_params_count The number of line parameters.
 * @param data_count The data counter.
 * @return 0 if the character array was added successfully, an error code otherwise.
 */
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
