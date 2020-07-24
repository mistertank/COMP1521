// hidden_strings.c

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
        return 1;
    }

    int buffer[3] = {0};
    int bufferIndex = 0;
    int isPrinting = 0;
    int c;

    while ((c = fgetc(in)) != EOF) {
        if (isprint(c)) {
            if (bufferIndex < 3 && !isPrinting) {
                buffer[bufferIndex] = c;
                bufferIndex++;
            } else {
                for (int i = 0; bufferIndex && i < 3; i++) {
                    putchar(buffer[i]);
                }
                bufferIndex = 0;
                putchar(c);
                isPrinting = 1;
            }
        } else {
            if (isPrinting) putchar('\n');
            isPrinting = 0;
            bufferIndex = 0;
        }
    }
    if (isPrinting) putchar('\n');

    return 0;
}
