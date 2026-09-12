#ifndef PLATFORM_FREERTOS_FREERTOS_EVENT_FLAGS_H
#define PLATFORM_FREERTOS_FREERTOS_EVENT_FLAGS_H

#include <stdbool.h>

#include "FreeRTOS.h"
#include "event_groups.h"

#include "lib/concurrency/event_flags.h"

typedef struct {
    EventGroupHandle_t handle;
    StaticEventGroup_t event_group_buffer;
    event_flags_t event_flags;
} freertos_event_flags_t;

bool freertos_event_flags_init(freertos_event_flags_t *event_flags);
event_flags_t *freertos_event_flags_handle(freertos_event_flags_t *event_flags);

#endif
