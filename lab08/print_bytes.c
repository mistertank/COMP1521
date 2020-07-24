// print_bytes.c

#include <stdio.h>
#include <ctype.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "usage: %s FILENAME\n", argv[0]);
        return 1;
    }

    char *filename = argv[1];
    FILE *in = fopen(filename, "r");
    if (in == NULL) {
        perror(filename);
    }

    int c;
    int nRead = 0;
    while ((c = fgetc(in)) != EOF) {
        printf("byte %4d: %3d 0x%02x", nRead, c, c);
        if (isprint(c)) {
            printf(" '%c'", c);
        }
        printf("\n");
        nRead++;
    }

    return 0;
}
