//
// Created by younghoon on 23. 1. 28.
//

#ifndef DLGO_PRINT_H
#define DLGO_PRINT_H
#include <iostream>
#include <sstream>
#include <string>

#include "goboard_slow.h"

namespace print {

const char* COLS = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

inline void printBoard(const goboard::Board& board) {
    std::ostringstream oss;
    for (auto row = board.num_rows; row > 0; --row) {
        oss << (row <= 9 ? " " : "") << row << ' ';
        for (auto col = 1; col < board.num_cols + 1; ++col) {
            auto point = gotypes::Point(row, col);
            oss << ((board.isOccupied(point))
                        ? (isBlack(board.getColor(point)) ? 'x' : 'o')
                        : '.');
        }
        oss << '\n';
    }
    oss << "   ";
    for (auto i = 0; i < board.num_cols; ++i)
        oss << COLS[i];
    std::cout << oss.str() << '\n';
};

inline void printMove(gotypes::Player player, const gotypes::Move& move) {
    std::ostringstream oss;
    if (player == gotypes::Player::black)
        oss << "black ";
    else
        oss << "white ";

    if (move.isPass()) {
        oss << "passes";
    } else if (move.isResign())
        oss << "resigns";
    else
        oss << COLS[move.point.col - 1] << move.point.row;
    std::cout << oss.str() << '\n';
}
}  // namespace print
#endif  //DLGO_PRINT_H
