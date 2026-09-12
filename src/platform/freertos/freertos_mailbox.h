#ifndef PLATFORM_FREERTOS_FREERTOS_MAILBOX_H
#define PLATFORM_FREERTOS_FREERTOS_MAILBOX_H

#include <stdbool.h>
#include <stddef.h>

#include "FreeRTOS.h"
#include "queue.h"

#include "lib/rtos_wrapper/mailbox.h"

typedef struct {
    QueueHandle_t handle;
    StaticQueue_t queue_buffer;
    mailbox_t mailbox;
} freertos_mailbox_t;

bool freertos_mailbox_init(freertos_mailbox_t *mailbox,
                           void *storage,
                           size_t capacity,
                           size_t item_size);
mailbox_t *freertos_mailbox_handle(freertos_mailbox_t *mailbox);

#endif
