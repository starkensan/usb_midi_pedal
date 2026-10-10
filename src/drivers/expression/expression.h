#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <stdbool.h>
#include <stdint.h>

/* Configure ADS1015 AIN0 for continuous 1.6 kSPS conversion. */
bool expression_init(void);

/* Consume one or more pending ALERT/RDY falling edges without blocking. */
bool expression_take_ready(void);

/* Read the most recent 12-bit conversion (single-ended range 0..2047). */
bool expression_read_raw(uint16_t *sample);

#endif /* EXPRESSION_H */
