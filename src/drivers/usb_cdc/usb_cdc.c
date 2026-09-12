#include "usb_cdc.h"

#if defined(LOG_CONFIG_OUTPUT_USB_CDC)
#include "pico/stdio_usb.h"
#endif

bool usb_cdc_init(void)
{
#if defined(LOG_CONFIG_OUTPUT_USB_CDC)
    return stdio_usb_init();
#else
    return false;
#endif
}
