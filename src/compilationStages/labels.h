#ifndef __CREATE_SYMBOLS__
#define __CREATE_SYMBOLS__

#include <stdlib.h>
#include <stdio.h>
#include "../datamodel.h"

int labels(Data_model *data_model,
           Line_params *line_params,
           size_t line_params_count);

int add_int_array_to_data_table(Data_model *data_model,
                                Line_params line_params,
                                int param_count);

int add_char_array_to_assembly(Data_model *data_model,
                               Line_params line_params,
                               size_t line_params_count);

int parse_string_into_int_array(Data_model *data_model,
                                char *buffer,
                                int **result_array,
                                const char *delimiters);

int is_label_data_extern_string(Line_params line_params, size_t line_params_count);
char *get_label_entry(Data_model data_model, Line_params line_params, size_t line_params_count);
void update_entry_symbol(Data_model *data_model, char *entry_label_name);
int is_label_entry(Line_params line_params, size_t line_params_count);
#endif
