//
// Created by younghoon on 23. 1. 28.
//
#include "game_state.h"

namespace gamestate {
std::unique_ptr<GameState> GameState::applyMove(const gotypes::Move &move) {
    auto next_board = board;
    if (move.isPlay())
        next_board.placeStone(next_player, move.point);
    return std::make_unique<GameState>(next_board, other(next_player), this,
                                       move);
}

std::unique_ptr<GameState> GameState::newGame(int board_size) {
    return std::make_unique<GameState>(goboard::Board(board_size, board_size),
                                       gotypes::Player::black, nullptr,
                                       gotypes::Move::None());
}

bool GameState::isOver() const {
    if (last_move.isNone())
        return false;
    if (last_move.isResign())
        return true;
    auto second_last_move = prev_state->last_move;
    if (second_last_move.isNone())
        return false;
    return last_move.isPass() and second_last_move.isPass();
}

bool GameState::isMoveSelfCapture(gotypes::Player player,
                                  const gotypes::Move &move) const {
    if (not move.isPlay())
        return false;
    auto next_board = board;
    next_board.placeStone(player, move.point);
    auto new_string = next_board.getGoString(move.point);
    return new_string.num_liberties() == 0;
}

bool GameState::doesMoveViolateKo(gotypes::Player player,
                                  const gotypes::Move &move) const {
    if (not move.isPlay())
        return false;
    auto next_board = board;
    next_board.placeStone(player, move.point);
    auto next_player_ = other(player);
    auto past_state   = prev_state;
    while (past_state != nullptr) {
        if (past_state->next_player == next_player_ and
            past_state->board == next_board) {
            return true;
        }
        past_state = past_state->prev_state;
    }
    return false;
}

bool GameState::isValidMove(const gotypes::Move &move) const {
    if (isOver())
        return false;
    if (move.isPass() or move.isResign())
        return true;
    return (not board.isOccupied(move.point)) and
           (not isMoveSelfCapture(next_player, move)) and
           (not doesMoveViolateKo(next_player, move));
}


}  // namespace gamestate