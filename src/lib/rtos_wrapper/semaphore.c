#include "semaphore.h"

#include "freertos_timeout.h"

bool semaphore_init(semaphore_t *semaphore, size_t maximum_count, size_t initial_count)
{
    if ((semaphore == NULL) || (maximum_count == 0U) || (initial_count > maximum_count)
        || (maximum_count > (size_t)((UBaseType_t)-1))) {
        return false;
    }

    semaphore->handle = xSemaphoreCreateCountingStatic(
        (UBaseType_t)maximum_count, (UBaseType_t)initial_count, &semaphore->semaphore_buffer);
    return semaphore->handle != NULL;
}

bool semaphore_give(semaphore_t *semaphore)
{
    if ((semaphore == NULL) || (semaphore->handle == NULL)) {
        return false;
    }

    return xSemaphoreGive(semaphore->handle) == pdPASS;
}

bool semaphore_take(semaphore_t *semaphore, uint32_t timeout_ms)
{
    if ((semaphore == NULL) || (semaphore->handle == NULL)) {
        return false;
    }

    return xSemaphoreTake(semaphore->handle, freertos_timeout_ms_to_ticks(timeout_ms)) == pdPASS;
}
