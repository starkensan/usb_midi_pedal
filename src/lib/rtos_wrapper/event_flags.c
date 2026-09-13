#include "event_flags.h"

#include "freertos_timeout.h"

static bool event_flags_bits_are_valid(event_flags_bits_t bits)
{
    return (bits != 0U) && ((bits & ~EVENT_FLAGS_USER_BITS_MASK) == 0U);
}

error_code_t event_flags_init(event_flags_t *event_flags)
{
    if (event_flags == NULL) {
        return ERROR_CODE_INVALID_ARGUMENT;
    }

    event_flags->handle = xEventGroupCreateStatic(&event_flags->event_group_buffer);
    return event_flags->handle != NULL ? ERROR_CODE_OK : ERROR_CODE_NOT_READY;
}

error_code_t event_flags_set(event_flags_t *event_flags, event_flags_bits_t bits)
{
    if (event_flags == NULL) {
        return ERROR_CODE_INVALID_ARGUMENT;
    }
    if (event_flags->handle == NULL) {
        return ERROR_CODE_NOT_READY;
    }
    if (!event_flags_bits_are_valid(bits)) {
        return ERROR_CODE_OUT_OF_RANGE;
    }

    (void)xEventGroupSetBits(event_flags->handle, (EventBits_t)bits);
    return ERROR_CODE_OK;
}

error_code_t event_flags_clear(event_flags_t *event_flags, event_flags_bits_t bits)
{
    if (event_flags == NULL) {
        return ERROR_CODE_INVALID_ARGUMENT;
    }
    if (event_flags->handle == NULL) {
        return ERROR_CODE_NOT_READY;
    }
    if (!event_flags_bits_are_valid(bits)) {
        return ERROR_CODE_OUT_OF_RANGE;
    }

    (void)xEventGroupClearBits(event_flags->handle, (EventBits_t)bits);
    return ERROR_CODE_OK;
}

error_code_t event_flags_wait(event_flags_t *event_flags,
                              event_flags_bits_t bits,
                              bool wait_all,
                              bool clear_on_exit,
                              uint32_t timeout_ms,
                              event_flags_bits_t *observed_bits)
{
    if (observed_bits != NULL) {
        *observed_bits = 0U;
    }

    if ((event_flags == NULL) || (observed_bits == NULL)) {
        return ERROR_CODE_INVALID_ARGUMENT;
    }
    if (event_flags->handle == NULL) {
        return ERROR_CODE_NOT_READY;
    }
    if (!event_flags_bits_are_valid(bits)) {
        return ERROR_CODE_OUT_OF_RANGE;
    }

    const EventBits_t received_bits = xEventGroupWaitBits(event_flags->handle,
                                                           (EventBits_t)bits,
                                                           clear_on_exit ? pdTRUE : pdFALSE,
                                                           wait_all ? pdTRUE : pdFALSE,
                                                           freertos_timeout_ms_to_ticks(timeout_ms));
    *observed_bits = (event_flags_bits_t)received_bits & EVENT_FLAGS_USER_BITS_MASK;
    if (wait_all ? ((*observed_bits & bits) == bits) : ((*observed_bits & bits) != 0U)) {
        return ERROR_CODE_OK;
    }

    return ERROR_CODE_TIMEOUT;
}

error_code_t event_flags_get(const event_flags_t *event_flags, event_flags_bits_t *bits)
{
    if ((event_flags == NULL) || (bits == NULL)) {
        return ERROR_CODE_INVALID_ARGUMENT;
    }
    if (event_flags->handle == NULL) {
        return ERROR_CODE_NOT_READY;
    }

    *bits = (event_flags_bits_t)xEventGroupGetBits(event_flags->handle) & EVENT_FLAGS_USER_BITS_MASK;

    return ERROR_CODE_OK;
}
