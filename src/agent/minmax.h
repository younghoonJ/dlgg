//
// Created by younghoon on 23. 2. 11.
//

#ifndef DLGO_MINMAX_H
#define DLGO_MINMAX_H
#include <chrono>
#include <iterator>
#include <random>

#include "src/agent/base.h"
#include "src/game/gamestate_fast.h"
#include "src/game/gotypes.h"

namespace agent {

class MinMaxAgent : public Agent {
    std::mt19937::result_type seed;
    std::mt19937 rng;
    enum class GameResult {
        LOSS = 1,
        DRAW = 2,
        WIN  = 3,
    };

public:
    MinMaxAgent() {
        seed = std::random_device()() ^
               std::chrono::duration_cast<std::chrono::seconds>(
                   std::chrono::system_clock::now().time_since_epoch())
                   .count() ^
               std::chrono::duration_cast<std::chrono::microseconds>(
                   std::chrono::high_resolution_clock::now().time_since_epoch())
                   .count();
        rng = std::mt19937(seed);
    }

    inline GameResult reverse_game_result(GameResult game_result) {
        if (game_result == GameResult::LOSS) return GameResult::WIN;
        if (game_result == GameResult::WIN) return GameResult::LOSS;
        return GameResult::DRAW;
    }

    inline GameResult bestResult(const gamestate::GameStateFast& gameState) {
        if (gameState.isOver()) {
            if (gameState.winner() == gameState.nextPlayer())
                return GameResult::WIN;
            else if (gameState.winner() == gotypes::Player::none)
                return GameResult::DRAW;
            else
                return GameResult::LOSS;
        }

        auto best_result_so_far = GameResult::LOSS;
        for (auto& move : gameState.legalMoves()) {
            auto next_state           = gameState.applyMove(move);
            auto opponent_best_result = bestResult(*next_state);
            auto our_result = reverse_game_result(opponent_best_result);
            if (int(our_result) > int(best_result_so_far)) {
                best_result_so_far = our_result;
            }
        }
        return best_result_so_far;
    }

    inline gotypes::Move selectMove(const gamestate::GameStateFast& gameState) {
        std::vector<gotypes::Move> winning_moves;
        std::vector<gotypes::Move> draw_moves;
        std::vector<gotypes::Move> losing_moves;
        for (const auto& move : gameState.legalMoves()) {
            auto next_state            = gameState.applyMove(move);
            auto opponent_best_outcome = bestResult(*next_state);
            auto our_best_outcome = reverse_game_result(opponent_best_outcome);

            if (our_best_outcome == GameResult::WIN)
                winning_moves.push_back(move);
            else if (our_best_outcome == GameResult::DRAW)
                draw_moves.push_back(move);
            else
                losing_moves.push_back(move);
        }

        if (not winning_moves.empty())
            return winning_moves[rng() % winning_moves.size()];
        if (not draw_moves.empty())
            return draw_moves[rng() % draw_moves.size()];
        return losing_moves[rng() % losing_moves.size()];
    }
};


}  // namespace agent
#endif  //DLGO_MINMAX_H
