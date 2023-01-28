#include <iostream>

#include "dlgo/src/goboard_slow.h"
#include "dlgo/src/gotypes.h"

int
main() {
    std::cout << "Hello, World!" << std::endl;
    auto a = gotypes::Player::white;


    gotypes::Point p{0, 1};
    auto ptr = std::make_unique<goboard::Gostring>(gotypes::Player::black,
                                                   std::set{p}, std::set{p});
    return 0;
}
