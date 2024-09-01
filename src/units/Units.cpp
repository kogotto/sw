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

} // namespace

Unit& Units::getById(uint32_t id) {
    const auto it = std::find_if(
        units.begin(),
        units.end(),
        [id] (const Unit& unit) {
            return getId(unit) == id;
        }
    );

    assert(it != units.end());

    return *it;
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
