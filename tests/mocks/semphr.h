#ifndef TEST_MOCK_SEMPHR_H
#define TEST_MOCK_SEMPHR_H

#include "FreeRTOS.h"

typedef struct {
    uint32_t unused;
} StaticSemaphore_t;

typedef void *SemaphoreHandle_t;

SemaphoreHandle_t xSemaphoreCreateMutexStatic(StaticSemaphore_t *semaphore_buffer);
SemaphoreHandle_t xSemaphoreCreateCountingStatic(UBaseType_t maximum_count,
                                                 UBaseType_t initial_count,
                                                 StaticSemaphore_t *semaphore_buffer);
BaseType_t xSemaphoreTake(SemaphoreHandle_t semaphore, uint32_t timeout);
BaseType_t xSemaphoreGive(SemaphoreHandle_t semaphore);

#endif /* TEST_MOCK_SEMPHR_H */
