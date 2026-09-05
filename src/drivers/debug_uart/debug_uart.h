#ifndef DRIVERS_DEBUG_UART_DEBUG_UART_H
#define DRIVERS_DEBUG_UART_DEBUG_UART_H

#include <stddef.h>

void debug_uart_init(void);
void debug_uart_write(const char *data, size_t length);

#endif
