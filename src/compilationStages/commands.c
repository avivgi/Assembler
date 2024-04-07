#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include "commands.h"

#include "../Utils/stringUtils.h"
#include "../Utils/languageUtils.h"
#include "../global_constants.h"
#include "../Utils/memoryUtils.h"
#include "../Utils/stringUtils.h"
#include "../Utils/languageUtils.h"
#include "../global_constants.h"
#include "../Utils/memoryUtils.h"

/*#include "../Utils/printUtils.h"*/

int commands(Data_model *data_model, Line_params *line_params, size_t line_params_count)
{
    As_Command assembler_commands[NUM_OF_COMMANDS_IN_LANGUAGE] = AS_COMMAND_LIST;
    int i;
    /* int num; */
    Word_entry instruction_entry;
    int addressing_target = ERR_INVALID_ADDRESSING;
    int addressing_source = ERR_INVALID_ADDRESSING;
    int result = ERR_WORD_NOT_FOUND;
    int word = 0;
    instruction_entry.word = 0;

    /* check if first word is label*/
    if ((*line_params).parsed_params[word][strlen((*line_params).parsed_params[word]) - 1] == ':')
    {
        word++;
    }

    /* check if the command is in command list */
    for (i = 0; i < NUM_OF_COMMANDS_IN_LANGUAGE; i++)
    {
        if (strcmp((*line_params).parsed_params[word],
                   (assembler_commands[i].command_name)) == 0)
        {
            result = i;
            word++;
            break;
        }
    }
    if (result == ERR_WORD_NOT_FOUND)
    {
        fprintf(stdout, "Error. Didn't find command name: %s in line %d\n",
                (*line_params).parsed_params[word], data_model->line_number);
        return ERR_WORD_NOT_FOUND;
    }

    /* check number of oprands allowed to have for the command*/

    if (assembler_commands[result].command_type == 0)
    {
        if ((*line_params).param_count != word)
        {
            fprintf(stdout, "Error. Number of opperands is illegal in line %d\n", data_model->line_number); /* line number in am file */
            return ERR_NUMBER_OPERANDS_FOR_COMMAND;
        }

        instruction_entry.address = data_model->instruction_count + CODE_START_ADDRESS;
        instruction_entry.dValue = result;
        write_bits_in_word(&instruction_entry.word, result, 4, 6); /* opcode */

        push((void **)&data_model->instructions_table, &data_model->instruction_count, sizeof(Word_entry), &instruction_entry);
    }

    else if (assembler_commands[result].command_type == 1)
    {
        /* check syntax and modife for easier handle */
        if ((*line_params).param_count != word + 1)
        {
            if (SYNTAX_ERROR == syntax_check_commands(data_model, line_params, (*line_params).param_count, word, assembler_commands[result].command_type))
            {
                return SYNTAX_ERROR;
            }
        }

        /* check address type */
        addressing_target = check_addressing(&(*line_params).parsed_params[word], data_model);
        if (addressing_target < 0)
        {
            return ERR_INVALID_ADDRESSING;
        }

        if (!(assembler_commands[result].allowed_target_operand_adderss_type & 1u << addressing_target))
        {
            fprintf(stdout, "Error. Wrong type of addressing of target in line %d\n", data_model->line_number);
            return ERR_WRONG_TYPE_OF_ADDRESSING;
        }

        instruction_entry.address = data_model->instruction_count + CODE_START_ADDRESS;
        instruction_entry.dValue = result;
        write_bits_in_word(&instruction_entry.word, result, 4, 6);            /* opcode */
        write_bits_in_word(&instruction_entry.word, addressing_target, 2, 2); /* addressing target */

        /* push command word */
        push((void **)&data_model->instructions_table, &data_model->instruction_count, sizeof(Word_entry), &instruction_entry);

        /* write the extra words and push */
        handle_addressing(data_model, addressing_target, &(*line_params).parsed_params[word], 0);
    }

    else if (assembler_commands[i].command_type == 2)
    {

        /* check syntax and modife for easier handle */

        if (SYNTAX_ERROR == syntax_check_commands(data_model, line_params, (*line_params).param_count, word, assembler_commands[result].command_type))
        {
            return SYNTAX_ERROR;
        }

        /* after syntax check the first operand will be in word - 1, and the second in word*/
        word--;

        /* check address type for source */
        addressing_source = check_addressing(&(*line_params).parsed_params[word], data_model);
        if (addressing_source < 0)
        {
            return ERR_INVALID_ADDRESSING;
        }

        if (!(assembler_commands[result].allowed_source_operand_adderss_type & 1u << addressing_source))
        {
            fprintf(stdout, "Error. Wrong type of addressing of source in line %d\n", data_model->line_number);
            return ERR_WRONG_TYPE_OF_ADDRESSING;
        }

        /* check address type for target */
        addressing_target = check_addressing(&(*line_params).parsed_params[word + 1], data_model);
        if (addressing_target < 0)
        {
            return ERR_INVALID_ADDRESSING;
        }

        if (!(assembler_commands[result].allowed_target_operand_adderss_type & 1u << addressing_target))
        {
            fprintf(stdout, "Error. Wrong type of addressing of target in line %d\n", data_model->line_number);
            return ERR_WRONG_TYPE_OF_ADDRESSING;
        }

        instruction_entry.address = data_model->instruction_count + CODE_START_ADDRESS;
        instruction_entry.dValue = result;
        write_bits_in_word(&instruction_entry.word, result, 4, 6);            /* opcode */
        write_bits_in_word(&instruction_entry.word, addressing_source, 2, 4); /* addressing source */
        write_bits_in_word(&instruction_entry.word, addressing_target, 2, 2); /* addressing target */

        /* push command word */
        push((void **)&data_model->instructions_table, &data_model->instruction_count, sizeof(Word_entry), &instruction_entry);

        /* if both operands are registers than they share the same word code */
        if (addressing_source == 3 && addressing_target == 3)
        {
            strcat((*line_params).parsed_params[word], ",");
            strcat((*line_params).parsed_params[word], (*line_params).parsed_params[word + 1]);
            handle_addressing(data_model, addressing_target, &(*line_params).parsed_params[word], 2);
        }
        else
        {
            /* write the extra words for source and push */
            handle_addressing(data_model, addressing_source, &(*line_params).parsed_params[word], 1);

            /* write the extra words for target and push */
            handle_addressing(data_model, addressing_target, &(*line_params).parsed_params[word + 1], 0);
        }
    }

    return result;
}

