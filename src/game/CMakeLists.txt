add_library(game STATIC
        gotypes.h
        goboard.h goboard.cpp
        zobrist.h
        gostring.h gostring.cpp
        scoring.h scoring.cpp
        gamestate_slow.h gamestate_fast.h)

target_include_directories(game PUBLIC ${PROJECT_SOURCE_DIR})