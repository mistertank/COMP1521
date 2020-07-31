// file_sizes.c
//
// By Soorriamoorthy Saruva (z5258068)

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>

int main(int argc, char *argv[]) {
    long int total = 0;
    for (int i = 1; i < argc; i++) {
        struct stat s;
        stat(argv[i], &s);
        total += (long)s.st_size;
        printf("%s: %ld bytes\n", argv[i], (long)s.st_size);
    }
    printf("Total: %ld bytes\n", total);
    return EXIT_SUCCESS;
}
