
#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include <ctype.h>
#include <string.h>
#include "labels.h"
#include "../Utils/stringUtils.h"
#include "../Utils/languageUtils.h"
#include "../global_constants.h"
#include "../Utils/memoryUtils.h"
#include "../Utils/stringUtils.h"
#include "../datamodel.h"
#include "../Utils/printUtils.h"

char *strdup(const char *s);

/**
 * Function to evaluate if there are symbols and create them.
 * @param symbols The array of symbols.
 * @param symbol_count The number of symbols in the array.
 * @param line_params The array of line parameters.
 * @param line_params_count The number of line parameters in the array.
 * @param instruction_count The number of instructions in the array.
 * @param data_count data counter.
 * @param assembly_code The array of assembly code.
 * @param assembly_code_count The number of assembly code in the array.
 * @return 0 if the symbol was created, an error code otherwise.
 */
int labels(Data_model *data_model,
           Line_params *line_params,
           size_t line_params_count)
{
    /*
    Symbol can be:
    LABEL: .data 1,2,3,4 // LABEL is a symbol. Symbol type is .data
    LABEL: .string "abc" // LABEL is a symbol. Symbol type is .data
    LABEL: mov r1, r2 // LABEL is a symbol. Symbol type is CODE

    symbols can also be: (not implemented in this function)
    LABEL1: .entry LABEL2 // LABEL2 is a symbol, LABEL1 is not a symbol and is redundant. Symbol type is .entry
    LABEL1: .extern LABEL2 // LABEL2 is a symbol, LABEL1 is not a symbol and is redundant Symbol type is .extern
    */

    int str_len = 0;
    /* int is_symbol = 0; */
    Symbol new_symbol;
    char *label_name;
    int result = 0;

    label_name = strdup((*line_params).parsed_params[0]);

    if (label_name == NULL)
        EXIT_ON_MEM_ALLOC_FAIL

    str_len = strlen(label_name);
    if (str_len < 2)
    {
        safe_free(1, label_name);
        return INFO_NOT_LABAL;
    }
    if (is_label_entry(*line_params, line_params_count))
    {
        safe_free(1, label_name);
        return LABEL_DATA_WAS_FOUND;
    }
    /*detect type LABEL: */
    if ((*line_params).parsed_params[0][str_len - 1] != ':')
    {
        safe_free(1, label_name);
        return INFO_NOT_LABAL;
    }
    label_name[str_len - 1] = '\0';
    str_len--;

    if ((legalLabel(label_name, &data_model->symbols, data_model->symbol_count)) != 0)
    {
        safe_free(1, label_name);
        return ERR_LABEL_OR_NAME_IS_TAKEN;
    }
    /*check if data or string*/
    /*detect type LABEL: .data */

    if (strcmp((*line_params).parsed_params[1], ".data") == 0)
    {
        new_symbol.type = DATA;
        new_symbol.value = data_model->data_count;
        result = add_int_array_to_data_table(data_model, (*line_params), (int)(*line_params).param_count);
        if (result < 0)
        {
            safe_free(1, label_name);
            return result;
        }
        result = LABEL_DATA_WAS_FOUND;
    }
    /*detect type LABEL: .string */
    else if (strcmp((*line_params).parsed_params[1], ".string") == 0)
    {
        new_symbol.type = DATA;
        new_symbol.value = data_model->data_count;
        add_char_array_to_assembly(data_model, (*line_params), line_params_count);
        result = LABEL_DATA_WAS_FOUND;
    }
    else if (strcmp((*line_params).parsed_params[1], ".extern") == 0)
    {
        fprintf(stderr, "INFO: Label (%s) before .extern. in line %d. Ignoring this label. Continue.\n", label_name, data_model->line_number);
        safe_free(1, label_name);
        return INFO_LABEL_BEFORE_EXTERN;
    }
    else
    {
        /*fallback to type LABEL: code*/
        new_symbol.type = CODE;
        new_symbol.value = data_model->instruction_count + CODE_START_ADDRESS;
        result = LABEL_CODE_WAS_FOUND;
    }
    strcpy(new_symbol.name, label_name);
    push((void **)&data_model->symbols, &data_model->symbol_count, sizeof(Symbol), &new_symbol);
    safe_free(1, label_name);
    return result;
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
int add_int_array_to_data_table(Data_model *data_model,
                                Line_params line_params,
                                int param_count)
{
    int i;
    int param = 2;
    int *arr = NULL;

    int array_size = 0;
    /*  size_t total_array_size = 0; */

    Word_entry data_entry;

    while (param < param_count)
    {
        array_size = parse_string_into_int_array(data_model, (line_params).parsed_params[param], &arr, ",");
        if (array_size <= 0)
        {
            fprintf(stderr, "Error: Not a number in line %d.\n", data_model->line_number);
            safe_free(1, arr);
            return ERR_VARIABLE_ISNT_INTEGER;
        }

        for (i = 0; i < array_size; i++)
        {
            data_entry.address = data_model->data_count;
            data_entry.dValue = arr[i];
            data_entry.word = data_entry.dValue;
            push((void **)&data_model->data_table, &data_model->data_count, sizeof(Word_entry), &data_entry);
        }
        param++;
    }
    safe_free(1, arr);
    return 0;
}

/**
 * @brief Parses a string into an integer array.
 *
 * This function takes a string buffer and splits it into tokens using the specified delimiters.
 * Each token is later converted to an integer and stored in the result_array.
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

int parse_string_into_int_array(Data_model *data_model,
                                char *buffer,
                                int **result_array,
                                const char *delimiters)
{
    char *token;
    int i = 0;
    int temp;
    char *buffer_c = NULL;

    /* Duplicate the input buffer */
    buffer_c = strdup(buffer);
    if (buffer_c == NULL)
        EXIT_ON_MEM_ALLOC_FAIL

    token = strtok(buffer_c, delimiters);
    while (token)
    {
        if (!is_number(token, &temp)) /* Check if the token is a number */
        {
            /* Check if the token is a label */
            if (isLabelExist(token, data_model->symbols, data_model->symbol_count) >= 0)
                temp = getLabelAddress(token, data_model->symbols, data_model->symbol_count);
            else
            {
                fprintf(stderr, "Error: Variable %s is not an integer or a data label in line %d.\n", token, data_model->line_number);
                safe_free(1, buffer_c);
                return ERR_VARIABLE_ISNT_INTEGER;
            }
        }

        *result_array = realloc(*result_array, (i + 1) * sizeof(int));
        if (*result_array == NULL)
        {
            safe_free(1, buffer_c);
            EXIT_ON_MEM_ALLOC_FAIL
        }

        (*result_array)[i++] = temp;

        token = strtok(NULL, delimiters);
    }

    safe_free(1, buffer_c);
    return i;
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
int add_char_array_to_assembly(Data_model *data_model,
                               Line_params line_params,
                               size_t line_params_count)
{
    int i;
    Word_entry data_entry;

    const char *str = (line_params).parsed_params[2];
    size_t str_len = strlen(str);

    for (i = 0; i < str_len; i++)
    {
        if (str[i] == 34) /* " sign" */
            continue;
        data_entry.address = data_model->data_count;
        data_entry.dValue = (int)str[i];
        data_entry.word = data_entry.dValue;
        push((void **)&data_model->data_table, &data_model->data_count, sizeof(Word_entry), &data_entry);
    }

    data_entry.address = data_model->data_count;
    data_entry.dValue = '\0';
    data_entry.word = data_entry.dValue;
    push((void **)&data_model->data_table, &data_model->data_count, sizeof(Word_entry), &data_entry);
    return 0;
}

int is_label_data_extern_string(Line_params line_params, size_t line_params_count)
{
    char *param0 = line_params.parsed_params[0];
    char *param1 = line_params.parsed_params[1];
    return strcmp(param0, ".data") == 0 || strcmp(param0, ".string") == 0 || strcmp(param0, ".extern") == 0 ||
                   strcmp(param1, ".data") == 0 || strcmp(param1, ".string") == 0 || strcmp(param1, ".extern") == 0
               ? 1
               : 0;
}
int is_label_entry(Line_params line_params, size_t line_params_count)
{
    char *param0 = line_params.parsed_params[0];
    char *param1 = line_params.parsed_params[1];

    return strcmp(param0, ".entry") == 0 || strcmp(param1, ".entry") == 0
               ? 1
               : 0;
}

char *get_label_entry(Data_model data_model, Line_params line_params, size_t line_params_count)
{
    int param;
    char *param0 = line_params.parsed_params[0];
    char *param1 = line_params.parsed_params[1];

    if (strcmp(param0, ".entry") == 0)
        param = 0;
    else if (strcmp(param1, ".entry") == 0)
        param = 1;
    else
        return "0";

    if (param == line_params_count)
        return "0";

    return isLabelExist(line_params.parsed_params[param + 1], data_model.symbols, data_model.symbol_count) != EER_LABEL_NOT_FOUND ? line_params.parsed_params[param + 1] : "0";
}

void update_entry_symbol(Data_model *data_model, char *entry_label_name)
{
    size_t i;
    for (i = 0; i < data_model->symbol_count; i++)
    {
        if (strcmp(data_model->symbols[i].name, entry_label_name) == 0)
            data_model->symbols[i]
                .type = ENTRY;
    }
}
