#include "app/rtos_mailbox/rtos_mailbox.h"

bool rtos_mailbox_init(rtos_mailbox_t *mailbox,
                       void *storage,
                       UBaseType_t capacity,
                       UBaseType_t item_size)
{
    if ((mailbox == NULL) || (storage == NULL) || (capacity == 0U) || (item_size == 0U)) {
        return false;
    }

    mailbox->handle = xQueueCreateStatic(
        capacity,
        item_size,
        (uint8_t *)storage,
        &mailbox->queue_buffer);

    return mailbox->handle != NULL;
}

bool rtos_mailbox_send(rtos_mailbox_t *mailbox,
                       const void *message,
                       TickType_t timeout)
{
    if ((mailbox == NULL) || (mailbox->handle == NULL) || (message == NULL)) {
        return false;
    }

    return xQueueSendToBack(mailbox->handle, message, timeout) == pdPASS;
}

bool rtos_mailbox_receive(rtos_mailbox_t *mailbox,
                          void *message,
                          TickType_t timeout)
{
    if ((mailbox == NULL) || (mailbox->handle == NULL) || (message == NULL)) {
        return false;
    }

    return xQueueReceive(mailbox->handle, message, timeout) == pdPASS;
}

UBaseType_t rtos_mailbox_message_count(const rtos_mailbox_t *mailbox)
{
    if ((mailbox == NULL) || (mailbox->handle == NULL)) {
        return 0U;
    }

    return uxQueueMessagesWaiting(mailbox->handle);
}
