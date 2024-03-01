#ifndef __DATA_MODEL__
#define __DATA_MODEL__

#include "global_constants.h"
enum Symbol_type
{
    MDEFINE,
    CODE,
    DATA
};

typedef struct
{
    char name[MAX_PARAM_SIZE];
    enum Symbol_type type;
    int value;
} Symbol;

typedef struct
{
    int address;
    int binary_code;
} Opcode;

#endif
