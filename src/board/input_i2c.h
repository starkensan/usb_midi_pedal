#ifndef BOARD_INPUT_I2C_H
#define BOARD_INPUT_I2C_H

#include <stdbool.h>

/* Initialize the shared I2C1 bus for the expression ADC and input expander. */
bool board_input_i2c_init(void);

#endif /* BOARD_INPUT_I2C_H */
