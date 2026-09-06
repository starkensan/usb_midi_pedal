#include "unity.h"

#include "app_state_machine.h"

static void assert_transition(
    app_state_t current_state,
    app_state_event_t event,
    app_state_t expected_state,
    app_state_action_t expected_action)
{
    app_state_transition_t transition;

    TEST_ASSERT_TRUE(app_state_machine_transition(current_state, event, &transition));
    TEST_ASSERT_EQUAL(expected_state, transition.next_state);
    TEST_ASSERT_EQUAL(expected_action, transition.action);
}

void setUp(void)
{
}

void tearDown(void)
{
}

void test_initial_state_is_boot(void)
{
    TEST_ASSERT_EQUAL(APP_STATE_BOOT, app_state_machine_initial_state());
}

void test_boot_completion_activates_initial_preset(void)
{
    assert_transition(
        APP_STATE_BOOT,
        APP_STATE_EVENT_BOOT_COMPLETED,
        APP_STATE_HOME_ACTIVE,
        APP_STATE_ACTION_APPLY_PRESET);
}

void test_home_active_enters_preset_selection_on_rotation(void)
{
    assert_transition(
        APP_STATE_HOME_ACTIVE,
        APP_STATE_EVENT_ENCODER_ROTATED,
        APP_STATE_HOME_PRESET_SELECT,
        APP_STATE_ACTION_NONE);
}

void test_preset_selection_applies_selected_preset_on_short_press(void)
{
    assert_transition(
        APP_STATE_HOME_PRESET_SELECT,
        APP_STATE_EVENT_ENCODER_SHORT_PRESSED,
        APP_STATE_HOME_APPLYING_PRESET,
        APP_STATE_ACTION_APPLY_PRESET);
}

void test_preset_application_returns_to_active_home(void)
{
    assert_transition(
        APP_STATE_HOME_APPLYING_PRESET,
        APP_STATE_EVENT_PRESET_APPLIED,
        APP_STATE_HOME_ACTIVE,
        APP_STATE_ACTION_NONE);
}

void test_long_press_waits_for_performance_to_stop_before_opening_settings(void)
{
    assert_transition(
        APP_STATE_HOME_ACTIVE,
        APP_STATE_EVENT_ENCODER_LONG_PRESSED,
        APP_STATE_HOME_DISABLING_PERFORMANCE,
        APP_STATE_ACTION_DISABLE_PERFORMANCE);
    assert_transition(
        APP_STATE_HOME_DISABLING_PERFORMANCE,
        APP_STATE_EVENT_PERFORMANCE_DISABLED,
        APP_STATE_SETTINGS_MENU,
        APP_STATE_ACTION_NONE);
}

void test_settings_edit_returns_to_menu_when_confirmed_or_cancelled(void)
{
    assert_transition(
        APP_STATE_SETTINGS_MENU,
        APP_STATE_EVENT_SETTINGS_EDIT_REQUESTED,
        APP_STATE_SETTINGS_EDIT,
        APP_STATE_ACTION_NONE);
    assert_transition(
        APP_STATE_SETTINGS_EDIT,
        APP_STATE_EVENT_SETTINGS_EDIT_CONFIRMED,
        APP_STATE_SETTINGS_MENU,
        APP_STATE_ACTION_NONE);
    assert_transition(
        APP_STATE_SETTINGS_EDIT,
        APP_STATE_EVENT_SETTINGS_EDIT_CANCELLED,
        APP_STATE_SETTINGS_MENU,
        APP_STATE_ACTION_NONE);
}

void test_save_returns_home_on_success_and_keeps_settings_on_failure(void)
{
    assert_transition(
        APP_STATE_SETTINGS_MENU,
        APP_STATE_EVENT_SETTINGS_SAVE_REQUESTED,
        APP_STATE_SETTINGS_SAVE,
        APP_STATE_ACTION_SAVE_AND_APPLY_SETTINGS);
    assert_transition(
        APP_STATE_SETTINGS_SAVE,
        APP_STATE_EVENT_SETTINGS_SAVE_SUCCEEDED,
        APP_STATE_HOME_ACTIVE,
        APP_STATE_ACTION_NONE);
    assert_transition(
        APP_STATE_SETTINGS_SAVE,
        APP_STATE_EVENT_SETTINGS_SAVE_FAILED,
        APP_STATE_SETTINGS_MENU,
        APP_STATE_ACTION_NONE);
}

