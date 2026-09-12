#include "FreeRTOS.h"
#include "task.h"

#include "board/board_config.h"
#include "drivers/rgb_led/rgb_led.h"
#include "drivers/usb_cdc/usb_cdc.h"
#include "drivers/usb_midi/usb_midi.h"
#include "platform/logging/logging.h"
#include "pico/stdlib.h"

enum {
    HEARTBEAT_TASK_STACK_WORDS = 256,
    HEARTBEAT_PERIOD_MS = 500,
    USB_CDC_TASK_STACK_WORDS = 256,
    USB_CDC_SERVICE_PERIOD_MS = 1,
    USB_MIDI_TASK_STACK_WORDS = 256,
    USB_MIDI_SERVICE_PERIOD_MS = 1,
};

static StaticTask_t heartbeat_task_buffer;
static StackType_t heartbeat_task_stack[HEARTBEAT_TASK_STACK_WORDS];

#if defined(LOG_CONFIG_OUTPUT_USB_CDC)
static StaticTask_t usb_cdc_task_buffer;
static StackType_t usb_cdc_task_stack[USB_CDC_TASK_STACK_WORDS];
#endif
static StaticTask_t usb_midi_task_buffer;
static StackType_t usb_midi_task_stack[USB_MIDI_TASK_STACK_WORDS];

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

#if defined(LOG_CONFIG_OUTPUT_USB_CDC)
static void usb_cdc_task(void *parameters)
{
    (void)parameters;

    for (;;) {
        usb_cdc_service();
        vTaskDelay(pdMS_TO_TICKS(USB_CDC_SERVICE_PERIOD_MS));
    }
}
#endif

static void usb_midi_task(void *parameters)
{
    (void)parameters;

    for (;;) {
        usb_midi_service();
        vTaskDelay(pdMS_TO_TICKS(USB_MIDI_SERVICE_PERIOD_MS));
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

    TaskHandle_t usb_midi = xTaskCreateStatic(
        usb_midi_task,
        "usb_midi",
        USB_MIDI_TASK_STACK_WORDS,
        NULL,
        tskIDLE_PRIORITY + 1,
        usb_midi_task_stack,
        &usb_midi_task_buffer);

    configASSERT(usb_midi != NULL);

#if defined(LOG_CONFIG_OUTPUT_USB_CDC)
    TaskHandle_t usb_cdc = xTaskCreateStatic(
        usb_cdc_task,
        "usb_cdc",
        USB_CDC_TASK_STACK_WORDS,
        NULL,
        tskIDLE_PRIORITY + 1,
        usb_cdc_task_stack,
        &usb_cdc_task_buffer);

    configASSERT(usb_cdc != NULL);
#endif

    vTaskStartScheduler();

    for (;;) {
        tight_loop_contents();
    }
}
