// final_q5.c
//
// By Soorriamoorthy Saruva (z5258068)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    char *envVar1 = getenv(argv[1]);
    char *envVar2 = getenv(argv[2]);
    printf(
        "%d\n",
        (envVar1 != NULL && envVar2 != NULL && strcmp(envVar1, envVar2) == 0)
    );
    return EXIT_SUCCESS;
}
