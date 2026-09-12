#include "event_flags.h"

#include "freertos_timeout.h"

static bool event_flags_bits_are_valid(event_flags_bits_t bits)
{
    return (bits != 0U) && ((bits & ~EVENT_FLAGS_USER_BITS_MASK) == 0U);
}

bool event_flags_init(event_flags_t *event_flags)
{
    if (event_flags == NULL) {
        return false;
    }

    event_flags->handle = xEventGroupCreateStatic(&event_flags->event_group_buffer);
    return event_flags->handle != NULL;
}

bool event_flags_set(event_flags_t *event_flags, event_flags_bits_t bits)
{
    if ((event_flags == NULL) || (event_flags->handle == NULL) || !event_flags_bits_are_valid(bits)) {
        return false;
    }

    (void)xEventGroupSetBits(event_flags->handle, (EventBits_t)bits);
    return true;
}

bool event_flags_clear(event_flags_t *event_flags, event_flags_bits_t bits)
{
    if ((event_flags == NULL) || (event_flags->handle == NULL) || !event_flags_bits_are_valid(bits)) {
        return false;
    }

    (void)xEventGroupClearBits(event_flags->handle, (EventBits_t)bits);
    return true;
}

bool event_flags_wait(event_flags_t *event_flags,
                      event_flags_bits_t bits,
                      bool wait_all,
                      bool clear_on_exit,
                      uint32_t timeout_ms,
                      event_flags_bits_t *observed_bits)
{
    if (observed_bits != NULL) {
        *observed_bits = 0U;
    }

    if ((event_flags == NULL) || (event_flags->handle == NULL) || !event_flags_bits_are_valid(bits)
        || (observed_bits == NULL)) {
        return false;
    }

    const EventBits_t received_bits = xEventGroupWaitBits(event_flags->handle,
                                                           (EventBits_t)bits,
                                                           clear_on_exit ? pdTRUE : pdFALSE,
                                                           wait_all ? pdTRUE : pdFALSE,
                                                           freertos_timeout_ms_to_ticks(timeout_ms));
    *observed_bits = (event_flags_bits_t)received_bits & EVENT_FLAGS_USER_BITS_MASK;
    return wait_all ? ((*observed_bits & bits) == bits) : ((*observed_bits & bits) != 0U);
}

event_flags_bits_t event_flags_get(const event_flags_t *event_flags)
{
    if ((event_flags == NULL) || (event_flags->handle == NULL)) {
        return 0U;
    }

    return (event_flags_bits_t)xEventGroupGetBits(event_flags->handle) & EVENT_FLAGS_USER_BITS_MASK;
}
