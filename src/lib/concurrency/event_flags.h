#ifndef LIB_CONCURRENCY_EVENT_FLAGS_H
#define LIB_CONCURRENCY_EVENT_FLAGS_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define EVENT_FLAGS_USER_BITS_MASK UINT32_C(0x00ffffff)

typedef uint32_t event_flags_bits_t;

typedef struct event_flags_operations {
    bool (*set)(void *context, event_flags_bits_t bits);
    bool (*clear)(void *context, event_flags_bits_t bits);
    bool (*wait)(void *context,
                 event_flags_bits_t bits,
                 bool wait_all,
                 bool clear_on_exit,
                 uint32_t timeout_ms,
                 event_flags_bits_t *observed_bits);
    event_flags_bits_t (*get)(const void *context);
} event_flags_operations_t;

typedef struct {
    void *context;
    const event_flags_operations_t *operations;
} event_flags_t;

bool event_flags_set(event_flags_t *event_flags, event_flags_bits_t bits);
bool event_flags_clear(event_flags_t *event_flags, event_flags_bits_t bits);
bool event_flags_wait(event_flags_t *event_flags,
                      event_flags_bits_t bits,
                      bool wait_all,
                      bool clear_on_exit,
                      uint32_t timeout_ms,
                      event_flags_bits_t *observed_bits);
event_flags_bits_t event_flags_get(const event_flags_t *event_flags);

#endif
