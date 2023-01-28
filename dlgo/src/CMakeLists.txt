add_library(src
        gotypes.h
        goboard_slow.h goboard_slow.cpp)
target_include_directories(src PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")