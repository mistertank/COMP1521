// smips.c
//
// Emulator for a subset of MIPS

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

////////////////////////////////////////////////////////////////////////
// Type Definitions & #defines

////////////
// Immediate
typedef int16_t Immediate;

////////////
// Registers
typedef uint8_t Register;

#define MIN_REGISTER ((Register) 0)
#define MAX_REGISTER ((Register) 31)
#define NUM_REGISTERS 32

typedef enum registerName {
    REGISTER_S,
    REGISTER_T,
    REGISTER_D,
} RegisterName;

////////////////////////
// Instruction Id / Type
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
    INVALID_INSTRUCTION = -1
} InstructionId;

#define MIN_INSTRUCTION ((InstructionId) ADD)
#define MAX_INSTRUCTION ((InstructionId) SYSCALL)

typedef struct instruction {
    InstructionId   id;
    Register        s, t, d;
    Immediate       imm;
} Instruction;

#define LEN_HEX_INSTRUCTION 32

////////////////////////////////////////////////////////////////////////
// Helper Function Declarations

///////////////////////
// Parsing Instructions
static Instruction parseInstruction(uint32_t bits);
static InstructionId bitsToInstructionId(uint32_t bits);
static Register bitsToRegisterNumber(uint32_t bits, RegisterName r);
static Immediate bitsToImmediate(uint32_t bits);

///////////////////////
// Running Instructions
static void runInstruction(int registers[NUM_REGISTERS], Instruction instruction);
static void add(int registers[NUM_REGISTERS], Instruction instruction);
static void sub(int registers[NUM_REGISTERS], Instruction instruction);
static void and(int registers[NUM_REGISTERS], Instruction instruction);
static void or(int registers[NUM_REGISTERS], Instruction instruction);
static void slt(int registers[NUM_REGISTERS], Instruction instruction);
static void mul(int registers[NUM_REGISTERS], Instruction instruction);
static void beq(int registers[NUM_REGISTERS], Instruction instruction);
static void bne(int registers[NUM_REGISTERS], Instruction instruction);
static void addi(int registers[NUM_REGISTERS], Instruction instruction);
static void slti(int registers[NUM_REGISTERS], Instruction instruction);
static void andi(int registers[NUM_REGISTERS], Instruction instruction);
static void ori(int registers[NUM_REGISTERS], Instruction instruction);
static void lui(int registers[NUM_REGISTERS], Instruction instruction);
static void syscall(int registers[NUM_REGISTERS]);

////////////////////////////////////////////////////////////////////////
// Main Function

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

    // Get Number of Instructions by reading the number of newline
    // characters
    int numInstructions = 0;
    for (int c = fgetc(in); c != EOF; c = fgetc(in)) {
        if (c == '\n') numInstructions++;
    }
    fseek(in, 0, SEEK_SET);

    // Create array of instructions
    Instruction *instructions = malloc(sizeof(Instruction) * numInstructions);
    if (instructions == NULL) {
        fprintf(stderr, "Could not allocate instructions array\n");
        return EXIT_FAILURE;
    }

    // Parse the instructions
    int currInstructionIndex = 0;
    uint32_t instructionBits = 0;
    while (fscanf(in, "%x", &instructionBits) != EOF) {
        Instruction new = parseInstruction(instructionBits);
        instructions[currInstructionIndex] = new;
        currInstructionIndex++;
        printf("%d %d\n", new.s, new.t);
    }
    fclose(in);

    free(instructions);
    return EXIT_SUCCESS;
}

////////////////////////////////////////////////////////////////////////
// Helper Function Implementations

///////////////////////
// Parsing Instructions

static Instruction parseInstruction(uint32_t bits) {
    return (Instruction){
        .id  = bitsToInstructionId(bits),
        .s   = bitsToRegisterNumber(bits, REGISTER_S),
        .t   = bitsToRegisterNumber(bits, REGISTER_T),
        .d   = bitsToRegisterNumber(bits, REGISTER_D),
        .imm = bitsToImmediate(bits),
    };
}

static InstructionId bitsToInstructionId(uint32_t bits) {
    // SYSCALL
    if (bits == 12) return SYSCALL;

    uint32_t first6Bits = ((0b111111u << 26) & bits) >> 26;
    uint32_t last6Bits = 0b111111 & bits;

    if (first6Bits == 0) {
        switch (last6Bits) {
            case 0b100000: return ADD;
            case 0b100010: return SUB;
            case 0b100100: return AND;
            case 0b100101: return OR;
            case 0b101010: return SLT;
        }
    }

    if (first6Bits == 0b011100 && last6Bits == 0b000010) return MUL;

    switch (first6Bits) {
        case 0b000100: return BEQ;
        case 0b000101: return BNE;
        case 0b001000: return ADDI;
        case 0b001010: return SLTI;
        case 0b001100: return ANDI;
        case 0b001101: return ORI;
        case 0b001111: return LUI;
    }

    return INVALID_INSTRUCTION;
}

static Register bitsToRegisterNumber(uint32_t bits, RegisterName r) {
    int offset = 0;
    switch (r) {
    case REGISTER_S:
        offset = 21;
        break;
    case REGISTER_D:
        offset = 16;
        break;
    case REGISTER_T:
        offset = 11;
        break;
    }
    uint32_t mask = 0b11111 << offset;
    return (bits & mask) >> offset;
}

static Immediate bitsToImmediate(uint32_t bits) {
    return bits & 0xFFFF;
}

///////////////////////
// Running Instructions

////////////////////////////////////////////////////////////////////////
