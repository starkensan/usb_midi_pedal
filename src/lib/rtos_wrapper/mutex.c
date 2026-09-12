#include "mutex.h"

#include "freertos_timeout.h"

bool freertos_mutex_init(mutex_t *mutex)
{
    if (mutex == NULL) {
        return false;
    }

    mutex->handle = xSemaphoreCreateMutexStatic(&mutex->mutex_buffer);
    return mutex->handle != NULL;
}

bool mutex_lock(mutex_t *mutex, uint32_t timeout_ms)
{
    if ((mutex == NULL) || (mutex->handle == NULL)) {
        return false;
    }

    return xSemaphoreTake(mutex->handle, freertos_timeout_ms_to_ticks(timeout_ms)) == pdPASS;
}

bool mutex_unlock(mutex_t *mutex)
{
    if ((mutex == NULL) || (mutex->handle == NULL)) {
        return false;
    }

    return xSemaphoreGive(mutex->handle) == pdPASS;
}
