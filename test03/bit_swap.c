// swap pairs of bits of a 64-bit value, using bitwise operators

#include <assert.h>
#include <stdint.h>
#include <stdlib.h>

// return value with pairs of bits swapped
uint64_t bit_swap(uint64_t value) {
    uint64_t result = 0;
    for (int i = 0; i < 32; i++) {
        uint64_t bit1_mask = (uint64_t)1 << (2 * i);
        uint64_t bit1 = (value & bit1_mask) >> (2 * i);
        uint64_t bit2_mask = (uint64_t)1 << (2 * i + 1);
        uint64_t bit2 = (value & bit2_mask) >> (2 * i + 1);
        result += (bit1 << (2 * i + 1)) + (bit2 << (2 * i));
    }
    return result;
}
