#ifndef LIB_RTOS_WAPPER_STATE_MACHINE_H
#define LIB_RTOS_WAPPER_STATE_MACHINE_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "error_code.h"
#include "event_flags.h"
#include "mailbox.h"

typedef uint32_t state_machine_state_t;

typedef struct {
    uint32_t id;
    uintptr_t parameter;
} state_machine_event_t;

typedef error_code_t (*state_machine_callback_t)(void *context,
                                                  state_machine_state_t current_state,
                                                  const state_machine_event_t *event,
                                                  state_machine_state_t *next_state);

typedef struct {
    state_machine_state_t state;
    state_machine_callback_t callback;
    void *context;
} state_machine_state_handler_t;

typedef struct {
    mailbox_t *mailbox;
    event_flags_t *event_flags;
    event_flags_bits_t state_changed_flags;
    const state_machine_state_handler_t *handlers;
    size_t handler_count;
    state_machine_state_t current_state;
} state_machine_t;

error_code_t state_machine_init(state_machine_t *machine,
                                mailbox_t *mailbox,
                                event_flags_t *event_flags,
                                event_flags_bits_t state_changed_flags,
                                const state_machine_state_handler_t *handlers,
                                size_t handler_count,
                                state_machine_state_t initial_state);
error_code_t state_machine_process_next(state_machine_t *machine, uint32_t timeout_ms);
error_code_t state_machine_current_state(const state_machine_t *machine,
                                         state_machine_state_t *state);

#endif
