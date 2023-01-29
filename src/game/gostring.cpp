//
// Created by younghoon on 23. 1. 28.
//
#include "gostring.h"

#include <stdexcept>

namespace gostring {
std::unique_ptr<gostring::Gostring> gostring::Gostring::merge_with(
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

void Gostring::merge_inplace(const Gostring &other) {
    if (color != other.color)
        throw std::runtime_error("color != other.color");
    for (const auto &stone : other.stones)
        stones.insert(stone);
    for (const auto &stone : other.liberties)
        liberties.insert(stone);
    for (const auto &stone : stones)
        if (isInLiberties(stone))
            liberties.erase(stone);
}
}  // namespace gostring
