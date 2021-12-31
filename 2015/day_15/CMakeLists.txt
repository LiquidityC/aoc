cmake_minimum_required(VERSION 3.16)

project(aoc_2015_15 C)

add_subdirectory("../../lib/libinput" "${CMAKE_CURRENT_BINARY_DIR}/libinput")
add_executable(main src/main.c)
target_link_libraries(main libinput)
