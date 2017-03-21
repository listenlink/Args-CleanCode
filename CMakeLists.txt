cmake_minimum_required(VERSION 2.8)

# project name
project(Args)

#Compiler flags
if(CMAKE_COMPILER_IS_GNUCXX)
    file(GLOB_RECURSE MAKE_HEADERS_VISIBLE_SRC *.cpp *.c *.hpp *.h)
    add_custom_target(MAKE_HEADERS_VISIBLE SOURCES ${MAKE_HEADERS_VISIBLE_SRC})
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall -pedantic -Wextra")
	if(NOT CMAKE_BUILD_TYPE)
        message(STATUS "Default build type: Release")
        set(CMAKE_BUILD_TYPE "Release")
    endif()
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_CURRENT_SOURCE_DIR}/external/)
file(GLOB ARGS_INC_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h)

add_subdirectory(external)
add_subdirectory(lib)
add_subdirectory(test)
