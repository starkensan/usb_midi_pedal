#include "task.h"

#include <string.h>

error_code_t rtos_task_init(rtos_task_t *task)
{
    if (task == NULL) {
        return ERROR_CODE_INVALID_ARGUMENT;
    }

    (void)memset(task, 0, sizeof(*task));
    return ERROR_CODE_OK;
}

error_code_t rtos_task_create(rtos_task_t *task,
                              rtos_task_entry_t entry,
                              const char *name,
                              StackType_t *stack_buffer,
                              size_t stack_depth,
                              void *parameter,
                              UBaseType_t priority)
{
    if ((task == NULL) || (entry == NULL) || (name == NULL) || (stack_buffer == NULL)) {
        return ERROR_CODE_INVALID_ARGUMENT;
    }
    if ((stack_depth == 0U) || (stack_depth > (size_t)((configSTACK_DEPTH_TYPE)-1))
        || (priority >= (UBaseType_t)configMAX_PRIORITIES)) {
        return ERROR_CODE_OUT_OF_RANGE;
    }
    if (task->handle != NULL) {
        return ERROR_CODE_NOT_READY;
    }

    task->handle = xTaskCreateStatic(entry,
                                     name,
                                     (configSTACK_DEPTH_TYPE)stack_depth,
                                     parameter,
                                     priority,
                                     stack_buffer,
                                     &task->task_buffer);
    return task->handle != NULL ? ERROR_CODE_OK : ERROR_CODE_NOT_READY;
}

error_code_t rtos_scheduler_start(void)
{
    vTaskStartScheduler();
    return ERROR_CODE_NOT_READY;
}

error_code_t rtos_task_delete(rtos_task_t *task)
{
    if (task == NULL) {
        return ERROR_CODE_INVALID_ARGUMENT;
    }
    if (task->handle == NULL) {
        return ERROR_CODE_NOT_READY;
    }
    if (task->handle == xTaskGetCurrentTaskHandle()) {
        return ERROR_CODE_UNSUPPORTED;
    }

    vTaskDelete(task->handle);
    task->handle = NULL;
    return ERROR_CODE_OK;
}
