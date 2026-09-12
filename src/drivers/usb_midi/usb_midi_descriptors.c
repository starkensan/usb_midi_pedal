#include <stddef.h>
#include <string.h>

#include "pico/unique_id.h"
#include "tusb.h"

enum {
    USB_MIDI_INTERFACE_AUDIO_CONTROL = 0U,
    USB_MIDI_INTERFACE_STREAMING,
    USB_MIDI_INTERFACE_COUNT,
    USB_MIDI_ENDPOINT_OUT = 0x01U,
    USB_MIDI_ENDPOINT_IN = 0x81U,
    USB_MIDI_ENDPOINT_SIZE = 64U,
    USB_MIDI_CONFIGURATION_LENGTH = TUD_CONFIG_DESC_LEN + TUD_MIDI_DESC_LEN,
    USB_MIDI_STRING_DESCRIPTOR_MAX_CHARS = 32U,
};

static const tusb_desc_device_t device_descriptor = {
    .bLength = sizeof(tusb_desc_device_t),
    .bDescriptorType = TUSB_DESC_DEVICE,
    .bcdUSB = 0x0200U,
    .bDeviceClass = 0U,
    .bDeviceSubClass = 0U,
    .bDeviceProtocol = 0U,
    .bMaxPacketSize0 = CFG_TUD_ENDPOINT0_SIZE,
    .idVendor = 0x2E8AU,
    .idProduct = 0x000AU,
    .bcdDevice = 0x0100U,
    .iManufacturer = 1U,
    .iProduct = 2U,
    .iSerialNumber = 3U,
    .bNumConfigurations = 1U,
};

static const uint8_t configuration_descriptor[] = {
    TUD_CONFIG_DESCRIPTOR(
        1U,
        USB_MIDI_INTERFACE_COUNT,
        0U,
        USB_MIDI_CONFIGURATION_LENGTH,
        0U,
        100U),
    TUD_MIDI_DESCRIPTOR(
        USB_MIDI_INTERFACE_AUDIO_CONTROL,
        0U,
        USB_MIDI_ENDPOINT_OUT,
        USB_MIDI_ENDPOINT_IN,
        USB_MIDI_ENDPOINT_SIZE),
};

static const char *const string_descriptors[] = {
    (const char[]){0x09, 0x04},
    "USB MIDI Pedal",
    "USB MIDI Pedal",
};

static uint16_t string_descriptor[USB_MIDI_STRING_DESCRIPTOR_MAX_CHARS + 1U];

const uint8_t *tud_descriptor_device_cb(void)
{
    return (const uint8_t *)&device_descriptor;
}

const uint8_t *tud_descriptor_configuration_cb(uint8_t index)
{
    (void)index;
    return configuration_descriptor;
}

const uint16_t *tud_descriptor_string_cb(uint8_t index, uint16_t langid)
{
    size_t character_count;

    (void)langid;

    if (index == 0U) {
        string_descriptor[1] = (uint16_t)string_descriptors[0][0]
            | ((uint16_t)string_descriptors[0][1] << 8U);
        character_count = 1U;
    } else if (index == 3U) {
        pico_get_unique_board_id_string(
            (char *)&string_descriptor[1], USB_MIDI_STRING_DESCRIPTOR_MAX_CHARS);
        character_count = strlen((const char *)&string_descriptor[1]);
        for (size_t position = character_count; position > 0U; --position) {
            string_descriptor[position] = (uint8_t)((const char *)&string_descriptor[1])[position - 1U];
        }
    } else {
        if (index >= (sizeof(string_descriptors) / sizeof(string_descriptors[0]))) {
            return NULL;
        }

        character_count = strlen(string_descriptors[index]);
        if (character_count > USB_MIDI_STRING_DESCRIPTOR_MAX_CHARS) {
            character_count = USB_MIDI_STRING_DESCRIPTOR_MAX_CHARS;
        }

        for (size_t position = 0U; position < character_count; ++position) {
            string_descriptor[position + 1U] = (uint8_t)string_descriptors[index][position];
        }
    }

    string_descriptor[0] = (uint16_t)((TUSB_DESC_STRING << 8U) | (2U * character_count + 2U));
    return string_descriptor;
}
