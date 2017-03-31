add_executable(unit-test main.cpp unit_test.cpp args_test.cpp)
target_link_libraries(unit-test args gtest)

