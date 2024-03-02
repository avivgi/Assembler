#include "languageUtils.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int legalLabel(char *label_name, Symbol **symbols, size_t symbol_count) /*not good enough*/
{
    As_Command assember_commands[NUM_OF_COMMANDS_IN_LANGUAGE] = AS_COMMAND_LIST;
    const char registers[NUNMER_OF_REGISTERS][3] = RESIGTERS;

    int i;
    if (isalpha(label_name[0]) == 0)
    {
        fprintf(stderr, "The first character of the label %s isn't a letter\n", label_name);
        return FIRST_LETTER_IS_NOT_A_LETTER;
    }
    for (i = 0; i < NUM_OF_COMMANDS_IN_LANGUAGE; i++)
    {
        if (strcmp(label_name, (assember_commands[i].command_name)) == 0)
        {
            fprintf(stderr, "The name of the label %s is already taken\n", label_name);
            return ERR_LABEL_OR_NAME_IS_TAKEN; /*illegal*/
        }
    }

    for (i = 0; i < NUNMER_OF_REGISTERS; i++)
    {
        if (strcmp(label_name, (registers[i])) == 0)
        {
            fprintf(stderr, "The name of the label %s is already taken\n", label_name);

            return ERR_LABEL_OR_NAME_IS_TAKEN; /*illegal*/
        }
    }
    for (i = 0; i < symbol_count; i++)
    {
        if (strcmp(label_name, (*symbols)[i - 1].name) == 0)
        {
            fprintf(stderr, "The name of the label %s is already taken\n", label_name);
            return ERR_LABEL_OR_NAME_IS_TAKEN;
        }
    }
    /*for (i = 0; i < macro_amount;i++){
        if(strcmp(label_name, (macro_names[i])) != 0){
            return ERR_LABEL_OR_NAME_IS_TAKEN; ~illegal~}
    }*/

    if (strcmp(label_name, ".data") == 0)
    {
        fprintf(stderr, "Illegal name for label\n");
        return ERR_LABEL_OR_NAME_IS_TAKEN; /*illegal*/
    }
    if (strcmp(label_name, ".string") == 0)
    {
        fprintf(stderr, "Illegal name for label\n");
        return ERR_LABEL_OR_NAME_IS_TAKEN; /*illegal*/
    }
    if (strcmp(label_name, ".entry") == 0)
    {
        fprintf(stderr, "Illegal name for label\n");
        return ERR_LABEL_OR_NAME_IS_TAKEN; /*illegal*/
    }
    if (strcmp(label_name, ".extern") == 0)
    {
        fprintf(stderr, "Illegal name for label\n");
        return ERR_LABEL_OR_NAME_IS_TAKEN; /*illegal*/
    }

    return 0;
}
