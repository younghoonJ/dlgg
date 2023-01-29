//
// Created by younghoon on 23. 1. 28.
//

#ifndef DLGO_UTILS_H
#define DLGO_UTILS_H
#include <cstring>
#include <iostream>
#include <sstream>
#include <string>

#include "src/game/goboard.h"

namespace utils {

constexpr const char* COLS = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
constexpr size_t LEN_COLS  = 26;

void printBoard(const goboard::Board& board);

void printMove(gotypes::Player player, const gotypes::Move& move);

gotypes::Point pointFromCoords(const std::string& coord);
}  // namespace utils
#endif  //DLGO_UTILS_H
