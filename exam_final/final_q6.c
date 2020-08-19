// final_q6.c
//
// By Soorriamoorthy Saruva (z5258068)

#include <stdio.h>

int isascii(int c) {
    return c >= 0 && c <= 127;
}

int main(int argc, char *argv[]) {
    char *filename = argv[1];
    FILE *in = fopen(filename, "r");
    int asciiCount = 0;

    int c;
    while ((c = fgetc(in)) != EOF) {
        asciiCount += isascii(c);
    }

    printf("%s contains %d ASCII bytes\n", filename, asciiCount);

    return 0;
}
