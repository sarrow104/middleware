cmake_minimum_required(VERSION 2.6)

add_definitions(-I../../)
add_definitions(-std=c++11)
add_definitions(-W -fexceptions -Wunused-variable -Wfatal-errors)

set(target_name "middleware")

if ("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
	add_definitions(-DNODEBUG -O2 -s)
else()
    set(target_name "${target_name}D")
	add_definitions(-O0 -g -ggdb)
endif()

project("${target_name}Lib")

set(EXECUTABLE_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/example")
set(LIBRARY_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/lib")
file(GLOB SRC "../src/**/*.cpp")

find_package(Boost COMPONENTS date_time filesystem system thread)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_library(${target_name} SHARED ${SRC})
set(TARGET_OUTPUT_FULL_PATH ${LIBRARY_OUTPUT_PATH}/${target_name})

target_link_libraries(${target_name} ${Boost_LIBRARIES} Threads::Threads rt)

add_executable(loop_array ../example/loop_array.cpp)
target_link_libraries(loop_array ${target_name} ${Boost_LIBRARIES} Threads::Threads rt)

add_executable(serializecpp ../example/serializecpp.cpp)
target_link_libraries(serializecpp ${target_name}  ${Boost_LIBRARIES} Threads::Threads rt)

add_executable(shared_memory ../example/shared_memory.cpp)
target_link_libraries(shared_memory ${target_name}  ${Boost_LIBRARIES} Threads::Threads rt)

add_executable(socket_io ../example/socket_io.cpp)
target_link_libraries(socket_io  ${target_name} ${Boost_LIBRARIES} Threads::Threads rt)

# ------------------
install (TARGETS ${target_name} LIBRARY DESTINATION lib)
file(GLOB HEADERS "../middleware/**/*.h")
install(DIRECTORY ../middleware DESTINATION include)
