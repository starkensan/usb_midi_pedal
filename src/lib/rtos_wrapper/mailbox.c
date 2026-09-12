#include "mailbox.h"

bool mailbox_send(mailbox_t *mailbox, const void *message, uint32_t timeout_ms)
{
    if ((mailbox == NULL) || (mailbox->context == NULL) || (mailbox->operations == NULL)
        || (mailbox->operations->send == NULL) || (message == NULL)) {
        return false;
    }

    return mailbox->operations->send(mailbox->context, message, timeout_ms);
}

bool mailbox_receive(mailbox_t *mailbox, void *message, uint32_t timeout_ms)
{
    if ((mailbox == NULL) || (mailbox->context == NULL) || (mailbox->operations == NULL)
        || (mailbox->operations->receive == NULL) || (message == NULL)) {
        return false;
    }

    return mailbox->operations->receive(mailbox->context, message, timeout_ms);
}

size_t mailbox_message_count(const mailbox_t *mailbox)
{
    if ((mailbox == NULL) || (mailbox->context == NULL) || (mailbox->operations == NULL)
        || (mailbox->operations->message_count == NULL)) {
        return 0U;
    }

    return mailbox->operations->message_count(mailbox->context);
}
