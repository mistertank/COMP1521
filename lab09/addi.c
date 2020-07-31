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
    uint32_t result = 0x20000000;
    result += (t & 0x1F) << 16;
    result += (s & 0x1F) << 21;
    result += ((uint16_t)i) & 0xFFFF;
    return result;
}
