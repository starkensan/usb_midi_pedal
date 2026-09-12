#include "FreeRTOS.h"
#include "task.h"

#include <stdbool.h>

#include "app/tasks/usb_midi_task.h"
#include "board/board_config.h"
#include "drivers/rgb_led/rgb_led.h"
#include "drivers/usb_cdc/usb_cdc.h"
#include "drivers/usb_midi/usb_midi.h"
#include "lib/logging/logging.h"
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
    if (!usb_midi_init()) {
        return 1;
    }

#if defined(LOG_CONFIG_OUTPUT_USB_CDC)
    if (!usb_cdc_init()) {
        return 1;
    }
#else
    stdio_init_all();
#endif

    logging_init();
    LOG_INFO("USB MIDI Pedal starting");

    TaskHandle_t heartbeat = xTaskCreateStatic(
        heartbeat_task,
        "heartbeat",
        HEARTBEAT_TASK_STACK_WORDS,
        NULL,
        tskIDLE_PRIORITY + 1,
        heartbeat_task_stack,
        &heartbeat_task_buffer);

    configASSERT(heartbeat != NULL);
    if (heartbeat == NULL) {
        return 1;
    }

    const bool usb_midi_task_started = usb_midi_task_start();
    configASSERT(usb_midi_task_started);
    if (!usb_midi_task_started) {
        return 1;
    }

    vTaskStartScheduler();

    for (;;) {
        tight_loop_contents();
    }
}
