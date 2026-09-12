#include "usb_midi_task.h"

#include "FreeRTOS.h"
#include "task.h"

#include "drivers/usb_midi/usb_midi.h"

enum {
    USB_MIDI_TASK_STACK_WORDS = 256,
    USB_MIDI_SERVICE_PERIOD_MS = 1,
};

static StaticTask_t usb_midi_task_buffer;
static StackType_t usb_midi_task_stack[USB_MIDI_TASK_STACK_WORDS];

static void usb_midi_task(void *parameters)
{
    (void)parameters;

    for (;;) {
        usb_midi_service();
        vTaskDelay(pdMS_TO_TICKS(USB_MIDI_SERVICE_PERIOD_MS));
    }
}

bool usb_midi_task_start(void)
{
    return xTaskCreateStatic(
               usb_midi_task,
               "usb_midi",
               USB_MIDI_TASK_STACK_WORDS,
               NULL,
               tskIDLE_PRIORITY + 1,
               usb_midi_task_stack,
               &usb_midi_task_buffer)
        != NULL;
}
