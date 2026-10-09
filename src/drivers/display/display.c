#include "display.h"

#include "board/board_config.h"
#include "hardware/gpio.h"
#include "pico/stdlib.h"

#define DISPLAY_WIDTH 128u
#define DISPLAY_PAGE_COUNT 8u
#define DISPLAY_COLUMN_OFFSET 2u
#define DISPLAY_I2C_TIMEOUT_US 100000u
#define DISPLAY_CONTROL_COMMAND 0x00u
#define DISPLAY_CONTROL_DATA 0x40u

static uint8_t display_i2c_address;
static bool display_is_initialized;

static bool display_write_bytes(const uint8_t *data, size_t length)
{
    int result;

    result = i2c_write_blocking_until(
        BOARD_OLED_I2C_INSTANCE,
        display_i2c_address,
        data,
        length,
        false,
        make_timeout_time_us(DISPLAY_I2C_TIMEOUT_US));

    return result == (int)length;
}

static bool display_write_raw_command(uint8_t command)
{
    const uint8_t packet[] = {DISPLAY_CONTROL_COMMAND, command};

    return display_write_bytes(packet, sizeof(packet));
}

bool display_write_command(uint8_t command)
{
    if (!display_is_initialized)
    {
        return false;
    }

    return display_write_raw_command(command);
}

static bool display_write_command_pair(uint8_t command, uint8_t value)
{
    const uint8_t packet[] = {DISPLAY_CONTROL_COMMAND, command, value};

    return display_write_bytes(packet, sizeof(packet));
}

bool display_init(uint8_t address)
{
    static const struct
    {
        uint8_t command;
        uint8_t value;
        bool has_parameter;
    } initialization_commands[] = {
        {0xaeu, 0x00u, false}, /* Display off. */
        {0xd5u, 0x80u, true},  /* Oscillator clock. */
        {0xa8u, 0x3fu, true},  /* 64 multiplex rows. */
        {0xd3u, 0x00u, true},  /* No display offset. */
        {0x40u, 0x00u, false}, /* Start line 0. */
        {0xadu, 0x8bu, true},  /* Enable DC-DC converter. */
        {0xa1u, 0x00u, false}, /* Segment remap. */
        {0xc8u, 0x00u, false}, /* Reverse COM scan. */
        {0xdau, 0x12u, true},  /* 128x64 COM pins. */
        {0x81u, 0x7fu, true},  /* Contrast. */
        {0xd9u, 0x22u, true},  /* Pre-charge period. */
        {0xdbu, 0x35u, true},  /* VCOM deselect level. */
        {0xa4u, 0x00u, false}, /* Display follows RAM. */
        {0xa6u, 0x00u, false}, /* Normal display. */
    };
    size_t index;

    display_is_initialized = false;

    if ((address != 0x3cu) && (address != 0x3du))
    {
        return false;
    }

    display_i2c_address = address;
    i2c_init(BOARD_OLED_I2C_INSTANCE, BOARD_I2C_BAUD_RATE_HZ);
    gpio_set_function(BOARD_OLED_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(BOARD_OLED_I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(BOARD_OLED_I2C_SDA_PIN);
    gpio_pull_up(BOARD_OLED_I2C_SCL_PIN);

    for (index = 0u; index < (sizeof(initialization_commands) / sizeof(initialization_commands[0])); ++index)
    {
        if (initialization_commands[index].has_parameter)
        {
            if (!display_write_command_pair(initialization_commands[index].command,
                                            initialization_commands[index].value))
            {
                return false;
            }
        }
        else if (!display_write_raw_command(initialization_commands[index].command))
        {
            return false;
        }
    }

    /* Display on. */
    if (!display_write_raw_command(0xafu))
    {
        return false;
    }

    display_is_initialized = true;
    return true;
}

bool display_write_page(uint8_t page, const uint8_t *pixels, size_t length)
{
    uint8_t packet[DISPLAY_WIDTH + 1u];
    uint8_t column;

    if (!display_is_initialized || (page >= DISPLAY_PAGE_COUNT) ||
        (pixels == NULL) || (length != DISPLAY_WIDTH))
    {
        return false;
    }

    if (!display_write_command((uint8_t)(0xb0u | page)) ||
        !display_write_command((uint8_t)(0x00u | (DISPLAY_COLUMN_OFFSET & 0x0fu))) ||
        !display_write_command((uint8_t)(0x10u | ((DISPLAY_COLUMN_OFFSET >> 4u) & 0x0fu))) )
    {
        return false;
    }

    packet[0] = DISPLAY_CONTROL_DATA;
    for (column = 0u; column < DISPLAY_WIDTH; ++column)
    {
        packet[column + 1u] = pixels[column];
    }

    return display_write_bytes(packet, sizeof(packet));
}
