#ifndef TEST_MOCK_QUEUE_H
#define TEST_MOCK_QUEUE_H

#include <stdint.h>

#include "FreeRTOS.h"

typedef struct {
    uint32_t unused;
} StaticQueue_t;

typedef void *QueueHandle_t;

QueueHandle_t xQueueCreateStatic(UBaseType_t capacity,
                                 UBaseType_t item_size,
                                 uint8_t *storage,
                                 StaticQueue_t *queue_buffer);
BaseType_t xQueueSendToBack(QueueHandle_t queue, const void *message, TickType_t timeout);
BaseType_t xQueueReceive(QueueHandle_t queue, void *message, TickType_t timeout);
UBaseType_t uxQueueMessagesWaiting(QueueHandle_t queue);

#endif /* TEST_MOCK_QUEUE_H */
