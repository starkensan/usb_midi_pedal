#include "log_level.h"

bool log_level_is_valid(log_level_t level)
{
    return level >= LOG_LEVEL_ERROR && level <= LOG_LEVEL_DEBUG;
}

bool log_level_is_enabled(log_level_t level, log_level_t minimum_level)
{
    return log_level_is_valid(level) && log_level_is_valid(minimum_level)
        && level <= minimum_level;
}
