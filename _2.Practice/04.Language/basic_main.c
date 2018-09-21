#include <stdio.h>
#include <stdlib.h>

/*
 * void main(void);
 * int main(void);
 * int main(int argc, char **argv);
 * int main(int argc, char **argv, char **environment);
 */

int main(int argc, char **argv) {
    int i;
    for (i = 0; i < argc; ++i) {
        fprintf(stdout, "[%2d] %s\n", i, argv[i]);
    }
    fflush(stdout);
    return EXIT_SUCCESS;
}
