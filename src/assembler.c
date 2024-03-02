#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "assembler.h"
#include "preCompile.h"
#include "compile.h"

/* AVIV GIDEONI & OMER RAZ OPEN UNIVERSITY 2024*/
/*program receives the name of a file, and opens it
in takes each unique word in the file and prints it and in which lines it appeared
*/

int main(int argc, char *argv[])
{

    int filesCount = argc - 1;
    int i = 1;
    if (filesCount < 1)
    {
        fprintf(stderr, "\nNo arguments recieved. Existing.\n");
        exit(1);
    }
    while (--argc)
        handleSourceFile(argv[i++]);

    exit(0);
}

void handleSourceFile(char *arg)
{

    // setupMemoryTables();

    preCompile(arg);

    compile(arg);

    // evalCounters();

    // printSymbolTable();

    // exportFiles();

    /// free(everything );
    //// fclose(src);
    //// fclose(destination);
}
