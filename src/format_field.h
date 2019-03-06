#ifndef _BCONVF_FORMAT_FIELD_H
#define _BCONVF_FORMAT_FIELD_H

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#include "regex_search.h"

struct Field
{
    uint8_t operation;
    uint8_t base;
    int leadingZeros;
    int justifyNumber;
};

typedef struct Field Field;

bool is_number(char possibleNumber)
{
    return possibleNumber >= 48 && possibleNumber <= 57;
}

bool is_format_base(char possibleOperation)
{
    switch(possibleOperation)
    {
        case 'b': return true;
        case 'o': return true;
        case 'd': return true;
        case 'h': return true;
        case 'H': return true;
        default: return false;
    }
}

uint8_t get_field_base(char operation)
{
    switch(operation)
    {
        case 'b': return 2;
        case 'o': return 8;
        case 'c': 
        case 'd': return 10;
        case 'h': 
        case 'H': return 16;
    }
}

int get_field_number(char *field, const size_t fieldSize)
{
    char *number = malloc(sizeof(char) * 2);
    size_t fieldIndex = 1;
    size_t numberIndex = 0;

    if(field[fieldIndex] == '-')
    {
        number[numberIndex] = '-';
        number[numberIndex+1] = '\0';
        numberIndex++;
        fieldIndex++;
    }

    while((is_number(field[fieldIndex])) && (fieldIndex < fieldSize))
    {
        number = realloc(number, sizeof(char) * (strlen(number)+1));
        number[numberIndex] = field[fieldIndex];
        numberIndex++;
        fieldIndex++;
    }

    number[numberIndex] = '\0';

    return atoi(number);
}

Field get_field(char *fieldStr)
{
    const size_t fieldStrSize = strlen(fieldStr);

    Field field;
    field.justifyNumber = 0;
    field.leadingZeros = 0;

    if(fieldStr[1] == '0')
        field.leadingZeros = get_field_number(fieldStr, fieldStrSize);
    else if(is_number(fieldStr[1]) || fieldStr[1] == '-')
        field.justifyNumber = get_field_number(fieldStr, fieldStrSize);

    field.operation = fieldStr[fieldStrSize-1];
    field.base = get_field_base(field.operation);
    return field;
}

char 
*get_field_string(char *format, const size_t formatSize, size_t *formatOffset)
{
    bool is_part_of_field(char possiblePartOfField, size_t fieldStringIndex);

    size_t i = 0;
    char *field = malloc(sizeof(char));
    field[0] = '\0';

    for(i = 0; i < formatSize; i++, *formatOffset += 1)
    {
        field = realloc(field, sizeof(char) * (strlen(field)+1));

        if(*formatOffset >= formatSize)
        {
            field[i] = '\0';
            return field;
        }

        if(is_part_of_field(format[*formatOffset], i))
            field[i] = format[*formatOffset];
        else
            break;
    }

    field[i] = '\0';

    return field;
}

bool is_part_of_field(char possiblePartOfField, size_t fieldStringIndex)
{
    if(possiblePartOfField == '%' && fieldStringIndex == 0)
        return true;

    if(possiblePartOfField == '-' && fieldStringIndex == 1)
        return true;

    if(is_format_base(possiblePartOfField) && fieldStringIndex > 0)
        return true;

    if(isdigit(possiblePartOfField) && fieldStringIndex > 0)
        return true;

    return false;
}

bool is_valid_field_string(char *possibleFieldStr)
{
    return regex_search(possibleFieldStr, "^%-?[0-9]*[bodhH]$");
}

void field_debug(Field field)
{
    printf(
        "\n\noperation: %c\nleadingZeros: %d\njustifyNumber: %d\nbase: %d\n",
        field.operation,
        field.leadingZeros,
        field.justifyNumber,
        field.base
    );  
}

#endif // _BCONVF_FORMAT_FIELD_H
