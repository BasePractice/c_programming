#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>

static volatile int value = 0;

int main() {
    pid_t pid;
    while ( true ) {
        pid = fork();
        if (pid != 0) {
            fprintf(stdout, "PID %d\n", pid);
            fflush(stdout);
        }
        value++;
    }
    return EXIT_SUCCESS;
}