/**
 * @brief check syntax for command line
 *
 * @param data_model
 * @param line_params a line in the code
 * @param line_params_count number of words in the line
 * @param word index for the word to start the check
 * @param operands_number nummber of operands expected
 * @return int 1 if syntax successful, otherwise negative error value
 */
int syntax_check_commands(Data_model *data_model, Line_params *line_params, size_t line_params_count, int word, int operands_number)
{
    char *ptr_open = NULL;
    if (operands_number == 1)
    {
        if ((ptr_open = strchr((*line_params).parsed_params[word], '[')) != NULL)
        {
            /* if last char in word is '[' */
            if (*ptr_open == (*line_params).parsed_params[word][strlen((*line_params).parsed_params[word]) - 1])
            {
                /* the index in word+1*/
                if (strchr((*line_params).parsed_params[word + 1], ']') != NULL)
                {
                    /* index and ] in word+1, opernad end */
                    strcat((*line_params).parsed_params[word], (*line_params).parsed_params[word + 1]);
                }
                else
                {
                    /* index in word+1, check if "]" is the word+2 */
                    if (strlen((*line_params).parsed_params[word + 2]) == 1 && (*line_params).parsed_params[word + 2][0] == ']')
                    {
                        /* "]" is the word+2, operand end*/
                        strcat((*line_params).parsed_params[word], (*line_params).parsed_params[word + 1]);
                        strcat((*line_params).parsed_params[word], (*line_params).parsed_params[word + 2]);
                    }
                    else
                    {
                        /* error */
                        fprintf(stdout, "Syntax Error. Unnecessary char in array in line %d\n", data_model->line_number);
                        return SYNTAX_ERROR;
                    }
                }
            }
            else
            {
                /* index in the word, check if "]" is word+1 */
                if (strlen((*line_params).parsed_params[word + 1]) == 1 && (*line_params).parsed_params[word + 1][0] == ']')
                {
                    /* operand end */
                    strcat((*line_params).parsed_params[word], (*line_params).parsed_params[word + 1]);
                }
                else
                {
                    /* error */
                    fprintf(stdout, "Syntax Error. Unnecessary char in array  in line %d\n", data_model->line_number);
                    return SYNTAX_ERROR;
                }
            }
        }
        else
        {
            /* syntax error number of operands / unnecessary white chars  */
            fprintf(stdout, "Syntax Error. There is illegal white char in line %d\n", data_model->line_number);
            return SYNTAX_ERROR;
        }
    }

    else if (operands_number == 2)
    {
        int i, j, operands_count;
        char **operands_arr = NULL;
        char *ptr_comma = NULL;

        for (i = word; i < line_params_count; i++)
        {
            if ((ptr_comma = strchr((*line_params).parsed_params[i], ',')) != NULL)
            {
                break;
            }
        }

        /* if comma not found or we have 1 more comma in the found word */
        if (ptr_comma == NULL || ptr_comma != strrchr((*line_params).parsed_params[i], ','))
        {
            fprintf(stdout, "Error. Number of operands illegal for the command in line %d\n", data_model->line_number);
            return SYNTAX_ERROR;
        }

        /* check for comma in the rest of the words */
        for (j = i + 1; j < line_params_count; j++)
        {
            if (strchr((*line_params).parsed_params[j], ',') != NULL)
            {
                fprintf(stdout, "Error. Number of operands illegal for the command in line %d\n", data_model->line_number);
                return SYNTAX_ERROR;
            }
        }

        /* if both operands and comma are in the same word */
        if (line_params_count == word + 1)
        {
            parse_string_into_string_array(data_model, (*line_params).parsed_params[word], &operands_arr, ",");
            strcpy((*line_params).parsed_params[word - 1], operands_arr[0]);
            strcpy((*line_params).parsed_params[word], operands_arr[1]);
            safe_free(1, operands_arr);
            return 1;
        }

        /* if the word with comma is just a comma */
        if (strlen((*line_params).parsed_params[i]) == 1)
        {
            /* check if first operand dont have spaces */
            if (i != 2)
            {
                if (SYNTAX_ERROR == syntax_check_commands(data_model, line_params, line_params_count, word, 1))
                {
                    fprintf(stdout, "first operand out of 2 have SYNTAX_ERROR in line %d\n", data_model->line_number);
                    return SYNTAX_ERROR;
                }
            }

            /* check if second operand dont have spaces */
            if (line_params_count != i + 2)
            {
                if (SYNTAX_ERROR == syntax_check_commands(data_model, line_params, line_params_count, i + 1, 1))
                {
                    fprintf(stdout, "second operand out of 2 have SYNTAX_ERROR in line %d\n", data_model->line_number);
                    return SYNTAX_ERROR;
                }
            }

            strcpy((*line_params).parsed_params[word - 1], (*line_params).parsed_params[word]);
            strcpy((*line_params).parsed_params[word], (*line_params).parsed_params[i + 1]);
        }

        /* comma include in parts of opernads */
        else
        {
            operands_count = parse_string_into_string_array(data_model, (*line_params).parsed_params[i], &operands_arr, ",");
            if (operands_count == 1)
            {
                /* check where in the word is the comma and remove it */

                /* if comma in start of second opernad */
                if (*ptr_comma == (*line_params).parsed_params[i][0])
                {
                    for (j = 0; j < strlen((*line_params).parsed_params[i]); j++)
                    {
                        (*line_params).parsed_params[i][j] = (*line_params).parsed_params[i][j + 1];
                    }

                    /* if first operand need syntax check */
                    if (i > 2)
                    {
                        if (SYNTAX_ERROR == syntax_check_commands(data_model, line_params, line_params_count, word, 1))
                        {
                            fprintf(stdout, "first operand out of 2 have SYNTAX_ERROR in line %d\n", data_model->line_number);
                            return SYNTAX_ERROR;
                        }
                    }

                    /* if second operand need syntax check */
                    if (line_params_count > i + 1)
                    {
                        if (SYNTAX_ERROR == syntax_check_commands(data_model, line_params, line_params_count, i, 1))
                        {
                            fprintf(stdout, "second operand out of 2 have SYNTAX_ERROR in line %d\n", data_model->line_number);
                            return SYNTAX_ERROR;
                        }
                    }

                    strcpy((*line_params).parsed_params[word - 1], (*line_params).parsed_params[word]);
                    strcpy((*line_params).parsed_params[word], (*line_params).parsed_params[i]);
                }

                /* if comma in end of first opernad */
                else if (*ptr_comma == (*line_params).parsed_params[i][strlen((*line_params).parsed_params[i]) - 1])
                {
                    *ptr_comma = '\0';
                    /* if first operand need syntax check */
                    if (i > word)
                    {
                        if (SYNTAX_ERROR == syntax_check_commands(data_model, line_params, line_params_count, word, 1))
                        {
                            fprintf(stdout, "first operand out of 2 have SYNTAX_ERROR in line %d\n", data_model->line_number);
                            return SYNTAX_ERROR;
                        }
                    }

                    /* if second operand need syntax check */
                    if (line_params_count > i + 2)
                    {
                        if (SYNTAX_ERROR == syntax_check_commands(data_model, line_params, line_params_count, i + 1, 1))
                        {
                            fprintf(stdout, "second operand out of 2 have SYNTAX_ERROR in line %d\n", data_model->line_number);
                            return SYNTAX_ERROR;
                        }
                    }

                    strcpy((*line_params).parsed_params[word - 1], (*line_params).parsed_params[word]);
                    strcpy((*line_params).parsed_params[word], (*line_params).parsed_params[i + 1]);
                }
            }

            /* some parts of both operands are with the comma */
            else if (operands_count == 2)
            {
                (*line_params).parsed_params[i] = operands_arr[0];
                /* if first operand need syntax check */
                if (i > 1)
                {
                    if (SYNTAX_ERROR == syntax_check_commands(data_model, line_params, line_params_count, word, 1))
                    {
                        fprintf(stdout, "first operand out of 2 have SYNTAX_ERROR in line %d\n", data_model->line_number);
                        return SYNTAX_ERROR;
                    }
                }

                (*line_params).parsed_params[i] = operands_arr[1];
                /* if second operand need syntax check */
                if (line_params_count > i + 1)
                {
                    if (SYNTAX_ERROR == syntax_check_commands(data_model, line_params, line_params_count, i, 1))
                    {
                        fprintf(stdout, "second operand of 2 have SYNTAX_ERROR in line %d\n", data_model->line_number);
                        return SYNTAX_ERROR;
                    }
                }

                strcpy((*line_params).parsed_params[word - 1], (*line_params).parsed_params[word]);
                strcpy((*line_params).parsed_params[word], (*line_params).parsed_params[i]);
            }
        }
    }

    return 1;
}

