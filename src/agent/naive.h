//
// Created by younghoon on 23. 1. 28.
//

#ifndef DLGO_NAIVE_H
#define DLGO_NAIVE_H
#include <chrono>
#include <random>

#include "base.h"
#include "src/game/goboard.h"

namespace agent {
class RandomBot : public Agent {
    std::mt19937::result_type seed;
    std::mt19937 rng;

public:
    RandomBot() {
        seed = std::random_device()() ^
               std::chrono::duration_cast<std::chrono::seconds>(
                   std::chrono::system_clock::now().time_since_epoch())
                   .count() ^
               std::chrono::duration_cast<std::chrono::microseconds>(
                   std::chrono::high_resolution_clock::now().time_since_epoch())
                   .count();
        rng = std::mt19937(seed);
    }

    bool isPointAnEye(const goboard::Board &board, const gotypes::Point &point,
                      gotypes::Player color);

    gotypes::Move selectMove(const gamestate::GameState &gameState) override;
};

class RandomBotFast : public Agent {
    std::mt19937::result_type seed;
    std::mt19937 rng;
    goboard::pointTable_t *nbrTable;
    goboard::pointTable_t *cornerTable;

public:
    RandomBotFast(goboard::pointTable_t *nbrTable,
                  goboard::pointTable_t *cornerTable)
        : nbrTable(nbrTable), cornerTable(cornerTable) {
        seed = std::random_device()() ^
               std::chrono::duration_cast<std::chrono::seconds>(
                   std::chrono::system_clock::now().time_since_epoch())
                   .count() ^
               std::chrono::duration_cast<std::chrono::microseconds>(
                   std::chrono::high_resolution_clock::now().time_since_epoch())
                   .count();
        rng = std::mt19937(seed);
    }

    bool isPointAnEye(const goboard::Board &board, const gotypes::Point &point,
                      gotypes::Player color);

    gotypes::Move selectMove(const gamestate::GameState &gameState) override;
};

}  // namespace agent
#endif  //DLGO_NAIVE_H
