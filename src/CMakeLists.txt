add_executable(usb_midi_pedal
    main.c
    app/logging/logging.c
    drivers/debug_uart/debug_uart.c
)

target_include_directories(usb_midi_pedal PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/app/logging
    ${CMAKE_CURRENT_LIST_DIR}/drivers/debug_uart
)

target_link_libraries(usb_midi_pedal PRIVATE
    pico_stdlib
    FreeRTOS-Kernel-Static
    hardware_i2c
    hardware_uart
)

target_include_directories(usb_midi_pedal PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/inc
)

target_compile_options(usb_midi_pedal PRIVATE
    -Wall
    -Wextra
)

if(LOG_OUTPUT STREQUAL "USB_CDC")
    target_compile_definitions(usb_midi_pedal PRIVATE LOG_CONFIG_OUTPUT_USB_CDC)
    pico_enable_stdio_usb(usb_midi_pedal 1)
    pico_enable_stdio_uart(usb_midi_pedal 0)
else()
    target_compile_definitions(usb_midi_pedal PRIVATE LOG_CONFIG_OUTPUT_UART)
    pico_enable_stdio_usb(usb_midi_pedal 0)
    pico_enable_stdio_uart(usb_midi_pedal 0)
endif()

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
