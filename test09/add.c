#include <stdio.h>
#include <stdint.h>
#include <assert.h>

#include "add.h"

// return the MIPS opcode for add $d, $s, $t
uint32_t add(uint32_t d, uint32_t s, uint32_t t) {
    uint32_t result = 0x20;
    result += (s & 0x1F) << 21;
    result += (t & 0x1F) << 16;
    result += (s & 0x1F) << 11;
    return result;
}
