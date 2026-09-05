#ifndef TEST_MOCK_HARDWARE_GPIO_H
#define TEST_MOCK_HARDWARE_GPIO_H

#include <stdbool.h>

typedef unsigned int uint;

#define GPIO_OUT true

void gpio_init(uint gpio);
void gpio_put(uint gpio, bool value);
void gpio_set_dir(uint gpio, bool out);

#endif /* TEST_MOCK_HARDWARE_GPIO_H */
