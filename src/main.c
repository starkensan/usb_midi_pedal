#include "FreeRTOS.h"
#include "task.h"

#include "board/board_config.h"
#include "drivers/rgb_led/rgb_led.h"
#include "pico/stdlib.h"

enum {
    HEARTBEAT_TASK_STACK_WORDS = 256,
    HEARTBEAT_PERIOD_MS = 500,
};

static StaticTask_t heartbeat_task_buffer;
static StackType_t heartbeat_task_stack[HEARTBEAT_TASK_STACK_WORDS];

static void heartbeat_task(void *parameters)
{
    (void)parameters;

    rgb_led_init();
    rgb_led_set_color(RGB_LED_COLOR_GREEN);

    for (;;) {
        rgb_led_toggle();
        vTaskDelay(pdMS_TO_TICKS(HEARTBEAT_PERIOD_MS));
    }
}

void vApplicationStackOverflowHook(TaskHandle_t task, char *task_name)
{
    (void)task;
    (void)task_name;

    taskDISABLE_INTERRUPTS();
    for (;;) {
        tight_loop_contents();
    }
}

int main(void)
{
    stdio_init_all();

    TaskHandle_t heartbeat = xTaskCreateStatic(
        heartbeat_task,
        "heartbeat",
        HEARTBEAT_TASK_STACK_WORDS,
        NULL,
        tskIDLE_PRIORITY + 1,
        heartbeat_task_stack,
        &heartbeat_task_buffer);

    configASSERT(heartbeat != NULL);

    vTaskStartScheduler();

    for (;;) {
        tight_loop_contents();
    }
}