/**
 * @brief write bits values in a word in a specific indexes
 *
 * @param word memory word to modify
 * @param write_value the value to write in the word
 * @param value_len number of bits to write
 * @param first_index most right index which the write_value will be write in word. 0 is the most right bit in the word
 * @return int the new value of the word
 */
int write_bits_in_word(Word *word, int write_value, int value_len, int first_index)
{
    int i;
    unsigned int temp = 0;

    for (i = value_len; i > 0; i--)
    {
        temp <<= 1;
        if (write_value & 1u << (i - 1))
        {
            temp = temp | 1u;
        }
    }

    *word = (*word) | temp << first_index;
    return *word;
}

/**
 * @brief check the addressing of an operand
 *
 * @param word operand to check
 * @param data_model
 * @return int addressing type of the command
 */
int check_addressing(char **word, Data_model *data_model)
{
    int i;
    int result = 1; /* defualt addressing is direct  */
    int *num = malloc(sizeof(int));
    char *ptr_open = NULL;
    char *ptr_close = NULL;
    char *index = NULL;

    /* 0 immediate  */
    if ((*word)[0] == '#')
    {
        if (is_number((*word) + 1, num))
        {
            result = 0;
        }
        else if (is_define((*word) + 1, data_model->symbols, data_model->symbol_count) >= 0)
        {
            result = 0;
        }
        else
        {
            fprintf(stdout, "Immediate addressing is missing a number in line %d\n", data_model->line_number);
            result = ERR_INVALID_ADDRESSING;
        }
    }

    /* 2 permenent index */
    else if ((ptr_open = strchr(*word, '[')) != NULL && (ptr_close = strchr(*word, ']')) != NULL)
    {
        size_t length;
        length = ptr_close - (ptr_open + 1);
        index = malloc((length + 1) * sizeof(char));
        if (!index)
        {
            EXIT_ON_MEM_ALLOC_FAIL
        }
        strncpy(index, ptr_open + 1, length);
        index[length] = '\0';

        if (is_number(index, num))
        {
            if (*num < 0)
            {
                fprintf(stdout, "Error. Index of array can not be negative in line\n", data_model->line_number);
                result = ERR_INVALID_ADDRESSING;
            }
            else
            {
                result = 2;
            }
        }
        else if ((i = is_define(index, data_model->symbols, data_model->symbol_count)) >= 0)
        {
            if (data_model->symbols[i].value < 0)
            {
                fprintf(stdout, "Error. Index of array can not be negative in line %d\n", data_model->line_number);
                result = ERR_INVALID_ADDRESSING;
            }
            else
            {
                result = 2;
            }
        }
        else
        {
            fprintf(stdout, "The array missing an number or define as index in line %d\n", data_model->line_number);
            result = ERR_INVALID_ADDRESSING;
        }
        safe_free(1, index);
    }

    else /* 3 direct register */
    {
        int i;
        char *registers[] = RESIGTERS;
        for (i = 0; i < NUM_OF_RESIGTERS; i++)
        {
            if (!(strcmp(*word, registers[i])))
            {
                result = 3;
            }
        }
    }

    safe_free(1, num);
    return result;
}

