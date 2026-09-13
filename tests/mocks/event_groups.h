#ifndef TEST_MOCK_EVENT_GROUPS_H
#define TEST_MOCK_EVENT_GROUPS_H

#include <stdint.h>

#include "FreeRTOS.h"

typedef uint32_t EventBits_t;

typedef struct {
    uint32_t unused;
} StaticEventGroup_t;

typedef void *EventGroupHandle_t;

EventGroupHandle_t xEventGroupCreateStatic(StaticEventGroup_t *event_group_buffer);
EventBits_t xEventGroupSetBits(EventGroupHandle_t event_group, EventBits_t bits);
EventBits_t xEventGroupClearBits(EventGroupHandle_t event_group, EventBits_t bits);
EventBits_t xEventGroupWaitBits(EventGroupHandle_t event_group,
                                EventBits_t bits_to_wait_for,
                                BaseType_t clear_on_exit,
                                BaseType_t wait_for_all,
                                TickType_t timeout);
EventBits_t xEventGroupGetBits(EventGroupHandle_t event_group);

#endif /* TEST_MOCK_EVENT_GROUPS_H */
