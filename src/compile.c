#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "stringUtils.h"
#include "compile.h"
#include "global_constants.h"

int compile(const char *arg)
{

    /* step 1*/
    int data_count = 0, instruction_count = 0;
    FILE *source, *destination;
    char *buffer=NULL; command[MAX_PARAM_SIZE], first_param[MAX_PARAM_SIZE]
    int result;

    char *fileName = (char *)calloc(strlen(arg) + 4, sizeof(char));

    if (!fileName)
    {
        fprintf(stderr, "Failed allocating memory, existing.\n");
        exit(1);
    }
    strcpy(fileName, arg);
    strcat(fileName, ".am");

    if ((source = fopen(fileName, "r")) == NULL)
    {
        fprintf(stderr, "Error! Failed open file %s\n", fileName);
        free(fileName);
        exit(1);
    }

    compile_first_stage();

/*step 16- if errors stop*/
/*step 17- update data with value IC+100 in symbol table*/

    compile_second_step();


exit(0);
        strcat(fileName, ".ob");
    if ((destination = fopen(fileName, "rw")) == NULL)
    {
        fprintf(stderr, "Error! Failed open file %s\n", fileName);
        free(fileName);
        exit(1);
    }

    return 0;
}



int compileFirstStage(){

    /* step 2 - read line */

    while (read_line(source, &buffer))
    {
        printf("%s\n",buffer);
        result = parse_command(buffer, command, first_param)
    }

/* step 3 - if type== define*/
/* step 4 - put define in mdefine table.*/
/* step 5+6  - is sybmol ? */
/* step 7 - is data or string */
/* step 8 - put symbol in symbol table */
/* step 9 - identify data/params and put them in mem table (which?) update DC */
/* step 10 - if extern or entry */
/* step 11 - if extern put in etx table*/
/* step 12 - if symbol put in symbol table*/
/* step 13 - lookup operation in table*/
/* step 14 - calculate L , build binary code of first word*/
/* step 15 - IC = IC + L . goto #2*/

return 0;
}



int    compile_second_step(){


}