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

    fseek(in, -1, SEEK_END);
    int c = fgetc(in);

    int pos = ftell(in);

    if (pos <= 1) {
        printf("%c\n", c);
        return 0;
    }


    if (c == '\n') {
        c = fgetc(in);
    }

    while (c != EOF && c != '\n') {
        // printf("c=%c %x\n", c, c);
        int r = fseek(in, -2, SEEK_CUR);
        if (r == -1) return 1;
        c = fgetc(in);
    }

    c = fgetc(in);
    putchar(c);

    int nPrint = 0;;

    while (c != EOF && c != '\n') {
        putchar(c);
        c = fgetc(in);
        nPrint++;
    }
    if (nPrint) putchar('\n');

    return 0;
}
