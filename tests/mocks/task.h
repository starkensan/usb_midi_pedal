#ifndef TEST_MOCK_TASK_H
#define TEST_MOCK_TASK_H

#include "FreeRTOS.h"

void vTaskDelay(TickType_t ticks);
TickType_t xTaskGetTickCount(void);
void vTaskDelayUntil(TickType_t *previous_wake_time, TickType_t increment);

#endif /* TEST_MOCK_TASK_H */
