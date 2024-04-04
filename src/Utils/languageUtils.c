#include "languageUtils.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

/**
 * Function to check if a label is legal in the language.
 * legal name is:
 * 1. The first character is a letter.
 * 2. The name is not a command name.
 * 3. The name is not a register name.
 * 4. The name is not a directive name.
 * 5. The name is not a label name.
 * @param label_name The name of the label to check.
 * @param symbols The array of symbols.
 * @param symbol_count The number of symbols in the array.
 * @return 0 if the label is legal, an error code otherwise.
 */
int legalLabel(char *label_name, Symbol **symbols, size_t symbol_count) /*not good enough*/
{
    As_Command assember_commands[NUM_OF_COMMANDS_IN_LANGUAGE] = AS_COMMAND_LIST;
    const char registers[NUNMER_OF_REGISTERS][3] = RESIGTERS;

    int i;
    if (isalpha(label_name[0]) == 0)
    {
        fprintf(stdout, "The first character of the label %s isn't a letter\n", label_name);
        return FIRST_LETTER_IS_NOT_A_LETTER;
    }
    for (i = 0; i < NUM_OF_COMMANDS_IN_LANGUAGE; i++)
    {
        if (strcmp(label_name, (assember_commands[i].command_name)) == 0)
        {
            fprintf(stdout, "The name of the label %s is already taken\n", label_name);
            return ERR_LABEL_OR_NAME_IS_TAKEN; /*illegal*/
        }
    }

    for (i = 0; i < NUNMER_OF_REGISTERS; i++)
    {
        if (strcmp(label_name, (registers[i])) == 0)
        {
            fprintf(stdout, "The name of the label %s is already taken\n", label_name);

            return ERR_LABEL_OR_NAME_IS_TAKEN; /*illegal*/
        }
    }
    for (i = 0; i < symbol_count; i++)
    {
        if (strcmp(label_name, (*symbols)[i].name) == 0)
        {
            fprintf(stdout, "The name of the label %s is already taken\n", label_name);
            return ERR_LABEL_OR_NAME_IS_TAKEN;
        }
    }
    /*for (i = 0; i < macro_amount;i++){
        if(strcmp(label_name, (macro_names[i])) != 0){
            return ERR_LABEL_OR_NAME_IS_TAKEN; ~illegal~}
    }*/

    if (strcmp(label_name, ".data") == 0)
    {
        fprintf(stdout, "Illegal name for label\n");
        return ERR_LABEL_OR_NAME_IS_TAKEN; /*illegal*/
    }
    if (strcmp(label_name, ".string") == 0)
    {
        fprintf(stdout, "Illegal name for label\n");
        return ERR_LABEL_OR_NAME_IS_TAKEN; /*illegal*/
    }
    if (strcmp(label_name, ".entry") == 0)
    {
        fprintf(stdout, "Illegal name for label\n");
        return ERR_LABEL_OR_NAME_IS_TAKEN; /*illegal*/
    }
    if (strcmp(label_name, ".extern") == 0)
    {
        fprintf(stdout, "Illegal name for label\n");
        return ERR_LABEL_OR_NAME_IS_TAKEN; /*illegal*/
    }

    return 0;
}
