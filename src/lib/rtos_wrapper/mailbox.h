#ifndef LIB_CONCURRENCY_MAILBOX_H
#define LIB_CONCURRENCY_MAILBOX_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "FreeRTOS.h"
#include "error_code.h"
#include "queue.h"

typedef struct {
    QueueHandle_t handle;
    StaticQueue_t queue_buffer;
} mailbox_t;

error_code_t mailbox_init(mailbox_t *mailbox, void *storage, size_t capacity, size_t item_size);
error_code_t mailbox_send(mailbox_t *mailbox, const void *message, uint32_t timeout_ms);
error_code_t mailbox_receive(mailbox_t *mailbox, void *message, uint32_t timeout_ms);
error_code_t mailbox_message_count(const mailbox_t *mailbox, size_t *count);

#endif
