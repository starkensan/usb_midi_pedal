#ifndef LIB_LOGGING_LOG_LEVEL_H
#define LIB_LOGGING_LOG_LEVEL_H

#include <stdbool.h>

typedef enum {
    LOG_LEVEL_ERROR = 0,
    LOG_LEVEL_WARN,
    LOG_LEVEL_INFO,
    LOG_LEVEL_DEBUG,
} log_level_t;

bool log_level_is_valid(log_level_t level);
bool log_level_is_enabled(log_level_t level, log_level_t minimum_level);

#endif
