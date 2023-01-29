cmake_minimum_required(VERSION 3.24)
project(dlgo CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0")

#if (NOT CMAKE_BUILD_TYPE)
#    set(CMAKE_BUILD_TYPE Release)
#endif ()
#
#set(CMAKE_CXX_FLAGS "-Wall -Wextra")
#set(CMAKE_CXX_FLAGS_DEBUG "-g")
#set(CMAKE_CXX_FLAGS_RELEASE "-O3")

message(${PROJECT_SOURCE_DIR})
add_subdirectory(src)
add_subdirectory(src/game)
add_subdirectory(src/agent)
add_subdirectory(src/utils)

add_executable(dlgo main.cpp)
target_link_libraries(dlgo game agent utils)

