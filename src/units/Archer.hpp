#pragma once

#include <cstdint>

#include "units/base.hpp"

namespace sw {

class Archer : public Base {
public:
    Archer(uint32_t id, uint32_t hp, uint32_t x, uint32_t y,
            uint32_t strength, uint32_t range, uint32_t agility)
        : Base{id, hp, x, y}
        , _strength{strength}
        , _range{range}
        , _agility{agility}
    {}

    uint32_t _strength;
    uint32_t _range;
    uint32_t _agility;
};

} // namespace sw
