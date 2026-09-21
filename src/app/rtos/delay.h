#ifndef APP_RTOS_DELAY_H
#define APP_RTOS_DELAY_H

#include <stdint.h>

#include "FreeRTOS.h"
#include "error_code.h"

typedef struct {
    TickType_t last_wake_time;
} rtos_periodic_delay_t;

error_code_t rtos_delay_ms(uint32_t delay_ms);
error_code_t rtos_periodic_delay_init(rtos_periodic_delay_t *delay);
error_code_t rtos_periodic_delay_wait(rtos_periodic_delay_t *delay, uint32_t period_ms);

#endif /* APP_RTOS_DELAY_H */
