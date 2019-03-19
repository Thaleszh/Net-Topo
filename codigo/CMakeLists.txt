cmake_minimum_required (VERSION 2.8)
project (net_topo)

include_directories(include)

# wildcard sources
file(GLOB SOURCES "src/*.cpp")


# GTest
#ADD_SUBDIRECTORY (gtest-1.6.0)
#enable_testing()
#include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

# flags
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(net_topo ${SOURCES})

#install(TARGETS net_topo DESTINATION /usr/lib)
#add_executable( runUnitTests tests.cc )
#target_link_libraries(runUnitTests gtest gtest_main)
#add_test( runUnitTests runUnitTests )
