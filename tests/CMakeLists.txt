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
    ${CMOCK_GPIO_SOURCE}
    ${CMOCK_GPIO_INCLUDE}
)

target_include_directories(rgb_led_test PRIVATE
    ${CMOCK_OUTPUT_DIRECTORY}
    ${CMAKE_CURRENT_SOURCE_DIR}/mocks
    ${PROJECT_SOURCE_DIR}/external/CMock/src
    ${PROJECT_SOURCE_DIR}/src
)
target_link_libraries(rgb_led_test PRIVATE unity::framework)
target_compile_options(rgb_led_test PRIVATE -Wall -Wextra -Werror)

add_test(NAME rgb_led_test COMMAND rgb_led_test)
