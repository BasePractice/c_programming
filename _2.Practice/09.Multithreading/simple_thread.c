#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>

#define MAX_THREADS   21

static volatile int thread_id = 0;

void *simple_thread_procedure(void *parameter) {
    int tid = thread_id;
    unsigned int timeout;

    ++thread_id;
    fprintf(stdout,
            "[%03d]Thread %lu started\n", tid, pthread_self());
    timeout = (unsigned int)rand() % 10 + 1;
    usleep(timeout * 1000 * 1000);
    fprintf(stdout,
            "[%03d]Thread %lu complete %d seconds\n", tid, pthread_self(), timeout);
    fflush(stdout);
    return parameter;
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

int main(int argc, char **argv) {
    pthread_t *threads;

    srand((unsigned int)time(0));
    threads = create_thread_count(
            MAX_THREADS, simple_thread_procedure, (void *) 10);
    destroy_thread_count(threads, MAX_THREADS);
    fflush(stdout);
    return EXIT_SUCCESS;
}
