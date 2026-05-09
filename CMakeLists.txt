cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)

project(rgb_demo C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

add_executable(rgb_demo main.c)

pico_generate_pio_header(rgb_demo ${CMAKE_CURRENT_LIST_DIR}/ws2812.pio)

target_link_libraries(rgb_demo
    pico_stdlib
    hardware_pio
    hardware_adc
)

pico_enable_stdio_usb(rgb_demo 1)
pico_enable_stdio_uart(rgb_demo 0)

pico_add_extra_outputs(rgb_demo)
