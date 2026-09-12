#include "unity.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "mailbox.h"

typedef struct {
    const void *sent_message;
    void *received_message;
    uint32_t timeout_ms;
    size_t message_count;
} mailbox_context_t;

static bool test_send(void *context, const void *message, uint32_t timeout_ms)
{
    mailbox_context_t *test_context = context;

    test_context->sent_message = message;
    test_context->timeout_ms = timeout_ms;
    return true;
}

static bool test_receive(void *context, void *message, uint32_t timeout_ms)
{
    mailbox_context_t *test_context = context;

    test_context->received_message = message;
    test_context->timeout_ms = timeout_ms;
    return true;
}

static size_t test_message_count(const void *context)
{
    const mailbox_context_t *test_context = context;

    return test_context->message_count;
}

static const mailbox_operations_t test_operations = {
    .send = test_send,
    .receive = test_receive,
    .message_count = test_message_count,
};

void setUp(void)
{
}

void tearDown(void)
{
}

void test_mailbox_send_delegates_to_implementation(void)
{
    mailbox_context_t context = {0};
    mailbox_t mailbox = {.context = &context, .operations = &test_operations};
    const uint8_t message = 42U;

    TEST_ASSERT_TRUE(mailbox_send(&mailbox, &message, 123U));
    TEST_ASSERT_EQUAL_PTR(&message, context.sent_message);
    TEST_ASSERT_EQUAL_UINT32(123U, context.timeout_ms);
}

void test_mailbox_receive_delegates_to_implementation(void)
{
    mailbox_context_t context = {0};
    mailbox_t mailbox = {.context = &context, .operations = &test_operations};
    uint8_t message = 0U;

    TEST_ASSERT_TRUE(mailbox_receive(&mailbox, &message, 456U));
    TEST_ASSERT_EQUAL_PTR(&message, context.received_message);
    TEST_ASSERT_EQUAL_UINT32(456U, context.timeout_ms);
}

void test_mailbox_message_count_delegates_to_implementation(void)
{
    mailbox_context_t context = {.message_count = 3U};
    mailbox_t mailbox = {.context = &context, .operations = &test_operations};

    TEST_ASSERT_EQUAL_UINT(3U, mailbox_message_count(&mailbox));
}

void test_mailbox_rejects_invalid_arguments(void)
{
    mailbox_t mailbox = {0};
    uint8_t message = 0U;

    TEST_ASSERT_FALSE(mailbox_send(NULL, &message, 0U));
    TEST_ASSERT_FALSE(mailbox_send(&mailbox, &message, 0U));
    TEST_ASSERT_FALSE(mailbox_receive(NULL, &message, 0U));
    TEST_ASSERT_FALSE(mailbox_receive(&mailbox, NULL, 0U));
    TEST_ASSERT_EQUAL_UINT(0U, mailbox_message_count(NULL));
    TEST_ASSERT_EQUAL_UINT(0U, mailbox_message_count(&mailbox));
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_mailbox_send_delegates_to_implementation);
    RUN_TEST(test_mailbox_receive_delegates_to_implementation);
    RUN_TEST(test_mailbox_message_count_delegates_to_implementation);
    RUN_TEST(test_mailbox_rejects_invalid_arguments);
    return UNITY_END();
}
