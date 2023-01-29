//
// Created by younghoon on 23. 1. 27.
//

#include "goboard.h"

#include <algorithm>
#include <stdexcept>

#include "gostring.h"
#include "gotypes.h"

namespace goboard {


void Board::placeStone(gotypes::Player player, const gotypes::Point &point) {
    if (not checkGridBound(point))
        throw std::runtime_error("checkGridBound");
    if (isOccupied(point))
        throw std::runtime_error("is already occupied");

    std::vector<gostring::Gostring *> adjacent_same_color;
    std::vector<gostring::Gostring *> adjacent_opposite_color;
    std::set<gotypes::Point> liberties;

    auto push_if_not_in = [](std::vector<gostring::Gostring *> &vec,
                             gostring::Gostring *gostring) {
        if (std::find_if(vec.begin(), vec.end(), [&](gostring::Gostring *s) {
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

    auto new_string = new gostring::Gostring(player, {point}, liberties);
    _goStrings.emplace_back(new_string);
    //    for (const auto go_str : adjacent_same_color) {
    //        new_string = new_string->merge_with(*go_str);
    //        _eraseOldString(*go_str);
    //    }
    for (const auto go_str : adjacent_same_color) {
        new_string->merge_inplace(*go_str);
        _eraseOldString(*go_str);
    }

    for (const auto &stone : new_string->stones) {
        grid[stone] = new_string;
    }
    for (const auto go_str : adjacent_opposite_color) {
        go_str->remove_liberty(point);
        if (go_str->num_liberties() == 0)
            removeString(*go_str);
    }
}

void Board::removeString(const gostring::Gostring &to_be_removed) {
    for (const auto &point : to_be_removed.stones) {
        for (const auto &nbr : point.neighbors()) {
            if (isOccupied(nbr)) {
                auto nbr_string = grid.at(nbr);
                if (not nbr_string->isEqual(to_be_removed)) {
                    nbr_string->add_liberty(point);
                }
            }
        }
        grid.erase(point);
    }
    _eraseOldString(to_be_removed);
}

Board::Board(const Board &other)
    : num_rows(other.num_rows), num_cols(other.num_cols) {
    for (const auto &ptr : other._goStrings)
        _goStrings.emplace_back(new gostring::Gostring(*ptr));
    for (const auto &ptr : _goStrings) {
        for (const auto &stone : ptr->stones) {
            grid[stone] = ptr.get();
        }
    }
}

void Board::_eraseOldString(const gostring::Gostring &oldString) {
    _goStrings.erase(
        std::find_if(_goStrings.begin(), _goStrings.end(),
                     [&](const std::unique_ptr<gostring::Gostring> &ptr) {
                         return ptr->isEqual(oldString);
                     }));
}

void BoardZob::placeStone(gotypes::Player player, const gotypes::Point &point) {
    if (not checkGridBound(point))
        throw std::runtime_error("checkGridBound");
    if (isOccupied(point))
        throw std::runtime_error("is already occupied");

    std::vector<gostring::Gostring *> adjacent_same_color;
    std::vector<gostring::Gostring *> adjacent_opposite_color;
    std::set<gotypes::Point> liberties;

    auto push_if_not_in = [](std::vector<gostring::Gostring *> &vec,
                             gostring::Gostring *gostring) {
        if (std::find_if(vec.begin(), vec.end(), [&](gostring::Gostring *s) {
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

    auto new_string = new gostring::Gostring(player, {point}, liberties);
    _goStrings.emplace_back(new_string);

    for (const auto go_str : adjacent_same_color) {
        //        new_string = new_string->merge_with(*go_str);
        new_string->merge_inplace(*go_str);
        _eraseOldString(*go_str);
    }
    for (const auto &stone : new_string->stones)
        grid[stone] = new_string;

    _hash ^= zobrist::get(point.row, point.col, player);

    for (const auto go_str : adjacent_opposite_color) {
        go_str->remove_liberty(point);
        if (go_str->num_liberties() == 0)
            removeString(*go_str);
    }
}

void BoardZob::removeString(const gostring::Gostring &to_be_removed) {
    for (const auto &point : to_be_removed.stones) {
        for (const auto &nbr : point.neighbors()) {
            if (isOccupied(nbr)) {
                auto nbr_string = grid.at(nbr);
                if (not nbr_string->isEqual(to_be_removed)) {
                    nbr_string->add_liberty(point);
                }
            }
        }
        grid.erase(point);
        _hash ^= zobrist::get(point.row, point.col, to_be_removed.color);
    }
    _eraseOldString(to_be_removed);
}


}  // namespace goboard
