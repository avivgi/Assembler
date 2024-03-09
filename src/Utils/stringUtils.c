#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include <stdlib.h>
#include <ctype.h>
#include "../global_constants.h"
#include "stringUtils.h"
#include "../datamodel.h"
#include "memoryUtils.h"
#include "../datamodel.h"

/* reads the line from stdin*/
int read_line(FILE *file, char **buffer)
{
    size_t size = MAX_LINE_LENGTH;
    size_t len = 0;
    int c;
    char *temp;

    *buffer = malloc(MAX_LINE_LENGTH);
    if (!(*buffer))
    {
        fprintf(stdout, "Error: Memory reallocation error. Existing\n");
        free(*buffer);
        exit(1);
    }

    while ((c = fgetc(file)) != EOF && c != '\n')
    {
        (*buffer)[len++] = (char)c;

        if (len == size - 1)
        {
            size *= 2;
            temp = realloc(*buffer, size);
            if (!temp)
            {
                fprintf(stdout, "Error: Memory reallocation error. Existing\n");
                free(*buffer);
                exit(1);
            }
            *buffer = temp;
        }
    }

    (*buffer)[len] = '\0';
    return (c == EOF) ? 0 : 1;
}

/*clean white spaces from a buffer*/
void clean_white_space(char **buffer)
{
    char *w = *buffer;
    char *r = *buffer;

    while (*r)
    {
        if (!isspace(*r))
            *w++ = *r;
        r++;
    }
    *w = '\0';
}

/* find if a string exists in a string array*/
int word_check(const char *list[], int length, char *word)
{
    int i;
    for (i = 0; i < length; i++)
    {
        if (strcmp(list[i], word) == 0)
            return i;
    }
    return ERR_WORD_NOT_FOUND;
}

int split(const char *input, char arr[][MAX_PARAM_SIZE], int arr_length, char split_by)
{
    /* int i;
     int x = 0;
    /* int j = 0;
    /* char temp[MAX_PARAM_SIZE];
    /* size_t length = strlen(input);
    /* for (i = 0; i < length; i++)
    /* {
    /*     if (x == MAX_PARAMS_IN_COMMAND - 1)
    /*     {
    /*         printf("Extraneous text after end of command\n");
    /*         return ERR_EXTRA_TEXT_AFTER_COMMAS;
    /*     }
    /*     if (input[i] == split_by)
    /*     {
    /*         temp[j] = '\0';
    /*         strcpy(arr[x], temp);
    /*         temp[0] = '\0';
    /*         j = 0;
    /*         if (x == arr_length)
    /*         {
    /*             return ERR_EXTRA_TEXT_AFTER_COMMAS;
    /*         }
    /*         x++;
    /*     }
    /*     else
    /*     {

    /*         if ((j < MAX_PARAM_SIZE - 1) && (temp[j] != '\n'))
    /*         {
    /*             temp[j] = input[i];
    /*             j++;
    /*         }
    /*     }
    /* }

    /* if (temp[j - 1] == '\n')
    /* {
    /*     temp[j - 1] = '\0';
    /* }
    /* else
    /* {
    /*     temp[j] = '\0';
    /* }
    /* strcpy(arr[x], temp);
    /* return x + 1;
    return 0; */
    return 0;
}

int parse_command(char *buffer, char *command, char *first_param)
{
    int i = 0;
    int j = 0;
    int result = 0;

    /* bypass spaced before command*/
    while (((buffer[i] != '\0') && (buffer[i] != ',')) && ((buffer[i] == ' ') || (buffer[i] == '\t')))
    {
        i++;
    }
    if (buffer[i] == ',')
    {
        printf("Illegal comma\n");
        return ERR_ILLEGAL_COMMA;
    }

    /*Getting Command*/
    while ((buffer[i] != '\0') && (buffer[i] != ' ') && (buffer[i] != '\t') && (buffer[i] != '\n'))
    {
        command[j] = buffer[i];
        result = 1;
        i++;
        j++;
        if (buffer[i] == ',')
        {
            printf("Illegal comma\n");
            return ERR_ILLEGAL_COMMA;
        }
        if (j == MAX_PARAM_SIZE)
        {
            printf("Undefined command name\n");
            return ERR_COMMAND_NOT_FOUND;
        }
    }
    command[i] = '\0';

    if (buffer[i] == ',')
    {
        printf("Undefined command name\n");
        return ERR_ILLEGAL_COMMA;
    }

    if (result == 0)
        return ERR_MISSING_ARGUMENT;

    /*cleaning spaced between command and 1st param*/
    while ((buffer[i] != '\0') && ((buffer[i] == ' ') || (buffer[i] == '\t')))
        i++;

    if (buffer[i] == ',')
    {
        /* aha! found , right after command*/
        printf("Illegal comma\n");
        return ERR_ILLEGAL_COMMA;
    }

    /*Getting 1st param*/
    j = 0;
    while ((buffer[i] != '\0') && (buffer[i] != ' ') && (buffer[i] != '\t') && (buffer[i] != '\n') && (buffer[i] != ','))
    {
        first_param[j] = buffer[i];
        result = 2;
        j++;
        i++;
    }
    first_param[j] = '\0';

    while ((buffer[i] != '\0') && (buffer[i] != '\n') && (buffer[i] != ','))
    {
        if (((buffer[i] != ' ') && (buffer[i] != '\t')))
        {
            result = 1;
            return ERR_EXTRA_TEXT_NO_COMMA_AFTER_FIRST_PARAM;
        }
        i++;
    }

    return result;
}

