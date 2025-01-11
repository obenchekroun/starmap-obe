cmake_minimum_required(VERSION 3.13)

# Include the Pico SDK initialization script
include($ENV{PICO_SDK_PATH}/pico_sdk_init.cmake)
include(pimoroni_pico_import.cmake)

project(test_display C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

set(PICO_BOARD pico_w)

# Initialize the Pico SDK
pico_sdk_init()

# append flag, best practice, suggested, don't lose previously defined flags
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lm")

# Definitions for compiler
add_definitions(-DLINUX_BUILD=1) # #define USE_DEV_LIB is needed for Waveshare driver to use lgpio library

# Add your executable and source files
add_executable(${PROJECT_NAME}
	pico_display_2_demo.cpp
)

include_directories($ENV{PICO_SDK_PATH}/../pimoroni-pico/drivers/st7789)
include_directories($ENV{PICO_SDK_PATH}/../pimoroni-pico/libraries/pico_display_2)
include_directories($ENV{PICO_SDK_PATH}/../pimoroni-pico/libraries/pico_graphics)
include_directories($ENV{PICO_SDK_PATH}/../pimoroni-pico/common)


# Enable USB stdio and disable UART stdio
pico_enable_stdio_usb(${PROJECT_NAME} 1)
pico_enable_stdio_uart(${PROJECT_NAME} 0)

# Link the Pico SDK to your project
target_link_libraries(${PROJECT_NAME} pico_stdlib hardware_spi hardware_pwm hardware_pio st7789_parallel.pio hardware_dma rgbled button pico_display_2 pico_cyw43_arch_none st7789 pico_graphics)

# Create map/bin/hex/uf2 files
pico_add_extra_outputs(${PROJECT_NAME})
