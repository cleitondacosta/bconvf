#ifndef _BCONVF_STRING_UTILS_H
#define _BCONVF_STRING_UTILS_H

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

char *int_to_str(uintmax_t number)
{
    size_t i;
    char *str = malloc(sizeof(char));
    str[0] = '\0';

    for(i = 0; number != 0; i++, number /= 10)
    {
        str = realloc(str, sizeof(char) + (strlen(str)+1));
        str[i] = (char)(number % 10) + '0';
    }

    str[i] = '\0';

    return str;
}

int char_to_int(char c)
{
    switch(c)
    {
        case '0': return 0;
        case '1': return 1;
        case '2': return 2;
        case '3': return 3;
        case '4': return 4;
        case '5': return 5;
        case '6': return 6;
        case '7': return 7;
        case '8': return 8;
        case '9': return 9;
        case 'a': 
        case 'A': return 10;
        case 'b': 
        case 'B': return 11;
        case 'c': 
        case 'C': return 12;
        case 'd': 
        case 'D': return 13;
        case 'e': 
        case 'E': return 14;
        case 'f': 
        case 'F': return 15;
        default:
            fprintf(stderr, "char_to_int(): \'%c\' isn't a number.\n", c);
            exit(EXIT_FAILURE);
    }
}

char *str_upper(char *str)
{
    const size_t strLen = strlen(str);
    size_t i;
    char *upperCased = malloc(sizeof(char) * (strLen+1));

    for(i = 0; i < strLen; i++)
        upperCased[i] = toupper(str[i]);

    upperCased[i] = '\0';

    return upperCased;
}

char *str_rev(char *str)
{
    size_t stringLen = strlen(str);
    size_t i;
    size_t j = stringLen-1;
    char *reversed = malloc(sizeof(char) * (stringLen+1));

    for(i = 0; i < stringLen; i++, j--)
        reversed[i] = str[j];

    reversed[i] = '\0';
    return reversed;
}

char *str_repeat_char(char letter, unsigned int times)
{
    char *charsRepeated = malloc(sizeof(char) * (times+1));

    if(charsRepeated == NULL)
    {
        fprintf(
            stderr,
            "str_repeat_char(): Fatal error: Couldn't allocate memory.\n"
        );
        exit(EXIT_FAILURE);
    }

    memset(charsRepeated, letter, times);
    charsRepeated[times] = '\0';

    return charsRepeated;
}

// a strcat wrapper to allocate memory
char *str_cat(char *str, char *strToCat)
{
    char *mergedString = malloc(
        sizeof(char) * (strlen(str) + strlen(strToCat) + 1)
    );
    mergedString[0] = '\0';

    strcat(mergedString, str);
    strcat(mergedString, strToCat);

    return mergedString;
}

char *str_cat_at_begin(char *str, char *strToCat)
{
    char *mergedString = malloc(
        sizeof(char) * (strlen(str) + strlen(strToCat) + 1)
    );

    mergedString[0] = '\0';

    strcat(mergedString, strToCat);
    strcat(mergedString, str);

    return mergedString;
}

#endif // _BCONVF_STRING_UTILS_H
