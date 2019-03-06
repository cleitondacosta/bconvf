#ifndef _BCONVF_NUMBER_H
#define _BCONVF_NUMBER_H

#include <stdint.h>

struct Number {
    // Since some bases can't be represented by integers, I use char*
    char *value;
    uint8_t base;
};

typedef struct Number Number;

#endif // _BCONVF_NUMBER_H
