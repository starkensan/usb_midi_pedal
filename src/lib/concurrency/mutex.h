#ifndef LIB_CONCURRENCY_MUTEX_H
#define LIB_CONCURRENCY_MUTEX_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct mutex_operations {
    bool (*lock)(void *context, uint32_t timeout_ms);
    bool (*unlock)(void *context);
} mutex_operations_t;

typedef struct {
    void *context;
    const mutex_operations_t *operations;
} mutex_t;

bool mutex_lock(mutex_t *mutex, uint32_t timeout_ms);
bool mutex_unlock(mutex_t *mutex);

#endif
