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

file(GLOB ARGS_SRC src/*.cpp)

#set(ExternalProjectCMakeArgs
#    -DCMAKE_INSTALL_PREFIX=${CMAKE_SOURCE_DIR}/external
#    -DCMAKE_Fortran_COMPILER=${CMAKE_Fortran_COMPILER}
#    -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
#    -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
#    -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
#    )
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/external/fmt)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/external/googletest)



add_subdirectory(src)
