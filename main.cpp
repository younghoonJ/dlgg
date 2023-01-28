#include <iostream>

#include "src/agent/naive.h"
#include "src/game/game_state.h"
#include "src/game/print.h"
#include "src/game/zobrist.h"

void run_naive() {
    int board_size = 9;
    auto bot_black = agent::RandomBot();
    auto bot_white = agent::RandomBot();

    std::vector<std::unique_ptr<gamestate::GameState>> game_states;
    auto new_game = gamestate::GameState::newGame(board_size);
    game_states.emplace_back(new_game.release());
    gamestate::GameState* game = game_states.back().get();

    while (not game->isOver()) {
        //        usleep(1000 * 1000);
        std::cout << "\033[2J\033[1;1H";
        print::printBoard(game->board);
        auto bot_move = (isBlack(game->next_player) ? bot_black : bot_white)
                            .selectMove(*game);
        print::printMove(game->next_player, bot_move);
        game_states.push_back(game->applyMove(bot_move));
        game = game_states.back().get();
    }
};

void run_naive_zob() {
    int board_size = 9;
    auto bot_black = agent::RandomBot();
    auto bot_white = agent::RandomBot();

    std::vector<std::unique_ptr<gamestate::GameStateZob>> game_states;
    auto new_game = gamestate::GameStateZob::newGame(board_size);
    game_states.emplace_back(new_game.release());
    gamestate::GameStateZob* game = game_states.back().get();

    while (not game->isOver()) {
        //        usleep(1000 * 1000);
        std::cout << "\033[2J\033[1;1H";
        print::printBoard(game->board);
        auto bot_move = (isBlack(game->next_player) ? bot_black : bot_white)
                            .selectMove(*game);
        print::printMove(game->next_player, bot_move);
        game_states.push_back(game->applyMove(bot_move));
        game = game_states.back().get();
    }
}

int main() {
    std::chrono::steady_clock::time_point begin_n =
        std::chrono::steady_clock::now();
    for (int i = 0; i < 100; ++i) {
        run_naive();
    }
    std::chrono::steady_clock::time_point end_n =
        std::chrono::steady_clock::now();

    std::chrono::steady_clock::time_point begin_z =
        std::chrono::steady_clock::now();
    for (int i = 0; i < 100; ++i) {
        run_naive_zob();
    }
    std::chrono::steady_clock::time_point end_z =
        std::chrono::steady_clock::now();

    std::cout << "Naive\t 100 random run: "
              << std::chrono::duration_cast<std::chrono::microseconds>(end_n -
                                                                       begin_n)
                     .count()
              << "[µs]" << std::endl;

    std::cout << "Zobrist\t 100 random run: "
              << std::chrono::duration_cast<std::chrono::microseconds>(end_z -
                                                                       begin_z)
                     .count()
              << "[µs]" << std::endl;

    return 0;
}
