#ifndef __LANGUAGE__
#define __LANGUAGE__

#define NUM_OF_COMMANDS_IN_LANGUAGE 16

typedef struct
{
    int index;
    char command_name[4];
    int command_type;
    unsigned char allowed_source_operand_adderss_type : 4;
    unsigned char allowed_target_operand_adderss_type : 4;
} As_Command;

As_Command as_command[NUM_OF_COMMANDS_IN_LANGUAGE] = {{0, "mov", 1, 0b1111, 0b0111},
                                                      {1, "cmp", 1, 0b1111, 0b1111},
                                                      {2, "add", 1, 0b1111, 0b0111},
                                                      {3, "sub", 1, 0b1111, 0b0111},
                                                      {4, "not", 1, 0b0000, 0b0111},
                                                      {5, "clr", 1, 0b0000, 0b0111},
                                                      {6, "lea", 1, 0b0011, 0b0111},
                                                      {7, "inc", 2, 0b0000, 0b0111},
                                                      {8, "dec", 2, 0b0000, 0b0111},
                                                      {9, "jmp", 2, 0b0000, 0b0101},
                                                      {10, "bne", 2, 0b0000, 0b0101},
                                                      {11, "red", 2, 0b0000, 0b0111},
                                                      {12, "prn", 2, 0b0000, 0b1111},
                                                      {13, "jsr", 2, 0b0000, 0b0101},
                                                      {14, "rts", 3, 0b0000, 0b0000},
                                                      {15, "htl", 3, 0b0000, 0b0000}};

enum sentance_type
{
    EMPTY,
    COMMENT,
    DEFINITOR,
    INSTRUCTOR,
    CONST
};

enum operand_adderss_type
{
    NA = -1,
    IMM = 0,
    DIRECT = 1,
    INDEX = 2,
    REGISTER = 3,
};

#endif