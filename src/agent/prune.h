//
// Created by younghoon on 23. 2. 10.
//

#ifndef DLGO_PRUNE_H
#define DLGO_PRUNE_H
#include "src/agent/base.h"

namespace agent {
class MinMaxBot : public Agent {
    int MAX_SCORE = 999999;
    int MIN_SCORE = -999999;
    goboard::pointTable_t *nbrTable;
    goboard::pointTable_t *cornalTable;
    int max_depth;

public:
    MinMaxBot(goboard::pointTable_t *nbrTable,
              goboard::pointTable_t *cornalTable, int max_depth = 2)
        : nbrTable(nbrTable), cornalTable(cornalTable), max_depth(max_depth) {}

    bool isPointAnEye(const goboard::Board &board, const gotypes::Point &point,
                      gotypes::Player color);
    gotypes::Move selectMove(
        const gamestate::GameStateFast &gameState) override;

    int captureDiff(const gamestate::GameStateFast &gameState);

    int bestResult(const gamestate::GameStateFast &gameState, int max_depth);
};

}  // namespace agent
#endif  //DLGO_PRUNE_H
