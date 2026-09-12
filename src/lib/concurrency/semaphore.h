#ifndef LIB_CONCURRENCY_SEMAPHORE_H
#define LIB_CONCURRENCY_SEMAPHORE_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct semaphore_operations {
    bool (*give)(void *context);
    bool (*take)(void *context, uint32_t timeout_ms);
} semaphore_operations_t;

typedef struct {
    void *context;
    const semaphore_operations_t *operations;
} semaphore_t;

bool semaphore_give(semaphore_t *semaphore);
bool semaphore_take(semaphore_t *semaphore, uint32_t timeout_ms);

#endif
