#include <stdio.h>
#include "datamodel.h"
#include <string.h>

int isLabelExist(char *label, Symbol *symbol_table, int symbol_count)
{
    int i;
    for (i = 0; i < symbol_count; i++)
    {
        if (strcmp(symbol_table[i].name, label) == 0)
        {
            return 1;
        }
    }
    return 0;
}

int getLabelAddress(char *label, Symbol *symbol_table, int symbol_count)
{
    int i;
    for (i = 0; i < symbol_count; i++)
    {
        if (strcmp(symbol_table[i].name, label) == 0)
        {
            printf("label %s found at %d\n", label, symbol_table[i].value);
            return symbol_table[i].value;
        }
    }
    return 0;
}
