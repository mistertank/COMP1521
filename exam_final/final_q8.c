// final_q8.c
//
// By Soorriamoorthy Saruva (z5258068)

#include <stdio.h>
#include <stdint.h>

int isutf8(uint32_t bytes, int numBytes) {
    if (numBytes == 1) {
        uint32_t mask = 1u << 31;
        uint32_t bit = (bytes & mask) >> 31;
        return bit == 0;
    }
    if (numBytes == 2) {
        uint32_t mask1 = 0b111u << 29;
        uint32_t bits1 = (bytes & mask1) >> 29;
        uint32_t mask2 = 0b11u << 22;
        uint32_t bits2 = (bytes & mask2) >> 22;

        return bits1 == 0b110 && bits2 == 0b10;
    }
    if (numBytes == 3) {
        uint32_t mask1 = 0b1111u << 28;
        uint32_t bits1 = (bytes & mask1) >> 28;
        uint32_t mask2 = 0b11u << 22;
        uint32_t bits2 = (bytes & mask2) >> 22;
        uint32_t mask3 = 0b11u << 14;
        uint32_t bits3 = (bytes & mask3) >> 14;

        return bits1 == 0b1110 && bits2 == 0b10 && bits3 == 0b10;
    }
    if (numBytes == 4) {
        uint32_t mask1 = 0b11111u << 27;
        uint32_t bits1 = (bytes & mask1) >> 27;
        uint32_t mask2 = 0b11u << 22;
        uint32_t bits2 = (bytes & mask2) >> 22;
        uint32_t mask3 = 0b11u << 14;
        uint32_t bits3 = (bytes & mask3) >> 14;
        uint32_t mask4 = 0b11u << 6;
        uint32_t bits4 = (bytes & mask4) >> 6;

        return bits1 == 0b11110 && bits2 == 0b10 && bits3 == 0b10 && bits4 == 0b10;
    }
    return 0;
}

int main(int argc, char *argv[]) {
    char *filename = argv[1];
    FILE *in = fopen(filename, "r");
    int utfCount = 0;

    int numBytesInChar = 1;
    int c;
    uint32_t currChar = 0;
    while ((c = fgetc(in)) != EOF) {
        currChar = ((uint32_t)c << ((4 - numBytesInChar) * 8)) | currChar;
        if (isutf8(currChar, numBytesInChar)) {
            utfCount += 1;
            currChar = 0;
            numBytesInChar = 1;
        } else if (numBytesInChar < 4) {
            numBytesInChar++;
        } else {
            printf("%s: invalid UTF-8 after %d valid UTF-8 characters\n", filename, utfCount);
            return 0;
        }
    }

    if (c == EOF && currChar) {
        printf("%s: invalid UTF-8 after %d valid UTF-8 characters\n", filename, utfCount);
        return 0;
    }

    printf("%s: %d UTF-8 characters\n", filename, utfCount);

    return 0;
}
