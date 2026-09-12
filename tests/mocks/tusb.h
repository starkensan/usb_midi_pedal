#ifndef TEST_MOCK_TUSB_H
#define TEST_MOCK_TUSB_H

#include <stdbool.h>
#include <stdint.h>

bool tusb_init(void);
void tud_task_ext(uint32_t timeout_ms, bool in_isr);
bool tud_midi_mounted(void);
uint32_t tud_midi_stream_write(uint8_t cable_number, const uint8_t *buffer, uint32_t buffer_size);
uint32_t tud_cdc_n_write(uint8_t interface_number, const void *buffer, uint32_t buffer_size);
uint32_t tud_cdc_n_write_flush(uint8_t interface_number);

#endif /* TEST_MOCK_TUSB_H */
