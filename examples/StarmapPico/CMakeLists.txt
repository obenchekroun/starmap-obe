cmake_minimum_required(VERSION 3.13)

# Include the Pico SDK initialization script
include($ENV{PICO_SDK_PATH}/pico_sdk_init.cmake)
include($ENV{PICO_SDK_PATH}/../pimoroni-pico/pimoroni_pico_import.cmake)

project(starmap C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialize the Pico SDK
pico_sdk_init()

set(PICO_BOARD pico_w)
#set(WIFI_SSID "<PUT WIFI SSID HERE>")
#set(WIFI_PASSWORD "<PUT WIFI PASSWORD>")

# append flag, best practice, suggested, don't lose previously defined flags
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lm")

# Definitions for compiler
add_definitions(-DLINUX_BUILD=1 -DPICO_CYW43_SUPPORTED=1 -DWITH_NTP=1) # #define USE_DEV_LIB is needed for Waveshare driver to use lgpio library

set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})

# add compiled files subdirectory
add_subdirectory($ENV{PICO_SDK_PATH}/../pimoroni-pico/drivers/st7789 build/st7789) # Pico dislay pack 2 driver
add_subdirectory($ENV{PICO_SDK_PATH}/../pimoroni-pico/libraries/pico_display_2 build/pico_display_2) # Pico dislay pack 2 lib
add_subdirectory($ENV{PICO_SDK_PATH}/../pimoroni-pico/libraries/pico_graphics build/pico_graphics) # Pico graphics lib
add_subdirectory($ENV{PICO_SDK_PATH}/../pimoroni-pico/common build/common) # Common dependencies for pimoroni libraries
add_subdirectory($ENV{PICO_SDK_PATH}/../pimoroni-pico/drivers/rgbled build/rgbled) # Managing buttons of display pack 2
add_subdirectory($ENV{PICO_SDK_PATH}/../pimoroni-pico/drivers/button build/button) # Managing buttons of display pack 2
add_subdirectory(../../lib/pico-ds3231 build/pico-ds3231) # DS3231 lib & drivers
add_subdirectory(../../lib/lwgps build/lwgps) #

include_directories($ENV{PICO_SDK_PATH}/../pimoroni-pico/drivers/st7789)
include_directories($ENV{PICO_SDK_PATH}/../pimoroni-pico/libraries/pico_display_2)
include_directories($ENV{PICO_SDK_PATH}/../pimoroni-pico/libraries/pico_graphics)
include_directories($ENV{PICO_SDK_PATH}/../pimoroni-pico/common)
include_directories($ENV{PICO_SDK_PATH}/../pimoroni-pico/drivers/rgbled)
include_directories($ENV{PICO_SDK_PATH}/../pimoroni-pico/drivers/button)
include_directories(../../lib/pico-ds3231)
include_directories(../../lib/ntp)
include_directories(../../lib/lwgps/src/lwgps)
include_directories(../../lib/lwgps/src/include/lwgps)

# Add your executable and source files
add_executable(${PROJECT_NAME}
    ../../src/Starmap.cpp
    ../../src/arrays.cpp
    main.cpp
)

target_compile_definitions(${PROJECT_NAME} PRIVATE
        WIFI_SSID=\"${WIFI_SSID}\"
        WIFI_PASSWORD=\"${WIFI_PASSWORD}\"
        )

# Enable USB stdio and disable UART stdio
pico_enable_stdio_usb(${PROJECT_NAME} 1)
pico_enable_stdio_uart(${PROJECT_NAME} 0)

# Link the Pico SDK to your project
target_link_libraries(${PROJECT_NAME} pico_stdlib hardware_spi hardware_pwm hardware_pio hardware_dma rgbled button pico_display_2 st7789 pico_graphics hardware_rtc pico_ds3231 pico_cyw43_arch_lwip_threadsafe_background lwgps
)


# Create map/bin/hex/uf2 files
pico_add_extra_outputs(${PROJECT_NAME})
