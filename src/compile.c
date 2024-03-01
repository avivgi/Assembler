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
    Symbol *symbols;
    Opcode opcodes[MAX_PROGRAM_LENGTH];

    compileFirstStage(filename, symbols, opcodes);
    printf("%d\n", opcodes[0].address);
    compileSecondStage(filename);

    return 0;
}
