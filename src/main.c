/* bconvf: Base converter formatted
 *
 * Convert a number to other base(s) using a printf-like formatting.
 *
 * It supports:
 *     %b (binary)
 *     %o (octal)
 *     %d (decimal)
 *     %h (lowercase hexadecimal)
 *     %H (uppercase hexadecimal)
 *     %10b (binary number aligned to right)
 *     %-10b (binary number aligned to left)
 *     %08b (binary number with at most 8 leading zeros)
 *
 * Usage: bconvf (-b|-o|-d|-h number) format
 *
 * Examples:
 *
 * description: Converts the decimal number 10 to binary (followed by \n)
 * command: bconvf -d 10 "%b\n"
 * output: 1010
 *
 * description: Converts the binary number 11111111 to binary, octal,
 *              decimal and uppercased hexadecimal (followed by \n)
 * command: bconvf -b 11111111 "%b %o %d 0x%H\n"
 * output: 11111111 377 255 0xFF
*/

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <math.h>

#include "number.h"
#include "format_field.h"
#include "format_sequence.h"
#include "base_converter.h"
#include "error_handler.h"

void print_loop(char *format, Number number);

struct option const longOptions[] = {
    {"binary", required_argument, NULL, 'b'},
    {"octal", required_argument, NULL, 'o'},
    {"decimal", required_argument, NULL, 'd'},
    {"hexadecimal", required_argument, NULL, 'h'},
    {0, 0, 0, 0}
};

int main(int argc, char **argv)
{
    Number number;
    char *format;

    char option;
    const char *options = ":b:o:d:h:";
    size_t numbersGiven = 0;

    while((option = getopt_long(argc, argv, options, longOptions, NULL)) != -1)
    {
        switch(option)
        {
            case 'b':
                number.value = optarg;
                number.base = 2;
                numbersGiven++;
                break;

            case 'o':
                number.value = optarg;
                number.base = 8;
                numbersGiven++;
                break;

            case 'd':
                number.value = optarg;
                number.base = 10;
                numbersGiven++;
                break;

            case 'h':
                number.value = optarg;
                number.base = 16;
                numbersGiven++;
                break;

            case '?':
                printf("Invalid option: \'%c\'\n", optopt);
                return EXIT_FAILURE;
                break;

            case ':':
                printf("The option \'%c\' requires an argument.\n", optopt);
                return EXIT_FAILURE;
                break;
        }
    }

    exit_if_missing_format_argument(argc, optind);
    exit_if_numbers_given_is_different_than_one(numbersGiven);
    exit_if_invalid_number(number);
    exit_if_number_too_long(number);

    format = argv[optind];
    
    print_loop(format, number);

    return EXIT_SUCCESS;
}

void print_loop(char *format, Number number)
{
    void handle_field(Number number, Field field);

    const size_t formatSize = strlen(format);
    
    for(size_t i = 0; i < formatSize; i++)
    {
        if(format[i] == '%')
        {
            char *possibleFieldStr = get_field_string(format, formatSize, &i);

            if(is_valid_field_string(possibleFieldStr))
            {
                Field field = get_field(possibleFieldStr);
                handle_field(number, field);
            }
            else
                fputs(possibleFieldStr, stdout);

            // get_field_string() lets the i in the next character to be
            // processed, so I decrement it because of the i++ in for
            i--;
        }
        else if(format[i] == '\\')
            putchar(get_sequence(format, formatSize, &i));
        else
            putchar(format[i]);
    }
}

void handle_field(Number number, Field field)
{
    char *convert_number(Number number, Field field);
    void add_spaces_left_if_needed(char **str, int times);
    void add_spaces_right_if_needed(char **str, int times);
    void add_leading_zeros_if_needed(char **str, int times);

    char *outputNumber = convert_number(number, field);

    add_spaces_left_if_needed(&outputNumber, field.justifyNumber);
    add_spaces_right_if_needed(&outputNumber, field.justifyNumber);
    add_leading_zeros_if_needed(&outputNumber, field.leadingZeros);

    fputs(outputNumber, stdout);
}

char *convert_number(Number number, Field field)
{
    char *number_to_base(Number number, uint8_t base);

    if(field.operation == 'H')
        return str_upper(number_to_base(number, field.base));

    return number_to_base(number, field.base);
}

void add_spaces_left_if_needed(char **str, int times)
{
    // Less than zero because it's left align (eg. %-8b (-8))
    if(times < 0)
    {
        int numberOfLeftSpacesToAdd = abs(times) - strlen(*str);

        if(numberOfLeftSpacesToAdd > 0)
        {
            char *leftSpacesToAdd = str_repeat_char(
                ' ',
                numberOfLeftSpacesToAdd
            );

            *str = str_cat_at_begin(*str, leftSpacesToAdd);
        }
    }
}

void add_spaces_right_if_needed(char **str, int times)
{
    if(times > 0)
    {
        int numberOfRightSpacesToAdd = times - strlen(*str);

        if(numberOfRightSpacesToAdd > 0)
        {
            char *rightSpacesToAdd = str_repeat_char(
                ' ',
                numberOfRightSpacesToAdd
            );

            *str = str_cat(*str, rightSpacesToAdd);
        }
    }
}

void add_leading_zeros_if_needed(char **str, int times)
{
    if(times > 0)
    {
        int numberOfLeadingZerosToAdd = times - strlen(*str);

        if(numberOfLeadingZerosToAdd > 0)
        {
            char *leadingZerosToAdd = str_repeat_char(
                '0', 
                numberOfLeadingZerosToAdd
            );

            *str = str_cat_at_begin(*str, leadingZerosToAdd);
        }
    }
}

char *number_to_base(Number number, uint8_t base)
{
    if(number.base == base)
        return number.value;

    const size_t numberLength = strlen(number.value);
    uintmax_t decimalNumber = 0; 
    size_t i, j;

    for(i = 0, j = numberLength-1; i < numberLength; i++, j--)
    {
        size_t currentNumber = char_to_int(number.value[j]);

        decimalNumber += currentNumber * pow(number.base, i);
    }

    char *decimalNumberStr = int_to_str(decimalNumber);
    decimalNumberStr = str_rev(decimalNumberStr);

    if(base == 10)
        return decimalNumberStr;
    else
        return decimal_to_base(decimalNumberStr, base);
}
