#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "stringUtils.h"
#include "compile.h"
#include "compileFirstStage.h"
#include "compileSecondStage.h"
#include "global_constants.h"
#include "datamodel.h"

int compile(const char *filename, Data_model *data_model, Language *language)
{
    compileFirstStage(filename, data_model, language);

    printf("%s\n", data_model->symbols[0].name);
    printf("%s\n", data_model->symbols[0].name);

    // printf("\n name is: %s value is: %d \n", symbols[symbol_count - 1].name, symbols[symbol_count - 1].value);
    compileSecondStage(filename);

    return 0;
}
