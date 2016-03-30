cmake_minimum_required(VERSION 3.0)

project(RTIMULib_connector)

set(EXEC_NAME RTIMULib_connector)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(RTIMULIB_CONNECTOR_SOURCE_FILES
        RTIMULib_connector.cpp
        RTIMULib_connector.h
        )


add_library(RTIMULib_connector SHARED ${RTIMULIB_CONNECTOR_SOURCE_FILES})

TARGET_LINK_LIBRARIES(RTIMULib_connector RTIMULib)
