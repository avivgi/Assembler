#ifndef __GLOBAL_CONSTANTS__
#define __GLOBAL_CONSTANTS__

#define MAX_LINE_LENGTH 80
#define MAX_PARAM_SIZE 20
#define MAX_PROGRAM_LENGTH 1000
#define MAX_PARAM_COUNT 40

/*Error types*/
#define ERR_ILLEGAL_COMMA -1
#define ERR_COMMAND_NOT_FOUND -2
#define ERR_MISSING_ARGUMENT -3
#define ERR_EXTRA_TEXT_NO_COMMA_AFTER_FIRST_PARAM -4
#define ERR_WORD_NOT_FOUND -5
#define ERR_LABEL_OR_NAME_IS_TAKEN -6
#define FIRST_LETTER_IS_NOT_A_LETTER -7
#define ERR_MEMORY_ALLOCATION_ERROR -8
#endif
