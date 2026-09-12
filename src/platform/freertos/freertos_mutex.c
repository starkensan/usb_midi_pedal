#include "platform/freertos/freertos_mutex.h"

#include "platform/freertos/freertos_timeout.h"

static bool freertos_mutex_lock(void *context, uint32_t timeout_ms)
{
    freertos_mutex_t *mutex = context;

    return xSemaphoreTake(mutex->handle, freertos_timeout_ms_to_ticks(timeout_ms)) == pdPASS;
}

static bool freertos_mutex_unlock(void *context)
{
    freertos_mutex_t *mutex = context;

    return xSemaphoreGive(mutex->handle) == pdPASS;
}

static const mutex_operations_t freertos_mutex_operations = {
    .lock = freertos_mutex_lock,
    .unlock = freertos_mutex_unlock,
};

bool freertos_mutex_init(freertos_mutex_t *mutex)
{
    if (mutex == NULL) {
        return false;
    }

    mutex->handle = xSemaphoreCreateMutexStatic(&mutex->mutex_buffer);
    if (mutex->handle == NULL) {
        mutex->mutex.context = NULL;
        mutex->mutex.operations = NULL;
        return false;
    }

    mutex->mutex.context = mutex;
    mutex->mutex.operations = &freertos_mutex_operations;
    return true;
}

mutex_t *freertos_mutex_handle(freertos_mutex_t *mutex)
{
    if ((mutex == NULL) || (mutex->handle == NULL)) {
        return NULL;
    }

    return &mutex->mutex;
}
