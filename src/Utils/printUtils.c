#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include "printUtils.h"

void print_symbol_table(Symbol **Symbols, size_t *symbol_count, const char *filename)
{
    FILE *dest;
    char fullFileName[100];
    int i;
    const char *symbolType;

    strcpy(fullFileName, filename);
    strcat(fullFileName, ".symbol");

    dest = fopen(fullFileName, "w");

    printf("\nPrinting symbol table with %d symbols\n", (int)*symbol_count);

    for (i = 0; i < *symbol_count; i++)
    {
        fprintf(dest, "Symbol name: %-10s\t", (*Symbols)[i].name);
        fprintf(dest, "Value: %d\t", (*Symbols)[i].value);

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
        fprintf(dest, "type: %d: %s\n", (*Symbols)[i].type, symbolType);
    }

    fclose(dest);
}

void print_word_entry_table(Word_entry *table, size_t size, const char *filename, const char *table_name)
{
    FILE *dest;
    char fullFileName[100];
    int i;

    strcpy(fullFileName, filename);
    strcat(fullFileName, ".");
    strcat(fullFileName, table_name);

    dest = fopen(fullFileName, "w");

    for (i = 0; i < size; i++)
    {
        fprintf(dest, "Address: %d\t", (table)[i].address);
        fprintf(dest, "value: %d\t word: ", (table)[i].dValue);
        print_word(dest, (table)[i].word);
        fprintf(dest, "\n");
    }
    fclose(dest);
}

void print_word(FILE *stream, Word word)
{
    int i;
    for (i = BITS_IN_WORD - 1; i >= 0; i--)
    {
        fprintf(stream, "%d", (word >> i) & 1);
    }
}
