find_package(Ruby 3.0 REQUIRED)

add_subdirectory(${PROJECT_SOURCE_DIR}/external/Unity
    ${CMAKE_CURRENT_BINARY_DIR}/unity)

set(CMOCK_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/generated)
set(CMOCK_GPIO_HEADER ${CMAKE_CURRENT_SOURCE_DIR}/mocks/hardware/gpio.h)
set(CMOCK_GPIO_SOURCE ${CMOCK_OUTPUT_DIRECTORY}/Mockgpio.c)
set(CMOCK_GPIO_INCLUDE ${CMOCK_OUTPUT_DIRECTORY}/Mockgpio.h)

add_custom_command(
    OUTPUT ${CMOCK_GPIO_SOURCE} ${CMOCK_GPIO_INCLUDE}
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMOCK_OUTPUT_DIRECTORY}
    COMMAND ${Ruby_EXECUTABLE} ${PROJECT_SOURCE_DIR}/external/CMock/lib/cmock.rb
        ${CMOCK_GPIO_HEADER}
        --mock_path=${CMOCK_OUTPUT_DIRECTORY}
    DEPENDS ${CMOCK_GPIO_HEADER} ${PROJECT_SOURCE_DIR}/external/CMock/lib/cmock.rb
    VERBATIM
)

add_executable(rgb_led_test
    drivers/rgb_led_test.c
    ${PROJECT_SOURCE_DIR}/src/drivers/rgb_led/rgb_led.c
    ${PROJECT_SOURCE_DIR}/external/CMock/src/cmock.c
    ${CMOCK_GPIO_SOURCE}
    ${CMOCK_GPIO_INCLUDE}
)

target_include_directories(rgb_led_test PRIVATE
    ${CMOCK_OUTPUT_DIRECTORY}
    ${CMAKE_CURRENT_SOURCE_DIR}/mocks
    ${CMAKE_CURRENT_SOURCE_DIR}/mocks/hardware
    ${PROJECT_SOURCE_DIR}/external/CMock/src
    ${PROJECT_SOURCE_DIR}/src
)
target_link_libraries(rgb_led_test PRIVATE unity::framework)
target_compile_options(rgb_led_test PRIVATE -Wall -Wextra -Werror)

add_test(NAME rgb_led_test COMMAND rgb_led_test)

add_executable(usb_midi_test
    drivers/usb_midi_test.c
    ${PROJECT_SOURCE_DIR}/src/drivers/usb_midi/usb_midi.c
)
target_include_directories(usb_midi_test PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/mocks
    ${PROJECT_SOURCE_DIR}/src
)
target_link_libraries(usb_midi_test PRIVATE unity::framework)
target_compile_options(usb_midi_test PRIVATE -Wall -Wextra -Werror)

add_test(NAME usb_midi_test COMMAND usb_midi_test)

add_executable(usb_cdc_test
    drivers/usb_cdc_test.c
    ${PROJECT_SOURCE_DIR}/src/drivers/usb_cdc/usb_cdc.c
)
target_include_directories(usb_cdc_test PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/mocks
    ${PROJECT_SOURCE_DIR}/src
)
target_compile_definitions(usb_cdc_test PRIVATE LOG_CONFIG_OUTPUT_USB_CDC)
target_link_libraries(usb_cdc_test PRIVATE unity::framework)
target_compile_options(usb_cdc_test PRIVATE -Wall -Wextra -Werror)

add_test(NAME usb_cdc_test COMMAND usb_cdc_test)

add_executable(test_log_level
    test_log_level.c
    ${PROJECT_SOURCE_DIR}/src/lib/logging/log_level.c
)

target_include_directories(test_log_level PRIVATE
    ${PROJECT_SOURCE_DIR}/src/lib/logging
)
target_link_libraries(test_log_level PRIVATE unity::framework)
target_compile_options(test_log_level PRIVATE -Wall -Wextra)

add_test(NAME test_log_level COMMAND test_log_level)

add_executable(test_mailbox
    test_mailbox.c
    ${PROJECT_SOURCE_DIR}/src/lib/concurrency/mailbox.c
)
target_include_directories(test_mailbox PRIVATE
    ${PROJECT_SOURCE_DIR}/src/lib/concurrency
)
target_link_libraries(test_mailbox PRIVATE unity::framework)
target_compile_options(test_mailbox PRIVATE -Wall -Wextra -Werror)

add_test(NAME test_mailbox COMMAND test_mailbox)

add_executable(test_event_flags
    test_event_flags.c
    ${PROJECT_SOURCE_DIR}/src/lib/concurrency/event_flags.c
)
target_include_directories(test_event_flags PRIVATE
    ${PROJECT_SOURCE_DIR}/src/lib/concurrency
)
target_link_libraries(test_event_flags PRIVATE unity::framework)
target_compile_options(test_event_flags PRIVATE -Wall -Wextra -Werror)

add_test(NAME test_event_flags COMMAND test_event_flags)

add_executable(test_semaphore
    test_semaphore.c
    ${PROJECT_SOURCE_DIR}/src/lib/concurrency/semaphore.c
)
target_include_directories(test_semaphore PRIVATE
    ${PROJECT_SOURCE_DIR}/src/lib/concurrency
)
target_link_libraries(test_semaphore PRIVATE unity::framework)
target_compile_options(test_semaphore PRIVATE -Wall -Wextra -Werror)

add_test(NAME test_semaphore COMMAND test_semaphore)

add_executable(test_mutex
    test_mutex.c
    ${PROJECT_SOURCE_DIR}/src/lib/concurrency/mutex.c
)
target_include_directories(test_mutex PRIVATE
    ${PROJECT_SOURCE_DIR}/src/lib/concurrency
)
target_link_libraries(test_mutex PRIVATE unity::framework)
target_compile_options(test_mutex PRIVATE -Wall -Wextra -Werror)

add_test(NAME test_mutex COMMAND test_mutex)

add_executable(app_state_machine_test
    app/app_state_machine_test.c
    ${PROJECT_SOURCE_DIR}/src/app/app_controller/app_state_machine.c
)
target_include_directories(app_state_machine_test PRIVATE
    ${PROJECT_SOURCE_DIR}/src/app/app_controller
)
target_link_libraries(app_state_machine_test PRIVATE unity::framework)
target_compile_options(app_state_machine_test PRIVATE -Wall -Wextra -Werror)

add_test(NAME app_state_machine_test COMMAND app_state_machine_test)
