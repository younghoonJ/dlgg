//
// Created by younghoon on 23. 1. 28.
//

#ifndef DLGO_HELPERS_H
#define DLGO_HELPERS_H

#include "src/game/goboard.h"

namespace agent {
bool isPointAnEye(const goboard::Board& board, const gotypes::Point& point,
                  gotypes::Player color);

}  // namespace agent


#endif  //DLGO_HELPERS_H
