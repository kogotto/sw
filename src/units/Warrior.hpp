#pragma once

#include <cstdint>

#include "units/base.hpp"

namespace sw {
class Context;
} // namespace sw

namespace sw {

class Warrior : public Base {
public:
    Warrior(uint32_t id, uint32_t hp, uint32_t x, uint32_t y,
            uint32_t strength)
        : Base{id, hp, x, y}
        , _strength{strength}
    {}

    uint32_t _strength;
};

bool processUnit(Warrior& unit, Context& context);

} // namespace sw
