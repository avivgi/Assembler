#ifndef __GLOBAL_CONSTANTS__
#define __GLOBAL_CONSTANTS__

#define DEBUG true

/*General*/
#define MAX_LINE_LENGTH 81
#define MAX_PARAM_SIZE 31
#define MAX_PARAM_COUNT 40
#define NUM_OF_RESIGTERS 8
#define BITS_IN_ENCRYPTED_WORD 7

/*messages*/
#define LABEL_DATA_WAS_FOUND 1
#define LABEL_CODE_WAS_FOUND 2
/*Error types*/
#define ERR_ILLEGAL_COMMA -1
#define ERR_COMMAND_NOT_FOUND -2
#define ERR_MISSING_ARGUMENT -3
#define ERR_EXTRA_TEXT_NO_COMMA_AFTER_FIRST_PARAM -4
#define ERR_WORD_NOT_FOUND -5
#define ERR_LABEL_OR_NAME_IS_TAKEN -6
#define FIRST_LETTER_IS_NOT_A_LETTER -7
#define ERR_MEMORY_ALLOCATION_ERROR -8
#define ERR_VARIABLE_ISNT_INTEGER -9
#define INFO_NOT_LABAL -10
#define ERR_OPEN_FILE -11
#define INFO_LABEL_BEFORE_EXTERN -12
#define EXTERN_FOUND_AND_ADDED_WITH_ERRORS -13
#define EXTERN_FOUND_AND_ADDED 0
#define NOT_EXTERN -15
#define ERR_NUMBER_OPERANDS_FOR_COMMAND -16
#define ERR_INVALID_ADDRESSING -17
#define ERR_WRONG_TYPE_OF_ADDRESSING -18
#define ERR_MISSING_COMMA -19
#define EER_MISSING_ARRAY_INDEX -20
#define SYNTAX_ERROR -21
#define EER_LABEL_NOT_FOUND -22
#define EER_LABEL_NOT_MDEFINE -23
#define INFO_LABEL_IS_ENTRY -24
#define ERR_NO_ARGUMENTS -25
#define ERR_COMMAND_TOO_LONG -26

#define false 0
#define true 1
typedef int Bool;

#endif
