#include "unity.h"

#include <string.h>

#include "lib/rtos_wrapper/event_flags.h"
#include "lib/rtos_wrapper/mailbox.h"
#include "lib/state_machine/state_machine.h"

static BaseType_t queue_result;
static EventBits_t event_bits;

EventGroupHandle_t xEventGroupCreateStatic(StaticEventGroup_t *event_group_buffer)
{
    return event_group_buffer;
}

EventBits_t xEventGroupSetBits(EventGroupHandle_t event_group, EventBits_t bits)
{
    (void)event_group;
    event_bits |= bits;
    return event_bits;
}

EventBits_t xEventGroupClearBits(EventGroupHandle_t event_group, EventBits_t bits)
{
    (void)event_group;
    event_bits &= ~bits;
    return event_bits;
}

EventBits_t xEventGroupWaitBits(EventGroupHandle_t event_group,
                                EventBits_t bits_to_wait_for,
                                BaseType_t clear_on_exit,
                                BaseType_t wait_for_all,
                                TickType_t timeout)
{
    (void)event_group;
    (void)bits_to_wait_for;
    (void)clear_on_exit;
    (void)wait_for_all;
    (void)timeout;
    return event_bits;
}

EventBits_t xEventGroupGetBits(EventGroupHandle_t event_group)
{
    (void)event_group;
    return event_bits;
}

QueueHandle_t xQueueCreateStatic(UBaseType_t capacity,
                                 UBaseType_t item_size,
                                 uint8_t *storage,
                                 StaticQueue_t *queue_buffer)
{
    (void)capacity;
    (void)item_size;
    (void)storage;
    return queue_buffer;
}

BaseType_t xQueueReceive(QueueHandle_t queue, void *message, TickType_t timeout)
{
    (void)queue;
    (void)timeout;
    memset(message, 0, sizeof(state_machine_event_t));
    return queue_result;
}

BaseType_t xQueueSendToBack(QueueHandle_t queue, const void *message, TickType_t timeout)
{
    (void)queue;
    (void)message;
    (void)timeout;
    return queue_result;
}

UBaseType_t uxQueueMessagesWaiting(QueueHandle_t queue)
{
    (void)queue;
    return 0U;
}

static error_code_t state_handler(void *context,
                                  state_machine_state_t current_state,
                                  const state_machine_event_t *event,
                                  state_machine_state_t *next_state)
{
    (void)context;
    (void)current_state;
    (void)event;
    (void)next_state;
    return ERROR_CODE_IO;
}

void setUp(void)
{
    queue_result = pdPASS;
    event_bits = 0U;
}

void tearDown(void)
{
}

void test_state_machine_propagates_callback_error(void)
{
    mailbox_t mailbox = {0};
    event_flags_t flags = {0};
    uint32_t storage = 0U;
    const state_machine_state_handler_t handlers[] = {
        {.state = 0U, .callback = state_handler, .context = NULL},
    };
    state_machine_t machine = {0};

    TEST_ASSERT_EQUAL(ERROR_CODE_OK, mailbox_init(&mailbox, &storage, 1U, sizeof(storage)));
    TEST_ASSERT_EQUAL(ERROR_CODE_OK, event_flags_init(&flags));
    TEST_ASSERT_EQUAL(ERROR_CODE_OK,
                      state_machine_init(&machine, &mailbox, &flags, UINT32_C(0x01), handlers, 1U, 0U));
    TEST_ASSERT_EQUAL(ERROR_CODE_IO, state_machine_process_next(&machine, 0U));
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_state_machine_propagates_callback_error);
    return UNITY_END();
}
