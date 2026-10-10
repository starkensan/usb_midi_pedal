#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* Initialize the SH1106 on the dedicated OLED I2C bus. */
bool display_init(uint8_t address);

/* Send one SH1106 command byte. */
bool display_write_command(uint8_t command);

/* Write one 128-column page (8 vertical pixels per byte). */
bool display_write_page(uint8_t page, const uint8_t *pixels, size_t length);

#endif /* DISPLAY_H */
