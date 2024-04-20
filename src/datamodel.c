#include <stdio.h>
#include "datamodel.h"
#include <string.h>
#include "Utils/memoryUtils.h"

/**
 * Checks if a label exists in the symbol table.
 *
 * @param label The label to search for.
 * @param symbol_table The symbol table to search in.
 * @param symbol_count The number of symbols in the symbol table.
 * @return Index of the label in the table, otherwise negative error.
 */

int isLabelExist(char *label, Symbol *symbol_table, int symbol_count)
{
    int i;
    for (i = 0; i < symbol_count; i++)
        if (strcmp(symbol_table[i].name, label) == 0)
            return i;
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

/**
 * Initializes the data model structure.
 *
 * @param data_model A pointer to the Data_model structure to be initialized.
 */
void initDataModel(Data_model *data_model)
{
    data_model->symbol_count = 0;
    data_model->symbols = NULL;
    data_model->instruction_count = 0;
    data_model->instructions_table = NULL;
    data_model->data_count = 0;
    data_model->data_table = NULL;
    data_model->line_number = 0;
    data_model->externals_count = 0;
    data_model->externals = NULL;
    data_model->line_status = NULL;
}

/**
 * Frees the memory allocated for a Data_model structure.
 *
 * @param data_model A pointer to the Data_model structure to be freed.
 */
void free_data_model(Data_model *data_model)
{
    safe_free(5,
              data_model->symbols,
              data_model->instructions_table,
              data_model->data_table,
              data_model->externals,
              data_model->line_status);
}

/**
 * Updates the line status indicator in the data model.
 * Increments the line number and reallocates memory for the line status array.
 * Sets the newly added line status to 0.
 *
 * @param data_model A pointer to the data model structure.
 */
void line_status_indicator(Data_model *data_model)
{
    data_model->line_number++;
    data_model->line_status = realloc(data_model->line_status, data_model->line_number * sizeof(int));
    data_model->line_status[data_model->line_number - 1] = 0;
}
