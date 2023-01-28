//
// Created by younghoon on 23. 1. 27.
//

#ifndef DLGO_GOBOARD_SLOW_H
#define DLGO_GOBOARD_SLOW_H

#include <map>
#include <memory>
#include <set>

#include "gotypes.h"

namespace goboard {


struct Gostring {
public:
    gotypes::Player color;
    std::set<gotypes::Point> stones;
    std::set<gotypes::Point> liberties;

    explicit Gostring(gotypes::Player color) : color(color) {}

    Gostring(const Gostring &gostring) = default;

    Gostring(gotypes::Player color, const std::set<gotypes::Point> &stones,
             const std::set<gotypes::Point> &liberties)
        : color(color), stones(stones), liberties(liberties) {}

    inline bool isIn(const gotypes::Point &point) {
        return stones.find(point) != stones.end();
    };

    inline bool isInLiberties(const gotypes::Point &point) {
        return liberties.find(point) != liberties.end();
    };

    inline bool isEqual(const Gostring &other) {
        return color == other.color and stones == other.stones and
               liberties == other.liberties;
    };

    inline void remove_liberty(const gotypes::Point &point) {
        liberties.erase(point);
    }

    inline void add_liberty(const gotypes::Point &point) {
        liberties.insert(point);
    }

    std::unique_ptr<Gostring> merged_with(const Gostring &other);

    inline std::size_t num_liberties() { return liberties.size(); };
};

class Board {
    std::map<gotypes::Point, Gostring *> grid;
    std::vector<std::unique_ptr<Gostring>> _goStrings;

    void _eraseOldString(const Gostring &oldString);

public:
    const int num_rows;
    const int num_cols;

    Board(int numRows, int numCols) : num_rows(numRows), num_cols(numCols) {}

    Board(const Board &other);

    void removeString(const Gostring &to_be_removed);

    inline bool checkGridBound(const gotypes::Point &point) const {
        return 1 <= point.row and point.row <= num_rows and 1 <= point.col and
               point.col <= num_cols;
    }

    inline bool isOccupied(const gotypes::Point &point) const {
        return grid.find(point) != grid.end();
    };

    inline gotypes::Player getColor(const gotypes::Point &point) const {
        return grid.at(point)->color;
    };

    inline Gostring &getGoString(const gotypes::Point &point) const {
        return *grid.at(point);
    }

    void placeStone(gotypes::Player player, const gotypes::Point &point);

    bool operator==(const Board &other) const {
        return grid.size() == other.grid.size() and
               std::equal(
                   grid.begin(), grid.end(), other.grid.begin(),
                   [&](const std::pair<gotypes::Point, Gostring *> &left,
                       const std::pair<gotypes::Point, Gostring *> &right) {
                       return left.first == right.first;
                   });
    };
};

}  // namespace goboard


#endif  //DLGO_GOBOARD_SLOW_H
