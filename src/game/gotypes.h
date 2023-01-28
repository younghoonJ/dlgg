//
// Created by younghoon on 23. 1. 26.
//

#ifndef DLGO_GOTYPES_H
#define DLGO_GOTYPES_H
#include <array>
#include <cstdint>
#include <map>
#include <memory>
#include <set>
#include <vector>

namespace gotypes {
enum class Player {
    black,
    white,
};

constexpr std::initializer_list<Player> iterColors = {Player::black,
                                                      Player::white};

inline Player other(const Player &player) {
    if (player == Player::black)
        return Player::white;
    return Player::black;
}

inline bool isBlack(const Player &player) {
    return player == Player::black;
}

struct Point {
    const int row;
    const int col;

    Point(int row, int col) : row(row), col(col) {}

    inline std::array<Point, 4> neighbors() const {
        return {Point{row - 1, col}, Point{row + 1, col}, Point{row, col - 1},
                Point{row, col + 1}};
    }

    bool operator==(const Point &other) const {
        return (row == other.row) and (col == other.col);
    }

    bool operator<(const Point &other) const {
        return row == other.row ? col < other.col : row < other.row;
    }
};

enum class MoveType {
    Play,
    Pass,
    Resign,
    None,
};

class Move {
    Move(MoveType moveType, const gotypes::Point &point)
        : moveType(moveType), point(point) {}

public:
    const MoveType moveType;
    const gotypes::Point point;

    Move(const Move &move) = default;

    inline bool isPass() const { return moveType == MoveType::Pass; }

    inline bool isResign() const { return moveType == MoveType::Resign; };

    inline bool isPlay() const { return moveType == MoveType::Play; }

    inline bool isNone() const { return moveType == MoveType::None; }

    inline static Move play(const gotypes::Point &point) {
        return {MoveType::Play, point};
    }

    inline static Move pass_turn() { return {MoveType::Pass, {0, 0}}; }

    inline static Move resign() { return {MoveType::Resign, {0, 0}}; }

    inline static Move None() { return {MoveType::None, {0, 0}}; }
};


}  // namespace gotypes

//
//namespace std {
//template<>
//struct hash<gotypes::Point> {
//    size_t
//    operator()(const gotypes::Point &point) const {
//        auto seed = std::hash<int8_t>{}(point.col);
//        seed ^= std::hash<int8_t>{}(point.row) + 0x9e3779b9 + (seed << 6) +
//                (seed >> 2);
//        return seed;
//    }
//};
//}  // namespace std

#endif  //DLGO_GOTYPES_H
