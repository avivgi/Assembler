#ifndef __COMMANDS__
#define __COMMANDS__

#include <stdlib.h>
#include <stdio.h>
#include "../datamodel.h"
#include "../language.h"

int commands(Data_model *data_model, Line_params **line_params, size_t *line_params_count);
int check_addressing(char **word);
int write_bits_in_word(Word *word, int write_value, int value_len, int first_index);
#endif
