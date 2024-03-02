#include <stdio.h>
#include <stdlib.h>
#include "memoryUtils.h"

/* convert an integer from any base to another base (up to hex) */
/* use it:  char *result = convertBase(num, fromBase, toBase);*/
char *convertBase(int num, int fromBase, int toBase)
{
    char hexDigits[] = "0123456789ABCDEF";
    int index = 0, i;
    char *finalResult = NULL;
    char *result = (char *)malloc(32 * sizeof(char));
    CHECK_MEMORY_ALLOCATION(result)
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
    CHECK_MEMORY_ALLOCATION(finalResult);

    /* Copy the result string in reverse order to finalResult */
    for (i = 0; i < index; i++)
    {
        finalResult[i] = result[index - i - 1];
    }

    finalResult[index] = '\0';
    free(result);
    return finalResult;
}
