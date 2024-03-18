#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include "commands.h"

#include "../Utils/stringUtils.h"
#include "../Utils/languageUtils.h"
#include "../global_constants.h"
#include "../Utils/memoryUtils.h"
#include "../Utils/stringUtils.h"
#include "../Utils/languageUtils.h"
#include "../global_constants.h"
#include "../Utils/memoryUtils.h"
char *strdup(const char *s);

int commands(Data_model *data_model, Line_params **line_params, size_t *line_params_count)
{
    As_Command assembler_commands[NUM_OF_COMMANDS_IN_LANGUAGE] = AS_COMMAND_LIST;
    int i;
    int result = ERR_WORD_NOT_FOUND;
    int error_flag = 0;

    for (i = 0; i < NUM_OF_COMMANDS_IN_LANGUAGE; i++)
    {
        if (strcmp((*line_params)[*line_params_count - 1].parsed_params[0],
                   (assembler_commands[i].command_name)) == 0)
        {
            result = i;
            break;
        }
    }
    if (result == ERR_WORD_NOT_FOUND)
    {
        fprintf(stdout, "Error. Didn't find command name: %s\n",
                (*line_params)[*line_params_count - 1].parsed_params[0]);
        return result;
    }

    return result;
}
