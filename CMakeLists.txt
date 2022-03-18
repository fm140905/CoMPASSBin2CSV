cmake_minimum_required(VERSION 3.16.3)

project(CoMPASSBin2CSV CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose Release or Debug" FORCE)
endif ()
message(STATUS "Build type: '${CMAKE_BUILD_TYPE}'")

add_executable(main main.cpp)