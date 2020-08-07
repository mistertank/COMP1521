// is_diary_public.c
//
// By Soorria Saruva (z5258068)

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>

int main(int argc, char *argv[]) {
    char *homePath = getenv("HOME");
    char *baseName = "/.diary";

    int fullPathLength = strlen(homePath) + strlen(baseName) + 1;
    char *fullPath = malloc(fullPathLength * sizeof(char));
    snprintf(fullPath, fullPathLength, "%s%s", homePath, baseName);

    struct stat s;
    if (stat(fullPath, &s) == -1) {
        printf("0\n");
        free(fullPath);
        return 0;
    }

    if (s.st_mode & S_IROTH) {
        printf("1\n");
    } else {
        printf("0\n");
    }

    free(fullPath);
    return 0;
}
