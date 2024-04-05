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
        /*char *operand_target = NULL;
        char *index = NULL;
        char *ptr_close = NULL;*/

        if ((*line_params).param_count != word + 1)
        {
            /* only array can have white chars / more word as operand */
            /* check if its array and appned */
            char *ptr_open = NULL;
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
                            fprintf(stderr, "Syntax Error. Index in word+1 but word+2 isn't just \"]\" in line %d\n", data_model->line_number);
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
                        fprintf(stderr, "Syntax Error. Index in word but word+1 isn't just \"]\" in line %d\n", data_model->line_number);
                        return SYNTAX_ERROR;
                    }
                }
            }
            else
            {
                /* syntax error number of operands / unnecessary white chars  */
                fprintf(stderr, "Syntax Error. There are illegal white chars in line %d\n", data_model->line_number);
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
        char **arr = NULL;
        int operands_count = 0;

        /* break word */
        operands_count = parse_string_into_string_array(data_model, (*line_params).parsed_params[word], &arr, ",");
        /* @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ */

        if (operands_count == 0)
        {
            fprintf(stderr, "the if is temporary code in line %d\n", data_model->line_number);
        }

        if ((*line_params).param_count == word + 1) /* both operands in 1 word */
        {
            if ((*line_params).parsed_params[word][0] == ',' || (*line_params).parsed_params[word][strlen((*line_params).parsed_params[word]) - 1] == ',' || strchr((*line_params).parsed_params[word], ',') != strrchr((*line_params).parsed_params[word], ','))
            {
                fprintf(stdout, "Error. Number of opperands is illegal in line %d\n", data_model->line_number);
                return ERR_NUMBER_OPERANDS_FOR_COMMAND;
            }

            /* break the word with comma */
        }
        else if ((*line_params).param_count == word + 2) /* comma include in one operand */
        {
            if ((*line_params).parsed_params[word][strlen((*line_params).parsed_params[word]) - 1] != ',')
            {
                if ((*line_params).parsed_params[word + 1][0] != ',')
                {
                    fprintf(stdout, "Error. Missing comma between operands in line %d\n", data_model->line_number);
                    return ERR_MISSING_COMMA;
                }
                else /* comma with second operand legally */
                {
                }
            }
            else /* comma with first operand legally */
            {
            }
        }
        else if ((*line_params).param_count == word + 3) /* comma is a seperate word and must be in word + 1  */
        {
            if (!(strcmp((*line_params).parsed_params[word + 1], ",")))
            {
                fprintf(stdout, "Error. Missing comma between operands in line %d\n", data_model->line_number);
                return ERR_MISSING_COMMA;
            }

            addressing_source = check_addressing(&(*line_params).parsed_params[word], data_model);
            if (addressing_source < 0)
            {
                fprintf(stdout, "Error. Invalid source addressing in line %d\n", data_model->line_number);
                return ERR_INVALID_ADDRESSING;
            }

            if (!(assembler_commands[result].allowed_source_operand_adderss_type & 1u << addressing_source))
            {
                fprintf(stdout, "Error. Wrong type of source addressing of target in line %d\n", data_model->line_number);
                return ERR_WRONG_TYPE_OF_ADDRESSING;
            }
        }
    }

    return result;
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
    unsigned char temp = 0;

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
 * @brief check addressing type for an operand
 *
 * @param word a string represent the opernad
 * @return int the type of addressing
 */
int check_addressing(char **word, Data_model *data_model)
{
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
            fprintf(stderr, "Immediate addressing is missing a number in line %d\n", data_model->line_number);
            result = ERR_INVALID_ADDRESSING;
        }

        /*printf("the word %s is immediate number\n", *word);*/
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
            result = 2;
        }
        else if (is_define(index, data_model->symbols, data_model->symbol_count) >= 0)
        {
            result = 2;
        }
        else
        {
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
 * @param word the operand
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
            fprintf(stderr, "0 number is: %d\n", *num);
        }
        else if ((i = is_define((*word) + 1, data_model->symbols, data_model->symbol_count)) >= 0)
        {
            *num = data_model->symbols[i].value;
            fprintf(stderr, "0 number is define: %d\n", *num);
        }
        else
        {
            fprintf(stderr, "Error in addressing type 0\n");
            safe_free(1, num);
            return result;
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
        fprintf(stderr, "adding blank word\n");
        operand_entry.address = data_model->instruction_count + CODE_START_ADDRESS;
        operand_entry.dValue = -1;
        /* setting the ARE bits to 11 */
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
            fprintf(stderr, "2 index is: %d\n", *num);
        }
        else if ((i = is_define(index, data_model->symbols, data_model->symbol_count)) >= 0)
        {
            *num = data_model->symbols[i].value;
            fprintf(stderr, "2 index is define: %d\n", *num);
        }
        else
        {
            fprintf(stderr, "Error in addressing type 2 in line %d\n", data_model->line_number);
            safe_free(2, num, index);
            return result;
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
                        fprintf(stderr, "source register is: %d\n", i);
                        operand_entry.address = data_model->instruction_count + CODE_START_ADDRESS;
                        operand_entry.dValue = i;
                        write_bits_in_word(&operand_entry.word, i, 3, 2);

                        push((void **)&data_model->instructions_table, &data_model->instruction_count, sizeof(Word_entry), &operand_entry);
                        result = 1;
                    }
                    else /* register_type == 1 source*/
                    {
                        fprintf(stderr, "target register is: %d\n", i);
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
            /* code */
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
