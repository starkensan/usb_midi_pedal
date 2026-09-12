#ifndef DRIVERS_USB_MIDI_USB_MIDI_H
#define DRIVERS_USB_MIDI_USB_MIDI_H

#include <stdbool.h>
#include <stdint.h>

bool usb_midi_init(void);
void usb_midi_service(void);
bool usb_midi_is_connected(void);
bool usb_midi_send_program_change(uint8_t channel, uint8_t program);
bool usb_midi_send_control_change(uint8_t channel, uint8_t controller, uint8_t value);

#endif /* DRIVERS_USB_MIDI_USB_MIDI_H */
