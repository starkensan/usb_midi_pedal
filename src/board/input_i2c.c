#include "input_i2c.h"

#include "board_config.h"
#include "hardware/gpio.h"

bool board_input_i2c_init(void)
{
    static bool initialized;

    if (initialized) {
        return true;
    }

    if (i2c_init(BOARD_INPUT_I2C_INSTANCE, BOARD_I2C_BAUD_RATE_HZ) == 0u) {
        return false;
    }

    gpio_set_function(BOARD_INPUT_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(BOARD_INPUT_I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(BOARD_INPUT_I2C_SDA_PIN);
    gpio_pull_up(BOARD_INPUT_I2C_SCL_PIN);
    initialized = true;
    return true;
}
