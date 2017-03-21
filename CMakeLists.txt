cmake_minimum_required(VERSION 2.8)

project(Args)

file(GLOB_RECURSE MAKE_HEADERS_VISIBLE_SRC *.cpp *.c *.hpp *.h)
add_custom_target(MAKE_HEADERS_VISIBLE SOURCES ${MAKE_HEADERS_VISIBLE_SRC})

#Compiler flags
if(CMAKE_COMPILER_IS_GNUCXX)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall -pedantic -Wextra")
	if(NOT CMAKE_BUILD_TYPE)
        message(STATUS "Default build type: Release")
        set(CMAKE_BUILD_TYPE "Release")
    endif()
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_CURRENT_SOURCE_DIR}/external/)

file(GLOB ARGS_SRC lib/*.cpp)
if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -wd4244") # disable warning for fmt
endif()
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/external/fmt)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/external/googletest)
set(BUILD_GMOCK OFF)
set(BUILD_GTEST ON)
if(MSVC)
    set(gtest_force_shared_crt ON) #Force MD build for visual studio
endif()

add_subdirectory(lib)
add_subdirectory(test)
