#Add external libraries
if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -wd4244") # disable warning for fmt
endif()

add_subdirectory(fmt)
add_subdirectory(googletest)

set(BUILD_GMOCK OFF)
set(BUILD_GTEST ON)
if(MSVC)
    set(gtest_force_shared_crt ON) #Force /MD build for visual studio
endif()

