set(PICO_SDK_PATH "${CMAKE_CURRENT_LIST_DIR}/../external/pico-sdk"
    CACHE PATH "Path to the Raspberry Pi Pico SDK" FORCE)

if(NOT EXISTS "${PICO_SDK_PATH}/external/pico_sdk_import.cmake")
    message(FATAL_ERROR
        "Pico SDK was not found. Run: git submodule update --init --recursive")
endif()

include("${PICO_SDK_PATH}/external/pico_sdk_import.cmake")
