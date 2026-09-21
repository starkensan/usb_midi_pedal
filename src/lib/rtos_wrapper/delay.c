#include "delay.h"

#include <stddef.h>

#include "freertos_timeout.h"
#include "task.h"

static error_code_t delay_ms_to_ticks(uint32_t delay_ms, TickType_t *delay_ticks)
{
    if (delay_ticks == NULL) {
        return ERROR_CODE_INVALID_ARGUMENT;
    }
    if (delay_ms == 0U) {
        return ERROR_CODE_OUT_OF_RANGE;
    }

    *delay_ticks = freertos_timeout_ms_to_ticks(delay_ms);
    return *delay_ticks == 0U ? ERROR_CODE_OUT_OF_RANGE : ERROR_CODE_OK;
}

error_code_t rtos_delay_ms(uint32_t delay_ms)
{
    TickType_t delay_ticks;
    const error_code_t result = delay_ms_to_ticks(delay_ms, &delay_ticks);

    if (result != ERROR_CODE_OK) {
        return result;
    }

    vTaskDelay(delay_ticks);
    return ERROR_CODE_OK;
}

error_code_t rtos_periodic_delay_init(rtos_periodic_delay_t *delay)
{
    if (delay == NULL) {
        return ERROR_CODE_INVALID_ARGUMENT;
    }

    delay->last_wake_time = xTaskGetTickCount();
    return ERROR_CODE_OK;
}

error_code_t rtos_periodic_delay_wait(rtos_periodic_delay_t *delay, uint32_t period_ms)
{
    TickType_t period_ticks;
    const error_code_t result = delay_ms_to_ticks(period_ms, &period_ticks);

    if (delay == NULL) {
        return ERROR_CODE_INVALID_ARGUMENT;
    }
    if (result != ERROR_CODE_OK) {
        return result;
    }

    vTaskDelayUntil(&delay->last_wake_time, period_ticks);
    return ERROR_CODE_OK;
}
