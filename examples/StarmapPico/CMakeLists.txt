cmake_minimum_required(VERSION 3.13)

# Needs to be done before including SDK pico
# https://forums.raspberrypi.com/viewtopic.php?t=376508
set(PICO_BOARD pico2_w)
set(PICO_PLATFORM rp2350)

# Set if want to use ntp/gps/save last location to flash memory
set(WITH_NTP 0)
set(WITH_GPS 1)
set(WITH_SAVE_LOCATION 1)

# Include the Pico SDK initialization script
include($ENV{PICO_SDK_PATH}/pico_sdk_init.cmake)
include($ENV{PICO_SDK_PATH}/../pimoroni-pico/pimoroni_pico_import.cmake)

project(starmap C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialize the Pico SDK
pico_sdk_init()

# append flag, best practice, suggested, don't lose previously defined flags
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lm")


# Definitions for compiler
add_compile_definitions(
  LINUX_BUILD=1
  #USE_DEV_LIB #define USE_DEV_LIB is needed for Waveshare driver to use lgpio library
)
if(WITH_NTP)
    add_compile_definitions(
        WITH_NTP
    )
endif()

if(WITH_GPS)
        add_compile_definitions(
            WITH_GPS
            LWRB_DISABLE_ATOMIC # #define LWRB_DISABLE_ATOMIC as not implemented to pico (for lwrb)
    )
endif()

if(WITH_SAVE_LOCATION)
        add_compile_definitions(
            WITH_SAVE_LOCATION
            LWRB_DISABLE_ATOMIC # #define LWRB_DISABLE_ATOMIC as not implemented to pico (for lwrb)
    )
endif()

set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})

# add compiled files subdirectory
add_subdirectory($ENV{PICO_SDK_PATH}/../pimoroni-pico/drivers/st7789 build/st7789) # Pico dislay pack 2 driver
add_subdirectory($ENV{PICO_SDK_PATH}/../pimoroni-pico/libraries/pico_display_2 build/pico_display_2) # Pico dislay pack 2 lib
add_subdirectory($ENV{PICO_SDK_PATH}/../pimoroni-pico/libraries/pico_graphics build/pico_graphics) # Pico graphics lib
add_subdirectory($ENV{PICO_SDK_PATH}/../pimoroni-pico/common build/common) # Common dependencies for pimoroni libraries
add_subdirectory($ENV{PICO_SDK_PATH}/../pimoroni-pico/drivers/rgbled build/rgbled) # Managing led of display pack 2
add_subdirectory($ENV{PICO_SDK_PATH}/../pimoroni-pico/drivers/button build/button) # Managing buttons of display pack 2
add_subdirectory(../../lib/pico-ds3231 build/pico-ds3231) # DS3231 lib & drivers
add_subdirectory(../../lib/lwgps build/lwgps) # gps NMEA parser
add_subdirectory(../../lib/lwrb build/lwrb) #ring buffer for the gps parser

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
include_directories(../../lib/lwrb/src/lwrb)
include_directories(../../lib/lwrb/src/include/lwrb)

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
set(my_lib
     pico_stdlib
     hardware_spi
     hardware_pwm
     hardware_pio
     hardware_dma
     rgbled
     button
     pico_display_2
     st7789
     pico_graphics
     pico_aon_timer #instead of hardware_rtc so the code works both for RP2040 and RP2350
     pico_ds3231
     #pico_cyw43_arch_lwip_threadsafe_background
)

if(WITH_NTP)
    set(my_lib ${my_lib} pico_cyw43_arch_lwip_threadsafe_background)
endif()

if(WITH_GPS)
    set(my_lib ${my_lib} lwgps lwrb)
endif()

if(WITH_SAVE_LOCATION)
    set(my_lib ${my_lib} hardware_flash)
endif()

target_link_libraries(${PROJECT_NAME} ${my_lib})


# Create map/bin/hex/uf2 files
pico_add_extra_outputs(${PROJECT_NAME})
