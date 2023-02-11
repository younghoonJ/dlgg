//
// Created by younghoon on 23. 2. 11.
//

#ifndef DLGO_BASE_H
#define DLGO_BASE_H

#include "src/game/gamestate_fast.h"
#include "src/game/gamestate_slow.h"
#include "src/game/gotypes.h"

namespace agent {
class Agent {
public:
    virtual gotypes::Move selectMove(
        const gamestate::GameStateSlow &gameStateSlow) {
        throw std::runtime_error("not implmeneted!");
    };

    virtual gotypes::Move selectMove(
        const gamestate::GameStateFast &gameStateFast) {
        throw std::runtime_error("not implmeneted!");
    };
};
}  // namespace agent
#endif  //DLGO_BASE_H
