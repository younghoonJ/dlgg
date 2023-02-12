//
// Created by younghoon on 23. 2. 10.
//
#include "prune.h"

namespace agent {

bool
agent::AlphaBetaAgent::isPointAnEye(const goboard::Board& board,
                               const gotypes::Point& point,
                               gotypes::Player color) {
    if (board.isOccupied(point)) return false;
    for (const auto& nbr : nbrTable->at(point.row).at(point.col))
        if (board.isOccupied(nbr) and board.getColor(nbr) != color)
            return false;

    auto friendly_corners  = 0;
    auto off_board_corners = 0;
    for (const auto& corner : point.corners()) {
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

int
AlphaBetaAgent::bestResult(const gamestate::GameStateFast& gameState, int max_depth) {
    if (gameState.isOver()) {
        if (gameState.winner() == gameState.nextPlayer())
            return MAX_SCORE;
        else
            return MIN_SCORE;
    }
    if (max_depth == 0) return captureDiff(gameState);

    auto best_so_far = MIN_SCORE;
    for (const auto& candidate_move : gameState.legalMoves()) {
        auto next_state           = gameState.applyMove(candidate_move);
        auto opponent_best_result = bestResult(*next_state, max_depth - 1);
        auto our_result           = -opponent_best_result;
        if (our_result > best_so_far) best_so_far = our_result;
    }
    return best_so_far;
}

int
AlphaBetaAgent::captureDiff(const gamestate::GameStateFast& gameState) {
    int cnt_black = 0;
    int cnt_white = 0;
    for (auto r = 1; r < gameState.board().num_rows + 1; ++r) {
        for (auto c = 1; c < gameState.board().num_cols + 1; ++c) {
            auto color = gameState.board().getColorOrNull({r, c});
            if (color == gotypes::Player::black)
                ++cnt_black;
            else if (color == gotypes::Player::white)
                ++cnt_white;
        }
    }
    if (gameState.nextPlayer() == gotypes::Player::black)
        return cnt_black - cnt_white;
    return cnt_white - cnt_black;
}

gotypes::Move
AlphaBetaAgent::selectMove(const gamestate::GameStateFast& gameState) {
    return Agent::selectMove(gameState);

}



}  // namespace agent