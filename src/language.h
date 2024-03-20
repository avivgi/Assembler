#ifndef __LANGUAGE__
#define __LANGUAGE__

#define NUNMER_OF_REGISTERS 8
#define CODE_START_ADDRESS 100

#define COMMAND_LENGTH 3
#define NUM_OF_COMMANDS_IN_LANGUAGE 16

typedef struct
{
    int index;
    char command_name[COMMAND_LENGTH + 1];
    int command_type;
    char allowed_source_operand_adderss_type;
    char allowed_target_operand_adderss_type;
} As_Command;

#define AS_COMMAND_LIST               \
    {                                 \
        {0, "mov", 2, 0xF, 0x7},      \
            {1, "cmp", 2, 0xF, 0xF},  \
            {2, "add", 2, 0xF, 0x7},  \
            {3, "sub", 2, 0xF, 0x7},  \
            {4, "not", 2, 0x0, 0x7},  \
            {5, "clr", 2, 0x0, 0x7},  \
            {6, "lea", 2, 0x3, 0x7},  \
            {7, "inc", 1, 0x0, 0x7},  \
            {8, "dec", 1, 0x0, 0x7},  \
            {9, "jmp", 1, 0x0, 0x5},  \
            {10, "bne", 1, 0x0, 0x5}, \
            {11, "red", 1, 0x0, 0x7}, \
            {12, "prn", 1, 0x0, 0xF}, \
            {13, "jsr", 1, 0x0, 0x5}, \
            {14, "rts", 0, 0x0, 0x0}, \
            {15, "hlt", 0, 0x0, 0x0}, \
    }

enum Sentence_type
{
    EMPTY,
    COMMENT,
    DEFINITOR,
    INSTRUCTOR,
    CONST
};

enum Operand_adderss_type
{
    NA = -1,
    IMM = 0,
    DIRECT = 1,
    INDEX = 2,
    REGISTER = 3
} operand_adderss_type;

/*enum Instructor_type
{
    DATA_INSTRUCTION,
    STRING,
    ENTRY,
    EXTERN
} instructor_type;*/

#define RESIGTERS                                      \
    {                                                  \
        "r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7" \
    }
#endif
