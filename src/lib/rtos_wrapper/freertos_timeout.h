#ifndef LIB_CONCURRENCY_FREERTOS_TIMEOUT_H
#define LIB_CONCURRENCY_FREERTOS_TIMEOUT_H

#include <stdint.h>

#include "FreeRTOS.h"

static inline TickType_t freertos_timeout_ms_to_ticks(uint32_t timeout_ms)
{
    const uint64_t ticks = ((uint64_t)timeout_ms * (uint64_t)configTICK_RATE_HZ) / UINT64_C(1000);

    if (ticks >= (uint64_t)portMAX_DELAY) {
        return portMAX_DELAY - (TickType_t)1U;
    }

    return (TickType_t)ticks;
}

#endif
