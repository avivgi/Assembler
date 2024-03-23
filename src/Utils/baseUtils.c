#include <stdio.h>
#include <stdlib.h>
#include "baseUtils.h"
#include "memoryUtils.h"
#include "printUtils.h"
#include "../global_constants.h"
/* convert an integer from any base to another base (up to hex) */
/* use it:  char *result = convertBase(num, fromBase, toBase);*/
char *convertBase(int num, int fromBase, int toBase)
{
    char hexDigits[] = "0123456789ABCDEF";
    int index = 0, i;
    char *finalResult = NULL;
    char *result = (char *)malloc(32 * sizeof(char));
    if (result == NULL)
        EXIT_ON_MEM_ALLOC_FAIL

    if (num < 0)
    {
        result[index++] = '-';
        num = -num;
    }

    if (num == 0)
    {
        result[index++] = '0';
    }

    while (num != 0 && index < 32)
    {
        result[index++] = hexDigits[num % toBase];
        num /= toBase;
    }

    finalResult = (char *)malloc((index + 1) * sizeof(char));
    if (!finalResult)
        EXIT_ON_MEM_ALLOC_FAIL

    /* Copy the result string in reverse order to finalResult */
    for (i = 0; i < index; i++)
    {
        finalResult[i] = result[index - i - 1];
    }

    finalResult[index] = '\0';
    free(result);
    return finalResult;
}

int int_to_word(Word *word, int num)
{
    int i;

    if (num > 2048 || num < -2048)
    {
        printf("Number is too large unable to convert.\n");
        return -1;
    }
    /* If num is negative, convert it to its binary representation using two's complement */
    if (num < 0)
    {
        num = -num;
        num = ~num + 1;
    }

    /* Convert num to binary and store it in the array word */
    for (i = 13; i >= 0; i--)
    {
        (*word)[i] = num & 1;
        num >>= 1;
    }
    return 0;
}
