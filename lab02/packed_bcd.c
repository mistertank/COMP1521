#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>

#define N_BCD_DIGITS 8

uint32_t packed_bcd(uint32_t packed_bcd);

int main(int argc, char *argv[]) {

    for (int arg = 1; arg < argc; arg++) {
        long l = strtol(argv[arg], NULL, 0);
        assert(l >= 0 && l <= UINT32_MAX);
        uint32_t packed_bcd_value = l;

        printf("%lu\n", (unsigned long)packed_bcd(packed_bcd_value));
    }

    return 0;
}

// given a packed BCD encoded value between 0 .. 99999999
// return the corresponding integer
uint32_t packed_bcd(uint32_t packed_bcd_value) {

    int result = 0;
    int mult = 1;

    for (int i = 0; i < N_BCD_DIGITS; i++) {
        uint32_t offset = i * 4;
        uint32_t mask = (uint32_t)15 << offset;
        uint32_t curr_digit = (packed_bcd_value & mask) >> offset;
        result += curr_digit * mult;
        mult *= 10;
    }

    return result;
}
