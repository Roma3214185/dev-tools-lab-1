cmake_minimum_required(VERSION 3.16)
project(DataInputService)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(DataInputServiceLib
        DataInputService.cpp
        DataInputService.h
)

add_executable(DataInputService
        main.cpp
)

add_subdirectory(tests)
target_link_libraries(DataInputService PRIVATE DataInputServiceLib)
