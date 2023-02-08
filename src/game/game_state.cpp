//
// Created by younghoon on 23. 1. 28.
//
#include "game_state.h"

#include <algorithm>
#include <utility>

namespace gamestate {


std::unique_ptr<GameState> GameStateSlow::applyMove(const gotypes::Move &move) {
    auto next_board = _board;
    if (move.isPlay()) next_board.placeStone(_next_player, move.point);
    return std::make_unique<GameStateSlow>(next_board, other(_next_player),
                                           this, move);
}

bool GameStateSlow::doesMoveViolateKo(gotypes::Player player,
                                      const gotypes::Move &move) const {
    if (not move.isPlay()) return false;
    auto next_board = _board;
    next_board.placeStone(player, move.point);
    auto next_player_ = other(player);
    auto past_state   = _prev_state.get();
    while (past_state) {
        if (_prev_state->_next_player == next_player_ and
            past_state->board() == next_board) {
            return true;
        }
        past_state = past_state->_prev_state.get();
    }
    return false;
}

GameStateSlow::GameStateSlow(const goboard::Board &board,
                             gotypes::Player nextPlayer,
                             GameStateSlow *prevState,
                             const gotypes::Move &lastMove)
    : _board(board),
      _next_player(nextPlayer),
      _prev_state(prevState),
      _last_move(lastMove) {}

std::unique_ptr<GameState> GameStateFast::applyMove(const gotypes::Move &move) {
    auto next_board = _board;
    if (move.isPlay()) next_board.placeStone(_next_player, move.point);
    return std::make_unique<GameStateFast>(next_board, other(_next_player),
                                           this, move);
}

bool GameStateFast::doesMoveViolateKo(gotypes::Player player,
                                      const gotypes::Move &move) const {
    if (not move.isPlay()) return false;
    auto next_board = _board;
    next_board.placeStone(player, move.point);
    auto other_player = other(player);
    auto next_hash    = next_board.getHash();
    return std::any_of(prev_state_hash.begin(), prev_state_hash.end(),
                       [&](const std::pair<gotypes::Player, uint64_t> &p) {
                           return p.first == other_player and
                                  p.second == next_hash;
                       });
}

GameStateFast::GameStateFast(const goboard::BoardFast &board,
                             gotypes::Player nextPlayer,
                             GameStateFast *prevState,
                             const gotypes::Move &lastMove)
    : _board(board),
      _next_player(nextPlayer),
      _prev_state(prevState),
      _last_move(lastMove) {
    if (_prev_state) {
        for (const auto &p : _prev_state->prev_state_hash)
            prev_state_hash.emplace_back(p);

        prev_state_hash.emplace_back(_prev_state->nextPlayer(),
                                     _prev_state->_board.getHash());
    }
}

}  // namespace gamestate
