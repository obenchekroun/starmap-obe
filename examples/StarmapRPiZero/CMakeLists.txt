# Set minimum required version of CMake
cmake_minimum_required(VERSION 3.13)

# Set name of project (as PROJECT_NAME) and C/C++ standards
project(starmap C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# append flag, best practice, suggested, don't lose previously defined flags
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lm")

# Definitions for compiler
add_definitions(-DLINUX_BUILD=1 -DUSE_DEV_LIB) #USE_DEV_LIB is needed for Waveshare driver to use lgpio library

# Directories with headers of lib
include_directories(${PROJECT_NAME} ../../lib/display/Config)
include_directories(${PROJECT_NAME} ../../lib/display/LCD)
include_directories(${PROJECT_NAME} ../../lib/display/GUI)
include_directories(${PROJECT_NAME} ../../lib/display/Fonts)

# Tell CMake where to find the executable source file
# Whenever this glob's value changes, cmake will rerun and update the build with the
# new/removed files.
file(GLOB_RECURSE disp_sources CONFIGURE_DEPENDS
    "../../lib/display/Config/*.h"
    "../../lib/display/Config/*.c"
    "../../lib/display/Fonts/*.h"
    "../../lib/display/Fonts/*.c"
    "../../lib/display/GUI/*.h"
    "../../lib/display/GUI/*.c"
    "../../lib/display/LCD/*.h"
    "../../lib/display/LCD/*.c"
)

add_executable(${PROJECT_NAME}
    ../../src/Starmap.cpp
    ../../src/arrays.cpp
    ${disp_sources}
    main.cpp
)

# Include lib png
find_package(PNG REQUIRED)
include_directories(${PNG_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} ${PNG_LIBRARY})

# Include lib lgpio
target_link_libraries(${PROJECT_NAME} lgpio)

# Include lib math
target_link_libraries(${PROJECT_NAME} m)
