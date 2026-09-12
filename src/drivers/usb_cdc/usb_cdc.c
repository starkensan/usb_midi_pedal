#include "usb_cdc.h"

#include "drivers/usb_midi/usb_midi_tinyusb.h"

#if defined(LOG_CONFIG_OUTPUT_USB_CDC)
#include "tusb.h"
#endif

bool usb_cdc_init(void)
{
#if defined(LOG_CONFIG_OUTPUT_USB_CDC)
    return true;
#else
    return false;
#endif
}

bool usb_cdc_write(const char *message, size_t length)
{
#if defined(LOG_CONFIG_OUTPUT_USB_CDC)
    if (message == NULL || !usb_midi_tinyusb_try_lock()) {
        return false;
    }

    if (tud_cdc_n_write_available(0U) < length) {
        usb_midi_tinyusb_unlock();
        return false;
    }

    const uint32_t written = tud_cdc_n_write(0U, message, (uint32_t)length);
    if (written == length) {
        (void)tud_cdc_n_write_flush(0U);
    }

    usb_midi_tinyusb_unlock();
    return written == length;
#else
    (void)message;
    (void)length;
    return false;
#endif
}
