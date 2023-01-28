//
// Created by younghoon on 23. 1. 28.
//
#include "gostring.h"

#include <stdexcept>

namespace gostring {
std::unique_ptr<gostring::Gostring> gostring::Gostring::merged_with(
    const gostring::Gostring &other) {
    if (color != other.color)
        throw std::runtime_error("color != other.color");
    auto new_string = new gostring::Gostring(color);
    for (const auto &stones_ : {stones, other.stones})
        for (const auto &s : stones_)
            new_string->stones.emplace(s);
    for (const auto &lib : {liberties, other.liberties})
        for (const auto &stone : lib)
            if (not new_string->isInLiberties(stone))
                new_string->liberties.emplace(stone);
    for (const auto &stone : new_string->stones)
        if (new_string->isInLiberties(stone))
            new_string->liberties.erase(stone);
    return std::unique_ptr<gostring::Gostring>{new_string};
}
}  // namespace gostring
