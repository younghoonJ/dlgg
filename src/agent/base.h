//
// Created by younghoon on 23. 1. 28.
//

#ifndef DLGO_BASE_H
#define DLGO_BASE_H
#include "src/game/game_state.h"
#include "src/game/gotypes.h"

namespace agent {
class Agent {
public:
    virtual gotypes::Move selectMove(const gamestate::GameState &gameState) = 0;
};


}  // namespace agent
#endif  //DLGO_BASE_H
