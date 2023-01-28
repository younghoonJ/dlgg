add_library(game STATIC
        gotypes.h
        goboard_slow.h goboard_slow.cpp
        game_state.h game_state.cpp print.h)

target_include_directories(game PUBLIC ${PROJECT_SOURCE_DIR})