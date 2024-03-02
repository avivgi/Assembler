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
    printf("\nPrinting symbol table with %lu symbols\n", *symbol_count);

    for (i = 0; i < *symbol_count; i++)
    {
        printf("Symbol name: %s\t", (*Symbols)[i].name);
        printf("Value: %d\t", (*Symbols)[i].value);
        printf("type: %d\n", (*Symbols)[i].type);
    }
}
void print_opcode_table(Opcode **Opcodes, size_t *opcode_count)
{
    int i;
    printf("\nPrinting opcode table with %lu opcodes\n", *opcode_count);
    for (i = 0; i < *opcode_count; i++)
    {
        printf("command address: %d\t", (*Opcodes)[i].address);
        printf("opcode: %d\t", (*Opcodes)[i].binary_code);
    }
}
