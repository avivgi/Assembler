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

char *strdup(const char *s);
int check_addressing(char **word);
int write_bits_in_word(Word *word, int write_value, int value_len, int first_index);

int commands(Data_model *data_model, Line_params **line_params, size_t *line_params_count)
{
    As_Command assembler_commands[NUM_OF_COMMANDS_IN_LANGUAGE] = AS_COMMAND_LIST;
    int i;
    Word_entry instruction_entry;
    int addressing_target = ERR_INVALID_ADDRESSING;
    int addressing_source = ERR_INVALID_ADDRESSING;
    int result = ERR_WORD_NOT_FOUND;
    int word = 0;
    instruction_entry.word = 0;

    /* check if first word is label*/
    if ((*line_params)[*line_params_count - 1].parsed_params[word][strlen((*line_params)[*line_params_count - 1].parsed_params[word]) - 1] == ':')
    {
        word++;
    }

    for (i = 0; i < NUM_OF_COMMANDS_IN_LANGUAGE; i++)
    {
        if (strcmp((*line_params)[*line_params_count - 1].parsed_params[word],
                   (assembler_commands[i].command_name)) == 0)
        {
            result = i;
            word++;
            break;
        }
    }
    if (result == ERR_WORD_NOT_FOUND)
    {
        fprintf(stdout, "Error. Didn't find command name: %s\n",
                (*line_params)[*line_params_count - 1].parsed_params[word]);
        return ERR_WORD_NOT_FOUND;
    }

    /* check number of oprands allowed and have for the command*/

    if (assembler_commands[result].command_type == 0)
    {
        if ((*line_params)[*line_params_count - 1].param_count != word)
        {
            fprintf(stdout, "Error. Number of opperands is illegal\n");
            return ERR_NUMBER_OPERANDS_FOR_COMMAND;
        }

        instruction_entry.address = data_model->instruction_count + CODE_START_ADDRESS;
        instruction_entry.dValue = result;

        fprintf(stdout, "\n@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
        fprintf(stdout, "\nword value before: %d\n", instruction_entry.word);
        write_bits_in_word(&instruction_entry.word, result, 4, 6);
        fprintf(stdout, "\nword value after: %d\n", instruction_entry.word);

        push((void **)&data_model->instructions_table, &data_model->instruction_count, sizeof(Word_entry), &instruction_entry);
    }

    else if (assembler_commands[result].command_type == 1)
    {
        if ((*line_params)[*line_params_count - 1].param_count != word + 1)
        {
            fprintf(stdout, "Error. Number of opperands is illegal\n");
            return ERR_NUMBER_OPERANDS_FOR_COMMAND;
        }

        if (strchr((*line_params)[*line_params_count - 1].parsed_params[word], ',') != NULL)
        {
            fprintf(stdout, "Error. Unexpected comma for command with 1 operand\n");
            return ERR_ILLEGAL_COMMA;
        }

        /* check address type */
        addressing_target = check_addressing(&(*line_params)[*line_params_count - 1].parsed_params[word]);
        if (addressing_target < 0)
        {
            fprintf(stdout, "Error. Invalid target addressing\n");
            return ERR_INVALID_ADDRESSING;
        }

        if (!(assembler_commands[result].allowed_target_operand_adderss_type & 1u << addressing_target))
        {
            fprintf(stdout, "Error. Wrong type of addressing of target\n");
            return ERR_WRONG_TYPE_OF_ADDRESSING;
        }

        /* push command word */
        push((void **)&data_model->instructions_table, &data_model->instruction_count, sizeof(Word_entry), &instruction_entry);

        /* write the extra word if needed and push */

        /* write the second extra word if needed and push */
    }

    else if (assembler_commands[i].command_type == 2)
    {
        char **arr = NULL;
        int operands_count = 0;

        /* break word */
        operands_count = parse_string_into_string_array(data_model, (*line_params)[*line_params_count - 1].parsed_params[word], &arr, ",");
        /* @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ */

        if (operands_count == 0)
        {
            printf("the if is temporary code");
        }

        if ((*line_params)[*line_params_count - 1].param_count == word + 1) /* both operands in 1 word */
        {
            if ((*line_params)[*line_params_count - 1].parsed_params[word][0] == ',' || (*line_params)[*line_params_count - 1].parsed_params[word][strlen((*line_params)[*line_params_count - 1].parsed_params[word]) - 1] == ',' || strchr((*line_params)[*line_params_count - 1].parsed_params[word], ',') != strrchr((*line_params)[*line_params_count - 1].parsed_params[word], ','))
            {
                fprintf(stdout, "Error. Number of opperands is illegal\n");
                return ERR_NUMBER_OPERANDS_FOR_COMMAND;
            }

            /* break the word with comma */
        }
        else if ((*line_params)[*line_params_count - 1].param_count == word + 2) /* comma include in one operand */
        {
            if ((*line_params)[*line_params_count - 1].parsed_params[word][strlen((*line_params)[*line_params_count - 1].parsed_params[word]) - 1] != ',')
            {
                if ((*line_params)[*line_params_count - 1].parsed_params[word + 1][0] != ',')
                {
                    fprintf(stdout, "Error. Missing comma between operands\n");
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
        else if ((*line_params)[*line_params_count - 1].param_count == word + 3) /* comma is a seperate word and must be in word + 1  */
        {
            if (!(strcmp((*line_params)[*line_params_count - 1].parsed_params[word + 1], ",")))
            {
                fprintf(stdout, "Error. Missing comma between operands\n");
                return ERR_MISSING_COMMA;
            }

            addressing_source = check_addressing(&(*line_params)[*line_params_count - 1].parsed_params[word]);
            if (addressing_source < 0)
            {
                fprintf(stdout, "Error. Invalid source addressing\n");
                return ERR_INVALID_ADDRESSING;
            }

            if (!(assembler_commands[result].allowed_source_operand_adderss_type & 1u << addressing_source))
            {
                fprintf(stdout, "Error. Wrong type of source addressing of target\n");
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

    temp <<= first_index;
    *word = (*word) | temp;

    return *word;
}

/**
 * @brief check addressing type for an operand
 *
 * @param word a string represent the opernad
 * @return int the type of addressing
 */
int check_addressing(char **word)
{
    int i;
    char registers[NUM_OF_RESIGTERS][2] = RESIGTERS;

    /* immediate  */
    if ((*word)[0] == '#')
    {
        /*check if its defined or everything is numbers */

        return 0;
    }

    /* direct */
    if (0)
    {
        return 1;
    }

    /* permenent index */
    if (0)
    {
        return 2;
    }

    /* direct register */
    for (i = 0; i < NUM_OF_RESIGTERS; i++)
    {
        if (!(strcmp(*word, registers[i])))
        {
            return 3;
        }
    }

    return ERR_INVALID_ADDRESSING;
}