#ifndef __CREATE_SYMBOLS__
#define __CREATE_SYMBOLS__

#include <stdlib.h>
#include <stdio.h>
#include "../datamodel.h"

int createSymbols(Symbol **symbols,
                  size_t *symbol_count,
                  Line_params **line_params,
                  size_t *line_params_count,
                  int *instruction_count,
                  int *data_count,
                  Assembly_code **assembly_code,
                  size_t *assembly_code_count);

int add_int_array_to_assembly(Assembly_code **assembly_code,
                              size_t *assembly_code_count,
                              Line_params line_params,
                              size_t *line_params_count,
                              int *data_count,
                              Symbol *symbols,
                              size_t symbol_count);

int add_char_array_to_assembly(Assembly_code **assembly_code,
                               size_t *assembly_code_count,
                               Line_params line_params,
                               size_t *line_params_count,
                               int *data_count);

int parse_string_into_int_array(const char *buffer,
                                int **result_array,
                                const char *delimitors,
                                size_t *count,
                                Symbol *symbols,
                                size_t symbol_count);
#endif
