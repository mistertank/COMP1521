// Compare 2 floats using bit operations only

#include <stdint.h>
#include <stdlib.h>
#include <assert.h>

#include "floats.h"

// float_less is given the bits of 2 floats bits1, bits2 as a uint32_t
// and returns 1 if bits1 < bits2, 0 otherwise
// 0 is return if bits1 or bits2 is Nan
// only bit operations and integer comparisons are used
uint32_t float_less(uint32_t bits1, uint32_t bits2) {
    float_components_t f1 = float_bits(bits1);
    float_components_t f2 = float_bits(bits2);

    if (is_nan(f1) || is_nan(f2)) return 0;
    if (bits1 == bits2) return 0;

    // f1 < f2
    if (is_negative_infinity(f1)) return 1;
    if (is_positive_infinity(f2)) return 1;
    // f1 > f2
    if (is_positive_infinity(f1)) return 0;
    if (is_negative_infinity(f2)) return 0;

    if (is_zero(f1) && is_zero(f2)) return 0;

    // f1 < f1
    if (f1.sign == 1 && f2.sign == 0) return 1;
    // f1 > f2
    if (f1.sign == 0 && f2.sign == 1) return 0;

    if (f1.sign == 0) {         // POSITIVE NUMBERS
        if (f1.exponent < f2.exponent) return 1;
        if (f1.exponent > f2.exponent) return 0;
    } else if (f1.sign == 1) {  // NEGATIVE NUMBERS
        if (f1.exponent < f2.exponent) return 0;
        if (f1.exponent > f2.exponent) return 1;
    }

    return 0;
}
