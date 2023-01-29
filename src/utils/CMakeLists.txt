add_library(utils STATIC
        print.h print.cpp)
target_include_directories(utils PUBLIC ${PROJECT_SOURCE_DIR})