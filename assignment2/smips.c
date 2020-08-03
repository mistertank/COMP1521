// smips.c
//
// Emulator for a subset of MIPS
//
// By Soorriamoorthy Saruva (z5258068)

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
typedef uint8_t RegisterId;

#define NUM_REGISTERS 32
#define ZERO_REGISTER ((RegisterId) 0)

#define SYSCALL_REQUEST_REGISTER ((RegisterId) 2)
#define SYSCALL_ARG_REGISTER ((RegisterId) 4)

typedef enum registerName {
    REGISTER_S,
    REGISTER_T,
    REGISTER_D,
} RegisterName;

////////////////////////
// Instructions
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

typedef struct instruction {
    InstructionId   id;
    RegisterId      s, t, d;
    Immediate       imm;
} Instruction;

#define MAX_NUM_INSTRUCTIONS 1000

///////////
// Syscalls
#define SYSCALL_PRINT_INT  1
#define SYSCALL_EXIT       10
#define SYSCALL_PRINT_CHAR 11

////////////////////////////////////////////////////////////////////////
// Helper Function Declarations

////////////////////////
// Decoding Instructions
static Instruction decodeInstruction(uint32_t bits);
static InstructionId bitsToInstructionId(uint32_t bits);
static RegisterId bitsToRegisterId(uint32_t bits, RegisterName r);
static Immediate bitsToImmediate(uint32_t bits);

///////////////////////
// Running Instructions
static void runInstruction(int registers[NUM_REGISTERS], Instruction i, int *PC);
static void doSyscall(int registers[NUM_REGISTERS], int *PC);

////////////////////////
// Printing Instructions
static void printInstruction(Instruction i, int instructionNum);

////////////////////////////////////////////////////////////////////////
// Main Function

