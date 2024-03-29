#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include <stdlib.h>
#include <ctype.h>
#include "stringUtils.h"
#include "../global_constants.h"
#include "../datamodel.h"
#include "memoryUtils.h"
#include "../datamodel.h"

char *strdup(const char *s);
#ifndef snprintf
void snprintf(char *str, int len, ...);
#endif

/**
 * @brief Reads a line from a file and places it in the given buffer.
 *
 * @param file The file to read from.
 * @param buffer Pointer to the buffer where the line will be stored.
 * @return Returns 1 if reached EOF, 0 otherwise.
 */
int read_line(FILE *file, char **buffer)
{
    size_t size = MAX_LINE_LENGTH;
    size_t len = 0;
    int c;
    char *temp;

    *buffer = malloc(MAX_LINE_LENGTH * sizeof(char));
    if (!(*buffer))
        EXIT_ON_MEM_ALLOC_FAIL

    while ((c = fgetc(file)) != EOF)
    {
        if (c == '\n' || c == EOF)
        {
            (*buffer)[len] = '\0';
            return 1;
        }

        (*buffer)[len++] = (char)c;

        if (len == size - 1)
        {
            size *= 2;
            temp = realloc(*buffer, size * sizeof(char));
            if (!temp)
            {
                free(*buffer);
                EXIT_ON_MEM_ALLOC_FAIL
            }
            *buffer = temp;
        }
    }

    if (len > 0)
    {
        (*buffer)[len] = '\0';
        return 1;
    }
    else
        return 0;
}

/**
 * @brief Cleans white spaces from a buffer by removing all whitespace characters.
 *
 * @param buffer Pointer to the buffer to clean.
 */
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

/**
 * @brief Gets the length of a string without white spaces.
 *
 * @param s The string to get the length of.
 * @return Returns the length of the string without white spaces.
 */
int string_length_without_white_spaces(const char *s)
{
    int length = 0;
    while (*s)
    {
        if (!isspace(*s))
            length++;
        s++;
    }
    return length;
}

/**
 * @brief Finds if a string exists in a string array.
 *
 * @param list The string array to search in.
 * @param length The length of the string array.
 * @param word The word to search for.
 * @return Returns the index of the found word, or ERR_WORD_NOT_FOUND if not found.
 */
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
/**
 * @brief Parses a command from a buffer and extracts the command and the first parameter.
 *
 * @param buffer The buffer containing the command and parameter.
 * @param command Pointer to store the extracted command.
 * @param first_param Pointer to store the extracted first parameter.
 * @return Returns the result code: 0 for success, or an error code if an error occurred.
 */
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
            return ERR_EXTRA_TEXT_NO_COMMA_AFTER_FIRST_PARAM;
        i++;
    }

    return result;
}

/**
 * @brief Parses a line and extracts the parameters using the specified delimiters.
 *
 * @param line_params Pointer to the array of Line_params structs.
 * @param line_params_count Pointer to the count of Line_params structs.
 * @param buffer The line to parse.
 * @param delimiters The delimiters to use for parsing.
 * @return Returns 0 for success.
 */
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
            for (i = 0; i < MAX_PARAM_COUNT; i++)
            {
                free((*line_params)[*line_params_count].parsed_params[i]);
            }
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

    /*
        printf("Contents of parsed_params:\n ");
        for (i = 0; i < (*line_params)[*line_params_count - 1].param_count; i++)
        {
            printf("Line #%lu -> parsed_params[%d]: %s\n", *line_params_count - 1, i, (*line_params)[*line_params_count - 1].parsed_params[i]);
        }
    */
    return 0; /* Indicate success */
}

/**
 * @brief Extracts a substring from a buffer.
 *
 * @param buffer The buffer to extract the substring from.
 * @param start The starting index of the substring.
 * @param end The ending index of the substring.
 * @return Returns the extracted substring.
 */
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

/**
 * @brief Checks if a parameter is a number.
 *
 * @param s The parameter to check.
 * @param result Pointer to store the converted number if it is a number.
 * @return Returns 1 if the parameter is a number, 0 otherwise.
 */
int is_number(const char *s, int *result)
{
    char *end;
    long val;
    if (s == NULL)
        return 0;

    errno = 0;
    val = strtol(s, &end, 10);
    if (errno == ERANGE || val > INT_MAX || val < INT_MIN)
        return 0;

    if (end == s)
        return 0;

    if (*end != '\0')
        return 0;

    *result = (int)val;
    return 1;
}
