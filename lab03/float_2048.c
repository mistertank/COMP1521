// Multiply a float by 2048 using bit operations only

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>

#include "floats.h"

uint32_t components_to_uint32(float_components_t bits) {
    uint32_t result = 0;
    result += bits.sign << 31;
    result += bits.exponent << 23;
    result += bits.fraction;
    return result;
}

// float_2048 is given the bits of a float f as a uint32_t
// it uses bit operations and + to calculate f * 2048
// and returns the bits of this value as a uint32_t
//
// if the result is too large to be represented as a float +inf or -inf is returned
//
// if f is +0, -0, +inf or -int, or Nan it is returned unchanged
//
// float_2048 assumes f is not a denormal number
//
uint32_t float_2048(uint32_t f) {
    float_components_t bits = float_bits(f);

    printf("%x %x %x %x\n", f, bits.sign, bits.exponent, bits.fraction);

    if (
        is_nan(bits)
        || is_negative_infinity(bits)
        || is_positive_infinity(bits)
        || is_zero(bits)
    ) return f;

    bits.exponent = bits.exponent + 11;

    if (bits.exponent > 0xFF) {
        if (bits.sign == 0) {
            return 0x7f800000; // inf
        } else {
            return 0xff800000; // -inf
        }
    }

    return components_to_uint32(bits);
}

