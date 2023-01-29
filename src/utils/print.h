//
// Created by younghoon on 23. 1. 28.
//

#ifndef DLGO_PRINT_H
#define DLGO_PRINT_H
#include <iostream>
#include <sstream>
#include <string>

#include "src/game/goboard.h"

namespace utils {

constexpr const char* COLS = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";


void printBoard(const goboard::Board& board);

void printMove(gotypes::Player player, const gotypes::Move& move);
}  // namespace utils
#endif  //DLGO_PRINT_H
