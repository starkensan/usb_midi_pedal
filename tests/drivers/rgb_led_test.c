#include "unity.h"

#include "Mockgpio.h"
#include "drivers/rgb_led/rgb_led.h"

void setUp(void)
{
    Mockgpio_Init();
}

void tearDown(void)
{
    Mockgpio_Verify();
    Mockgpio_Destroy();
}

static void expect_initialization(void)
{
    gpio_init_Expect(18U);
    gpio_put_Expect(18U, true);
    gpio_set_dir_Expect(18U, GPIO_OUT);
    gpio_init_Expect(19U);
    gpio_put_Expect(19U, true);
    gpio_set_dir_Expect(19U, GPIO_OUT);
    gpio_init_Expect(20U);
    gpio_put_Expect(20U, true);
    gpio_set_dir_Expect(20U, GPIO_OUT);
}

void test_rgb_led_init_configures_all_channels_off(void)
{
    expect_initialization();
    rgb_led_init();
}

void test_rgb_led_on_applies_active_low_red_color(void)
{
    expect_initialization();
    rgb_led_init();

    gpio_put_Expect(18U, true);
    gpio_put_Expect(19U, true);
    gpio_put_Expect(20U, true);
    rgb_led_set_color(RGB_LED_COLOR_RED);

    gpio_put_Expect(18U, false);
    gpio_put_Expect(19U, true);
    gpio_put_Expect(20U, true);
    rgb_led_on();
}

void test_rgb_led_toggle_turns_current_color_off_and_on(void)
{
    expect_initialization();
    rgb_led_init();

    gpio_put_Expect(18U, true);
    gpio_put_Expect(19U, true);
    gpio_put_Expect(20U, true);
    rgb_led_set_color(RGB_LED_COLOR_BLUE);

    gpio_put_Expect(18U, true);
    gpio_put_Expect(19U, true);
    gpio_put_Expect(20U, false);
    rgb_led_toggle();

    gpio_put_Expect(18U, true);
    gpio_put_Expect(19U, true);
    gpio_put_Expect(20U, true);
    rgb_led_toggle();
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_rgb_led_init_configures_all_channels_off);
    RUN_TEST(test_rgb_led_on_applies_active_low_red_color);
    RUN_TEST(test_rgb_led_toggle_turns_current_color_off_and_on);

    return UNITY_END();
}
