#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "preCompile.h"
#include "Utils/stringUtils.h"
#include "global_constants.h"
#include "Utils/memoryUtils.h"

#define INITIAL_COMMAND_LINE_SIZE 2

/**
 * Precompiles the given file.
 *step 1 - read line
 *step 2 - check if the line is a comment
 *step 3 - check if the line is a macro
 *step 4 - if the line is a macro, add it to the list of macros
 *step 5 - if the line is a macro, read the macro and add it to the list of macros
 *step 6 - if the line is a macro, continue to the next line
 *step 7 - if the line is not a macro, print it to the destination file
 * @param arg The name of the file to precompile.
 * @return Returns an integer indicating the success or failure of the precompilation process.
 */
int preCompile(const char *arg)
{
    macro *list_of_macros = NULL;
    int number_of_macros = 0, j = 0, i = 0;
    FILE *source, *destination;
    char check_for_macro[MAX_PARAM_SIZE];
    char macro_name[MAX_PARAM_SIZE];
    char *line = NULL;
    int line_number = 0;
    int is_macro = 0;
    char *start_of_macro;

    char *fileName = (char *)calloc(strlen(arg) + 4, sizeof(char));
    if (!fileName)
        EXIT_ON_MEM_ALLOC_FAIL_EARLY

    strcpy(fileName, arg);
    strcat(fileName, ".as");
    if ((source = fopen(fileName, "r")) == NULL)
    {
        fprintf(stderr, "Error: Error. Failed open file %s\n", fileName);
        safe_free(1, fileName);
        return (ERR_OPEN_FILE);
    }
    strcpy(fileName, arg);
    strcat(fileName, ".am");
    if ((destination = fopen(fileName, "w+")) == NULL)
    {
        fprintf(stderr, "Error: Error! Failed open file %s\n", fileName);
        safe_free(1, fileName);
        fclose(source);
        return (ERR_OPEN_FILE);
    }

    /*start reading the file and looking for macros*/
    while (read_line(source, &line))
    {
        is_macro = 0;
        line_number++;

        if (string_length_without_white_spaces(line) < 3)
            continue;

        if (is_comment(&line))
            continue;

        parse_command(line, check_for_macro, macro_name, line_number);

        start_of_macro = strstr(line, "mcr");
        if (start_of_macro != NULL)
            if (strcmp(check_for_macro, "mcr") == 0) /*beginning of a macro*/
            {
                /*need to increase the size of the macro list*/
                list_of_macros = realloc(list_of_macros, (++number_of_macros) * sizeof(macro));
                if (list_of_macros == NULL)
                {
                    safe_free(2, fileName, line);
                    fclose(source);
                    fclose(destination);
                    EXIT_ON_MEM_ALLOC_FAIL_EARLY
                }

                list_of_macros[number_of_macros - 1].number_of_lines = 0;
                list_of_macros[number_of_macros - 1].command_line = malloc(INITIAL_COMMAND_LINE_SIZE * sizeof(Command_line));
                if (list_of_macros[number_of_macros - 1].command_line == NULL)
                {
                    safe_free(3, fileName, line, list_of_macros);
                    fclose(source);
                    fclose(destination);
                    EXIT_ON_MEM_ALLOC_FAIL_EARLY
                }
                strcpy(list_of_macros[number_of_macros - 1].macro_name, macro_name);

                /*pulling the content of the macro*/
                while (read_line(source, &line))
                {
                    char *end_of_macro = strstr(line, "endmcr");
                    if (end_of_macro != NULL)
                        break;
                    if (list_of_macros[number_of_macros - 1].number_of_lines >= INITIAL_COMMAND_LINE_SIZE)
                    {
                        list_of_macros[number_of_macros - 1].command_line = realloc(list_of_macros[number_of_macros - 1].command_line,
                                                                                    (list_of_macros[number_of_macros - 1].number_of_lines * 2) * sizeof(Command_line));
                        if (list_of_macros[number_of_macros - 1].command_line == NULL)
                        {
                            safe_free(3, fileName, line, list_of_macros);
                            fclose(source);
                            fclose(destination);
                            EXIT_ON_MEM_ALLOC_FAIL_EARLY
                        }
                    }
                    strcpy(list_of_macros[number_of_macros - 1].command_line[list_of_macros[number_of_macros - 1].number_of_lines], line);
                    list_of_macros[number_of_macros - 1].number_of_lines++;
                } /*end of macro*/
                continue;
            }

        /*check if the line is a macro*/
        for (j = 0; j < number_of_macros; j++)
        {
            start_of_macro = strstr(line, list_of_macros[j].macro_name);
            if (start_of_macro != NULL)
            {
                for (i = 0; i < list_of_macros[j].number_of_lines; i++)
                    fprintf(destination, "%s\n", list_of_macros[j].command_line[i]);
                is_macro = 1;
                break;
            }
        }
        if (!is_macro) /*line isn't a macro, print normally*/
            fprintf(destination, "%s\n", line);
    }

    safe_free(3, fileName, line, list_of_macros);
    fclose(source);
    fclose(destination);
    return 0;
}

/**
 * Checks if a given line is a comment.
 *
 * @param line The line to check.
 * @return true if the line is a comment, false otherwise.
 */
Bool is_comment(char **line)
{
    int i = 0;
    while (((*line)[i] == ' ' || (*line)[i] == '\t') && (i < strlen(*line)))
    {
        i++;
    }
    if ((*line)[i] == ';')
        return true;
    return false;
}
