// smips.c
//
// Emulator for a subset of MIPS

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

////////////////////////////////////////////////////////////////////////
// Type Definitions

typedef enum instructionId {
    ADD,
    SUB,
    AND,
    OR,
    SLT,
    MUL,
    BEQ,
    BNE,
    ADDI,
    SLTI,
    ANDI,
    ORI,
    LUI,
    SYSCALL,
    UNKNOWN_INSTRUCTION = -1
} InstructionId;

#define MIN_INSTRUCTION ((InstructionId) ADD)
#define MAX_INSTRUCTION ((InstructionId) SYSCALL)

typedef uint16_t Immediate;
typedef uint8_t  Register;

#define MIN_REGISTER ((Register) 0)
#define MAX_REGISTER ((Register) 31)

typedef struct instruction{
    InstructionId   id;
    Register        r, s, t, d;
    Immediate       imm;
} Instruction;

////////////////////////////////////////////////////////////////////////
// Helper Function Declarations

////////////////////////////////////////////////////////////////////////
// Main Functions

int main(int argc, char *argv[]) {
    // Check number of arguments
    if (argc != 2) {
        fprintf(stderr, "usage: %s FILENAME\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Open file
    char *filename = argv[1];
    FILE *in = fopen(filename, "r");
    if (in == NULL) {
        perror(filename);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
