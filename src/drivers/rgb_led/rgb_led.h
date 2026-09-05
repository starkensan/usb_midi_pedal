#ifndef RGB_LED_H
#define RGB_LED_H

#include <stdint.h>

typedef enum {
    RGB_LED_COLOR_OFF = 0U,
    RGB_LED_COLOR_RED = 1U << 0,
    RGB_LED_COLOR_GREEN = 1U << 1,
    RGB_LED_COLOR_BLUE = 1U << 2,
    RGB_LED_COLOR_YELLOW = RGB_LED_COLOR_RED | RGB_LED_COLOR_GREEN,
    RGB_LED_COLOR_MAGENTA = RGB_LED_COLOR_RED | RGB_LED_COLOR_BLUE,
    RGB_LED_COLOR_CYAN = RGB_LED_COLOR_GREEN | RGB_LED_COLOR_BLUE,
    RGB_LED_COLOR_WHITE = RGB_LED_COLOR_RED | RGB_LED_COLOR_GREEN | RGB_LED_COLOR_BLUE,
} rgb_led_color_t;

void rgb_led_init(void);
void rgb_led_set_color(rgb_led_color_t color);
void rgb_led_on(void);
void rgb_led_off(void);
void rgb_led_toggle(void);

#endif
