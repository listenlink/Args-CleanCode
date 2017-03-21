add_library(args STATIC args.cpp)
target_link_libraries(args fmt)

add_executable(unit-test main.cpp unit_test.cpp)

target_link_libraries(unit-test args gtest)
