//
// Created by younghoon on 23. 1. 27.
//

#include "goboard_slow.h"

#include <algorithm>
#include <stdexcept>

#include "gotypes.h"

namespace goboard {
std::unique_ptr<Gostring> Gostring::merged_with(const Gostring &other) {
    if (color != other.color)
        throw std::runtime_error("color != other.color");
    auto new_string = new Gostring(color);
    for (const auto &stones_ : {stones, other.stones})
        for (const auto &s : stones_)
            new_string->stones.emplace(s);
    for (const auto &lib : {liberties, other.liberties})
        for (const auto &stone : lib)
            if (not new_string->isInLiberties(stone))
                new_string->liberties.emplace(stone);
    for (const auto &stone : new_string->stones)
        if (new_string->isInLiberties(stone))
            new_string->liberties.erase(stone);
    return std::unique_ptr<Gostring>{new_string};
}

void Board::placeStone(gotypes::Player player, const gotypes::Point &point) {
    if (not checkGridBound(point))
        throw std::runtime_error("checkGridBound");
    if (isOccupied(point))
        throw std::runtime_error("is already occupied");

    std::vector<Gostring *> adjacent_same_color;
    std::vector<Gostring *> adjacent_opposite_color;
    std::set<gotypes::Point> liberties;

    auto push_if_not_in = [](std::vector<Gostring *> &vec, Gostring *gostring) {
        if (std::find_if(vec.begin(), vec.end(), [&](Gostring *s) {
                return gostring->isEqual(*s);
            }) == vec.end()) {
            vec.push_back(gostring);
        }
    };

    for (const auto &nbr : point.neighbors()) {
        if (not checkGridBound(nbr))
            continue;
        if (not isOccupied(nbr)) {
            liberties.insert(nbr);
        } else if (getColor(nbr) == player) {
            push_if_not_in(adjacent_same_color, grid.at(nbr));
        } else {
            push_if_not_in(adjacent_opposite_color, grid.at(nbr));
        }
    }

    auto new_string =
        std::make_unique<Gostring>(player, std::set{point}, liberties);
    for (const auto s : adjacent_same_color) {
        new_string = new_string->merged_with(*s);
        _eraseOldString(*s);
    }
    for (const auto &p : new_string->stones) {
        grid[p] = new_string.get();
    }
    for (const auto s : adjacent_opposite_color) {
        s->remove_liberty(point);
    }
    for (const auto s : adjacent_opposite_color) {
        if (s->num_liberties() == 0)
            removeString(*s);
    }
    _goStrings.emplace_back(new_string.release());
}

void Board::removeString(const Gostring &to_be_removed) {
    for (const auto &point : to_be_removed.stones) {
        for (const auto &nbr : point.neighbors()) {
            if (isOccupied(nbr)) {
                auto nbr_string = grid.at(nbr);
                if (not nbr_string->isEqual(to_be_removed)) {
                    nbr_string->add_liberty(point);
                }
            }
        }
        //        grid[point] = nullptr;
        grid.erase(point);
    }
    _eraseOldString(to_be_removed);
}

Board::Board(const Board &other)
    : num_rows(other.num_rows), num_cols(other.num_cols) {
    for (const auto &ptr : other._goStrings)
        _goStrings.emplace_back(new Gostring(*ptr));
    for (const auto &ptr : _goStrings) {
        for (const auto &stone : ptr->stones) {
            grid[stone] = ptr.get();
        }
    }
}

void Board::_eraseOldString(const Gostring &oldString) {
    auto it = std::find_if(_goStrings.begin(), _goStrings.end(),
                           [&](const std::unique_ptr<Gostring> &ptr) {
                               return ptr->isEqual(oldString);
                           });
    _goStrings.erase(it);
}


}  // namespace goboard
