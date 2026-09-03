cmake_minimum_required(VERSION 3.20)

set(PICO_BOARD pimoroni_tiny2040 CACHE STRING "Target board")
set(PICO_PLATFORM rp2040 CACHE STRING "Target platform")

include(cmake/pico_sdk_import.cmake)

project(usb_midi_pedal VERSION 0.1.0 LANGUAGES C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

pico_sdk_init()

add_library(freertos_config INTERFACE)
target_include_directories(freertos_config SYSTEM INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/config
)
set(FREERTOS_CONFIG_FILE_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/config"
    CACHE PATH "Path to FreeRTOSConfig.h")

include(cmake/FreeRTOS_Kernel_import.cmake)

add_subdirectory(src)
