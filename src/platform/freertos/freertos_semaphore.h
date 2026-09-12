#ifndef PLATFORM_FREERTOS_FREERTOS_SEMAPHORE_H
#define PLATFORM_FREERTOS_FREERTOS_SEMAPHORE_H

#include <stdbool.h>
#include <stddef.h>

#include "FreeRTOS.h"
#include "semphr.h"

#include "lib/rtos_wrapper/semaphore.h"

typedef struct {
    SemaphoreHandle_t handle;
    StaticSemaphore_t semaphore_buffer;
    semaphore_t semaphore;
} freertos_semaphore_t;

bool freertos_semaphore_init(freertos_semaphore_t *semaphore,
                             size_t maximum_count,
                             size_t initial_count);
semaphore_t *freertos_semaphore_handle(freertos_semaphore_t *semaphore);

#endif
