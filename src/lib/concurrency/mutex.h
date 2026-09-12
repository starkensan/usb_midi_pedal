#ifndef LIB_CONCURRENCY_MUTEX_H
#define LIB_CONCURRENCY_MUTEX_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "FreeRTOS.h"
#include "semphr.h"

typedef struct {
    SemaphoreHandle_t handle;
    StaticSemaphore_t mutex_buffer;
} mutex_t;

bool freertos_mutex_init(mutex_t *mutex);
bool mutex_lock(mutex_t *mutex, uint32_t timeout_ms);
bool mutex_unlock(mutex_t *mutex);

#endif
