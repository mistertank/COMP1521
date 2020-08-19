// calls `final_q2' with each of the command-line arguments.
// see the paper and `final_q2.c' for a description of the question.
// do not change this file.

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int final_q2(uint32_t value);

int main(int argc, char *argv[]) {
    for (int arg = 1; arg < argc; arg++) {
        uint32_t w = strtol(argv[arg], NULL, 0);

        printf("final_q2(0x%08x) returned %d\n", w, final_q2(w));
    }

    return 0;
}
