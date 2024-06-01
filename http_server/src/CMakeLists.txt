# Add subdirectories for server, image_processing and utils
add_subdirectory(utils)
add_subdirectory(image_processing)
add_subdirectory(server)
add_subdirectory(binary_functions)

# Add the executable
add_executable(MyExecutable main.cpp)


# Add the directory containing the libraries
link_directories(${CMAKE_SOURCE_DIR}/lib)



# Link the executable with the server, image_processing and utils libraries
target_link_libraries(MyExecutable PRIVATE utils image_processing server binary_functions Ws2_32)
