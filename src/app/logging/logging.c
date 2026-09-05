#include "logging.h"

#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

#include "FreeRTOS.h"
#include "semphr.h"

#include "pico/stdio.h"

#if defined(LOG_CONFIG_OUTPUT_UART)
#include "debug_uart.h"
#endif

enum {
    LOG_BUFFER_SIZE = 256,
};

static StaticSemaphore_t log_mutex_storage;
static SemaphoreHandle_t log_mutex;

static const char *const level_names[] = {
    [LOG_LEVEL_ERROR] = "ERROR",
    [LOG_LEVEL_WARN] = "WARN",
    [LOG_LEVEL_INFO] = "INFO",
    [LOG_LEVEL_DEBUG] = "DEBUG",
};

static bool is_enabled(log_level_t level)
{
    return level <= LOG_CONFIG_MIN_LEVEL;
}

static size_t bounded_length(const char *text, size_t maximum_length)
{
    size_t length = 0;

    while (length < maximum_length && text[length] != '\0') {
        ++length;
    }

    return length;
}

static void write_output(const char *message, size_t length)
{
#if defined(LOG_CONFIG_OUTPUT_USB_CDC)
    (void)fwrite(message, 1U, length, stdout);
    stdio_flush();
#else
    debug_uart_write(message, length);
#endif
}

void logging_init(void)
{
    log_mutex = xSemaphoreCreateMutexStatic(&log_mutex_storage);
    configASSERT(log_mutex != NULL);

#if defined(LOG_CONFIG_OUTPUT_UART)
    debug_uart_init();
#endif
}

void logging_write(log_level_t level, const char *format, ...)
{
    va_list arguments;

    va_start(arguments, format);
    logging_vwrite(level, format, arguments);
    va_end(arguments);
}

void logging_vwrite(log_level_t level, const char *format, va_list arguments)
{
    char buffer[LOG_BUFFER_SIZE];
    int prefix_length;
    int message_length;
    size_t length;

    if (level > LOG_LEVEL_DEBUG || format == NULL || !is_enabled(level)) {
        return;
    }

    configASSERT(log_mutex != NULL);
    (void)xSemaphoreTake(log_mutex, portMAX_DELAY);

    prefix_length = snprintf(buffer, sizeof(buffer), "[%s] ", level_names[level]);
    if (prefix_length > 0 && (size_t)prefix_length < sizeof(buffer)) {
        message_length = vsnprintf(
            &buffer[prefix_length],
            sizeof(buffer) - (size_t)prefix_length - 2U,
            format,
            arguments);

        if (message_length >= 0) {
            length = bounded_length(buffer, sizeof(buffer) - 2U);
            buffer[length++] = '\r';
            buffer[length++] = '\n';
            buffer[length] = '\0';
            write_output(buffer, length);
        }
    }

    (void)xSemaphoreGive(log_mutex);
}
