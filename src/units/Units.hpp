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
    using Raw = std::vector<Unit>;
    using iterator = Raw::iterator;

    void spawn(Unit&& unit) {
        units.push_back(std::move(unit));
    }

    bool contains(uint32_t id);
    Unit& getById(uint32_t id);

    std::vector<iterator> unitsInRange(Cell position, int min, int max);

    void removeDead();

    auto begin() { return units.begin(); }
    auto end() { return units.end(); }

    auto begin() const { return units.begin(); }
    auto end() const { return units.end(); }
private:
    std::vector<Unit> units;
};

} // namespace sw
