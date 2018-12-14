#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <memory.h>
#include <stdbool.h>
#include <errno.h>
#include <assert.h>
#include "monitor.h"

void
monitor_init(struct Monitor *monitor, void *object) {
    monitor->object = object;
#if defined(_WIN32)
    InitializeCriticalSectionAndSpinCount(&monitor->synchronize_object, 0x00000400);
#else
    pthread_mutex_init(&monitor->synchronize_object, 0);
#endif
}

void
monitor_destroy(struct Monitor *monitor) {
#if defined(_WIN32)
    DeleteCriticalSection(&monitor->synchronize_object);
#else
    pthread_mutex_destroy(&monitor->synchronize_object);
#endif
    monitor->object = 0;
}

void
monitor_enter(struct Monitor *monitor, FUNCTION_MONITOR_ENTER cb) {
#if defined(_WIN32)
    EnterCriticalSection(&monitor->synchronize_object);
#else
    pthread_mutex_lock(&monitor->synchronize_object);
#endif
    (*cb)(monitor->object);
#if defined(_WIN32)
    LeaveCriticalSection(&monitor->synchronize_object);
#else
    pthread_mutex_unlock(&monitor->synchronize_object);
#endif

}
