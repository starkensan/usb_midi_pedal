#include "unity.h"

#include <string.h>

#include "lib/rtos_wrapper/event_flags.h"
#include "lib/rtos_wrapper/mailbox.h"
#include "lib/rtos_wrapper/mutex.h"
#include "lib/rtos_wrapper/semaphore.h"
#include "lib/rtos_wrapper/state_machine.h"

static BaseType_t queue_result;
static BaseType_t semaphore_result;
static EventBits_t event_bits;
static UBaseType_t queue_count;

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

BaseType_t xQueueSendToBack(QueueHandle_t queue, const void *message, TickType_t timeout)
{
    (void)queue;
    (void)message;
    (void)timeout;
    return queue_result;
}

BaseType_t xQueueReceive(QueueHandle_t queue, void *message, TickType_t timeout)
{
    (void)queue;
    (void)timeout;
    memset(message, 0, sizeof(state_machine_event_t));
    return queue_result;
}

UBaseType_t uxQueueMessagesWaiting(QueueHandle_t queue)
{
    (void)queue;
    return queue_count;
}

SemaphoreHandle_t xSemaphoreCreateMutexStatic(StaticSemaphore_t *semaphore_buffer)
{
    return semaphore_buffer;
}

SemaphoreHandle_t xSemaphoreCreateCountingStatic(UBaseType_t maximum_count,
                                                 UBaseType_t initial_count,
                                                 StaticSemaphore_t *semaphore_buffer)
{
    (void)maximum_count;
    (void)initial_count;
    return semaphore_buffer;
}

BaseType_t xSemaphoreTake(SemaphoreHandle_t semaphore, uint32_t timeout)
{
    (void)semaphore;
    (void)timeout;
    return semaphore_result;
}

BaseType_t xSemaphoreGive(SemaphoreHandle_t semaphore)
{
    (void)semaphore;
    return semaphore_result;
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
    semaphore_result = pdPASS;
    event_bits = 0U;
    queue_count = 0U;
}

void tearDown(void)
{
}

void test_event_flags_reports_invalid_range_timeout_and_value(void)
{
    event_flags_t flags = {0};
    event_flags_bits_t bits = 0U;

    TEST_ASSERT_EQUAL(ERROR_CODE_INVALID_ARGUMENT, event_flags_init(NULL));
    TEST_ASSERT_EQUAL(ERROR_CODE_OK, event_flags_init(&flags));
    TEST_ASSERT_EQUAL(ERROR_CODE_OUT_OF_RANGE, event_flags_set(&flags, 0U));
    TEST_ASSERT_EQUAL(ERROR_CODE_OK, event_flags_set(&flags, UINT32_C(0x01)));
    TEST_ASSERT_EQUAL(ERROR_CODE_OK, event_flags_get(&flags, &bits));
    TEST_ASSERT_EQUAL_UINT32(UINT32_C(0x01), bits);
    TEST_ASSERT_EQUAL(ERROR_CODE_TIMEOUT,
                      event_flags_wait(&flags, UINT32_C(0x02), false, false, 0U, &bits));
}

void test_mailbox_reports_result_codes_and_count(void)
{
    mailbox_t mailbox = {0};
    uint32_t storage[2] = {0};
    uint32_t message = 0U;
    size_t count = 0U;

    TEST_ASSERT_EQUAL(ERROR_CODE_INVALID_ARGUMENT, mailbox_init(NULL, storage, 2U, sizeof(message)));
    TEST_ASSERT_EQUAL(ERROR_CODE_OK, mailbox_init(&mailbox, storage, 2U, sizeof(message)));
    queue_result = pdFAIL;
    TEST_ASSERT_EQUAL(ERROR_CODE_TIMEOUT, mailbox_send(&mailbox, &message, 0U));
    queue_count = 1U;
    TEST_ASSERT_EQUAL(ERROR_CODE_OK, mailbox_message_count(&mailbox, &count));
    TEST_ASSERT_EQUAL_size_t(1U, count);
}

void test_semaphore_and_mutex_report_timeout(void)
{
    semaphore_t semaphore = {0};
    mutex_t mutex = {0};

    TEST_ASSERT_EQUAL(ERROR_CODE_OUT_OF_RANGE, semaphore_init(&semaphore, 0U, 0U));
    TEST_ASSERT_EQUAL(ERROR_CODE_OK, semaphore_init(&semaphore, 1U, 0U));
    TEST_ASSERT_EQUAL(ERROR_CODE_OK, freertos_mutex_init(&mutex));
    semaphore_result = pdFAIL;
    TEST_ASSERT_EQUAL(ERROR_CODE_TIMEOUT, semaphore_take(&semaphore, 0U));
    TEST_ASSERT_EQUAL(ERROR_CODE_TIMEOUT, mutex_lock(&mutex, 0U));
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
    RUN_TEST(test_event_flags_reports_invalid_range_timeout_and_value);
    RUN_TEST(test_mailbox_reports_result_codes_and_count);
    RUN_TEST(test_semaphore_and_mutex_report_timeout);
    RUN_TEST(test_state_machine_propagates_callback_error);
    return UNITY_END();
}
