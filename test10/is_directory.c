// is_directory.c
//
// By Soorria Saruva (z5258068)

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

int main(int argc, char *argv[]) {
    char *path = argv[1];

    struct stat pathStat;
    if (stat(path, &pathStat) == -1) {
        printf("0\n");
        return 0;
    }

    if (S_ISDIR(pathStat.st_mode)) {
        printf("1\n");
    } else {
        printf("0\n");
    }

    return 0;
}
