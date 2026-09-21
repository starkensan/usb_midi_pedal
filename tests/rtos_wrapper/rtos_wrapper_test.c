#include "unity.h"

#include <string.h>

#include "lib/rtos_wrapper/event_flags.h"
#include "lib/rtos_wrapper/delay.h"
#include "lib/rtos_wrapper/mailbox.h"
#include "lib/rtos_wrapper/mutex.h"
#include "lib/rtos_wrapper/semaphore.h"
#include "lib/rtos_wrapper/timer.h"

static BaseType_t queue_result;
static BaseType_t semaphore_result;
static EventBits_t event_bits;
static UBaseType_t queue_count;
static BaseType_t timer_command_result;
static BaseType_t timer_create_result;
static TickType_t tick_count;
static TickType_t delayed_ticks;
static TickType_t periodic_delay_ticks;

TimerHandle_t xTimerCreateStatic(const char *name,
                                 TickType_t period,
                                 BaseType_t auto_reload,
                                 void *context,
                                 TimerCallbackFunction_t callback,
                                 StaticTimer_t *timer_buffer)
{
    (void)name;
    (void)period;
    (void)auto_reload;
    (void)context;
    (void)callback;
    return timer_create_result == pdPASS ? timer_buffer : NULL;
}

BaseType_t xTimerStart(TimerHandle_t timer, TickType_t timeout)
{
    (void)timer;
    (void)timeout;
    return timer_command_result;
}

BaseType_t xTimerStop(TimerHandle_t timer, TickType_t timeout)
{
    (void)timer;
    (void)timeout;
    return timer_command_result;
}

BaseType_t xTimerReset(TimerHandle_t timer, TickType_t timeout)
{
    (void)timer;
    (void)timeout;
    return timer_command_result;
}

void vTaskDelay(TickType_t ticks)
{
    delayed_ticks = ticks;
}

TickType_t xTaskGetTickCount(void)
{
    return tick_count;
}

void vTaskDelayUntil(TickType_t *previous_wake_time, TickType_t increment)
{
    periodic_delay_ticks = increment;
    *previous_wake_time += increment;
}

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
    memset(message, 0, sizeof(uint32_t));
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

void setUp(void)
{
    queue_result = pdPASS;
    semaphore_result = pdPASS;
    event_bits = 0U;
    queue_count = 0U;
    timer_command_result = pdPASS;
    timer_create_result = pdPASS;
    tick_count = 0U;
    delayed_ticks = 0U;
    periodic_delay_ticks = 0U;
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

static void timer_callback(TimerHandle_t timer)
{
    (void)timer;
}

void test_timer_initializes_and_reports_command_failure(void)
{
    rtos_timer_t timer = {0};

    TEST_ASSERT_EQUAL(ERROR_CODE_INVALID_ARGUMENT,
                      rtos_timer_init(NULL, "timer", 10U, false, timer_callback, NULL));
    TEST_ASSERT_EQUAL(ERROR_CODE_OUT_OF_RANGE,
                      rtos_timer_init(&timer, "timer", 0U, false, timer_callback, NULL));
    timer_create_result = pdFAIL;
    TEST_ASSERT_EQUAL(ERROR_CODE_NOT_READY,
                      rtos_timer_init(&timer, "timer", 10U, false, timer_callback, NULL));
    timer_create_result = pdPASS;
    TEST_ASSERT_EQUAL(ERROR_CODE_OK,
                      rtos_timer_init(&timer, "timer", 10U, true, timer_callback, NULL));
    TEST_ASSERT_EQUAL(ERROR_CODE_OK, rtos_timer_start(&timer));
    TEST_ASSERT_EQUAL(ERROR_CODE_OK, rtos_timer_stop(&timer));
    TEST_ASSERT_EQUAL(ERROR_CODE_OK, rtos_timer_reset(&timer));

    timer_command_result = pdFAIL;
    TEST_ASSERT_EQUAL(ERROR_CODE_NOT_READY, rtos_timer_start(&timer));
}

void test_delay_waits_for_relative_and_periodic_intervals(void)
{
    rtos_periodic_delay_t periodic_delay = {0};

    TEST_ASSERT_EQUAL(ERROR_CODE_OUT_OF_RANGE, rtos_delay_ms(0U));
    TEST_ASSERT_EQUAL(ERROR_CODE_OK, rtos_delay_ms(10U));
    TEST_ASSERT_EQUAL_UINT32(10U, delayed_ticks);

    TEST_ASSERT_EQUAL(ERROR_CODE_INVALID_ARGUMENT, rtos_periodic_delay_init(NULL));
    tick_count = 100U;
    TEST_ASSERT_EQUAL(ERROR_CODE_OK, rtos_periodic_delay_init(&periodic_delay));
    TEST_ASSERT_EQUAL_UINT32(100U, periodic_delay.last_wake_time);
    TEST_ASSERT_EQUAL(ERROR_CODE_OK, rtos_periodic_delay_wait(&periodic_delay, 20U));
    TEST_ASSERT_EQUAL_UINT32(20U, periodic_delay_ticks);
    TEST_ASSERT_EQUAL_UINT32(120U, periodic_delay.last_wake_time);
    TEST_ASSERT_EQUAL(ERROR_CODE_INVALID_ARGUMENT, rtos_periodic_delay_wait(NULL, 20U));
    TEST_ASSERT_EQUAL(ERROR_CODE_OUT_OF_RANGE, rtos_periodic_delay_wait(&periodic_delay, 0U));
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_event_flags_reports_invalid_range_timeout_and_value);
    RUN_TEST(test_mailbox_reports_result_codes_and_count);
    RUN_TEST(test_semaphore_and_mutex_report_timeout);
    RUN_TEST(test_timer_initializes_and_reports_command_failure);
    RUN_TEST(test_delay_waits_for_relative_and_periodic_intervals);
    return UNITY_END();
}
