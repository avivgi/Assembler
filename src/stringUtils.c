#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "global_constants.h"
#include "stringUtils.h"
#include "datamodel.h"
#include "memoryUtils.h"

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
        fprintf(stderr, "Error: Memory reallocation error. Existing\n");
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
                fprintf(stderr, "Error: Memory reallocation error. Existing\n");
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
    // /* int i;
    // /* int x = 0;
    // /* int j = 0;
    // /* char temp[MAX_PARAM_SIZE];
    // /* size_t length = strlen(input);
    // /* for (i = 0; i < length; i++)
    // /* {
    // /*     if (x == MAX_PARAMS_IN_COMMAND - 1)
    // /*     {
    // /*         printf("Extraneous text after end of command\n");
    // /*         return ERR_EXTRA_TEXT_AFTER_COMMAS;
    // /*     }
    // /*     if (input[i] == split_by)
    // /*     {
    // /*         temp[j] = '\0';
    // /*         strcpy(arr[x], temp);
    // /*         temp[0] = '\0';
    // /*         j = 0;
    // /*         if (x == arr_length)
    // /*         {
    // /*             return ERR_EXTRA_TEXT_AFTER_COMMAS;
    // /*         }
    // /*         x++;
    // /*     }
    // /*     else
    // /*     {

    // /*         if ((j < MAX_PARAM_SIZE - 1) && (temp[j] != '\n'))
    // /*         {
    // /*             temp[j] = input[i];
    // /*             j++;
    // /*         }
    // /*     }
    // /* }

    // /* if (temp[j - 1] == '\n')
    // /* {
    // /*     temp[j - 1] = '\0';
    // /* }
    // /* else
    // /* {
    // /*     temp[j] = '\0';
    // /* }
    // /* strcpy(arr[x], temp);
    // /* return x + 1;
    // return 0; */
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

int parse_line(Line_params **line_params, size_t *line_params_count, char *buffer)
{
    char *token;
    int i;

    /* init new Line Params*/

    /* Allocate memory for a new Line_params struct */
    *line_params = realloc(*line_params, (*line_params_count + 1) * sizeof(Line_params));
    if (*line_params == NULL)
    {
        fprintf(stderr, "Failed allocating memory, existing.\n");
        exit(ERR_MEMORY_ALLOCATION_ERROR);
    }

    /* Initialize the new Line_params struct */
    (*line_params)[*line_params_count].line_type = -1; /* Example line type, replace with your logic*/

    /* Allocate memory for parsed_params array of char pointers */
    (*line_params)[*line_params_count]
        .parsed_params = malloc(MAX_PARAM_COUNT * sizeof(char *));
    if ((*line_params)[*line_params_count].parsed_params == NULL)
    {
        fprintf(stderr, "Failed allocating memory, existing.\n");
        exit(ERR_MEMORY_ALLOCATION_ERROR);
    }

    /* Allocate memory for param_type array */
    (*line_params)[*line_params_count].param_type = malloc(MAX_PARAM_COUNT * sizeof(int));
    if ((*line_params)[*line_params_count].param_type == NULL)
    {
        fprintf(stderr, "Failed allocating memory, existing.\n");
        exit(ERR_MEMORY_ALLOCATION_ERROR);
    }

    /* Allocate memory for each char pointer in parsed_params */
    for (i = 0; i < MAX_PARAM_COUNT; i++)
    {
        (*line_params)[*line_params_count].parsed_params[i] = malloc((MAX_PARAM_SIZE + 1) * sizeof(char));
        if ((*line_params)[*line_params_count].parsed_params[i] == NULL)
        {
            fprintf(stderr, "Failed allocating memory, existing.\n");
            exit(ERR_MEMORY_ALLOCATION_ERROR);
        }
    }

    /* end of init new Line Params*/
    i = 0;
    token = strtok(buffer, "\t\n\f\r ");
    snprintf((*line_params)[*line_params_count].parsed_params[i++], MAX_PARAM_SIZE, "%s", token);

    while (token != NULL)
    {
        token = strtok(NULL, "\t\n\f\r ");
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
