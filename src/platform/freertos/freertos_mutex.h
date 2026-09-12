#ifndef PLATFORM_FREERTOS_FREERTOS_MUTEX_H
#define PLATFORM_FREERTOS_FREERTOS_MUTEX_H

#include <stdbool.h>

#include "FreeRTOS.h"
#include "semphr.h"

#include "lib/rtos_wrapper/mutex.h"

typedef struct {
    SemaphoreHandle_t handle;
    StaticSemaphore_t mutex_buffer;
    mutex_t mutex;
} freertos_mutex_t;

bool freertos_mutex_init(freertos_mutex_t *mutex);
mutex_t *freertos_mutex_handle(freertos_mutex_t *mutex);

#endif
