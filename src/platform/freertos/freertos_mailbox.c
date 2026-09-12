#include "platform/freertos/freertos_mailbox.h"

#include <stdint.h>

static TickType_t timeout_ms_to_ticks(uint32_t timeout_ms)
{
    const uint64_t ticks = ((uint64_t)timeout_ms * (uint64_t)configTICK_RATE_HZ) / UINT64_C(1000);

    if (ticks >= (uint64_t)portMAX_DELAY) {
        return portMAX_DELAY - (TickType_t)1U;
    }

    return (TickType_t)ticks;
}

static bool freertos_mailbox_send(void *context, const void *message, uint32_t timeout_ms)
{
    freertos_mailbox_t *mailbox = context;

    return xQueueSendToBack(mailbox->handle, message, timeout_ms_to_ticks(timeout_ms)) == pdPASS;
}

static bool freertos_mailbox_receive(void *context, void *message, uint32_t timeout_ms)
{
    freertos_mailbox_t *mailbox = context;

    return xQueueReceive(mailbox->handle, message, timeout_ms_to_ticks(timeout_ms)) == pdPASS;
}

static size_t freertos_mailbox_message_count(const void *context)
{
    const freertos_mailbox_t *mailbox = context;

    return (size_t)uxQueueMessagesWaiting(mailbox->handle);
}

static const mailbox_operations_t freertos_mailbox_operations = {
    .send = freertos_mailbox_send,
    .receive = freertos_mailbox_receive,
    .message_count = freertos_mailbox_message_count,
};

bool freertos_mailbox_init(freertos_mailbox_t *mailbox,
                           void *storage,
                           size_t capacity,
                           size_t item_size)
{
    if ((mailbox == NULL) || (storage == NULL) || (capacity == 0U) || (item_size == 0U)
        || (capacity > (size_t)((UBaseType_t)-1)) || (item_size > (size_t)((UBaseType_t)-1))) {
        return false;
    }

    mailbox->handle = xQueueCreateStatic(
        (UBaseType_t)capacity,
        (UBaseType_t)item_size,
        (uint8_t *)storage,
        &mailbox->queue_buffer);
    if (mailbox->handle == NULL) {
        mailbox->mailbox.context = NULL;
        mailbox->mailbox.operations = NULL;
        return false;
    }

    mailbox->mailbox.context = mailbox;
    mailbox->mailbox.operations = &freertos_mailbox_operations;
    return true;
}

mailbox_t *freertos_mailbox_handle(freertos_mailbox_t *mailbox)
{
    if ((mailbox == NULL) || (mailbox->handle == NULL)) {
        return NULL;
    }

    return &mailbox->mailbox;
}
