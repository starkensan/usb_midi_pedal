#include "mailbox.h"

#include "freertos_timeout.h"

error_code_t mailbox_init(mailbox_t *mailbox, void *storage, size_t capacity, size_t item_size)
{
    if ((mailbox == NULL) || (storage == NULL) || (capacity == 0U) || (item_size == 0U)) {
        return ERROR_CODE_INVALID_ARGUMENT;
    }
    if ((capacity > (size_t)((UBaseType_t)-1)) || (item_size > (size_t)((UBaseType_t)-1))
        || (capacity > (SIZE_MAX / item_size))) {
        return ERROR_CODE_OUT_OF_RANGE;
    }

    mailbox->handle = xQueueCreateStatic(
        (UBaseType_t)capacity, (UBaseType_t)item_size, (uint8_t *)storage, &mailbox->queue_buffer);
    return mailbox->handle != NULL ? ERROR_CODE_OK : ERROR_CODE_NOT_READY;
}

error_code_t mailbox_send(mailbox_t *mailbox, const void *message, uint32_t timeout_ms)
{
    if ((mailbox == NULL) || (message == NULL)) {
        return ERROR_CODE_INVALID_ARGUMENT;
    }
    if (mailbox->handle == NULL) {
        return ERROR_CODE_NOT_READY;
    }

    return xQueueSendToBack(mailbox->handle, message, freertos_timeout_ms_to_ticks(timeout_ms)) == pdPASS
               ? ERROR_CODE_OK
               : ERROR_CODE_TIMEOUT;
}

error_code_t mailbox_receive(mailbox_t *mailbox, void *message, uint32_t timeout_ms)
{
    if ((mailbox == NULL) || (message == NULL)) {
        return ERROR_CODE_INVALID_ARGUMENT;
    }
    if (mailbox->handle == NULL) {
        return ERROR_CODE_NOT_READY;
    }

    return xQueueReceive(mailbox->handle, message, freertos_timeout_ms_to_ticks(timeout_ms)) == pdPASS
               ? ERROR_CODE_OK
               : ERROR_CODE_TIMEOUT;
}

error_code_t mailbox_message_count(const mailbox_t *mailbox, size_t *count)
{
    if ((mailbox == NULL) || (count == NULL)) {
        return ERROR_CODE_INVALID_ARGUMENT;
    }
    if (mailbox->handle == NULL) {
        return ERROR_CODE_NOT_READY;
    }

    *count = (size_t)uxQueueMessagesWaiting(mailbox->handle);

    return ERROR_CODE_OK;
}
