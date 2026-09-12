#ifndef LIB_CONCURRENCY_MAILBOX_H
#define LIB_CONCURRENCY_MAILBOX_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct mailbox_operations {
    bool (*send)(void *context, const void *message, uint32_t timeout_ms);
    bool (*receive)(void *context, void *message, uint32_t timeout_ms);
    size_t (*message_count)(const void *context);
} mailbox_operations_t;

typedef struct {
    void *context;
    const mailbox_operations_t *operations;
} mailbox_t;

bool mailbox_send(mailbox_t *mailbox, const void *message, uint32_t timeout_ms);
bool mailbox_receive(mailbox_t *mailbox, void *message, uint32_t timeout_ms);
size_t mailbox_message_count(const mailbox_t *mailbox);

#endif
