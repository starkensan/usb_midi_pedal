#include "event_flags.h"

static bool event_flags_bits_are_valid(event_flags_bits_t bits)
{
    return (bits != 0U) && ((bits & ~EVENT_FLAGS_USER_BITS_MASK) == 0U);
}

bool event_flags_set(event_flags_t *event_flags, event_flags_bits_t bits)
{
    if ((event_flags == NULL) || (event_flags->context == NULL) || (event_flags->operations == NULL)
        || (event_flags->operations->set == NULL) || !event_flags_bits_are_valid(bits)) {
        return false;
    }

    return event_flags->operations->set(event_flags->context, bits);
}

bool event_flags_clear(event_flags_t *event_flags, event_flags_bits_t bits)
{
    if ((event_flags == NULL) || (event_flags->context == NULL) || (event_flags->operations == NULL)
        || (event_flags->operations->clear == NULL) || !event_flags_bits_are_valid(bits)) {
        return false;
    }

    return event_flags->operations->clear(event_flags->context, bits);
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

    if ((event_flags == NULL) || (event_flags->context == NULL) || (event_flags->operations == NULL)
        || (event_flags->operations->wait == NULL) || !event_flags_bits_are_valid(bits)
        || (observed_bits == NULL)) {
        return false;
    }

    return event_flags->operations->wait(
        event_flags->context, bits, wait_all, clear_on_exit, timeout_ms, observed_bits);
}

event_flags_bits_t event_flags_get(const event_flags_t *event_flags)
{
    if ((event_flags == NULL) || (event_flags->context == NULL) || (event_flags->operations == NULL)
        || (event_flags->operations->get == NULL)) {
        return 0U;
    }

    return event_flags->operations->get(event_flags->context) & EVENT_FLAGS_USER_BITS_MASK;
}
