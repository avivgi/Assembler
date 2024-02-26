#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "assembler.h"

/* AVIV GIDEONI OPEN UNIVERSITY 2024*/
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
    {
        handleSourceFile(argv[i]);
        i++;
    }
    exit(0);
}

void handleSourceFile(char *arg)
{

    // setupMemoryTables();

    // parseSourceFile(src, destination);

    // exportMacroTable();

    // parseAssemblyCode(destination);

    // evalCounters();

    //  printSymbolTable();

    // parseAssemblyCode(destination);

    // exportFiles();

    /// printf("\Errors occoured during parsing. existing.\n");
    ///  printf("\nexpanding macros for %s source file\nfailed due to some code errors\nmoving on to the next file if exist\n\n", fileName);

    /// free(everything );
    // fclose(src);
    // fclose(destination);
}