int main(int argc, char *argv[]) {
    // Check number of arguments
    if (argc != 2) {
        fprintf(stderr, "usage: %s [FILENAME]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Open file
    char *filename = argv[1];
    FILE *in = fopen(filename, "r");
    if (in == NULL) {
        perror(filename);
        exit(EXIT_FAILURE);
    }

    // Create array of instruciton
    Instruction instructions[MAX_NUM_INSTRUCTIONS];
    // Initialise registers to 0
    int32_t registers[NUM_REGISTERS] = {0};

    // Decode the instructions
    int nInstructions = 0;
    uint32_t instructionBits = 0;
    while (fscanf(in, "%x", &instructionBits) != EOF) {
        Instruction new = decodeInstruction(instructionBits);

        // Exit if the instruction is invalid
        if (new.id == INVALID_INSTRUCTION) {
            fprintf(
                stderr, "%s:%d: invalid instruction code: %08x\n",
                filename, nInstructions + 1, instructionBits
            );
            exit(EXIT_FAILURE);
        }

        instructions[nInstructions] = new;
        nInstructions++;
    }
    fclose(in);

    // Print out the instructions
    printf("Program\n");
    for (int i = 0; i < nInstructions; i++) {
        printInstruction(instructions[i], i);
    }

    // Run the instructions
    printf("Output\n");
    int PC = 0;
    while (PC < nInstructions) {
        runInstruction(registers, instructions[PC], &PC);
        PC++;
    }

    // Print out non-zero registers
    printf("Registers After Execution\n");
    for (int i = 0; i < NUM_REGISTERS; i++) {
        if (registers[i] != 0) {
            printf("$%-2d = %d\n", i, registers[i]);
        }
    }

    return EXIT_SUCCESS;
}

////////////////////////////////////////////////////////////////////////
// Helper Function Implementations

////////////////////////
// Decoding Instructions

// Decodes the bits of the instruction and returns the information in
// the instruction.
static Instruction decodeInstruction(uint32_t bits) {
    Instruction new = {
        .id  = bitsToInstructionId(bits),
        .s   = bitsToRegisterId(bits, REGISTER_S),
        .t   = bitsToRegisterId(bits, REGISTER_T),
        .d   = bitsToRegisterId(bits, REGISTER_D),
        .imm = bitsToImmediate(bits),
    };
    return new;
}

// Extract the instruction from the instruction's bits.
static InstructionId bitsToInstructionId(uint32_t bits) {
    // SYSCALL
    if (bits == 0xC) return SYSCALL;

    // Get the parts that are different between instructions
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

// Extract the specified register from the instruction's bits.
static RegisterId bitsToRegisterId(uint32_t bits, RegisterName r) {
    int offset = 0;
    switch (r) {
        case REGISTER_S:
            offset = 21;
            break;
        case REGISTER_T:
            offset = 16;
            break;
        case REGISTER_D:
            offset = 11;
            break;
    }
    uint32_t mask = 0x1F << offset;
    return (bits & mask) >> offset;
}

// Extract the immediate from the instruction's bits.
static Immediate bitsToImmediate(uint32_t bits) {
    // Get the last 16 bits
    return (Immediate)(bits & 0xFFFF);
}

///////////////////////
// Running Instructions

// Runs the given instruction, modifying the registers and program
// counter as required.
static void runInstruction(int registers[NUM_REGISTERS], Instruction i, int *PC) {
    // Do nothing if the register being set is register $0
    if (
        (i.id >= ADD && i.id <= MUL && i.d == ZERO_REGISTER) ||
        (i.id >= ADDI && i.id <= LUI && i.t == ZERO_REGISTER)
    ) return;

    switch (i.id) {
        case ADD:
            registers[i.d] = registers[i.s] + registers[i.t];
            break;
        case SUB:
            registers[i.d] = registers[i.s] - registers[i.t];
            break;
        case AND:
            registers[i.d] = registers[i.s] & registers[i.t];
            break;
        case OR:
            registers[i.d] = registers[i.s] | registers[i.t];
            break;
        case SLT:
            registers[i.d] = registers[i.s] < registers[i.t];
            break;
        case MUL:
            registers[i.d] = registers[i.s] * registers[i.t];
            break;
        case BEQ:
            if (registers[i.s] == registers[i.t]) *PC += i.imm - 1;
            break;
        case BNE:
            if (registers[i.s] != registers[i.t]) *PC += i.imm - 1;
            break;
        case ADDI:
            registers[i.t] = registers[i.s] + i.imm;
            break;
        case SLTI:
            registers[i.t] = registers[i.s] < i.imm;
            break;
        case ANDI:
            registers[i.t] = registers[i.s] & i.imm;
            break;
        case ORI:
            registers[i.t] = registers[i.s] | i.imm;
            break;
        case LUI:
            registers[i.t] = i.imm << 16;
            break;
        case SYSCALL:
            doSyscall(registers, PC);
            break;
        default:
            break;
    }
}

// Handle System call
static void doSyscall(int registers[NUM_REGISTERS], int *PC) {
    // Get syscall request and argument from registers
    int request = registers[SYSCALL_REQUEST_REGISTER];
    int arg = registers[SYSCALL_ARG_REGISTER];

    if (request == SYSCALL_PRINT_INT) {
        printf("%d", arg);
    } else if (request == SYSCALL_PRINT_CHAR) {
        // print out low byte
        putchar(arg & 0xFF);
    } else {
        if (request != SYSCALL_EXIT) {
            printf("Unknown system call: %d\n", request);
        }

        // Set program counter past number of instructions so no more
        // instructions are run.
        *PC = MAX_NUM_INSTRUCTIONS;
    }
}

////////////////////////
// Printing Instructions

// Print out the instruction in the required format.
static void printInstruction(Instruction i, int instructionNum) {
    // Print instruction number
    printf("%3d: ", instructionNum);

    // Print MIPS Instruction
    switch (i.id) {
        case  ADD: printf("add  "); break;
        case  SUB: printf("sub  "); break;
        case  AND: printf("and  "); break;
        case   OR: printf("or   "); break;
        case  SLT: printf("slt  "); break;
        case  MUL: printf("mul  "); break;
        case  BEQ: printf("beq  "); break;
        case  BNE: printf("bne  "); break;
        case ADDI: printf("addi "); break;
        case SLTI: printf("alti "); break;
        case ANDI: printf("andi "); break;
        case  ORI: printf("ori  "); break;
        case  LUI: printf("lui  "); break;
        case SYSCALL: printf("syscall"); break;
        case INVALID_INSTRUCTION: exit(EXIT_FAILURE);
    }

    // Print registers / immediate
    switch (i.id) {
        case ADD:   // $d, $s, $t
        case SUB:
        case AND:
        case OR:
        case SLT:
        case MUL:
            printf("$%d, $%d, $%d", i.d, i.s, i.t);
            break;

        case BEQ:   // $s, $t, Imm
        case BNE:
            printf("$%d, $%d, %d", i.s, i.t, i.imm);
            break;

        case ADDI:  // $t, $s, Imm
        case SLTI:
        case ANDI:
        case ORI:
            printf("$%d, $%d, %d", i.t, i.s, i.imm);
            break;

        case LUI:   // $t, Imm
            printf("$%d, %d", i.t, i.imm);
            break;

        // No register and/or immediate printed
        case SYSCALL:
            break;
        case INVALID_INSTRUCTION:
            break;
        // default:
        //     break;
    }

    putchar('\n');
}

////////////////////////////////////////////////////////////////////////

