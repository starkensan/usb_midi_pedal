#include "usb_midi_task.h"

#include "FreeRTOS.h"
#include "task.h"

#include "drivers/usb_midi/usb_midi.h"

enum {
    USB_MIDI_TASK_STACK_WORDS = 256,
    USB_MIDI_SERVICE_PERIOD_MS = 1,
#if defined(USB_MIDI_DIAGNOSTIC_SEND_ENABLED)
    USB_MIDI_DIAGNOSTIC_PERIOD_MS = 1000,
    USB_MIDI_DIAGNOSTIC_CHANNEL = 0,
    USB_MIDI_DIAGNOSTIC_PROGRAM = 0,
    USB_MIDI_DIAGNOSTIC_CONTROLLER = 1,
    USB_MIDI_DIAGNOSTIC_CONTROL_VALUE = 64,
#endif
};

static StaticTask_t usb_midi_task_buffer;
static StackType_t usb_midi_task_stack[USB_MIDI_TASK_STACK_WORDS];

#if defined(USB_MIDI_DIAGNOSTIC_SEND_ENABLED)
static void usb_midi_send_diagnostic_message(void)
{
    static bool send_program_change = true;

    if (send_program_change) {
        send_program_change = !usb_midi_send_program_change(
            USB_MIDI_DIAGNOSTIC_CHANNEL, USB_MIDI_DIAGNOSTIC_PROGRAM);
    } else {
        send_program_change = usb_midi_send_control_change(
            USB_MIDI_DIAGNOSTIC_CHANNEL,
            USB_MIDI_DIAGNOSTIC_CONTROLLER,
            USB_MIDI_DIAGNOSTIC_CONTROL_VALUE);
    }
}
#endif

static void usb_midi_task(void *parameters)
{
#if defined(USB_MIDI_DIAGNOSTIC_SEND_ENABLED)
    TickType_t last_diagnostic_send_time = xTaskGetTickCount();
#endif

    (void)parameters;

    for (;;) {
        usb_midi_service();
#if defined(USB_MIDI_DIAGNOSTIC_SEND_ENABLED)
        const TickType_t current_time = xTaskGetTickCount();

        if (!usb_midi_is_connected()) {
            last_diagnostic_send_time = current_time;
        } else if ((current_time - last_diagnostic_send_time)
                   >= pdMS_TO_TICKS(USB_MIDI_DIAGNOSTIC_PERIOD_MS)) {
            usb_midi_send_diagnostic_message();
            last_diagnostic_send_time = current_time;
        }
#endif
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
