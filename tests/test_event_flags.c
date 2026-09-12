#include "unity.h"

#include <stdbool.h>
#include <stdint.h>

#include "event_flags.h"

typedef struct {
    event_flags_bits_t bits;
    event_flags_bits_t requested_bits;
    bool wait_all;
    bool clear_on_exit;
    uint32_t timeout_ms;
} event_flags_context_t;

static bool test_set(void *context, event_flags_bits_t bits)
{
    event_flags_context_t *test_context = context;

    test_context->requested_bits = bits;
    test_context->bits |= bits;
    return true;
}

static bool test_clear(void *context, event_flags_bits_t bits)
{
    event_flags_context_t *test_context = context;

    test_context->requested_bits = bits;
    test_context->bits &= ~bits;
    return true;
}

static bool test_wait(void *context,
                      event_flags_bits_t bits,
                      bool wait_all,
                      bool clear_on_exit,
                      uint32_t timeout_ms,
                      event_flags_bits_t *observed_bits)
{
    event_flags_context_t *test_context = context;

    test_context->requested_bits = bits;
    test_context->wait_all = wait_all;
    test_context->clear_on_exit = clear_on_exit;
    test_context->timeout_ms = timeout_ms;
    *observed_bits = test_context->bits;
    return true;
}

static event_flags_bits_t test_get(const void *context)
{
    const event_flags_context_t *test_context = context;

    return test_context->bits;
}

static const event_flags_operations_t test_operations = {
    .set = test_set,
    .clear = test_clear,
    .wait = test_wait,
    .get = test_get,
};

void setUp(void)
{
}

void tearDown(void)
{
}

void test_event_flags_delegates_operations(void)
{
    event_flags_context_t context = {.bits = UINT32_C(0x000003)};
    event_flags_t event_flags = {.context = &context, .operations = &test_operations};
    event_flags_bits_t observed_bits = 0U;

    TEST_ASSERT_TRUE(event_flags_set(&event_flags, UINT32_C(0x000004)));
    TEST_ASSERT_EQUAL_HEX32(UINT32_C(0x000004), context.requested_bits);
    TEST_ASSERT_TRUE(event_flags_clear(&event_flags, UINT32_C(0x000001)));
    TEST_ASSERT_EQUAL_HEX32(UINT32_C(0x000001), context.requested_bits);
    TEST_ASSERT_TRUE(event_flags_wait(
        &event_flags, UINT32_C(0x000006), true, true, 123U, &observed_bits));
    TEST_ASSERT_EQUAL_HEX32(UINT32_C(0x000006), context.requested_bits);
    TEST_ASSERT_TRUE(context.wait_all);
    TEST_ASSERT_TRUE(context.clear_on_exit);
    TEST_ASSERT_EQUAL_UINT32(123U, context.timeout_ms);
    TEST_ASSERT_EQUAL_HEX32(UINT32_C(0x000006), observed_bits);
    TEST_ASSERT_EQUAL_HEX32(UINT32_C(0x000006), event_flags_get(&event_flags));
}

void test_event_flags_rejects_invalid_arguments_and_bits(void)
{
    event_flags_t event_flags = {0};
    event_flags_bits_t observed_bits = UINT32_MAX;

    TEST_ASSERT_FALSE(event_flags_set(NULL, UINT32_C(0x000001)));
    TEST_ASSERT_FALSE(event_flags_set(&event_flags, UINT32_C(0x000001)));
    TEST_ASSERT_FALSE(event_flags_set(&event_flags, 0U));
    TEST_ASSERT_FALSE(event_flags_clear(NULL, UINT32_C(0x000001)));
    TEST_ASSERT_FALSE(event_flags_clear(&event_flags, UINT32_C(0x01000000)));
    TEST_ASSERT_FALSE(event_flags_wait(
        NULL, UINT32_C(0x000001), false, false, 0U, &observed_bits));
    TEST_ASSERT_EQUAL_HEX32(0U, observed_bits);
    TEST_ASSERT_FALSE(event_flags_wait(&event_flags, 0U, false, false, 0U, &observed_bits));
    TEST_ASSERT_FALSE(event_flags_wait(
        &event_flags, UINT32_C(0x000001), false, false, 0U, NULL));
    TEST_ASSERT_EQUAL_HEX32(0U, event_flags_get(NULL));
    TEST_ASSERT_EQUAL_HEX32(0U, event_flags_get(&event_flags));
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_event_flags_delegates_operations);
    RUN_TEST(test_event_flags_rejects_invalid_arguments_and_bits);
    return UNITY_END();
}
