//
// Created by younghoon on 23. 1. 27.
//

#ifndef DLGO_GOBOARD_SLOW_H
#define DLGO_GOBOARD_SLOW_H
#include <map>
#include <set>

#include "gotypes.h"

namespace goboard {
enum class MoveType {
    play,
    pass,
    resign,
};

class Move {
    Move(MoveType moveType, const gotypes::Point& point)
        : moveType(moveType), point(point) {}

public:
    const MoveType moveType;
    const gotypes::Point point;

    inline static Move
    play(const gotypes::Point& point) {
        return {MoveType::play, point};
    }

    inline static Move
    pass_turn() {
        return {MoveType::pass, {0, 0}};
    }

    inline static Move
    resign() {
        return {MoveType::resign, {0, 0}};
    }
};

struct Gostring {
public:
    gotypes::Player color;
    std::set<gotypes::Point> stones;
    std::set<gotypes::Point> liberties;

    Gostring(gotypes::Player color, const std::set<gotypes::Point>& stones,
             const std::set<gotypes::Point>& liberties)
        : color(color), stones(stones), liberties(liberties) {}

    inline bool
    isEqual(const Gostring& other) {
        return color == other.color and stones == other.stones and
               liberties == other.liberties;
    };

    inline void
    remove_liberty(const gotypes::Point& point) {
        liberties.erase(point);
    }

    inline void
    add_liberty(const gotypes::Point& point) {
        liberties.insert(point);
    }

    Gostring
    merged_with(const Gostring& other);

    inline std::size_t
    num_liberties() {
        return liberties.size();
    };
};

class Board {
    std::map<gotypes::Point, Gostring*> grid;

    inline bool
    checkGridBound(const gotypes::Point& point) const {
        return 1 <= point.row and point.row <= num_rows and 1 <= point.col and
               point.col <= num_cols;
    }

    inline gotypes::Player
    getColor(const gotypes::Point& point) const {
        if (grid.find(point) != grid.end())
            return grid.at(point)->color;
        return gotypes::Player::none;
    };


public:
    int8_t num_rows;
    int8_t num_cols;

    Board(int8_t numRows, int8_t numCols)
        : num_rows(numRows), num_cols(numCols) {}

    bool
    placeStone(gotypes::Player player, const gotypes::Point& point);
};
}  // namespace goboard


#endif  //DLGO_GOBOARD_SLOW_H