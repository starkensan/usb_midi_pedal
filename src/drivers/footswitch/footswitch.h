#ifndef FOOTSWITCH_H
#define FOOTSWITCH_H

#include <stdbool.h>
#include <stdint.h>

/* Initialize the shared input I2C bus and MCP23017 footswitch inputs. */
bool footswitch_init(void);

/* Read GPA0..GPA5, normalized to pressed=1. Upper two bits are always zero. */
bool footswitch_read(uint8_t *pressed_mask);

#endif /* FOOTSWITCH_H */
