#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include "printUtils.h"
#include "../global_constants.h"

/**
 * @brief Prints the symbol table to a file.
 *
 * This function takes an array of Symbol structures, the number of symbols in the array, and the filename.
 * It opens a file with the given filename and writes the symbol table to it.
 * Each symbol's name, value, and type are printed in a formatted manner.
 * The symbol type is converted from an enum value to a string representation.
 *
 * @param Symbols The array of Symbol structures representing the symbol table.
 * @param symbol_count The number of symbols in the array.
 * @param filename The name of the file to write the symbol table to.
 */
void print_symbol_table(Symbol **Symbols, size_t *symbol_count, const char *filename)
{
    FILE *dest;
    char fullFileName[100];
    int i;
    const char *symbolType;

    strcpy(fullFileName, filename);
    strcat(fullFileName, ".symbol");

    dest = fopen(fullFileName, "w");

    for (i = 0; i < *symbol_count; i++)
    {
        fprintf(dest, "Symbol name: %-10s\t", (*Symbols)[i].name);
        fprintf(dest, "Value: %d\t", (*Symbols)[i].value);

        switch ((*Symbols)[i].type)
        {
        case UNDEFINED:
            symbolType = "UNDEFINED";
            break;
        case MDEFINE:
            symbolType = "MDEFINE";
            break;
        case CODE:
            symbolType = "CODE";
            break;
        case DATA:
            symbolType = "DATA";
            break;
        case ENTRY:
            symbolType = "ENTRY";
            break;
        case EXTERN:
            symbolType = "EXTERN";
            break;
        default:
            symbolType = "UNKNOWN";
            break;
        }
        fprintf(dest, "type: %d: %s\n", (*Symbols)[i].type, symbolType);
    }

    fclose(dest);
}

/**
 * @brief Prints the word entry table to a file.
 *
 * This function takes an array of Word_entry structures, the size of the array, the filename, and the table name.
 * It opens a file with the given filename and table name and writes the word entry table to it.
 * Each word entry's address, value, and word are printed in a formatted manner.
 * The word is printed using the print_word() function.
 *
 * @param table The array of Word_entry structures representing the word entry table.
 * @param size The size of the array.
 * @param filename The name of the file to write the word entry table to.
 * @param table_name The name of the table.
 */
void print_word_entry_table(Word_entry *table, size_t size, const char *filename, const char *table_name)
{
    FILE *dest;
    char fullFileName[100];
    int i;

    strcpy(fullFileName, filename);
    strcat(fullFileName, ".");
    strcat(fullFileName, table_name);

    dest = fopen(fullFileName, "w");

    for (i = 0; i < size; i++)
    {
        fprintf(dest, "Address: %d\t", (table)[i].address);
        fprintf(dest, "value: %d\t word: ", (table)[i].dValue);
        print_word(dest, (table)[i].word);
        fprintf(dest, "\n");
    }
    fclose(dest);
}

/**
 * @brief Prints a word to a file.
 *
 * This function takes a file stream and a Word value.
 * It prints the binary representation of the word to the file stream.
 *
 * @param stream The file stream to write the word to.
 * @param word The Word value to print.
 */
void print_word(FILE *stream, Word word)
{
    int i;
    for (i = BITS_IN_WORD - 1; i >= 0; i--)
    {
        fprintf(stream, "%d", (word >> i) & 1);
    }
}

/**
 * @brief Prints the entry or extern table to a file.
 *
 * This function takes a Data_model structure, the filename, and the symbol type (ENTRY or EXTERN).
 * It searches for the first symbol of the given type in the symbol table of the data model.
 * If found, it opens a file with the appropriate extension (".ent" for ENTRY or ".ext" for EXTERN) and writes the table to it.
 * Each symbol's name and value are printed in a formatted manner.
 *
 * @param data_model The Data_model structure containing the symbol table.
 * @param filename The name of the file to write the table to.
 * @param type The symbol type (ENTRY or EXTERN).
 */
