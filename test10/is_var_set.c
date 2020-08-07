// is_var_set.c
//
// By Soorria Saruva (z5258068)

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    char *var = argv[1];
    char *value = getenv(var);

    if (value != NULL && value[0] != '\0') {
        printf("1\n");
    } else {
        printf("0\n");
    }

    return 0;
}
