#include "timer.h"

#include <string.h>

#include "freertos_timeout.h"

static error_code_t timer_is_ready(const rtos_timer_t *timer)
{
    if (timer == NULL) {
        return ERROR_CODE_INVALID_ARGUMENT;
    }
    if (timer->handle == NULL) {
        return ERROR_CODE_NOT_READY;
    }

    return ERROR_CODE_OK;
}

error_code_t rtos_timer_init(rtos_timer_t *timer,
                             const char *name,
                             uint32_t period_ms,
                             bool auto_reload,
                             TimerCallbackFunction_t callback,
                             void *context)
{
    const TickType_t period_ticks = freertos_timeout_ms_to_ticks(period_ms);

    if ((timer == NULL) || (name == NULL) || (callback == NULL)) {
        return ERROR_CODE_INVALID_ARGUMENT;
    }
    if ((period_ms == 0U) || (period_ticks == 0U)) {
        return ERROR_CODE_OUT_OF_RANGE;
    }

    (void)memset(timer, 0, sizeof(*timer));
    timer->handle = xTimerCreateStatic(name,
                                       period_ticks,
                                       auto_reload ? pdTRUE : pdFALSE,
                                       context,
                                       callback,
                                       &timer->timer_buffer);
    return timer->handle != NULL ? ERROR_CODE_OK : ERROR_CODE_NOT_READY;
}

error_code_t rtos_timer_start(rtos_timer_t *timer)
{
    const error_code_t result = timer_is_ready(timer);

    return result != ERROR_CODE_OK ? result
                                   : (xTimerStart(timer->handle, 0U) == pdPASS ? ERROR_CODE_OK
                                                                               : ERROR_CODE_NOT_READY);
}

error_code_t rtos_timer_stop(rtos_timer_t *timer)
{
    const error_code_t result = timer_is_ready(timer);

    return result != ERROR_CODE_OK ? result
                                   : (xTimerStop(timer->handle, 0U) == pdPASS ? ERROR_CODE_OK
                                                                              : ERROR_CODE_NOT_READY);
}

error_code_t rtos_timer_reset(rtos_timer_t *timer)
{
    const error_code_t result = timer_is_ready(timer);

    return result != ERROR_CODE_OK ? result
                                   : (xTimerReset(timer->handle, 0U) == pdPASS ? ERROR_CODE_OK
                                                                               : ERROR_CODE_NOT_READY);
}
