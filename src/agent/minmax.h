//
// Created by younghoon on 23. 2. 10.
//

#ifndef DLGO_MINMAX_H
#define DLGO_MINMAX_H
#include "src/agent/base.h"

namespace agent {
class MinMaxBot : public Agent {

    goboard::pointTable_t *nbrTable;
    goboard::pointTable_t *cornalTable;
    int max_depth;

public:
    MinMaxBot(goboard::pointTable_t *nbrTable,
              goboard::pointTable_t *cornalTable, int max_depth = 2)
        : nbrTable(nbrTable), cornalTable(cornalTable), max_depth(max_depth) {}

    bool isPointAnEye(const goboard::Board &board, const gotypes::Point &point,
                      gotypes::Player color);
    gotypes::Move selectMove(const gamestate::GameState &gameState) override;

    int captureDiff(const gamestate::GameState &gameState);
};

}  // namespace agent
#endif  //DLGO_MINMAX_H
