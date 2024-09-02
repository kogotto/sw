#include "units/Units.hpp"

#include <algorithm>
#include <cassert>

namespace sw {

namespace {

uint32_t getId(const Unit& unit) {
    return asBase(unit)._id;
}

bool isDead(const Unit& unit) {
    return std::visit(
        [] (const auto& unit) {
            return unit.isDead();
        },
        unit
    );
}

auto find(std::vector<Unit>& units, uint32_t id) {
    return std::find_if(
        units.begin(),
        units.end(),
        [id] (const Unit& unit) {
            return getId(unit) == id;
        }
    );
}

} // namespace

bool Units::contains(uint32_t id) {
    auto it = find(units, id);
    return it != units.end();
}

Unit& Units::getById(uint32_t id) {
    auto it = find(units, id);
    assert(it != units.end());
    return *it;
}

auto Units::unitsInRange(Cell position, int min, int max) -> std::vector<iterator> {
    std::vector<iterator> result;

    for (auto it = units.begin(); it != units.end(); ++it) {
        auto& current = asBase(*it);
        const int dist = distance(current.getPosition(), position);
        if (min <= dist && dist < max) {
            result.push_back(it);
        }
    }

    return result;
}

void Units::removeDead() {
    units.erase(
        std::remove_if(
            units.begin(),
            units.end(),
            isDead
        ),
        units.end()
    );
}

} // namespace sw
