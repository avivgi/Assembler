#ifndef __CREATE_EXTERN__
#define __CREATE_EXTERN__

#include <stdlib.h>
#include <stdio.h>
#include "../datamodel.h"

int externs(Data_model *data_model, Line_params *line_params, size_t line_params_count);
void add_extern_reference(Data_model *data_model, char *label);
int is_label_extern(Data_model *data_model, char *label);

#endif
