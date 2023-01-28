//
// Created by younghoon on 23. 1. 28.
//

#ifndef DLGO_GOSTRING_H
#define DLGO_GOSTRING_H
#include <map>
#include <memory>
#include <set>

#include "gotypes.h"

namespace gostring {


struct Gostring {
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

    inline bool isEqual(const Gostring &other) const {
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

    inline std::size_t num_liberties() const { return liberties.size(); };
};
}  // namespace gostring
#endif  //DLGO_GOSTRING_H
