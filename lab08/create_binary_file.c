// create_binary_file.c

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "usage: %s FILENAME BYTES\n", argv[0]);
        return EXIT_FAILURE;
    }

    char *filename = argv[1];

    FILE *out = fopen(filename, "w");
    if (out == NULL) {
        perror(filename);
        return EXIT_FAILURE;
    }

    for (int i = 2; i < argc; i++) {
        fputc(atoi(argv[i]), out);
    }

    return EXIT_SUCCESS;
}
