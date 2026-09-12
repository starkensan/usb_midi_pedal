#include "drivers/log_output/log_output.h"

#include <stdio.h>

#include "pico/stdio.h"

#if defined(LOG_CONFIG_OUTPUT_UART)
#include "debug_uart.h"
#endif

void log_output_init(void)
{
#if defined(LOG_CONFIG_OUTPUT_UART)
    debug_uart_init();
#endif
}

error_code_t log_output_write(const char *message, size_t length)
{
#if defined(LOG_CONFIG_OUTPUT_USB_CDC)
    if (fwrite(message, 1U, length, stdout) != length) {
        return ERROR_CODE_IO;
    }

    stdio_flush();
#else
    debug_uart_write(message, length);
#endif

    return ERROR_CODE_OK;
}
