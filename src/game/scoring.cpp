//
// Created by younghoon on 23. 2. 10.
//
#include "scoring.h"

namespace scoring {
Territory::Territory(const std::map<gotypes::Point, TerritoryMark>& state) {
    for (const auto& kv : state) {
        switch (kv.second) {
            case BLACK_STONE:
                ++num_black_stones;
                break;
            case WHITE_STONE:
                ++num_white_stones;
                break;
            case BLACK_TERRITORY:
                ++num_black_territory;
                break;
            case WHITE_TERRITORY:
                ++num_white_territory;
                break;
            case DAME:
                ++num_dame;
                dame_points.emplace_back(kv.first);
                break;
        }
    }
}

Territory
Territory::evaluate(const goboard::Board& board) {
    std::map<gotypes::Point, TerritoryMark> state;
    for (auto r = 1; r < board.num_rows + 1; ++r) {
        for (auto c = 1; c < board.num_cols + 1; ++c) {
            auto p = gotypes::Point{r, c};
            if (state.find(p) != state.end()) continue;

            auto stone = board.getColorOrNull(p);
            if (stone == gotypes::Player::black) {
                state.insert({p, BLACK_STONE});
            } else if (stone == gotypes::Player::white) {
                state.insert({p, WHITE_STONE});
            } else {

            }
        }
    }
}

}  // namespace scoring
