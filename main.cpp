#include <unistd.h>

#include <iostream>

#include "src/agent/naive.h"
#include "src/game/game_state.h"
#include "src/game/print.h"

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
        //                std::cout << "\x1B[2J\x1B[H";
        std::cout << "\033[2J\033[1;1H";
        //        system("clear");

        print::printBoard(game->board);
        auto bot_move = (isBlack(game->next_player) ? bot_black : bot_white)
                            .selectMove(*game);
        print::printMove(game->next_player, bot_move);
        game_states.push_back(game->applyMove(bot_move));
        game = game_states.back().get();
    }
};

int main() {
    run_naive();

    return 0;
}
