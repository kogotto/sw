#pragma once

#include <variant>
#include <vector>

#include "units/Warrior.hpp"
#include "units/Archer.hpp"

namespace sw {

using Unit = std::variant<Warrior, Archer>;

inline Base& asBase(Unit& unit) {
    return std::visit(
        [] (auto&& unit) -> Base& {
            return unit;
        },
        unit
    );
}

inline const Base& asBase(const Unit& unit) {
    return std::visit(
        [] (auto&& unit) -> const Base& {
            return unit;
        },
        unit
    );
}

class Units {
public:
    void spawn(Unit&& unit) {
        units.push_back(std::move(unit));
    }
    Unit& getById(uint32_t id);
    void removeDead();
private:
    std::vector<Unit> units;
};

} // namespace sw
