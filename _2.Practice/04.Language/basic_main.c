#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    int i;
    for (i = 0; i < argc; ++i) {
        fprintf(stdout, "[%2d] %s\n", i, argv[i]);
    }
    fflush(stdout);
    return EXIT_SUCCESS;
}
