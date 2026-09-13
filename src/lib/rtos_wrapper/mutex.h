#ifndef LIB_CONCURRENCY_MUTEX_H
#define LIB_CONCURRENCY_MUTEX_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "FreeRTOS.h"
#include "error_code.h"
#include "semphr.h"

typedef struct {
    SemaphoreHandle_t handle;
    StaticSemaphore_t mutex_buffer;
} mutex_t;

error_code_t freertos_mutex_init(mutex_t *mutex);
error_code_t mutex_lock(mutex_t *mutex, uint32_t timeout_ms);
error_code_t mutex_unlock(mutex_t *mutex);

#endif
