#ifndef DRIVERS_LOG_OUTPUT_LOG_OUTPUT_H
#define DRIVERS_LOG_OUTPUT_LOG_OUTPUT_H

#include <stddef.h>

#include "error_code.h"

void log_output_init(void);
error_code_t log_output_write(const char *message, size_t length);

#endif
