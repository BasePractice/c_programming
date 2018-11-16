#ifndef C_PROGRAMMING_PRACTICE_MONITOR_H
#define C_PROGRAMMING_PRACTICE_MONITOR_H

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

#if defined(__cplusplus)
extern "C" {
#endif

typedef void (*FUNCTION_MONITOR_ENTER)(void *object);

struct Monitor {
    void *object;
    Type synchronize_object;
};

void monitor_init(struct Monitor *monitor, void *object);

void monitor_destroy(struct Monitor *monitor);

void monitor_enter(struct Monitor *monitor, FUNCTION_MONITOR_ENTER cb);


#if defined(__cplusplus)
}
#endif

#endif //C_PROGRAMMING_PRACTICE_MONITOR_H
