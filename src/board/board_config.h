#ifndef BOARD_CONFIG_H
#define BOARD_CONFIG_H

#include "hardware/i2c.h"
#include "hardware/uart.h"

/* Shared I2C bus for the OLED, expression ADC, and footswitch expander. */
#define BOARD_I2C_INSTANCE i2c1
#define BOARD_I2C_SDA_PIN 2u
#define BOARD_I2C_SCL_PIN 3u
#define BOARD_I2C_BAUD_RATE_HZ 400000u

#define BOARD_OLED_I2C_ADDRESS 0x3cu
#define BOARD_EXPRESSION_ADC_I2C_ADDRESS 0x48u
#define BOARD_FOOTSWITCH_EXPANDER_I2C_ADDRESS 0x20u

/* ADS1015 ALERT/RDY output; it is asserted on the falling edge. */
#define BOARD_EXPRESSION_ADC_READY_PIN 0u

/* DIN MIDI output. */
#define BOARD_DIN_MIDI_UART_INSTANCE uart1
#define BOARD_DIN_MIDI_UART_TX_PIN 4u

/* Rotary encoder inputs and active-low push button. */
#define BOARD_ENCODER_BUTTON_PIN 5u
#define BOARD_ENCODER_A_PIN 27u
#define BOARD_DEBUG_UART_INSTANCE uart0
#define BOARD_DEBUG_UART_TX_PIN 28u
#define BOARD_ENCODER_B_PIN 29u

#endif /* BOARD_CONFIG_H */
