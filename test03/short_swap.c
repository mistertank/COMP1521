// Swap bytes of a short

#include <stdint.h>
#include <stdlib.h>
#include <assert.h>

// given uint16_t value return the value with its bytes swapped
uint16_t short_swap(uint16_t value) {
    // PUT YOUR CODE HERE
    uint16_t first_mask = (0xFF << 8);
    uint16_t first_byte = (value & first_mask) >> 8;
    uint16_t second_byte = (value & 0xFF);
    return (second_byte << 8) + first_byte;
}
