#include "platform/freertos/freertos_event_flags.h"

#include "platform/freertos/freertos_timeout.h"

static bool freertos_event_flags_set(void *context, event_flags_bits_t bits)
{
    freertos_event_flags_t *event_flags = context;

    (void)xEventGroupSetBits(event_flags->handle, (EventBits_t)bits);
    return true;
}

static bool freertos_event_flags_clear(void *context, event_flags_bits_t bits)
{
    freertos_event_flags_t *event_flags = context;

    (void)xEventGroupClearBits(event_flags->handle, (EventBits_t)bits);
    return true;
}

static bool freertos_event_flags_wait(void *context,
                                      event_flags_bits_t bits,
                                      bool wait_all,
                                      bool clear_on_exit,
                                      uint32_t timeout_ms,
                                      event_flags_bits_t *observed_bits)
{
    freertos_event_flags_t *event_flags = context;
    const EventBits_t received_bits = xEventGroupWaitBits(event_flags->handle,
                                                           (EventBits_t)bits,
                                                           clear_on_exit ? pdTRUE : pdFALSE,
                                                           wait_all ? pdTRUE : pdFALSE,
                                                           freertos_timeout_ms_to_ticks(timeout_ms));

    *observed_bits = (event_flags_bits_t)received_bits & EVENT_FLAGS_USER_BITS_MASK;
    if (wait_all) {
        return (*observed_bits & bits) == bits;
    }

    return (*observed_bits & bits) != 0U;
}

static event_flags_bits_t freertos_event_flags_get(const void *context)
{
    const freertos_event_flags_t *event_flags = context;

    return (event_flags_bits_t)xEventGroupGetBits(event_flags->handle) & EVENT_FLAGS_USER_BITS_MASK;
}

static const event_flags_operations_t freertos_event_flags_operations = {
    .set = freertos_event_flags_set,
    .clear = freertos_event_flags_clear,
    .wait = freertos_event_flags_wait,
    .get = freertos_event_flags_get,
};

bool freertos_event_flags_init(freertos_event_flags_t *event_flags)
{
    if (event_flags == NULL) {
        return false;
    }

    event_flags->handle = xEventGroupCreateStatic(&event_flags->event_group_buffer);
    if (event_flags->handle == NULL) {
        event_flags->event_flags.context = NULL;
        event_flags->event_flags.operations = NULL;
        return false;
    }

    event_flags->event_flags.context = event_flags;
    event_flags->event_flags.operations = &freertos_event_flags_operations;
    return true;
}

event_flags_t *freertos_event_flags_handle(freertos_event_flags_t *event_flags)
{
    if ((event_flags == NULL) || (event_flags->handle == NULL)) {
        return NULL;
    }

    return &event_flags->event_flags;
}
