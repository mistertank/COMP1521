#include "bit_rotate.h"

// return the value bits rotated left n_rotations
uint16_t bit_rotate(int n_rotations, uint16_t bits) {
    if (n_rotations >= 0) {
        n_rotations %= 16;
        return (bits << n_rotations) + (bits >> (16 - n_rotations));
    }

    n_rotations = (-n_rotations) % 16;
    uint16_t mask = 0xFFFFu >> (16 - n_rotations);
    uint16_t first_n_bits = (bits & mask) << (16 - n_rotations);
    return (bits >> n_rotations) + first_n_bits;
}
