#ifndef __LANGUAGE__
#define __LANGUAGE__

#define BITS_IN_WORD 14

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
        {0, "mov", 2, 0xF, 0xE},      \
            {1, "cmp", 2, 0xF, 0xF},  \
            {2, "add", 2, 0xF, 0xE},  \
            {3, "sub", 2, 0xF, 0xE},  \
            {4, "not", 1, 0x0, 0xE},  \
            {5, "clr", 1, 0x0, 0xE},  \
            {6, "lea", 2, 0x6, 0xE},  \
            {7, "inc", 1, 0x0, 0xE},  \
            {8, "dec", 1, 0x0, 0xE},  \
            {9, "jmp", 1, 0x0, 0xA},  \
            {10, "bne", 1, 0x0, 0xA}, \
            {11, "red", 1, 0x0, 0xE}, \
            {12, "prn", 1, 0x0, 0xF}, \
            {13, "jsr", 1, 0x0, 0xA}, \
            {14, "rts", 0, 0x0, 0x0}, \
            {15, "hlt", 0, 0x0, 0x0}, \
    }

#define RESIGTERS                                      \
    {                                                  \
        "r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7" \
    }
#endif
