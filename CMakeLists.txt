cmake_minimum_required(VERSION 3.0)

#set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY  ~/ClionProjects/RaspberryPiMouse/)

add_subdirectory(src/server)
add_subdirectory(src/client)