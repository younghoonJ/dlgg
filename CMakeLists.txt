cmake_minimum_required(VERSION 3.24)
project(dlgo CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
message(${PROJECT_SOURCE_DIR})
add_subdirectory(src)
add_subdirectory(src/game)
add_subdirectory(src/agent)

add_executable(dlgo main.cpp)
target_link_libraries(dlgo game agent)

