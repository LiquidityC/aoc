cmake_minimum_required(VERSION 3.6.0)
project(libcollections C)
enable_testing()

add_library(libcollections
	src/linkedlist.c
	src/matrix.c
	src/util.c
	)

add_executable(test_linkedlist
	test/test_linkedlist.c
	src/linkedlist.c
	src/util.c
	)

add_test(
	NAME linkedlist
	COMMAND test_linkedlist
	)

target_include_directories(libcollections
	PUBLIC include)
include_directories(include)
