#include "unity.h"

#include <stdbool.h>
#include <stdint.h>

#include "semaphore.h"

typedef struct {
    uint32_t timeout_ms;
    bool gave;
} semaphore_context_t;

static bool test_give(void *context)
{
    semaphore_context_t *test_context = context;

    test_context->gave = true;
    return true;
}

static bool test_take(void *context, uint32_t timeout_ms)
{
    semaphore_context_t *test_context = context;

    test_context->timeout_ms = timeout_ms;
    return true;
}

static const semaphore_operations_t test_operations = {
    .give = test_give,
    .take = test_take,
};

void setUp(void)
{
}

void tearDown(void)
{
}

void test_semaphore_delegates_operations(void)
{
    semaphore_context_t context = {0};
    semaphore_t semaphore = {.context = &context, .operations = &test_operations};

    TEST_ASSERT_TRUE(semaphore_give(&semaphore));
    TEST_ASSERT_TRUE(context.gave);
    TEST_ASSERT_TRUE(semaphore_take(&semaphore, 123U));
    TEST_ASSERT_EQUAL_UINT32(123U, context.timeout_ms);
}

void test_semaphore_rejects_invalid_arguments(void)
{
    semaphore_t semaphore = {0};

    TEST_ASSERT_FALSE(semaphore_give(NULL));
    TEST_ASSERT_FALSE(semaphore_give(&semaphore));
    TEST_ASSERT_FALSE(semaphore_take(NULL, 0U));
    TEST_ASSERT_FALSE(semaphore_take(&semaphore, 0U));
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_semaphore_delegates_operations);
    RUN_TEST(test_semaphore_rejects_invalid_arguments);
    return UNITY_END();
}
