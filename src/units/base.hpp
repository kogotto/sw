#pragma once

#include <cstdint>

namespace sw {

class Base {
public:
    Base(uint32_t id, uint32_t hp, uint32_t x, uint32_t y)
        : _id{id}
        , _hp{hp}
        , _x{x}
        , _y{y}
    {}

    bool isDead() const {
        return _hp == 0;
    }

    uint32_t _id;
    uint32_t _hp;

    uint32_t _x;
    uint32_t _y;
};

} // namespace sw
