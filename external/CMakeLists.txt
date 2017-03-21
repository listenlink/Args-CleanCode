#Add external libraries
if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -wd4244") # disable warning for fmt
endif()



set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
set(BUILD_GTEST ON CACHE BOOL "" FORCE)
if(MSVC)
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE) #Force /MD build for visual studio
endif()

add_subdirectory(fmt)
add_subdirectory(googletest)
