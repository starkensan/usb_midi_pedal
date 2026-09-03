set(FREERTOS_KERNEL_PATH "${CMAKE_CURRENT_LIST_DIR}/../external/FreeRTOS-Kernel"
    CACHE PATH "Path to the FreeRTOS Kernel" FORCE)

set(_FREERTOS_IMPORT
    "${FREERTOS_KERNEL_PATH}/portable/ThirdParty/GCC/RP2040/FreeRTOS_Kernel_import.cmake")

if(NOT EXISTS "${_FREERTOS_IMPORT}")
    message(FATAL_ERROR
        "FreeRTOS Kernel was not found. Run: git submodule update --init --recursive")
endif()

include("${_FREERTOS_IMPORT}")
unset(_FREERTOS_IMPORT)
