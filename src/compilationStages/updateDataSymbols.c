#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../datamodel.h"

void update_data_symbols(size_t instruction_count, size_t symbol_count, Symbol *symbol_table)
{
    int i = 0;
    for (i = 0; i < symbol_count; i++)
    {
        if (symbol_table[i].type == DATA)
        {
            symbol_table[i].value += instruction_count;
        }
    }
}
