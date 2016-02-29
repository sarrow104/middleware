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

file(GLOB_RECURSE SRC "../src/*.cpp")

find_package(Boost COMPONENTS date_time filesystem system thread)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_library(${target_name} SHARED ${SRC})
set(TARGET_OUTPUT_FULL_PATH ${LIBRARY_OUTPUT_PATH}/${target_name})

target_link_libraries(${target_name} ${Boost_LIBRARIES} Threads::Threads rt)

#incidental
add_executable(incidental ../example/example_incidental.cpp)
target_link_libraries(incidental ${target_name} ${Boost_LIBRARIES} Threads::Threads rt)

#logsys
add_executable(logsys ../example/example_logsys.cpp)
target_link_libraries(logsys ${target_name} ${Boost_LIBRARIES} Threads::Threads rt)

#loop_array
add_executable(loop_array ../example/example_loop_array.cpp)
target_link_libraries(loop_array ${target_name} ${Boost_LIBRARIES} Threads::Threads rt)

#middleware_base
add_executable(middleware_base ../example/example_middleware_base.cpp)
target_link_libraries(middleware_base ${target_name} ${Boost_LIBRARIES} Threads::Threads rt)

#middleware_base_c
add_executable(middleware_base_c ../example/example_middleware_base.c)
target_link_libraries(middleware_base_c ${target_name} ${Boost_LIBRARIES} Threads::Threads rt)

#protocol
add_executable(protocol ../example/example_protocol.cpp)
target_link_libraries(protocol ${target_name} ${Boost_LIBRARIES} Threads::Threads rt)

#segmentation_pack
add_executable(segmentation_pack ../example/example_segmentation_pack.cpp)
target_link_libraries(segmentation_pack ${target_name} ${Boost_LIBRARIES} Threads::Threads rt)

#serializecpp
add_executable(serializecpp ../example/example_serializecpp.cpp)
target_link_libraries(serializecpp ${target_name}  ${Boost_LIBRARIES} Threads::Threads rt)

#shared_memory
add_executable(shared_memory ../example/example_shared_memory.cpp)
target_link_libraries(shared_memory ${target_name}  ${Boost_LIBRARIES} Threads::Threads rt)

#socket_asio
add_executable(socket_asio ../example/example_socket_asio.cpp)
target_link_libraries(socket_asio  ${target_name} ${Boost_LIBRARIES} Threads::Threads rt)

#socket_io
add_executable(socket_io ../example/example_socket_io.cpp)
target_link_libraries(socket_io  ${target_name} ${Boost_LIBRARIES} Threads::Threads rt)

#threadpool
add_executable(threadpool ../example/example_threadpool.cpp)
target_link_libraries(threadpool  ${target_name} ${Boost_LIBRARIES} Threads::Threads rt)

# ------------------
install(TARGETS ${target_name} LIBRARY DESTINATION lib)
install(DIRECTORY ../middleware DESTINATION include)