void print_entry_and_extern_table(Data_model data_model, const char *filename, enum Symbol_type type)
{
    FILE *dest;
    char fullFileName[100];
    int i, found_at_inx = -1;

    for (i = 0; i < data_model.symbol_count; i++)
    {
        if (data_model.symbols[i].type == type)
        {
            found_at_inx = i;
            break;
        }
    }

    if (found_at_inx == -1)
        return;

    strcpy(fullFileName, filename);
    if (type == ENTRY)
        strcat(fullFileName, ".ent");
    else if (type == EXTERN)
        strcat(fullFileName, ".ext");
    if ((dest = fopen(fullFileName, "w")) == NULL)
    {
        fprintf(stderr, "Error! Failed open file %s\n", fullFileName);
        return;
    }

    for (i = found_at_inx; i < data_model.symbol_count; i++)
    {
        if (data_model.symbols[i].type == type)
            fprintf(dest, "%-10s\t%d\n", data_model.symbols[i].name, data_model.symbols[i].value);
    }

    fclose(dest);
}

/**
 * @brief Prints the instruction table to a file.
 *
 * This function takes a Data_model structure and the filename.
 * It opens a file with the extension ".ob" and writes the instruction table to it.
 * The file header is printed first, containing the number of instructions and data entries.
 * Then, the assembly lines are printed using the print_assembly_line() function.
 *
 * @param data_model The Data_model structure containing the instruction table.
 * @param filename The name of the file to write the instruction table to.
 */
void print_instruction_table(Data_model data_model, const char *filename)
{
    FILE *dest;
    char fullFileName[100];
    strcpy(fullFileName, filename);
    strcat(fullFileName, ".ob");
    if ((dest = fopen(fullFileName, "w")) == NULL)
    {
        fprintf(stderr, "Error! Failed open file %s\n", fullFileName);
        return;
    }
    /* File header*/
    fprintf(dest, "%zu %zu\n", data_model.instruction_count, data_model.data_count);

    print_assembly_line(dest, data_model.instructions_table, data_model.instruction_count);
    print_assembly_line(dest, data_model.data_table, data_model.data_count);

    fclose(dest);
}

/**
 * @brief Prints an assembly line to a file.
 *
 * This function takes a file stream, an array of Word_entry structures, and the size of the array.
 * It prints each word entry's address, word, and encrypted word to the file stream.
 * The word is printed using the print_word() function, and the encrypted word is obtained using the encrypt_word() function.
 *
 * @param dest The file stream to write the assembly line to.
 * @param word_entry The array of Word_entry structures representing the assembly line.
 * @param size The size of the array.
 */
void print_assembly_line(FILE *dest, Word_entry *word_entry, size_t size)
{
    int i;
    for (i = 0; i < size; i++)
    {
        fprintf(dest, "%04d ", word_entry[i].address);
        if (DEBUG)
        {
            print_word(dest, word_entry[i].word);
            fprintf(dest, " %d ", word_entry[i].dValue);
        }
        fprintf(dest, "%s\n", encrypt_word(word_entry[i].word));
    }
}

/**
 * @brief Encrypts a word.
 *
 * This function takes a Word value and converts it to a string representation.
 * Each bit in the word is converted to a specific character:
 * 0 -> '*'
 * 1 -> '#'
 * 2 -> '%'
 * 3 -> '!'
 * The resulting string is dynamically allocated and returned.
 *
 * @param word The Word value to encrypt.
 * @return The encrypted word as a dynamically allocated string.
 */
char *encrypt_word(Word word)
{
    char *result = (char *)malloc(BITS_IN_WORD + 1);
    int i;
    for (i = 0; i < BITS_IN_WORD; i++)
    {
        switch ((word >> (BITS_IN_WORD - 1 - i)) & 1)
        {
        case 0:
            result[i] = '*';
            break;
        case 1:
            result[i] = '#';
            break;
        case 2:
            result[i] = '%';
            break;
        case 3:
            result[i] = '!';
            break;
        default:
            result[i] = ' ';
            break;
        }
    }
    result[BITS_IN_WORD] = '\0';
    return result;
}
