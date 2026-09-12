#include "platform/freertos/freertos_semaphore.h"

#include "platform/freertos/freertos_timeout.h"

static bool freertos_semaphore_give(void *context)
{
    freertos_semaphore_t *semaphore = context;

    return xSemaphoreGive(semaphore->handle) == pdPASS;
}

static bool freertos_semaphore_take(void *context, uint32_t timeout_ms)
{
    freertos_semaphore_t *semaphore = context;

    return xSemaphoreTake(semaphore->handle, freertos_timeout_ms_to_ticks(timeout_ms)) == pdPASS;
}

static const semaphore_operations_t freertos_semaphore_operations = {
    .give = freertos_semaphore_give,
    .take = freertos_semaphore_take,
};

bool freertos_semaphore_init(freertos_semaphore_t *semaphore,
                             size_t maximum_count,
                             size_t initial_count)
{
    if ((semaphore == NULL) || (maximum_count == 0U) || (initial_count > maximum_count)
        || (maximum_count > (size_t)((UBaseType_t)-1))) {
        return false;
    }

    semaphore->handle = xSemaphoreCreateCountingStatic(
        (UBaseType_t)maximum_count, (UBaseType_t)initial_count, &semaphore->semaphore_buffer);
    if (semaphore->handle == NULL) {
        semaphore->semaphore.context = NULL;
        semaphore->semaphore.operations = NULL;
        return false;
    }

    semaphore->semaphore.context = semaphore;
    semaphore->semaphore.operations = &freertos_semaphore_operations;
    return true;
}

semaphore_t *freertos_semaphore_handle(freertos_semaphore_t *semaphore)
{
    if ((semaphore == NULL) || (semaphore->handle == NULL)) {
        return NULL;
    }

    return &semaphore->semaphore;
}
