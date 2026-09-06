#include "app_state_machine.h"

#include <stddef.h>

app_state_t app_state_machine_initial_state(void)
{
    return APP_STATE_BOOT;
}

bool app_state_machine_transition(
    app_state_t current_state,
    app_state_event_t event,
    app_state_transition_t *transition)
{
    app_state_transition_t result = {
        .next_state = current_state,
        .action = APP_STATE_ACTION_NONE,
    };
    bool handled = true;

    if (transition == NULL || current_state >= APP_STATE_COUNT || event >= APP_STATE_EVENT_COUNT) {
        return false;
    }

    switch (current_state) {
    case APP_STATE_BOOT:
        if (event == APP_STATE_EVENT_BOOT_COMPLETED) {
            result.next_state = APP_STATE_HOME_ACTIVE;
            result.action = APP_STATE_ACTION_APPLY_PRESET;
        } else {
            handled = false;
        }
        break;

    case APP_STATE_HOME_ACTIVE:
        if (event == APP_STATE_EVENT_ENCODER_ROTATED) {
            result.next_state = APP_STATE_HOME_PRESET_SELECT;
        } else if (event == APP_STATE_EVENT_ENCODER_LONG_PRESSED) {
            result.next_state = APP_STATE_HOME_DISABLING_PERFORMANCE;
            result.action = APP_STATE_ACTION_DISABLE_PERFORMANCE;
        } else {
            handled = false;
        }
        break;

    case APP_STATE_HOME_PRESET_SELECT:
        if (event == APP_STATE_EVENT_ENCODER_ROTATED) {
            result.next_state = APP_STATE_HOME_PRESET_SELECT;
        } else if (event == APP_STATE_EVENT_ENCODER_SHORT_PRESSED) {
            result.next_state = APP_STATE_HOME_APPLYING_PRESET;
            result.action = APP_STATE_ACTION_APPLY_PRESET;
        } else if (event == APP_STATE_EVENT_ENCODER_LONG_PRESSED) {
            result.next_state = APP_STATE_HOME_DISABLING_PERFORMANCE;
            result.action = APP_STATE_ACTION_DISABLE_PERFORMANCE;
        } else {
            handled = false;
        }
        break;

    case APP_STATE_HOME_APPLYING_PRESET:
        if (event == APP_STATE_EVENT_PRESET_APPLIED) {
            result.next_state = APP_STATE_HOME_ACTIVE;
        } else {
            handled = false;
        }
        break;

    case APP_STATE_HOME_DISABLING_PERFORMANCE:
        if (event == APP_STATE_EVENT_PERFORMANCE_DISABLED) {
            result.next_state = APP_STATE_SETTINGS_MENU;
        } else {
            handled = false;
        }
        break;

    case APP_STATE_SETTINGS_MENU:
        if (event == APP_STATE_EVENT_SETTINGS_EDIT_REQUESTED) {
            result.next_state = APP_STATE_SETTINGS_EDIT;
        } else if (event == APP_STATE_EVENT_SETTINGS_SAVE_REQUESTED) {
            result.next_state = APP_STATE_SETTINGS_SAVE;
            result.action = APP_STATE_ACTION_SAVE_AND_APPLY_SETTINGS;
        } else if (event == APP_STATE_EVENT_SETTINGS_DISCARD_REQUESTED
            || event == APP_STATE_EVENT_ENCODER_LONG_PRESSED) {
            result.next_state = APP_STATE_SETTINGS_DISCARD_CONFIRM;
        } else if (event == APP_STATE_EVENT_FACTORY_RESET_REQUESTED) {
            result.next_state = APP_STATE_SETTINGS_FACTORY_RESET_CONFIRM;
        } else {
            handled = false;
        }
        break;

    case APP_STATE_SETTINGS_EDIT:
        if (event == APP_STATE_EVENT_SETTINGS_EDIT_CONFIRMED
            || event == APP_STATE_EVENT_SETTINGS_EDIT_CANCELLED) {
            result.next_state = APP_STATE_SETTINGS_MENU;
        } else {
            handled = false;
        }
        break;

    case APP_STATE_SETTINGS_SAVE:
        if (event == APP_STATE_EVENT_SETTINGS_SAVE_SUCCEEDED) {
            result.next_state = APP_STATE_HOME_ACTIVE;
        } else if (event == APP_STATE_EVENT_SETTINGS_SAVE_FAILED) {
            result.next_state = APP_STATE_SETTINGS_MENU;
        } else {
            handled = false;
        }
        break;

    case APP_STATE_SETTINGS_DISCARD_CONFIRM:
        if (event == APP_STATE_EVENT_SETTINGS_DISCARD_CONFIRMED) {
            result.next_state = APP_STATE_HOME_ACTIVE;
            result.action = APP_STATE_ACTION_DISCARD_SETTINGS_AND_ENABLE_PERFORMANCE;
        } else {
            handled = false;
        }
        break;

    case APP_STATE_SETTINGS_FACTORY_RESET_CONFIRM:
        if (event == APP_STATE_EVENT_FACTORY_RESET_CONFIRMED) {
            result.next_state = APP_STATE_HOME_ACTIVE;
            result.action = APP_STATE_ACTION_FACTORY_RESET_AND_ENABLE_PERFORMANCE;
        } else if (event == APP_STATE_EVENT_FACTORY_RESET_FAILED) {
            result.next_state = APP_STATE_SETTINGS_MENU;
        } else {
            handled = false;
        }
        break;

    case APP_STATE_COUNT:
    default:
        handled = false;
        break;
    }

    if (handled) {
        *transition = result;
    }

    return handled;
}
