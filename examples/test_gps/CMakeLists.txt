cmake_minimum_required(VERSION 3.13)

# Include the Pico SDK initialization script
include($ENV{PICO_SDK_PATH}/pico_sdk_init.cmake)
include($ENV{PICO_SDK_PATH}/../pimoroni-pico/pimoroni_pico_import.cmake)

project(uart_gps C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialize the Pico SDK
pico_sdk_init()

set(PICO_BOARD pico_w)
#set(PICO_BOARD pico2_w)
#set(PICO_PLATFORM rp2350)
#set(WIFI_SSID "<PUT WIFI SSID HERE>")
#set(WIFI_PASSWORD "<PUT WIFI PASSWORD>")

# append flag, best practice, suggested, don't lose previously defined flags
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lm")

# Definitions for compiler
add_definitions(-DLINUX_BUILD=1 -DPICO_CYW43_SUPPORTED=1 -DWITH_NTP=1 -DLWRB_DISABLE_ATOMIC)
# #define USE_DEV_LIB is needed for Waveshare driver to use lgpio library
# #define WITH_NTP to use NTP
# #define LWRB_DISABLE_ATOMIC as not implemented to pico

set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})

# add compiled files subdirectory
add_subdirectory(../../lib/lwgps build/lwgps) #
add_subdirectory(../../lib/lwrb build/lwrb) #

include_directories(../../lib/lwgps/src/lwgps)
include_directories(../../lib/lwgps/src/include/lwgps)

#include_directories(../../lib/lwrb/src/lwrb)
#include_directories(../../lib/lwrb/src/include/lwrb)

# Add your executable and source files
add_executable(${PROJECT_NAME}
    main.cpp
)


# Enable USB stdio and disable UART stdio
pico_enable_stdio_usb(${PROJECT_NAME} 1)
pico_enable_stdio_uart(${PROJECT_NAME} 0)

# Link the Pico SDK to your project
target_link_libraries(${PROJECT_NAME} pico_stdlib hardware_pio lwgps lwrb
)


# Create map/bin/hex/uf2 files
pico_add_extra_outputs(${PROJECT_NAME})
