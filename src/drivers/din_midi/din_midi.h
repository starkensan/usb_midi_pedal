#ifndef DRIVERS_DIN_MIDI_DIN_MIDI_H
#define DRIVERS_DIN_MIDI_DIN_MIDI_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

bool din_midi_init(void);
bool din_midi_write(const uint8_t *bytes, size_t length);

#endif /* DRIVERS_DIN_MIDI_DIN_MIDI_H */
