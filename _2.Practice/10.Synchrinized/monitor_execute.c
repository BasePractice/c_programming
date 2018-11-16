#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <memory.h>
#include <stdbool.h>
#include <errno.h>
#include <assert.h>
#include <monitor.h>

void increment(void *object) {
    assert(object != 0);
    (*((int *)object))++;
}

int main(int argc, char **argv) {
    struct Monitor monitor;
    int counter = 0;

    monitor_init(&monitor, &counter);
    monitor_enter(&monitor, increment);
    monitor_destroy(&monitor);
    assert(counter == 1);
    return EXIT_SUCCESS;
}
