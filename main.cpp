#include <iostream>

#include "src/agent/naive.h"
#include "src/game/game_state.h"
#include "src/game/zobrist.h"
#include "src/utils/utils.h"

void run_naive(int board_size = 19) {
    auto bot_black = agent::RandomBot();
    auto bot_white = agent::RandomBot();

    auto game = gamestate::GameState::newGame(board_size);
    while (not game->isOver()) {
        //                usleep(1000 * 1000);
        std::cout << "\033[2J\033[1;1H";
        utils::printBoard(game->board);
        auto bot_move = (isBlack(game->next_player) ? bot_black : bot_white)
                            .selectMove(*game);
        //        utils::printMove(game->next_player, bot_move);
        game = game.release()->applyMove(bot_move);
    }
};

void run_naive_zob(int board_size = 19) {
    auto bot_black = agent::RandomBot();
    auto bot_white = agent::RandomBot();

    auto game = gamestate::GameStateZob::newGame(board_size);
    while (not game->isOver()) {
        //        usleep(1000 * 1000);
        std::cout << "\033[2J\033[1;1H";
        utils::printBoard(game->board);
        auto bot_move = (isBlack(game->next_player) ? bot_black : bot_white)
                            .selectMove(*game);
        //        utils::printMove(game->next_player, bot_move);
        game = game.release()->applyMove(bot_move);
    }
}

auto time_run(int num_iter, void (*run_game_fn)(int), int board_size) {
    auto begin = std::chrono::steady_clock::now();
    for (int i = 0; i < num_iter; ++i)
        run_game_fn(board_size);
    auto end = std::chrono::steady_clock::now();
    return std::chrono::duration_cast<std::chrono::microseconds>(end - begin)
        .count();
}

void player_game(int board_size) {
    auto game      = gamestate::GameStateZob::newGame(board_size);
    auto bot_white = agent::RandomBot();
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
                    if (game->board.isOccupied(point) or
                        not game->board.checkGridBound(point)) {
                        std::cout << "invalid move: " << human_input << '\n';
                    } else {
                        return gotypes::Move::play(point);
                    }
                }
            }
            return bot_white.selectMove(*game);
        }();
        utils::printMove(game->next_player, move);
        game = game.release()->applyMove(move);
    }
}

int main() {
    //    zobrist::writeAsVec(zobrist::makeHash());

    int board_size = 9;

    int testN       = 10;
    auto naive_time = time_run(testN, run_naive, board_size);
    auto zob_time   = time_run(testN, run_naive, board_size);
    std::cout << "Naive\t" << testN << " random run: " << naive_time << "[µs]"
              << std::endl;
    std::cout << "Zobrist\t" << testN << " random run: " << zob_time << "[µs]"
              << std::endl;

    //    player_game(board_size);

    return 0;
}
