//
// Created by younghoon on 23. 1. 26.
//

#ifndef DLGO_GOTYPES_H
#define DLGO_GOTYPES_H
#include <array>
#include <cstdint>
#include <vector>

namespace gotypes {
enum class Player {
    black,
    white,
    none,
};

inline Player
other(const Player &player) {
    if (player == Player::black)
        return Player::white;
    if (player == Player::white)
        return Player::black;
    return Player::none;
}

struct Point {
    const int8_t row;
    const int8_t col;

    Point(int8_t row, int8_t col) : row(row), col(col) {}

    inline std::array<Point, 4>
    neighbors() const {
        return {Point{static_cast<int8_t>(row - 1), col},
                Point{static_cast<int8_t>(row + 1), col},
                Point{row, static_cast<int8_t>(col - 1)},
                Point{row, static_cast<int8_t>(col + 1)}};
    }

    bool
    operator==(const Point &other) const {
        return (row == other.row) and (col == other.col);
    }

    bool
    operator<(const Point &other) const {
        return row == other.row ? col < other.col : row < other.row;
    }
};

}  // namespace gotypes

namespace std {
template<>
struct hash<gotypes::Point> {
    size_t
    operator()(const gotypes::Point &point) const {
        auto seed = std::hash<int8_t>{}(point.col);
        seed ^= std::hash<int8_t>{}(point.row) + 0x9e3779b9 + (seed << 6) +
                (seed >> 2);
        return seed;
    }
};

}  // namespace std

#endif  //DLGO_GOTYPES_H
