//
// Created by younghoon on 23. 1. 28.
//

#ifndef DLGO_GAME_STATE_H
#define DLGO_GAME_STATE_H

#include "goboard.h"

namespace gamestate {


struct GameState {
    goboard::Board board;
    gotypes::Player next_player;
    std::unique_ptr<GameState> prev_state;
    gotypes::Move last_move;

    GameState(const goboard::Board &board, gotypes::Player nextPlayer,
              GameState *prevState, const gotypes::Move &lastMove)
        : board(board),
          next_player(nextPlayer),
          prev_state(prevState),
          last_move(lastMove) {}

    std::unique_ptr<GameState> applyMove(const gotypes::Move &move);

    static std::unique_ptr<GameState> newGame(int board_size);

    bool isOver() const;

    bool isMoveSelfCapture(gotypes::Player player,
                           const gotypes::Move &move) const;

    bool doesMoveViolateKo(gotypes::Player player,
                           const gotypes::Move &move) const;

    bool isValidMove(const gotypes::Move &move) const;
};

struct GameStateZob {
    goboard::BoardZob board;
    gotypes::Player next_player;
    std::unique_ptr<GameStateZob> prev_state;
    std::vector<std::pair<gotypes::Player, uint64_t>> prev_state_hash;
    gotypes::Move last_move;

    GameStateZob(goboard::BoardZob board, gotypes::Player nextPlayer,
                 GameStateZob *prevState, const gotypes::Move &lastMove);

    std::unique_ptr<GameStateZob> applyMove(const gotypes::Move &move);

    static std::unique_ptr<GameStateZob> newGame(int board_size);

    bool isOver() const;

    bool isMoveSelfCapture(gotypes::Player player,
                           const gotypes::Move &move) const;

    bool doesMoveViolateKo(gotypes::Player player,
                           const gotypes::Move &move) const;

    bool isValidMove(const gotypes::Move &move) const;
};

}  // namespace gamestate

#endif  //DLGO_GAME_STATE_H
