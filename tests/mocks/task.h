#ifndef TEST_MOCK_TASK_H
#define TEST_MOCK_TASK_H

#include "FreeRTOS.h"

typedef struct {
    uint32_t unused;
} StaticTask_t;

typedef void *TaskHandle_t;
typedef void (*TaskFunction_t)(void *parameter);

TaskHandle_t xTaskCreateStatic(TaskFunction_t entry,
                               const char *name,
                               configSTACK_DEPTH_TYPE stack_depth,
                               void *parameter,
                               UBaseType_t priority,
                               StackType_t *stack_buffer,
                               StaticTask_t *task_buffer);
void vTaskStartScheduler(void);
void vTaskDelete(TaskHandle_t task);
TaskHandle_t xTaskGetCurrentTaskHandle(void);

#endif /* TEST_MOCK_TASK_H */
