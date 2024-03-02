#ifndef __CREATE_DEFINE_SYMBOL__
#define __CREATE_DEFINE_SYMBOL__

#include <stdlib.h>
#include <stdio.h>
#include "../datamodel.h"

int createDefineSymbol(Symbol **symbols, size_t *symbol_count, Line_params **line_params, size_t *line_params_count, char **buffer);

#endif
