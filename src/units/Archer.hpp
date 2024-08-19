#pragma once

#include <cstdint>

namespace sw {

class Archer {
public:
    bool isDead() const {
        return _hp == 0;
    }
private:
    uint32_t _id;
    uint32_t _hp;
    uint32_t _strength;
    uint32_t _range;
    uint32_t _agility;
};

} // namespace sw
