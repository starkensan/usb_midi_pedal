#ifndef LIB_CONCURRENCY_EVENT_FLAGS_H
#define LIB_CONCURRENCY_EVENT_FLAGS_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "FreeRTOS.h"
#include "event_groups.h"

#define EVENT_FLAGS_USER_BITS_MASK UINT32_C(0x00ffffff)

typedef uint32_t event_flags_bits_t;

typedef struct {
    EventGroupHandle_t handle;
    StaticEventGroup_t event_group_buffer;
} event_flags_t;

bool event_flags_init(event_flags_t *event_flags);
bool event_flags_set(event_flags_t *event_flags, event_flags_bits_t bits);
bool event_flags_clear(event_flags_t *event_flags, event_flags_bits_t bits);
bool event_flags_wait(event_flags_t *event_flags,
                      event_flags_bits_t bits,
                      bool wait_all,
                      bool clear_on_exit,
                      uint32_t timeout_ms,
                      event_flags_bits_t *observed_bits);
event_flags_bits_t event_flags_get(const event_flags_t *event_flags);

#endif
