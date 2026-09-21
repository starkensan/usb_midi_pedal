#ifndef APP_RTOS_TIMER_H
#define APP_RTOS_TIMER_H

#include <stdbool.h>
#include <stdint.h>

#include "FreeRTOS.h"
#include "error_code.h"
#include "timers.h"

typedef struct {
    TimerHandle_t handle;
    StaticTimer_t timer_buffer;
} rtos_timer_t;

error_code_t rtos_timer_init(rtos_timer_t *timer,
                             const char *name,
                             uint32_t period_ms,
                             bool auto_reload,
                             TimerCallbackFunction_t callback,
                             void *context);
error_code_t rtos_timer_start(rtos_timer_t *timer);
error_code_t rtos_timer_stop(rtos_timer_t *timer);
error_code_t rtos_timer_reset(rtos_timer_t *timer);

#endif /* APP_RTOS_TIMER_H */
