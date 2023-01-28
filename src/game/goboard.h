//
// Created by younghoon on 23. 1. 27.
//

#ifndef DLGO_GOBOARD_H
#define DLGO_GOBOARD_H

#include <map>
#include <memory>
#include <set>

#include "gostring.h"
#include "gotypes.h"
#include "zobrist.h"

namespace goboard {


class Board {
protected:
    std::map<gotypes::Point, gostring::Gostring *> grid;
    std::vector<std::unique_ptr<gostring::Gostring>> _goStrings;

    void _eraseOldString(const gostring::Gostring &oldString);

public:
    const int num_rows;
    const int num_cols;

    Board(int numRows, int numCols) : num_rows(numRows), num_cols(numCols) {}

    Board(const Board &other);

    virtual void removeString(const gostring::Gostring &to_be_removed);

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

    inline gostring::Gostring &getGoString(const gotypes::Point &point) const {
        return *grid.at(point);
    }

    virtual void placeStone(gotypes::Player player,
                            const gotypes::Point &point);

    bool operator==(const Board &other) const {
        return grid.size() == other.grid.size() and
               std::equal(
                   grid.begin(), grid.end(), other.grid.begin(),
                   [&](const std::pair<gotypes::Point, gostring::Gostring *>
                           &left,
                       const std::pair<gotypes::Point, gostring::Gostring *>
                           &right) { return left.first == right.first; });
    };
};

class BoardZob : public Board {
    uint64_t _hash;

public:
    BoardZob(int numRows, int numCols, uint64_t hash = zobrist::EMPTY_BOARD)
        : Board(numRows, numCols), _hash(hash){};

    inline uint64_t getHash() const { return _hash; }

    void placeStone(gotypes::Player player,
                    const gotypes::Point &point) override;
    void removeString(const gostring::Gostring &to_be_removed) override;
};


}  // namespace goboard


#endif  //DLGO_GOBOARD_H
