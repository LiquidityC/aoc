cmake_minimum_required(VERSION 3.6.0)
project(libinput C)

add_library(libinput src/lib)
target_include_directories(libinput
	PUBLIC include)
