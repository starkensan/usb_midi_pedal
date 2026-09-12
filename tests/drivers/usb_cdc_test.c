#include <string.h>

#include "unity.h"

#include "drivers/usb_cdc/usb_cdc.h"

static bool lock_result;
static uint32_t write_result;
static uint32_t write_count;
static uint32_t flush_count;
static uint8_t last_interface_number;
static char last_message[16];

bool usb_midi_tinyusb_try_lock(void)
{
    return lock_result;
}

void usb_midi_tinyusb_unlock(void)
{
}

uint32_t tud_cdc_n_write(uint8_t interface_number, const void *buffer, uint32_t buffer_size)
{
    ++write_count;
    last_interface_number = interface_number;
    (void)memcpy(last_message, buffer, buffer_size);
    return write_result;
}

uint32_t tud_cdc_n_write_flush(uint8_t interface_number)
{
    ++flush_count;
    last_interface_number = interface_number;
    return 0U;
}

void setUp(void)
{
    lock_result = true;
    write_result = 0U;
    write_count = 0U;
    flush_count = 0U;
    last_interface_number = 0U;
    (void)memset(last_message, 0, sizeof(last_message));
}

void tearDown(void)
{
}

void test_usb_cdc_initializes_and_writes_while_holding_tinyusb_lock(void)
{
    static const char message[] = "log";

    write_result = sizeof(message) - 1U;

    TEST_ASSERT_TRUE(usb_cdc_init());
    TEST_ASSERT_TRUE(usb_cdc_write(message, sizeof(message) - 1U));
    TEST_ASSERT_EQUAL_UINT32(1U, write_count);
    TEST_ASSERT_EQUAL_UINT32(1U, flush_count);
    TEST_ASSERT_EQUAL_UINT8(0U, last_interface_number);
    TEST_ASSERT_EQUAL_STRING(message, last_message);
}

void test_usb_cdc_rejects_writes_when_tinyusb_is_busy(void)
{
    lock_result = false;

    TEST_ASSERT_FALSE(usb_cdc_write("log", 3U));
    TEST_ASSERT_EQUAL_UINT32(0U, write_count);
    TEST_ASSERT_EQUAL_UINT32(0U, flush_count);
}

void test_usb_cdc_rejects_partial_writes(void)
{
    write_result = 2U;

    TEST_ASSERT_FALSE(usb_cdc_write("log", 3U));
    TEST_ASSERT_EQUAL_UINT32(1U, write_count);
    TEST_ASSERT_EQUAL_UINT32(0U, flush_count);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_usb_cdc_initializes_and_writes_while_holding_tinyusb_lock);
    RUN_TEST(test_usb_cdc_rejects_writes_when_tinyusb_is_busy);
    RUN_TEST(test_usb_cdc_rejects_partial_writes);
    return UNITY_END();
}
