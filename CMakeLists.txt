cmake_minimum_required(VERSION 4.0)
project(DataInputService)

set(CMAKE_CXX_STANDARD 20)

add_executable(DataInputService
        main.cpp
        DataInputService.cpp
        DataInputService.h
)
