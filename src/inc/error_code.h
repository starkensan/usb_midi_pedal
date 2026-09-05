#ifndef USB_MIDI_PEDAL_ERROR_CODE_H
#define USB_MIDI_PEDAL_ERROR_CODE_H

/**
 * @brief Common result codes returned by application components.
 */
typedef enum {
    ERROR_CODE_OK = 0,
    ERROR_CODE_INVALID_ARGUMENT = -1,
    ERROR_CODE_OUT_OF_RANGE = -2,
    ERROR_CODE_NOT_READY = -3,
    ERROR_CODE_IO = -4,
    ERROR_CODE_TIMEOUT = -5,
    ERROR_CODE_STORAGE = -6,
    ERROR_CODE_UNSUPPORTED = -7,
} error_code_t;

#endif /* USB_MIDI_PEDAL_ERROR_CODE_H */
