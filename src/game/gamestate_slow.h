//
// Created by younghoon on 23. 2. 11.
//

#ifndef DLGO_GAMESTATE_SLOW_H
#define DLGO_GAMESTATE_SLOW_H
#include "goboard.h"
#include "scoring.h"

namespace gamestate {
class GameStateSlow {
    goboard::Board _board;
    gotypes::Player _next_player;
    std::unique_ptr<GameStateSlow> _prev_state;
    gotypes::Move _last_move;

public:
    GameStateSlow(const goboard::Board &board, gotypes::Player nextPlayer,
                  GameStateSlow *prevState, const gotypes::Move &lastMove)
        : _board(board),
          _next_player(nextPlayer),
          _prev_state(prevState),
          _last_move(lastMove) {}

    inline bool isOver() const {
        if (lastMove().isNone()) return false;
        if (lastMove().isResign()) return true;
        if (prevState().lastMove().isNone()) return false;
        return lastMove().isPass() and prevState().lastMove().isPass();
    };

    inline virtual bool isMoveSelfCapture(gotypes::Player player,
                                          const gotypes::Move &move) const {
        if (not move.isPlay()) return false;
        auto next_board = board();
        next_board.placeStone(player, move.point);
        auto new_string = next_board.getGoString(move.point);
        return new_string.num_liberties() == 0;
    }

    inline bool isValidMove(const gotypes::Move &move) const {
        if (isOver()) return false;
        if (move.isPass() or move.isResign()) return true;
        return (not board().isOccupied(move.point)) and
               (not isMoveSelfCapture(nextPlayer(), move)) and
               (not doesMoveViolateKo(nextPlayer(), move));
    }

    inline gotypes::Player winner() const {
        if (not isOver()) return gotypes::Player::none;
        if (lastMove().isResign()) return nextPlayer();
        auto game_result = computeResult();
        return game_result.winner();
    }

    inline scoring::GameResult computeResult() const {
        auto territory = scoring::Territory::evaluate(board());
        return {
            double(territory.num_black_territory + territory.num_black_stones),
            double(territory.num_white_territory + territory.num_white_stones),
            7.5};
    }

    inline virtual std::vector<gotypes::Move> legalMoves() const {
        std::vector<gotypes::Move> moves;
        for (int row = 1; row < board().num_rows + 1; ++row) {
            for (int col = 1; col < board().num_cols + 1; ++col) {
                auto move = gotypes::Move::play({row, col});
                if (isValidMove(move)) {
                    moves.push_back(move);
                }
            }
        }
        moves.push_back(gotypes::Move::pass_turn());
        moves.push_back(gotypes::Move::resign());
        return moves;
    }

    inline const goboard::Board &board() const { return _board; };

    inline const gotypes::Move &lastMove() const { return _last_move; };

    inline const gotypes::Player nextPlayer() const { return _next_player; };

    inline const GameStateSlow &prevState() const { return *_prev_state; }

    inline std::unique_ptr<GameStateSlow> applyMove(const gotypes::Move &move) const {
        auto next_board = _board;
        if (move.isPlay()) next_board.placeStone(_next_player, move.point);
        return std::make_unique<GameStateSlow>(
            next_board, other(_next_player), const_cast<GameStateSlow *>(this),
            move);
    }

    inline bool doesMoveViolateKo(gotypes::Player player,
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

    static inline std::unique_ptr<GameStateSlow> newGame(int board_size) {
        return std::make_unique<GameStateSlow>(
            goboard::Board(board_size, board_size), gotypes::Player::black,
            nullptr, gotypes::Move::None());
    }
};


}  // namespace gamestate
#endif  //DLGO_GAMESTATE_SLOW_H
