//
// Created by younghoon on 23. 2. 10.
//

#ifndef DLGO_SCORING_H
#define DLGO_SCORING_H
#include <ostream>

#include "goboard.h"
#include "gotypes.h"

class GameState;

namespace scoring {
enum TerritoryMark {
    BLACK_STONE,
    WHITE_STONE,
    BLACK_TERRITORY,
    WHITE_TERRITORY,
    DAME,
};

struct Territory {
    int num_black_territory = 0;
    int num_white_territory = 0;
    int num_black_stones    = 0;
    int num_white_stones    = 0;
    int num_dame            = 0;
    std::vector<gotypes::Point> dame_points;

    explicit Territory(const std::map<gotypes::Point, TerritoryMark>& state);
    static Territory evaluate(const goboard::Board& board);
};

class GameResult {
public:
    double b;
    double w;
    double komi;

    GameResult(double b, double w, double komi) : b(b), w(w), komi(komi) {}

    inline gotypes::Player winner() const {
        if (b > w + komi) return gotypes::Player::black;
        return gotypes::Player::white;
    };

    inline double winning_margin() const { return std::abs(b - w - komi); };

    friend std::ostream& operator<<(std::ostream& os, const GameResult& gs) {
        auto val = gs.w + gs.komi;
        if (gs.b > val) os << "B+" << (gs.b - val);
        os << "W+" << (val - gs.b);
        return os;
    }

};


}  // namespace scoring
#endif  //DLGO_SCORING_H
