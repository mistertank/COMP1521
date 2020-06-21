// count bits in a uint64_t

#include <assert.h>
#include <stdint.h>
#include <stdlib.h>

// return how many 1 bits value contains
int bit_count(uint64_t value) {
    int count = 0;
    for (int i = 0; i < 64; i++) {
        uint64_t mask = (uint64_t)1 << i;
        uint64_t curr_bit = (value & mask) >> i;
        count += curr_bit;
    }
    return count;
}
