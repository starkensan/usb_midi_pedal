#ifndef INPUT_I2C_H
#define INPUT_I2C_H

#include <stdbool.h>

/* Initialize the shared I2C1 bus used by input devices. */
bool board_input_i2c_init(void);

#endif /* INPUT_I2C_H */
