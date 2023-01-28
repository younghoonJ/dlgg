//
// Created by younghoon on 23. 1. 28.
//

#include "helpers.h"

namespace agent {

bool isPointAnEye(const goboard::Board& board, const gotypes::Point& point,
                  gotypes::Player color) {
    if (board.isOccupied(point))
        return false;
    for (const auto& nbr : point.neighbors())
        if (board.checkGridBound(nbr) and board.isOccupied(nbr) and
            board.getColor(nbr) != color)
            return false;

    auto friendly_corners  = 0;
    auto off_board_corners = 0;
    for (const auto& corner : {gotypes::Point{point.row - 1, point.col - 1},
                               {point.row - 1, point.col + 1},
                               {point.row + 1, point.col - 1},
                               {point.row + 1, point.col + 1}}) {
        if (board.checkGridBound(corner)) {
            if (board.isOccupied(corner) and board.getColor(corner) == color)
                ++friendly_corners;
        } else {
            ++off_board_corners;
        }
    }
    if (off_board_corners > 0)
        return off_board_corners + friendly_corners == 4;
    return friendly_corners >= 3;
}
}  // namespace agent
