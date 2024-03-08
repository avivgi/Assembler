#ifndef __CREATE_EXTERN__
#define __CREATE_EXTERN__

#include <stdlib.h>
#include <stdio.h>
#include "../datamodel.h"

int createExtern(Symbol **symbols, size_t *symbol_count, Line_params **line_params, size_t *line_params_count, int *instruction_count, int *data_count);

#endif
