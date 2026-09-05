#include "debug_uart.h"

#include <stdint.h>

#include "board/board_config.h"
#include "hardware/gpio.h"
#include "hardware/uart.h"

enum {
    DEBUG_UART_BAUD_RATE = 115200,
};

void debug_uart_init(void)
{
    uart_init(BOARD_DEBUG_UART_INSTANCE, DEBUG_UART_BAUD_RATE);
    gpio_set_function(BOARD_DEBUG_UART_TX_PIN, GPIO_FUNC_UART);
}

void debug_uart_write(const char *data, size_t length)
{
    if (data != NULL && length > 0U) {
        uart_write_blocking(BOARD_DEBUG_UART_INSTANCE, (const uint8_t *)data, length);
    }
}
