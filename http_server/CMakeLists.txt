cmake_minimum_required(VERSION 3.5)

project(TicTacToe)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(include)

# Add subdirectories
add_subdirectory(src)
#add_subdirectory(lib)




