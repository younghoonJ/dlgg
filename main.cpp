#include <iostream>

#include "src/agent/naive.h"
#include "src/game/game_state.h"
#include "src/game/zobrist.h"
#include "src/utils/utils.h"

void run_naive(int board_size = 19) {
    auto bot_black = agent::RandomBot();
    auto bot_white = agent::RandomBot();

    std::vector<std::unique_ptr<gamestate::GameState>> game_states;
    auto new_game = gamestate::GameState::newGame(board_size);
    game_states.emplace_back(new_game.release());
    gamestate::GameState* game = game_states.back().get();

    while (not game->isOver()) {
        //                usleep(1000 * 1000);
        std::cout << "\033[2J\033[1;1H";
        utils::printBoard(game->board);
        auto bot_move = (isBlack(game->next_player) ? bot_black : bot_white)
                            .selectMove(*game);
        //        utils::printMove(game->next_player, bot_move);
        game_states.push_back(game->applyMove(bot_move));
        game = game_states.back().get();
    }
};

void run_naive_zob(int board_size = 19) {
    auto bot_black = agent::RandomBot();
    auto bot_white = agent::RandomBot();

    std::vector<std::unique_ptr<gamestate::GameStateZob>> game_states;
    auto new_game = gamestate::GameStateZob::newGame(board_size);
    game_states.emplace_back(new_game.release());
    gamestate::GameStateZob* game = game_states.back().get();

    while (not game->isOver()) {
        //        usleep(1000 * 1000);
        std::cout << "\033[2J\033[1;1H";
        utils::printBoard(game->board);
        auto bot_move = (isBlack(game->next_player) ? bot_black : bot_white)
                            .selectMove(*game);
        utils::printMove(game->next_player, bot_move);
        game_states.push_back(game->applyMove(bot_move));
        game = game_states.back().get();
    }
}

auto time_run(int num_iter, void (*run_game_fn)(int), int board_size) {
    std::chrono::steady_clock::time_point begin =
        std::chrono::steady_clock::now();
    for (int i = 0; i < num_iter; ++i) {
        run_game_fn(board_size);
    }
    std::chrono::steady_clock::time_point end =
        std::chrono::steady_clock::now();
    return std::chrono::duration_cast<std::chrono::microseconds>(end - begin)
        .count();
}

void player_game(int board_size) {
    std::vector<std::unique_ptr<gamestate::GameStateZob>> game_states;
    auto new_game  = gamestate::GameStateZob::newGame(board_size);
    auto bot_white = agent::RandomBot();
    game_states.emplace_back(new_game.release());
    gamestate::GameStateZob* game = game_states.back().get();
    while (not game->isOver()) {
        std::cout << "\033[2J\033[1;1H";
        utils::printBoard(game->board);
        auto move = [&]() {
            if (isBlack(game->next_player)) {
                while (true) {
                    std::cout << "Enter Position (ex: B3):";
                    std::string human_input;
                    std::cin >> human_input;
                    auto point = utils::pointFromCoords(human_input);
                    if (game->board.isOccupied(point)) {
                        std::cout << "there is a stone at: " << human_input
                                  << '\n';
                    } else if (not game->board.checkGridBound(point)) {
                        std::cout << "invalid value: " << human_input << '\n';
                    } else {
                        return gotypes::Move::play(point);
                    }
                }
            }
            return bot_white.selectMove(*game);
        }();
        utils::printMove(game->next_player, move);
        game_states.push_back(game->applyMove(move));
        game = game_states.back().get();
    }
}

int main() {
    //    zobrist::writeAsVec(zobrist::makeHash());

    int board_size = 9;

    //    int testN      = 100;
    //    auto naive_time = time_run(testN, run_naive, board_size);
    //    auto zob_time   = time_run(testN, run_naive, board_size);
    //
    //    std::cout << "Naive\t" << testN << " random run: " << naive_time << "[µs]"
    //              << std::endl;
    //
    //    std::cout << "Zobrist\t" << testN << " random run: " << zob_time << "[µs]"
    //              << std::endl;

    player_game(board_size);

    return 0;
}
