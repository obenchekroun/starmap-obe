# Find all source files in the current directory
# and save the names to the DIR_Config_SRCS variable
aux_source_directory(. DIR_LCD_SRCS)

include_directories(../Config)

# Generate link libraries
add_library(LCD ${DIR_LCD_SRCS})
target_link_libraries(LCD PUBLIC Config)
