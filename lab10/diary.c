// diary.c
//
// By Soorria S Saruva

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char *getDiaryPath();

int main(int argc, char *argv[]) {
    if (argc <= 1) {
        fprintf(stderr, "usage: %s MESSAGE\n", argv[0]);
        return 1;
    }
    char *diaryPath = getDiaryPath();

    FILE *diary = fopen(diaryPath, "a");
    if (diary == NULL) {
        perror(diaryPath);
        return 1;
    }

    for (int i = 1; i < argc; i++) {
        fprintf(diary, "%s", argv[i]);
        fputc((i < argc - 1) ? ' ' : '\n', diary);
    }

    fclose(diary);
    free(diaryPath);
    return 0;
}

static char *getDiaryPath() {
    char *homePath = getenv("HOME");
    char *baseName = "/.diary";

    int resultLength = strlen(homePath) + strlen(baseName) + 1;

    char *fullPath = malloc(sizeof(char) * resultLength);
    sprintf(fullPath, "%s%s", homePath, baseName);
    return fullPath;
}
