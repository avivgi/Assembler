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

int compile(const char *filename)
{
    size_t symbol_count = 0;
    size_t opcode_count = 0;

    Symbol *symbols = NULL;
    Opcode *opcodes = NULL;

    Line_params *line_params = NULL;
    size_t line_params_count = 0;

    compileFirstStage(filename, &symbols, &symbol_count, &opcodes, &opcode_count, &line_params, &line_params_count);
    compileSecondStage(filename);

    print_symbol_table(&symbols, &symbol_count);
    print_opcode_table(&opcodes, &opcode_count);
    return 0;
}
