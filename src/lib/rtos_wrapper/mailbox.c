#include "mailbox.h"

#include "freertos_timeout.h"

bool mailbox_init(mailbox_t *mailbox, void *storage, size_t capacity, size_t item_size)
{
    if ((mailbox == NULL) || (storage == NULL) || (capacity == 0U) || (item_size == 0U)
        || (capacity > (size_t)((UBaseType_t)-1)) || (item_size > (size_t)((UBaseType_t)-1))) {
        return false;
    }

    mailbox->handle = xQueueCreateStatic(
        (UBaseType_t)capacity, (UBaseType_t)item_size, (uint8_t *)storage, &mailbox->queue_buffer);
    return mailbox->handle != NULL;
}

bool mailbox_send(mailbox_t *mailbox, const void *message, uint32_t timeout_ms)
{
    if ((mailbox == NULL) || (mailbox->handle == NULL) || (message == NULL)) {
        return false;
    }

    return xQueueSendToBack(mailbox->handle, message, freertos_timeout_ms_to_ticks(timeout_ms)) == pdPASS;
}

bool mailbox_receive(mailbox_t *mailbox, void *message, uint32_t timeout_ms)
{
    if ((mailbox == NULL) || (mailbox->handle == NULL) || (message == NULL)) {
        return false;
    }

    return xQueueReceive(mailbox->handle, message, freertos_timeout_ms_to_ticks(timeout_ms)) == pdPASS;
}

size_t mailbox_message_count(const mailbox_t *mailbox)
{
    if ((mailbox == NULL) || (mailbox->handle == NULL)) {
        return 0U;
    }

    return (size_t)uxQueueMessagesWaiting(mailbox->handle);
}
