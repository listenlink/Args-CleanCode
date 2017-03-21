cmake_minimum_required(VERSION 2.8)

project(Args)

if(NOT CMAKE_BUILD_TYPE)
  message(STATUS "Default build type: Release")
  set(CMAKE_BUILD_TYPE "Release")
endif()

# Add visibility of headers
file(GLOB_RECURSE MAKE_HEADERS_VISIBLE_SRC *.cpp *.c *.hpp *.h)
add_custom_target(MAKE_HEADERS_VISIBLE SOURCES ${MAKE_HEADERS_VISIBLE_SRC})
#Compiler flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall -Wextra -pedantic")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_CURRENT_SOURCE_DIR}/external/)

file(GLOB ARGS_SRC lib/*.cpp)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/external/fmt)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/external/googletest)

add_subdirectory(lib)
add_subdirectory(test)
