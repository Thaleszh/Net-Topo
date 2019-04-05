cmake_minimum_required (VERSION 3.1)
project (net_topo)

include_directories(include)


# wildcard sources
file(GLOB SOURCES "src/*.cpp")


# GTest
find_package(GTest REQUIRED)
find_package(Threads REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
enable_testing()


# flags
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(net_topo ${SOURCES})

install(TARGETS net_topo DESTINATION /usr/lib)
add_executable( runUnitTests tests/tests.cc )
target_link_libraries(runUnitTests gtest gtest_main net_topo pthread)
add_test( runUnitTests runUnitTests )
