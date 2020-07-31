// Sample solution for COMP1521 lab exercises
//
// generate the opcode for an addi instruction

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>

#include "addi.h"

// return the MIPS opcode for addi $t,$s, i
uint32_t addi(int t, int s, int i) {
    uint32_t result = 0;
    uint32_t tBits, sBits, iBits;
    tBits = (t & 0x1F) << 16;
    result += tBits;
    sBits = (s & 0x1F) << 21;
    result += sBits;
    iBits = ((uint16_t)i) & 0xFFFF;
    result += iBits;
    return result;
}
