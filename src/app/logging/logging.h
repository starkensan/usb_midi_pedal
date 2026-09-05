#ifndef APP_LOGGING_LOGGING_H
#define APP_LOGGING_LOGGING_H

#include <stdarg.h>

typedef enum {
    LOG_LEVEL_ERROR = 0,
    LOG_LEVEL_WARN,
    LOG_LEVEL_INFO,
    LOG_LEVEL_DEBUG,
} log_level_t;

void logging_init(void);
void logging_write(log_level_t level, const char *format, ...);
void logging_vwrite(log_level_t level, const char *format, va_list arguments);

#define LOG_ERROR(...) logging_write(LOG_LEVEL_ERROR, __VA_ARGS__)
#define LOG_WARN(...)  logging_write(LOG_LEVEL_WARN, __VA_ARGS__)
#define LOG_INFO(...)  logging_write(LOG_LEVEL_INFO, __VA_ARGS__)
#define LOG_DEBUG(...) logging_write(LOG_LEVEL_DEBUG, __VA_ARGS__)

#endif
