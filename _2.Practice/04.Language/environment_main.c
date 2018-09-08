#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv, char **environment) {
    char **env;
    for (env = environment; (*env) != 0; env++) {
        fprintf(stdout, "%s\n", (*env));
    }
    fflush(stdout);
    return EXIT_SUCCESS;
}
