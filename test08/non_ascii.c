// non_ascii.c
//
// By Soorria Saruva (z5258068)

#include <stdio.h>

int isnotascii(int c) {
    return c >= 128 && c <= 255;
}

int main(int argc, char *argv[]) {
    FILE *in = fopen(argv[1], "r");
    int pos = 0;
    int c;

    while ((c = fgetc(in)) != EOF) {
        if (isnotascii(c)) {
            printf("%s: byte %d is non-ASCII\n", argv[1], pos);
            return 0;
        }
        pos++;
    }
    printf("%s is all ASCII\n", argv[1]);

    return 0;
}
