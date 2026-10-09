#include "din_midi.h"

#include "board/board_config.h"
#include "hardware/gpio.h"
#include "hardware/uart.h"

enum {
    DIN_MIDI_BAUD_RATE = 31250U,
    DIN_MIDI_DATA_BITS = 8U,
    DIN_MIDI_STOP_BITS = 1U,
};

static bool din_midi_initialized;

bool din_midi_init(void)
{
    const uint actual_baud_rate = uart_init(BOARD_DIN_MIDI_UART_INSTANCE, DIN_MIDI_BAUD_RATE);
    if (actual_baud_rate == 0U) {
        din_midi_initialized = false;
        return false;
    }

    uart_set_format(BOARD_DIN_MIDI_UART_INSTANCE,
                    DIN_MIDI_DATA_BITS,
                    DIN_MIDI_STOP_BITS,
                    UART_PARITY_NONE);
    gpio_set_function(BOARD_DIN_MIDI_UART_TX_PIN, GPIO_FUNC_UART);
    din_midi_initialized = true;
    return true;
}

bool din_midi_write(const uint8_t *bytes, size_t length)
{
    if (!din_midi_initialized || bytes == NULL || length == 0U) {
        return false;
    }

    uart_write_blocking(BOARD_DIN_MIDI_UART_INSTANCE, bytes, length);
    uart_tx_wait_blocking(BOARD_DIN_MIDI_UART_INSTANCE);
    return true;
}
