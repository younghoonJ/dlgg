//
// Created by younghoon on 23. 1. 28.
//
#include "naive.h"

#include <algorithm>

#include "helpers.h"

namespace agent {
gotypes::Move RandomBot::selectMove(const gamestate::GameState &gameState) {
    std::vector<gotypes::Point> candidates;
    for (auto r = 1; r < gameState.board.num_rows + 1; ++r) {
        for (auto c = 1; c < gameState.board.num_cols + 1; ++c) {
            auto candidate = gotypes::Point(r, c);
            if (gameState.isValidMove(gotypes::Move::play(candidate)) and
                not isPointAnEye(gameState.board, candidate,
                                 gameState.next_player)) {
                candidates.push_back(candidate);
            }
        }
    }
    if (candidates.empty())
        return gotypes::Move::pass_turn();
    return gotypes::Move::play(candidates[rng() % candidates.size()]);
}

gotypes::Move RandomBot::selectMove(const gamestate::GameStateZob &gameState) {
    std::vector<gotypes::Point> candidates;
    for (auto r = 1; r < gameState.board.num_rows + 1; ++r) {
        for (auto c = 1; c < gameState.board.num_cols + 1; ++c) {
            auto candidate = gotypes::Point(r, c);
            if (gameState.isValidMove(gotypes::Move::play(candidate)) and
                not isPointAnEye(gameState.board, candidate,
                                 gameState.next_player)) {
                candidates.push_back(candidate);
            }
        }
    }
    if (candidates.empty())
        return gotypes::Move::pass_turn();
    return gotypes::Move::play(candidates[rng() % candidates.size()]);
}
}  // namespace agent
