#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

// given a uint32_t value,
// return 1 iff the least significant (bottom) byte
// is equal to the 2nd least significant byte; and
// return 0 otherwise
int final_q2(uint32_t value) {
    uint32_t lowestByte = value & 0xFF;
    uint32_t secondLowestByte = (value & 0xFF00) >> 8;
    return lowestByte == secondLowestByte;
}
