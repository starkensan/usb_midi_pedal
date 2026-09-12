#ifndef LIB_CONCURRENCY_SEMAPHORE_H
#define LIB_CONCURRENCY_SEMAPHORE_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "FreeRTOS.h"
#include "semphr.h"

typedef struct {
    SemaphoreHandle_t handle;
    StaticSemaphore_t semaphore_buffer;
} semaphore_t;

bool semaphore_init(semaphore_t *semaphore, size_t maximum_count, size_t initial_count);
bool semaphore_give(semaphore_t *semaphore);
bool semaphore_take(semaphore_t *semaphore, uint32_t timeout_ms);

#endif
