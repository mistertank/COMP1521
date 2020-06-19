// Extract the 3 parts of a float using bit operations only

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>

#include "floats.h"

// separate out the 3 components of a float
float_components_t float_bits(uint32_t f) {
    float_components_t result;

    uint32_t sign_mask = (uint32_t) 1 << 31;
    result.sign = (f & sign_mask) >> 31;

    uint32_t exponent_mask = (uint32_t) 0xFF << 23;
    result.exponent = (f & exponent_mask) >> 23;

    uint32_t fraction_mask = 0x7FFFFF; // Last 23 bits
    result.fraction = f & fraction_mask;

    return result;
}

// given the 3 components of a float
// return 1 if it is NaN, 0 otherwise
int is_nan(float_components_t f) {
    return f.sign == 0 && f.exponent == 0xFF && f.fraction != 0;
}

// given the 3 components of a float
// return 1 if it is inf, 0 otherwise
int is_positive_infinity(float_components_t f) {
    return f.sign == 0 && f.exponent == 0xFF && f.fraction == 0;
}

// given the 3 components of a float
// return 1 if it is -inf, 0 otherwise
int is_negative_infinity(float_components_t f) {
    return f.sign == 1 && f.exponent == 0xFF && f.fraction == 0;
}

// given the 3 components of a float
// return 1 if it is 0 or -0, 0 otherwise
int is_zero(float_components_t f) {
    return f.exponent == 0 && f.fraction == 0;
}

