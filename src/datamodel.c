#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "datamodel.h"
#include "language.h"
#include "memoryUtils.h"
#include <memory.h>

void initilize_data_model(Data_model *data_model)
{
    data_model->line_params_count = 0;
    data_model->line_params = malloc(sizeof(Line_params));
    data_model->opcodes = NULL;
    data_model->opcode_count = 0;
    data_model->symbols = NULL;
    data_model->symbol_count = 0;
    printf("Data model initilized\n");
}
