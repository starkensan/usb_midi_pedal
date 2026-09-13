#ifndef TEST_MOCK_FREERTOS_H
#define TEST_MOCK_FREERTOS_H

#include <stdint.h>

typedef int32_t BaseType_t;
typedef uint32_t UBaseType_t;
typedef uint32_t TickType_t;

#define pdTRUE ((BaseType_t)1)
#define pdFALSE ((BaseType_t)0)
#define pdPASS pdTRUE
#define pdFAIL pdFALSE
#define portMAX_DELAY UINT32_MAX

#define configTICK_RATE_HZ 1000U

#endif /* TEST_MOCK_FREERTOS_H */
