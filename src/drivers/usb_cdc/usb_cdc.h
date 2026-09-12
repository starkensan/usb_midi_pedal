#ifndef DRIVERS_USB_CDC_USB_CDC_H
#define DRIVERS_USB_CDC_USB_CDC_H

#include <stdbool.h>
#include <stddef.h>

bool usb_cdc_init(void);
bool usb_cdc_write(const char *message, size_t length);

#endif
