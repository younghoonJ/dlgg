//
// Created by younghoon on 23. 1. 28.
//

#ifndef DLGO_GAME_STATE_H
#define DLGO_GAME_STATE_H

#include <ios>
#include <memory>

#include "goboard.h"

namespace gamestate {
// abstract class GameState
class GameState {
public:
    virtual ~GameState()                             = default;
    virtual const goboard::Board &board() const      = 0;
    virtual const gotypes::Move &lastMove() const    = 0;
    virtual const gotypes::Player nextPlayer() const = 0;
    virtual const GameState &prevState() const       = 0;
    virtual std::unique_ptr<GameState> applyMove(const gotypes::Move &move) = 0;
    virtual bool doesMoveViolateKo(gotypes::Player player,
                                   const gotypes::Move &move) const         = 0;
    bool isOver() const;
    bool isMoveSelfCapture(gotypes::Player player,
                           const gotypes::Move &move) const;
    bool isValidMove(const gotypes::Move &move) const;
};

class GameStateSlow : public GameState {
    goboard::Board _board;
    gotypes::Player _next_player;
    std::unique_ptr<GameStateSlow> _prev_state;
    gotypes::Move _last_move;

public:
    GameStateSlow(const goboard::Board &board, gotypes::Player nextPlayer,
                  GameStateSlow *prevState, const gotypes::Move &lastMove);

    inline const goboard::Board &board() const override { return _board; };

    inline const gotypes::Move &lastMove() const override {
        return _last_move;
    };

    inline const gotypes::Player nextPlayer() const override {
        return _next_player;
    };

    const GameState &prevState() const override { return *_prev_state; }

    std::unique_ptr<GameState> applyMove(const gotypes::Move &move) override;

    bool doesMoveViolateKo(gotypes::Player player,
                           const gotypes::Move &move) const override;
};

class GameStateFast : public GameState {
    goboard::BoardZob _board;
    gotypes::Player _next_player;
    std::unique_ptr<GameStateFast> _prev_state;
    gotypes::Move _last_move;
    std::vector<std::pair<gotypes::Player, uint64_t>> prev_state_hash;

public:
    GameStateFast(const goboard::BoardZob &board, gotypes::Player nextPlayer,
                  GameStateFast *prevState, const gotypes::Move &lastMove);

    inline const goboard::Board &board() const override { return _board; }

    inline const gotypes::Move &lastMove() const override { return _last_move; }

    const gotypes::Player nextPlayer() const override { return _next_player; }

    const GameState &prevState() const override { return *_prev_state; }

    std::unique_ptr<GameState> applyMove(const gotypes::Move &move) override;

    bool doesMoveViolateKo(gotypes::Player player,
                           const gotypes::Move &move) const override;
};

template<class T>
std::unique_ptr<GameState> newGame(int board_size) {
    return std::make_unique<T>(goboard::Board(board_size, board_size),
                               gotypes::Player::black, nullptr,
                               gotypes::Move::None());
}
}  // namespace gamestate

#endif  //DLGO_GAME_STATE_H
