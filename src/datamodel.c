#include <stdio.h>
#include "datamodel.h"
#include <string.h>

/**
 * Checks if a label exists in the symbol table.
 *
 * @param label The label to search for.
 * @param symbol_table The symbol table to search in.
 * @param symbol_count The number of symbols in the symbol table.
 * @return 1 if the label exists in the symbol table, 0 otherwise.
 */

int isLabelExist(char *label, Symbol *symbol_table, int symbol_count)
{
    int i;
    for (i = 0; i < symbol_count; i++)
    {
        if (strcmp(symbol_table[i].name, label) == 0)
        {
            return i;
        }
    }
    return EER_LABEL_NOT_FOUND;
}

/**
 * Gets the address of a label in the symbol table.
 *
 * @param label The label to search for.
 * @param symbol_table The symbol table to search in.
 * @param symbol_count The number of symbols in the symbol table.
 * @return The address of the label in the symbol table.
 */
int getLabelAddress(char *label, Symbol *symbol_table, int symbol_count)
{
    int i;
    for (i = 0; i < symbol_count; i++)
    {
        if (strcmp(symbol_table[i].name, label) == 0)
            return symbol_table[i].value;
    }
    return 0;
}

void initDataModel(Data_model *data_model)
{
    data_model->symbol_count = 0;
    data_model->symbols = NULL;
    data_model->instruction_count = 0;
    data_model->instructions_table = NULL;
    data_model->data_count = 0;
    data_model->data_table = NULL;
}
