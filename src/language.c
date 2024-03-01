#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "language.h"

void initlize_language(Language *language)
{
    As_Command as_command[NUM_OF_COMMANDS_IN_LANGUAGE] = AS_COMMAND_LIST;
    size_t j = 0;
    int i = 0;
    while (j < NUM_OF_COMMANDS_IN_LANGUAGE)
    {
        const char *registers[] = REGISTERS;
        language->as_command[i] = as_command[i];
        for (i = 0; i < NUN_OF_REGISTERS; i++)
        {
            language->registers[i] = strdup(registers[i]);
            if (language->registers[i] == NULL)
            {
                printf("Memory allocation failed.\n");
                exit(1);
            }
        }
        j++;
    }
    printf("Language initilized\n");
}
