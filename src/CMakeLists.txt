add_executable(usb_midi_pedal
    main.c
    drivers/rgb_led/rgb_led.c
)

target_include_directories(usb_midi_pedal PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
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

pico_enable_stdio_usb(usb_midi_pedal 0)
pico_enable_stdio_uart(usb_midi_pedal 0)
pico_add_extra_outputs(usb_midi_pedal)
