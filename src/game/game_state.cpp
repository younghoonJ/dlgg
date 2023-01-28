//
// Created by younghoon on 23. 1. 28.
//
#include "game_state.h"

#include <algorithm>

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

bool GameStateZob::doesMoveViolateKo(gotypes::Player player,
                                     const gotypes::Move &move) const {
    if (not move.isPlay())
        return false;
    auto next_board = board;
    next_board.placeStone(player, move.point);
    auto next_situation = std::make_pair(other(player), next_board.getHash());
    return std::find_if(prev_state_hash.begin(), prev_state_hash.end(),
                        [&](const std::pair<gotypes::Player, uint64_t> &p) {
                            return p.first == next_situation.first and
                                   p.second == next_situation.second;
                        }) != prev_state_hash.end();
}

std::unique_ptr<GameStateZob> GameStateZob::applyMove(
    const gotypes::Move &move) {
    auto next_board = board;
    if (move.isPlay())
        next_board.placeStone(next_player, move.point);
    return std::make_unique<GameStateZob>(next_board, other(next_player), this,
                                          move);
}

std::unique_ptr<GameStateZob> GameStateZob::newGame(int board_size) {
    return std::make_unique<GameStateZob>(
        goboard::BoardZob(board_size, board_size), gotypes::Player::black,
        nullptr, gotypes::Move::None());
}

GameStateZob::GameStateZob(const goboard::BoardZob &board,
                           gotypes::Player nextPlayer, GameStateZob *prevState,
                           const gotypes::Move &lastMove)
    : board(board),
      next_player(nextPlayer),
      prev_state(prevState),
      last_move(lastMove) {
    if (prevState != nullptr) {
        for (const auto &p : prevState->prev_state_hash)
            prev_state_hash.emplace_back(p);

        prev_state_hash.emplace_back(prevState->next_player,
                                     prevState->board.getHash());
    }
}

bool GameStateZob::isOver() const {
    if (last_move.isNone())
        return false;
    if (last_move.isResign())
        return true;
    auto second_last_move = prev_state->last_move;
    if (second_last_move.isNone())
        return false;
    return last_move.isPass() and second_last_move.isPass();
}

bool GameStateZob::isMoveSelfCapture(gotypes::Player player,
                                     const gotypes::Move &move) const {
    if (not move.isPlay())
        return false;
    auto next_board = board;
    next_board.placeStone(player, move.point);
    auto new_string = next_board.getGoString(move.point);
    return new_string.num_liberties() == 0;
}

bool GameStateZob::isValidMove(const gotypes::Move &move) const {
    if (isOver())
        return false;
    if (move.isPass() or move.isResign())
        return true;
    return (not board.isOccupied(move.point)) and
           (not isMoveSelfCapture(next_player, move)) and
           (not doesMoveViolateKo(next_player, move));
}


}  // namespace gamestate