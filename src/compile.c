#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "stringUtils.h"
#include "compile.h"
#include "compileFirstStage.h"
#include "compileSecondStage.h"
#include "global_constants.h"
#include "datamodel.h"

int compile(const char *filename)
{
    size_t symbol_count = 0;
    size_t opcode_count = 0;

    Symbol *symbols = NULL;
    Opcode *opcodes = NULL;
    //[MAX_PROGRAM_LENGTH];

    Line_params *line_params = NULL;
    size_t line_params_count;

    compileFirstStage(filename, &symbols, &symbol_count, &opcodes, &opcode_count, &line_params, &line_params_count);
    printf("%s\n", symbols[0].name);
    printf("%s\n", symbols[1].name);

    // printf("\n name is: %s value is: %d \n", symbols[symbol_count - 1].name, symbols[symbol_count - 1].value);
    compileSecondStage(filename);

    return 0;
}
