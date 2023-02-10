//
// Created by younghoon on 23. 2. 10.
//
#include "scoring.h"

#include <queue>

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

auto _nbr_coord =
    std::vector<std::pair<int, int>>{{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

std::pair<std::vector<gotypes::Point>, std::vector<gotypes::Player>>
collect_region(const gotypes::Point& start_pos, const goboard::Board& board) {
    std::vector<std::vector<bool>> visited(
        board.num_rows, std::vector<bool>(board.num_cols, false));
    std::vector<gotypes::Point> all_pts;
    std::vector<gotypes::Player> all_borders;
    std::queue<gotypes::Point> q;

    q.push(start_pos);
    while (not q.empty()) {
        auto p = q.front();
        q.pop();
        visited[p.row][p.col] = true;
        auto now_color        = board.getColor(p);
        for (const auto& tp : _nbr_coord) {
            auto nbr = gotypes::Point{tp.first, tp.second};
            if (visited[nbr.row][nbr.col]) continue;
            auto nbr_color = board.getColorOrNull(nbr);
            if (nbr_color == now_color) {
                q.push(nbr);
                all_pts.push_back(nbr);
            } else {
                all_borders.push_back(nbr_color);
            }
        }
    }
    return {all_pts, all_borders};
}

Territory
Territory::evaluate(const goboard::Board& board) {
    std::map<gotypes::Point, TerritoryMark> status;
    for (auto r = 1; r < board.num_rows + 1; ++r) {
        for (auto c = 1; c < board.num_cols + 1; ++c) {
            auto p = gotypes::Point{r, c};
            if (status.find(p) != status.end()) continue;

            auto stone_color = board.getColorOrNull(p);
            if (stone_color == gotypes::Player::black) {
                status.insert({p, BLACK_STONE});
            } else if (stone_color == gotypes::Player::white) {
                status.insert({p, WHITE_STONE});
            } else {
                auto pair      = collect_region(p, board);  // group, nbrs
                auto fill_with = [&]() {
                    if (pair.second.size() == 1) {
                        auto nbr_stone_color = pair.second.at(0);
                        return nbr_stone_color == gotypes::Player::black
                                   ? TerritoryMark::BLACK_TERRITORY
                                   : TerritoryMark::WHITE_TERRITORY;
                    } else {
                        return TerritoryMark::DAME;
                    }
                }();
                for (auto& pos : pair.first)
                    status.insert({pos, fill_with});
            }
        }
    }
    return Territory{status};
}


}  // namespace scoring
