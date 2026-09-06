#include "unity.h"

#include "log_level.h"

void setUp(void)
{
}

void tearDown(void)
{
}

void test_log_level_is_valid_accepts_defined_levels(void)
{
    TEST_ASSERT_TRUE(log_level_is_valid(LOG_LEVEL_ERROR));
    TEST_ASSERT_TRUE(log_level_is_valid(LOG_LEVEL_WARN));
    TEST_ASSERT_TRUE(log_level_is_valid(LOG_LEVEL_INFO));
    TEST_ASSERT_TRUE(log_level_is_valid(LOG_LEVEL_DEBUG));
}

void test_log_level_is_valid_rejects_out_of_range_levels(void)
{
    TEST_ASSERT_FALSE(log_level_is_valid((log_level_t)-1));
    TEST_ASSERT_FALSE(log_level_is_valid((log_level_t)(LOG_LEVEL_DEBUG + 1)));
}

void test_log_level_is_enabled_accepts_equal_or_more_severe_levels(void)
{
    TEST_ASSERT_TRUE(log_level_is_enabled(LOG_LEVEL_ERROR, LOG_LEVEL_INFO));
    TEST_ASSERT_TRUE(log_level_is_enabled(LOG_LEVEL_INFO, LOG_LEVEL_INFO));
}

void test_log_level_is_enabled_rejects_more_verbose_or_invalid_levels(void)
{
    TEST_ASSERT_FALSE(log_level_is_enabled(LOG_LEVEL_DEBUG, LOG_LEVEL_INFO));
    TEST_ASSERT_FALSE(log_level_is_enabled((log_level_t)-1, LOG_LEVEL_INFO));
    TEST_ASSERT_FALSE(log_level_is_enabled(LOG_LEVEL_INFO, (log_level_t)-1));
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_log_level_is_valid_accepts_defined_levels);
    RUN_TEST(test_log_level_is_valid_rejects_out_of_range_levels);
    RUN_TEST(test_log_level_is_enabled_accepts_equal_or_more_severe_levels);
    RUN_TEST(test_log_level_is_enabled_rejects_more_verbose_or_invalid_levels);
    return UNITY_END();
}
