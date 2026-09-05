#include "rgb_led.h"

#include <stdbool.h>

#include "board/board_config.h"
#include "hardware/gpio.h"

static rgb_led_color_t current_color = RGB_LED_COLOR_OFF;
static bool is_on;

static void rgb_led_write_channel(uint pin, bool enabled)
{
    gpio_put(pin, BOARD_RGB_LED_IS_ACTIVE_LOW ? !enabled : enabled);
}

static void rgb_led_apply(void)
{
    rgb_led_write_channel(
        BOARD_RGB_LED_RED_PIN,
        is_on && ((current_color & RGB_LED_COLOR_RED) != 0U));
    rgb_led_write_channel(
        BOARD_RGB_LED_GREEN_PIN,
        is_on && ((current_color & RGB_LED_COLOR_GREEN) != 0U));
    rgb_led_write_channel(
        BOARD_RGB_LED_BLUE_PIN,
        is_on && ((current_color & RGB_LED_COLOR_BLUE) != 0U));
}

void rgb_led_init(void)
{
    const uint pins[] = {
        BOARD_RGB_LED_RED_PIN,
        BOARD_RGB_LED_GREEN_PIN,
        BOARD_RGB_LED_BLUE_PIN,
    };

    current_color = RGB_LED_COLOR_OFF;
    is_on = false;

    for (uint index = 0; index < 3U; ++index) {
        gpio_init(pins[index]);
        gpio_put(pins[index], true);
        gpio_set_dir(pins[index], GPIO_OUT);
    }
}

void rgb_led_set_color(rgb_led_color_t color)
{
    current_color = color & RGB_LED_COLOR_WHITE;
    rgb_led_apply();
}

void rgb_led_on(void)
{
    is_on = true;
    rgb_led_apply();
}

void rgb_led_off(void)
{
    is_on = false;
    rgb_led_apply();
}

void rgb_led_toggle(void)
{
    is_on = !is_on;
    rgb_led_apply();
}
