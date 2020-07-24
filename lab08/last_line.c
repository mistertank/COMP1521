// last_line.c

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

    fseek(in, -2, SEEK_END);
    int c = fgetc(in);

    while (c != EOF && c != '\n') {
        printf("c=%c %x\n", c, c);
        int r = fseek(in, -2, SEEK_CUR);
        if (r == -1) return 1;
        c = fgetc(in);
    }

    c = fgetc(in);
    printf("c=%c %02x\n", c, c);

    while (c != EOF && c != '\n') {
        putchar(c);
        c = fgetc(in);
    }
    putchar('\n');

    return 0;
}
