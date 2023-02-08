//
// Created by younghoon on 23. 1. 27.
//

#include "goboard.h"

#include <algorithm>
#include <stdexcept>

#include "gostring.h"
#include "gotypes.h"

namespace goboard {

pointTable_t makeNeighborTable(int board_size) {
    auto bound_max = board_size + 1;
    auto res       = std::vector<std::vector<std::vector<gotypes::Point>>>(
        bound_max, std::vector<std::vector<gotypes::Point>>(
                       bound_max, std::vector<gotypes::Point>()));
    for (int r = 1; r < bound_max; ++r) {
        for (int c = 1; c < bound_max; ++c) {
            for (auto &nbr : gotypes::Point{r, c}.neighbors()) {
                if ((1 <= nbr.row and nbr.row < bound_max) and
                    (1 <= nbr.col and nbr.col < bound_max))
                    res[r][c].emplace_back(nbr);
            }
        }
    }
    return res;
}

pointTable_t makeCornerTable(int board_size) {
    auto bound_max = board_size + 1;
    auto res       = std::vector<std::vector<std::vector<gotypes::Point>>>(
        bound_max, std::vector<std::vector<gotypes::Point>>(
                       bound_max, std::vector<gotypes::Point>()));
    for (int r = 1; r < bound_max; ++r) {
        for (int c = 1; c < bound_max; ++c) {
            for (auto &nbr : gotypes::Point{r, c}.corners()) {
                if ((1 <= nbr.row and nbr.row < bound_max) and
                    (1 <= nbr.col and nbr.col < bound_max))
                    res[r][c].emplace_back(nbr);
            }
        }
    }
    return res;
}

void Board::placeStone(gotypes::Player player, const gotypes::Point &point) {
    if (not checkGridBound(point)) throw std::runtime_error("checkGridBound");
    if (isOccupied(point)) throw std::runtime_error("is already occupied");

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
        if (not checkGridBound(nbr)) continue;
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
        if (go_str->num_liberties() == 0) removeString(*go_str);
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

void BoardFast::placeStone(gotypes::Player player,
                           const gotypes::Point &point) {
    if (not checkGridBound(point)) throw std::runtime_error("checkGridBound");
    if (isOccupied(point)) throw std::runtime_error("is already occupied");

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

    for (const auto &nbr : nbrtable->at(point.row).at(point.col)) {
        auto it = grid.find(nbr);
        if (it == grid.end()) {
            liberties.insert(nbr);
        } else if (it->second->color == player) {
            push_if_not_in(adjacent_same_color, it->second);
        } else {
            push_if_not_in(adjacent_opposite_color, it->second);
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
        if (go_str->num_liberties() == 0) removeString(*go_str);
    }
}

void BoardFast::removeString(const gostring::Gostring &to_be_removed) {
    for (const auto &point : to_be_removed.stones) {
        for (const auto &nbr : nbrtable->at(point.row).at(point.col)) {
            auto nbrstr_it = grid.find(nbr);
            if (nbrstr_it != grid.end()) {
                if (not nbrstr_it->second->isEqual(to_be_removed)) {
                    nbrstr_it->second->add_liberty(point);
                }
            }
        }
        grid.erase(point);
        _hash ^= zobrist::get(point.row, point.col, to_be_removed.color);
    }
    _eraseOldString(to_be_removed);
}

bool BoardFast::isSelfCapture(gotypes::Player player,
                              const gotypes::Point &point) const {
    std::vector<gostring::Gostring *> friendly_strings;
    for (const auto &nbr : nbrtable->at(point.row).at(point.col)) {
        auto nbrstr_it = grid.find(nbr);
        if (nbrstr_it == grid.end()) {
            // has a liberty
            return false;
        } else if (nbrstr_it->second->color == player) {
            friendly_strings.push_back(nbrstr_it->second);
        } else {
            if (nbrstr_it->second->num_liberties() == 1) {
                // real capture, not self capture
                return false;
            }
        }
    }
    if (std::all_of(
            friendly_strings.begin(), friendly_strings.end(),
            [&](gostring::Gostring *r) { return r->num_liberties() == 1; }))
        return true;
    return false;
}


}  // namespace goboard
