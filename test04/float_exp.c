#include "float_exp.h"

// given the 32 bits of a float return the exponent
uint32_t float_exp(uint32_t f) {
    uint32_t mask = (0xFFu << 23);
    return (f & mask) >> 23;
}
