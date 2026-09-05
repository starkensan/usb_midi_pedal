#ifndef USB_MIDI_PEDAL_ERROR_CODE_H
#define USB_MIDI_PEDAL_ERROR_CODE_H

/**
 * @brief Common result codes returned by application components.
 */
typedef enum {
    ERROR_CODE_OK = 0,
    ERROR_CODE_INVALID_ARGUMENT,
    ERROR_CODE_OUT_OF_RANGE,
    ERROR_CODE_NOT_READY,
    ERROR_CODE_IO,
    ERROR_CODE_TIMEOUT,
    ERROR_CODE_STORAGE,
    ERROR_CODE_UNSUPPORTED,
} error_code_t;

#endif /* USB_MIDI_PEDAL_ERROR_CODE_H */
