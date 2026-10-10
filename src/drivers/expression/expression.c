#include "expression.h"

#include "board/board_config.h"
#include "hardware/gpio.h"
#include "hardware/i2c.h"
#include "hardware/sync.h"

#include <stddef.h>

#define ADS1015_REGISTER_CONVERSION 0x00u
#define ADS1015_REGISTER_CONFIG 0x01u
#define ADS1015_REGISTER_LOW_THRESHOLD 0x02u
#define ADS1015_REGISTER_HIGH_THRESHOLD 0x03u

#define ADS1015_CONFIG_CONTINUOUS_AIN0_4V096_1600SPS 0x4280u
#define ADS1015_CONFIG_OS_START_CONVERSION 0x8000u

static volatile bool data_ready;
static bool initialized;

static bool write_register(uint8_t reg, uint16_t value)
{
    const uint8_t bytes[] = {
        reg,
        (uint8_t)(value >> 8),
        (uint8_t)value,
    };

    return i2c_write_timeout_us(BOARD_INPUT_I2C_INSTANCE,
                                BOARD_EXPRESSION_ADC_I2C_ADDRESS,
                                bytes,
                                sizeof(bytes),
                                false,
                                BOARD_INPUT_I2C_TIMEOUT_US) == (int)sizeof(bytes);
}

static bool select_register(uint8_t reg)
{
    return i2c_write_timeout_us(BOARD_INPUT_I2C_INSTANCE,
                                BOARD_EXPRESSION_ADC_I2C_ADDRESS,
                                &reg,
                                1u,
                                true,
                                BOARD_INPUT_I2C_TIMEOUT_US) == 1;
}

static void data_ready_gpio_callback(uint gpio, uint32_t events)
{
    if ((gpio == BOARD_EXPRESSION_ADC_READY_PIN) &&
        ((events & GPIO_IRQ_EDGE_FALL) != 0u)) {
        data_ready = true;
    }
}

bool expression_init(void)
{
    initialized = false;
    data_ready = false;

    gpio_set_irq_enabled(BOARD_EXPRESSION_ADC_READY_PIN,
                         GPIO_IRQ_EDGE_FALL,
                         false);
    gpio_init(BOARD_EXPRESSION_ADC_READY_PIN);
    gpio_set_dir(BOARD_EXPRESSION_ADC_READY_PIN, GPIO_IN);
    gpio_pull_up(BOARD_EXPRESSION_ADC_READY_PIN);

    /* Threshold MSBs enable conversion-ready mode on ALERT/RDY. */
    if (!write_register(ADS1015_REGISTER_LOW_THRESHOLD, 0x0000u) ||
        !write_register(ADS1015_REGISTER_HIGH_THRESHOLD, 0x8000u) ||
        !write_register(ADS1015_REGISTER_CONFIG,
                        ADS1015_CONFIG_CONTINUOUS_AIN0_4V096_1600SPS |
                            ADS1015_CONFIG_OS_START_CONVERSION)) {
        return false;
    }

    gpio_set_irq_enabled_with_callback(BOARD_EXPRESSION_ADC_READY_PIN,
                                       GPIO_IRQ_EDGE_FALL,
                                       true,
                                       data_ready_gpio_callback);
    initialized = true;
    return true;
}

bool expression_take_ready(void)
{
    const uint32_t interrupt_state = save_and_disable_interrupts();
    const bool ready = data_ready;
    data_ready = false;
    restore_interrupts(interrupt_state);

    return ready;
}

bool expression_read_raw(uint16_t *sample)
{
    uint8_t bytes[2];
    uint16_t raw;
    int16_t signed_sample;

    if (!initialized || (sample == NULL) ||
        !select_register(ADS1015_REGISTER_CONVERSION) ||
        (i2c_read_timeout_us(BOARD_INPUT_I2C_INSTANCE,
                             BOARD_EXPRESSION_ADC_I2C_ADDRESS,
                             bytes,
                             sizeof(bytes),
                             false,
                             BOARD_INPUT_I2C_TIMEOUT_US) != (int)sizeof(bytes))) {
        return false;
    }

    raw = (uint16_t)((((uint16_t)bytes[0] << 8) | bytes[1]) >> 4);
    if ((raw & 0x0800u) != 0u) {
        signed_sample = (int16_t)(raw | 0xf000u);
    } else {
        signed_sample = (int16_t)raw;
    }

    *sample = (signed_sample < 0) ? 0u : (uint16_t)signed_sample;
    return true;
}
