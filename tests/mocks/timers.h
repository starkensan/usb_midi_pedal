#ifndef TEST_MOCK_TIMERS_H
#define TEST_MOCK_TIMERS_H

#include "FreeRTOS.h"

typedef struct {
    uint32_t unused;
} StaticTimer_t;

typedef void *TimerHandle_t;
typedef void (*TimerCallbackFunction_t)(TimerHandle_t timer);

TimerHandle_t xTimerCreateStatic(const char *name,
                                 TickType_t period,
                                 BaseType_t auto_reload,
                                 void *context,
                                 TimerCallbackFunction_t callback,
                                 StaticTimer_t *timer_buffer);
BaseType_t xTimerStart(TimerHandle_t timer, TickType_t timeout);
BaseType_t xTimerStop(TimerHandle_t timer, TickType_t timeout);
BaseType_t xTimerReset(TimerHandle_t timer, TickType_t timeout);

#endif /* TEST_MOCK_TIMERS_H */
