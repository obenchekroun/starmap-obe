cmake_minimum_required(VERSION 3.22)

# Register library to the system
add_library(lwrb INTERFACE)
target_sources(lwrb INTERFACE ${CMAKE_CURRENT_LIST_DIR}/src/lwrb/lwrb.c)
target_include_directories(lwrb INTERFACE ${CMAKE_CURRENT_LIST_DIR}/src/include)
