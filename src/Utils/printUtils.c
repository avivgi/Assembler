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
void print_assembly_code_table(Assembly_code **Assembly_codes, size_t *assembly_code_count)
{
    int i;
    printf("\nPrinting assembly_code table with %lu assembly_codes\n", *assembly_code_count);
    for (i = 0; i < *assembly_code_count; i++)
    {
        printf("Address: %d\t", (*Assembly_codes)[i].address);
        printf("Machine Code: %d\n", (*Assembly_codes)[i].binary_code);
    }
}
