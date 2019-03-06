#ifndef _BCONVF_BASE_CONVERTER_H
#define _BCONVF_BASE_CONVERTER_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "string_utils.h"

char *decimal_to_base(char *numberStr, uint8_t base)
{
    long long number = atoll(numberStr);

    if(number == 0)
        return "0";

    if(base == 10)
        return numberStr;

    size_t i;
    char *outputNumber = malloc(sizeof(char));
    outputNumber[0] = '\0';

    for(i = 0; number != 0; i++, number /= base)
    {
        uint8_t res = number % base;

        outputNumber = realloc(
            outputNumber,
            sizeof(char) * (strlen(outputNumber)+1)
        );
        outputNumber[i] = ((res > 9) ? res-10 + 'a' : res + '0');
    }

    outputNumber[i] = '\0';

    return str_rev(outputNumber);
}

#endif // _BCONVF_BASE_CONVERTER_H
