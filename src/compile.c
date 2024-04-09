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
 * @param filename The name of the file to compile.
 * @return Returns an integer indicating the success or failure of the compilation process.
 */
int compile(const char *filename)
{
    Data_model data_model;
    int result;

    initDataModel(&data_model);

    result = compileFirstStage(filename, &data_model);
    if (result > 0)
    {
        fprintf(stderr, "Compilation terminated due to errors for file: %s.\n", filename);
        if (!DEBUG)
            return result;
    }
    result += compileSecondStage(filename, &data_model);
    if (result != 0)
    {
        fprintf(stderr, "Compilation terminated due to errors for file: %s.\n", filename);
        if (!DEBUG)
            return result;
    }

    if (DEBUG)
    {
        print_symbol_table(&data_model.symbols, &data_model.symbol_count, filename);
        print_word_entry_table(data_model.instructions_table, data_model.instruction_count, filename, "instruction");
        print_word_entry_table(data_model.data_table, data_model.data_count, filename, "data");
        printf("Result after two phases: %d\n", result);
    }

    print_instruction_table(data_model, filename);
    print_entry_and_extern_table(data_model, filename, ENTRY);
    print_extern_table(data_model, filename);

    return 0;
}
