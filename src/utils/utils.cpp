//
// Created by younghoon on 23. 1. 29.
//
#include "utils.h"

namespace utils {

void printBoard(const goboard::Board& board) {
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
}

void printMove(gotypes::Player player, const gotypes::Move& move) {
    std::ostringstream oss;
    oss << (player == gotypes::Player::black ? "black" : "white");
    if (move.isPass()) {
        oss << "passes";
    } else if (move.isResign())
        oss << "resigns";
    else
        oss << COLS[move.point.col - 1] << move.point.row;
    std::cout << oss.str() << '\n';
}

gotypes::Point pointFromCoords(const std::string& coord) {
    auto col = [&]() -> int {
        for (auto i = 0; i < LEN_COLS; ++i)
            if (COLS[i] == coord.at(0))
                return i + 1;
        return -1;
    }();
    if (col == -1)
        throw std::runtime_error("wrong input:" + coord);
    auto row = stoi(coord.substr(1));
    return {row, col};
}
}  // namespace utils