// last_line.c

#include <ctype.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "usage: %s FILENAME\n", argv[0]);
        return 1;
    }

    char *filename = argv[1];
    FILE *in = fopen(filename, "r");
    if (in == NULL) {
        perror(filename);
        return 1;
    }

    fseek(in, -1, SEEK_END);
    int c = fgetc(in);

    int pos = ftell(in);
    // printf("%d\n", pos);

    if (pos <= 1) {
        if (isascii(c)) {
            putchar(c);
        }
        return 0;
    }

    if (c == '\n') {
        fseek(in, -2, SEEK_CUR);
        c = fgetc(in);
    }

    while (c != EOF && c != '\n') {
        // printf("c=%c %x\n", c, c);
        int r = fseek(in, -2, SEEK_CUR);
        if (r == -1) break;
        c = fgetc(in);
    }

    putchar(c);
    c = fgetc(in);

    while (c != EOF) {
        putchar(c);
        c = fgetc(in);
    }

    return 0;
}
