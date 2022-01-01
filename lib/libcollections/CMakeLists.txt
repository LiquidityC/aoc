cmake_minimum_required(VERSION 3.6.0)
project(libcollections C)
enable_testing()

add_library(libcollections
	src/linkedlist.c
	src/matrix.c
	src/hashtable.c
	src/util.c
	)

target_include_directories(libcollections
	PUBLIC include)

# Tests
add_executable(test_linkedlist
	test/test_linkedlist.c
	src/linkedlist.c
	src/util.c
	)
add_executable(test_hashtable
	test/test_hashtable.c
	src/hashtable.c
	src/util.c
	)
include_directories(include)
add_test(
	NAME linkedlist
	COMMAND test_linkedlist
	)
add_test(
	NAME hashtable
	COMMAND test_hashtable
	)
