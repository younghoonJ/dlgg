add_library(utils STATIC
        utils.h utils.cpp)
target_include_directories(utils PUBLIC ${PROJECT_SOURCE_DIR})