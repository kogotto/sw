#pragma once

#include <variant>
#include <vector>

#include "units/Warrior.hpp"
#include "units/Archer.hpp"

namespace sw {

using Unit = std::variant<Warrior, Archer>;

class Units {
public:
    void removeDead();
private:
    std::vector<Unit> units;
};

} // namespace sw
