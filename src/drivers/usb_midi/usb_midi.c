#include "usb_midi.h"
#include "usb_midi_tinyusb.h"

#include <stddef.h>

#include "FreeRTOS.h"
#include "semphr.h"
#include "tusb.h"

enum {
    USB_MIDI_CABLE_NUMBER = 0U,
    MIDI_CHANNEL_COUNT = 16U,
    MIDI_DATA_MAX = 127U,
    MIDI_STATUS_PROGRAM_CHANGE = 0xC0U,
    MIDI_STATUS_CONTROL_CHANGE = 0xB0U,
};

static StaticSemaphore_t tinyusb_mutex_storage;
static SemaphoreHandle_t tinyusb_mutex;

bool usb_midi_tinyusb_try_lock(void)
{
    return tinyusb_mutex != NULL && xSemaphoreTake(tinyusb_mutex, 0U) == pdTRUE;
}

void usb_midi_tinyusb_unlock(void)
{
    (void)xSemaphoreGive(tinyusb_mutex);
}

bool usb_midi_init(void)
{
    if (tinyusb_mutex == NULL) {
        tinyusb_mutex = xSemaphoreCreateMutexStatic(&tinyusb_mutex_storage);
        if (tinyusb_mutex == NULL) {
            return false;
        }
    }

    return tusb_init();
}

void usb_midi_service(void)
{
    if (!usb_midi_tinyusb_try_lock()) {
        return;
    }

    tud_task_ext(0U, false);
    usb_midi_tinyusb_unlock();
}

bool usb_midi_is_connected(void)
{
    if (!usb_midi_tinyusb_try_lock()) {
        return false;
    }

    const bool connected = tud_midi_mounted();
    usb_midi_tinyusb_unlock();
    return connected;
}

bool usb_midi_send_program_change(uint8_t channel, uint8_t program)
{
    const uint8_t message[] = {
        (uint8_t)(MIDI_STATUS_PROGRAM_CHANGE | channel),
        program,
    };

    if (channel >= MIDI_CHANNEL_COUNT || program > MIDI_DATA_MAX) {
        return false;
    }

    if (!usb_midi_tinyusb_try_lock()) {
        return false;
    }

    const bool sent =
        tud_midi_stream_write(USB_MIDI_CABLE_NUMBER, message, sizeof(message)) == sizeof(message);
    usb_midi_tinyusb_unlock();
    return sent;
}

bool usb_midi_send_control_change(uint8_t channel, uint8_t controller, uint8_t value)
{
    const uint8_t message[] = {
        (uint8_t)(MIDI_STATUS_CONTROL_CHANGE | channel),
        controller,
        value,
    };

    if (channel >= MIDI_CHANNEL_COUNT || controller > MIDI_DATA_MAX || value > MIDI_DATA_MAX) {
        return false;
    }

    if (!usb_midi_tinyusb_try_lock()) {
        return false;
    }

    const bool sent =
        tud_midi_stream_write(USB_MIDI_CABLE_NUMBER, message, sizeof(message)) == sizeof(message);
    usb_midi_tinyusb_unlock();
    return sent;
}
