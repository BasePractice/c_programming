#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <memory.h>
#include <stdbool.h>
#include <errno.h>

#if defined(_WIN32)
#  ifndef WIN32_LEAN_AND_MEAN
#  define WIN32_LEAN_AND_MEAN
#  endif

#include <windows.h>
typedef CRITICAL_SECTION Type;
#else

#include <unistd.h>
#include <pthread.h>

typedef pthread_mutex_t Type;
#endif

typedef int Object;

typedef void (*FUNCTION_MONITOR_ENTER)(Object object);

struct Monitor {
    Object object;
    Type synchronize_object;
};

void monitor_init(struct Monitor *monitor, Object object);

void monitor_destroy(struct Monitor *monitor);

void monitor_enter(struct Monitor *monitor, FUNCTION_MONITOR_ENTER cb);

//EnterCriticalSection(&monitor->synchronize_object);
//LeaveCriticalSection(&monitor->synchronize_object);


int main(int argc, char **argv) {
    return EXIT_SUCCESS;
}

void monitor_init(struct Monitor *monitor, Object object) {
    monitor->object = object;
#if defined(_WIN32)
    InitializeCriticalSectionAndSpinCount(&monitor->synchronize_object, 0x00000400);
#else
    pthread_mutex_init(&monitor->synchronize_object, 0);
#endif
}

void monitor_destroy(struct Monitor *monitor) {
#if defined(_WIN32)
    DeleteCriticalSection(&monitor->synchronize_object);
#else
    pthread_mutex_destroy(&monitor->synchronize_object);
#endif
}
