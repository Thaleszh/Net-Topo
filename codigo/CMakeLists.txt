cmake_minimum_required (VERSION 3.1)
project (net_topo)

include_directories(include)

set(CHARM_DIR ~/Documents/charm-6.9.0)


# wildcard sources
file(GLOB SOURCES "src/*.cpp")


# GTest
find_package(GTest REQUIRED)
find_package(Threads REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
enable_testing()

find_package(GTest REQUIRED)
find_package(Threads REQUIRED)


# flags
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
add_library(net_topo ${SOURCES})

# charm
list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

list(APPEND CMAKE_PREFIX_PATH "${CHARM_DIR}")
find_package(CONV REQUIRED)

target_link_libraries(net_topo CONV::conv_core CONV::conv_ldb)

install(TARGETS net_topo DESTINATION /usr/lib)

add_executable( runUnitTests tests/tests.cc )

target_link_libraries(runUnitTests gtest gtest_main net_topo pthread)

add_test( runUnitTests runUnitTests )
