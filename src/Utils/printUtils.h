#ifndef __PRINTUTILS_H__
#define __PRINTUTILS_H__
#include <stdlib.h>
#include <stdio.h>

#include "../language.h"
#include "../datamodel.h"

void print_symbol_table(Symbol **Symbols, size_t *symbol_count);
void print_assembly_code_table(Assembly_code **Assembly_codes, size_t *assembly_code_count);

#endif
