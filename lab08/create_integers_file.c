// create_integers_file.c

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc < 4) {
        fprintf(stderr, "usage: %s FILENAME MIN MAX\n", argv[0]);
        exit(1);
    }

    char *filename = argv[1];
    int min = atoi(argv[2]);
    int max = atoi(argv[3]);

    FILE *out = fopen(filename, "w");
    if (out == NULL) {
        perror(filename);
        exit(1);
    }

    for (int i = min; i <= max; i++) {
        fprintf(out, "%d\n", i);
    }

    return 0;
}
