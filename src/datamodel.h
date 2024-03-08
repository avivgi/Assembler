#ifndef __DATA_MODEL__
#define __DATA_MODEL__
#include "global_constants.h"

enum Symbol_type
{
    UNDEFINED,
    MDEFINE,
    CODE,
    DATA,
    ENTRY,
    EXTERN
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
} Assembly_code;

typedef struct
{
    char name[MAX_PARAM_SIZE];
    int address;
} Reference_address;

typedef struct
{
    char **parsed_params;
    int *param_type;
    int param_count;
    int line_type;
} Line_params;

int isLabelExist(char *label, Symbol *symbol_table, int symbol_count);
int getLabelAddress(char *label, Symbol *symbol_table, int symbol_count);

#endif
