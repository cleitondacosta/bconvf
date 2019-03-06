#ifndef _BCONVF_FORMAT_SEQUENCE_H
#define _BCONVF_FORMAT_SEQUENCE_H

#include <stdbool.h>

bool is_sequence(char possibleSequence)
{
    return possibleSequence == '\\';
}

char get_sequence(const char *format, const size_t formatSize, size_t *offset)
{
    if(*offset+1 >= formatSize)
        return format[*offset];

    *offset += 1;

    switch(format[*offset])
    {
        case 'a': return '\a';
        case 'b': return '\b';
        case 'f': return '\f';
        case 'n': return '\n';
        case 'r': return '\r';
        case 't': return '\t';
        case 'v': return '\v';
        case '\\': return '\\';
        default:
            *offset -= 1;
            return format[*offset];
    }
}

#endif // _BCONVF_FORMAT_SEQUENCE_H
