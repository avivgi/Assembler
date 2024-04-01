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

    Line_params *line_params = NULL;
    size_t line_params_count = 0;

    initDataModel(&data_model);

    compileFirstStage(filename, &data_model, &line_params, &line_params_count);
    compileSecondStage();

    if (DEBUG)
    {
        print_symbol_table(&data_model.symbols, &data_model.symbol_count, filename);

        printf("\nPrinting instructions_table table with %lu entries\n", (unsigned long)data_model.instruction_count);
        print_word_entry_table(data_model.instructions_table, data_model.instruction_count, filename, "instruction");

        printf("\nPrinting data_table table with %lu entries\n", (unsigned long)data_model.data_count);
        print_word_entry_table(data_model.data_table, data_model.data_count, filename, "data");
    }
    print_instruction_table(data_model, filename);
    print_entry_and_extern_table(data_model, filename, ENTRY);
    print_entry_and_extern_table(data_model, filename, EXTERN);

    return 0;
}
