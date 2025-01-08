# Set minimum required version of CMake
cmake_minimum_required(VERSION 3.13)

# Set name of project (as PROJECT_NAME) and C/C++ standards
project(starmap C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

add_definitions(-DLINUX_BUILD=1)

# append flag, best practice, suggested, don't lose previously defined flags
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

# Tell CMake where to find the executable source file
add_executable(${PROJECT_NAME}
    ../../src/Starmap.cpp
    ../../src/arrays.cpp
    main.cpp
)

#Some flags for error showing
set(CMAKE_CXX_FLAGS_INIT "-Wno-deprecated")

# Create map/bin/hex/uf2 files

find_package(PNG REQUIRED)
include_directories(${PNG_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} ${PNG_LIBRARY})
