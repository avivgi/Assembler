#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../language.h"
#include "../datamodel.h"

/**
 * @brief Updates the address of data symbols and data table in the data model.
 *
 * This function updates the address of data symbols and data table in the data model
 * by adding the instruction count and the code start address.
 *
 * @param data_model A pointer to the Data_model struct.
 */
void update_data_address(Data_model *data_model)
{
    int i = 0;
    size_t instruction_count = data_model->instruction_count;

    /* Update the address of data symbols */
    for (i = 0; i < data_model->symbol_count; i++)
    {
        if (data_model->symbols[i].type == DATA)
        {
            data_model->symbols[i].value += instruction_count + CODE_START_ADDRESS;
        }
    }

    /* Update the address of data table */
    for (i = 0; i < data_model->data_count; i++)
    {
        data_model->data_table[i].address += instruction_count + CODE_START_ADDRESS;
    }
}