/**
 * @brief adding insruction words depands on addressing type
 *
 * @param data_model
 * @param addressing addressing type
 * @param word the operand. in case of 2 registers than both will
 * @param register_type 2= 2 registers, 1= source, 0= target or not a register
 * @return int 1 is the handle is successful, otherwise ERR_INVALID_ADDRESSING
 */
int handle_addressing(Data_model *data_model, int addressing, char **word, int register_type)
{
    int i;
    int result = ERR_INVALID_ADDRESSING;
    int *num = malloc(sizeof(int));
    Word_entry operand_entry;
    Word_entry index_entry;
    operand_entry.word = 0;
    index_entry.word = 0;

    if (!num)
    {
        EXIT_ON_MEM_ALLOC_FAIL
    }

    /* 0 immediate  */
    if (addressing == 0)
    {
        if (is_number((*word) + 1, num))
        {
            /* the number as integer is now in num */
        }
        else
        {
            i = is_define((*word) + 1, data_model->symbols, data_model->symbol_count);
            *num = data_model->symbols[i].value;
        }

        operand_entry.address = data_model->instruction_count + CODE_START_ADDRESS;
        operand_entry.dValue = *num;
        write_bits_in_word(&operand_entry.word, *num, 12, 2);

        push((void **)&data_model->instructions_table, &data_model->instruction_count, sizeof(Word_entry), &operand_entry);
        result = 1;
    }

    /* 1 direct */
    else if (addressing == 1)
    {
        operand_entry.address = data_model->instruction_count + CODE_START_ADDRESS;
        operand_entry.dValue = -1;
        /* turn on the two ARE bits*/
        write_bits_in_word(&operand_entry.word, 3, 2, 0);

        push((void **)&data_model->instructions_table, &data_model->instruction_count, sizeof(Word_entry), &operand_entry);
        result = 1;
    }

    /* 2 permenent index */
    else if (addressing == 2)
    {
        char *ptr_open = NULL;
        char *ptr_close = NULL;
        char *index = NULL;
        size_t length;

        operand_entry.address = data_model->instruction_count + CODE_START_ADDRESS;
        operand_entry.dValue = -1;
        /* setting the ARE bits to 11 */
        write_bits_in_word(&operand_entry.word, 3, 2, 0);

        push((void **)&data_model->instructions_table, &data_model->instruction_count, sizeof(Word_entry), &operand_entry);

        /* extract index and convert to num */
        ptr_open = strchr(*word, '[');
        ptr_close = strchr(*word, ']');
        length = ptr_close - (ptr_open + 1);
        index = malloc((length + 1) * sizeof(char));
        if (!index)
        {
            EXIT_ON_MEM_ALLOC_FAIL
        }
        strncpy(index, ptr_open + 1, length);
        index[length] = '\0';

        if (is_number(index, num))
        {
            /* the number as integer is now in num */
        }
        else
        {
            i = is_define(index, data_model->symbols, data_model->symbol_count);
            *num = data_model->symbols[i].value;
        }

        index_entry.address = data_model->instruction_count + CODE_START_ADDRESS;
        index_entry.dValue = *num;
        write_bits_in_word(&index_entry.word, *num, 12, 2);

        push((void **)&data_model->instructions_table, &data_model->instruction_count, sizeof(Word_entry), &index_entry);

        safe_free(1, index);
        result = 1;
    }

    /* 3 direct register */
    else if (addressing == 3)
    {
        char *registers[] = RESIGTERS;
        if (register_type != 2)
        {
            for (i = 0; i < NUM_OF_RESIGTERS; i++)
            {
                if (!(strcmp(*word, registers[i])))
                {
                    if (register_type == 0) /* register_type == 0 target*/
                    {
                        operand_entry.address = data_model->instruction_count + CODE_START_ADDRESS;
                        operand_entry.dValue = i;
                        write_bits_in_word(&operand_entry.word, i, 3, 2);

                        push((void **)&data_model->instructions_table, &data_model->instruction_count, sizeof(Word_entry), &operand_entry);
                        result = 1;
                    }
                    else /* register_type == 1 source*/
                    {
                        operand_entry.address = data_model->instruction_count + CODE_START_ADDRESS;
                        operand_entry.dValue = i;
                        write_bits_in_word(&operand_entry.word, i, 3, 5);

                        push((void **)&data_model->instructions_table, &data_model->instruction_count, sizeof(Word_entry), &operand_entry);
                        result = 1;
                    }
                }
            }
        }
        else /* register_type == 2 one word for both registers*/
        {
            char **registers_arr = NULL;
            int register_index;
            parse_string_into_string_array(data_model, *word, &registers_arr, ",");
            operand_entry.address = data_model->instruction_count + CODE_START_ADDRESS;

            for (register_index = 0; register_index < register_type; register_index++)
            {
                for (i = 0; i < NUM_OF_RESIGTERS; i++)
                {
                    if (!(strcmp(registers_arr[register_index], registers[i])))
                    {
                        break;
                    }
                }

                if (register_index == 0)
                {
                    operand_entry.dValue = i * 10;
                    write_bits_in_word(&operand_entry.word, i, 3, 5);
                }
                else
                {
                    operand_entry.dValue += i;
                    write_bits_in_word(&operand_entry.word, i, 3, 2);
                }
            }

            push((void **)&data_model->instructions_table, &data_model->instruction_count, sizeof(Word_entry), &operand_entry);
            result = 1;
        }
    }

    safe_free(1, num);
    return result;
}

/**
 * @brief check if a word is a define label.
 *
 * @param label the word to check.
 * @param symbol_table The symbol table to search in.
 * @param symbol_count The number of symbols in the symbol table.
 * @return int the index of the define label in the sybmol table, otherwise negative number
 */
int is_define(char *label, Symbol *symbol_table, int symbol_count)
{
    int i;
    if ((i = isLabelExist(label, symbol_table, symbol_count)) >= 0)
    {
        if (symbol_table[i].type == MDEFINE)
        {
            return i;
        }
    }
    return EER_LABEL_NOT_MDEFINE;
}

/* should update operands on 2nd pass. return 0 in success and something else on errors*/
int updateOperands(Data_model *data_model, Line_params *line_params, size_t line_params_count)
{

    return 1;
}
