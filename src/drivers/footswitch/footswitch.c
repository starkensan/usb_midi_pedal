#include "footswitch.h"

#include <stddef.h>

#include "board/board_config.h"

#define MCP23017_REG_IODIRA 0x00u
#define MCP23017_REG_IPOLA  0x02u
#define MCP23017_REG_GPPUA  0x0cu
#define MCP23017_REG_GPIOA  0x12u
#define FOOTSWITCH_INPUT_MASK 0x3fu

static bool write_register(uint8_t reg, uint8_t value)
{
    const uint8_t data[2] = {reg, value};
    return i2c_write_timeout_us(BOARD_INPUT_I2C_INSTANCE,
                                BOARD_FOOTSWITCH_EXPANDER_I2C_ADDRESS,
                                data, sizeof(data), true,
                                BOARD_INPUT_I2C_TIMEOUT_US) == (int)sizeof(data);
}

bool footswitch_init(void)
{
    /* Inputs are externally pulled up; keep all Port A pins high impedance. */
    if (!write_register(MCP23017_REG_IODIRA, 0xffu)) {
        return false;
    }
    if (!write_register(MCP23017_REG_IPOLA, 0x00u)) {
        return false;
    }
    if (!write_register(MCP23017_REG_GPPUA, 0x00u)) {
        return false;
    }
    return true;
}

bool footswitch_read(uint8_t *pressed_mask)
{
    uint8_t reg = MCP23017_REG_GPIOA;
    uint8_t gpio_state;

    if (pressed_mask == NULL) {
        return false;
    }

    if (i2c_write_timeout_us(BOARD_INPUT_I2C_INSTANCE,
                             BOARD_FOOTSWITCH_EXPANDER_I2C_ADDRESS,
                             &reg, sizeof(reg), true,
                             BOARD_INPUT_I2C_TIMEOUT_US) != (int)sizeof(reg)) {
        return false;
    }
    if (i2c_read_timeout_us(BOARD_INPUT_I2C_INSTANCE,
                            BOARD_FOOTSWITCH_EXPANDER_I2C_ADDRESS,
                            &gpio_state, sizeof(gpio_state), false,
                            BOARD_INPUT_I2C_TIMEOUT_US) != (int)sizeof(gpio_state)) {
        return false;
    }

    *pressed_mask = (uint8_t)(~gpio_state & FOOTSWITCH_INPUT_MASK);
    return true;
}
