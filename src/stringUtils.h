#ifndef __STRING_UTILS__
#define __STRING_UTILS__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "global_constants.h"

void read_line(FILE *file, char **buffer);

/*clean white spaces from a buffer*/
void clean_white_space(char **buffer);

/* find if a string exists in a string array*/
int word_check(const char *list[], int length, char *word);

/*splits a given char array into array of strings using a given delimeter */
int split(const char *input, char arr[][MAX_PARAM_SIZE], int arr_length, char split_by);

/* parases a the command and first parameter*/
int parse_command(char *buffer, char *command, char *first_param);
#endif
