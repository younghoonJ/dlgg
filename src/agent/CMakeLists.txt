add_library(agent STATIC
        naive.h naive.cpp
        base.h)

target_include_directories(agent PUBLIC ${PROJECT_SOURCE_DIR})
target_link_libraries(agent PUBLIC game)