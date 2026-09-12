#include "unity.h"

#include <stdbool.h>
#include <stdint.h>

#include "mutex.h"

typedef struct {
    uint32_t timeout_ms;
    bool unlocked;
} mutex_context_t;

static bool test_lock(void *context, uint32_t timeout_ms)
{
    mutex_context_t *test_context = context;

    test_context->timeout_ms = timeout_ms;
    return true;
}

static bool test_unlock(void *context)
{
    mutex_context_t *test_context = context;

    test_context->unlocked = true;
    return true;
}

static const mutex_operations_t test_operations = {
    .lock = test_lock,
    .unlock = test_unlock,
};

void setUp(void)
{
}

void tearDown(void)
{
}

void test_mutex_delegates_operations(void)
{
    mutex_context_t context = {0};
    mutex_t mutex = {.context = &context, .operations = &test_operations};

    TEST_ASSERT_TRUE(mutex_lock(&mutex, 123U));
    TEST_ASSERT_EQUAL_UINT32(123U, context.timeout_ms);
    TEST_ASSERT_TRUE(mutex_unlock(&mutex));
    TEST_ASSERT_TRUE(context.unlocked);
}

void test_mutex_rejects_invalid_arguments(void)
{
    mutex_t mutex = {0};

    TEST_ASSERT_FALSE(mutex_lock(NULL, 0U));
    TEST_ASSERT_FALSE(mutex_lock(&mutex, 0U));
    TEST_ASSERT_FALSE(mutex_unlock(NULL));
    TEST_ASSERT_FALSE(mutex_unlock(&mutex));
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_mutex_delegates_operations);
    RUN_TEST(test_mutex_rejects_invalid_arguments);
    return UNITY_END();
}
