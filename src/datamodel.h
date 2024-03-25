#ifndef __DATA_MODEL__
#define __DATA_MODEL__
#include "global_constants.h"

#define BITS_IN_WORD 14
typedef char Word[BITS_IN_WORD];

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
    char name[MAX_PARAM_SIZE];
    int address;
} Reference_address;

typedef struct
{
    char **parsed_params;
    // int *param_type;
    int param_count;
    int line_type;
} Line_params;

typedef struct
{
    int address;
    Word word;
    int dValue;
} Word_entry;

typedef struct
{
    Word_entry *instructions_table;
    size_t instruction_count;
    Word_entry *data_table;
    size_t data_count;
    Reference_address *entries;
    size_t entry_count;
    Reference_address *externals;
    size_t externals_count;
    Symbol *symbols;
    size_t symbol_count;
} Data_model;

int isLabelExist(char *label, Symbol *symbol_table, int symbol_count);
int getLabelAddress(char *label, Symbol *symbol_table, int symbol_count);
void initDataModel(Data_model *data_model);

#endif
