cmake_minimum_required(VERSION 4.1)

project(solution C CXX)

# Compile and link flags
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Wall -Wno-unused -fsanitize=address -fsanitize=undefined -fno-omit-frame-pointer")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wno-unused -fsanitize=address -fsanitize=undefined -fno-omit-frame-pointer")
set(CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -fsanitize=address -fsanitize=undefined -fno-omit-frame-pointer")

add_subdirectory("../../lib/libinput" "${CMAKE_CURRENT_BINARY_DIR}/libinput")
add_subdirectory("../../lib/libcollections" "${CMAKE_CURRENT_BINARY_DIR}/libcollections")

file(GLOB C_SOURCE_FILES CONFIGURE_DEPENDS src/*.c)
file(GLOB CXX_SOURCE_FILES CONFIGURE_DEPENDS src/*.cpp)

foreach(F IN LISTS C_SOURCE_FILES)
	get_filename_component(EXENAME ${F} NAME_WE)
	add_executable(${EXENAME} ${F})
	target_link_libraries(${EXENAME} libinput libcollections m)
endforeach ()

foreach(F IN LISTS CXX_SOURCE_FILES)
	get_filename_component(EXENAME ${F} NAME_WE)
	add_executable(${EXENAME} ${F})
	set_property(TARGET ${EXENAME} PROPERTY CXX_STANDARD 23)
	target_link_libraries(${EXENAME} libinput libcollections m)
endforeach ()
