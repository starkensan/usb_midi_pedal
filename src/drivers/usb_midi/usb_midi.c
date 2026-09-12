#include "usb_midi.h"

#include "tusb.h"

enum {
    USB_MIDI_CABLE_NUMBER = 0U,
    MIDI_CHANNEL_COUNT = 16U,
    MIDI_DATA_MAX = 127U,
    MIDI_STATUS_PROGRAM_CHANGE = 0xC0U,
    MIDI_STATUS_CONTROL_CHANGE = 0xB0U,
};

bool usb_midi_init(void)
{
    return tusb_init();
}

void usb_midi_service(void)
{
    tud_task();
}

bool usb_midi_is_connected(void)
{
    return tud_midi_mounted();
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

    return tud_midi_stream_write(USB_MIDI_CABLE_NUMBER, message, sizeof(message)) == sizeof(message);
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

    return tud_midi_stream_write(USB_MIDI_CABLE_NUMBER, message, sizeof(message)) == sizeof(message);
}
