#ifndef __COMPILE_FIRST_STAGE__
#define __COMPILE_FIRST_STAGE__
#include "datamodel.h"

int compileFirstStage(const char *filename,
                      Symbol **symbols,
                      size_t *symbol_count,
                      Assembly_code **assembly_codes,
                      size_t *assembly_code_count,
                      Line_params **line_params,
                      size_t *line_params_count,
                      Reference_address **entries,
                      size_t *entry_count,
                      Reference_address **externals,
                      size_t *externals_count);

int compileFirstStage2(const char *filename, Symbol **symbols, size_t *symbol_count, Assembly_code **assembly_codes, size_t *assembly_code_count, Line_params **line_params, size_t *line_params_count);
#endif
