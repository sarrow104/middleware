cmake_minimum_required(VERSION 2.6)

add_definitions(-I./include)
add_definitions(-std=c++11)
add_definitions(-W -fexceptions -Wunused-variable -Wfatal-errors)

if ("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
	set(target_name "loop_array")
	add_definitions(-DNODEBUG -O2 -s)
else()
	set(target_name "loop_arrayD")
	add_definitions(-O0 -g -ggdb)
endif()

project(${target_name})

set(EXECUTABLE_OUTPUT_PATH "${CMAKE_SOURCE_DIR}")
file(GLOB SRC "../**/*.cpp")

find_package(Boost COMPONENTS date_time filesystem system thread)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_executable(${target_name} ${SRC})
set(TARGET_OUTPUT_FULL_PATH ${EXECUTABLE_OUTPUT_PATH}/${target_name})
if ("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
	add_custom_command(
		TARGET ${target_name}
		POST_BUILD
		COMMAND ${CMAKE_STRIP} ${TARGET_OUTPUT_FULL_PATH})
endif()
target_link_libraries(${target_name} ${Boost_LIBRARIES} Threads::Threads)

