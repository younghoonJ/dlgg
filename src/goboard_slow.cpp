//
// Created by younghoon on 23. 1. 27.
//

#include "goboard_slow.h"

#include <algorithm>
#include <stdexcept>

namespace goboard {
Gostring
Gostring::merged_with(const Gostring& other) {
    if (color != other.color)
        throw std::runtime_error("color != other.color");
    std::set<gotypes::Point> new_stones(stones.begin(), stones.end());
    for (const auto& s : other.stones)
        new_stones.emplace(s);

    std::set<gotypes::Point> new_liberties;
    for (const auto& lib : {liberties, other.liberties})
        for (const auto& s : lib)
            if (new_stones.find(s) != new_stones.end())
                new_liberties.emplace(s);
    return {color, new_stones, new_liberties};
}

bool
Board::placeStone(gotypes::Player player, const gotypes::Point& point) {
    if (not checkGridBound(point))
        return false;
    if (getColor(point) != gotypes::Player::none)
        return false;
    std::vector<Gostring*> adjacent_same_color;
    std::vector<Gostring*> adjacent_opposite_color;
    std::vector<gotypes::Point> liberties;
    for (const auto& nbr : point.neighbors()) {
        if (not checkGridBound(nbr))
            continue;
    }
}
}  // namespace goboard
