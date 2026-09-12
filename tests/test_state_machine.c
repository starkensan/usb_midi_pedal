#include "unity.h"

#include <stdint.h>

#include "state_machine.h"

enum {
    STATE_IDLE = 1U,
    STATE_RUNNING = 2U,
    EVENT_START = 10U,
};

typedef struct {
    state_machine_event_t event;
    bool has_event;
} mailbox_context_t;

typedef struct {
    event_flags_bits_t set_bits;
} event_flags_context_t;

typedef struct {
    uint32_t event_id;
    uintptr_t parameter;
} callback_context_t;

static bool receive_event(void *context, void *message, uint32_t timeout_ms)
{
    mailbox_context_t *mailbox_context = context;

    (void)timeout_ms;
    if (!mailbox_context->has_event) {
        return false;
    }

    *(state_machine_event_t *)message = mailbox_context->event;
    mailbox_context->has_event = false;
    return true;
}

static bool set_flags(void *context, event_flags_bits_t bits)
{
    event_flags_context_t *event_flags_context = context;

    event_flags_context->set_bits |= bits;
    return true;
}

static bool idle_callback(void *context,
                          state_machine_state_t current_state,
                          const state_machine_event_t *event,
                          state_machine_state_t *next_state)
{
    callback_context_t *callback_context = context;

    TEST_ASSERT_EQUAL_UINT32(STATE_IDLE, current_state);
    callback_context->event_id = event->id;
    callback_context->parameter = event->parameter;
    if (event->id == EVENT_START) {
        *next_state = STATE_RUNNING;
    }

    return true;
}

static const mailbox_operations_t mailbox_operations = {
    .receive = receive_event,
};

static const event_flags_operations_t event_flags_operations = {
    .set = set_flags,
};

void setUp(void)
{
}

void tearDown(void)
{
}

void test_state_machine_dispatches_event_and_notifies_state_change(void)
{
    mailbox_context_t mailbox_context = {
        .event = {.id = EVENT_START, .parameter = (uintptr_t)123U},
        .has_event = true,
    };
    event_flags_context_t event_flags_context = {0};
    callback_context_t callback_context = {0};
    mailbox_t mailbox = {.context = &mailbox_context, .operations = &mailbox_operations};
    event_flags_t event_flags = {.context = &event_flags_context, .operations = &event_flags_operations};
    const state_machine_state_handler_t handlers[] = {
        {.state = STATE_IDLE, .callback = idle_callback, .context = &callback_context},
        {.state = STATE_RUNNING, .callback = idle_callback, .context = &callback_context},
    };
    state_machine_t machine;

    TEST_ASSERT_TRUE(state_machine_init(
        &machine, &mailbox, &event_flags, UINT32_C(0x01), handlers, 2U, STATE_IDLE));
    TEST_ASSERT_TRUE(state_machine_process_next(&machine, 0U));
    TEST_ASSERT_EQUAL_UINT32(EVENT_START, callback_context.event_id);
    TEST_ASSERT_EQUAL_UINT(123U, callback_context.parameter);
    TEST_ASSERT_EQUAL_UINT32(STATE_RUNNING, state_machine_current_state(&machine));
    TEST_ASSERT_EQUAL_HEX32(UINT32_C(0x01), event_flags_context.set_bits);
}

void test_state_machine_rejects_invalid_setup_and_empty_mailbox(void)
{
    mailbox_context_t mailbox_context = {0};
    event_flags_context_t event_flags_context = {0};
    mailbox_t mailbox = {.context = &mailbox_context, .operations = &mailbox_operations};
    event_flags_t event_flags = {.context = &event_flags_context, .operations = &event_flags_operations};
    const state_machine_state_handler_t handlers[] = {
        {.state = STATE_IDLE, .callback = idle_callback, .context = NULL},
    };
    state_machine_t machine;

    TEST_ASSERT_FALSE(state_machine_init(
        NULL, &mailbox, &event_flags, UINT32_C(0x01), handlers, 1U, STATE_IDLE));
    TEST_ASSERT_FALSE(state_machine_init(
        &machine, &mailbox, &event_flags, 0U, handlers, 1U, STATE_IDLE));
    TEST_ASSERT_FALSE(state_machine_init(
        &machine, &mailbox, &event_flags, UINT32_C(0x01), handlers, 1U, STATE_RUNNING));
    const state_machine_state_handler_t invalid_handlers[] = {
        {.state = STATE_IDLE, .callback = NULL, .context = NULL},
    };
    TEST_ASSERT_FALSE(state_machine_init(
        &machine, &mailbox, &event_flags, UINT32_C(0x01), invalid_handlers, 1U, STATE_IDLE));
    TEST_ASSERT_TRUE(state_machine_init(
        &machine, &mailbox, &event_flags, UINT32_C(0x01), handlers, 1U, STATE_IDLE));
    TEST_ASSERT_FALSE(state_machine_process_next(&machine, 0U));
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_state_machine_dispatches_event_and_notifies_state_change);
    RUN_TEST(test_state_machine_rejects_invalid_setup_and_empty_mailbox);
    return UNITY_END();
}
