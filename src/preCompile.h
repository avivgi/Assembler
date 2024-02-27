#ifndef __PRE_ASSEMBLER__
#define __PRE_ASSEMBLER__
#include "global_constants.h"

typedef char Command_line[MAX_LINE_LENGTH];

typedef struct
{
    char macro_name[MAX_PARAM_SIZE];
    Command_line *command_line;
    int number_of_lines;
} macro;

int preCompile(const char *arg);

#endif
