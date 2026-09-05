# Select the diagnostic log transport and the minimum emitted severity.
# Valid LOG_OUTPUT values: USB_CDC, UART
# Valid LOG_LEVEL values: ERROR, WARN, INFO, DEBUG
set(LOG_OUTPUT "UART" CACHE STRING "Diagnostic log output transport")
set_property(CACHE LOG_OUTPUT PROPERTY STRINGS USB_CDC UART)

set(LOG_LEVEL "DEBUG" CACHE STRING "Minimum diagnostic log severity")
set_property(CACHE LOG_LEVEL PROPERTY STRINGS ERROR WARN INFO DEBUG)

if(NOT LOG_OUTPUT STREQUAL "USB_CDC" AND NOT LOG_OUTPUT STREQUAL "UART")
    message(FATAL_ERROR "LOG_OUTPUT must be USB_CDC or UART, but was '${LOG_OUTPUT}'")
endif()

if(NOT LOG_LEVEL STREQUAL "ERROR" AND NOT LOG_LEVEL STREQUAL "WARN"
   AND NOT LOG_LEVEL STREQUAL "INFO" AND NOT LOG_LEVEL STREQUAL "DEBUG")
    message(FATAL_ERROR "LOG_LEVEL must be ERROR, WARN, INFO, or DEBUG, but was '${LOG_LEVEL}'")
endif()
