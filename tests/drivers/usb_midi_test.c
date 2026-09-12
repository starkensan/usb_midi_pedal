#include <string.h>

#include "FreeRTOS.h"
#include "semphr.h"
#include "unity.h"

#include "drivers/usb_midi/usb_midi.h"

static bool init_result;
static bool mounted;
static uint32_t write_result;
static uint32_t write_count;
static uint8_t last_cable_number;
static uint8_t last_message[3];
static uint32_t service_count;
static bool mutex_take_result;
static uint32_t mutex_take_count;
static uint32_t mutex_give_count;

SemaphoreHandle_t xSemaphoreCreateMutexStatic(StaticSemaphore_t *semaphore_buffer)
{
    return semaphore_buffer;
}

BaseType_t xSemaphoreTake(SemaphoreHandle_t semaphore, uint32_t timeout)
{
    (void)semaphore;
    (void)timeout;
    ++mutex_take_count;
    return mutex_take_result ? pdTRUE : pdFALSE;
}

BaseType_t xSemaphoreGive(SemaphoreHandle_t semaphore)
{
    (void)semaphore;
    ++mutex_give_count;
    return pdTRUE;
}

bool tusb_init(void)
{
    return init_result;
}

void tud_task(void)
{
    ++service_count;
}

bool tud_midi_mounted(void)
{
    return mounted;
}

uint32_t tud_midi_stream_write(uint8_t cable_number, const uint8_t *buffer, uint32_t buffer_size)
{
    ++write_count;
    last_cable_number = cable_number;
    (void)memcpy(last_message, buffer, buffer_size);
    return write_result;
}

void setUp(void)
{
    init_result = true;
    mounted = false;
    write_result = 0U;
    write_count = 0U;
    last_cable_number = 0U;
    (void)memset(last_message, 0, sizeof(last_message));
    service_count = 0U;
    mutex_take_result = true;
    mutex_take_count = 0U;
    mutex_give_count = 0U;
}

void tearDown(void)
{
}

void test_usb_midi_initializes_services_and_reports_connection(void)
{
    init_result = false;
    TEST_ASSERT_FALSE(usb_midi_init());

    init_result = true;
    TEST_ASSERT_TRUE(usb_midi_init());

    usb_midi_service();
    TEST_ASSERT_EQUAL_UINT32(1U, service_count);

    TEST_ASSERT_FALSE(usb_midi_is_connected());
    mounted = true;
    TEST_ASSERT_TRUE(usb_midi_is_connected());
}

void test_usb_midi_sends_program_change(void)
{
    write_result = 2U;

    TEST_ASSERT_TRUE(usb_midi_send_program_change(3U, 42U));
    TEST_ASSERT_EQUAL_UINT32(1U, write_count);
    TEST_ASSERT_EQUAL_UINT8(0U, last_cable_number);
    TEST_ASSERT_EQUAL_UINT8(0xC3U, last_message[0]);
    TEST_ASSERT_EQUAL_UINT8(42U, last_message[1]);
}

void test_usb_midi_sends_control_change(void)
{
    write_result = 3U;

    TEST_ASSERT_TRUE(usb_midi_send_control_change(2U, 11U, 100U));
    TEST_ASSERT_EQUAL_UINT32(1U, write_count);
    TEST_ASSERT_EQUAL_UINT8(0U, last_cable_number);
    TEST_ASSERT_EQUAL_UINT8(0xB2U, last_message[0]);
    TEST_ASSERT_EQUAL_UINT8(11U, last_message[1]);
    TEST_ASSERT_EQUAL_UINT8(100U, last_message[2]);
}

void test_usb_midi_serializes_tinyusb_access(void)
{
    TEST_ASSERT_TRUE(usb_midi_init());

    usb_midi_service();
    TEST_ASSERT_EQUAL_UINT32(1U, mutex_take_count);
    TEST_ASSERT_EQUAL_UINT32(1U, mutex_give_count);

    mutex_take_result = false;
    TEST_ASSERT_FALSE(usb_midi_send_program_change(0U, 10U));
    TEST_ASSERT_EQUAL_UINT32(0U, write_count);
    TEST_ASSERT_EQUAL_UINT32(2U, mutex_take_count);
    TEST_ASSERT_EQUAL_UINT32(1U, mutex_give_count);
}

void test_usb_midi_rejects_invalid_messages_and_partial_writes(void)
{
    write_result = 1U;
    TEST_ASSERT_FALSE(usb_midi_send_program_change(0U, 10U));
    TEST_ASSERT_EQUAL_UINT32(1U, write_count);

    TEST_ASSERT_FALSE(usb_midi_send_program_change(16U, 10U));
    TEST_ASSERT_FALSE(usb_midi_send_program_change(0U, 128U));
    TEST_ASSERT_FALSE(usb_midi_send_control_change(16U, 1U, 1U));
    TEST_ASSERT_FALSE(usb_midi_send_control_change(0U, 128U, 1U));
    TEST_ASSERT_FALSE(usb_midi_send_control_change(0U, 1U, 128U));
    TEST_ASSERT_EQUAL_UINT32(1U, write_count);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_usb_midi_initializes_services_and_reports_connection);
    RUN_TEST(test_usb_midi_sends_program_change);
    RUN_TEST(test_usb_midi_sends_control_change);
    RUN_TEST(test_usb_midi_serializes_tinyusb_access);
    RUN_TEST(test_usb_midi_rejects_invalid_messages_and_partial_writes);
    return UNITY_END();
}
