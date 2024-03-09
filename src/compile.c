#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Utils/stringUtils.h"
#include "compile.h"
#include "compileFirstStage.h"
#include "compileSecondStage.h"
#include "global_constants.h"
#include "datamodel.h"
#include "Utils/printUtils.h"

/**
 * Compiles the given file.
 *
 * @param filename The name of the file to compile.
 * @return Returns an integer indicating the success or failure of the compilation process.
 */
int compile(const char *filename)
{
    Assembly_code *assembly_codes = NULL;
    size_t assembly_code_count = 0;

    Symbol *symbols = NULL;
    size_t symbol_count = 0;

    Reference_address *entries = NULL;
    size_t entry_count = 0;

    Reference_address *externals = NULL;
    size_t externals_count = 0;

    Line_params *line_params = NULL;
    size_t line_params_count = 0;

    compileFirstStage(filename, &symbols, &symbol_count, &assembly_codes, &assembly_code_count, &line_params, &line_params_count, &entries, &entry_count, &externals, &externals_count);
    compileSecondStage(filename);

    print_symbol_table(&symbols, &symbol_count);
    print_assembly_code_table(&assembly_codes, &assembly_code_count);
    return 0;
}
