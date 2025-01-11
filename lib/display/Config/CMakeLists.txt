# Find all source files in the current directory
# and save the names to the DIR_Config_SRCS variable
aux_source_directory(. DIR_Config_SRCS)

# Generate link libraries
add_library(Config ${DIR_Config_SRCS})
