#pragma once

#include <cstdint>

#include "units/base.hpp"

namespace sw {
class Context;
} // namespace sw

namespace sw {

class Warrior : public Base {
public:
    Warrior(uint32_t id, int hp, int x, int y,
            int strength)
        : Base{id, hp, x, y}
        , _strength{strength}
    {}

    int _strength;
};

bool processUnit(Warrior& unit, Context& context);

} // namespace sw
