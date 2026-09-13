#include "semaphore.h"

#include "freertos_timeout.h"

error_code_t semaphore_init(semaphore_t *semaphore, size_t maximum_count, size_t initial_count)
{
    if (semaphore == NULL) {
        return ERROR_CODE_INVALID_ARGUMENT;
    }
    if ((maximum_count == 0U) || (initial_count > maximum_count)
        || (maximum_count > (size_t)((UBaseType_t)-1))) {
        return ERROR_CODE_OUT_OF_RANGE;
    }

    semaphore->handle = xSemaphoreCreateCountingStatic(
        (UBaseType_t)maximum_count, (UBaseType_t)initial_count, &semaphore->semaphore_buffer);
    return semaphore->handle != NULL ? ERROR_CODE_OK : ERROR_CODE_NOT_READY;
}

error_code_t semaphore_give(semaphore_t *semaphore)
{
    if (semaphore == NULL) {
        return ERROR_CODE_INVALID_ARGUMENT;
    }
    if (semaphore->handle == NULL) {
        return ERROR_CODE_NOT_READY;
    }

    return xSemaphoreGive(semaphore->handle) == pdPASS ? ERROR_CODE_OK : ERROR_CODE_NOT_READY;
}

error_code_t semaphore_take(semaphore_t *semaphore, uint32_t timeout_ms)
{
    if (semaphore == NULL) {
        return ERROR_CODE_INVALID_ARGUMENT;
    }
    if (semaphore->handle == NULL) {
        return ERROR_CODE_NOT_READY;
    }

    return xSemaphoreTake(semaphore->handle, freertos_timeout_ms_to_ticks(timeout_ms)) == pdPASS
               ? ERROR_CODE_OK
               : ERROR_CODE_TIMEOUT;
}
