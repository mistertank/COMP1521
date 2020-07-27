// compare_file.c
//
// By Soorria Saruva (z5258068)

#include <stdio.h>
#include <ctype.h>

int main(int argc, char *argv[]) {
    FILE *file1 = fopen(argv[1], "r");
    FILE *file2 = fopen(argv[2], "r");
    int pos = 0;
    int c1 = fgetc(file1);
    int c2 = fgetc(file2);

    while (c1 != EOF && c2 != EOF) {
        if (c1 != c2) {
            printf("Files differ at byte %d\n", pos);
            return 0;
        }
        pos++;
        c1 = fgetc(file1);
        c2 = fgetc(file2);
    }

    if (c1 == c2) {
        printf("Files are identical\n");
    } else if (c1 == EOF) {
        printf("EOF on %s\n", argv[1]);
    } else if (c2 == EOF) {
        printf("EOF on %s\n", argv[2]);
    }

    return 0;
}
