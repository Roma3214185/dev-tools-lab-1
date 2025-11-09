cmake_minimum_required(VERSION 3.16)
project(DataInputServiceTests)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

enable_testing()

find_package(Catch2 3 REQUIRED)

add_executable(DataInputServiceTests
        datainputservice_tests.cpp
)

target_link_libraries(DataInputServiceTests PRIVATE DataInputServiceLib Catch2::Catch2WithMain)

include(CTest)
include(Catch)
catch_discover_tests(DataInputServiceTests)
