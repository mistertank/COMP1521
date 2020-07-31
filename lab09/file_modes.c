// file_modes.c
//
// By Soorriamoorthy Saruva (z5258068)

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>

static void printModes(mode_t m, char *path);

int main(int argc, char *argv[]) {
    for (int i = 1; i < argc; i++) {
        struct stat s;
        stat(argv[i], &s);
        mode_t m = s.st_mode;
        printModes(m, argv[i]);
    }
    return EXIT_SUCCESS;
}

static void printModes(mode_t m, char *path) {
    if (S_ISDIR(m)) {
        putchar('d');
    } else if (S_ISREG(m)) {
        putchar('-');
    }

    putchar((m & S_IRUSR) ? 'r' : '-');
    putchar((m & S_IWUSR) ? 'w' : '-');
    putchar((m & S_IXUSR) ? 'x' : '-');
    putchar((m & S_IRGRP) ? 'r' : '-');
    putchar((m & S_IWGRP) ? 'w' : '-');
    putchar((m & S_IXGRP) ? 'x' : '-');
    putchar((m & S_IROTH) ? 'r' : '-');
    putchar((m & S_IWOTH) ? 'w' : '-');
    putchar((m & S_IXOTH) ? 'x' : '-');

    printf(" %s\n", path);
}
