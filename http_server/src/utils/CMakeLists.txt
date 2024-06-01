# Create a library from logger.cpp
add_library(utils logger.cpp)

# Include directories
#target_include_directories(utils PRIVATE ${CMAKE_SOURCE_DIR}/include/utils)