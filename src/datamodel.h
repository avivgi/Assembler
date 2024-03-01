#ifndef __DATA_MODEL__
#define __DATA_MODEL__

#include <stdlib.h>
#include "global_constants.h"

enum Symbol_type
{
    MDEFINE,
    CODE,
    DATA
};

typedef struct
{
    char name[MAX_PARAM_SIZE];
    enum Symbol_type type;
    int value;
} Symbol;

typedef struct
{
    int address;
    int binary_code;
} Opcode;

typedef struct
{
    char **parsed_params;
    int *param_type;
    int param_count;
    int line_type;
} Line_params;

typedef struct
{
    Line_params *line_params;
    size_t line_params_count;
    Opcode *opcodes;
    size_t opcode_count;
    Symbol *symbols;
    size_t symbol_count;

} Data_model;

void initilize_data_model(Data_model *data_model);

#endif