void test_discard_confirmation_restores_performance(void)
{
    assert_transition(
        APP_STATE_SETTINGS_MENU,
        APP_STATE_EVENT_SETTINGS_DISCARD_REQUESTED,
        APP_STATE_SETTINGS_DISCARD_CONFIRM,
        APP_STATE_ACTION_NONE);
    assert_transition(
        APP_STATE_SETTINGS_DISCARD_CONFIRM,
        APP_STATE_EVENT_SETTINGS_DISCARD_CONFIRMED,
        APP_STATE_HOME_ACTIVE,
        APP_STATE_ACTION_DISCARD_SETTINGS_AND_ENABLE_PERFORMANCE);
}

void test_factory_reset_returns_home_on_success_and_menu_on_failure(void)
{
    assert_transition(
        APP_STATE_SETTINGS_MENU,
        APP_STATE_EVENT_FACTORY_RESET_REQUESTED,
        APP_STATE_SETTINGS_FACTORY_RESET_CONFIRM,
        APP_STATE_ACTION_NONE);
    assert_transition(
        APP_STATE_SETTINGS_FACTORY_RESET_CONFIRM,
        APP_STATE_EVENT_FACTORY_RESET_CONFIRMED,
        APP_STATE_HOME_ACTIVE,
        APP_STATE_ACTION_FACTORY_RESET_AND_ENABLE_PERFORMANCE);
    assert_transition(
        APP_STATE_SETTINGS_FACTORY_RESET_CONFIRM,
        APP_STATE_EVENT_FACTORY_RESET_FAILED,
        APP_STATE_SETTINGS_MENU,
        APP_STATE_ACTION_NONE);
}

void test_invalid_event_keeps_the_callers_transition_unchanged(void)
{
    app_state_transition_t transition = {
        .next_state = APP_STATE_SETTINGS_MENU,
        .action = APP_STATE_ACTION_SAVE_AND_APPLY_SETTINGS,
    };

    TEST_ASSERT_FALSE(app_state_machine_transition(
        APP_STATE_HOME_ACTIVE, APP_STATE_EVENT_SETTINGS_SAVE_REQUESTED, &transition));
    TEST_ASSERT_EQUAL(APP_STATE_SETTINGS_MENU, transition.next_state);
    TEST_ASSERT_EQUAL(APP_STATE_ACTION_SAVE_AND_APPLY_SETTINGS, transition.action);
}

void test_invalid_arguments_are_rejected(void)
{
    app_state_transition_t transition;

    TEST_ASSERT_FALSE(app_state_machine_transition(
        APP_STATE_COUNT, APP_STATE_EVENT_BOOT_COMPLETED, &transition));
    TEST_ASSERT_FALSE(app_state_machine_transition(
        APP_STATE_BOOT, APP_STATE_EVENT_COUNT, &transition));
    TEST_ASSERT_FALSE(app_state_machine_transition(
        APP_STATE_BOOT, APP_STATE_EVENT_BOOT_COMPLETED, NULL));
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_initial_state_is_boot);
    RUN_TEST(test_boot_completion_activates_initial_preset);
    RUN_TEST(test_home_active_enters_preset_selection_on_rotation);
    RUN_TEST(test_preset_selection_applies_selected_preset_on_short_press);
    RUN_TEST(test_preset_application_returns_to_active_home);
    RUN_TEST(test_long_press_waits_for_performance_to_stop_before_opening_settings);
    RUN_TEST(test_settings_edit_returns_to_menu_when_confirmed_or_cancelled);
    RUN_TEST(test_save_returns_home_on_success_and_keeps_settings_on_failure);
    RUN_TEST(test_discard_confirmation_restores_performance);
    RUN_TEST(test_factory_reset_returns_home_on_success_and_menu_on_failure);
    RUN_TEST(test_invalid_event_keeps_the_callers_transition_unchanged);
    RUN_TEST(test_invalid_arguments_are_rejected);
    return UNITY_END();
}
