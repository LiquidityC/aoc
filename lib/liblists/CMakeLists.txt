cmake_minimum_required(VERSION 3.6.0)
project(liblists C)
enable_testing()

add_library(liblists
	src/linkedlist.c
	)

add_executable(test_linkedlist
	test/test_linkedlist.c
	src/linkedlist.c
	)

add_test(
	NAME linkedlist
	COMMAND test_linkedlist
	)

include_directories(include)
