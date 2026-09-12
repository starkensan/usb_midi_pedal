#include "state_machine.h"

static const state_machine_state_handler_t *find_handler(const state_machine_t *machine,
                                                          state_machine_state_t state)
{
    for (size_t index = 0U; index < machine->handler_count; ++index) {
        if (machine->handlers[index].state == state) {
            return &machine->handlers[index];
        }
    }

    return NULL;
}

static bool handlers_are_valid(const state_machine_state_handler_t *handlers, size_t handler_count)
{
    for (size_t index = 0U; index < handler_count; ++index) {
        if (handlers[index].callback == NULL) {
            return false;
        }
    }

    return true;
}

bool state_machine_init(state_machine_t *machine,
                        mailbox_t *mailbox,
                        event_flags_t *event_flags,
                        event_flags_bits_t state_changed_flags,
                        const state_machine_state_handler_t *handlers,
                        size_t handler_count,
                        state_machine_state_t initial_state)
{
    if ((machine == NULL) || (mailbox == NULL) || (event_flags == NULL) || (handlers == NULL)
        || (handler_count == 0U) || (state_changed_flags == 0U)
        || ((state_changed_flags & ~EVENT_FLAGS_USER_BITS_MASK) != 0U)
        || !handlers_are_valid(handlers, handler_count)) {
        return false;
    }

    machine->mailbox = mailbox;
    machine->event_flags = event_flags;
    machine->state_changed_flags = state_changed_flags;
    machine->handlers = handlers;
    machine->handler_count = handler_count;
    machine->current_state = initial_state;

    return find_handler(machine, initial_state) != NULL;
}

bool state_machine_process_next(state_machine_t *machine, uint32_t timeout_ms)
{
    state_machine_event_t event;
    state_machine_state_t next_state;
    const state_machine_state_handler_t *handler;

    if ((machine == NULL) || (machine->mailbox == NULL) || (machine->event_flags == NULL)
        || (machine->handlers == NULL) || (machine->handler_count == 0U)) {
        return false;
    }

    if (!mailbox_receive(machine->mailbox, &event, timeout_ms)) {
        return false;
    }

    handler = find_handler(machine, machine->current_state);
    if ((handler == NULL) || (handler->callback == NULL)) {
        return false;
    }

    next_state = machine->current_state;
    if (!handler->callback(handler->context, machine->current_state, &event, &next_state)) {
        return false;
    }

    if (next_state == machine->current_state) {
        return true;
    }

    if (find_handler(machine, next_state) == NULL) {
        return false;
    }

    machine->current_state = next_state;
    return event_flags_set(machine->event_flags, machine->state_changed_flags);
}

state_machine_state_t state_machine_current_state(const state_machine_t *machine)
{
    if (machine == NULL) {
        return 0U;
    }

    return machine->current_state;
}
