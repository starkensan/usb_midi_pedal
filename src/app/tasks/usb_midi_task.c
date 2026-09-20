#include "usb_midi_task.h"

#include "FreeRTOS.h"
#include "drivers/usb_midi/usb_midi.h"
#include "lib/rtos_wrapper/task.h"

enum {
    USB_MIDI_TASK_STACK_WORDS = 256,
    USB_MIDI_SERVICE_PERIOD_MS = 1,
};

static rtos_task_t usb_midi_task_state;
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
    if (rtos_task_init(&usb_midi_task_state) != ERROR_CODE_OK) {
        return false;
    }

    return rtos_task_create(&usb_midi_task_state,
                            usb_midi_task,
                            "usb_midi",
                            usb_midi_task_stack,
                            USB_MIDI_TASK_STACK_WORDS,
                            NULL,
                            tskIDLE_PRIORITY + 1)
        == ERROR_CODE_OK;
}
