#ifndef DRIVERS_USB_MIDI_USB_MIDI_TINYUSB_H
#define DRIVERS_USB_MIDI_USB_MIDI_TINYUSB_H

#include <stdbool.h>

bool usb_midi_tinyusb_try_lock(void);
void usb_midi_tinyusb_unlock(void);

#endif /* DRIVERS_USB_MIDI_USB_MIDI_TINYUSB_H */
