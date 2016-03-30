cmake_minimum_required(VERSION 2.8)

project(server)

set(EXEC_NAME Server)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c11")

set(SERVER_SOURCE_FILES
        main.c
        mouse/mouse_device.c
        mouse/mouse_device.h
        socket/server.c
        socket/server.h
        socket/multicastServer.c
        socket/multicastServer.h
        mouse/mouse.c
        mouse/mouse.h)


add_executable(Server ${SERVER_SOURCE_FILES})

TARGET_LINK_LIBRARIES(Server pthread m)