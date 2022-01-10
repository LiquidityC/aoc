cmake_minimum_required(VERSION 3.16)

project(aoc_2015_19 CXX)
add_compile_options(-ggdb -std=c++20 -fsanitize=address -fno-omit-frame-pointer -Wall -Wextra -pedantic)
add_link_options(-fsanitize=address)

add_subdirectory("../../lib/libinput" "${CMAKE_CURRENT_BINARY_DIR}/libinput")
add_subdirectory("../../lib/libcollections" "${CMAKE_CURRENT_BINARY_DIR}/libcollections")
add_executable(main src/main.cpp)
target_link_libraries(main libinput libcollections m)
