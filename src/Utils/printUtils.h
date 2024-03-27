#ifndef __PRINTUTILS_H__
#define __PRINTUTILS_H__
#include <stdlib.h>
#include <stdio.h>

#include "../language.h"
#include "../datamodel.h"

void print_symbol_table(Symbol **Symbols, size_t *symbol_count, const char *filename);
void print_word_entry_table(Word_entry *table, size_t size, const char *filename, const char *table_name);
void print_word(FILE *stream, Word word);
#endif