int parse_line(Line_params **line_params, size_t *line_params_count, const char *buffer, char *delimitors)
{
    char *token;
    int i;
    char *buffer_c = strdup(buffer);
    if (buffer_c == NULL)
        EXIT_ON_MEM_ALLOC_FAIL

    /* init new Line Params*/

    /* Allocate memory for a new Line_params struct */
    *line_params = realloc(*line_params, (*line_params_count + 1) * sizeof(Line_params));
    if (*line_params == NULL)
    {
        free(buffer_c);
        EXIT_ON_MEM_ALLOC_FAIL
    }

    /* Initialize the new Line_params struct */
    (*line_params)[*line_params_count].line_type = -1;

    /* Allocate memory for parsed_params array of char pointers */
    (*line_params)[*line_params_count]
        .parsed_params = malloc(MAX_PARAM_COUNT * sizeof(char *));
    if ((*line_params)[*line_params_count].parsed_params == NULL)
    {
        free(buffer_c);
        EXIT_ON_MEM_ALLOC_FAIL
    }

    /* Allocate memory for param_type array */
    (*line_params)[*line_params_count].param_type = malloc(MAX_PARAM_COUNT * sizeof(int));
    if ((*line_params)[*line_params_count].param_type == NULL)
    {
        free(buffer_c);
        EXIT_ON_MEM_ALLOC_FAIL
    }

    /* Allocate memory for each char pointer in parsed_params */
    for (i = 0; i < MAX_PARAM_COUNT; i++)
    {
        (*line_params)[*line_params_count].parsed_params[i] = malloc((MAX_PARAM_SIZE + 1) * sizeof(char));
        if ((*line_params)[*line_params_count].parsed_params[i] == NULL)
        {
            free(buffer_c);
            EXIT_ON_MEM_ALLOC_FAIL
        }
    }

    /* end of init new Line Params*/
    i = 0;
    token = strtok(buffer_c, delimitors);
    snprintf((*line_params)[*line_params_count].parsed_params[i++], MAX_PARAM_SIZE, "%s", token);

    while (token != NULL)
    {
        token = strtok(NULL, delimitors);
        snprintf((*line_params)[*line_params_count].parsed_params[i++], MAX_PARAM_SIZE, "%s", token);
    }

    (*line_params)[*line_params_count].param_count = i - 1;
    (*line_params_count)++;

    printf("Contents of parsed_params:\n ");
    for (i = 0; i < (*line_params)[*line_params_count - 1].param_count; i++)
    {
        printf("Line #%d -> parsed_params[%d]: %s\n", *line_params_count - 1, i, (*line_params)[*line_params_count - 1].parsed_params[i]);
    }

    return 0; /* Indicate success */
}

char *mid(char *buffer, int start, int end)
{
    int length;
    char *result = NULL;
    if (end == 0)
        end = strlen(buffer);
    length = end - start + 1;
    result = malloc((length + 1) * sizeof(char));
    if (result == NULL)
        EXIT_ON_MEM_ALLOC_FAIL

    strncpy(result, buffer + start, length);
    result[length] = '\0';

    return result;
}

/*util function that checks if a parameter is a number ANSI C. if is number the funtion returns 1 and change *result to the number. if not, the function returns 0 */
int is_number(const char *s, int *result)
{
    char *end;
    long val;
    if (s == NULL)
    {
        return 0;
    }
    errno = 0;
    val = strtol(s, &end, 10);
    if (errno == ERANGE || val > INT_MAX || val < INT_MIN)
    {
        return 0;
    }
    if (end == s)
    {
        return 0;
    }
    if (*end != '\0')
    {
        return 0;
    }
    *result = (int)val;
    return 1;
}
