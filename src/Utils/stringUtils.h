#ifndef __STRING_UTILS__
#define __STRING_UTILS__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../global_constants.h"
#include "../datamodel.h"

int read_line(FILE *file, char **buffer);
void clean_white_space(char **buffer);
int string_length_without_white_spaces(const char *s);
int word_check(const char *list[], int length, char *word);
int parse_command(char *buffer, char *command, char *first_param);
int parse_line(Line_params **line_params, size_t *line_params_count, const char *buffer, char *delimitors);
char *mid(char *buffer, int start, int end);
int is_number(const char *s, int *result);
#endif
