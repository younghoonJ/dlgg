add_library(game STATIC
        gotypes.h
        goboard.h goboard.cpp
        game_state.h game_state.cpp
        zobrist.h gostring.h gostring.cpp)

target_include_directories(game PUBLIC ${PROJECT_SOURCE_DIR})