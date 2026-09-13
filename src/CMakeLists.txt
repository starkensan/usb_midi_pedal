add_executable(usb_midi_pedal
    main.c
    lib/rtos_wrapper/event_flags.c
    lib/rtos_wrapper/mailbox.c
    lib/rtos_wrapper/mutex.c
    lib/rtos_wrapper/semaphore.c
    lib/rtos_wrapper/state_machine.c
    lib/logging/log_level.c
    lib/logging/logging.c
    drivers/log_output/log_output.c
    drivers/rgb_led/rgb_led.c
    drivers/debug_uart/debug_uart.c
    drivers/usb_cdc/usb_cdc.c
    drivers/usb_midi/usb_midi.c
    drivers/usb_midi/usb_midi_descriptors.c
    app/app_controller/app_state_machine.c
    app/tasks/usb_midi_task.c
)

target_include_directories(usb_midi_pedal PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/drivers/debug_uart
    ${CMAKE_CURRENT_LIST_DIR}/drivers/log_output
    ${CMAKE_CURRENT_LIST_DIR}/drivers/usb_cdc
    ${CMAKE_CURRENT_LIST_DIR}/drivers/usb_midi
    ${CMAKE_CURRENT_LIST_DIR}/app/tasks
    ${CMAKE_CURRENT_LIST_DIR}/lib/logging
)

target_link_libraries(usb_midi_pedal PRIVATE
    pico_stdlib
    pico_unique_id
    FreeRTOS-Kernel-Static
    tinyusb_device
    hardware_i2c
    hardware_uart
)

target_include_directories(usb_midi_pedal PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/inc
    ${PROJECT_SOURCE_DIR}/config
)

target_compile_options(usb_midi_pedal PRIVATE
    -Wall
    -Wextra
)

if(LOG_OUTPUT STREQUAL "USB_CDC")
    target_compile_definitions(usb_midi_pedal PRIVATE LOG_CONFIG_OUTPUT_USB_CDC)
else()
    target_compile_definitions(usb_midi_pedal PRIVATE LOG_CONFIG_OUTPUT_UART)
endif()

pico_enable_stdio_usb(usb_midi_pedal 0)
pico_enable_stdio_uart(usb_midi_pedal 0)

if(LOG_LEVEL STREQUAL "ERROR")
    target_compile_definitions(usb_midi_pedal PRIVATE LOG_CONFIG_MIN_LEVEL=LOG_LEVEL_ERROR)
elseif(LOG_LEVEL STREQUAL "WARN")
    target_compile_definitions(usb_midi_pedal PRIVATE LOG_CONFIG_MIN_LEVEL=LOG_LEVEL_WARN)
elseif(LOG_LEVEL STREQUAL "INFO")
    target_compile_definitions(usb_midi_pedal PRIVATE LOG_CONFIG_MIN_LEVEL=LOG_LEVEL_INFO)
else()
    target_compile_definitions(usb_midi_pedal PRIVATE LOG_CONFIG_MIN_LEVEL=LOG_LEVEL_DEBUG)
endif()

pico_add_extra_outputs(usb_midi_pedal)
