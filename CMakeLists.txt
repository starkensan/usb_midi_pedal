# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.3.0)
set(toolchainVersion 15_2_Rel1)
set(picotoolVersion 2.3.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
cmake_minimum_required(VERSION 3.20)

option(USB_MIDI_PEDAL_BUILD_FIRMWARE "Build the RP2040 firmware" ON)

set(PICO_BOARD pimoroni_tiny2040 CACHE STRING "Target board")
set(PICO_PLATFORM rp2040 CACHE STRING "Target platform")

project(usb_midi_pedal VERSION 0.1.0 LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(USB_MIDI_PEDAL_BUILD_FIRMWARE)
    include(cmake/pico_sdk_import.cmake)
    enable_language(CXX ASM)
    pico_sdk_init()

    add_library(freertos_config INTERFACE)
    target_include_directories(freertos_config SYSTEM INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}/config
    )
    set(FREERTOS_CONFIG_FILE_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/config"
        CACHE PATH "Path to FreeRTOSConfig.h")

    include(cmake/FreeRTOS_Kernel_import.cmake)
    add_subdirectory(src)
else()
    include(CTest)

    if(BUILD_TESTING)
        add_subdirectory(tests)
    endif()
endif()
