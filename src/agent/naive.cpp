//
// Created by younghoon on 23. 1. 28.
//
#include "naive.h"

#include <algorithm>

namespace agent {
gotypes::Move RandomBot::selectMove(const gamestate::GameState &gameState) {
    std::vector<gotypes::Point> candidates;
    for (auto r = 1; r < gameState.board().num_rows + 1; ++r) {
        for (auto c = 1; c < gameState.board().num_cols + 1; ++c) {
            auto candidate = gotypes::Point(r, c);
            if (gameState.isValidMove(gotypes::Move::play(candidate)) and
                not isPointAnEye(gameState.board(), candidate,
                                 gameState.nextPlayer())) {
                candidates.push_back(candidate);
            }
        }
    }
    if (candidates.empty()) return gotypes::Move::pass_turn();
    return gotypes::Move::play(candidates[rng() % candidates.size()]);
}

bool RandomBot::isPointAnEye(const goboard::Board &board,
                             const gotypes::Point &point,
                             gotypes::Player color) {
    if (board.isOccupied(point)) return false;
    for (const auto &nbr : point.neighbors())
        if (board.checkGridBound(nbr) and board.isOccupied(nbr) and
            board.getColor(nbr) != color)
            return false;

    auto friendly_corners  = 0;
    auto off_board_corners = 0;
    for (const auto &corner : point.corners()) {
        if (board.checkGridBound(corner)) {
            if (board.isOccupied(corner) and board.getColor(corner) == color)
                ++friendly_corners;
        } else {
            ++off_board_corners;
        }
    }
    if (off_board_corners > 0) return off_board_corners + friendly_corners == 4;
    return friendly_corners >= 3;
}

gotypes::Move RandomBotFast::selectMove(const gamestate::GameState &gameState) {
    std::vector<gotypes::Point> candidates;
    for (auto r = 1; r < gameState.board().num_rows + 1; ++r) {
        for (auto c = 1; c < gameState.board().num_cols + 1; ++c) {
            auto candidate = gotypes::Point(r, c);
            if (gameState.isValidMove(gotypes::Move::play(candidate)) and
                not isPointAnEye(gameState.board(), candidate,
                                 gameState.nextPlayer())) {
                candidates.push_back(candidate);
            }
        }
    }
    if (candidates.empty()) return gotypes::Move::pass_turn();
    return gotypes::Move::play(candidates[rng() % candidates.size()]);
}

bool RandomBotFast::isPointAnEye(const goboard::Board &board,
                                 const gotypes::Point &point,
                                 gotypes::Player color) {
    if (board.isOccupied(point)) return false;
    for (const auto &nbr : nbrTable->at(point.row).at(point.col))
        if (board.isOccupied(nbr) and board.getColor(nbr) != color)
            return false;

    auto friendly_corners  = 0;
    auto off_board_corners = 0;
    for (const auto &corner : point.corners()) {
        if (board.checkGridBound(corner)) {
            if (board.isOccupied(corner) and board.getColor(corner) == color)
                ++friendly_corners;
        } else {
            ++off_board_corners;
        }
    }
    if (off_board_corners > 0) return off_board_corners + friendly_corners == 4;
    return friendly_corners >= 3;
}
}  // namespace agent
