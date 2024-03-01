#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "assembler.h"
#include "preCompile.h"
#include "compile.h"
#include "datamodel.h"
#include "language.h"

/* AVIV GIDEONI OPEN UNIVERSITY 2024*/
/*program receives the name of a file, and opens it
in takes each unique word in the file and prints it and in which lines it appeared
*/

int main(int argc, char *argv[])
{
    Data_model data_model;
    Language language;

    int filesCount = argc - 1;
    int i = 1;

    initlize_language(&language);
    initilize_data_model(&data_model);

    if (filesCount < 1)
    {
        fprintf(stderr, "\nNo arguments recieved. Existing.\n");
        exit(1);
    }
    while (--argc)
        handleSourceFile(argv[i++], data_model, language);

    exit(0);
}

void handleSourceFile(char *arg, Data_model data_model, Language language)
{

    preCompile(arg);
    printf("Precompilition completed\n");
    compile(arg, &data_model, &language);

    // printSymbolTable();

    // exportFiles();

    /// free(everything );

    //// fclose(src);

    //// fclose(destination);
}
