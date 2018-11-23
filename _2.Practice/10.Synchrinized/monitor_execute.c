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

pthread_t *
create_thread_count(size_t count,
                    void *(*procedure)(void *), void *parameter) {
    size_t i;
    pthread_t *threads = (pthread_t *) calloc(count, sizeof(pthread_t));
    for (i = 0; i < count; ++i) {
        if (pthread_create(&threads[i], 0, procedure, parameter) == 0) {
            fprintf(stdout,
                    "[%03d]Thread %lu created\n", (int) i, threads[i]);
        } else {
            fprintf(stdout, "[%03d]We catch error %d\n", (int) i, errno);
        }
    }
    return threads;
}

void
destroy_thread_count(pthread_t *threads, size_t count) {
    size_t i;

    for (i = 0; i < count; ++i) {
        void *ret_value = 0;
        pthread_join(threads[i], &ret_value);
    }
    free(threads);
}

void *
simple_thread_procedure(void *parameter) {
    monitor_enter((struct Monitor *)parameter, increment);
    return 0;
}

int main(int argc, char **argv) {
    struct Monitor monitor;
    pthread_t *threads;
    int counter = 0;
    size_t n = 10;

    monitor_init(&monitor, &counter);
    threads = create_thread_count(
            n, simple_thread_procedure, (void *) &monitor);
    destroy_thread_count(threads, n);
    monitor_destroy(&monitor);
    assert(counter == n);
    return EXIT_SUCCESS;
}
