#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <errno.h>
#include <limits.h>

#include "printUtils.h"

void print_symbol_table(Symbol **Symbols, size_t *symbol_count)
{
    int i;
    const char *symbolType;
    printf("\nPrinting symbol table with %lu symbols\n", (long unsigned int)*symbol_count);

    for (i = 0; i < *symbol_count; i++)
    {
        printf("Symbol name: %-10s\t", (*Symbols)[i].name);
        printf("Value: %d\t", (*Symbols)[i].value);
        switch ((*Symbols)[i].type)
        {
        case UNDEFINED:
            symbolType = "UNDEFINED";
            break;
        case MDEFINE:
            symbolType = "MDEFINE";
            break;
        case CODE:
            symbolType = "CODE";
            break;
        case DATA:
            symbolType = "DATA";
            break;
        case ENTRY:
            symbolType = "ENTRY";
            break;
        case EXTERN:
            symbolType = "EXTERN";
            break;
        default:
            symbolType = "UNKNOWN";
            break;
        }
        printf("type: %d: %s\n", (*Symbols)[i].type, symbolType);
    }
}

void print_word_entry_table(Word_entry *table, size_t size)
{
    int i;
    for (i = 0; i < size; i++)
    {
        printf("Address: %d\t", i);
        printf("Machine Code: %d\n", (table)[i].dValue);
    }
}
