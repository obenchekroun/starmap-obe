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

# # Directories with headers of lib
# include_directories(${PROJECT_NAME} ../../lib/display/Config)
# include_directories(${PROJECT_NAME} ../../lib/display/LCD)
# include_directories(${PROJECT_NAME} ../../lib/display/GUI)
# include_directories(${PROJECT_NAME} ../../lib/display/Fonts)

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

# add_executable(${PROJECT_NAME}
#     ../../src/Starmap.cpp
#     ../../src/arrays.cpp
#     # ../../lib/display/Config/Debug.h
#     ../../lib/display/Config/DEV_Config.c
#     ../../lib/display/Config/DEV_Config.h
#     # ../../lib/display/Fonts/fonts.h
#     # ../../lib/display/Fonts/font8.c
#     # ../../lib/display/Fonts/font12.c
#     # ../../lib/display/Fonts/font12CN.c
#     # ../../lib/display/Fonts/font16.c
#     # ../../lib/display/Fonts/font20.c
#     # ../../lib/display/Fonts/font24.c
#     # ../../lib/display/Fonts/font24CN.c
#     # ../../lib/display/Fonts/font48.c
#     # ../../lib/display/Fonts/font50.c
#     # ../../lib/display/GUI/GUI_BMP.h
#     # ../../lib/display/GUI/GUI_BMP.c
#     # ../../lib/display/GUI/GUI_Paint.h
#     # ../../lib/display/GUI/GUI_Paint.c
#     # ../../lib/display/LCD/LCD_0in96.h
#     # ../../lib/display/LCD/LCD_0in96.c
#     # ../../lib/display/LCD/LCD_1in3.h
#     # ../../lib/display/LCD/LCD_1in3.c
#     # ../../lib/display/LCD/LCD_1in8.h
#     # ../../lib/display/LCD/LCD_1in8.c
#     # ../../lib/display/LCD/LCD_1in9.h
#     # ../../lib/display/LCD/LCD_1in9.c
#     # ../../lib/display/LCD/LCD_1in14.h
#     # ../../lib/display/LCD/LCD_1in14.c
#     # ../../lib/display/LCD/LCD_1in28.h
#     # ../../lib/display/LCD/LCD_1in28.c
#     # ../../lib/display/LCD/LCD_1in47.h
#     # ../../lib/display/LCD/LCD_1in47.c
#     # ../../lib/display/LCD/LCD_1in54.h
#     # ../../lib/display/LCD/LCD_1in54.c
#     # ../../lib/display/LCD/LCD_1in69.h
#     # ../../lib/display/LCD/LCD_1in69.c
#     # ../../lib/display/LCD/LCD_2inch.h
#     # ../../lib/display/LCD/LCD_2inch.c
#     # ../../lib/display/LCD/LCD_2inch4.h
#     # ../../lib/display/LCD/LCD_2inch4.c
#     main.cpp
# )

# include_directories(${PROJECT_NAME} ../../lib/display/Config)
# include_directories(${PROJECT_NAME} ../../lib/display/LCD)
# include_directories(${PROJECT_NAME} ../../lib/display/GUI)
# include_directories(${PROJECT_NAME} ../../lib/display/Fonts)

target_include_directories(${PROJECT_NAME} PRIVATE ../../lib/display/Config)
target_include_directories(${PROJECT_NAME} PRIVATE ../../lib/display/LCD)
target_include_directories(${PROJECT_NAME} PRIVATE ../../lib/display/GUI)
target_include_directories(${PROJECT_NAME} PRIVATE ../../lib/display/Fonts)

# Include lib png
find_package(PNG REQUIRED)
include_directories(${PNG_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} ${PNG_LIBRARY})

# Include lib lgpio
target_link_libraries(${PROJECT_NAME} lgpio)

# Include lib math
target_link_libraries(${PROJECT_NAME} m)
