#ifndef _BCONVF_ERROR_HANDLER_H
#define _BCONVF_ERROR_HANDLER_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "regex_search.h"
#include "number.h"

void fatal_error_if_malloc_failed(void *mallocAllocated)
{
    if(mallocAllocated == NULL)
    {
        fprintf(stderr, "Fatal error: Malloc failed.\n");
        exit(EXIT_FAILURE);
    }
}

void exit_if_missing_format_argument(int argc, int optind)
{
    if((argc - optind) != 1)
    {
        printf("Usage: bconvf (-b|-o|-d|-h NUMBER) FORMAT\n");
        exit(EXIT_FAILURE);
    }
}

void exit_if_numbers_given_is_different_from_one(int numbersGiven)
{
    if(numbersGiven != 1)
    {
        fprintf(stderr, "You need to pass one number.\n");
        printf("Usage: bconvf (-b|-o|-d|-h NUMBER) FORMAT\n");
        exit(EXIT_FAILURE);
    }
}


void exit_if_invalid_number(Number number)
{
    bool isValid = false;

    switch(number.base)
    {
        case 2:
            isValid = regex_search(number.value, "^[01]+$");
            break;

        case 8:
            isValid = regex_search(number.value, "^[0-7]+$");
            break;

        case 10: 
            isValid = regex_search(number.value, "^[0-9]+$");
            break;

        case 16: 
            isValid = regex_search(number.value, "^[0-9a-fA-F]+$");
            break;
    }

    if(!isValid)
    {
        fprintf( stderr, "Error: Invalid number: \"%s\"\n", number.value);
        exit(EXIT_FAILURE);
    }
}

void exit_if_number_too_long(Number number)
{
    bool isTooLong = false;

    switch(number.base)
    {
        case 2:
            isTooLong = strlen(number.value) > 53;
            break;

        case 8:
            isTooLong = strlen(number.value) > 17;
            break;

        case 10: 
            isTooLong = strlen(number.value) > 15;
            break;

        case 16: 
            isTooLong = strlen(number.value) > 13;
            break;
    }

    if(isTooLong)
    {
        fprintf(stderr, "Error: number is too long.\n");
        exit(EXIT_FAILURE);
    }
}

#endif // _BCONVF_ERROR_HANDLER_H
