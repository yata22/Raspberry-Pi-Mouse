cmake_minimum_required(VERSION 3.0)

add_subdirectory(RTIMULib_connector)

project(client)

set(EXEC_NAME Client)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c11")

set(CLIENT_SOURCE_FILES
        main.c
        socket/client.c
        socket/client.h
        socket/multicastClient.c
        socket/multicastClient.h
        joystick/joystick.c
        joystick/joystick.h
        mouse/mouseClient.c
        mouse/mouseClient.h
        fifo/fifo.c
        fifo/fifo.h)

link_directories(RTIMULib_connector)

add_executable(Client ${CLIENT_SOURCE_FILES})

TARGET_LINK_LIBRARIES(Client RTIMULib_connector)
