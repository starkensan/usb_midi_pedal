#include "mutex.h"

#include "freertos_timeout.h"

error_code_t freertos_mutex_init(mutex_t *mutex)
{
    if (mutex == NULL) {
        return ERROR_CODE_INVALID_ARGUMENT;
    }

    mutex->handle = xSemaphoreCreateMutexStatic(&mutex->mutex_buffer);
    return mutex->handle != NULL ? ERROR_CODE_OK : ERROR_CODE_NOT_READY;
}

error_code_t mutex_lock(mutex_t *mutex, uint32_t timeout_ms)
{
    if (mutex == NULL) {
        return ERROR_CODE_INVALID_ARGUMENT;
    }
    if (mutex->handle == NULL) {
        return ERROR_CODE_NOT_READY;
    }

    return xSemaphoreTake(mutex->handle, freertos_timeout_ms_to_ticks(timeout_ms)) == pdPASS
               ? ERROR_CODE_OK
               : ERROR_CODE_TIMEOUT;
}

error_code_t mutex_unlock(mutex_t *mutex)
{
    if (mutex == NULL) {
        return ERROR_CODE_INVALID_ARGUMENT;
    }
    if (mutex->handle == NULL) {
        return ERROR_CODE_NOT_READY;
    }

    return xSemaphoreGive(mutex->handle) == pdPASS ? ERROR_CODE_OK : ERROR_CODE_NOT_READY;
}
