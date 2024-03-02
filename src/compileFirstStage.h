#ifndef __COMPILE_FIRST_STAGE__
#define __COMPILE_FIRST_STAGE__
#include "datamodel.h"

int compileFirstStage(const char *filename, Symbol **symbols, size_t *symbol_count, Opcode **opcodes, size_t *opcode_count, Line_params **line_params, size_t *line_params_count);
int compileFirstStage2(const char *filename, Symbol **symbols, size_t *symbol_count, Opcode **opcodes, size_t *opcode_count, Line_params **line_params, size_t *line_params_count);
#endif
