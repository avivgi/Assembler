#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../language.h"
#include "../datamodel.h"

void update_data_address(Data_model *data_model)
{

    int i = 0;
    size_t instruction_count = data_model->instruction_count;
    printf("%d %d\n", data_model->data_count, data_model->symbol_count);
    if (instruction_count == 0)
        return;

    for (i = 0; i < data_model->symbol_count; i++)
        if (data_model->symbols[i].type == DATA)
            data_model->symbols[i].value += instruction_count + CODE_START_ADDRESS;

    for (i = 0; i < data_model->data_count; i++)
        data_model->data_table[i].address += instruction_count + CODE_START_ADDRESS;
}
