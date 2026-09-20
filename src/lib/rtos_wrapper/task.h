#ifndef LIB_RTOS_WRAPPER_TASK_H
#define LIB_RTOS_WRAPPER_TASK_H

#include <stddef.h>

#include "FreeRTOS.h"
#include "error_code.h"
#include <task.h>

typedef void (*rtos_task_entry_t)(void *parameter);

typedef struct {
    TaskHandle_t handle;
    StaticTask_t task_buffer;
} rtos_task_t;

error_code_t rtos_task_init(rtos_task_t *task);
error_code_t rtos_task_create(rtos_task_t *task,
                              rtos_task_entry_t entry,
                              const char *name,
                              StackType_t *stack_buffer,
                              size_t stack_depth,
                              void *parameter,
                              UBaseType_t priority);
error_code_t rtos_scheduler_start(void);
error_code_t rtos_task_delete(rtos_task_t *task);

#endif /* LIB_RTOS_WRAPPER_TASK_H */
