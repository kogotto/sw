#pragma once

#include <cstdint>

namespace sw {

class Warrior {
public:
    bool isDead() const {
        return _hp == 0;
    }
private:
    uint32_t _id;
    uint32_t _hp;
    uint32_t _strength;
};

} // namespace sw
