#ifndef RTOS_MAILBOX_H
#define RTOS_MAILBOX_H

#include <stdbool.h>

#include "FreeRTOS.h"
#include "queue.h"

typedef struct {
    QueueHandle_t handle;
    StaticQueue_t queue_buffer;
} rtos_mailbox_t;

bool rtos_mailbox_init(rtos_mailbox_t *mailbox,
                       void *storage,
                       UBaseType_t capacity,
                       UBaseType_t item_size);

bool rtos_mailbox_send(rtos_mailbox_t *mailbox,
                       const void *message,
                       TickType_t timeout);

bool rtos_mailbox_receive(rtos_mailbox_t *mailbox,
                          void *message,
                          TickType_t timeout);

UBaseType_t rtos_mailbox_message_count(const rtos_mailbox_t *mailbox);

#endif
