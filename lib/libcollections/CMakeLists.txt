cmake_minimum_required(VERSION 3.6.0)
project(libcollections C)
enable_testing()

add_compile_options(
	-ggdb
	-fsanitize=address
	-fsanitize=undefined
	-fno-omit-frame-pointer
	-Wall
	-Wextra
	-pedantic
	)
add_link_options(
	-fsanitize=address
	-fsanitize=undefined
	)


add_library(libcollections
	src/linkedlist.c
	src/matrix.c
	src/hashtable.c
	src/array.c
	src/heap.c
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
add_executable(test_array
	test/test_array.c
	src/array.c
	src/util.c
	)

add_executable(test_heap
	test/test_heap.c
	src/array.c
	src/heap.c
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
add_test(
	NAME array
	COMMAND test_array
	)
add_test(
	NAME heap
	COMMAND test_heap
	)
