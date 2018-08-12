#include <stdio.h>
#include <stdlib.h>
#include "if.h"

int main(int argc, char **argv) {
    char *hello = hello_text();
    fprintf(stdout, "Text: %s\n", hello);
    return EXIT_SUCCESS;
}
