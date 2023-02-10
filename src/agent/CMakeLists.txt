add_library(agent STATIC
        naive.h naive.cpp
        base.h minmax.h minmax.cpp)

target_include_directories(agent PUBLIC ${PROJECT_SOURCE_DIR})
target_link_libraries(agent PUBLIC game)